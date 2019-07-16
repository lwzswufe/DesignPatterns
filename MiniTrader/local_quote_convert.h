// 接受本地数据转并返回 回测系统需要的数据结构

// 本地数据结构
#include "local_quote_struct.h"
// 回测数据结构
#include "quote_api_struct.h"

// 转换股票静态信息数据
StockStaticInfo * convert_stockinfo(StockInfo* stockinfo_ptr, bool* is_last);

// 转换深度行情数据
DepthData * convert_depthdata(SnapData* snap_ptr, LevelData* level_ptr);

// 转换委托流数据
TickByTickStruct * convert_tickorder(TickOrder* tickorder_ptr);

// 转换成交流数据
TickByTickStruct * convert_ticktrade(TickTrade* ticktrade_ptr);

