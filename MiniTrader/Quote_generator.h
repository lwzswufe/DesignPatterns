#include "local_quote_struct.h"

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
    StockInfo* Get_StockInfo();
    // 获取逐笔委托 
    TickOrder* Get_TickOrder();
    // 逐笔成交信息
    TickTrade* Get_TickTrade();
    // 获取深度行情数据
    SnapData* Get_SnapData();
    // 获取深度行情数据
    LevelData* Get_LevelData();
private:
    // 当前日期
    int date;
    // 当前时间
    int64_t time_now;
    // 下一个数据的时间
    int64_t time_next;
    // 股票静态数据数组
    const StockInfo* stockinfo_array;
    // 股票静态数据当前指针
    const StockInfo* stockinfo_ptr;
    // 股票快照数据数组
    const SnapData* snapdata_array;
    // 股票快照数据当前指针
    const SnapData* snapdata_ptr;
    // 股票10档行情数据数组
    const LevelData* leveldata_array;
    // 股票10档行情数据当前指针
    const LevelData* leveldata_ptr;
    // 逐笔委托数据数组
    const TickOrder* tickorder_array;
    // 逐笔委托数据当前指针
    const TickOrder* tickorder_ptr;
    // 逐笔成交数据数组
    const TickTrade* ticktrade_array;
    // 逐笔成交数据当前指针
    const TickTrade* ticktrade_ptr;
};