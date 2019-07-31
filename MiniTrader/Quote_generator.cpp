#include <stdio.h>
#include <string.h>
#include "Quote_generator.h"
#include "read_local_quote.h"
#ifndef _READ_INI_H_
#define _READ_INI_H_
#include "Read_ini.h"
#endif
#include <iostream>
using std::cout; using std::endl;

Quote_Generator::Quote_Generator(const char* ini_file)
{   
    IniType conf = Read_ini(ini_file);
    show_ini(conf);
    const char *dir_ = conf["main"]["backtest_dir_linux"].c_str();
    this->dirname = new char[256];
    strncpy(this->dirname, dir_, strlen(dir_));
    printf("data dir:%s\n", this->dirname);
    this->datenum = GetIntFromKey(conf["main"], "backtest_date");
    if (this->datenum > 20300101 || this->datenum < 20000101)
    {
        printf("error date:%d", this->datenum);
        throw runtime_error("error date");
    }
    else
        printf("backtest date :%d\n", this->datenum);
    // 初始化时间为上午九点
    // this->time_now = this->datenum * 1000000 * 1000 + 90000 * 1000;
    this->time_now = 90000 * 1000;
    printf("init time:%ld\n", this->time_now);
    // 数组指针初始化
    this->stockinfo_array = NULL;
    this->snapdata_array = NULL;
    this->leveldata_array = NULL;
    this->tickorder_array = NULL;
    this->ticktrade_array = NULL;
    // 当前指针初始化
    this->stockinfo_ptr = NULL;
    this->snapdata_ptr = NULL;
    this->leveldata_ptr = NULL;
    this->tickorder_ptr = NULL;
    this->ticktrade_ptr = NULL;
}

void Quote_Generator::load_data(bool is_am)
{   
    /*
    股票信息列表： "/home/wiz/data/uplimit_list.csv"
    快照行情:     "/home/wiz/data/L2_data/2017/01/03/am_hq_snap_spot.csv"
    十档行情:     "/home/wiz/data/L2_data/2017/01/03/am_snap_level_spot.csv"
    订单流:       "/home/wiz/data/L2_data/2017/01/03/am_hq_order_spot.csv"
    成交流:       "/home/wiz/data/L2_data/2017/01/03/am_hq_trade_spot.csv"
    */
    char am[3] = "am", pm[3] = "pm", *timetype, dir_stockinfo, dir_data;
    char filename[256];
    int total_num;
    if (is_am)
    {   //上午需载入股票信息数据
        timetype = am;
        sprintf(filename,"%s/uplimit_list.csv", this->dirname);
        this->stockinfo_array = read_stock_info_from_file(filename, this->datenum, &total_num);
        this->stockinfo_ptr = this->stockinfo_array;
    }
    else 
    {
        timetype = pm;
    }
    printf("load %d %s data....\n", this->datenum, timetype);
    int year = this->datenum / 10000;
    int month = this->datenum % 10000 / 100;
    int date = this->datenum % 100;

    // 载入数据
    sprintf(filename,"%s/L2_data/%04d/%02d/%02d/%s_hq_snap_spot.csv", this->dirname, year, month, date, timetype);
    this->snapdata_array = read_snap_data_from_file(filename, &total_num);
    sprintf(filename,"%s/L2_data/%04d/%02d/%02d/%s_snap_level_spot.csv", this->dirname, year, month, date, timetype);
    this->leveldata_array = read_level_data_from_file(filename, &total_num);
    sprintf(filename,"%s/L2_data/%04d/%02d/%02d/%s_hq_order_spot.csv", this->dirname, year, month, date, timetype);
    this->tickorder_array = read_tick_order_from_file(filename, &total_num);
    sprintf(filename,"%s/L2_data/%04d/%02d/%02d/%s_hq_trade_spot.csv", this->dirname, year, month, date, timetype);
    this->ticktrade_array = read_tick_trade_from_file(filename, &total_num);
    // 记录需推送的下一条数据的位置
    this->snapdata_ptr = this->snapdata_array;
    this->leveldata_ptr = this->leveldata_array;
    this->tickorder_ptr = this->tickorder_array;
    this->ticktrade_ptr = this->ticktrade_array;
    printf("load %d %s data over\n", this->datenum, timetype);
}

bool Quote_Generator::check_is_need_load_data()
{
    if (this->tickorder_array == NULL &&
        this->ticktrade_array == NULL &&
        this->snapdata_array == NULL &&
        this->leveldata_array == NULL)
        return true;
    else
        return false;
}

int64_t Quote_Generator::Get_Time()
{
    return this->time_now;
}

double Quote_Generator::Get_Clock()
{
    double now = (this->time_now % 1000000000) / 1000.0;
    return now;
}

const StockInfo* Quote_Generator::Get_StockInfo()
{
    if (this->stockinfo_ptr != NULL && this->stockinfo_ptr->pre_close_price > 0)
    {
        return this->stockinfo_ptr++;
    }
    else 
    {   
        if (this->stockinfo_array != NULL)
            delete[] this->stockinfo_array;
        this->stockinfo_array = NULL;
        this->stockinfo_ptr = NULL;
        printf("release StockInfo Array\n");
        return NULL;
    }
}

