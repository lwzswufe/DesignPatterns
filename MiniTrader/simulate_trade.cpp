#include <string.h>
#include "Read_ini.h"
#include "simulate_trade.h"
#include "read_local_quote.h"

namespace Sim
{
    // 交易线程已读取 成交报告数
    size_t tradereport_num = 0;
    // 储存成交报告
    vector<SimTradeReport*> tradereport_vec;
    // 报单数
    size_t localorder_num = 0;
    // 储存已报订单
    vector<SimOrder *> localorder_vec;
    // 已报撤单数
    size_t cancelorder_num = 0;
    // 储存交易线程生成的订单数据
    vector<SimTickOrder *> tickorder_vec;
    // 行情线程已读取的订单数
    size_t tickorder_read_num = 0;
    // 储存交易线程生成的交易数据
    vector<SimTickTrade *> ticktrade_vec;
    // 储存行情线程已读取的成交数
    size_t ticktrade_read_num = 0;
    // 行情是否启动
    bool quote_start = false;
    // 交易是否启动
    bool trade_start = false;
    // 储存每个股票的订单
    map<int, deque<SimOrder *>> order_map;
    // 缓存最近生成成交记录 链表
    extern SimTradeReport first_tradereport;
    // 储存最新的行情订单编号
    int64_t latest_order_seq = 0;
    // 储存最新的交易订单编号
    int64_t latest_trade_seq = 0;

    // 配置文件名
    char * dirname;
    // 当前日期
    int datenum;
    // 当前时间
    int64_t time_now;
    // 下一个数据的时间
    int64_t time_next;
    // 股票静态数据数组管理结构体
    SimDataManager* stockinfo_manager;
    // 股票快照数据数组管理结构体
    SimDataManager* snapdata_manager;
    // 股票10档行情数据数组管理结构体
    SimDataManager* leveldata_manager;
    // 逐笔委托数据数组管理结构体
    SimDataManager* tickorder_manager;
    // 逐笔成交数据数据管理结构体
    SimDataManager* ticktrade_manager;

    // 股票静态数据返回链表头节点
    SimStockInfo* stockinfo_head_node;
    // 股票快照数据返回链表头节点
    SimSnapData* snapdata_head_node;
    // 股票10档行情数据返回链表头节点
    SimLevelData* leveldata_head_node;
    // 逐笔委托数据返回链表头节点
    SimTickOrder* tickorder_head_node;
    // 逐笔成交数据返回链表头节点
    SimTickTrade* ticktrade_head_node;

} // namespace Sim

void Sim::initial_generator_data()
{
    tickorder_vec.reserve(512);
    ticktrade_vec.reserve(512);
    
    IniType conf = Read_ini("Trade.ini");
    show_ini(conf);
    const char *dir_ = conf["main"]["backtest_dir_linux"].c_str();
    dirname = new char[256];
    strncpy(dirname, dir_, strlen(dir_));
    printf("data dir:%s\n", dirname);
    datenum = GetIntFromKey(conf["main"], "backtest_date");
    if (datenum > 20300101 || datenum < 20000101)
    {
        printf("error date:%d", datenum);
        throw runtime_error("error date");
    }
    else
        printf("backtest date :%d\n", datenum);
    // 初始化时间为上午九点
    // time_now = datenum * 1000000 * 1000 + 90000 * 1000;
    time_now = 90000 * 1000;
    printf("init time:%ld\n", time_now);
    // 数组指针初始化
    SimDataManager* stockinfo_manager = new SimDataManager;
    memset(stockinfo_manager, 0, sizeof(SimDataManager));
    SimDataManager* snapdata_manager = new SimDataManager;
    memset(snapdata_manager, 0, sizeof(SimDataManager));
    SimDataManager* leveldata_manager = new SimDataManager;
    memset(leveldata_manager, 0, sizeof(SimDataManager));
    SimDataManager* tickorder_manager = new SimDataManager;
    memset(tickorder_manager, 0, sizeof(SimDataManager));
    SimDataManager* ticktrade_manager = new SimDataManager;
    memset(ticktrade_manager, 0, sizeof(SimDataManager));
    // 返回数据头结点指针初始化
    stockinfo_head_node = new SimStockInfo;
    memset(stockinfo_head_node, 0, sizeof(SimStockInfo));
    snapdata_head_node = new SimSnapData;
    memset(snapdata_head_node, 0, sizeof(SimSnapData));
    leveldata_head_node = new SimLevelData;
    memset(leveldata_head_node, 0, sizeof(SimLevelData));
    tickorder_head_node = new SimTickOrder;
    memset(tickorder_head_node, 0, sizeof(SimTickOrder));
    ticktrade_head_node = new SimTickTrade;
    memset(ticktrade_head_node, 0, sizeof(SimTickTrade));
    // 初始化完毕
    printf("stockinfo_head_node:%p\n", stockinfo_head_node);
    printf("SimData initial over\n");
}

