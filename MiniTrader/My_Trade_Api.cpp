#include <unistd.h>
#include <pthread.h>
#include <memory>
#include "My_Trade_Api.h"
#include "generator_global.h"
#include "local_trade_convert.h"

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

uint64_t MyTradeApi::InsertOrder(OrderInsertInfo *order)
{   
    LocalOrder * localorder = new LocalOrder;
    localorder = convert_order_struct(order);
    return Sim::insert_order(localorder);
}

uint64_t MyTradeApi::CancelOrder(const uint64_t order__id)
{   
    return Sim::cancel_order(order__id);
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


// 交易订单模拟成交线程
void *task_push_trade(void* arg)
{   
    Trade_Push_Param_Struct *param = (Trade_Push_Param_Struct*)arg;
    printf("ini_file:%s\n", param->ini_file);
    double * clock_ptr = &(param->clock);
    TradeSpi* spi = param->spi;
    TickTrade* latest_ticktrade, new_ticktrade;
    LocalOrder* localorder;
    // xtp数据指针
    bool is_last, is_am = true;
    ErrorInfo errorinfo;
    // 本地数据指针
    // 线程id
    pthread_t pid = pthread_self();
    while(!Sim::quote_start)
    {
        sleep(1);
    }
    printf("%s start\n", __func__);
    while(Sim::quote_start)
    {
        for (auto iter: Sim::lastest_ticktrade_map)
        {
            if (iter.first == 0)
                continue;
            latest_ticktrade = Sim::get_lastest_ticktrade(iter.first);
            if (latest_ticktrade == NULL)
                continue;
            // 遍历股票
            for (auto localorder: Sim::order_map[iter.first])
            {   // 模拟成交
                new_ticktrade = Sim::simulate_trade(latest_ticktrade, localorder);
                // 若成交
                if (new_ticktrade != NULL);
                {   // 推送成交事件
                    spi->OnTradeEvent();
                    spi->OnOrderEvent();
                }
            }
        }
        sleep(1);
    }
    printf("%s end\n", __func__);
    pthread_exit(0);
}