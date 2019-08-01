/*
定义行情生成器和交易生成器中需要用到的全局变量
*/
#include "local_quote_struct.h"
#include "local_trade_struct.h"
#include <vector>
using std::vector;
#include <map>
using std::map;
#include <deque>
using std::deque;

namespace Sim
{   
    // 储存交易线程生成的订单数据
    extern vector<TickOrder*> tickorder_vec;    
    // 行情线程已读取的订单数
    extern size_t tickorder_read_num;
    // 储存交易线程生成的交易数据
    extern vector<TickTrade*> ticktrade_vec;
    // 储存行情线程已读取的成交数
    extern size_t ticktrade_read_num;
    // 储存每个股票的最新成交信息
    extern map<int, TickTrade*> lastest_ticktrade_map;
    // 储存每个股票的订单
    extern map<int, deque<LocalOrder*> order_map;
    // 行情是否启动
    extern bool quote_start;
    // 交易是否启动
    extern bool trade_start;
    // 数据初始化
    void initial_generator_data();
    // 查找某个股票的最新成交信息 若未找到 返回NULL
    TickTrade* get_lastest_ticktrade(const char* code);
    // 设置某个股票的最新成交信息
    void set_lastest_ticktrade(TickTrade* ticktrade);
    // 撤单
    unsigned cancel_order(unsigned order_id);
    // 报单
    unsigned insert_order(LocalOrder* order);
    // 模拟成交 返回成交信息并修改订单的成交状态 若未成交返回NULL
    TickTrade* simulate_trade(const TickTrade* ticktrade, LocalOrder* order);
}