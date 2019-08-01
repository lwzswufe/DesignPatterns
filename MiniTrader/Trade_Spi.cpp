#include "Trade_Spi.h"
#include <iostream>
#include <cstring>
#include <iomanip>
#include <stdio.h>
#include <time.h>
#include <sys/timeb.h>
#include <unistd.h>
#include <sstream>
#include <map>
using namespace std;

// 交易连接状态
extern bool TRADE_STATUS;
// 是否获取所有持仓
extern bool GET_ALL_POSITION;
// 是否获取所有订单
extern bool GET_ALL_ORDER;

MyTradeSpi::MyTradeSpi()
{
    ;
}

MyTradeSpi::~MyTradeSpi()
{
    ;
}

void MyTradeSpi::OnOrderEvent(OrderInfo *order_info)
{   // 每次订单状态更新时，都会被调用
    // Order_Ptr order_ptr = get_order_info_from_xtp(order_info);
    // if (order_ptr != NULL)
    // {   
    //     Data::order_ptr_deque.push_back(order_ptr);
    // }
}

void MyTradeSpi::OnTradeEvent(TradeReport *trade_info)
{   // 订单有成交发生的时候，会被调用
    // if (trade_info->business_type == BUSINESS_TYPE_CASH &&  ///<普通股票业务（股票买卖，ETF买卖等）
    //      ( trade_info->side == SIDE_BUY ||                  ///买（新股申购，ETF买，配股，信用交易中担保品买）
    //        trade_info->side == SIDE_MARGIN_TRADE)           ///融资买入
    //     )
    // { 
    //     Order_Ptr order_ptr = find_order(trade_info->order_xtp_id);
    //     char s[LOGSIZE];
    //     if (order_ptr != NULL)
    //     {   
    //         order_ptr->traded_vol += trade_info->quantity;
    //         if (order_ptr->state == OrderState::SENDED)
    //         {
    //             if (order_ptr->vol == order_ptr->traded_vol)
    //             {
    //                 order_ptr->state = OrderState::TRADED;
    //             }
    //             else
    //             {
    //                 order_ptr->state = OrderState::PARTTRADE;
    //             }
    //             Data::order_ptr_deque.push_back(order_ptr);
    //             sprintf(s, "!!!!! find %s_order_id:%lld traded  !!!!!!",
    //             trade_info->ticker, trade_info->order_xtp_id);
    //             SendMsg(_LOG_LEVEL_TRACE, "TRADE", s);
    //         }
    //     }
    // }
    // updateTradeNum();
    // //收到成交回报后，向监控端发送日志
    // char strlog[1024] = { 0 };
    // sprintf(strlog, "%lld trade event.", trade_info->order_xtp_id);
    // SendMsg(_LOG_LEVEL_INFO, "TradeEvent", strlog);
}

void MyTradeSpi::OnQueryOrder(QueryOrderRsp * order_info, bool is_last)
{
    // Order_Ptr order_ptr = get_order_info_from_xtp(order_info);
    // if (order_ptr != NULL)
    //     Data::order_ptr_deque.push_back(order_ptr);
    // if (is_last)
    // {
    //     GET_ALL_ORDER = true;
    //     SendMsg(_LOG_LEVEL_TRACE, "trade", "successful get all Order");
    // }
}

void MyTradeSpi::OnQueryPosition(QueryStkPositionRsp * investor_position, bool is_last)
{
    // Position_Ptr position_ptr;
    // int account_id = 1; //// 默认设置 后续版本改进
    // position_ptr = get_position_info_from_xtp(investor_position, account_id);
    // if ( position_ptr != NULL )
    //     Data::position_ptr_deque.push_back(position_ptr);
    // char s[LOGSIZE];
    // sprintf(s, "new_position: %s vol:%d", position_ptr->code, position_ptr->sellable_qty);
    // SendMsg(_LOG_LEVEL_TRACE, "trade_spi", s);

    // if ( is_last )
    // {
    //     GET_ALL_POSITION = true;
    //     SendMsg(_LOG_LEVEL_TRACE, "trade_spi", "successful get all position");
    // }
}

void MyTradeSpi::setUserAPI(TradeApi* pApi) 
{ 
    pUserApi = pApi; 
}