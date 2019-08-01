#include "generator_global.h"
#include <string.h>

namespace Sim
{   
    // 储存交易线程生成的订单数据
    vector<TickOrder*> tickorder_vec;    
    // 行情线程已读取的订单数
    size_t tickorder_read_num = 0;
    // 储存交易线程生成的交易数据
    vector<TickTrade*> ticktrade_vec;
    // 储存行情线程已读取的成交数
    size_t ticktrade_read_num = 0;
    // 行情是否启动
    bool quote_start = false;
    // 交易是否启动
    bool trade_start = false;
    // 储存每个股票的最新成交信息
    map<int, TickTrade*> lastest_ticktrade_map;
    // 储存每个股票的订单
    map<int, deque<LocalOrder*> order_map;
}

void Sim::initial_generator_data()
{
    tickorder_vec.reserve(512);
    ticktrade_vec.reserve(512);
    lastest_ticktrade_map[0] = NULL;
}

TickTrade* Sim::get_lastest_ticktrade(const char* code)
{   
    if (code == NULL)
        return NULL;
    int codenum = atoi(code);
    return lastest_ticktrade_map[codenum];
}

void Sim::set_lastest_ticktrade(TickTrade* ticktrade)
{
    if (ticktrade == NULL || ticktrade->code == NULL || ticktrade->type == LOCAL_TRADETYPE_CANCEL)
        return;
    int codenum = atoi(ticktrade->code);
    lastest_ticktrade_map[codenum] = ticktrade;
}