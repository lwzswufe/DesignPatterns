// 接受本地数据转并返回 回测系统需要的数据结构

// 本地数据结构
#ifndef _SIM_QUOTE_STRUCT_H_
#include "sim_quote_struct.h"
#endif
// 回测数据结构
#include "Quote_Api_Struct.h"

// 转换股票静态信息数据
void convert_stockinfo(const SimStockInfo* stockinfo_ptr, StockStaticInfo * stockstaticinfo_ptr, bool* is_last);

// 转换深度行情数据
void convert_depthdata(const SimSnapData* snap_ptr, const SimLevelData* level_ptr, DepthData * data_ptr);

// 转换委托流数据
void convert_tickorder(const SimTickOrder* tickorder_ptr, TickByTickStruct * tick_ptr);

// 转换成交流数据
void convert_ticktrade(const SimTickTrade* ticktrade_ptr, TickByTickStruct * tick_ptr);

