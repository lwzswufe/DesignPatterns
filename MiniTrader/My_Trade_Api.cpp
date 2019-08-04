#include <unistd.h>
#include <pthread.h>
#include <memory>
#include "My_Trade_Api.h"
#include "simulate_trade.h"
#include "sim_trade_convert.h"

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
    SimOrder * simorder = new SimOrder;
    convert_order_struct(order, simorder);
    uint64_t order_id = Sim::insert_order(simorder);
    order->order__id = order_id;
    return order_id;
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
    
    // xtp数据指针
    bool is_last, is_am = true;
    ErrorInfo errorinfo;
    TradeReport tradereport, *trade_info = &tradereport;
    OrderInfo orderinfo, *order_info = &orderinfo;
    // 本地数据指针
    const SimTradeReport* sim_tradereport;
    const SimOrder* simorder;
    vector<SimOrder*> traded_order_vec;
    // 线程id
    pthread_t pid = pthread_self();
    while(!Sim::quote_start)
    {
        sleep(1);
    }
    printf("%s start\n", __func__);
    while(Sim::quote_start)
    {   // 遍历股票
        while (Sim::tradereport_num < Sim::tradereport_vec.size())
        {
            sim_tradereport = Sim::tradereport_vec[Sim::tradereport_num];
            Sim::tradereport_num++;
            convert_trade_struct(sim_tradereport, trade_info);
            spi->OnTradeEvent(trade_info);
            convert_order_struct(sim_tradereport->order, order_info);
            spi->OnOrderEvent(order_info);
        }
        sleep(1);
    }
    printf("%s end\n", __func__);
    pthread_exit(0);
}