#include "Trade_generator.h"
#include <string.h>

namespace Sim
{   
    // 储存已报订单
    vector<SimOrder*> localorder_vec;
    // 报单数
    size_t localorder_num = 0;
    // 储存交易线程生成的订单数据
    vector<SimTickOrder*> tickorder_vec;    
    // 行情线程已读取的订单数
    size_t tickorder_read_num = 0;
    // 储存交易线程生成的交易数据
    vector<SimTickTrade*> ticktrade_vec;
    // 储存行情线程已读取的成交数
    size_t ticktrade_read_num = 0;
    // 行情是否启动
    bool quote_start = false;
    // 交易是否启动
    bool trade_start = false;
    // 储存每个股票的最新成交信息
    map<int, SimTickTrade*> lastest_ticktrade_map;
    // 储存每个股票的订单
    map<int, deque<SimOrder*>> order_map;
    // 缓存最近成交的订单
    deque<SimTradeReport*> trade_report_cache;
    // 储存最新的行情订单编号
    int64_t latest_order_seq = 0;
    // 储存最新的交易订单编号
    int64_t latest_trade_seq = 0;
}

void Sim::initial_generator_data()
{
    tickorder_vec.reserve(512);
    ticktrade_vec.reserve(512);
    lastest_ticktrade_map[0] = NULL;
}

// SimTickTrade* Sim::get_lastest_ticktrade(const char* code)
// {   
//     if (code == NULL)
//         return NULL;
//     int codenum = atoi(code);
//     return lastest_ticktrade_map[codenum];
// }

// SimTickTrade* Sim::get_lastest_ticktrade(const int codenum)
// {
//     return lastest_ticktrade_map[codenum];
// }

void Sim::set_lastest_ticktrade(SimTickTrade* ticktrade)
{
    if (ticktrade == NULL || ticktrade->code == NULL || ticktrade->type == SIM_TRADETYPE_CANCEL)
        return;
    int codenum = atoi(ticktrade->code);
    lastest_ticktrade_map[codenum] = ticktrade;
}

unsigned Sim::cancel_order(unsigned order_id)
{
    cancelorder_num++;
    if (order_id > localorder_num)
        printf("error cancel order_id:%u\n", order_id);
    else 
    {   
        SimOrder* order = localorder_vec[order_id - 1];
        SimTickTrade* ticktrade;
        switch (order->state)
        {
        case SIM_ORDERSTATE_CANCELED:
            printf("error cancel order_id:%u this order is canceled\n", order_id);
            break;
        case SIM_ORDERSTATE_TRADED:
            printf("error cancel order_id:%u this order is traded\n", order_id);
            break;
        case SIM_ORDERSTATE_PARTTRADED:
        case SIM_ORDERSTATE_SENDED:
            order->state = SIM_ORDERSTATE_CANCELED;
            ticktrade = convert_cancel_struct(order);
            ticktrade_vec.push_back(ticktrade);
            break;
        default:
            printf("error cancel order_id:%u this order is error\n", order_id);
            break;
        }
    }
    return cancelorder_num + 2000;
}

unsigned Sim::insert_order(SimOrder* order)
{   
    localorder_num++;
    localorder_vec.push_back(order);
    SimTickOrder * new_tickorder = convert_order_struct(order);
    tickorder_vec.push_back(new_tickorder);
    return localorder_num;
}

SimTickOrder* Sim::convert_order_struct(const SimOrder* order)
{   
    if (order == NULL)
        return NULL;
    SimTickOrder* tickorder = new SimTickOrder;
    latest_order_seq++;
    memset(tickorder, 0, sizeof(SimTickOrder));
    tickorder->side = order->side;
    tickorder->type = order->type;
    tickorder->price = order->price;
    tickorder->qty = order->vol;
    tickorder->seq = latest_order_seq;
    tickorder->exchange_id = order->exchange_id;
    strncpy(tickorder->code, order->code, SIM_CODESIZE);
    return tickorder;
}

SimTickTrade* Sim::convert_cancel_struct(SimOrder* order)
{
    if (order == NULL)
        return NULL;
    latest_trade_seq++;
    SimTickTrade* ticktrade = new SimTickTrade;
    memset(ticktrade, 0, sizeof(SimTickTrade));
    if (order->side = SIM_ORDERSIDE_BUY)
        ticktrade->bid_no = order->seq;
    else
        ticktrade->ask_no = order->seq;
    ticktrade->type = SIM_TRADETYPE_CANCEL;
    ticktrade->price = order->price;
    ticktrade->qty = order->vol - order->traded_vol;
    ticktrade->seq = latest_trade_seq;
    ticktrade->exchange_id = order->exchange_id;
    strncpy(ticktrade->code, order->code, SIM_CODESIZE);
    return ticktrade;
}

SimOrder* Sim::get_traded_order()
{

}

SimTradeReport* Sim::simulate_trade()
{   
    SimTradeReport* first_tradereport;
    for (auto iter: Sim::lastest_ticktrade_map)
    {   
        int codenum = iter.first;
        if (iter.first == 0 || !Sim::order_map[codenum].empty() )
            continue;
        const SimTickTrade* latest_ticktrade = lastest_ticktrade_map[codenum];
        if (latest_ticktrade == NULL)
            continue;
        // 倒序遍历订单 方便删除
        size_t order_num = Sim::order_map[codenum].size();
        for (unsigned i= order_num - 1; i>=0; i--)
        {   // 模拟成交
            SimOrder* simorder = Sim::order_map[codenum][i];
            SimTickTrade* new_ticktrade = simulate_trade(latest_ticktrade, simorder);
            if (new_ticktrade != NULL)
            {   
                // 储存新生成的成交数据
                ticktrade_vec.push_back(new_ticktrade);
                // 储存订单成交信息
                SimTradeReport tradereport = {simorder, new_ticktrade};
                trade_report_cache.push_back(tradereport);
                // 从待匹配成交队列里删除这笔订单
                Sim::order_map[codenum].erase(Sim::order_map[codenum].begin() + i);
            }
        }
    }
}