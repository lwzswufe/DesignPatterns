#include "Trade_Spi.h"

// 行情推送线程参数结构体
struct Trade_Push_Param_Struct
{
    bool  push_status;                      // 推送状态 true推送  false不推送
    double  clock;                          // HHMMSS.sss
	const char* ini_file;					// 配置文件
    TradeSpi* spi;                          // 行情回调类
};
extern void *task_push_trade(void* arg);
class MyTradeApi :public TradeApi
{
public:
    MyTradeApi(uint8_t client_id);

	///注册回调接口
	///@param spi 派生自回调接口类的实例，请在登录之前设定
	void RegisterSpi(TradeSpi *spi);

	///用户登录请求
	///@return session_id表明此资金账号登录是否成功，“0”表示登录失败，可以调用GetApiLastError()来获取错误代码，非“0”表示登录成功，此时需要记录下这个返回值session_id，与登录的资金账户对应
	///@remark 此函数为同步阻塞式，不需要异步等待登录成功，当函数返回即可进行后续操作，此api可支持多个账户连接，但是同一个账户同一个client_id只能有一个session连接，后面的登录在前一个session存续期间，无法连接
	uint64_t Login();

	///登出请求
	///@return 登出是否成功，“0”表示登出成功，“-1”表示登出失败
	///@param session_id 资金账户对应的session_id,登录时得到
	int Logout();

	///报单录入请求
	///@return 报单在系统中的ID,如果为‘0’表示报单发送失败，此时用户可以调用GetApiLastError()来获取错误代码，非“0”表示报单发送成功，用户需要记录下返回的order__id，它保证一个交易日内唯一，不同的交易日不保证唯一性
	///@param order 报单录入信息，其中order.order_client_id字段是用户自定义字段，用户输入什么值，订单响应OnOrderEvent()返回时就会带回什么值，类似于备注，方便用户自己定位订单。当然，如果你什么都不填，也是可以的。order.order__id字段无需用户填写，order.ticker必须不带空格，以'\0'结尾
	///@param session_id 资金账户对应的session_id,登录时得到
	///@remark 交易所接收订单后，会在报单响应函数OnOrderEvent()中返回报单未成交的状态，之后所有的订单状态改变（除了部成状态）都会通过报单响应函数返回
	uint64_t InsertOrder(OrderInsertInfo *order);

	///报单操作请求
	///@return 撤单在系统中的ID,如果为‘0’表示撤单发送失败，此时用户可以调用GetApiLastError()来获取错误代码，非“0”表示撤单发送成功，用户需要记录下返回的order_cancel__id，它保证一个交易日内唯一，不同的交易日不保证唯一性
	///@param order__id 需要撤销的委托单在系统中的ID
	///@param session_id 资金账户对应的session_id,登录时得到
	///@remark 如果撤单成功，会在报单响应函数OnOrderEvent()里返回原单部撤或者全撤的消息，如果不成功，会在OnCancelOrderError()响应函数中返回错误原因
	uint64_t CancelOrder(const uint64_t order__id);

	///请求查询报单
	///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
	///@param query_param 需要查询的订单相关筛选条件，其中合约代码可以为空，则默认所有存在的合约代码，如果不为空，请不带空格，并以'\0'结尾，其中起始时间格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点，结束时间格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
	int QueryOrders(const QueryOrderReq *query_param);

	///请求查询投资者持仓
	///@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
	///@param ticker 需要查询的持仓合约代码，可以为空，如果不为空，请不带空格，并以'\0'结尾
	int QueryPosition(const char *ticker);

private:

    TradeSpi * _spi;

    void start_push_trade_thread();

    void close_push_trade_thread();

    bool trade_pthread_status;

    Trade_Push_Param_Struct trade_param;
    
    char ini_file[256];
};