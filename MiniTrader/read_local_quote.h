#include "local_quote_struct.h"
#define QUOTE_LINESIZE 1024
#define QUOTE_BLOCKSIZE 1024
#define QUOTE_BLOCKNUM 32768

// 从文件里读取股票静态信息
// const char* filename 文件名
// const int date 指定日期
// 返回首个股票信息的指针
// 失败返回NULL
StockInfo* read_stock_info_from_file(const char* filename, const int date, int *array_size);

// 从文件里读取行情快照  高开低收等行情数据
// const char* filename 文件名 
// 返回首个snap行情信息的指针
// 失败返回NULL
SnapData* read_snap_data_from_file(const char* filename, int *array_size);

// 从文件里读取10档行情 
// const char* filename 文件名
// 返回首个level行情信息的指针
// 失败返回NULL
LevelData* read_level_data_from_file(const char* filename, int *array_size);

// 从文件里读取逐笔委托
// const char* filename 文件名
// 返回首个委托行情信息的指针
// 失败返回NULL
TickOrder* read_tick_order_from_file(const char* filename, int *array_size);

// 从文件里读取逐笔成交
// const char* filename 文件名
// 返回首个成交行情信息的指针
// 失败返回NULL
TickTrade* read_tick_trade_from_file(const char* filename, int *array_size);

// 读取股票静态信息
// char* line  需要读取的字符串
// StockInfo* stockinfo_ptr 需要赋值的结构体
// const int date 指定日期
// 返回读取结果  true 读取成功   false 读取失败
bool read_stock_info_from_line(char* line, StockInfo* stockinfo_ptr, const int date);

// 读取深度行情
// char* line  需要读取的字符串
// MarketData* marketdata_ptr 需要赋值的结构体
// 返回读取结果  true 读取成功   false 读取失败
bool read_snap_data_from_line(char* line, SnapData* marketdata_ptr);

// 读取10档行情
// char* line  需要读取的字符串
// MarketData* marketdata_ptr 需要赋值的结构体
// 返回读取结果  true 读取成功   false 读取失败
bool read_level_data_from_line(char* line, LevelData* marketdata_ptr);

// 读取逐笔委托
// char* line  需要读取的字符串
// TickByTickEntrust* entrust_ptr 需要赋值的结构体
// 返回读取结果  true 读取成功   false 读取失败
bool read_tick_order_from_line(char* line, TickOrder* tickorder_ptr);

// 读取逐笔成交
// char* line  需要读取的字符串
// TickByTickTrade* trade_ptr 需要赋值的结构体
// 返回读取结果  true 读取成功   false 读取失败
bool read_tick_trade_from_line(char* line, TickTrade* ticktrade_ptr);
