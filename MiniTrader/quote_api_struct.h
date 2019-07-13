//定义行情类相关数据结构

#ifndef _QUOTE_API_STRUCT_H_
#define _QUOTE_API_STRUCT_H_

#include "base_data_struct.h"

///指定的合约
typedef struct StockCodeStruct
{
    ///交易所代码
    EXCHANGE_TYPE exchange_id;
    ///合约代码（不包含交易所信息）例如"600000"，不带空格，以'\0'结尾
	char	ticker[TICKER_LEN];
} StockCode;

enum MARKETDATA_TYPE {
    MARKETDATA_ACTUAL = 0, // 现货(股票/基金/债券等)
    MARKETDATA_OPTION = 1, // 期权
};

///快照行情
typedef struct SnapDataStruct
{
    // 代码
    ///交易所代码
    EXCHANGE_TYPE exchange_id;
    ///合约代码（不包含交易所信息），不带空格，以'\0'结尾
    char	ticker[TICKER_LEN];

    // 价格
	///最新价
	double	last_price;
	///昨收盘
	double	pre_close_price;
	///今开盘
	double	open_price;
	///最高价
	double	high_price;
	///最低价
	double	low_price;
    ///今收盘
    double	close_price;

	// 涨跌停
	///涨停价
	double	upper_limit_price;
	///跌停价
	double	lower_limit_price;

    /// 时间类，格式为YYYYMMDDHHMMSSsss
    int64_t data_time;

    // 量额数据
    ///数量，为总成交量（单位股，与交易所一致）
    int64_t	qty;
    ///成交金额，为总成交金额（单位元，与交易所一致）
    double	turnover;
    ///当日均价=(turnover/qty)
    double	avg_price;

    // 额外数据
    ///成交笔数
    int64_t trades_count;
    ///当前交易状态说明
    char ticker_status[8];
} SnapData;

///10档行情
typedef struct LevelDataStruct
{
    // 代码
    ///交易所代码
    EXCHANGE_TYPE exchange_id;
    ///合约代码（不包含交易所信息），不带空格，以'\0'结尾
    char	ticker[TICKER_LEN];
    /// 时间类，格式为YYYYMMDDHHMMSSsss
    int64_t data_time;

    // 买卖盘
    ///十档申买价
    double bid[10];
    ///十档申卖价
    double	ask[10];
    ///十档申买量
    int64_t	bid_qty[10];
    ///十档申卖量
    int64_t	ask_qty[10];

    // 买卖队列
    // 买一队列数据
    int64_t bid1_qty[50];
    // 买一队列的有效委托笔数	
    int bid1_count;
    // 买一队列总委托笔数
    int max_bid1_count;
    // 卖一队列数据
    int64_t ask1_qty[50];
    // 卖一队列的有效委托笔数
    int ask1_count;
    // 卖一队列总委托笔数
    int max_ask1_count;

} LevelData;

///股票行情静态信息
typedef struct StockStaticInfo {
    ///交易所代码
    EXCHANGE_TYPE exchange_id;
    ///合约代码（不包含交易所信息），不带空格，以'\0'结尾
    char    ticker[TICKER_LEN];
    /// 合约名称
    char    ticker_name[TICKER_NAME_LEN];
    /// 合约类型
	TICKER_TYPE ticker_type;
    ///昨收盘
    double  pre_close_price;
    ///涨停板价
    double  upper_limit_price;
    ///跌停板价
    double  lower_limit_price;
	///最小变动价位
	double  price_tick;
    /// 合约最小交易量(买)
    int32_t  buy_qty_unit;
    /// 合约最小交易量(卖)
	int32_t sell_qty_unit;
} StockInfo;


///定单薄
typedef struct OrderBookStruct {
    ///交易所代码
    EXCHANGE_TYPE exchange_id;
    ///合约代码（不包含交易所信息），不带空格，以'\0'结尾
    char    ticker[TICKER_LEN];

    ///最新价
    double last_price;
    ///数量，为总成交量
    int64_t qty;
    ///成交金额，为总成交金额
    double  turnover;
    ///成交笔数
    int64_t trades_count;

    // 买卖盘
    ///十档申买价
    double bid[10];
    ///十档申卖价
    double  ask[10];
    ///十档申买量
    int64_t bid_qty[10];
    ///十档申卖量
    int64_t ask_qty[10];
    /// 时间类
    int64_t data_time;
} OrderBook;

////////////////////////////////// 逐笔数据


///逐笔委托(仅适用深交所)
struct TickByTickOrder {
    ///交易所代码
    EXCHANGE_TYPE exchange_id;
    ///合约代码（不包含交易所信息），不带空格，以'\0'结尾
    char ticker[TICKER_LEN];
    ///委托时间 
    int64_t data_time;
    ///委托序号(在同一个channel_no内唯一，从1开始连续)
    int64_t seq;
    ///委托价格
    double  price;
    ///委托数量
    int64_t qty;
    ///'1':买; '2':卖; 'G':借入; 'F':出借
    char  side;
    ///订单类别: '1': 市价; '2': 限价; 'U': 本方最优
    char type;
};

///逐笔成交
struct TickByTickTrade {
    ///交易所代码
    EXCHANGE_TYPE exchange_id;
    ///合约代码（不包含交易所信息），不带空格，以'\0'结尾
    char ticker[TICKER_LEN];
    ///成交时间
    int64_t data_time;
    ///委托序号(在同一个channel_no内唯一，从1开始连续)
    int64_t seq;
    ///成交价格
    double price;
    ///成交量
    int64_t qty;
    ///成交金额(仅适用上交所)
    double money;
    ///买方订单号
    int64_t bid_no;
    ///卖方订单号
    int64_t ask_no;
    /// SH: 内外盘标识('B':主动买; 'S':主动卖; 'N':未知)
    /// SZ: 成交标识('4':撤; 'F':成交)
    char type;
};

///逐笔数据信息
typedef struct TickByTickStruct {
    ///预留
    int64_t seq;
    ///委托时间 or 成交时间
    int64_t data_time;
    ///委托 or 成交
    TBT_TYPE type;

    union {
        TickByTickOrder     order;
        TickByTickTrade     trade;
    };
} TickByTick;

#endif
