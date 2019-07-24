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
