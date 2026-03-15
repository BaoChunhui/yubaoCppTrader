/*!
 * \file YCTTypes.h
 * \project	yubaoCppTrader
 *
 * \author yubao
 * \date 2026/03/08
 * 
 * \brief yubaoCppTrader基本数据类型定义文件
 */
#pragma once
#include "YCTMacros.h"
#include <stdint.h>

NS_YCT_BEGIN

/*
 *	合约分类
 */
enum class ContractCategory
{
	CC_Stock,			//股票
	CC_Future,			//期货
	CC_FutOption,		//期货期权，商品期权是这个分类
	CC_Combination,		//组合
	CC_Spot,			//即期
	CC_EFP,				//期转现
	CC_SpotOption,		//现货期权，股指期权是这个分类
	CC_ETFOption,		//个股期权，ETF期权是这个分类

	CC_DC_Spot   = 20,	//数币现货
	CC_DC_Swap,			//数币永续
	CC_DC_Future,		//数币期货
	CC_DC_Margin,		//数币杠杆
	CC_DC_Option,		//数币期权

	CC_UserIndex = 90	//自定义指数
};

/*
 *	期权类型
 */
enum class OptionType
{
	OT_None = 0,
	OT_Call = '1',		//看涨期权
	OT_Put  = '2'		//看跌期权
};

/*
 *	平仓类型
 */
enum class CoverMode
{
	CM_OpenCover,		//开平
	CM_CoverToday,		//开平昨平今
	CM_UNFINISHED,		//平未了结的
	CM_None				//不区分开平
};

/*
 *	交易模式
 */
enum class TradingMode
{
	TM_Both,	//多空都支持
	TM_Long,	//只能做多
	TM_LongT1,	//做多T+1
	TM_None = 9	//不能交易
};

/*
 *	价格模式
 */
enum class PriceMode
{
	PM_Both,		//市价限价都支持
	PM_Limit,		//只支持限价
	PM_Market,		//只支持市价
	PM_None = 9		//不支持交易
};

/*
 *	K线数据类型（开、高、低、收、量、额、日期、时间）
 */
enum class YCTKlineFieldType
{
	KFT_OPEN,
	KFT_HIGH,
	KFT_LOW,
	KFT_CLOSE,
	KFT_DATE,
	KFT_TIME,
	KFT_VOLUME,
	KFT_SVOLUME
};

/*
 *	K线周期（PERIOD_NAME 下标需用 static_cast<int>(YCTKlinePeriod::xxx)）
 */
enum class YCTKlinePeriod
{
	KP_Tick,
	KP_Minute1,
	KP_Minute5,
	KP_DAY,
	KP_Week,
	KP_Month
};

static const char* PERIOD_NAME[] =
{
	"tick",
	"min1",
	"min5",
	"day",
	"week",
	"month"
};

/*
 *	日志级别
 */
enum class YCTLogLevel
{
	LL_ALL    = 100,
	LL_DEBUG,
	LL_INFO,
	LL_WARN,
	LL_ERROR,
	LL_FATAL,
	LL_NONE
};

/*
 *	价格类型
 */
enum class YCTPriceType
{
	PT_ANYPRICE = 0,			//市价单
	PT_LIMITPRICE,				//限价单
	PT_BESTPRICE,				//最优价
	PT_LASTPRICE,				//最新价

	PT_CTP_LASTPLUSONETICKS = 20,	//最新价+1ticks
	PT_CTP_LASTPLUSTWOTICKS,
	PT_CTP_LASTPLUSTHREETICKS,
	PT_CTP_ASK1,
	PT_CTP_ASK1PLUSONETICKS,
	PT_CTP_ASK1PLUSTWOTICKS,
	PT_CTP_ASK1PLUSTHREETICKS,
	PT_CTP_BID1,
	PT_CTP_BID1PLUSONETICKS,
	PT_CTP_BID1PLUSTWOTICKS,
	PT_CTP_BID1PLUSTHREETICKS,
	PT_CTP_FIVELEVELPRICE,		//五档价，中金所市价

	PT_DC_POSTONLY = 100,		//只做maker单
	PT_DC_FOK,
	PT_DC_IOC,
	PT_DC_OPTLIMITIOC
};

/*
 *	时间条件
 */
enum class YCTTimeCondition
{
	TC_IOC  = '1',	//立即完成,否则撤销
	TC_GFS,			//本节有效
	TC_GFD			//当日有效
};

/*
 *	订单标志
 */
