#ifndef _SIM_QUOTE_STRUCT_H_
#include "sim_quote_struct.h"
#endif
#ifndef _SIM_TRADE_STRUCT_H_
#include "sim_trade_struct.h"
#endif
#include "Trade_Api_Struct.h"
#define _STRATEGY_H_

namespace Strategy
{   
    // 待发送订单数
    extern unsigned send_order_num;
    // 已发送订单数
    extern unsigned sended_order_num;
    // 已匹配订单数
    extern unsigned march_order_num;
    // 最大订单数
    extern const unsigned max_order_num;
    // 用户订单队列 已发+未发
    extern SimOrder user_order_ptr_arr[20];
    // 在委托流里匹配订单
    void march_order(const SimTickOrder* tickorder);
    // 根据委托流发送订单
    void send_order(const SimTickOrder* tickorder);
};

// 测试持仓信息
void test_position(const SimPosition* ptr);

// 测试读取股票信息
void test_stock_info(const SimStockInfo* ptr);

// 测试读取快照数据
void test_snap_data(const SimSnapData* ptr);

// 测试读取10档行情数据
void test_level_data(const SimLevelData* ptr);

// 测试委托流
void test_tick_order(const SimTickOrder* ptr);

// 测试成交流
void test_tick_trade(const SimTickTrade * ptr);