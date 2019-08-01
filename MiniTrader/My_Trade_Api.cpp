#include <unistd.h>
#include <pthread.h>
#include <memory>
#include "My_Trade_Api.h"

TradeApi * TradeApi::CreateTradeApi(uint8_t client_id)
{   
    return new MyTradeApi(client_id);
}

void MyTradeApi::RegisterSpi(TradeSpi *spi)
{
    this->_spi = spi;
}

uint64_t  MyTradeApi::Login()
{
    return 0;
}

int MyTradeApi::Logout()
{
    return 0;
}

MyTradeApi::MyTradeApi(uint8_t client_id)
{   
    sprintf(this->ini_file, "Trade.ini");
    this->trade_pthread_status = false;
    this->trade_param.push_status = true;
    this->trade_param.clock = 0;
    this->trade_param.ini_file = this->ini_file;
    this->trade_param.spi = NULL;
}


void MyTradeApi::start_push_trade_thread()
{   
    if (this->trade_pthread_status)
    {
        printf("push_trade_thread has started\n");
        return;
    }
    pthread_t pid_trade;      
    int pthread_ret;
    this->trade_param.spi = this->_spi;
    pthread_ret = pthread_create(&pid_trade, NULL, task_push_trade, &(this->trade_param));         
    if(pthread_ret!=0)
    {
        printf("Create pthread_trade error!\n");
    }
    else 
    {   
        this->trade_pthread_status = true;
        printf("Create pthread_trade successful!\n");
    }
}

void MyTradeApi::close_push_trade_thread()
{
    this->trade_param.push_status = false;
    printf("stop pthread_trade\n");
    this->trade_pthread_status = false;
}


// 行情推送线程
void *task_push_trade(void* arg)
{   
    Trade_Push_Param_Struct *param = (Trade_Push_Param_Struct*)arg;
    printf("ini_file:%s\n", param->ini_file);
    double * clock_ptr = &(param->clock);
    TradeSpi* spi = param->spi;
    Trade_Generator generator(param->ini_file);
    // xtp数据指针
    bool is_last, is_am = true;
    ErrorInfo errorinfo;
    // 本地数据指针
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