// 接受本地数据转并返回 回测系统需要的数据结构

// 本地数据结构
#ifndef _SIM_TRADE_STRUCT_H_
#include "sim_trade_struct.h"
#endif
// 回测数据结构
#include "Trade_Api_Struct.h"

// 将报单结构体转化为模拟交易系统报单结构体
void convert_order_struct(const OrderInsertInfo* orderinsert, SimOrder* simorder);

// 将模拟成交系统报单结构体转化为订单信息结构体  
void convert_order_struct(const SimOrder* order, OrderInfo*);

// 将成交信息转化为成交通知
// void convert_trade_struct(const SimOrder* order, const SimTickTrade* ticktrade, TradeReport* tradereport);

// 将成交信息转化为成交通知
void convert_trade_struct(const SimTradeReport* simtradereport, TradeReport* tradereport);