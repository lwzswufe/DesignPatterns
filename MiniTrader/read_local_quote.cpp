#include <stdio.h>
#include <string.h>
#include <math.h>
#include "read_local_quote.h"

StockInfo* read_stock_info_from_file(const char* filename, const int date)
{
    FILE * pFile;
    char line[QUOTE_LINESIZE];
    size_t total_num = 0, block_num = 0, block_count = 0;
    StockInfo* stockinfo_ptr_arr[QUOTE_BLOCKNUM];
    memset(stockinfo_ptr_arr, 0, sizeof(StockInfo *) * QUOTE_BLOCKNUM);
    pFile = fopen(filename , "r");
    if ( pFile == NULL ) 
    {
        perror("Error opening file");
        return NULL;
    }
    // 首次分配空间
    StockInfo* stockinfo_ptr = new StockInfo[QUOTE_BLOCKSIZE];
    stockinfo_ptr_arr[block_num] = stockinfo_ptr;
    block_num++;
    while ( ! feof (pFile) )
    {   
        // 读取到文件末尾
        if ( fgets (line , QUOTE_LINESIZE , pFile) == NULL ) 
            break;
        // 按行读取数据
        if ( read_stock_info_from_line(line, stockinfo_ptr, date) )
        {   
            
            block_count++;
            total_num++;
            stockinfo_ptr++;
            // 当前块block空间使用完毕 分配新block
            if ( block_count == QUOTE_BLOCKSIZE )
            {
                stockinfo_ptr = new StockInfo[QUOTE_BLOCKSIZE];
                stockinfo_ptr_arr[block_num] = stockinfo_ptr;
                block_num++;
                block_count = 0;
            }
        }
    }
    fclose (pFile);
    // 为所有数据分配新空间
    stockinfo_ptr = new StockInfo[total_num+1];
    size_t block_size;
    // 依次将每一个块的数据拷贝到新空间中去
    for (int block_id=0; block_id<block_num; block_id++)
    {   
        if (block_id < block_num - 1)
            block_size = QUOTE_BLOCKSIZE;
        else 
            block_size = block_count;
        memcpy(stockinfo_ptr + QUOTE_BLOCKSIZE * block_id, stockinfo_ptr_arr[block_id], sizeof(StockInfo) * block_size);
        delete[] stockinfo_ptr_arr[block_id];
    }
    // 末元素充值为0
    memset(stockinfo_ptr+total_num, 0, sizeof(StockInfo));
    printf("date:%d stock_num:%lu\n", date, total_num);
    return stockinfo_ptr;
}

// 从文件里读取深度行情
void read_market_data_from_file(const char* filename, MarketData* marketdata_ptr)
{

}

// 从文件里读取逐笔委托
void read_tickbytick_order_from_file(const char* filename, TickByTick* tickbytick)
{

}

// 从文件里读取逐笔成交
void read_tickbytick_trade_from_file(const char* filename, TickByTick* tickbytick)
{

}

bool read_stock_info_from_line(char* line, StockInfo* stockinfo_ptr, const int date)
{
    if (line == NULL || stockinfo_ptr == NULL || strlen(line) < 20)
        return false;
    // 获取市场与证券代码
    char market;
    market = line[8];
    switch(market)
    {
        case 'H': stockinfo_ptr->exchange_id = EXCHANGE_SH; break;
        case 'Z': stockinfo_ptr->exchange_id = EXCHANGE_SZ; break;
        default : return false;
    }
    strncpy(stockinfo_ptr->ticker, line, 6);
    stockinfo_ptr->ticker[6] = 0;
    // 获取日期与价格数据
    int date_;
    double preclose, uplimit;
    // 跳过前10个字符xxxxxx.SZ,
    line += 10;
    int match_num = sscanf(line, "%d,%*lf,%*lf,%*lf,%*lf,%lf,%*lf,%*lf,%*lf,%*lf,%lf", 
                          &date_, &(stockinfo_ptr->pre_close_price), &(stockinfo_ptr->upper_limit_price));
    if (match_num < 3 || date_ != date)
        return false;
    // 计算跌停价
    stockinfo_ptr->lower_limit_price = round(stockinfo_ptr->pre_close_price * 90 + 0.01) / 100.0;
    return true;
}