enum class YCTOrderFlag
{
	OF_NOR = '0',	//普通订单
	OF_FAK,			//fak
	OF_FOK			//fok
};

/*
 *	开平方向
 */
enum class YCTOffsetType
{
	OT_OPEN          = '0',	//开仓
	OT_CLOSE,					//平仓,上期为平昨
	OT_FORCECLOSE,				//强平
	OT_CLOSETODAY,				//平今
	OT_CLOSEYESTERDAY			//平昨
};

/*
 *	多空方向
 */
enum class YCTDirectionType
{
	DT_LONG,	//做多
	DT_SHORT,	//做空
	DT_NET		//净
};

/*
 *	业务类型
 */
enum class YCTBusinessType
{
	BT_CASH    = '0',
	BT_ETF     = '1',
	BT_EXECUTE = '2',
	BT_QUOTE   = '3',
	BT_FORQUOTE = '4',
	BT_FREEZE  = '5',
	BT_CREDIT  = '6',
	BT_UNKNOWN	//未知业务类型
};

/*
 *	订单操作类型
 */
enum class YCTActionFlag
{
	AF_CANCEL = '0',	//撤销
	AF_MODIFY = '3'		//修改
};

/*
 *	订单状态
 */
enum class YCTOrderState
{
	OS_AllTraded            = '0',	//全部成交
	OS_PartTraded_Queuing,
	OS_PartTraded_NotQueuing,
	OS_NotTraded_Queuing,
	OS_NotTraded_NotQueuing,
	OS_Canceled,
	OS_Submitting  = 'a',
	OS_Cancelling,
	OS_Nottouched
};

/*
 *	订单类型
 */
enum class YCTOrderType
{
	OT_Normal   = 0,
	OT_Exception,
	OT_System,
	OT_Hedge
};

/*
 *	成交类型
 */
enum class YCTTradeType
{
	TT_Common              = '0',
	TT_OptionExecution     = '1',
	TT_OTC                 = '2',
	TT_EFPDerived          = '3',
	TT_CombinationDerived  = '4'
};

/*
 *	错误代码
 */
enum class YCTErroCode
{
	EC_NONE        = 0,
	EC_ORDERINSERT,
	EC_ORDERCANCEL,
	EC_EXECINSERT,
	EC_EXECCANCEL,
	EC_UNKNOWN = 9999
};

/*
 *	比较字段
 */
enum class YCTCompareField
{
	CF_NEWPRICE  = 0,
	CF_BIDPRICE,
	CF_ASKPRICE,
	CF_PRICEDIFF,
	CF_NONE = 9
};

/*
 *	比较类型（C++ enum class，使用方式：YCTCompareType::CT_Equal）
 */
enum class YCTCompareType
{
	CT_Equal         = 0,	//等于
	CT_Larger,				//大于
	CT_Smaller,				//小于
	CT_LargerOrEqual,		//大于等于
	CT_SmallerOrEqual		//小于等于
};

/*
 *	行情解析器事件
 */
enum class YCTParserEvent
{
	PE_Connect = 0,
	PE_Close,
	PE_Login,
	PE_Logout
};

/*
 *	交易模块事件
 */
enum class YCTTraderEvent
{
	TE_Connect = 0,
	TE_Close,
	TE_Login,
	TE_Logout
};

/*
 *	交易状态
 */
enum class YCTTradeStatus
{
	TS_BeforeTrading   = '0',
	TS_NotTrading      = '1',
	TS_Continous      = '2',
	TS_AuctionOrdering = '3',
	TS_AuctionBalance  = '4',
	TS_AuctionMatch    = '5',
	TS_Closed          = '6'
};

/*
 *	买卖方向类型
 */
typedef uint32_t YCTBSDirectType;
#define BSDT_Buy		'B'	//买入	
#define BSDT_Sell	    'S'	//卖出
#define BSDT_Unknown    ' '	//未知
#define BSDT_Borrow	    'G'	//借入
#define BSDT_Lend	    'F'	//借出

/*
 *	成交类型
 */
typedef uint32_t YCTTransType;
#define TT_Unknown	'U'	//未知类型
#define TT_Match	'M'	//撮合成交
#define TT_Cancel	'C'	//撤单

/*
 *	委托明细类型
 */
typedef uint32_t YCTOrdDetailType;
#define ODT_Unknown		0	//未知类型
#define ODT_BestPrice	'U'	//本方最优
#define ODT_AnyPrice	'1'	//市价
#define ODT_LimitPrice	'2'	//限价

NS_YCT_END