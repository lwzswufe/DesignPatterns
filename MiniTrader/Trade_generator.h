/*
定义行情生成器和交易生成器中需要用到的全局变量
*/
#include "sim_quote_struct.h"
#include "sim_trade_struct.h"
#include <vector>
using std::vector;
#include <map>
using std::map;
#include <deque>
using std::deque;

namespace Sim
{   
    // 储存已报订单
    extern vector<SimOrder*> localorder_vec;
    // 已报单数
    extern size_t localorder_num;
    // 已报撤单数
    extern size_t cancelorder_num;
    // 储存交易线程生成的订单数据
    extern vector<SimTickOrder*> tickorder_vec;    
    // 行情线程已读取的订单数
    extern size_t tickorder_read_num;
    // 储存交易线程生成的交易数据
    extern vector<SimTickTrade*> ticktrade_vec;
    // 储存行情线程已读取的成交数
    extern size_t ticktrade_read_num;
    // 储存每个股票的最新成交信息
    extern map<int, SimTickTrade*> lastest_ticktrade_map;
    // 储存每个股票的订单
    extern map<int, deque<SimOrder*>> order_map;
    // 缓存最近成交的订单
    extern deque<SimTradeReport*> trade_report_cache;
    // 储存最新的行情订单编号
    extern int64_t latest_order_seq;
    // 储存最新的交易订单编号
    extern int64_t latest_trade_seq;
    // 行情是否启动
    extern bool quote_start;
    // 交易是否启动
    extern bool trade_start;
    // 数据初始化
    void initial_generator_data();
    // 订单模拟成交
    void simulate_trade();
    // // 查找某个股票的最新成交信息 若未找到 返回NULL
    // SimTickTrade* get_lastest_ticktrade(const char* code);
    // // 查找某个股票的最新成交信息 若未找到 返回NULL
    // SimTickTrade* get_lastest_ticktrade(const int codenum);
    // 模拟成交 返回成交信息并修改订单的成交状态 若未成交返回NULL
    SimTickTrade* simulate_trade(const SimTickTrade* ticktrade, SimOrder* order);
    // 模拟撤单 返回成交信息并修改订单的成交状态 若撤单失败返回NULL
    SimTickTrade* convert_cancel_struct(SimOrder* order);
    // 将报单信息转化为最新的行情信息
    SimTickOrder* convert_order_struct(const SimOrder* order);
    // 获取已成交订单 返回NULL表示没有成交的订单
    SimOrder* get_traded_order();
    // 设置某个股票的最新成交信息
    void set_lastest_ticktrade(SimTickTrade* ticktrade);
    // 撤单 撤销localorder_vec的第几笔报单 
    unsigned cancel_order(unsigned order_id);
    // 报单 返回本次报单是localorder_vec的第几笔报单
    unsigned insert_order(SimOrder* order);
    
}