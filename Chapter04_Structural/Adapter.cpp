#include <string.h>
#include <stdio.h>
#include "Adapter.h"

void user_strategy::strategy_1(Quote quotation)
{
    if (quotation.trade_pr * 1.00 > quotation.settlement * 1.07)
    {
        Order order{quotation.trade_pr, 100, 0};
        strncpy(order.code, quotation.code, CODESIZE);
        this->order_vec.push_back(order);
    }
}

buy_strategy::buy_strategy()
{
    ;
}

void buy_strategy::update_quote(Quote quotation)
{
    this->strategy_1(quotation);
}

// buy_strategy::order_vec this->order_vec 均可访问 order_vec 
void buy_strategy::send_order(deque<Order>& order_deque)
{   
    printf("order_vec size:%lu %lu\n", buy_strategy::order_vec.size(), this->order_vec.size());
    for (Order order: this->order_vec)
        order_deque.push_back(order);
    this->order_vec.clear();
    printf("order_vec size:%lu %lu\n", buy_strategy::order_vec.size(), this->order_vec.size());
}