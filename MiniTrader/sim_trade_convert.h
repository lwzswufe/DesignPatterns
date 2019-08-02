// 接受本地数据转并返回 回测系统需要的数据结构

// 本地数据结构
#ifndef _SIM_TRADE_STRUCT_
#define _SIM_TRADE_STRUCT_
#include "sim_trade_struct.h"
#endif
// 回测数据结构
#include "Trade_Api_Struct.h"

// 将报单结构体转化为交易系统报单结构体
SimOrder* convert_order_struct(const OrderInsertInfo* orderinsert);

// 将模拟成交系统报单结构体转化为订单信息结构体  
OrderInfo* convert_order_struct(const SimOrder* order);

// 将成交信息转化为成交通知
TradeReport* convert_trade_struct(const SimOrder* order, const SimTickTrade* ticktrade);