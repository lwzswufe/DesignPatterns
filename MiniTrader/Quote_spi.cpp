#include "Quote_spi.h"
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

}

void MyQuoteSpi::OnDepthMarketData(DepthMarketData * market_data, int64_t bid1_qty[], int32_t bid1_count, 
                                                             int32_t max_bid1_count, int64_t ask1_qty[], 
                                                             int32_t ask1_count, int32_t max_ask1_count)
{   // 深度行情通知 包含买一卖一队列
   
}

void MyQuoteSpi::OnTickByTick(TickByTick *tbt_data)
{ // 退订逐笔行情应答，包括股票、指数和期权
    if (tbt_data->ticker == NULL)
        return;
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
        //order action
        //get_orderbook_data(tbt_data, orbook_ptr);
        break;
        case TBT_TRADE:
        //get_tradebook_data(tbt_data, orbook_ptr);
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
