// 接受本地数据转并返回 回测系统需要的数据结构

// 本地数据结构
#ifndef _LOCAL_TRADE_STRUCT_
#define _LOCAL_TRADE_STRUCT_
#include "local_trade_struct.h"
#endif
// 回测数据结构
#include "Trade_Api_Struct.h"

// 将报单结构体转化为交易系统报单结构体
LocalOrder* convert_order_struct(const OrderInsertInfo* orderinsert);

// 将模拟成交系统报单结构体转化为订单信息结构体  
OrderInfo* convert_order_struct(const LocalOrder* localorder);