unsigned Sim::cancel_order(unsigned order_id)
{
    cancelorder_num++;
    if (order_id > localorder_num)
        printf("error cancel order_id:%u\n", order_id);
    else
    {
        SimOrder *order = localorder_vec[order_id - 1];
        SimTickTrade *ticktrade;
        switch (order->state)
        {
        case SIM_ORDERSTATE_CANCELED:
            printf("error cancel order_id:%u this order is canceled\n", order_id);
            break;
        case SIM_ORDERSTATE_TRADED:
            printf("error cancel order_id:%u this order is traded\n", order_id);
            break;
        case SIM_ORDERSTATE_PARTTRADED:
        case SIM_ORDERSTATE_SENDED:
            order->state = SIM_ORDERSTATE_CANCELED;
            ticktrade = convert_order_struct(order, SIM_TRADETYPE_CANCEL);
            ticktrade_vec.push_back(ticktrade);
            break;
        default:
            printf("error cancel order_id:%u this order is error\n", order_id);
            break;
        }
    }
    return cancelorder_num + 2000;
}

unsigned Sim::insert_order(SimOrder *order)
{
    localorder_num++;
    order->id = localorder_num;
    localorder_vec.push_back(order);
    int codenum = atoi(order->code);
    order_map[codenum].push_back(order);    // 在待撮合成交队列里增加订单
    SimTickOrder *new_tickorder = convert_order_struct(order, false);
    tickorder_vec.push_back(new_tickorder); // 在已发送订单数组里增加订单
    return localorder_num;
}

SimTickOrder *Sim::convert_order_struct(const SimOrder *order, bool reverse)
{
    if (order == NULL)
        return NULL;
    SimTickOrder *tickorder = new SimTickOrder;
    // 更新订单序列号计数
    latest_order_seq++;
    memset(tickorder, 0, sizeof(SimTickOrder));
    if (reverse)
    {
        if (order->side == SIM_ORDERSIDE_BUY)
            tickorder->side = SIM_ORDERSIDE_SELL;
        else
            tickorder->side = SIM_ORDERSIDE_BUY;
    }
    else
        tickorder->side = order->side;
    tickorder->type = order->type;
    tickorder->price = order->price;
    tickorder->qty = order->vol;
    tickorder->seq = latest_order_seq;
    tickorder->exchange_id = order->exchange_id;
    strncpy(tickorder->code, order->code, SIM_CODESIZE);
    return tickorder;
}

SimTickTrade *Sim::convert_order_struct(const SimOrder *order, const char tradetype)
{
    if (order == NULL)
        return NULL;
    // 更新成交序列号计数
    latest_trade_seq++;
    SimTickTrade *ticktrade = new SimTickTrade;
    memset(ticktrade, 0, sizeof(SimTickTrade));
    if (order->side == SIM_ORDERSIDE_BUY)
    {
        ticktrade->bid_no = order->seq;
        if (tradetype == SIM_TRADETYPE_DEAL)
            ticktrade->ask_no = latest_trade_seq++;
    }
    else
    {
        ticktrade->ask_no = order->seq;
        if (tradetype == SIM_TRADETYPE_DEAL)
            ticktrade->bid_no = latest_trade_seq++;
    }
    ticktrade->type = tradetype;
    ticktrade->price = order->price;
    ticktrade->qty = order->vol - order->traded_vol;
    ticktrade->seq = latest_trade_seq;
    ticktrade->exchange_id = order->exchange_id;
    strncpy(ticktrade->code, order->code, SIM_CODESIZE);
    return ticktrade;
}

