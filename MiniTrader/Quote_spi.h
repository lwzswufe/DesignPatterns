#pragma once
#include "quote_api.h"


#define NUM_OF_ROUND 1

class MyQuoteSpi : public QuoteSpi
{
public:
	MyQuoteSpi();

	~MyQuoteSpi();

	void OnDisconnected(int reason);

	void OnError(ErrorInfo *error_info, bool is_last);

    void OnQueryAllTickers(StockStaticInfo* ticker_info, ErrorInfo *error_info, bool is_last);

	void OnDepthMarketData(DepthData *market_data, int64_t bid1_qty[], int32_t bid1_count, 
                                                    int32_t max_bid1_count, int64_t ask1_qty[], 
                                                    int32_t ask1_count, int32_t max_ask1_count);
	void OnTickByTick(TickByTick *tbt_data);

private:

	bool IsErrorRspInfo(ErrorInfo *pRspInfo);

};

class MyQuoteApi :public QuoteApi
{
public:
	void Release();

	const char *GetTradingDay();

	const char* GetApiVersion();

	ErrorInfo *GetApiLastError();

	void SetUDPBufferSize(uint32_t buff_size);

	void RegisterSpi(QuoteSpi *spi);

	void SetHeartBeatInterval(uint32_t interval);

	int Login(const char* ip, int port, const char* user, const char* password, PROTOCOL_TYPE sock_type);

	int Logout();

	int QueryAllTickers(EXCHANGE_TYPE exchange_id);

private:
	// 开启行情推送线程
	void start_push_quote_thread();
	// 关闭行情推送线程
	void close_push_quote_thread();

	QuoteSpi* _spi;

};

class Quote_Generator;
// 行情推送线程参数结构体
struct Quote_Push_Param_Struct
{
    bool push_status;           // 推送状态 true推送  false不推送
    QuoteSpi* spi;              // 行情回调类
	Quote_Generator* generator; // 行情生成类
};

// 推送行情线程
void *task_push_quote(void* args);