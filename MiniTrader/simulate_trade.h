/*
模拟交易
*/
#ifndef _SIM_QUOTE_STRUCT_H_
#include "sim_quote_struct.h"
#endif
#ifndef _SIM_Trade_STRUCT_H_
#include "sim_trade_struct.h"
#endif
#include <vector>
using std::vector;
#include <map>
using std::map;
#include <deque>
using std::deque;

namespace Sim
{   
    // 交易线程已读取 成交报告数
    extern size_t tradereport_num;
    // 储存成交报告
    extern vector<SimTradeReport*> tradereport_vec;
    // 已报单数
    extern size_t localorder_num;
    // 储存已报订单
    extern vector<SimOrder*> localorder_vec;
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
    // 储存每个股票的订单
    extern map<int, deque<SimOrder*>> order_map;
    // 缓存最近生成成交记录 链表
    extern SimTradeReport first_tradereport;
    // 储存最新的行情订单编号
    extern int64_t latest_order_seq;
    // 储存最新的交易订单编号
    extern int64_t latest_trade_seq;
    // 行情是否启动
    extern bool quote_start;
    // 交易是否启动
    extern bool trade_start;

    // 配置文件名
    extern char * dirname;
    // 当前日期
    extern int datenum;
    // 当前时间
    extern int64_t time_now;
    // 下一个数据的时间
    extern int64_t time_next;
    // 股票静态数据数组管理结构体
    extern SimDataManager* stockinfo_manager;
    // 股票快照数据数组管理结构体
    extern SimDataManager* snapdata_manager;
    // 股票10档行情数据数组管理结构体
    extern SimDataManager* leveldata_manager;
    // 逐笔委托数据数组管理结构体
    extern SimDataManager* tickorder_manager;
    // 逐笔成交数据数据管理结构体
    extern SimDataManager* ticktrade_manager;

    // 股票静态数据返回链表头节点
    extern SimStockInfo* stockinfo_head_node;
    // 股票快照数据返回链表头节点
    extern SimSnapData* snapdata_head_node;
    // 股票10档行情数据返回链表头节点
    extern SimLevelData* leveldata_head_node;
    // 逐笔委托数据返回链表头节点
    extern SimTickOrder* tickorder_head_node;
    // 逐笔成交数据返回链表头节点
    extern SimTickTrade* ticktrade_head_node;

    // 数据初始化
    void initial_generator_data();
    // 订单模拟成交
    // 返回成交记录链表的起始节点指针
    void simulate_trade(const SimTickTrade* ticktrade);
    // // 查找某个股票的最新成交信息 若未找到 返回NULL
    // SimTickTrade* get_lastest_ticktrade(const char* code);
    // // 查找某个股票的最新成交信息 若未找到 返回NULL
    // SimTickTrade* get_lastest_ticktrade(const int codenum);
    // 模拟成交 返回成交信息并修改订单的成交状态 若未成交返回NULL
    SimTickTrade* simulate_trade(const SimTickTrade* ticktrade, SimOrder* order);
    // 模拟撤单 返回成交信息并修改订单的成交状态 若撤单失败返回NULL
    SimTickTrade* convert_order_struct(const SimOrder* order, const char tradetype);
    // 将报单信息转化为最新的行情信息
    // const SimOrder* order 用户订单
    // bool reverse 是否生成反向订单 true 生成反向订单 false 不生成反向订单
    SimTickOrder* convert_order_struct(const SimOrder* order, bool reverse);
    // 获取已成交订单 返回NULL表示没有成交的订单
    SimOrder* get_traded_order();
    // 撤单 撤销localorder_vec的第几笔报单 
    unsigned cancel_order(unsigned order_id);
    // 报单 返回本次报单是localorder_vec的第几笔报单
    unsigned insert_order(SimOrder* order);

    // 载入上午/下午的数据
    void load_data(bool is_am);
    // 判断是否需要载入数据 若所有数据指针都为NULL就返回true 否则返回false
    bool check_is_need_load_data();
    // 获取当前时间 HHMMSS
    int Get_Timenum();
    // 获取当前时间 yyyymmddHHMMSSsss
    int64_t Get_Time();
    // 获取当前时刻 HHMMSS.sss
    double Get_Clock();
    // 获取股票静态信息
    // 返回链表头结点指针 通过next访问其他元素
    // 头结点指针数据无意义 
    const SimStockInfo* Get_StockInfo();
    // 获取逐笔委托 
    // 返回链表头结点指针 通过next访问其他元素
    // 头结点指针数据无意义 
    const SimTickOrder* Get_TickOrder();
    // 逐笔成交信息
    // 返回链表头结点指针 通过next访问其他元素
    // 头结点指针数据无意义 
    const SimTickTrade* Get_TickTrade();
    // 获取深度行情数据
    // 返回链表头结点指针 通过next访问其他元素
    // 头结点指针数据无意义
    const SimSnapData* Get_SnapData();
    // 获取深度行情数据
    // 返回链表头结点指针 通过next访问其他元素
    // 头结点指针数据无意义
    const SimLevelData* Get_LevelData();
    // 更新时间
    void update_time();  
    // 更新下一次数据更新时间
    void update_time_next(int64_t data_time);
};