SimTickTrade *Sim::simulate_trade(const SimTickTrade *ticktrade, SimOrder *order)
{
    if (ticktrade == NULL || order == NULL)
        return NULL;
    if (order->state != SIM_ORDERSTATE_PARTTRADED && order->state != SIM_ORDERSTATE_SENDED)
        return NULL;
    SimTickTrade *new_ticktrade = NULL;
    if (order->side == SIM_ORDERSIDE_BUY)
    {
        if (ticktrade->price < order->price) // 最近一次成交价小于报单价
            ;
        else if (ticktrade->price > order->price) // 最近一次成交价大于报单价
            return NULL;
        else if (ticktrade->bid_no >= order->seq) // 最近一次成交价等于报单价 且买序列号大于本订单序列号
            ;
        else
            return NULL;
        order->traded_vol = order->vol;       // 更新成交量
        order->state = SIM_ORDERSTATE_TRADED; // 更新订单状态
        SimTickOrder *tickorder = convert_order_struct(order, true);
        tickorder_vec.push_back(tickorder); // 为这笔交易生成一笔反向订单
        new_ticktrade = convert_order_struct(order, SIM_TRADETYPE_DEAL);
    }
    else
    {
        if (ticktrade->price > order->price) // 最近一次成交价大于报单价
            ;
        else if (ticktrade->price < order->price) // 最近一次成交价小于报单价
            return NULL;
        else if (ticktrade->ask_no >= order->seq) // 最近一次成交价等于报单价 且买序列号大于本订单序列号
            ;
        else
            return NULL;
        order->traded_vol = order->vol;       // 更新成交量
        order->state = SIM_ORDERSTATE_TRADED; // 更新订单状态
        SimTickOrder *tickorder = convert_order_struct(order, true);
        tickorder_vec.push_back(tickorder); // 为这笔交易生成一笔反向订单
        new_ticktrade = convert_order_struct(order, SIM_TRADETYPE_DEAL);
    }
    return new_ticktrade;
}

void Sim::simulate_trade(const SimTickTrade *ticktrade)
{
    // 初始化
    int codenum = atoi(ticktrade->code);
    if (ticktrade == NULL || codenum == 0 || Sim::order_map[codenum].empty())
        return;
    // 倒序遍历订单 方便删除
    size_t order_num = Sim::order_map[codenum].size();
    for (unsigned i = order_num - 1; i >= 0; i--)
    { // 模拟成交
        SimOrder *simorder = Sim::order_map[codenum][i];
        // 检查订单状态
        if (simorder->state != SIM_ORDERSTATE_SENDED || simorder->state != SIM_ORDERSTATE_PARTTRADED)
        {
            // 从待匹配成交队列里删除这笔订单
            Sim::order_map[codenum].erase(Sim::order_map[codenum].begin() + i);
            continue;
        }
        SimTickTrade *new_ticktrade = simulate_trade(ticktrade, simorder);
        if (new_ticktrade != NULL)
        {
            // 储存新生成的成交数据
            ticktrade_vec.push_back(new_ticktrade);
            // 储存订单成交信息
            SimTradeReport *new_tradereport_ptr = new SimTradeReport{simorder, new_ticktrade, NULL};
            // 从待匹配成交队列里删除这笔订单
            Sim::order_map[codenum].erase(Sim::order_map[codenum].begin() + i);
        }
    }
}

void Sim::load_data(bool is_am)
{   
    /*
    股票信息列表： "/home/wiz/data/uplimit_list.csv"
    快照行情:     "/home/wiz/data/L2_data/2017/01/03/am_hq_snap_spot.csv"
    十档行情:     "/home/wiz/data/L2_data/2017/01/03/am_snap_level_spot.csv"
    订单流:       "/home/wiz/data/L2_data/2017/01/03/am_hq_order_spot.csv"
    成交流:       "/home/wiz/data/L2_data/2017/01/03/am_hq_trade_spot.csv"
    */
    char am[3] = "am", pm[3] = "pm", *timetype, dir_stockinfo, dir_data;
    char filename[256];
    int total_num;
    if (is_am)
    {   //上午需载入股票信息数据
        timetype = am;
        sprintf(filename,"%s/uplimit_list.csv", dirname);
        stockinfo_manager = read_stock_info_from_file(filename, datenum);
        printf("stockinfo_head_node:%p\n", stockinfo_head_node);
    }
    else 
    {
        timetype = pm;
    }
    printf(">>>>>load %d %s data start>>>>>\n", datenum, timetype);
    int year = datenum / 10000;
    int month = datenum % 10000 / 100;
    int date = datenum % 100;

    // 载入数据
    sprintf(filename,"%s/L2_data/%04d/%02d/%02d/%s_hq_snap_spot.csv", dirname, year, month, date, timetype);
    snapdata_manager = read_snap_data_from_file(filename);
    sprintf(filename,"%s/L2_data/%04d/%02d/%02d/%s_snap_level_spot.csv", dirname, year, month, date, timetype);
    leveldata_manager = read_level_data_from_file(filename);
    sprintf(filename,"%s/L2_data/%04d/%02d/%02d/%s_hq_order_spot.csv", dirname, year, month, date, timetype);
    tickorder_manager = read_tick_order_from_file(filename);
    sprintf(filename,"%s/L2_data/%04d/%02d/%02d/%s_hq_trade_spot.csv", dirname, year, month, date, timetype);
    ticktrade_manager = read_tick_trade_from_file(filename);

    printf(">>>>>load %d %s data over<<<<<\n", datenum, timetype);
}

