#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "read_local_quote.h"

// 将一行数据拆解为字符串数据 数据中无效数据为NULL
void split_line_to_word_array(char* line, const char* separator, char** word_array, const int word_num)
{
    char *token = NULL;                     // 找到的字符串
    int i = 0;
    token = strtok(line, separator);
    while (token != NULL && i< word_num)
    {   
        word_array[i] = token;
        i++;
        token = strtok(NULL, separator);
    }
    // 子字符串数量小于word_num word_array 未赋值部分赋值为NULL 
    for(; i< word_num; i++)
        word_array[i] = NULL;
}

StockInfo* read_stock_info_from_file(const char* filename, const int date, int *array_size)
{
    FILE * pFile;
    char line[QUOTE_LINESIZE];
    size_t total_num = 0, block_num = 0, block_count = 0;
    StockInfo* stockinfo_ptr_arr[QUOTE_BLOCKNUM];
    memset(stockinfo_ptr_arr, 0, sizeof(StockInfo *) * QUOTE_BLOCKNUM);
    pFile = fopen(filename , "r");
    if ( pFile == NULL ) 
    {   
        char s[256];
        sprintf(s, "Error opening file:%s", filename);
        perror(s);
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
    *array_size = total_num;
    // 末元素充值为0
    memset(stockinfo_ptr+total_num, 0, sizeof(StockInfo));
    printf("date:%d stock_num:%lu\n", date, total_num);
    return stockinfo_ptr;
}

// 从文件里读取深度行情
SnapData* read_snap_data_from_file(const char* filename, int *array_size)
{
    FILE * pFile;
    char line[QUOTE_LINESIZE];
    size_t total_num = 0, block_num = 0, block_count = 0;
    SnapData* snapdata_ptr_arr[QUOTE_BLOCKNUM];
    memset(snapdata_ptr_arr, 0, sizeof(SnapData *) * QUOTE_BLOCKNUM);
    pFile = fopen(filename , "r");
    if ( pFile == NULL ) 
    {
        char s[256];        
        sprintf(s, "Error opening file:%s", filename);         
        perror(s);
        return NULL;
    }
    // 首次分配空间
    SnapData* snapdata_ptr = new SnapData[QUOTE_BLOCKSIZE];
    snapdata_ptr_arr[block_num] = snapdata_ptr;
    block_num++;
    while ( ! feof (pFile) )
    {   
        // 读取到文件末尾
        if ( fgets (line , QUOTE_LINESIZE , pFile) == NULL ) 
            break;
        // 按行读取数据
        if ( read_snap_data_from_line(line, snapdata_ptr) )
        {   
            
            block_count++;
            total_num++;
            snapdata_ptr++;
            // 当前块block空间使用完毕 分配新block
            if ( block_count == QUOTE_BLOCKSIZE )
            {
                snapdata_ptr = new SnapData[QUOTE_BLOCKSIZE];
                snapdata_ptr_arr[block_num] = snapdata_ptr;
                block_num++;
                block_count = 0;
            }
        }
    }
    fclose (pFile);
    // 为所有数据分配新空间
    snapdata_ptr = new SnapData[total_num+1];
    size_t block_size;
    // 依次将每一个块的数据拷贝到新空间中去
    for (int block_id=0; block_id<block_num; block_id++)
    {   
        if (block_id < block_num - 1)
            block_size = QUOTE_BLOCKSIZE;
        else 
            block_size = block_count;
        memcpy(snapdata_ptr + QUOTE_BLOCKSIZE * block_id, snapdata_ptr_arr[block_id], sizeof(SnapData) * block_size);
        delete[] snapdata_ptr_arr[block_id];
    }
    *array_size = total_num;
    // 末元素充值为0
    memset(snapdata_ptr+total_num, 0, sizeof(SnapData));
    printf("get %ld snap data\n", total_num);
    return snapdata_ptr;
}

LevelData* read_level_data_from_file(const char* filename, int *array_size)
{
    FILE * pFile;
    char line[QUOTE_LINESIZE];
    size_t total_num = 0, block_num = 0, block_count = 0;
    LevelData* leveldata_ptr_arr[QUOTE_BLOCKNUM];
    memset(leveldata_ptr_arr, 0, sizeof(LevelData *) * QUOTE_BLOCKNUM);
    pFile = fopen(filename , "r");
    if ( pFile == NULL ) 
    {
        char s[256];         
        sprintf(s, "Error opening file:%s", filename);         
        perror(s);
        return NULL;
    }
    // 首次分配空间
    LevelData* leveldata_ptr = new LevelData[QUOTE_BLOCKSIZE];
    leveldata_ptr_arr[block_num] = leveldata_ptr;
    block_num++;
    while ( ! feof (pFile) )
    {   
        // 读取到文件末尾
        if ( fgets (line , QUOTE_LINESIZE , pFile) == NULL ) 
            break;
        // 按行读取数据
        if ( read_level_data_from_line(line, leveldata_ptr) )
        {   
            block_count++;
            total_num++;
            leveldata_ptr++;
            // 当前块block空间使用完毕 分配新block
            if ( block_count == QUOTE_BLOCKSIZE )
            {
                leveldata_ptr = new LevelData[QUOTE_BLOCKSIZE];
                leveldata_ptr_arr[block_num] = leveldata_ptr;
                block_num++;
                block_count = 0;
            }
        }
    }
    fclose (pFile);
    // 为所有数据分配新空间
    leveldata_ptr = new LevelData[total_num+1];
    size_t block_size;
    // 依次将每一个块的数据拷贝到新空间中去
    for (int block_id=0; block_id<block_num; block_id++)
    {   
        if (block_id < block_num - 1)
            block_size = QUOTE_BLOCKSIZE;
        else 
            block_size = block_count;
        memcpy(leveldata_ptr + QUOTE_BLOCKSIZE * block_id, leveldata_ptr_arr[block_id], sizeof(LevelData) * block_size);
        delete[] leveldata_ptr_arr[block_id];
    }
    *array_size = total_num;
    // 末元素充值为0
    memset(leveldata_ptr+total_num, 0, sizeof(LevelData));
    printf("get %ld level data\n", total_num);
    return leveldata_ptr;
}


// 从文件里读取逐笔委托
TickOrder* read_tick_order_from_file(const char* filename, int *array_size)
{
    FILE * pFile;
    char line[QUOTE_LINESIZE];
    size_t total_num = 0, block_num = 0, block_count = 0;
    TickOrder* tickorder_ptr_arr[QUOTE_BLOCKNUM];
    memset(tickorder_ptr_arr, 0, sizeof(TickOrder *) * QUOTE_BLOCKNUM);
    pFile = fopen(filename , "r");
    if ( pFile == NULL ) 
    {
        char s[256];         
        sprintf(s, "Error opening file:%s", filename);         
        perror(s);
        return NULL;
    }
    // 首次分配空间
    TickOrder* tickorder_ptr = new TickOrder[QUOTE_BLOCKSIZE];
    tickorder_ptr_arr[block_num] = tickorder_ptr;
    block_num++;
    while ( ! feof (pFile) )
    {   
        // 读取到文件末尾
        if ( fgets (line , QUOTE_LINESIZE , pFile) == NULL ) 
            break;
        // 按行读取数据
        if ( read_tick_order_from_line(line, tickorder_ptr) )
        {   
            block_count++;
            total_num++;
            tickorder_ptr++;
            // 当前块block空间使用完毕 分配新block
            if ( block_count == QUOTE_BLOCKSIZE )
            {
                tickorder_ptr = new TickOrder[QUOTE_BLOCKSIZE];
                tickorder_ptr_arr[block_num] = tickorder_ptr;
                block_num++;
                block_count = 0;
            }
        }
    }
    fclose (pFile);
    // 为所有数据分配新空间
    tickorder_ptr = new TickOrder[total_num+1];
    size_t block_size;
    // 依次将每一个块的数据拷贝到新空间中去
    for (int block_id=0; block_id<block_num; block_id++)
    {   
        if (block_id < block_num - 1)
            block_size = QUOTE_BLOCKSIZE;
        else 
            block_size = block_count;
        memcpy(tickorder_ptr + QUOTE_BLOCKSIZE * block_id, tickorder_ptr_arr[block_id], sizeof(TickOrder) * block_size);
        delete[] tickorder_ptr_arr[block_id];
    }
    *array_size = total_num;
    // 末元素充值为0
    memset(tickorder_ptr+total_num, 0, sizeof(TickOrder));
    printf("get %ld tickorder data\n", total_num);
    return tickorder_ptr;
}

// 从文件里读取逐笔成交
TickTrade* read_tick_trade_from_file(const char* filename, int *array_size)
{
    FILE * pFile;
    char line[QUOTE_LINESIZE];
    size_t total_num = 0, block_num = 0, block_count = 0;
    TickTrade* ticktrade_ptr_arr[QUOTE_BLOCKNUM];
    memset(ticktrade_ptr_arr, 0, sizeof(TickTrade *) * QUOTE_BLOCKNUM);
    pFile = fopen(filename , "r");
    if ( pFile == NULL ) 
    {
        char s[256];         
        sprintf(s, "Error opening file:%s", filename);         
        perror(s);
        return NULL;
    }
    // 首次分配空间
    TickTrade* ticktrade_ptr = new TickTrade[QUOTE_BLOCKSIZE];
    ticktrade_ptr_arr[block_num] = ticktrade_ptr;
    block_num++;
    while ( ! feof (pFile) )
    {   
        // 读取到文件末尾
        if ( fgets (line , QUOTE_LINESIZE , pFile) == NULL ) 
            break;
        // 按行读取数据
        if ( read_tick_trade_from_line(line, ticktrade_ptr) )
        {   
            block_count++;
            total_num++;
            ticktrade_ptr++;
            // 当前块block空间使用完毕 分配新block
            if ( block_count == QUOTE_BLOCKSIZE )
            {
                ticktrade_ptr = new TickTrade[QUOTE_BLOCKSIZE];
                ticktrade_ptr_arr[block_num] = ticktrade_ptr;
                block_num++;
                block_count = 0;
            }
        }
    }
    fclose (pFile);
    // 为所有数据分配新空间
    ticktrade_ptr = new TickTrade[total_num+1];
    size_t block_size;
    // 依次将每一个块的数据拷贝到新空间中去
    for (int block_id=0; block_id<block_num; block_id++)
    {   
        if (block_id < block_num - 1)
            block_size = QUOTE_BLOCKSIZE;
        else 
            block_size = block_count;
        memcpy(ticktrade_ptr + QUOTE_BLOCKSIZE * block_id, ticktrade_ptr_arr[block_id], sizeof(TickTrade) * block_size);
        delete[] ticktrade_ptr_arr[block_id];
    }
    *array_size = total_num;
    // 末元素充值为0
    memset(ticktrade_ptr+total_num, 0, sizeof(TickTrade));
    printf("get %ld ticktrade data\n", total_num);
    return ticktrade_ptr;
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
        case 'H': stockinfo_ptr->exchange_id = STOCK_EXCHANGE_SH; break;
        case 'Z': stockinfo_ptr->exchange_id = STOCK_EXCHANGE_SZ; break;
        default : return false;
    }
    strncpy(stockinfo_ptr->code, line, 6);
    stockinfo_ptr->code[6] = 0;
    // 获取日期与价格数据
    int date_;
    double preclose, uplimit;
    // 跳过前10个字符xxxxxx.SZ,
    line += 10;
    // 获取证券代码与价格信息
    const int word_num = 11;
    char * word_array[word_num];
    const char *tab = ",";
    split_line_to_word_array(line, tab, word_array, word_num);

    date_ = atoi(word_array[0]);
    stockinfo_ptr->pre_close_price = atof(word_array[5]);
    stockinfo_ptr->upper_limit_price = atof(word_array[10]);

    if (date_ != date)
        return false;
    // 计算跌停价
    stockinfo_ptr->lower_limit_price = round(stockinfo_ptr->pre_close_price * 90 + 0.01) / 100.0;
    return true;
}

