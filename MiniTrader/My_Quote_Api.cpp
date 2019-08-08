#include <unistd.h>
#include <pthread.h>
#include <memory>
#include "My_Quote_Api.h"
#include "sim_quote_convert.h"
#include "simulate_trade.h"
#include "Strategy.h"

QuoteApi * QuoteApi::CreateQuoteApi(const char *ini_file)
{   
    return new MyQuoteApi(ini_file);
}

void MyQuoteApi::RegisterSpi(QuoteSpi *spi)
{
    this->_spi = spi;
}

int MyQuoteApi::Login()
{
    return 0;
}

int MyQuoteApi::Logout()
{
    this->close_push_quote_thread();
    return 0;
}

MyQuoteApi::MyQuoteApi(const char* ini_file)
{   
    this->quote_pthread_status = false;
    this->quote_param.push_status = true;
    this->quote_param.clock = 0;
    this->quote_param.ini_file = ini_file;
    this->quote_param.spi = NULL;
}

void MyQuoteApi::start_push_quote_thread()
{   
    if (this->quote_pthread_status)
    {
        printf("push_quote_thread has started\n");
        return;
    }
    pthread_t pid_quote;      
    int pthread_ret;
    this->quote_param.spi = this->_spi;
    pthread_ret = pthread_create(&pid_quote, NULL, task_push_quote, &(this->quote_param));         
    if(pthread_ret!=0)
    {
        printf("Create pthread_quote error!\n");
    }
    else 
    {   
        this->quote_pthread_status = true;
        printf("Create pthread_quote successful!\n");
    }
}

void MyQuoteApi::close_push_quote_thread()
{
    this->quote_param.push_status = false;
    printf("stop pthread_quote\n");
    this->quote_pthread_status = false;
}

int MyQuoteApi::QueryAllTickers(EXCHANGE_TYPE exchange_id)
{   
    this->start_push_quote_thread();
    return 0;
}


double MyQuoteApi::GetClock()
{   
    return ApiTimenum;
}

int ApiTimenum = 0;

int GetTimenum()
{
    return ApiTimenum;
}

// 行情推送线程
void *task_push_quote(void* arg)
{   
    Quote_Push_Param_Struct *param = (Quote_Push_Param_Struct*)arg;
    // printf("ini_file:%s\n", param->ini_file);
    Sim::initial_generator_data();
    Sim::quote_start = true;
    printf("SIM::quote_start\n");

    double * clock_ptr = &(param->clock);
    QuoteSpi* spi = param->spi;
    // xtp数据指针
    bool is_last, is_am = true;
    ErrorInfo errorinfo;
    StockStaticInfo stockinfo, *stockinfo_ptr = &stockinfo;
    TickByTickStruct tickbytick, *tick_ptr = &tickbytick;
    DepthData depthdata, *depthdata_ptr = &depthdata;
    // 本地数据指针
    const SimStockInfo* info_ptr;
    const SimTickOrder* tickorder_ptr;
    const SimTickTrade* ticktrade_ptr;
    const SimSnapData*  snap_ptr;
    const SimLevelData* level_ptr;
    // 线程id
    pthread_t pid = pthread_self();
    printf("%s start\n", __func__);
    // 推送股票信息
    Sim::load_data(is_am);
    info_ptr = Sim::Get_StockInfo();
    while (info_ptr->next != NULL)
    {   
        info_ptr = info_ptr->next;
        test_stock_info(info_ptr);
        convert_stockinfo(info_ptr, stockinfo_ptr, &is_last);
        spi->OnQueryAllTickers(stockinfo_ptr, &errorinfo, is_last);
    }
    // 开始推送行情
    *clock_ptr = Sim::Get_Clock();
    printf("quote_push_thread start time:%.3lf\n", *clock_ptr);
    while(*clock_ptr < 150000.00 && param->push_status)
    {   // 判断是否需要载入下午的数据
        if (Sim::check_is_need_load_data())
        {   
            if (is_am)
            {
                is_am = false;
                Sim::load_data(is_am);
            }
            else 
                break;
        }
        ticktrade_ptr = Sim::Get_TickTrade();
        tickorder_ptr = Sim::Get_TickOrder();
        // 推送委托流
        while(tickorder_ptr->next != NULL)
        {   
            tickorder_ptr = tickorder_ptr->next;
            test_tick_order(tickorder_ptr);
            Strategy::march_order(tickorder_ptr);
            Strategy::send_order(tickorder_ptr);
            convert_tickorder(tickorder_ptr, tick_ptr);
            spi->OnTickByTick(tick_ptr);
        } 
        // 推送成交流
        while(ticktrade_ptr->next != NULL)
        {   
            ticktrade_ptr = ticktrade_ptr->next;
            test_tick_trade(ticktrade_ptr);
            convert_ticktrade(ticktrade_ptr, tick_ptr);
            spi->OnTickByTick(tick_ptr);
        }
        // 推送深度行情数据
        snap_ptr = Sim::Get_SnapData();
        level_ptr = Sim::Get_LevelData();
        while(snap_ptr->next != NULL && level_ptr->next != NULL)
        {   
            snap_ptr = snap_ptr->next;
            level_ptr = level_ptr->next;
            test_snap_data(snap_ptr);
            test_level_data(level_ptr);
            convert_depthdata(snap_ptr, level_ptr, depthdata_ptr);
            spi->OnDepthMarketData(depthdata_ptr);
        }
        // 设置时间
        *clock_ptr = Sim::Get_Clock();
        ApiTimenum = Sim::Get_Timenum();
        Sim::update_time();
    }
    printf("%s end\n", __func__);
    pthread_exit(0);
}