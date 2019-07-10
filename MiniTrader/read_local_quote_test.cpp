#include <stdio.h>
#include "read_local_quote.h"

/*
g++ base_data_struct.h quote_api_struct.h read_local_quote.h read_local_quote.cpp read_local_quote_test.cpp -o read_local_quote_test.exe
*/

int main()
{   
    // char file_stockinfo[256] = "F:\\data\\uplimit_list.csv";
    // int date = 20190110, stock_num = 0;
    // StockInfo* stockinfo_ptr = read_stock_info_from_file(file_stockinfo, date);
    // while (stockinfo_ptr->pre_close_price > 0)
    // {
    //     printf("code:%s Market:%d preclose:%.2lf\tuplimit:%.2lf\n", 
    //     stockinfo_ptr->ticker, stockinfo_ptr->exchange_id,  stockinfo_ptr->pre_close_price, stockinfo_ptr->upper_limit_price);
    //     stockinfo_ptr++;
    //     stock_num++;
    // }
    // printf("date:%d stock_num:%d\n", date, stock_num);

    char file_marketdata[256] = "D:\\data\\L2_data\\2017\\01\\03\\am_hq_snap_spot.csv";
    MarketData* marketdata_ptr = read_market_data_from_file(file_marketdata);
    int marketdata_num = 0, marketdata_flag = 1;
    while (marketdata_ptr->data_time > 0)
    {   
        marketdata_num++;
        if (marketdata_num >= marketdata_flag)
        {
            marketdata_flag *= 2;
            printf("code:%s Market:%d preclose:%.2lf\tuplimit:%.2lf\n", 
            marketdata_ptr->ticker, marketdata_ptr->exchange_id,  marketdata_ptr->pre_close_price, marketdata_ptr->upper_limit_price);
        }
        marketdata_ptr++;
    }
    printf("marketdata_num:%d\n", marketdata_num);
    return 0;
}