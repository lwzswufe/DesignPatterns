#include <stdio.h>
#include "read_local_quote.h"

/*
g++ sim_quote_struct.h sim_trade_struct.h read_local_quote.h read_local_quote.cpp read_local_quote_test.cpp -o read_local_quote_test.exe -g
*/
void test_position(const char * filename)
{
    int position_num = 0;
    SimDataManager* data_manager = read_position_from_file(filename, NULL);
    printf("%p\n", data_manager);
    SimPosition* ptr = (SimPosition*)data_manager->begin;
    while (ptr != data_manager->end)
    {   
        
        printf("code:%s Market:%d total:%ld sellable:%ld price:%.2lf\n", 
        ptr->code, ptr->exchange_id, ptr->total_qty, ptr->sellable_qty, ptr->price);
        ptr++;
    }
    printf("position_num:%d\n", position_num);
}

// 测试读取股票信息
void test_stock_info(const char * filename, const int date)
{
    int stock_num = 0;
    SimDataManager* data_manager = read_stock_info_from_file(filename, NULL, date);
    printf("%p\n", data_manager);
    SimStockInfo* ptr = (SimStockInfo*)data_manager->begin;
    while (ptr != data_manager->end)
    {   
        printf("code:%s Market:%d preclose:%.2lf\tuplimit:%.2lf\n", 
        ptr->code, ptr->exchange_id,  ptr->pre_close_price, ptr->upper_limit_price);
        ptr++;
    }
    printf("date:%d stock_num:%d\n", date, stock_num);
}

// 测试读取快照数据
void test_snap_data(const char* filename)
{
    int data_num = 0, snapdata_flag = 1;
    SimDataManager* data_manager = read_snap_data_from_file(filename, NULL);
    SimSnapData* ptr = (SimSnapData*)data_manager->begin;
    while (ptr!= data_manager->end)
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
    int data_num = 0, leveldata_flag = 1;
    SimDataManager* data_manager = read_level_data_from_file(filename, NULL);
    SimLevelData* ptr = (SimLevelData*)data_manager->begin;
    while (ptr != data_manager->end)
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
    int data_num = 0, flag = 1;
    SimDataManager* data_manager = read_tick_order_from_file(filename, NULL);
    SimTickOrder* ptr = (SimTickOrder*) data_manager->begin;
    int64_t last_time = 0;
    while (ptr != data_manager->end)
    {   
        data_num++;
        if (data_num >= flag)
        {
            flag *= 2;
            printf("line:%08d code:%s Market:%d seq:%09ld price:%.2lf vol:%ld side:%c type:%c\n", data_num, 
            ptr->code, ptr->exchange_id, ptr->seq, ptr->price, ptr->qty, ptr->side, ptr->type);
        }
        if (ptr->data_time < last_time)
        {
            printf("error time:%ld last_time:%ld\n", ptr->data_time, last_time);
        }
        last_time = ptr->data_time;
        ptr++;
    }
    printf("tickorder_num:%d\n", data_num);
}

void test_tick_trade(const char * filename)
{
    int data_num = 0, flag = 1;
    SimDataManager* data_manager = read_tick_trade_from_file(filename, NULL);
    SimTickTrade* ptr = (SimTickTrade*) data_manager->begin;
    while (ptr != data_manager->end)
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
    test_position("position.txt");
    test_stock_info("/home/wiz/data/uplimit_list.csv", 20190103);
    test_snap_data("/home/wiz/data/L2_data/2017/01/03/am_hq_snap_spot.csv");
    test_level_data("/home/wiz/data/L2_data/2017/01/03/am_snap_level_spot.csv");
    test_tick_order("/home/wiz/data/L2_data/2017/01/03/am_hq_order_spot.csv");
    test_tick_trade("/home/wiz/data/L2_data/2017/01/03/am_hq_trade_spot.csv");
    return 0;
}