// 读取深度行情
bool read_snap_data_from_line(char* line, SnapData* ptr)
{
    if (line == NULL || ptr == NULL || strlen(line) < 100)
        return false;
    // 获取时间
    int match_num, codenum;
    line += 9;
    sscanf(line, "%ld", &(ptr->data_time));
  
    // 
    line += 72;
    // 获取证券代码与价格信息
    const int word_num = 26;
    char * word_array[word_num];
    const char *tab = "\t";
    split_line_to_word_array(line, tab, word_array, word_num);

    codenum = atoi(word_array[1]);
    ptr->pre_close_price = atof(word_array[4]);
    ptr->open_price = atof(word_array[7]);
    ptr->high_price = atof(word_array[8]);
    ptr->low_price = atof(word_array[9]);
    ptr->last_price = atof(word_array[10]);
    ptr->qty = atoi(word_array[12]);
    ptr->turnover = atof(word_array[13]);
    ptr->upper_limit_price = atof(word_array[24]);
    ptr->lower_limit_price = atof(word_array[25]);
    // 获取市场与证券代码

    char code[32];
    sprintf(code, "%06d", codenum);
    switch(code[0])
    {
        case '0': 
        case '3': ptr->exchange_id = STOCK_EXCHANGE_SZ; break;
        case '6': ptr->exchange_id = STOCK_EXCHANGE_SH; break;
        default : return false;
    }
    strncpy(ptr->code, code, 6);
    ptr->code[6] = 0;
    return true;
}

