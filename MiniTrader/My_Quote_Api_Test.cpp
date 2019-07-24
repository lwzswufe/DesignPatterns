#include "My_Quote_Api.h"
#include <stdio.h> 

'''
g++ base_data_struct.h quote_api_struct.h Quote_api.h Quote_spi.h local_quote_struct.h read_local_quote.h local_quote_convert.h Quote_generator.h My_Quote
'''
int main()
{   
    const char filename[256]{"/home/wiz/data/ini.txt"};
    QuoteApi* api_ = QuoteApi::CreateQuoteApi(filename);
    QuoteSpi* spi_= new MyQuoteSpi();
    api_->RegisterSpi(spi);
    api_->login();
    api_->QueryAllTickers(EXCHANGE_SZ);
    while (api_->get_clock() < 150000)
    {
        ;
    }
    api_->logout();
    printf("%s test over", __FUNC__);
    return 0;
}