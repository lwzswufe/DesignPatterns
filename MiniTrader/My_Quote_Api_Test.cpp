#include <stdio.h> 
#include <iostream>
using std::cout; using std::endl;
#include <exception>
using std::runtime_error; 
#include "My_Quote_Api.h"
/* 
g++ base_data_struct.h quote_api_struct.h Quote_api.h Quote_spi.h local_quote_struct.h read_local_quote.h local_quote_convert.h Quote_generator.h My_Quote
*/
extern int GetTimenum();

int main_program()
{   
    int last_timenum = 0, now_timenum;
    const char filename[256] = "Trade.ini";
    QuoteApi* api_ = QuoteApi::CreateQuoteApi(filename);
    QuoteSpi* spi_= new MyQuoteSpi();
    api_->RegisterSpi(spi_);
    api_->Login();
    api_->QueryAllTickers(EXCHANGE_SZ);
    now_timenum = GetTimenum();
    printf("timenum:%d\n", now_timenum);
    while (GetTimenum() < 145700)
    {
        if (last_timenum + 500 < now_timenum)
        {   
            last_timenum = now_timenum;
            printf("timenum:%d\n", now_timenum);
        }
        now_timenum = GetTimenum();
    }
    api_->Logout();
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