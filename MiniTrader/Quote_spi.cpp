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
    static int sh_stock_num = 0, sz_stock_num = 0, ST_stock_num = 0;
    static int err_num = 0;

    // if (check_code(ticker_info))
    // {   
    //     Code_Ptr code = ticker_info->ticker;
    //     if (ticker_info->pre_close_price < 0.01 ||
    //         ticker_info->upper_limit_price / ticker_info->pre_close_price < 1.090)
    //     {   
    //         ST_stock_num++;
    //         if (ST_stock_num < 5)
    //         {
    //             char s[256];
    //             sprintf(s, "find ST stock:%s preclose:%.2lf uplimit:%.2lf", 
    //                     code, ticker_info->upper_limit_price, ticker_info->pre_close_price);
    //             SendMsg(XTP_LOG_LEVEL_INFO, "Quote", s);
    //         }
    //     }
    //     else
    //     {
    //         bool ret = Data::add_code(code);
    //         if (ret)
    //         {   
    //             if (code[0] == '6')
    //                 sh_stock_num++;
    //             else
    //                 sz_stock_num++;
    //             Stock_Data_Ptr data_ptr = get_stock_static_info(ticker_info, is_last);
    //         }
    //         else
    //         {
    //             err_num++;
    //             if (err_num < 10)
    //                 printf("------get error code:%s -----\n", code);
    //         }
    //     }
    // }
    // if (is_last)
    // {
    //     char s[256];
    //     if(ticker_info->exchange_id == EXCHANGE_SH)
    //     {   
    //         sprintf(s, "Successful GET_ALL_SH_STOCK stock_num:%d", sh_stock_num);
    //         GET_ALL_SH_STOCK = true;
    //         SendMsg(XTP_LOG_LEVEL_INFO, "Quote", s);
    //     }
    //     else if (ticker_info->exchange_id == EXCHANGE_SZ)
    //     {   
    //         sprintf(s, "Successful GET_ALL_SZ_STOCK stock_num:%d", sz_stock_num);
    //         GET_ALL_SZ_STOCK = true;
    //         SendMsg(XTP_LOG_LEVEL_INFO, "Quote", s);
    //     }
    // }
}

void MyQuoteSpi::OnDepthMarketData(MarketData * market_data, int64_t bid1_qty[], int32_t bid1_count, 
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

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#include "Quote_api.h"

QuoteApi * QuoteApi::CreateQuoteApi(uint8_t client_id, const char *save_file_path, LOG_LEVEL log_level)
{
    return new MyQuoteApi;
}

void MyQuoteApi::RegisterSpi(QuoteSpi *spi)
{
    this->_spi = spi;
}

int MyQuoteApi::Login(const char* ip, int port, const char* user, const char* password, PROTOCOL_TYPE sock_type)
{
    this->start_push_quote_thread();
}

int MyQuoteApi::Logout()
{
    this->close_push_quote_thread();
}

#include <unistd.h>
#include <pthread.h>
#include <memory>
#include "local_quote_struct.h"
#include "local_quote_convert.h"
// 行情推送线程
void *task_push_quote(void* arg)
{   
    Quote_Push_Param_Struct *param = (Quote_Push_Param_Struct*)arg;
    QuoteSpi* spi = param->spi;
    Quote_Generator* generator = param->generator;
    Quote_Generator gr("a.txt");
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
    info_ptr = generator->Get_StockInfo();
    while (info_ptr != NULL)
    {   
        convert_stockinfo(info_ptr, stockinfo_ptr, &is_last);
        spi->OnQueryAllTickers(stockinfo_ptr, &errorinfo, is_last);
        info_ptr = generator->Get_StockInfo();
    }
    generator->load_data(is_am);
    // 开始推送行情
    while(generator->Get_Clock() < 150000.00)
    {   // 判断是否需要载入下午的数据
        if (generator->check_is_need_load_data())
        {   
            if (is_am)
            {
                is_am = false;
                generator->load_data(is_am);
            }
        }
        // 推送行情数据
        tickorder_ptr = generator->Get_TickOrder();
        while(tickorder_ptr != NULL)
        {   
            convert_tickorder(tickorder_ptr, tick_ptr);
            spi->OnTickByTick(tick_ptr);
            tickorder_ptr = generator->Get_TickOrder();
        }

        ticktrade_ptr = generator->Get_TickTrade();
        while(ticktrade_ptr != NULL)
        {   
            convert_ticktrade(ticktrade_ptr, tick_ptr);
            spi->OnTickByTick(tick_ptr);
            ticktrade_ptr = generator->Get_TickTrade();
        }

        snap_ptr = generator->Get_SnapData();
        level_ptr = generator->Get_LevelData();
        while(snap_ptr != NULL && level_ptr != NULL)
        {
            convert_depthdata(snap_ptr, level_ptr, depthdata_ptr);
            spi->OnDepthMarketData(depthdata_ptr);
            snap_ptr = generator->Get_SnapData();
            level_ptr = generator->Get_LevelData();
        }
        // 设置时间
        SetCurrentTime(generator->Get_Time());
    }
    printf("%s end\n", __func__);
    pthread_exit(0);
}