#define _SIM_QUOTE_STRUCT_H_
//定义本地行情类相关数据结构
#define SIM_EXCHANGE_SH 1
#define SIM_EXCHANGE_SZ 2
#define SIM_CODESIZE 8
#define SIM_NAMESIZE 16
#define SIM_TRADETYPE_DEAL 'F'
#define SIM_TRADETYPE_CANCEL '4'
#define SIM_ORDERTYPE_MARKET '1'
#define SIM_ORDERTYPE_LIMIT '2'
#define SIM_ORDERSIDE_BUY '1'
#define SIM_ORDERSIDE_SELL '2'
#include <stdint.h>
///快照行情
struct SimSnapData
{
    // 代码
    ///交易所代码
    int exchange_id;
    ///合约代码（不包含交易所信息），不带空格，以'\0'结尾
    char code[SIM_CODESIZE];

    // 价格
    ///最新价
    double last_price;
    ///昨收盘
    double pre_close_price;
    ///今开盘
    double open_price;
    ///最高价
    double high_price;
    ///最低价
    double low_price;
    ///今收盘
    double close_price;

    // 涨跌停
    ///涨停价
    double upper_limit_price;
    ///跌停价
    double lower_limit_price;

    /// 时间类，格式为YYYYMMDDHHMMSSsss
    int64_t data_time;

    // 量额数据
    ///数量，为总成交量（单位股，与交易所一致）
    int64_t qty;
    ///成交金额，为总成交金额（单位元，与交易所一致）
    double turnover;
    ///当日均价=(turnover/qty)
    double avg_price;
    // 用户向用户传送行情时指向下一个元素
    SimSnapData *next;
};

///10档行情
struct SimLevelData
{
    // 代码
    ///交易所代码
    int exchange_id;
    ///合约代码（不包含交易所信息），不带空格，以'\0'结尾
    char code[SIM_CODESIZE];
    /// 时间类，格式为YYYYMMDDHHMMSSsss
    int64_t data_time;

    // 买卖盘
    ///十档申买价
    double bid[10];
    ///十档申卖价
    double ask[10];
    ///十档申买量
    int64_t bid_qty[10];
    ///十档申卖量
    int64_t ask_qty[10];

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
    // 用户向用户传送行情时指向下一个元素
    SimLevelData *next;
};

///股票行情静态信息
struct SimStockInfo
{
    ///交易所代码
    int exchange_id;
    ///合约代码（不包含交易所信息），不带空格，以'\0'结尾
    char code[SIM_CODESIZE];
    /// 合约名称
    char name[16];
    ///昨收盘
    double pre_close_price;
    ///涨停板价
    double upper_limit_price;
    ///跌停板价
    double lower_limit_price;
    ///最小变动价位
    double price_tick;
    // 用户向用户传送行情时指向下一个元素
    SimStockInfo *next;
};

///逐笔委托(仅适用深交所)
struct SimTickOrder
{
    ///交易所代码
    int exchange_id;
    ///合约代码（不包含交易所信息），不带空格，以'\0'结尾
    char code[SIM_CODESIZE];
    ///'1':买; '2':卖; 'G':借入; 'F':出借
    char side;
    ///订单类别: '1': 市价; '2': 限价; 'U': 本方最优
    char type;
    ///委托时间
    int64_t data_time;
    ///委托序号(在同一个channel_no内唯一，从1开始连续)
    int64_t seq;
    ///委托价格
    double price;
    ///委托数量
    int64_t qty;
    // 用户向用户传送行情时指向下一个元素
    SimTickOrder *next;
};

///逐笔成交
struct SimTickTrade
{
    ///交易所代码
    int exchange_id;
    ///合约代码（不包含交易所信息），不带空格，以'\0'结尾
    char code[SIM_CODESIZE];
    /// SH: 内外盘标识('B':主动买; 'S':主动卖; 'N':未知)
    /// SZ: 成交标识('4':撤; 'F':成交)
    char type;
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
    // 用户向用户传送行情时指向下一个元素
    SimTickTrade *next;
};

// simulate 模拟系统数据管理结构体
struct SimDataManager
{   
    // 数据起始指针
    void *begin;         
    // 数据末尾指针
    void *end;           
    // 数据当前指针
    void *current;        
};