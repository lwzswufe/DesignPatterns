#include <stdio.h> 
#include <iostream>
using std::cout; using std::endl;
#include <exception>
using std::runtime_error; 
#include "My_Quote_Api.h"
#include "My_Trade_Api.h"
#include "sim_trade_convert.h"
#include "Strategy.h"

extern int GetTimenum();

int main_program()
{   
    ApiTimenum = 0;
    int last_timenum = 0, now_timenum;
    const char filename[256] = "Trade.ini";

    QuoteApi* api_quote = QuoteApi::CreateQuoteApi(filename);
    QuoteSpi* spi_quote = new MyQuoteSpi();
    api_quote->RegisterSpi(spi_quote);
    api_quote->Login();

    TradeApi* api_trade = TradeApi::CreateTradeApi(1);
    TradeSpi* spi_trade = new MyTradeSpi();
    api_trade->RegisterSpi(spi_trade);
    api_trade->Login();

    api_quote->QueryAllTickers(EXCHANGE_SZ);
    now_timenum = GetTimenum();

    api_trade->QueryOrders(NULL);
    api_trade->QueryPosition(NULL);

    printf("timenum:%d\n", now_timenum);
    while (now_timenum < 150000)
    {   
        if (last_timenum + 100 < now_timenum)
        {   
            last_timenum = now_timenum;
            printf("timenum:%d\n", now_timenum);
        }
        while(Strategy::send_order_num > Strategy::sended_order_num)
        {   
            SimOrder* simorder = Strategy::user_order_ptr_arr + Strategy::sended_order_num;
            OrderInsertInfo order_new, *orderinsert=&order_new;
            convert_order_struct(simorder, orderinsert);
            api_trade->InsertOrder(orderinsert);
            Strategy::sended_order_num++;
        }
        now_timenum = GetTimenum();
    }
    printf("timenum:%d  timeout\n", now_timenum);
    api_quote->Logout();
    api_trade->Logout();
    printf(">>>>>>>>%s test over<<<<<<<<<<<\n", __FILE__);
    return 0;
}


int main()
{
    try
    {
        main_program();
    }
    catch (runtime_error &err)
    {
        cout << err.what() << endl;
    }
    return 0;
}