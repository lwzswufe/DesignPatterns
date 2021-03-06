#include <unistd.h>
#include <pthread.h>
#include <memory>
#include "My_Trade_Api.h"
#include "simulate_trade.h"
#include "sim_trade_convert.h"
#include "Strategy.h"

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
    printf("trade_api login\n");
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

int MyTradeApi::QueryOrders(const QueryOrderReq *query_param)
{
    return 0;
}

int MyTradeApi::QueryPosition(const char *ticker)
{   
    return 0;
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
    QueryStkPositionRsp positionrsq, *position_info=&positionrsq;
    // 本地数据指针
    const SimTradeReport* sim_tradereport;
    const SimOrder* simorder;
    const SimPosition* simposition;
    vector<SimOrder*> traded_order_vec;
    // 线程id
    pthread_t pid = pthread_self();
    printf("wait for start quote\n");
    while(!Sim::quote_start)
    {
        sleep(1);
    }
    printf("%s start\n", __func__);
    while(Sim::quote_start)
    {   // 检查成交回报是否有更新 撤单 成交
        while (Sim::tradereport_num < Sim::tradereport_vec.size())
        {
            sim_tradereport = Sim::tradereport_vec[Sim::tradereport_num];
            Sim::tradereport_num++;
            convert_trade_struct(sim_tradereport, trade_info);
            spi->OnTradeEvent(trade_info);
            convert_order_struct(sim_tradereport->order, order_info);
            spi->OnOrderEvent(order_info);
        }
        // 订单查询推送
        simorder = Sim::Get_Order();
        while(simorder->next != NULL)
        {
            simorder = simorder->next;
            convert_order_struct(simorder, order_info);
            is_last = simorder->next == NULL;
            spi->OnQueryOrder(order_info, is_last);
        } 
        // 持仓信息推送
        simposition = Sim::Get_Position();
        while(simposition->next != NULL)
        {
            simposition = simposition->next;
            // test_position(simposition);
            convert_position_struct(simposition, position_info);
            is_last = simposition->next == NULL;
            spi->OnQueryPosition(position_info, is_last);
        }
        sleep(1);
    }
    printf("%s end\n", __func__);
    pthread_exit(0);
}