#include <stdio.h>
#include "Bridge.h"

/*
g++ Data_Struct.h Bridge.h Bridge.cpp Bridge_demo.cpp -o Bridge_demo.exe
*/

int main()
{   
    user_strategy u_s;
    buy_strategy b_s(&u_s);
    deque<Order> order_deque;
    for ( int i=0; i<20; i++ )
    {
        Quote quote{105 + i * 2, 100 + i};
        sprintf(quote.code, "%06d", 300000 + i);
        b_s.update_quote(quote);
    }
    b_s.send_order(order_deque);
    return 0;
}
