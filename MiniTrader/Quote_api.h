#ifndef QUOTE_API_H_
#define QUOTE_API_H_
#include "quote_api_struct.h"

/*!
* \class ::API::QuoteSpi
行情回调类
*/
class QuoteSpi
{
public:

	///当客户端与行情后台通信连接断开时，该方法被调用。
	///@param reason 错误原因，请与错误代码表对应
	///@remark api不会自动重连，当断线发生时，请用户自行选择后续操作。可以在此函数中调用Login重新登录。注意用户重新登录后，需要重新订阅行情
	virtual void OnDisconnected(int reason) {};


	///错误应答
	///@param error_info 当服务器响应发生错误时的具体的错误代码和错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
	///@remark 此函数只有在服务器发生错误时才会调用，一般无需用户处理
	virtual void OnError(ErrorInfo *error_info) {};

	///深度行情通知，包含买一卖一队列
	///@param market_data 行情数据
	///@param bid1_qty 买一队列数据
	///@param bid1_count 买一队列的有效委托笔数
	///@param max_bid1_count 买一队列总委托笔数
	///@param ask1_qty 卖一队列数据
	///@param ask1_count 卖一队列的有效委托笔数
	///@param max_ask1_count 卖一队列总委托笔数
	///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
	virtual void OnDepthMarketData(MarketData *market_data, int64_t bid1_qty[], int32_t bid1_count, int32_t max_bid1_count, int64_t ask1_qty[], int32_t ask1_count, int32_t max_ask1_count) {};

	///逐笔行情通知，包括股票、指数和期权
	///@param tbt_data 逐笔行情数据，包括逐笔委托和逐笔成交，此为共用结构体，需要根据type来区分是逐笔委托还是逐笔成交，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
	virtual void OnTickByTick(TickByTick *tbt_data) {};

	///查询可交易合约的应答
	///@param ticker_info 可交易合约信息
	///@param error_info 查询可交易合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
	///@param is_last 是否此次查询可交易合约的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
	virtual void OnQueryAllTickers(StockStaticInfo* ticker_info, ErrorInfo *error_info, bool is_last) {};
};

/*
行情订阅接口类
*/

class QuoteApi
{
public:
	///创建QuoteApi
	///@remark 如果一个账户需要在多个客户端登录，请使用不同的client_id，系统允许一个账户同时登录多个客户端，但是对于同一账户，相同的client_id只能保持一个session连接，后面的登录在前一个session存续期间，无法连接
	static QuoteApi *CreateQuoteApi(const char *inifile);

	///删除接口对象本身
	///@remark 不再使用本接口对象时,调用该函数删除接口对象
	virtual void Release() = 0;


	///获取当前
	virtual int GetClock() = 0;

	///获取API的系统错误
	///@return 返回的错误信息，可以在Login、Logout、订阅、取消订阅失败时调用，获取失败的原因
	///@remark 可以在调用api接口失败时调用，例如login失败时
	virtual ErrorInfo *GetApiLastError() = 0;

	///设置采用UDP方式连接时的接收缓冲区大小
	///@remark 需要在Login之前调用，默认大小和最小设置均为64MB。此缓存大小单位为MB，请输入2的次方数，例如128MB请输入128。
	virtual void SetUDPBufferSize(uint32_t buff_size) = 0;

	///注册回调接口
	///@param spi 派生自回调接口类的实例，请在登录之前设定
	virtual void RegisterSpi(QuoteSpi *spi) = 0;

	///设置心跳检测时间间隔，单位为秒
	///@param interval 心跳检测时间间隔，单位为秒
	///@remark 此函数必须在Login之前调用
	virtual void SetHeartBeatInterval(uint32_t interval) = 0;


	///用户登录请求
	virtual int Login() = 0;


	///登出请求
	///@return 登出是否成功，“0”表示登出成功，非“0”表示登出出错，此时用户可以调用GetApiLastError()来获取错误代码
	///@remark 此函数为同步阻塞式，不需要异步等待登出，当函数返回即可进行后续操作
	virtual int Logout() = 0;

	///获取当前交易日可交易合约
	///@return 查询是否成功，“0”表示查询成功，非“0”表示查询不成功
	///@param exchange_id 交易所代码
	virtual int QueryAllTickers(EXCHANGE_TYPE exchange_id) = 0;
protected:
	~QuoteApi() {};
};

#endif
