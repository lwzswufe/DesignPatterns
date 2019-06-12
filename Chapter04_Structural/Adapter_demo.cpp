#include <stdio.h>
#include "Adapter.h"

/*
g++ Data_Struct.h Adapter.h Adapter.cpp Adapter_demo.cpp -o Adapter_demo.exe
*/

int main()
{   
    buy_strategy b_s;
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