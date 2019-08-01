#include "Quote_Spi.h"
#include <stdio.h>

void MyQuoteSpi::OnError(ErrorInfo *error_info, bool is_last)
{
    IsErrorRspInfo(error_info);
    // SendMsg(XTP_LOG_LEVEL_ERROR, "quote", error_info->error_msg, 2);
}

MyQuoteSpi::MyQuoteSpi()
{
}

MyQuoteSpi::~MyQuoteSpi()
{
}

void MyQuoteSpi::OnDisconnected(int reason)
{
    //cout << "--->>> " << "OnDisconnected quote" << endl;
    //cout << "--->>> Reason = " << reason << endl;
    //断线后，可以重新连接
    //重新连接成功后，需要重新向服务器发起订阅请求

    //断线，向监控端发送断线日志
    char strlog[1024] = { 0 };
    sprintf(strlog, "Disconnected from quote.");
    // SendMsg(XTP_LOG_LEVEL_ERROR, "Disconnected quote", strlog, 2);
    char s[256];
    sprintf(s, "--->>> Reason = %d", reason);
    // SendMsg(XTP_LOG_LEVEL_ERROR, "Disconnected quote", s, 2);
    // QUOTE_STATUS = false;
}

void MyQuoteSpi::OnQueryAllTickers(StockStaticInfo * ticker_info, ErrorInfo * error_info, bool is_last)
{   // 查询可交易合约的应答
    static int call_times = 0, call_times_flag = 1;
    call_times ++;
    if (call_times > call_times_flag)
    {   
        printf("idx:%08d code:%s Market:%d preclose:%.2lf\tuplimit:%.2lf\n", call_times_flag,
        ticker_info->ticker, ticker_info->exchange_id,  ticker_info->pre_close_price, ticker_info->upper_limit_price);
        call_times_flag *= 2;
    }
}

void MyQuoteSpi::OnDepthMarketData(DepthMarketData * market_data)
{   // 深度行情通知 包含买一卖一队列
    static int call_times = 0, call_times_flag = 1;
    call_times ++;
    if (call_times > call_times_flag)
    {   
        printf("code:%s Market:%d preclose:%.2lf\tuplimit:%.2lf\tb1pr:%.2lf b1vol_queue:%d\n", 
            market_data->ticker, market_data->exchange_id,  market_data->pre_close_price, 
            market_data->upper_limit_price, market_data->bid[0], market_data->bid1_count);
            for (int i=0; i<50; i++)
            {   
                if (market_data->bid1_qty[i] > 0)
                    printf("%ld|", market_data->bid1_qty[i]);
            }
            printf("\n");
        call_times_flag *= 2;
    }
}

void MyQuoteSpi::OnTickByTick(TickByTick *tbt_data)
{ // 退订逐笔行情应答，包括股票、指数和期权
    static int call_times_order = 0, call_times_order_flag = 1;
    static int call_times_trade = 0, call_times_trade_flag = 1;
    if (tbt_data->ticker == NULL)
    {   
        printf("spi get NULL tickbytick\n");
        return;
    }
    if (tbt_data->exchange_id == EXCHANGE_SZ &&
        (tbt_data->ticker[0] == '0' || tbt_data->ticker[0] == '3'))
        ;//验证深圳代码  00 or 30
    //else if (tbt_data->exchange_id == XTP_EXCHANGE_SH &&
        //tbt_data->ticker[0] == '6')
        //;//验证上海代码 60
    else
        return;
    // Orbook_Ptr orbook_ptr = find_orbook_ptr(tbt_data->ticker);
    // if (orbook_ptr == NULL)
    //     return;
    switch(tbt_data->type)
    {
        case TBT_ENTRUST:
            call_times_order++;
            if ( call_times_order > call_times_order_flag )
            {
                printf("idx:%08d code:%s Market:%d seq:%09ld price:%.2lf vol:%ld side:%c type:%c\n", call_times_order,
                tbt_data->ticker, tbt_data->exchange_id, tbt_data->order.seq, tbt_data->order.price,
                tbt_data->order.qty, tbt_data->order.side, tbt_data->order.type);
                call_times_order_flag *= 2;
            }
            break;
        case TBT_TRADE:
            call_times_trade++;
            if ( call_times_trade > call_times_trade_flag )
            {   
                printf("idx:%08d code:%s Market:%d seq:%09ld seq_buy:%09ld seq_sell:%09ld price:%.2lf vol:%ld type:%c\n", call_times_trade, 
                tbt_data->ticker, tbt_data->exchange_id, tbt_data->trade.seq, tbt_data->trade.bid_no, tbt_data->trade.ask_no,
                tbt_data->trade.price, tbt_data->trade.qty, tbt_data->trade.type);
                call_times_trade_flag *= 2;
            }   
            break;
    }

    //处理委托流和成交流
    //SendMsg(XTP_LOG_LEVEL_INFO, "Quote", s);
    //Get data
      
    
}

bool MyQuoteSpi::IsErrorRspInfo(ErrorInfo *pRspInfo)
{
    return true;
}
