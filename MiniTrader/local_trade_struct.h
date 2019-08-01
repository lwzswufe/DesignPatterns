//定义本地交易类相关数据结构
#include "local_quote_struct.h"

typedef char LocalOrderType;
typedef char LocalOrderState;

struct LocalOrder
{
    LocalOrderType type;                                      // 订单类型
    LocalOrderState state;                                    // 订单状态
    int64_t id;                                               // 订单编号 每一个订单唯一
    int64_t vol;                                              // 委托量
    int64_t traded_vol;                                       // 已成交量
    double price;                                             // 委托价格
    double order_time;                                        // 报单时间
    int account_id;                                           // 账户编号
    int exchange_id;                                          // 交易所代码
    char code[LOCAL_CODESIZE];                                // 证券代码
};

struct LocalPosition
{
    double price;                                              // 持仓成本
    int64_t sellable_qty;                                      // 可卖数量
    int64_t total_qty;                                         // 总持仓
    int account_id;                                            // 账户编号
    int exchange_id;                                           // 交易所代码
    char code[LOCAL_CODESIZE];                                 // 股票持仓
};