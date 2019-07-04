//定义基础数据基本类型
#ifndef _BASE_DATA_STRUCT_H_
#define _BASE_DATA_STRUCT_H_
#include <stdint.h>

/// 存放版本号的字符串长度
#define VERSION_LEN 16
/// 版本号类型
typedef char VersionType[VERSION_LEN];
/// 可交易日字符串长度
#define TRADING_DAY_LEN 9
/// 存放证券代码的字符串长度
#define TICKER_LEN 16
/// 存放证券名称的字符串长度
#define TICKER_NAME_LEN 64
/// 本地报单编号的字符串长度
#define LOCAL_ORDER_LEN         11
/// 交易所单号的字符串长度
#define ORDER_EXCH_LEN          17
/// 成交执行编号的字符串长度
#define EXEC_ID_LEN             18
/// 交易所交易员代码字符串长度
#define BRANCH_PBU_LEN          7
/// 用户资金账户的字符串长度
#define ACCOUNT_NAME_LEN        16

/////////////////////////////////////////////////////////////////////////
// LOG_LEVEL是日志输出级别类型
/////////////////////////////////////////////////////////////////////////
typedef enum LOG_LEVEL {
	LOG_LEVEL_FATAL,    ///<严重错误级别
	LOG_LEVEL_ERROR,    ///<错误级别
	LOG_LEVEL_WARNING,  ///<警告级别
	LOG_LEVEL_INFO,     ///<info级别
	LOG_LEVEL_DEBUG,    ///<debug级别
	LOG_LEVEL_TRACE     ///<trace级别
}LOG_LEVEL;

/////////////////////////////////////////////////////////////////////////
// PROTOCOL_TYPE是通讯传输协议方式
/////////////////////////////////////////////////////////////////////////
typedef enum PROTOCOL_TYPE
{
	PROTOCOL_TCP = 1,	///<采用TCP方式传输
	PROTOCOL_UDP		///<采用UDP方式传输(仅行情接口支持)
}PROTOCOL_TYPE;



/////////////////////////////////////////////////////////////////////////
// EXCHANGE_TYPE是交易所类型
/////////////////////////////////////////////////////////////////////////
typedef enum EXCHANGE_TYPE
{
	EXCHANGE_SH = 1,	///<上证
	EXCHANGE_SZ,		///<深证
    EXCHANGE_UNKNOWN	///<不存在的交易所类型
}EXCHANGE_TYPE;

//////////////////////////////////////////////////////////////////////////
// MARKET_TYPE市场类型
//////////////////////////////////////////////////////////////////////////
typedef enum MARKET_TYPE
{
    MKT_INIT = 0,///<初始化值或者未知
    MKT_SZ_A = 1,///<深圳A股
    MKT_SH_A,    ///<上海A股
    MKT_UNKNOWN   ///<未知交易市场类型
}MARKET_TYPE;


/////////////////////////////////////////////////////////////////////////
// PRICE_TYPE是价格类型
/////////////////////////////////////////////////////////////////////////
typedef enum PRICE_TYPE
{
	PRICE_LIMIT = 1,           ///<限价单-沪 / 深 / 沪期权（除普通股票业务外，其余业务均使用此种类型）
	PRICE_BEST_OR_CANCEL,      ///<即时成交剩余转撤销，市价单-深 / 沪期权
	PRICE_BEST5_OR_LIMIT,      ///<最优五档即时成交剩余转限价，市价单-沪
	PRICE_BEST5_OR_CANCEL,     ///<最优5档即时成交剩余转撤销，市价单-沪深
	PRICE_ALL_OR_CANCEL,       ///<全部成交或撤销,市价单-深 / 沪期权
	PRICE_FORWARD_BEST,        ///<本方最优，市价单-深
	PRICE_REVERSE_BEST_LIMIT,  ///<对方最优剩余转限价，市价单-深 / 沪期权
	PRICE_LIMIT_OR_CANCEL,	   ///<期权限价申报FOK
	PRICE_TYPE_UNKNOWN,		   ///<未知或者无效价格类型
}PRICE_TYPE;



