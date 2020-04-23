#ifndef _QUOTE_API_H_
#include "Quote_Api.h"
#endif

#define NUM_OF_ROUND 1

class MyQuoteSpi : public QuoteSpi
{
public:
	MyQuoteSpi();

	~MyQuoteSpi();

	void OnDisconnected(int reason);

	void OnError(ErrorInfo *error_info, bool is_last);

    void OnQueryAllTickers(StockStaticInfo* ticker_info, ErrorInfo *error_info, bool is_last);

	void OnDepthMarketData(DepthData *market_data);
	
	void OnTickByTick(TickByTick *tbt_data);

private:

	bool IsErrorRspInfo(ErrorInfo *pRspInfo);

};
