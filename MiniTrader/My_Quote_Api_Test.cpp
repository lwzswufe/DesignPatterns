#include <stdio.h> 
#include <iostream>
using std::cout; using std::endl;
#include <exception>
using std::runtime_error; 
#include "My_Quote_Api.h"
#include "My_Trade_Api.h"
/* 
g++ base_data_struct.h quote_api_struct.h Quote_api.h Quote_spi.h local_quote_struct.h read_local_quote.h local_quote_convert.h Quote_generator.h My_Quote
*/
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
    printf("timenum:%d\n", now_timenum);
    while (GetTimenum() < 145700)
    {
        if (last_timenum + 100 < now_timenum)
        {   
            last_timenum = now_timenum;
            printf("timenum:%d\n", now_timenum);
        }
        now_timenum = GetTimenum();
    }
    printf("timenum:%d  timeout\n", now_timenum);
    api_quote->Logout();
    api_trade->Logout();
    printf("%s test over", __FILE__);
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