/////////////////////////////////////////////////////////////////////////
// SIDE_TYPE是买卖方向类型
/////////////////////////////////////////////////////////////////////////
typedef char SIDE_TYPE;

///买（新股申购，ETF买，配股，信用交易中担保品买）
#define SIDE_BUY            1
///卖（逆回购，ETF卖，信用交易中担保品卖）
#define SIDE_SELL           2
///申购
#define SIDE_PURCHASE       7
///赎回
#define SIDE_REDEMPTION     8
///拆分
#define SIDE_SPLIT          9
///合并
#define SIDE_MERGE          10
///改版之后的side的备兑，暂不支持
#define SIDE_COVER          11
///改版之后的side锁定（对应开平标识为开）/解锁（对应开平标识为平）
#define SIDE_FREEZE         12
/// 融资买入
#define SIDE_MARGIN_TRADE	21
/// 融券卖出
#define SIDE_SHORT_SELL		22
/// 卖券还款
#define SIDE_REPAY_MARGIN	23
/// 买券还券
#define SIDE_REPAY_STOCK	24
/// 现金还款（不放在普通订单协议，另加请求和查询协议）
//#define SIDE_CASH_REPAY_MARGIN	25
/// 现券还券
#define SIDE_STOCK_REPAY_STOCK	26
/// 余券划转
#define SIDE_SURSTK_TRANS       27
/// 担保品转入
#define SIDE_GRTSTK_TRANSIN     28
/// 担保品转出
#define SIDE_GRTSTK_TRANSOUT    29

///未知或者无效买卖方向
#define SIDE_UNKNOWN        30



/////////////////////////////////////////////////////////////////////////
// POSITION_EFFECT_TYPE是开平标识类型
/////////////////////////////////////////////////////////////////////////
typedef char POSITION_EFFECT_TYPE;

/// 初始值或未知值开平标识，现货适用
#define POSITION_EFFECT_INIT                0
/// 开
#define POSITION_EFFECT_OPEN                1
/// 平
#define POSITION_EFFECT_CLOSE               2
/// 强平
#define POSITION_EFFECT_FORCECLOSE          3
/// 平今
#define POSITION_EFFECT_CLOSETODAY          4
/// 平昨
#define POSITION_EFFECT_CLOSEYESTERDAY      5
/// 强减
#define POSITION_EFFECT_FORCEOFF            6
/// 本地强平
#define POSITION_EFFECT_LOCALFORCECLOSE     7
/// 信用业务追保强平
#define POSITION_EFFECT_CREDIT_FORCE_COVER  8
/// 信用业务清偿强平
#define POSITION_EFFECT_CREDIT_FORCE_CLEAR  9
/// 信用业务合约到期强平
#define POSITION_EFFECT_CREDIT_FORCE_DEBT   10
/// 信用业务无条件强平
#define POSITION_EFFECT_CREDIT_FORCE_UNCOND 11
/// 未知的开平标识类型
#define POSITION_EFFECT_UNKNOWN             12


/////////////////////////////////////////////////////////////////////////
// ORDER_ACTION_STATUS_TYPE是报单操作状态类型
/////////////////////////////////////////////////////////////////////////
typedef enum ORDER_ACTION_STATUS_TYPE
{
	ORDER_ACTION_STATUS_SUBMITTED = 1,	///<已经提交
	ORDER_ACTION_STATUS_ACCEPTED,		///<已经接受
	ORDER_ACTION_STATUS_REJECTED		///<已经被拒绝
}ORDER_ACTION_STATUS_TYPE;