bool read_level_data_from_line(char* line, LevelData* ptr)
{
     if (line == NULL || ptr == NULL || strlen(line) < 100)
        return false;
    // 获取时间
    // 跳过日期字段
    line += 9;
    sscanf(line, "%ld", &(ptr->data_time));
    // 跳过时间字段
    line += 72;
    // 获取价格信息
    const int word_num = 50, vol_num = 50;
    char * word_array[word_num], *vol_array[vol_num];
    const char *tab = "\t", *vertical_bar = "|";
    split_line_to_word_array(line, tab, word_array, word_num);
    // 数据验证
    if (word_array[word_num - 2] == NULL)
        return false;
    // 获取市场与证券代码
    char code[8];
    strncpy(code, word_array[1], 6);
    switch(code[0])
    {
        case '0': 
        case '3': ptr->exchange_id = STOCK_EXCHANGE_SZ; break;
        case '6': ptr->exchange_id = STOCK_EXCHANGE_SH; break;
        default : return false;
    }
    strncpy(ptr->code, code, 6);
    ptr->code[6] = 0;
    // 从word_array的第idx元素 获取 第i档报价 
    int idx = 4;
    for (int i=0; i<10; i++)
    {
        ptr->ask[i] = atof(word_array[idx++]);
        ptr->bid[i] = atof(word_array[idx++]);
        ptr->ask_qty[i] = atoi(word_array[idx++]);
        ptr->bid_qty[i] = atoi(word_array[idx++]);
    }
    // 读取买队列
    ptr->max_bid1_count = atoi(word_array[idx++]);
    ptr->bid1_count = atoi(word_array[idx++]);
    split_line_to_word_array(word_array[idx++], vertical_bar, vol_array, vol_num);
    for (int i=0; i<ptr->bid1_count && i<vol_num && vol_array[i] != NULL; i++)
        ptr->bid1_qty[i] = atoi(vol_array[i]);
    // 读取卖队列
    ptr->max_ask1_count = atoi(word_array[idx++]);
    ptr->ask1_count = atoi(word_array[idx++]);
    split_line_to_word_array(word_array[idx++], vertical_bar, vol_array, vol_num);
    for (int i=0; i<ptr->ask1_count && i<vol_num && vol_array[i] != NULL; i++)
        ptr->ask1_qty[i] = atoi(vol_array[i]);

    return true;
}

