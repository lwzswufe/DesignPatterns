#pragma once
#include "Trade_Api.h"

#define MAX_REQUEST_ID_NUM 10000

class MyTradeSpi : public TradeSpi
{
public:
	MyTradeSpi();
	~MyTradeSpi();

	virtual void OnOrderEvent(OrderInfo *order_info);

	virtual void OnTradeEvent(TradeReport *trade_info);

	virtual void OnQueryOrder(QueryOrderRsp *order_info, bool is_last);

	virtual void OnQueryPosition(QueryStkPositionRsp *investor_position, bool is_last);

	void setUserAPI(TradeApi* pApi);

private:

	TradeApi* pUserApi;

};