const TickOrder* Quote_Generator::Get_TickOrder()
{   
    if (this->tickorder_ptr == NULL)
        return NULL;
    if (this->tickorder_ptr->data_time > 0)
    {   // 若当前数据时间小于等于系统时间 就返回当前指针 再++当前指针 

        if (this->tickorder_ptr->data_time <= this->time_now)
        {
            return this->tickorder_ptr++;
        }
        else 
        {   
            // 若下一个数据时间小于当前系统下一次推送行情时间 更新 系统下一次推送行情时间 为 下一个数据时间
            if ( this->time_next > this->tickorder_ptr->data_time )
                this->time_next = this->tickorder_ptr->data_time;
            // 系统下一次推送行情时间小于下一个数据的时间 设置 系统下一次推送行情时间 为 下一个数据的时间
            else if (this->time_next <= this->time_now)
                this->time_next = this->tickorder_ptr->data_time;
            return NULL;
        }
    }
    else
    {   // 当前数据指针移动到数组末尾 释放内存
        if (this->tickorder_array != NULL)
            delete[] this->tickorder_array;
        this->tickorder_array = NULL;
        this->tickorder_ptr = NULL;
        printf("release TickOrder Array\n");
        return NULL;
    }
}

const TickTrade* Quote_Generator::Get_TickTrade()
{   
    if (this->ticktrade_ptr == NULL)
        return NULL;
    if (this->ticktrade_ptr->data_time > 0)
    {   // 若当前数据时间小于等于系统时间 就返回当前指针 再++当前指针 

        if (this->ticktrade_ptr->data_time <= this->time_now)
        {
            return this->ticktrade_ptr++;
        }
        else 
        {   
            // 若当前数据时间小于当前系统下一次推送行情时间 更新 系统下一次推送行情时间 为 当前数据时间
            if ( this->time_next > this->ticktrade_ptr->data_time )
                this->time_next = this->ticktrade_ptr->data_time;
            // 系统下一次推送行情时间小于下一个数据的时间 设置 系统下一次推送行情时间 为 下一个数据的时间
            else if (this->time_next <= this->time_now)
                this->time_next = this->ticktrade_ptr->data_time;
            return NULL;
        }
    }
    else
    {   // 当前数据指针移动到数组末尾 释放内存
        if (this->ticktrade_array != NULL)
            delete[] this->ticktrade_array;
        this->ticktrade_array = NULL;
        this->ticktrade_ptr = NULL;
        printf("release TickTrade Array\n");
    }
}

const SnapData* Quote_Generator::Get_SnapData()
{   
    if (this->snapdata_ptr == NULL)
    {
        return NULL;
    }
    if (this->snapdata_ptr->data_time > 0)
    {   // 若当前数据时间小于等于系统时间 就返回当前指针 再++当前指针 
        if (this->snapdata_ptr->data_time <= this->time_now)
        {
            return this->snapdata_ptr++;
        }
        else 
        {   
            // 若当前数据时间小于当前系统下一次推送行情时间 更新 系统下一次推送行情时间 为 当前数据时间
            if ( this->time_next > this->snapdata_ptr->data_time )
                this->time_next = this->snapdata_ptr->data_time;
            // 系统下一次推送行情时间小于下一个数据的时间 设置 系统下一次推送行情时间 为 下一个数据的时间
            else if (this->time_next <= this->time_now)
                this->time_next = this->snapdata_ptr->data_time;
            return NULL;
        }
    }
    else
    {   // 当前数据指针移动到数组末尾 释放内存
        if (this->snapdata_array != NULL)
            delete[] this->snapdata_array;
        this->snapdata_array = NULL;
        this->snapdata_ptr = NULL;
        printf("release SnapData Array\n");
    }
}

const LevelData* Quote_Generator::Get_LevelData()
{   
    if (this->leveldata_ptr == NULL)
    {
        return NULL;
    }
    if (this->leveldata_ptr->data_time > 0)
    {   // 若当前数据时间小于等于系统时间 就返回当前指针 再++当前指针 
        if (this->leveldata_ptr->data_time <= this->time_now)
        {
            return this->leveldata_ptr++;
        }
        else 
        {   
            // 若当前数据时间小于当前系统下一次推送行情时间 更新 系统下一次推送行情时间 为 当前数据时间
            if ( this->time_next > this->leveldata_ptr->data_time )
                this->time_next = this->leveldata_ptr->data_time;
            // 系统下一次推送行情时间小于下一个数据的时间 设置 系统下一次推送行情时间 为 下一个数据的时间
            else if (this->time_next <= this->time_now)
                this->time_next = this->leveldata_ptr->data_time;
            return NULL;
        }
    }
    else
    {   // 当前数据指针移动到数组末尾 释放内存
        if (this->leveldata_array != NULL)
            delete[] this->leveldata_array;
        this->leveldata_array = NULL;
        this->leveldata_ptr = NULL;
        printf("release LevelData Array\n");
    }
}

void Quote_Generator::update_time()
{
    // printf("update time  %lld  to  %lld\n", this->time_now, this->time_next);
    this->time_now = this->time_next;
}