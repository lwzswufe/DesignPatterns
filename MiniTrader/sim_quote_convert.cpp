#include "sim_quote_convert.h"
#include <string.h>

void convert_stockinfo(const SimStockInfo* stockinfo_ptr, StockStaticInfo * stockstaticinfo_ptr, bool* is_last)
{   
    // 交易所
    switch(stockinfo_ptr->exchange_id)
    {
        case SIM_EXCHANGE_SZ: stockstaticinfo_ptr->exchange_id=EXCHANGE_SZ; break;
        case SIM_EXCHANGE_SH: stockstaticinfo_ptr->exchange_id=EXCHANGE_SH; break;
        default:                stockstaticinfo_ptr->exchange_id=EXCHANGE_UNKNOWN;
    }
    // 合约代码
    strncpy(stockstaticinfo_ptr->ticker, stockinfo_ptr->code, SIM_CODESIZE);
    // 合约名称
    strncpy(stockstaticinfo_ptr->ticker_name, stockinfo_ptr->name, SIM_NAMESIZE);
    // 昨收盘
    stockstaticinfo_ptr->pre_close_price = stockinfo_ptr->pre_close_price;
    // 涨停板价
    stockstaticinfo_ptr->upper_limit_price = stockinfo_ptr->upper_limit_price;
    // 跌停板价
    stockstaticinfo_ptr->lower_limit_price = stockinfo_ptr->lower_limit_price;
	// 最小变动价位
	stockstaticinfo_ptr->price_tick = 0.01;

    if (stockinfo_ptr->next != NULL)
        *is_last = false;
    else
        *is_last = true;
}

// 转换深度行情数据
void convert_depthdata(const SimSnapData* snap_ptr, const SimLevelData* level_ptr, DepthData * data_ptr)
{
    // 交易所
    switch(snap_ptr->exchange_id)
    {
        case SIM_EXCHANGE_SZ: data_ptr->exchange_id=EXCHANGE_SZ; break;
        case SIM_EXCHANGE_SH: data_ptr->exchange_id=EXCHANGE_SH; break;
        default:                data_ptr->exchange_id=EXCHANGE_UNKNOWN;
    }
    // 合约代码
    strncpy(data_ptr->ticker, snap_ptr->code, SIM_CODESIZE);

    // 价格
	// 最新价
	data_ptr->last_price = snap_ptr->last_price;
	// 昨收盘
	data_ptr->pre_close_price = snap_ptr->pre_close_price;
	// 今开盘
	data_ptr->open_price = snap_ptr->open_price;
	// 最高价
	data_ptr->high_price = snap_ptr->high_price;
	// 最低价
	data_ptr->low_price = snap_ptr->low_price;
    // 今收盘
    data_ptr->close_price = snap_ptr->close_price;

	// 涨跌停
	// 涨停价
	data_ptr->upper_limit_price = snap_ptr->upper_limit_price;
	// 跌停价
	data_ptr->lower_limit_price = snap_ptr->lower_limit_price;

    // 量额数据
    ///数量，为总成交量（单位股，与交易所一致）
    data_ptr->qty = snap_ptr->qty;
    ///成交金额，为总成交金额（单位元，与交易所一致）
    data_ptr->turnover = snap_ptr->turnover;
    ///当日均价=(turnover/qty)
    data_ptr->avg_price = snap_ptr->avg_price;

    //  时间类，格式为YYYYMMDDHHMMSSsss
    int64_t data_time = snap_ptr->data_time;

    // 买卖盘
    // 十档申买价
    memcpy(data_ptr->bid, level_ptr->bid, 10 * sizeof(double));
    // 十档申卖价
    memcpy(data_ptr->ask, level_ptr->ask, 10 * sizeof(double));
    // 十档申买量
    memcpy(data_ptr->bid_qty, level_ptr->bid_qty, 10 * sizeof(int64_t));
    // 十档申卖量
    memcpy(data_ptr->ask_qty, level_ptr->ask_qty, 10 * sizeof(int64_t));

    // 买卖队列
    // 买一队列数据
    memcpy(data_ptr->bid1_qty, level_ptr->bid1_qty, 50 * sizeof(int64_t));
    // 买一队列的有效委托笔数	
    data_ptr->bid1_count = level_ptr->bid1_count;
    // 买一队列总委托笔数
    data_ptr->max_bid1_count = level_ptr->max_ask1_count;
    // 卖一队列数据
    memcpy(data_ptr->ask1_qty, level_ptr->ask1_qty, 50 * sizeof(int64_t));
    // 卖一队列的有效委托笔数
    data_ptr->ask1_count = level_ptr->ask1_count;
    // 卖一队列总委托笔数
    data_ptr->max_ask1_count = level_ptr->max_ask1_count;
}

