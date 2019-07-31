#include <unistd.h>
#include <pthread.h>
#include <memory>
#include "My_Quote_Api.h"
#include "local_quote_convert.h"

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

int ApiTimenum;

int GetTimenum()
{
    return ApiTimenum;
}

// 行情推送线程
void *task_push_quote(void* arg)
{   
    Quote_Push_Param_Struct *param = (Quote_Push_Param_Struct*)arg;
    printf("ini_file:%s\n", param->ini_file);
    double * clock_ptr = &(param->clock);
    QuoteSpi* spi = param->spi;
    Quote_Generator generator(param->ini_file);
    // xtp数据指针
    bool is_last, is_am = true;
    ErrorInfo errorinfo;
    StockStaticInfo stockinfo, *stockinfo_ptr = &stockinfo;
    TickByTickStruct tickbytick, *tick_ptr = &tickbytick;
    DepthData depthdata, *depthdata_ptr = &depthdata;
    // 本地数据指针
    const StockInfo* info_ptr;
    const TickOrder* tickorder_ptr;
    const TickTrade* ticktrade_ptr;
    const SnapData* snap_ptr;
    const LevelData* level_ptr;
    // 线程id
    pthread_t pid = pthread_self();
    printf("%s start\n", __func__);
    // 推送股票信息
    generator.load_data(is_am);
    info_ptr = generator.Get_StockInfo();
    while (info_ptr != NULL)
    {   
        convert_stockinfo(info_ptr, stockinfo_ptr, &is_last);
        spi->OnQueryAllTickers(stockinfo_ptr, &errorinfo, is_last);
        info_ptr = generator.Get_StockInfo();
    }
    // 开始推送行情
    *clock_ptr = generator.Get_Clock();
    printf("quote_push_thread start time:%.3lf\n", *clock_ptr);
    while(*clock_ptr < 150000.00 && param->push_status)
    {   // 判断是否需要载入下午的数据
        if (generator.check_is_need_load_data())
        {   
            if (is_am)
            {
                is_am = false;
                generator.load_data(is_am);
            }
        }
        // 推送行情数据
        tickorder_ptr = generator.Get_TickOrder();
        while(tickorder_ptr != NULL)
        {   
            convert_tickorder(tickorder_ptr, tick_ptr);
            spi->OnTickByTick(tick_ptr);
            tickorder_ptr = generator.Get_TickOrder();
        }

        ticktrade_ptr = generator.Get_TickTrade();
        while(ticktrade_ptr != NULL)
        {   
            convert_ticktrade(ticktrade_ptr, tick_ptr);
            spi->OnTickByTick(tick_ptr);
            ticktrade_ptr = generator.Get_TickTrade();
        }
        
        snap_ptr = generator.Get_SnapData();
        level_ptr = generator.Get_LevelData();
        while(snap_ptr != NULL && level_ptr != NULL)
        {
            convert_depthdata(snap_ptr, level_ptr, depthdata_ptr);
            spi->OnDepthMarketData(depthdata_ptr);
            snap_ptr = generator.Get_SnapData();
            level_ptr = generator.Get_LevelData();
        }
        // 设置时间
        *clock_ptr = generator.Get_Clock();
        ApiTimenum = generator.Get_Timenum();
        // SetCurrentTime(generator.Get_Time());
        generator.update_time();
    }
    printf("%s end\n", __func__);
    pthread_exit(0);
}