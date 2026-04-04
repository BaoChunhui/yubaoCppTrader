/*!
 * \file ITraderApi.h
 * \project	yubaoCppTrader
 *
 * \author yubao
 * \date 2026/04/04
 *
 * \brief 交易通道对接接口定义
 */
#pragma once
#include <functional>
#include <stdint.h>
#include "YCTTypes.h"

NS_YCT_BEGIN

class YCTVariant;
class YCTEntrust;
class YCTOrderInfo;
class YCTTradeInfo;
class YCTEntrustAction;
class YCTError;
class YCTArray;
class IBaseDataMgr;

typedef std::function<void()> CommonExecuter;

#pragma region "Stock Trading API definations"
/*
 *	股票交易接口回调
 */
class IStkTraderSpi
{
public:
	virtual ~IStkTraderSpi() = default;
};

/*
 *	股票交易接口
 */
class IStkTraderApi
{
public:
	virtual ~IStkTraderApi() = default;
};
#pragma endregion

#pragma region "Option Trading API definations"
/*
 *	期权交易接口回调
 */
class IOptTraderSpi
{
public:
	virtual ~IOptTraderSpi() = default;

	virtual void onRspEntrustOpt(YCTEntrust* entrust, YCTError* err) {}
	virtual void onRspOrdersOpt(const YCTArray* ayOrders) {}
	virtual void onPushOrderOpt(YCTOrderInfo* orderInfo) {}
};

/*
 *	期权交易接口
 */
class IOptTraderApi
{
public:
	virtual ~IOptTraderApi() = default;

	/*
	 *	下单接口
	 *	entrust 下单的具体数据结构
	 */
	virtual int orderInsertOpt(YCTEntrust* entrust) { return -1; }

	/*
	 *	订单操作接口
	 *	action	操作的具体数据结构
	 */
	virtual int orderActionOpt(YCTEntrustAction* action) { return -1; }

	/*
	 *	查询期权订单
	 */
	virtual int queryOrdersOpt(YCTBusinessType bType) { return -1; }
};
#pragma endregion

// 委托回调接口
class ITraderSpi
{
public:
	virtual ~ITraderSpi() = default;

	/*
	 *	获取基础数据管理器
	 */
	virtual IBaseDataMgr* getBaseDataMgr() = 0;

	/*
	 *	处理交易接口的日志
	 */
	virtual void handleTraderLog(YCTLogLevel ll, const char* message) {}

	/*
	 *	获取股票交易接口Spi
	 */
	virtual IStkTraderSpi* getStkSpi() { return NULL; }

	/*
	 *	获取期权交易接口Spi
	 */
	virtual IOptTraderSpi* getOptSpi() { return NULL; }

	/*
	 *	处理交易接口事件
	 */
	virtual void handleEvent(YCTTraderEvent e, int32_t ec) = 0;

	/*
	 *	登录回报
	 */
	virtual void onLoginResult(bool bSucc, const char* msg, uint32_t tradingdate) = 0;

	/*
	 *	注销回报
	 */
	virtual void onLogout() {}

	/*
	 *	委托回报
	 */
	virtual void onRspEntrust(YCTEntrust* entrust, YCTError* err) {}

	/*
	 * 资金查询回报
	 */
	virtual void onRspAccount(YCTArray* ayAccounts) {}

	/*
	 *	持仓查询回报
	 */
	virtual void onRspPosition(const YCTArray* ayPositions) {}

	/*
	 *	订单查询回报
	 */
	virtual void onRspOrders(const YCTArray* ayOrders) {}

	/*
	 *	成交查询回报
	 */
	virtual void onRspTrades(const YCTArray* ayTrades) {}

	/*
	 *	结算单查询回报
	 */
	virtual void onRspSettlementInfo(uint32_t uDate, const char* content) {}

	/*
	 *	订单回报推送
	 */
	virtual void onPushOrder(YCTOrderInfo* orderInfo) {}

	/*
	 *	成交回报推送
	 */
	virtual void onPushTrade(YCTTradeInfo* tradeRecord) {}

	/*
	 *	交易接口错误回报
	 */
	virtual void onTraderError(YCTError* err, void* pData = NULL) {}

	/*
	 *	合约状态推送
	 */
	virtual void onPushInstrumentStatus(const char* exchg, const char* code, YCTTradeStatus state) {}
};

// 下单接口管理接口
class ITraderApi
{
public:
	virtual ~ITraderApi() {}

	virtual IStkTraderApi* getStkTrader() { return NULL; }
	virtual IOptTraderApi* getOptTrader() { return NULL; }

	/*
	 *	初始化交易模块
	 */
	virtual bool init(YCTVariant* params) { return false; }

	/*
	 *	释放交易模块
	 */
	virtual void release() {}

	/*
	 *	注册回调接口
	 */
	virtual void registerSpi(ITraderSpi* listener) {}

	//////////////////////////////////////////////////////////////////////////
	// 业务逻辑接口

	/*
	 *	连接服务器
	 */
	virtual void connect() {}

	/*
	 *	断开连接
	 */
	virtual void disconnect() {}

	virtual bool isConnected() { return false; }

	/*
	 *	生成委托单号
	 */
	virtual bool makeEntrustID(char* buffer, int length) { return false; }

	/*
	 *	登录接口
	 */
	virtual int login(const char* user, const char* pass, const char* productInfo) { return -1; }

	/*
	 *	注销接口
	 */
	virtual int logout() { return -1; }

	/*
	 *	下单接口
	 *	entrust 下单的具体数据结构
	 */
	virtual int orderInsert(YCTEntrust* entrust) { return -1; }

	/*
	 *	订单操作接口
	 *	action	操作的具体数据结构
	 */
	virtual int orderAction(YCTEntrustAction* action) { return -1; }

	/*
	 *	查询账户信息
	 */
	virtual int queryAccount() { return -1; }

	/*
	 *	查询持仓信息
	 */
	virtual int queryPositions() { return -1; }

	/*
	 *	查询所有订单
	 */
	virtual int queryOrders() { return -1; }

	/*
	 *	查询成交明细
	 */
	virtual int queryTrades() { return -1; }

	/*
	 *	查询结算单
	 */
	virtual int querySettlement(uint32_t uDate) { return 0; }
};

NS_YCT_END

// 动态库导出 Trader 工厂时使用
typedef yct::ITraderApi* (*FuncCreateTrader)();
typedef void (*FuncDeleteTrader)(yct::ITraderApi*& trader);