/////////////////////////////////////////////////////////////////////////
// ORDER_STATUS_TYPE是报单状态类型
/////////////////////////////////////////////////////////////////////////
typedef enum ORDER_STATUS_TYPE
{
    ORDER_STATUS_INIT = 0,///<初始化
    ORDER_STATUS_ALLTRADED = 1,           ///<全部成交
    ORDER_STATUS_PARTTRADEDQUEUEING,  ///<部分成交
    ORDER_STATUS_PARTTRADEDNOTQUEUEING, ///<部分撤单
    ORDER_STATUS_NOTRADEQUEUEING,   ///<未成交
    ORDER_STATUS_CANCELED,  ///<已撤单
    ORDER_STATUS_REJECTED,  ///<已拒绝
    ORDER_STATUS_UNKNOWN  ///<未知订单状态
}ORDER_STATUS_TYPE;

/////////////////////////////////////////////////////////////////////////
// ORDER_SUBMIT_STATUS_TYPE是报单提交状态类型
/////////////////////////////////////////////////////////////////////////
typedef enum ORDER_SUBMIT_STATUS_TYPE
{
    ORDER_SUBMIT_STATUS_INSERT_SUBMITTED = 1, ///<订单已经提交
    ORDER_SUBMIT_STATUS_INSERT_ACCEPTED,///<订单已经被接受
    ORDER_SUBMIT_STATUS_INSERT_REJECTED,///<订单已经被拒绝
    ORDER_SUBMIT_STATUS_CANCEL_SUBMITTED,///<撤单已经提交
    ORDER_SUBMIT_STATUS_CANCEL_REJECTED,///<撤单已经被拒绝
    ORDER_SUBMIT_STATUS_CANCEL_ACCEPTED ///<撤单已经被接受
}ORDER_SUBMIT_STATUS_TYPE;


/////////////////////////////////////////////////////////////////////////
// TE_RESUME_TYPE是公有流（订单响应、成交回报）重传方式
/////////////////////////////////////////////////////////////////////////
typedef enum TE_RESUME_TYPE
{
	TERT_RESTART = 0,	///<从本交易日开始重传
	TERT_RESUME,		///<从从上次收到的续传（暂未支持）
	TERT_QUICK			///<只传送登录后公有流（订单响应、成交回报）的内容
}TE_RESUME_TYPE;


//////////////////////////////////////////////////////////////////////////
// ETF_REPLACE_TYPE现金替代标识定义
//////////////////////////////////////////////////////////////////////////
typedef enum ETF_REPLACE_TYPE
{
    ERT_CASH_FORBIDDEN = 0,             ///<禁止现金替代
    ERT_CASH_OPTIONAL,                  ///<可以现金替代
    ERT_CASH_MUST,                      ///<必须现金替代
    ERT_CASH_RECOMPUTE_INTER_SZ,        ///<深市退补现金替代
    ERT_CASH_MUST_INTER_SZ,             ///<深市必须现金替代
    ERT_CASH_RECOMPUTE_INTER_OTHER,     ///<非沪深市场成分证券退补现金替代
    ERT_CASH_MUST_INTER_OTHER,          ///<表示非沪深市场成份证券必须现金替代
    EPT_INVALID                         ///<无效值
}ETF_REPLACE_TYPE;


//////////////////////////////////////////////////////////////////////////
// TICKER_TYPE证券类型
//////////////////////////////////////////////////////////////////////////
typedef enum TICKER_TYPE
{
	TICKER_TYPE_STOCK = 0,            ///<普通股票
	TICKER_TYPE_INDEX,                ///<指数
	TICKER_TYPE_FUND,                 ///<基金
	TICKER_TYPE_BOND,                 ///<债券
	TICKER_TYPE_OPTION,               ///<期权
	TICKER_TYPE_UNKNOWN               ///<未知类型
	
}TICKER_TYPE;