bool read_tick_order_from_line(char* line, TickOrder* ptr)
{
    if (line == NULL || ptr == NULL || strlen(line) < 100)
        return false;
    int match_num, codenum;
    // 获取时间
    // 跳过日期字段
    line += 9;
    sscanf(line, "%ld", &(ptr->data_time));
    // 跳过时间字段
    line += 72;
    match_num = sscanf(line, "%*d\t%*d\t%ld\t%d\t%*d\t%lf\t%ld\t%c\t%c",
                        &(ptr->seq), &codenum, &(ptr->price), &(ptr->qty), &(ptr->side), &(ptr->type));
    if (match_num < 6)
        return false;
    // 获取市场与证券代码
    char code[32];
    sprintf(code, "%06d", codenum);
    switch(code[0])
    {
        case '0': 
        case '3': ptr->exchange_id = STOCK_EXCHANGE_SZ; break;
        case '6': ptr->exchange_id = STOCK_EXCHANGE_SH; break;
        default : return false;
    }
    strncpy(ptr->code, code, 6);
    ptr->code[6] = 0;
    return true;
}

bool read_tick_trade_from_line(char* line, TickTrade* ptr)
{
    if (line == NULL || ptr == NULL || strlen(line) < 100)
        return false;
    int match_num, codenum;
    // 获取时间
    // 跳过日期字段
    line += 9;
    sscanf(line, "%ld", &(ptr->data_time));
    // 跳过时间字段
    line += 72;
    match_num = sscanf(line, "%*d\t%*d\t%ld\t%d\t%*d\t%ld\t%ld\t%lf\t%ld\t%c",
                        &(ptr->seq), &codenum, &(ptr->bid_no), &(ptr->ask_no), &(ptr->price), &(ptr->qty), &(ptr->type));
    if (match_num < 6)
        return false;
    // 获取市场与证券代码
    char code[32];
    sprintf(code, "%06d", codenum);
    switch(code[0])
    {
        case '0': 
        case '3': ptr->exchange_id = STOCK_EXCHANGE_SZ; break;
        case '6': ptr->exchange_id = STOCK_EXCHANGE_SH; break;
        default : return false;
    }
    strncpy(ptr->code, code, 6);
    ptr->code[6] = 0;
    return true;
}