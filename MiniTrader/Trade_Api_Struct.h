#ifndef _TRADE_API_STRUCT_H_
#define _TRADE_API_STRUCT_H_
#include "base_data_struct.h"

//=====================客户端接口定义=================================
///新订单请求
struct OrderInsertInfo
{
    ///系统订单ID，无需用户填写，在系统中唯一
    uint64_t                order__id;
    ///报单引用，由客户自定义
    uint32_t	            order_client_id;
    ///合约代码 客户端请求不带空格，以'\0'结尾
    char                    ticker[TICKER_LEN];
    ///交易市场
    MARKET_TYPE             market;
    ///价格
    double                  price;
    ///止损价（保留字段）
    double                  stop_price;
    ///数量(股票单位为股，逆回购单位为张)
    int64_t                 quantity;
    ///报单价格
    PRICE_TYPE              price_type;
    ///买卖方向
    SIDE_TYPE               side;
    ///开平标志
    POSITION_EFFECT_TYPE    position_effect;
	///业务类型
	BUSINESS_TYPE           business_type;
 };


///撤单失败响应消息
struct OrderCancelInfo
{
    ///撤单ID
    uint64_t                 order_cancel__id;
    ///原始订单ID
    uint64_t                 order__id;
};


///报单响应结构体
struct OrderInfo
{
    ///系统订单ID，在系统中唯一
	uint64_t                order__id;
	///报单引用，用户自定义
	uint32_t	            order_client_id;
    ///报单操作引用，用户自定义（暂未使用）
    uint32_t                order_cancel_client_id;
    ///撤单在系统中的id，在系统中唯一
    uint64_t                order_cancel__id;
	///合约代码
	char                    ticker[TICKER_LEN];
	///交易市场
	MARKET_TYPE             market;
	///价格
	double                  price;
	///数量，此订单的报单数量
	int64_t                 quantity;
	///报单价格条件
	PRICE_TYPE              price_type;
    ///买卖方向
    SIDE_TYPE               side;
    ///开平标志
    POSITION_EFFECT_TYPE    position_effect;
	///业务类型
	BUSINESS_TYPE           business_type;
	///今成交数量，为此订单累计成交数量
	int64_t                 qty_traded;
	///剩余数量，当撤单成功时，表示撤单数量
	int64_t                 qty_left;
	///委托时间，格式为YYYYMMDDHHMMSSsss
	int64_t                 insert_time;
	///最后修改时间，格式为YYYYMMDDHHMMSSsss
	int64_t                 update_time;
	///撤销时间，格式为YYYYMMDDHHMMSSsss
	int64_t                 cancel_time;
	///成交金额，为此订单的成交总金额
	double                  trade_amount;
	///本地报单编号 OMS生成的单号，不等同于order__id，为服务器传到报盘的单号
	char                    order_local_id[LOCAL_ORDER_LEN];
	///报单状态，订单响应中没有部分成交状态的推送，在查询订单结果中，会有部分成交状态
	ORDER_STATUS_TYPE       order_status;
	///报单类型
	SendOrderType           order_type;
};


///报单成交结构体
struct TradeReport
{
    ///系统订单ID，此成交回报相关的订单ID，在系统中唯一
    uint64_t                 order__id;
    ///报单引用
    uint32_t                 order_client_id;
    ///合约代码
    char                     ticker[TICKER_LEN];
    ///交易市场
    MARKET_TYPE          market;
    ///订单号，引入ID后，该字段实际和order__id重复。接口中暂时保留。
    uint64_t                 local_order_id;
    ///成交编号，深交所唯一，上交所每笔交易唯一，当发现2笔成交回报拥有相同的exec_id，则可以认为此笔交易自成交
    char                     exec_id[EXEC_ID_LEN];
    ///价格，此次成交的价格
    double                   price;
    ///数量，此次成交的数量，不是累计数量
    int64_t                  quantity;
    ///成交时间，格式为YYYYMMDDHHMMSSsss
    int64_t                  trade_time;
    ///成交金额，此次成交的总金额 = price*quantity
    double                   trade_amount;
    ///成交序号 --回报记录号，每个交易所唯一,report_index+market字段可以组成唯一标识表示成交回报
    uint64_t                 report_index;
    ///报单编号 --交易所单号，上交所为空，深交所有此字段
    char                     order_exch_id[ORDER_EXCH_LEN];
    ///成交类型  --成交回报中的执行类型
    TradeType                trade_type;
    ///买卖方向
    SIDE_TYPE                side;
    ///开平标志
    POSITION_EFFECT_TYPE     position_effect;
	///业务类型
	BUSINESS_TYPE            business_type;
};


//////////////////////////////////////////////////////////////////////////
///报单查询
//////////////////////////////////////////////////////////////////////////
///报单查询请求-条件查询
struct QueryOrderReq
{
    ///证券代码，可以为空，如果为空，则默认查询时间段内的所有成交回报
    char      ticker[TICKER_LEN];
    ///格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点
    int64_t   begin_time;
    ///格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
    int64_t   end_time;  
};

///报单查询响应结构体
typedef struct OrderInfo QueryOrderRsp;


//////////////////////////////////////////////////////////////////////////
///成交回报查询
//////////////////////////////////////////////////////////////////////////
///查询成交报告请求-根据执行编号查询（保留字段）
struct QueryReportByExecIdReq
{
    ///订单系统ID
    uint64_t  order__id;  
    ///成交执行编号
    char  exec_id[EXEC_ID_LEN];
};

///查询成交回报请求-查询条件
struct QueryTraderReq
{
    ///证券代码，可以为空，如果为空，则默认查询时间段内的所有成交回报
    char      ticker[TICKER_LEN];
    ///开始时间，格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点
    int64_t   begin_time; 
    ///结束时间，格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
    int64_t   end_time;  
};

///成交回报查询响应结构体
typedef struct TradeReport  QueryTradeRsp;

//////////////////////////////////////////////////////////////////////////
///查询股票持仓情况
//////////////////////////////////////////////////////////////////////////
struct QueryStkPositionRsp
{
    ///证券代码
    char                ticker[TICKER_LEN];
    ///证券名称
    char                ticker_name[TICKER_NAME_LEN];
    ///交易市场
    MARKET_TYPE     market;
    ///总持仓
    int64_t             total_qty;
    ///可卖持仓
    int64_t				sellable_qty;
    ///持仓成本
    double              avg_price;
    ///浮动盈亏（保留字段）
    double              unrealized_pnl;
    ///昨日持仓
    int64_t             yesterday_position;
    ///今日申购赎回数量（申购和赎回数量不可能同时存在，因此可以共用一个字段）
    int64_t				purchase_redeemable_qty;

    /// 持仓方向
	POSITION_DIRECTION_TYPE      position_direction;
	///保留字段1
	uint32_t			reserved1;
    /// 可行权合约
    int64_t             executable_option;
    /// 可锁定标的
    int64_t             lockable_position;
    /// 可行权标的
    int64_t             executable_underlying;
    /// 已锁定标的
    int64_t             locked_position;
    /// 可用已锁定标的
    int64_t             usable_locked_position;
};
#endif