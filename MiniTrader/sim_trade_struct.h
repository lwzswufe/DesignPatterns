//定义本地交易类相关数据结构
#ifndef _SIM_QUOTE_STRUCT_H_
#include "sim_quote_struct.h"
#endif
#define _SIM_TRADE_STRUCT_H_
typedef char SIM_ORDERTYPE;
typedef char SIM_ORDERSIDE;
typedef char SIM_ORDERSTATE;
#define SIM_ORDERSTATE_TRADED     'd'
#define SIM_ORDERSTATE_PARTTRADED 'p'
#define SIM_ORDERSTATE_CANCELED   'c'
#define SIM_ORDERSTATE_SENDED     's'
#define SIM_ORDERSTATE_UNKNOWN     0

struct SimOrder
{
    SIM_ORDERTYPE type;                                       // 订单类型
    SIM_ORDERSIDE side;                                       // 订单类型
    SIM_ORDERSTATE state;                                     // 订单状态
    int64_t id;                                               // 订单编号 每一个订单唯一
    int64_t seq;                                              // 订单序列号
    int64_t vol;                                              // 委托量
    int64_t traded_vol;                                       // 已成交量
    double price;                                             // 委托价格
    double order_time;                                        // 报单时间
    int account_id;                                           // 账户编号
    int exchange_id;                                          // 交易所代码
    char code[SIM_CODESIZE];                                  // 证券代码
    SimOrder* next;                                           // 下一个订单数据
};

struct SimPosition
{
    double price;                                              // 持仓成本
    int64_t sellable_qty;                                      // 可卖数量
    int64_t total_qty;                                         // 总持仓
    int account_id;                                            // 账户编号
    int exchange_id;                                           // 交易所代码
    char code[SIM_CODESIZE];                                   // 股票持仓
    SimPosition* next;                                         // 下一个持仓数据
};

struct SimTradeReport
{
    SimOrder*      order;                                       // 订单信息
    SimTickTrade*  trade;                                       // 成交信息
    SimTradeReport* next;                                       // 下一个成交数据
};