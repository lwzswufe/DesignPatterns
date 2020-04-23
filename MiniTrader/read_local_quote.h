#ifndef _SIM_QUOTE_STRUCT_H_
#define _SIM_QUOTE_STRUCT_H_
#include "sim_quote_struct.h"
#endif
#ifndef _SIM_TRADE_STRUCT_H_
#define _SIM_TRADE_STRUCT_H_
#include "sim_trade_struct.h"
#endif
#define QUOTE_LINESIZE 1024
#define QUOTE_BLOCKSIZE 1024
#define QUOTE_BLOCKNUM 32768
#define QUOTE_SEQ_MULTIPLE 10

// 从文件里读取股票静态信息
// const char* filename 文件名
// SimDataManager* data_manager 数据管理指针 若为空 则函数会创建一个新的数据管理指针
// const int date 指定日期
// 返回股票信息的数据管理指针
// 失败返回NULL
SimDataManager* read_stock_info_from_file(const char* filename, SimDataManager* data_manager, const int date);

// 从文件里读取行情快照  高开低收等行情数据
// const char* filename 文件名 
// SimDataManager* data_manager 数据管理指针 若为空 则函数会创建一个新的数据管理指针
// 返回snap行情信息的数据管理指针
// 失败返回NULL
SimDataManager* read_snap_data_from_file(const char* filename, SimDataManager* data_manager);

// 从文件里读取10档行情 
// const char* filename 文件名
// SimDataManager* data_manager 数据管理指针 若为空 则函数会创建一个新的数据管理指针
// 返回level行情信息的数据管理指针
// 失败返回NULL
SimDataManager* read_level_data_from_file(const char* filename, SimDataManager* data_manager);

// 从文件里读取逐笔委托
// const char* filename 文件名
// SimDataManager* data_manager 数据管理指针 若为空 则函数会创建一个新的数据管理指针
// 返回委托行情信息的数据管理指针
// 失败返回NULL
SimDataManager* read_tick_order_from_file(const char* filename, SimDataManager* data_manager);

// 从文件里读取逐笔成交
// const char* filename 文件名
// SimDataManager* data_manager 数据管理指针 若为空 则函数会创建一个新的数据管理指针
// 返回成交行情信息的数据管理指针
// 失败返回NULL
SimDataManager* read_tick_trade_from_file(const char* filename, SimDataManager* data_manager);

// 从文件里读取持仓数据
// const char* filename 文件名
// SimDataManager* data_manager 数据管理指针 若为空 则函数会创建一个新的数据管理指针
// 返回成交行情信息的数据管理指针
// 失败返回NULL
SimDataManager* read_position_from_file(const char* filename, SimDataManager* data_manager);

// 读取股票静态信息
// char* line  需要读取的字符串
// SimStockInfo* stockinfo_ptr 需要赋值的结构体
// const int date 指定日期
// 返回读取结果  true 读取成功   false 读取失败
bool read_stock_info_from_line(char* line, SimStockInfo* stockinfo_ptr, const int date);

// 读取深度行情
// char* line  需要读取的字符串
// MarketData* marketdata_ptr 需要赋值的结构体
// 返回读取结果  true 读取成功   false 读取失败
bool read_snap_data_from_line(char* line, SimSnapData* marketdata_ptr);

// 读取10档行情
// char* line  需要读取的字符串
// MarketData* marketdata_ptr 需要赋值的结构体
// 返回读取结果  true 读取成功   false 读取失败
bool read_level_data_from_line(char* line, SimLevelData* marketdata_ptr);

// 读取逐笔委托
// char* line  需要读取的字符串
// TickByTickEntrust* entrust_ptr 需要赋值的结构体
// 返回读取结果  true 读取成功   false 读取失败
bool read_tick_order_from_line(char* line, SimTickOrder* tickorder_ptr);

// 读取逐笔成交
// char* line  需要读取的字符串
// TickBySimTickTrade* trade_ptr 需要赋值的结构体
// 返回读取结果  true 读取成功   false 读取失败
bool read_tick_trade_from_line(char* line, SimTickTrade* ticktrade_ptr);

// 读取持仓数据
// char* line  需要读取的字符串
// SimPosition* position_ptr 需要赋值的结构体
// 返回读取结果  true 读取成功   false 读取失败
bool read_position_from_line(char* line, SimPosition* position_ptr);