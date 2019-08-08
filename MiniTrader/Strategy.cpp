#ifndef _STRATEGY_H_
#include "Strategy.h"
#endif
#include <stdio.h>
#include <memory.h>
#include <string.h>

namespace Strategy
{   
    // 待发送订单数
    unsigned send_order_num = 0;
    // 已发送订单数
    unsigned sended_order_num = 0;
    // 已匹配订单数
    unsigned march_order_num = 0;
    // 最大订单数
    const unsigned max_order_num = 20;
    // 用户订单队列 已发+未发
    SimOrder user_order_ptr_arr[20];
};

void Strategy::march_order(const SimTickOrder* tickorder)
{
    if (march_order_num >= send_order_num)
        return;
    const SimOrder* simorder = user_order_ptr_arr + send_order_num;
    if (tickorder->side == SIM_ORDERSIDE_BUY &&
        tickorder->type == SIM_ORDERTYPE_LIMIT &&
        tickorder->price == simorder->price &&
        tickorder->qty == simorder->vol &&
        strcmp(tickorder->code, simorder->code) == 0)
    {   
        march_order_num++;
        printf("successful match order:%d\n", march_order_num);
    }
}

void Strategy::send_order(const SimTickOrder* tickorder)
{
    if (send_order_num >= max_order_num)
        return;
    if (tickorder->side == SIM_ORDERSIDE_BUY &&
        tickorder->type == SIM_ORDERTYPE_LIMIT &&
        tickorder->qty >= 8 * 10000 )
    {   
        SimOrder* simorder = user_order_ptr_arr + send_order_num;
        memset(simorder, 0, sizeof(SimOrder));
        strncpy(simorder->code, tickorder->code, SIM_CODESIZE);
        simorder->exchange_id = tickorder->exchange_id;
        simorder->price = tickorder->price;
        simorder->vol = (send_order_num + 1) * 100;
        simorder->side = SIM_ORDERSIDE_BUY;
        simorder->type = SIM_ORDERTYPE_LIMIT;
        simorder->state = SIM_ORDERSTATE_SENDED;
        simorder->account_id = 1;
        simorder->id = -1;
        send_order_num++;
        printf("create order:%d\n", send_order_num);
    }
}

void test_position(const SimPosition* ptr)
{   
    static int flag = 1, call_times = 0;
    call_times++;
    if (call_times >= flag)
    {  
        flag *= 2;
        printf("index:%08d code:%s Market:%d total:%ld sellable:%ld price:%.2lf\n", call_times,
        ptr->code, ptr->exchange_id, ptr->total_qty, ptr->sellable_qty, ptr->price);
    }
}

// 测试读取股票信息
void test_stock_info(const SimStockInfo* ptr)
{   
    static int flag = 1, call_times = 0;
    call_times++;
    if (call_times >= flag)
    {  
        flag *= 2;
        printf("index:%08d code:%s Market:%d preclose:%.2lf\tuplimit:%.2lf\n", call_times,
        ptr->code, ptr->exchange_id,  ptr->pre_close_price, ptr->upper_limit_price);
    }
}

// 测试读取快照数据
void test_snap_data(const SimSnapData* ptr)
{   
    static int flag = 1, call_times = 0;
    call_times++;
    if (call_times >= flag)
    {
        flag *= 2;
        printf("index:%08d code:%s Market:%d preclose:%.2lf\tuplimit:%.2lf\n", call_times,
        ptr->code, ptr->exchange_id,  ptr->pre_close_price, ptr->upper_limit_price);
    }
}

// 测试读取10档行情数据
void test_level_data(const SimLevelData* ptr)
{
    static int flag = 1, call_times = 0;
    call_times++;
    if (call_times >= flag)
    {
        flag *= 2;
        printf("index:%08d code:%s Market:%d b1pr:%.2lf b1vol_queue:%d\n", call_times, 
                ptr->code, ptr->exchange_id,  ptr->bid[0], ptr->bid1_count);
    }
}

void test_tick_order(const SimTickOrder* ptr)
{
    static int flag = 1, call_times = 0;
    call_times++;
    if (call_times >= flag)
    {
        flag *= 2;
        printf("index:%08d code:%s Market:%d seq:%09ld price:%.2lf vol:%ld side:%c type:%c\n", call_times, 
        ptr->code, ptr->exchange_id, ptr->seq, ptr->price, ptr->qty, ptr->side, ptr->type);
    }
}

void test_tick_trade(const SimTickTrade * ptr)
{
    static int flag = 1, call_times = 0;
    call_times++;
    if (call_times >= flag)
    {
        flag *= 2;
        printf("index:%08d code:%s Market:%d seq:%09ld seq_buy:%09ld seq_sell:%09ld price:%.2lf vol:%ld type:%c\n", call_times, 
        ptr->code, ptr->exchange_id, ptr->seq, ptr->bid_no, ptr->ask_no, ptr->price, ptr->qty, ptr->type);
    }
}