bool Sim::check_is_need_load_data()
{
    if (tickorder_manager->current != tickorder_manager->end &&
        ticktrade_manager->begin == NULL &&
        snapdata_manager->begin == NULL &&
        leveldata_manager->begin == NULL)
        return true;
    else
        return false;
}

int64_t Sim::Get_Time()
{
    return time_now;
}

int Sim::Get_Timenum()
{
    return (time_now % 1000000000) / 1000;
}

double Sim::Get_Clock()
{
    double now = (time_now % 1000000000) / 1000.0;
    return now;
}

const SimStockInfo* Sim::Get_StockInfo()
{   
    SimStockInfo* stockinfo_ptr = (SimStockInfo*) stockinfo_manager->current;
    SimStockInfo* return_ptr = stockinfo_head_node, *last_ptr = return_ptr;
    return_ptr->next = NULL;
    if (stockinfo_ptr == NULL)
    {
        return return_ptr;
    }
    while (stockinfo_ptr != stockinfo_manager->end)
    {   // 若当前数据时间小于等于系统时间 就返回当前指针 再++当前指针   
        last_ptr->next = stockinfo_ptr;    
        last_ptr = stockinfo_ptr;
        ++stockinfo_ptr;
    }
    return_ptr->next = NULL;
    return return_ptr;
}

const SimTickOrder* Sim::Get_TickOrder()
{   
    SimTickOrder* tickorder_ptr = (SimTickOrder*) tickorder_manager->current;
    SimTickOrder* return_ptr = tickorder_head_node, *last_ptr = return_ptr;
    return_ptr->next = NULL;
    if (tickorder_ptr == NULL)
    {   // 数据未初始化
        return return_ptr;
    }
    if (tickorder_ptr == tickorder_manager->end)
    {   // 当前数据指针移动到数组末尾 释放内存
        // if (tickorder_manager->begin != NULL)
        // {   
        //     SimTickOrder* tickorder_array = (SimTickOrder*)tickorder_manager->begin;
        //     delete[] tickorder_array;
        // }
        // memset(tickorder_manager, 0, sizeof(SimDataManager));
        // printf("release SimTickOrder Array\n");
        return return_ptr;
    }
    while (tickorder_ptr != tickorder_manager->end &&
           tickorder_ptr->data_time <= time_now)
    {   // 若当前数据时间小于等于系统时间 就返回当前指针 再++当前指针   
        last_ptr->next = tickorder_ptr;    
        last_ptr = tickorder_ptr;
        latest_order_seq = tickorder_ptr->seq;
        ++tickorder_ptr;
    }
    // 记录本次行情推送哪里
    tickorder_manager->current = tickorder_ptr;
    // 更新下一次行情更新时间
    if (tickorder_ptr != tickorder_manager->end)
    {   // 若未到达末尾 更新时间
        update_time_next(tickorder_ptr->data_time);
    }
    // 读取新生成的成交数据
    while (tickorder_read_num < tickorder_vec.size())
    {   
        tickorder_ptr = tickorder_vec[tickorder_read_num];
        last_ptr->next = tickorder_ptr;
        last_ptr = tickorder_ptr;
        tickorder_read_num++;
    }
    return_ptr->next = NULL;
    return return_ptr;
}

const SimTickTrade* Sim::Get_TickTrade()
{   
    SimTickTrade* ticktrade_ptr = (SimTickTrade*) ticktrade_manager->current;
    SimTickTrade* return_ptr = ticktrade_head_node, *last_ptr = return_ptr;
    return_ptr->next = NULL;
    if (ticktrade_ptr == NULL)
    {   // 数据未初始化
        return return_ptr;
    }
    if (ticktrade_ptr == ticktrade_manager->end)
    {   // 当前数据指针移动到数组末尾 释放内存
        // if (ticktrade_manager->begin != NULL)
        // {   
        //     SimTickTrade* ticktrade_array = (SimTickTrade*)ticktrade_manager->begin;
        //     delete[] ticktrade_array;
        // }
        // memset(ticktrade_manager, 0, sizeof(SimDataManager));
        // printf("release SimTickTrade Array\n");
        return return_ptr;
    }
    while (ticktrade_ptr != ticktrade_manager->end &&
           ticktrade_ptr->data_time <= time_now)
    {   // 若当前数据时间小于等于系统时间 就返回当前指针 再++当前指针   
        last_ptr->next = ticktrade_ptr;    
        last_ptr = ticktrade_ptr;
        latest_trade_seq = ticktrade_ptr->seq;
        simulate_trade(ticktrade_ptr);
        ++ticktrade_ptr;
    }
    // 记录本次行情推送哪里
    ticktrade_manager->current = ticktrade_ptr;
    // 更新下一次行情更新时间
    if (ticktrade_ptr != ticktrade_manager->end)
    {   // 若未到达末尾 更新时间
        update_time_next(ticktrade_ptr->data_time);
    }
    // 读取新生成的成交数据
    while (ticktrade_read_num < ticktrade_vec.size())
    {   
        ticktrade_ptr = ticktrade_vec[ticktrade_read_num];
        last_ptr->next = ticktrade_ptr;
        last_ptr = ticktrade_ptr;
        ticktrade_read_num++;
    }
    return_ptr->next = NULL;
    return return_ptr;
}

