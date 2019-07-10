#include "quote_api_struct.h"
#define QUOTE_LINESIZE 1024
#define QUOTE_BLOCKSIZE 1024
#define QUOTE_BLOCKNUM 32768

// 从文件里读取股票静态信息
// const char* filename 文件名
// const int date 指定日期
// 返回首个股票信息的指针
// 失败返回NULL
StockInfo* read_stock_info_from_file(const char* filename, const int date);

// 从文件里读取深度行情
// const char* filename 文件名
// 返回首个股票深度行情信息的指针
// 失败返回NULL
MarketData* read_market_data_from_file(const char* filename);

// 从文件里读取逐笔委托
void read_tickbytick_order_from_file(const char* filename, TickByTick* tickbytick);
// 从文件里读取逐笔成交
void read_tickbytick_trade_from_file(const char* filename, TickByTick* tickbytick);

// 读取股票静态信息
// char* line  需要读取的字符串
// StockInfo* stockinfo_ptr 选哟赋值的结构体
// const int date 指定日期
bool read_stock_info_from_line(char* line, StockInfo* stockinfo_ptr, const int date);
// 读取深度行情
bool read_market_data_from_line(char* line, MarketData* marketdata_ptr);
// 读取逐笔委托
bool read_tickbytick_order_from_line(char* line, TickByTick* tickbytick);
// 读取逐笔成交
bool read_tickbytick_trade_from_line(char* line, TickByTick* tickbytick);
