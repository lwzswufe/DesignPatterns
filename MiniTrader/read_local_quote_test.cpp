#include <stdio.h>
#include "read_local_quote.h"

/*
g++ local_quote_struct.h read_local_quote.h read_local_quote.cpp read_local_quote_test.cpp -o read_local_quote_test.exe
*/

// 测试读取股票信息
void test_stock_info(const char * filename)
{
    int date = 20190110, stock_num = 0, total_num;
    SimStockInfo* ptr = read_stock_info_from_file(filename, date, &total_num);
    while (ptr->pre_close_price > 0)
    {
        printf("code:%s Market:%d preclose:%.2lf\tuplimit:%.2lf\n", 
        ptr->code, ptr->exchange_id,  ptr->pre_close_price, ptr->upper_limit_price);
        ptr++;
        stock_num++;
    }
    printf("date:%d stock_num:%d\n", date, stock_num);
}

// 测试读取快照数据
void test_snap_data(const char* filename)
{
    int data_num = 0, snapdata_flag = 1, total_num;
    SimSnapData* ptr = read_snap_data_from_file(filename, &total_num);
    while (ptr->data_time > 0)
    {   
        data_num++;
        if (data_num >= snapdata_flag)
        {
            snapdata_flag *= 2;
            printf("code:%s Market:%d preclose:%.2lf\tuplimit:%.2lf\n", 
            ptr->code, ptr->exchange_id,  ptr->pre_close_price, ptr->upper_limit_price);
        }
        ptr++;
    }
    printf("data_num:%d\n", data_num);
}

// 测试读取10档行情数据
void test_level_data(const char * filename)
{
    int data_num = 0, leveldata_flag = 1, total_num;
    SimLevelData* ptr = read_level_data_from_file(filename, &total_num);
    while (ptr->data_time > 0)
    {   
        data_num++;
        if (data_num >= leveldata_flag)
        {
            leveldata_flag *= 2;
            printf("line:%08d code:%s Market:%d b1pr:%.2lf b1vol_queue:%d\n", data_num, 
            ptr->code, ptr->exchange_id,  ptr->bid[0], ptr->bid1_count);
            for (int i=0; i<50; i++)
            {   
                if (ptr->bid1_qty[i] > 0)
                    printf("%ld|", ptr->bid1_qty[i]);
            }
            printf("\n");
        }
        ptr++;
    }
    printf("data_num:%d\n", data_num);
}

void test_tick_order(const char * filename)
{
    int data_num = 0, flag = 1, total_num;
    SimTickOrder* ptr = read_tick_order_from_file(filename, &total_num);
    while (ptr->data_time > 0)
    {   
        data_num++;
        if (data_num >= flag)
        {
            flag *= 2;
            printf("line:%08d code:%s Market:%d seq:%09ld price:%.2lf vol:%ld side:%c type:%c\n", data_num, 
            ptr->code, ptr->exchange_id, ptr->seq, ptr->price, ptr->qty, ptr->side, ptr->type);
        }
        ptr++;
    }
    printf("tickorder_num:%d\n", data_num);
}

void test_tick_trade(const char * filename)
{
    int data_num = 0, flag = 1, total_num;
    SimTickTrade* ptr = read_tick_trade_from_file(filename, &total_num);
    while (ptr->data_time > 0)
    {   
        data_num++;
        if (data_num >= flag)
        {
            flag *= 2;
            printf("line:%08d code:%s Market:%d seq:%09ld seq_buy:%09ld seq_sell:%09ld price:%.2lf vol:%ld type:%c\n", data_num, 
            ptr->code, ptr->exchange_id, ptr->seq, ptr->bid_no, ptr->ask_no, ptr->price, ptr->qty, ptr->type);
        }
        ptr++;
    }
    printf("tickorder_num:%d\n", data_num);
}

int main()
{   
    test_stock_info("/home/wiz/data/uplimit_list.csv");
    test_snap_data("/home/wiz/data/L2_data/2017/01/03/am_hq_snap_spot.csv");
    test_level_data("/home/wiz/data/L2_data/2017/01/03/am_snap_level_spot.csv");
    test_tick_order("/home/wiz/data/L2_data/2017/01/03/am_hq_order_spot.csv");
    test_tick_trade("/home/wiz/data/L2_data/2017/01/03/am_hq_trade_spot.csv");
    return 0;
}