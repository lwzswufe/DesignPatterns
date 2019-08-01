#include "local_trade_struct.h"

class Trade_generator
{
public:
    // 发送订单
    TickOrder* insert_order(LocalOrder* order);
    // 撤销订单
    TickTrade* cancel_order(LocalOrder* order);
    

private:
    // 模拟成交 返回成交量 成交量
    int64_t simulate_trade(const TickTrade* ticktrade, LocalOrder* order);
    // 储存已报订单
    LocalOrder order_arr[512];
};