//////////////////////////////////////////////////////////////////////////
// BUSINESS_TYPE证券业务类型
//////////////////////////////////////////////////////////////////////////
typedef enum BUSINESS_TYPE
{
	BUSINESS_TYPE_CASH = 0,            ///<普通股票业务（股票买卖，ETF买卖等）
	BUSINESS_TYPE_IPOS,                ///<新股申购业务（对应的price type需选择限价类型）
	BUSINESS_TYPE_REPO,                ///<回购业务 ( 对应的price type填为限价，side填为卖 )
	BUSINESS_TYPE_ETF,                 ///<ETF申赎业务
	BUSINESS_TYPE_MARGIN,              ///<融资融券业务（暂未支持）
	BUSINESS_TYPE_DESIGNATION,         ///<转托管（未支持）
	BUSINESS_TYPE_ALLOTMENT,		   ///<配股业务（对应的price type需选择限价类型,side填为买）
	BUSINESS_TYPE_STRUCTURED_FUND_PURCHASE_REDEMPTION,	   ///<分级基金申赎业务
	BUSINESS_TYPE_STRUCTURED_FUND_SPLIT_MERGE,	   ///<分级基金拆分合并业务
	BUSINESS_TYPE_MONEY_FUND,		   ///<货币基金业务（暂未支持）
    BUSINESS_TYPE_OPTION,              ///<期权业务
    BUSINESS_TYPE_EXECUTE,             ///<行权
    BUSINESS_TYPE_FREEZE,              ///<锁定解锁，暂不支持
	BUSINESS_TYPE_UNKNOWN              ///<未知类型

}BUSINESS_TYPE;

//////////////////////////////////////////////////////////////////////////
// ACCOUNT_TYPE账户类型
//////////////////////////////////////////////////////////////////////////
typedef enum ACCOUNT_TYPE
{
    ACCOUNT_NORMAL = 0,	///<普通账户
    ACCOUNT_CREDIT,		///<信用账户
    ACCOUNT_DERIVE,		///<衍生品账户
    ACCOUNT_UNKNOWN		///<未知账户类型
}ACCOUNT_TYPE;


/////////////////////////////////////////////////////////////////////////
// TBT_TYPE是一个逐笔回报类型
/////////////////////////////////////////////////////////////////////////
typedef enum TBT_TYPE {
	TBT_ENTRUST = 1,	///<逐笔委托
	TBT_TRADE = 2,		///<逐笔成交
}TBT_TYPE;

/////////////////////////////////////////////////////////////////////////
///TradeType是成交类型类型
/////////////////////////////////////////////////////////////////////////
typedef char TradeType;

///普通成交
#define TRDT_COMMON '0'
///现金替代
#define TRDT_CASH '1'
///一级市场成交
#define TRDT_PRIMARY '2'
///跨市场资金成交
#define TRDT_CROSS_MKT_CASH '3'


/////////////////////////////////////////////////////////////////////////
///TRADEType是报单类型类型
/////////////////////////////////////////////////////////////////////////
typedef char SendOrderType;

///正常
#define ORDT_Normal '0'
///报价衍生
#define ORDT_DeriveFromQuote '1'
///组合衍生
#define ORDT_DeriveFromCombination '2'
///组合报单
#define ORDT_Combination '3'
///条件单
#define ORDT_ConditionalOrder '4'
///互换单
#define ORDT_Swap '5'

typedef enum POSITION_DIRECTION_TYPE {
	POSITION_DIRECTION_NET = 0,	    ///<净
	POSITION_DIRECTION_LONG,		///<多（期权则为权利方）
    POSITION_DIRECTION_SHORT,       ///<空（期权则为义务方）
    POSITION_DIRECTION_COVERED,     ///<备兑（期权则为备兑义务方）
}POSITION_DIRECTION_TYPE;

///错误信息的字符串长度
#define ERR_MSG_LEN  124
///响应信息
typedef struct ErrorInfoStruct
{
	///错误代码
	int32_t	error_id;
	///错误信息
	char	error_msg[ERR_MSG_LEN];
} ErrorInfo;

#endif
