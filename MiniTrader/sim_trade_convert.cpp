#include "sim_trade_convert.h"
#include <string.h>
#include <stdio.h>
// 将报单结构体转化为交易系统报单结构体
void convert_order_struct(const OrderInsertInfo* orderinfo, SimOrder* simorder)
{   
    if (orderinfo == NULL || simorder == NULL)
        return;
    simorder->state = SIM_ORDERSTATE_SENDED;
    simorder->price = orderinfo->price;
    simorder->vol = orderinfo->quantity;
    simorder->traded_vol = 0;
    simorder->seq = 0;
    simorder->id = 0;
    simorder->account_id = 0;
    simorder->order_time = 0.0;
    strncpy(simorder->code, orderinfo->ticker, SIM_CODESIZE);
    switch (orderinfo->market)
    {
    case MKT_SH_A:
        simorder->exchange_id = SIM_EXCHANGE_SH;    
        break;
    case MKT_SZ_A:
        simorder->exchange_id = SIM_EXCHANGE_SZ;
        break;
    default:
        printf("error market %d in file:%s line:%d\n", orderinfo->market, __FILE__, __LINE__);
    }

    switch (orderinfo->side)
    {
    case SIDE_BUY:
        simorder->side = SIM_ORDERSIDE_BUY;
        break;
    case SIDE_SELL:
        simorder->side = SIM_ORDERSIDE_SELL;
        break;
    default:
        printf("error order type %d in file:%s line:%d \n", orderinfo->side, __FILE__, __LINE__);
        break;
    }
}

void convert_order_struct(const SimOrder* simorder, OrderInfo* orderinfo)
{   
    if (orderinfo == NULL || simorder == NULL)
        return;
    orderinfo->order_client_id = 0;
    strncpy(orderinfo->ticker, simorder->code, SIM_CODESIZE);
    orderinfo->price = simorder->price;
    orderinfo->quantity = simorder->vol;
    orderinfo->price_type = PRICE_LIMIT; // 默认限价单
    orderinfo->business_type = BUSINESS_TYPE_CASH; // 默认普通股票业务

    switch (simorder->exchange_id)
    {
    case SIM_EXCHANGE_SH:
        orderinfo->market = MKT_SZ_A;    
        break;
    case SIM_EXCHANGE_SZ:
        orderinfo->market = MKT_SH_A;
        break;
    default:
        printf("error market %d in file:%s line:%d\n", simorder->exchange_id, __FILE__, __LINE__);
    }

    switch (simorder->side)
    {
    case SIM_ORDERSIDE_BUY:
        orderinfo->side = SIDE_BUY;        // 默认买入开仓
        orderinfo->position_effect = POSITION_EFFECT_OPEN;
        break;
    case SIM_ORDERSIDE_SELL:
        orderinfo->side = SIDE_SELL;       // 默认卖出平仓
        orderinfo->position_effect = POSITION_EFFECT_CLOSE;
        break;
    default:
        printf("error order type %d in file:%s line:%d \n", simorder->type, __FILE__, __LINE__);
        break;
    }
}

// 将成交信息转化为成交通知
void convert_trade_struct(const SimTradeReport* simtradereport, TradeReport* tradereport)
{
    if (simtradereport == NULL || tradereport == NULL)
        return;
    const SimOrder* simorder = simtradereport->order;
    const SimTickTrade* simtrade = simtradereport->trade;
    
    if (simorder == NULL || simtrade == NULL)
        return;
    
    tradereport->order__id = simorder->id;
    strncpy(tradereport->ticker, simorder->code, SIM_CODESIZE);
    tradereport->report_index = simtrade->seq;
    tradereport->price = simtrade->price;
    tradereport->quantity = simtrade->qty;
    tradereport->trade_time = simtrade->data_time;
    tradereport->trade_amount = simtrade->qty * simtrade->price;
    tradereport->report_index = simtrade->seq;
    tradereport->trade_type = TRDT_COMMON;  // 默认普通成交
    
    switch (simorder->side)
    {
    case SIM_ORDERSIDE_BUY:
        tradereport->side = SIDE_BUY;        // 默认买入开仓
        tradereport->position_effect = POSITION_EFFECT_OPEN;
        break;
    case SIM_ORDERSIDE_SELL:
        tradereport->side = SIDE_SELL;       // 默认卖出平仓
        tradereport->position_effect = POSITION_EFFECT_CLOSE;
        break;
    default:
        printf("error order type %d in file:%s line:%d \n", simorder->type, __FILE__, __LINE__);
        break;
    }
    
    switch (simorder->exchange_id)
    {
    case SIM_EXCHANGE_SH:
        tradereport->market = MKT_SZ_A;    
        break;
    case SIM_EXCHANGE_SZ:
        tradereport->market = MKT_SH_A;
        break;
    default:
        printf("error market %d in file:%s line:%d\n", simorder->exchange_id, __FILE__, __LINE__);
    }
}