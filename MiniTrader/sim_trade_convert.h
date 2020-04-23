// 接受本地数据转并返回 回测系统需要的数据结构
// 本地数据结构
#ifndef _SIM_TRADE_STRUCT_H_
#include "sim_trade_struct.h"
#endif
// 回测数据结构
#include "Trade_Api_Struct.h"

// 将模拟交易系统报单结构体转化为报单结构体
void convert_order_struct(const SimOrder* simorder, OrderInsertInfo* orderinsert);

// 将报单结构体转化为模拟交易系统报单结构体
void convert_order_struct(const OrderInsertInfo* orderinsert, SimOrder* simorder);

// 将模拟成交系统报单结构体转化为订单信息结构体  
void convert_order_struct(const SimOrder* order, OrderInfo* orderinfo);

// 将持仓信息转化为成交通知
void convert_position_struct(const SimPosition* simposition, QueryStkPositionRsp* position_info);

// 将成交信息转化为成交通知
void convert_trade_struct(const SimTradeReport* simtradereport, TradeReport* tradereport);