const SimSnapData* Sim::Get_SnapData()
{   
    SimSnapData* snapdata_ptr = (SimSnapData*) snapdata_manager->current;
    SimSnapData* return_ptr = snapdata_head_node, *last_ptr = return_ptr;
    return_ptr->next = NULL;
    if (snapdata_ptr == NULL)
    {   // 数据未初始化
        return return_ptr;
    }
    // if (snapdata_ptr == snapdata_manager->end)
    // {   // 当前数据指针移动到数组末尾 释放内存
        // if (snapdata_manager->begin != NULL)
        // {   
        //     SimSnapData* snapdata_array = (SimSnapData*)snapdata_manager->begin;
        //     delete[] snapdata_array;
        // }
        // memset(snapdata_manager, 0, sizeof(SimDataManager));
        // printf("release SimSnapData Array\n");
    //     return return_ptr;
    // }
    while (snapdata_ptr != snapdata_manager->end &&
           snapdata_ptr->data_time <= time_now)
    {   // 若当前数据时间小于等于系统时间 就返回当前指针 再++当前指针   
        last_ptr->next = snapdata_ptr;    
        last_ptr = snapdata_ptr;
        ++snapdata_ptr;
    }
    // 记录本次行情推送哪里
    snapdata_manager->current = snapdata_ptr;
    // 更新下一次行情更新时间
    if (snapdata_ptr != snapdata_manager->end)
    {   // 若未到达末尾 更新时间
        update_time_next(snapdata_ptr->data_time);
    }
    return_ptr->next = NULL;
    return return_ptr;
}

const SimLevelData* Sim::Get_LevelData()
{   
    SimLevelData* leveldata_ptr = (SimLevelData*) leveldata_manager->current;
    SimLevelData* return_ptr = leveldata_head_node, *last_ptr = return_ptr;
    return_ptr->next = NULL;
    if (leveldata_ptr == NULL)
    {   // 数据未初始化
        return return_ptr;
    }
    // if (leveldata_ptr == leveldata_manager->end)
    // {   // 当前数据指针移动到数组末尾 释放内存
        // if (leveldata_manager->begin != NULL)
        // {   
        //     SimLevelData* leveldata_array = (SimLevelData*)leveldata_manager->begin;
        //     delete[] leveldata_array;
        // }
        // memset(leveldata_manager, 0, sizeof(SimDataManager));
        // printf("release SimLevelData Array\n");
    //     return return_ptr;
    // }
    while (leveldata_ptr != leveldata_manager->end &&
           leveldata_ptr->data_time <= time_now)
    {   // 若当前数据时间小于等于系统时间 就返回当前指针 再++当前指针   
        last_ptr->next = leveldata_ptr;    
        last_ptr = leveldata_ptr;
        ++leveldata_ptr;
    }
    // 记录本次行情推送哪里
    leveldata_manager->current = leveldata_ptr;
    // 更新下一次行情更新时间
    if (leveldata_ptr != leveldata_manager->end)
    {   // 若未到达末尾 更新时间
        update_time_next(leveldata_ptr->data_time);
    }
    return_ptr->next = NULL;
    return return_ptr;
}

void Sim::update_time()
{
    // printf("update time  %lld  to  %lld\n", time_now, time_next);
    time_now = time_next;
}

void Sim::update_time_next(int64_t data_time)
{
    // 若当前数据时间小于当前系统下一次推送行情时间 更新 系统下一次推送行情时间 为 当前数据时间
    if ( time_next > data_time )
        time_next = data_time;
    // 系统下一次推送行情时间小于下一个数据的时间 设置 系统下一次推送行情时间 为 下一个数据的时间
    else if (time_next <= time_now)
        time_next = data_time;
}