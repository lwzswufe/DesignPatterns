#ifndef _LOCAL_QUOTE_STRUCT_
#define _LOCAL_QUOTE_STRUCT_
#include "local_quote_struct.h"
#endif
class Quote_Generator
{
public: 
    Quote_Generator(const char* ini_file);
    // 载入上午/下午的数据
    void load_data(bool is_am);
    // 判断是否需要载入数据 若所有数据指针都为NULL就返回true 否则返回false
    bool check_is_need_load_data();
    // 获取当前时间 yyyymmddHHMMSSsss
    int64_t Get_Time();
    // 获取当前时刻 HHMMSS.sss
    double Get_Clock();
    // 获取股票静态信息
    const StockInfo* Get_StockInfo();
    // 获取逐笔委托 
    const TickOrder* Get_TickOrder();
    // 逐笔成交信息
    const TickTrade* Get_TickTrade();
    // 获取深度行情数据
    const SnapData* Get_SnapData();
    // 获取深度行情数据
    const LevelData* Get_LevelData();
    // 更新时间
    void update_time();
private:
    // 配置文件名
    char * dirname;
    // 当前日期
    int datenum;
    // 当前时间
    int64_t time_now;
    // 下一个数据的时间
    int64_t time_next;
    // 股票静态数据数组
    StockInfo* stockinfo_array;
    // 股票静态数据当前指针
    StockInfo* stockinfo_ptr;
    // 股票快照数据数组
    SnapData* snapdata_array;
    // 股票快照数据当前指针
    SnapData* snapdata_ptr;
    // 股票10档行情数据数组
    LevelData* leveldata_array;
    // 股票10档行情数据当前指针
    LevelData* leveldata_ptr;
    // 逐笔委托数据数组
    TickOrder* tickorder_array;
    // 逐笔委托数据当前指针
    TickOrder* tickorder_ptr;
    // 逐笔成交数据数组
    TickTrade* ticktrade_array;
    // 逐笔成交数据当前指针
    TickTrade* ticktrade_ptr;
};