// 转换委托流数据
void convert_tickorder(const SimTickOrder* tickorder_ptr, TickByTickStruct * tick_ptr)
{
    // 交易所
    switch(tickorder_ptr->exchange_id)
    {
        case SIM_EXCHANGE_SZ: tick_ptr->exchange_id=EXCHANGE_SZ; break;
        case SIM_EXCHANGE_SH: tick_ptr->exchange_id=EXCHANGE_SH; break;
        default:                tick_ptr->exchange_id=EXCHANGE_UNKNOWN;
    }
    // 合约代码
    strncpy(tick_ptr->ticker, tickorder_ptr->code, SIM_CODESIZE);

    // 委托时间 or 成交时间
    tick_ptr->data_time = tickorder_ptr->data_time;
    // 委托 or 成交
    tick_ptr->type = TBT_ENTRUST;
    ///委托序号(在同一个channel_no内唯一，从1开始连续)
    tick_ptr->order.seq = tickorder_ptr->seq;
    ///委托价格
    tick_ptr->order.price = tickorder_ptr->price;
    ///委托数量
    tick_ptr->order.qty = tickorder_ptr->qty;
    ///'1':买; '2':卖; 'G':借入; 'F':出借
    tick_ptr->order.side = tickorder_ptr->side;
    ///订单类别: '1': 市价; '2': 限价; 'U': 本方最优
    tick_ptr->order.type = tickorder_ptr->type;
}

// 转换成交流数据
void convert_ticktrade(const SimTickTrade* ticktrade_ptr, TickByTickStruct * tick_ptr)
{
        // 交易所
    switch(ticktrade_ptr->exchange_id)
    {
        case SIM_EXCHANGE_SZ: tick_ptr->exchange_id=EXCHANGE_SZ; break;
        case SIM_EXCHANGE_SH: tick_ptr->exchange_id=EXCHANGE_SH; break;
        default:                tick_ptr->exchange_id=EXCHANGE_UNKNOWN;
    }
    // 合约代码
    strncpy(tick_ptr->ticker, ticktrade_ptr->code, SIM_CODESIZE);

    // 委托时间 or 成交时间
    tick_ptr->data_time = ticktrade_ptr->data_time;
    // 委托 or 成交
    tick_ptr->type = TBT_TRADE;
    // 成交序号(在同一个channel_no内唯一，从1开始连续)
    tick_ptr->trade.seq = ticktrade_ptr->seq;
    // 成交价格
    tick_ptr->trade.price = ticktrade_ptr->price;
    // 成交数量
    tick_ptr->trade.qty = ticktrade_ptr->qty;
    ///成交金额(仅适用上交所)
    tick_ptr->trade.money = ticktrade_ptr->money;
    // 买方订单号
    tick_ptr->trade.bid_no = ticktrade_ptr->bid_no;
    // 卖方订单号
    tick_ptr->trade.ask_no = ticktrade_ptr->ask_no;
    // SH: 内外盘标识('B':主动买; 'S':主动卖; 'N':未知)
    // SZ: 成交标识('4':撤; 'F':成交)
    tick_ptr->trade.type = ticktrade_ptr->type;
}