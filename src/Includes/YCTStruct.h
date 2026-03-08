/*!
 * \file YCTStruct.h
 * \project	yubaoCppTrader
 *
 * \author yubao
 * \date 2026/03/08
 * 
 * \brief yubaoCppTrader基础结构体定义
 */
#pragma once
#include <memory>
#include <stdint.h>
#include <string.h>
#include "YCTTypes.h"

#ifdef _MSC_VER
#pragma warning(disable:4200)
#endif

NS_YCT_BEGIN

#pragma pack(push, 8)

struct YCTBarStruct
{
    YCTBarStruct()
    {
        memset(this, 0, sizeof(YCTBarStruct));
    }

	uint32_t	date;		//日期
	uint32_t	reserve_;	//占位符
	uint64_t	time;		//时间
	double		open;		//开
	double		high;		//高
	double		low;		//低
	double		close;		//收
	double		settle;		//结算
	
	double		money;		//成交金额
	double		vol;		//成交量

    union
	{
		double		hold;	//总持
		double		bid;	//买价，期权专用，主要期权spread比较大。
	};

	union
	{
		double		add;	//增仓
		double		ask;	//卖价，期权专用，主要期权spread比较大。
	};
};

struct YCTTickStruct
{
    YCTTickStruct()
    {
        memset(this, 0, sizeof(YCTTickStruct));
    }
    
	char		exchg[MAX_EXCHANGE_LENGTH];
	char		code[MAX_INSTRUMENT_LENGTH];

	double		price;				//最新价
	double		open;				//开盘价
	double		high;				//最高价
	double		low;				//最低价
	double		settle_price;		//结算价

	double		upper_limit;		//涨停价
	double		lower_limit;		//跌停价

	double		total_volume;		//总成交量
	double		volume;				//成交量
	double		total_turnover;		//总成交额
	double		turn_over;			//成交额
	double		open_interest;		//总持
	double		diff_interest;		//增仓

	uint32_t	trading_date;		//交易日,如20140327
	uint32_t	action_date;		//自然日期,如20140327
	uint32_t	action_time;		//发生时间,精确到毫秒,如105932000
	uint32_t	reserve_;			//占位符

	double		pre_close;			//昨收价
	double		pre_settle;			//昨结算
	double		pre_interest;		//上日总持

	double		bid_prices[10];		//委买价格
	double		ask_prices[10];		//委卖价格
	double		bid_qty[10];		//委买量
	double		ask_qty[10];		//委卖量
};

struct YCTOrdQueStruct
{
    YCTOrdQueStruct()
	{
		memset(this, 0, sizeof(YCTOrdQueStruct));
	}

	char		exchg[MAX_EXCHANGE_LENGTH];
	char		code[MAX_INSTRUMENT_LENGTH];

	uint32_t	trading_date;		//交易日,如20140327
	uint32_t	action_date;		//自然日期,如20140327
	uint32_t	action_time;		//发生时间,精确到毫秒,如105932000
	YCTBSDirectType	side;			//委托方向

	double		price;			    //委托价格
	uint32_t	order_items;	    //订单个数
	uint32_t	qsize;			    //队列长度
	uint32_t	volumes[50];	    //委托明细
};

struct YCTOrdDtlStruct
{
    YCTOrdDtlStruct()
    {
        memset(this, 0, sizeof(YCTOrdDtlStruct));
    }
    
	char		exchg[MAX_EXCHANGE_LENGTH];
	char		code[MAX_INSTRUMENT_LENGTH];

	uint32_t		trading_date;		//交易日,如20140327
	uint32_t		action_date;		//自然日期,如20140327
	uint32_t		action_time;		//发生时间,精确到毫秒,如105932000

	uint64_t			index;			//委托编号(从1开始,递增1)
	double				price;			//委托价格
	uint32_t			volume;			//委托数量
	YCTBSDirectType		side;		//委托方向
	YCTOrdDetailType	otype;		//委托类型
};

struct YCTTransStruct
{
	YCTTransStruct()
	{
		memset(this, 0, sizeof(YCTTransStruct));
	}

	char		exchg[MAX_EXCHANGE_LENGTH];
	char		code[MAX_INSTRUMENT_LENGTH];

	uint32_t	trading_date;		//交易日,如20140327
	uint32_t	action_date;		//自然日期,如20140327
	uint32_t	action_time;		//发生时间,精确到毫秒,如105932000
	int64_t		index;			    //成交编号(从1开始,递增1)

	YCTTransType	ttype;			//成交类型: 'C', 0
	YCTBSDirectType	side;			//BS标志

	double			price;			//成交价格
	uint32_t		volume;			//成交数量
	int64_t			askorder;		//叫卖序号
	int64_t			bidorder;		//叫买序号
};

#pragma pack(pop)

NS_YCT_END