#include "Quote_generator.h"
#include "read_local_quote.h"

Quote_Generator::Quote_Generator(const char* ini_file);
{   
    // 初始化时间为上午九点
    this->time_now = date * 1000000 * 1000 + 90000 * 1000
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
    const char* filename;
    int total_num;
    if (is_am)
    {   //上午需载入股票信息数据
        this->stockinfo_array = read_stock_info_from_file(ini_file this->date, &total_num);
        this->stockinfo_ptr = this->fist_stockinfo_ptr;
    }
    else // 释放上午的数据
        this->release_data(); 
    // 载入数据
    this->snapdata_array = read_snap_data_from_file(filename, &total_num);
    this->leveldata_array = read_level_data_from_file(filename, &total_num);
    this->tickorder_array = read_tick_order_from_file(filename, &total_num);
    this->ticktrade_array = read_tick_trade_from_file(filename, &total_num);
    // 记录需推送的下一条数据的位置
    this->snapdata_ptr = this->snapdata_array;
    this->leveldata_ptr = this->leveldata_array;
    this->tickorder_ptr = this->tickorder_array;
    this->ticktrade_ptr = this->ticktrade_array;
}

void Quote_Generator::check_is_need_load_data()
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

StockInfo* Quote_Generator::Get_StockInfo()
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

TickOrder* Quote_Generator::Get_TickOrder()
{   
    if (this->tickorder_ptr->data_time > 0)
    {   // 若当前数据时间小于等于系统时间 就返回当前指针 再++当前指针 
        if (this->tickorder_ptr != NULL && 
            this->tickorder_ptr->data_time <= this->time_now;
            )
        {
            return this->tickorder_ptr++;
        }
        else 
        {   
            // 若当前数据时间小于当前系统下一次推送行情时间 更新 系统下一次推送行情时间 为 当前数据时间
            if ( this->time_next > this->tickorder_ptr->data_time )
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

TickTrade* Get_TickTrade()
{
    if (this->ticktrade_ptr->data_time > 0)
    {   // 若当前数据时间小于等于系统时间 就返回当前指针 再++当前指针 
        if (this->ticktrade_ptr != NULL && 
            this->ticktrade_ptr->data_time <= this->time_now;
            )
        {
            return this->ticktrade_ptr++;
        }
        else 
        {   
            // 若当前数据时间小于当前系统下一次推送行情时间 更新 系统下一次推送行情时间 为 当前数据时间
            if ( this->time_next > this->ticktrade_ptr->data_time )
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

SnapData* Quote_Generator::Get_SnapData();
{
    if (this->snapdata_ptr->data_time > 0)
    {   // 若当前数据时间小于等于系统时间 就返回当前指针 再++当前指针 
        if (this->snapdata_ptr != NULL && 
            this->snapdata_ptr->data_time <= this->time_now;
            )
        {
            return this->snapdata_ptr++;
        }
        else 
        {   
            // 若当前数据时间小于当前系统下一次推送行情时间 更新 系统下一次推送行情时间 为 当前数据时间
            if ( this->time_next > this->snapdata_ptr->data_time )
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

LevelData* Quote_Generator::Get_LevelData();
{
    if (this->leveldata_ptr->data_time > 0)
    {   // 若当前数据时间小于等于系统时间 就返回当前指针 再++当前指针 
        if (this->leveldata_ptr != NULL && 
            this->leveldata_ptr->data_time <= this->time_now;
            )
        {
            return this->leveldata_ptr++;
        }
        else 
        {   
            // 若当前数据时间小于当前系统下一次推送行情时间 更新 系统下一次推送行情时间 为 当前数据时间
            if ( this->time_next > this->leveldata_ptr->data_time )
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

