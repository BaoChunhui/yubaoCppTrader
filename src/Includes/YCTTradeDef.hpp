/*!
 * \file YCTTradeDef.hpp
 * \project	yubaoCppTrader
 *
 * \author yubao
 * \date 2026/04/04
 * 
 * \brief yubaoCppTrader交易数据对象定义,包括委托、订单、成交、持仓、资金明细等数据
 */
#pragma once
#include "YCTObject.hpp"
#include "YCTTypes.h"
#include "YCTMacros.h"

#include <cstring>

NS_YCT_BEGIN

class YCTContractInfo;

//委托数据结构,用户客户端向服务端发起
class YCTEntrust : public YCTPoolObject<YCTEntrust>
{
protected:
	char			m_strExchg[MAX_EXCHANGE_LENGTH]{};
	char			m_strCode[MAX_INSTRUMENT_LENGTH]{};
	double			m_dVolume;
	double			m_iPrice;

	bool			m_bIsNet;
	bool			m_bIsBuy;

	YCTDirectionType	m_direction;
	YCTPriceType		m_priceType;
	YCTOrderFlag		m_orderFlag;
	YCTOffsetType		m_offsetType;

	char				m_strEntrustID[64] = { 0 };
	char				m_strUserTag[64] = { 0 };

	YCTBusinessType		m_businessType;

	YCTContractInfo*	m_pContract;

public:
    YCTEntrust()
    : m_iPrice(0)
    , m_dVolume(0)
    , m_businessType(YCTBusinessType::BT_CASH)
    , m_direction(YCTDirectionType::DT_LONG)
    , m_priceType(YCTPriceType::PT_ANYPRICE)
    , m_orderFlag(YCTOrderFlag::OF_NOR)
    , m_offsetType(YCTOffsetType::OT_OPEN)
    , m_bIsNet(false)
    , m_bIsBuy(true)
    , m_pContract(NULL)
    {
    }

    virtual ~YCTEntrust(){}

	static inline YCTEntrust* create(const char* code, double vol, double price, const char* exchg = "", YCTBusinessType bType = YCTBusinessType::BT_CASH) noexcept
	{
		YCTEntrust* pRet = YCTEntrust::allocate();
		if(pRet)
		{
			if (!exchg)
				exchg = "";
			if (!code)
				code = "";

			std::size_t len = std::strlen(exchg);
			if (len >= MAX_EXCHANGE_LENGTH)
				len = MAX_EXCHANGE_LENGTH - 1;
			std::memcpy(pRet->m_strExchg, exchg, len);
			pRet->m_strExchg[len] = '\0';

			len = std::strlen(code);
			if (len >= MAX_INSTRUMENT_LENGTH)
				len = MAX_INSTRUMENT_LENGTH - 1;
			std::memcpy(pRet->m_strCode, code, len);
			pRet->m_strCode[len] = '\0';

			pRet->m_dVolume = vol;
			pRet->m_iPrice = price;
			pRet->m_businessType = bType;
			return pRet;
		}

		return NULL;
	}

	inline void setExchange(const char* exchg, std::size_t len = 0) noexcept
	{
		if (!exchg)
		{
			m_strExchg[0] = '\0';
			return;
		}
		const std::size_t cap = MAX_EXCHANGE_LENGTH - 1;
		const std::size_t slen = std::strlen(exchg);
		std::size_t n = (len == 0) ? slen : len;
		if (n > slen)
			n = slen;
		if (n > cap)
			n = cap;
		std::memcpy(m_strExchg, exchg, n);
		m_strExchg[n] = '\0';
	}
	inline void setCode(const char* code, std::size_t len = 0) noexcept
	{
		if (!code)
		{
			m_strCode[0] = '\0';
			return;
		}
		const std::size_t cap = MAX_INSTRUMENT_LENGTH - 1;
		const std::size_t slen = std::strlen(code);
		std::size_t n = (len == 0) ? slen : len;
		if (n > slen)
			n = slen;
		if (n > cap)
			n = cap;
		std::memcpy(m_strCode, code, n);
		m_strCode[n] = '\0';
	}

	inline void setDirection(YCTDirectionType dType)noexcept {m_direction = dType;}
	inline void setPriceType(YCTPriceType pType)noexcept {m_priceType = pType;}
	inline void setOrderFlag(YCTOrderFlag oFlag)noexcept {m_orderFlag = oFlag;}
	inline void setOffsetType(YCTOffsetType oType)noexcept {m_offsetType = oType;}

	inline YCTDirectionType	getDirection() const noexcept {return m_direction;}
	inline YCTPriceType		getPriceType() const noexcept {return m_priceType;}
	inline YCTOrderFlag		getOrderFlag() const noexcept {return m_orderFlag;}
	inline YCTOffsetType	getOffsetType() const noexcept {return m_offsetType;}

	inline void setBusinessType(YCTBusinessType bType) noexcept { m_businessType = bType; }
	inline YCTBusinessType	getBusinessType() const  noexcept { return m_businessType; }

	inline void setVolume(double volume) noexcept { m_dVolume = volume; }
	inline void setPrice(double price) noexcept { m_iPrice = price; }

	inline double getVolume() const noexcept { return m_dVolume; }
	inline double getPrice() const noexcept { return m_iPrice; }

	inline const char* getCode() const noexcept { return m_strCode; }
	inline const char* getExchg() const  noexcept { return m_strExchg; }

	inline void setEntrustID(const char* eid) noexcept
	{
		if (!eid)
		{
			m_strEntrustID[0] = '\0';
			return;
		}
		std::strncpy(m_strEntrustID, eid, sizeof(m_strEntrustID) - 1);
		m_strEntrustID[sizeof(m_strEntrustID) - 1] = '\0';
	}
	inline const char* getEntrustID() const  noexcept { return m_strEntrustID; }
	inline char* getEntrustID() noexcept { return m_strEntrustID; }

	inline void setUserTag(const char* tag) noexcept
	{
		if (!tag)
		{
			m_strUserTag[0] = '\0';
			return;
		}
		std::strncpy(m_strUserTag, tag, sizeof(m_strUserTag) - 1);
		m_strUserTag[sizeof(m_strUserTag) - 1] = '\0';
	}
	inline const char* getUserTag() const  noexcept { return m_strUserTag; }
	inline char* getUserTag()  noexcept { return m_strUserTag; }

	inline void setNetDirection(bool isBuy) noexcept { m_bIsNet = true; m_bIsBuy = isBuy; }
	inline bool isNet() const  noexcept { return m_bIsNet; }
	inline bool isBuy() const  noexcept { return m_bIsBuy; }

	inline void setContractInfo(YCTContractInfo* cInfo) noexcept { m_pContract = cInfo; }
	inline YCTContractInfo* getContractInfo() const  noexcept { return m_pContract; }
};

//委托操作: 撤单、改单
class YCTEntrustAction : public YCTPoolObject<YCTEntrustAction>
{
protected:
	char			m_strExchg[MAX_EXCHANGE_LENGTH]{};
	char			m_strCode[MAX_INSTRUMENT_LENGTH]{};

	char			m_strEnturstID[64] = { 0 };
	YCTActionFlag	m_actionFlag = YCTActionFlag::AF_CANCEL;

	char			m_strOrderID[64] = { 0 };
	char			m_strUserTag[64] = { 0 };

	YCTBusinessType		m_businessType;
	YCTContractInfo*	m_pContract;

public:
	YCTEntrustAction()
		: m_actionFlag(YCTActionFlag::AF_CANCEL)
		, m_businessType(YCTBusinessType::BT_CASH)
		, m_pContract(NULL){}

	virtual ~YCTEntrustAction(){}

	static inline YCTEntrustAction* create(const char* code, const char* exchg = "", YCTBusinessType bType = YCTBusinessType::BT_CASH) noexcept
	{
		YCTEntrustAction* pRet = YCTEntrustAction::allocate();
		if(pRet)
		{
			if (!exchg)
				exchg = "";
			if (!code)
				code = "";

			std::size_t len = std::strlen(exchg);
			if (len >= MAX_EXCHANGE_LENGTH)
				len = MAX_EXCHANGE_LENGTH - 1;
			std::memcpy(pRet->m_strExchg, exchg, len);
			pRet->m_strExchg[len] = '\0';

			len = std::strlen(code);
			if (len >= MAX_INSTRUMENT_LENGTH)
				len = MAX_INSTRUMENT_LENGTH - 1;
			std::memcpy(pRet->m_strCode, code, len);
			pRet->m_strCode[len] = '\0';

			pRet->m_businessType = bType;
			return pRet;
		}

		return NULL;
	}

	static inline YCTEntrustAction* createCancelAction(const char* eid, const char* oid) noexcept
	{
		YCTEntrustAction* pRet = YCTEntrustAction::allocate();
		if(pRet)
		{
			if (!eid)
				eid = "";
			if (!oid)
				oid = "";

			std::size_t eid_len = std::strlen(eid);
			if (eid_len >= sizeof(pRet->m_strEnturstID) - 1)
				eid_len = sizeof(pRet->m_strEnturstID) - 1;
			std::memcpy(pRet->m_strEnturstID, eid, eid_len);
			pRet->m_strEnturstID[eid_len] = '\0';

			std::size_t oid_len = std::strlen(oid);
			if (oid_len >= sizeof(pRet->m_strOrderID) - 1)
				oid_len = sizeof(pRet->m_strOrderID) - 1;
			std::memcpy(pRet->m_strOrderID, oid, oid_len);
			pRet->m_strOrderID[oid_len] = '\0';

			return pRet;
		}

		return NULL;
	}

	inline const char* getExchg() const  noexcept { return m_strExchg; }
	inline const char* getCode() const noexcept {return m_strCode;}

	inline void setExchange(const char* exchg, std::size_t len = 0) noexcept
	{
		if (!exchg)
		{
			m_strExchg[0] = '\0';
			return;
		}
		const std::size_t cap = MAX_EXCHANGE_LENGTH - 1;
		const std::size_t slen = std::strlen(exchg);
		std::size_t n = (len == 0) ? slen : len;
		if (n > slen)
			n = slen;
		if (n > cap)
			n = cap;
		std::memcpy(m_strExchg, exchg, n);
		m_strExchg[n] = '\0';
	}
	inline void setCode(const char* code, std::size_t len = 0) noexcept
	{
		if (!code)
		{
			m_strCode[0] = '\0';
			return;
		}
		const std::size_t cap = MAX_INSTRUMENT_LENGTH - 1;
		const std::size_t slen = std::strlen(code);
		std::size_t n = (len == 0) ? slen : len;
		if (n > slen)
			n = slen;
		if (n > cap)
			n = cap;
		std::memcpy(m_strCode, code, n);
		m_strCode[n] = '\0';
	}

	inline void setActionFlag(YCTActionFlag af) noexcept {m_actionFlag = af;}
	inline YCTActionFlag getActionFlag() const noexcept {return m_actionFlag;}

	inline void setEntrustID(const char* eid) noexcept
	{
		if (!eid)
		{
			m_strEnturstID[0] = '\0';
			return;
		}
		std::strncpy(m_strEnturstID, eid, sizeof(m_strEnturstID) - 1);
		m_strEnturstID[sizeof(m_strEnturstID) - 1] = '\0';
	}
	inline const char* getEntrustID() const noexcept {return m_strEnturstID;}
	inline char* getEntrustID() noexcept { return m_strEnturstID; }

	inline void setOrderID(const char* oid) noexcept
	{
		if (!oid)
		{
			m_strOrderID[0] = '\0';
			return;
		}
		std::strncpy(m_strOrderID, oid, sizeof(m_strOrderID) - 1);
		m_strOrderID[sizeof(m_strOrderID) - 1] = '\0';
	}
	inline const char* getOrderID() const noexcept {return m_strOrderID;}

	inline void setBusinessType(YCTBusinessType bType) noexcept { m_businessType = bType; }
	inline YCTBusinessType	getBusinessType() const  noexcept { return m_businessType; }

	inline void setUserTag(const char* tag) noexcept
	{
		if (!tag)
		{
			m_strUserTag[0] = '\0';
			return;
		}
		std::strncpy(m_strUserTag, tag, sizeof(m_strUserTag) - 1);
		m_strUserTag[sizeof(m_strUserTag) - 1] = '\0';
	}
	inline const char* getUserTag() const  noexcept { return m_strUserTag; }
	inline char* getUserTag()  noexcept { return m_strUserTag; }

	inline void setContractInfo(YCTContractInfo* cInfo)  noexcept { m_pContract = cInfo; }
	inline YCTContractInfo* getContractInfo() const  noexcept { return m_pContract; }
};

//订单信息,查看订单状态变化等
class YCTOrderInfo : public YCTPoolObject<YCTOrderInfo>
{
private:
	//这部分成员和YCTEntrust一致
	char			m_strExchg[MAX_EXCHANGE_LENGTH]{};
	char			m_strCode[MAX_INSTRUMENT_LENGTH]{};
	double			m_dVolume;
	double			m_iPrice;

	bool			m_bIsNet;
	bool			m_bIsBuy;

	YCTDirectionType	m_direction;
	YCTPriceType		m_priceType;
	YCTOrderFlag		m_orderFlag;
	YCTOffsetType		m_offsetType;

	char				m_strEntrustID[64] = { 0 };
	char				m_strUserTag[64] = { 0 };

	YCTBusinessType		m_businessType;
	YCTContractInfo*	m_pContract;

	//这部分是Order专有的成员
	uint32_t	m_uInsertDate;
	uint64_t	m_uInsertTime;
	double		m_dVolTraded;
	double		m_dVolLeft;
	bool		m_bIsError;

	YCTOrderState	m_orderState;
	YCTOrderType	m_orderType;
	char			m_strOrderID[64] = { 0 };
	char			m_strStateMsg[256] = { 0 };

public:
	YCTOrderInfo()
		: m_iPrice(0)
		, m_dVolume(0)
		, m_businessType(YCTBusinessType::BT_CASH)
		, m_direction(YCTDirectionType::DT_LONG)
		, m_priceType(YCTPriceType::PT_ANYPRICE)
		, m_orderFlag(YCTOrderFlag::OF_NOR)
		, m_offsetType(YCTOffsetType::OT_OPEN)
		, m_bIsNet(false)
		, m_bIsBuy(true)
		, m_pContract(NULL)
		, m_orderState(YCTOrderState::OS_Submitting)
		, m_orderType(YCTOrderType::OT_Normal)
		, m_uInsertDate(0)
		, m_uInsertTime(0)
		, m_dVolTraded(0.0)
		, m_dVolLeft(0.0)
		, m_bIsError(false)
	{
	}

	virtual ~YCTOrderInfo(){}

	static inline YCTOrderInfo* create(YCTEntrust* entrust = NULL) noexcept
	{
		YCTOrderInfo* pRet = YCTOrderInfo::allocate();
		if (!pRet)
			return NULL;

		if (entrust != NULL)
		{
			const char* exchg = entrust->getExchg();
			if (!exchg)
				exchg = "";
			std::size_t len = std::strlen(exchg);
			if (len >= MAX_EXCHANGE_LENGTH)
				len = MAX_EXCHANGE_LENGTH - 1;
			std::memcpy(pRet->m_strExchg, exchg, len);
			pRet->m_strExchg[len] = '\0';

			const char* code = entrust->getCode();
			if (!code)
				code = "";
			len = std::strlen(code);
			if (len >= MAX_INSTRUMENT_LENGTH)
				len = MAX_INSTRUMENT_LENGTH - 1;
			std::memcpy(pRet->m_strCode, code, len);
			pRet->m_strCode[len] = '\0';

			pRet->m_iPrice = entrust->getPrice();
			pRet->m_dVolume = entrust->getVolume();

			pRet->m_direction = entrust->getDirection();
			pRet->m_offsetType = entrust->getOffsetType();
			pRet->m_orderFlag = entrust->getOrderFlag();
			pRet->m_priceType = entrust->getPriceType();

			pRet->setEntrustID(entrust->getEntrustID());
			pRet->setUserTag(entrust->getUserTag());

			pRet->m_dVolLeft = entrust->getVolume();
			pRet->m_businessType = entrust->getBusinessType();
		}

		return pRet;
	}

	//这部分是和YCTEntrust同步的
	inline void setExchange(const char* exchg, std::size_t len = 0) noexcept
	{
		if (!exchg)
		{
			m_strExchg[0] = '\0';
			return;
		}
		const std::size_t cap = MAX_EXCHANGE_LENGTH - 1;
		const std::size_t slen = std::strlen(exchg);
		std::size_t n = (len == 0) ? slen : len;
		if (n > slen)
			n = slen;
		if (n > cap)
			n = cap;
		std::memcpy(m_strExchg, exchg, n);
		m_strExchg[n] = '\0';
	}
	inline void setCode(const char* code, std::size_t len = 0) noexcept
	{
		if (!code)
		{
			m_strCode[0] = '\0';
			return;
		}
		const std::size_t cap = MAX_INSTRUMENT_LENGTH - 1;
		const std::size_t slen = std::strlen(code);
		std::size_t n = (len == 0) ? slen : len;
		if (n > slen)
			n = slen;
		if (n > cap)
			n = cap;
		std::memcpy(m_strCode, code, n);
		m_strCode[n] = '\0';
	}

	inline void setDirection(YCTDirectionType dType) noexcept { m_direction = dType; }
	inline void setPriceType(YCTPriceType pType) noexcept { m_priceType = pType; }
	inline void setOrderFlag(YCTOrderFlag oFlag) noexcept { m_orderFlag = oFlag; }
	inline void setOffsetType(YCTOffsetType oType) noexcept { m_offsetType = oType; }

	inline YCTDirectionType	getDirection() const noexcept { return m_direction; }
	inline YCTPriceType		getPriceType() const noexcept { return m_priceType; }
	inline YCTOrderFlag		getOrderFlag() const noexcept { return m_orderFlag; }
	inline YCTOffsetType	getOffsetType() const noexcept { return m_offsetType; }

	inline void setBusinessType(YCTBusinessType bType) noexcept { m_businessType = bType; }
	inline YCTBusinessType	getBusinessType() const noexcept { return m_businessType; }

	inline void setVolume(double volume) noexcept { m_dVolume = volume; }
	inline void setPrice(double price) noexcept { m_iPrice = price; }

	inline double getVolume() const noexcept { return m_dVolume; }
	inline double getPrice() const noexcept { return m_iPrice; }

	inline const char* getCode() const noexcept { return m_strCode; }
	inline const char* getExchg() const noexcept { return m_strExchg; }

	inline void setEntrustID(const char* eid) noexcept
	{
		if (!eid)
		{
			m_strEntrustID[0] = '\0';
			return;
		}
		std::strncpy(m_strEntrustID, eid, sizeof(m_strEntrustID) - 1);
		m_strEntrustID[sizeof(m_strEntrustID) - 1] = '\0';
	}
	inline const char* getEntrustID() const noexcept { return m_strEntrustID; }
	inline char* getEntrustID() noexcept { return m_strEntrustID; }

	inline void setUserTag(const char* tag) noexcept
	{
		if (!tag)
		{
			m_strUserTag[0] = '\0';
			return;
		}
		std::strncpy(m_strUserTag, tag, sizeof(m_strUserTag) - 1);
		m_strUserTag[sizeof(m_strUserTag) - 1] = '\0';
	}
	inline const char* getUserTag() const noexcept { return m_strUserTag; }
	inline char* getUserTag() noexcept { return m_strUserTag; }

	inline void setNetDirection(bool isBuy) noexcept { m_bIsNet = true; m_bIsBuy = isBuy; }
	inline bool isNet() const noexcept { return m_bIsNet; }
	inline bool isBuy() const noexcept { return m_bIsBuy; }

	inline void setContractInfo(YCTContractInfo* cInfo) noexcept { m_pContract = cInfo; }
	inline YCTContractInfo* getContractInfo() const noexcept { return m_pContract; }

	inline void	setOrderDate(uint32_t uDate) noexcept { m_uInsertDate = uDate; }
	inline void	setOrderTime(uint64_t uTime) noexcept { m_uInsertTime = uTime; }
	inline void	setVolTraded(double vol) noexcept { m_dVolTraded = vol; }
	inline void	setVolLeft(double vol) noexcept { m_dVolLeft = vol; }

	inline void	setOrderID(const char* oid) noexcept
	{
		if (!oid)
		{
			m_strOrderID[0] = '\0';
			return;
		}
		std::strncpy(m_strOrderID, oid, sizeof(m_strOrderID) - 1);
		m_strOrderID[sizeof(m_strOrderID) - 1] = '\0';
	}
	inline void	setOrderState(YCTOrderState os) noexcept { m_orderState = os; }
	inline void	setOrderType(YCTOrderType ot) noexcept { m_orderType = ot; }

	inline uint32_t getOrderDate() const noexcept { return m_uInsertDate; }
	inline uint64_t getOrderTime() const noexcept { return m_uInsertTime; }
	inline double getVolTraded() const noexcept { return m_dVolTraded; }
	inline double getVolLeft() const noexcept { return m_dVolLeft; }

	inline YCTOrderState	getOrderState() const noexcept { return m_orderState; }
	inline YCTOrderType		getOrderType() const noexcept { return m_orderType; }
	inline const char*		getOrderID() const noexcept { return m_strOrderID; }
	inline char*			getOrderID() noexcept { return m_strOrderID; }

	inline void setStateMsg(const char* msg) noexcept
	{
		if (!msg)
		{
			m_strStateMsg[0] = '\0';
			return;
		}
		std::strncpy(m_strStateMsg, msg, sizeof(m_strStateMsg) - 1);
		m_strStateMsg[sizeof(m_strStateMsg) - 1] = '\0';
	}
	inline const char* getStateMsg() const noexcept { return m_strStateMsg; }

	inline bool isAlive() const noexcept
	{
		if (m_bIsError)
			return false;

		switch (m_orderState)
		{
		case YCTOrderState::OS_AllTraded:
		case YCTOrderState::OS_Canceled:
			return false;
		default:
			return true;
		}
	}

	inline void	setError(bool bError = true) noexcept { m_bIsError = bError; }
	inline bool	isError() const noexcept { return m_bIsError; }
};

//成交信息
class YCTTradeInfo : public YCTPoolObject<YCTTradeInfo>
{
protected:
	char				m_strExchg[MAX_EXCHANGE_LENGTH]{};
	char				m_strCode[MAX_INSTRUMENT_LENGTH]{};
	char				m_strTradeID[64] = { 0 };
	char				m_strRefOrder[64] = { 0 };
	char				m_strUserTag[64] = { 0 };

	uint32_t			m_uTradeDate;
	uint64_t			m_uTradeTime;
	double				m_dVolume;
	double				m_dPrice;

	bool				m_bIsNet;
	bool				m_bIsBuy;

	YCTDirectionType	m_direction;
	YCTOffsetType		m_offsetType;
	YCTOrderType		m_orderType;
	YCTTradeType		m_tradeType;

	double				m_dAmount;

	YCTBusinessType		m_businessType;
	YCTContractInfo*	m_pContract;

public:
	YCTTradeInfo()
		: m_uTradeDate(0)
		, m_uTradeTime(0)
		, m_dVolume(0)
		, m_dPrice(0)
		, m_bIsNet(false)
		, m_bIsBuy(true)
		, m_direction(YCTDirectionType::DT_LONG)
		, m_offsetType(YCTOffsetType::OT_OPEN)
		, m_orderType(YCTOrderType::OT_Normal)
		, m_tradeType(YCTTradeType::TT_Common)
		, m_dAmount(0)
		, m_businessType(YCTBusinessType::BT_CASH)
		, m_pContract(NULL)
	{
	}

	virtual ~YCTTradeInfo() {}

	static inline YCTTradeInfo* create(const char* code, const char* exchg = "", YCTBusinessType bType = YCTBusinessType::BT_CASH) noexcept
	{
		YCTTradeInfo* pRet = YCTTradeInfo::allocate();
		if (!pRet)
			return NULL;

		if (!exchg)
			exchg = "";
		if (!code)
			code = "";

		std::size_t len = std::strlen(exchg);
		if (len >= MAX_EXCHANGE_LENGTH)
			len = MAX_EXCHANGE_LENGTH - 1;
		std::memcpy(pRet->m_strExchg, exchg, len);
		pRet->m_strExchg[len] = '\0';

		len = std::strlen(code);
		if (len >= MAX_INSTRUMENT_LENGTH)
			len = MAX_INSTRUMENT_LENGTH - 1;
		std::memcpy(pRet->m_strCode, code, len);
		pRet->m_strCode[len] = '\0';

		pRet->m_businessType = bType;
		return pRet;
	}

	inline void setExchange(const char* exchg, std::size_t len = 0) noexcept
	{
		if (!exchg)
		{
			m_strExchg[0] = '\0';
			return;
		}
		const std::size_t cap = MAX_EXCHANGE_LENGTH - 1;
		const std::size_t slen = std::strlen(exchg);
		std::size_t n = (len == 0) ? slen : len;
		if (n > slen)
			n = slen;
		if (n > cap)
			n = cap;
		std::memcpy(m_strExchg, exchg, n);
		m_strExchg[n] = '\0';
	}

	inline void setCode(const char* code, std::size_t len = 0) noexcept
	{
		if (!code)
		{
			m_strCode[0] = '\0';
			return;
		}
		const std::size_t cap = MAX_INSTRUMENT_LENGTH - 1;
		const std::size_t slen = std::strlen(code);
		std::size_t n = (len == 0) ? slen : len;
		if (n > slen)
			n = slen;
		if (n > cap)
			n = cap;
		std::memcpy(m_strCode, code, n);
		m_strCode[n] = '\0';
	}

	inline void setTradeID(const char* tradeid) noexcept
	{
		if (!tradeid)
		{
			m_strTradeID[0] = '\0';
			return;
		}
		std::strncpy(m_strTradeID, tradeid, sizeof(m_strTradeID) - 1);
		m_strTradeID[sizeof(m_strTradeID) - 1] = '\0';
	}

	inline void setRefOrder(const char* oid) noexcept
	{
		if (!oid)
		{
			m_strRefOrder[0] = '\0';
			return;
		}
		std::strncpy(m_strRefOrder, oid, sizeof(m_strRefOrder) - 1);
		m_strRefOrder[sizeof(m_strRefOrder) - 1] = '\0';
	}

	inline void setDirection(YCTDirectionType dType) noexcept { m_direction = dType; }
	inline void setOffsetType(YCTOffsetType oType) noexcept { m_offsetType = oType; }
	inline void setOrderType(YCTOrderType ot) noexcept { m_orderType = ot; }
	inline void setTradeType(YCTTradeType tt) noexcept { m_tradeType = tt; }

	inline void setVolume(double volume) noexcept { m_dVolume = volume; }
	inline void setPrice(double price) noexcept { m_dPrice = price; }

	inline void setTradeDate(uint32_t uDate) noexcept { m_uTradeDate = uDate; }
	inline void setTradeTime(uint64_t uTime) noexcept { m_uTradeTime = uTime; }

	inline void setAmount(double amount) noexcept { m_dAmount = amount; }

	inline YCTDirectionType getDirection() const noexcept { return m_direction; }
	inline YCTOffsetType getOffsetType() const noexcept { return m_offsetType; }
	inline YCTOrderType getOrderType() const noexcept { return m_orderType; }
	inline YCTTradeType getTradeType() const noexcept { return m_tradeType; }

	inline double getVolume() const noexcept { return m_dVolume; }
	inline double getPrice() const noexcept { return m_dPrice; }

	inline const char* getCode() const noexcept { return m_strCode; }
	inline const char* getExchg() const noexcept { return m_strExchg; }
	inline const char* getTradeID() const noexcept { return m_strTradeID; }
	inline const char* getRefOrder() const noexcept { return m_strRefOrder; }

	inline char* getTradeID() noexcept { return m_strTradeID; }
	inline char* getRefOrder() noexcept { return m_strRefOrder; }

	inline uint32_t getTradeDate() const noexcept { return m_uTradeDate; }
	inline uint64_t getTradeTime() const noexcept { return m_uTradeTime; }

	inline double getAmount() const noexcept { return m_dAmount; }

	inline void setUserTag(const char* tag) noexcept
	{
		if (!tag)
		{
			m_strUserTag[0] = '\0';
			return;
		}
		std::strncpy(m_strUserTag, tag, sizeof(m_strUserTag) - 1);
		m_strUserTag[sizeof(m_strUserTag) - 1] = '\0';
	}
	inline const char* getUserTag() const noexcept { return m_strUserTag; }
	inline char* getUserTag() noexcept { return m_strUserTag; }

	inline void setBusinessType(YCTBusinessType bType) noexcept { m_businessType = bType; }
	inline YCTBusinessType getBusinessType() const noexcept { return m_businessType; }

	inline void setNetDirection(bool isBuy) noexcept { m_bIsNet = true; m_bIsBuy = isBuy; }
	inline bool isNet() const noexcept { return m_bIsNet; }
	inline bool isBuy() const noexcept { return m_bIsBuy; }

	inline void setContractInfo(YCTContractInfo* cInfo) noexcept { m_pContract = cInfo; }
	inline YCTContractInfo* getContractInfo() const noexcept { return m_pContract; }
};

//持仓信息
class YCTPositionItem : public YCTPoolObject<YCTPositionItem>
{
protected:
	char				m_strExchg[MAX_EXCHANGE_LENGTH]{};
	char				m_strCode[MAX_INSTRUMENT_LENGTH]{};
	char				m_strCurrency[8] = { 0 };

	YCTDirectionType	m_direction;
	double				m_dPrePosition;
	double				m_dNewPosition;
	double				m_dAvailPrePos;
	double				m_dAvailNewPos;
	double				m_dTotalPosCost;
	double				m_dMargin;
	double				m_dAvgPrice;
	double				m_dDynProfit;

	YCTBusinessType		m_businessType;
	YCTContractInfo*	m_pContract;

public:
	YCTPositionItem()
		: m_direction(YCTDirectionType::DT_LONG)
		, m_dPrePosition(0)
		, m_dNewPosition(0)
		, m_dAvailPrePos(0)
		, m_dAvailNewPos(0)
		, m_dTotalPosCost(0)
		, m_dMargin(0)
		, m_dAvgPrice(0)
		, m_dDynProfit(0)
		, m_businessType(YCTBusinessType::BT_CASH)
		, m_pContract(NULL)
	{
	}

	virtual ~YCTPositionItem() {}

	static inline YCTPositionItem* create(const char* code, const char* currency = "CNY", const char* exchg = "",
		YCTBusinessType bType = YCTBusinessType::BT_CASH) noexcept
	{
		YCTPositionItem* pRet = YCTPositionItem::allocate();
		if (!pRet)
			return NULL;

		if (!exchg)
			exchg = "";
		if (!code)
			code = "";
		if (!currency)
			currency = "CNY";

		std::size_t len = std::strlen(exchg);
		if (len >= MAX_EXCHANGE_LENGTH)
			len = MAX_EXCHANGE_LENGTH - 1;
		std::memcpy(pRet->m_strExchg, exchg, len);
		pRet->m_strExchg[len] = '\0';

		len = std::strlen(code);
		if (len >= MAX_INSTRUMENT_LENGTH)
			len = MAX_INSTRUMENT_LENGTH - 1;
		std::memcpy(pRet->m_strCode, code, len);
		pRet->m_strCode[len] = '\0';

		len = std::strlen(currency);
		if (len >= sizeof(pRet->m_strCurrency))
			len = sizeof(pRet->m_strCurrency) - 1;
		std::memcpy(pRet->m_strCurrency, currency, len);
		pRet->m_strCurrency[len] = '\0';

		pRet->m_businessType = bType;
		return pRet;
	}

	inline void setExchange(const char* exchg, std::size_t len = 0) noexcept
	{
		if (!exchg)
		{
			m_strExchg[0] = '\0';
			return;
		}
		const std::size_t cap = MAX_EXCHANGE_LENGTH - 1;
		const std::size_t slen = std::strlen(exchg);
		std::size_t n = (len == 0) ? slen : len;
		if (n > slen)
			n = slen;
		if (n > cap)
			n = cap;
		std::memcpy(m_strExchg, exchg, n);
		m_strExchg[n] = '\0';
	}

	inline void setCode(const char* code, std::size_t len = 0) noexcept
	{
		if (!code)
		{
			m_strCode[0] = '\0';
			return;
		}
		const std::size_t cap = MAX_INSTRUMENT_LENGTH - 1;
		const std::size_t slen = std::strlen(code);
		std::size_t n = (len == 0) ? slen : len;
		if (n > slen)
			n = slen;
		if (n > cap)
			n = cap;
		std::memcpy(m_strCode, code, n);
		m_strCode[n] = '\0';
	}

	inline void setCurrency(const char* currency) noexcept
	{
		if (!currency)
		{
			m_strCurrency[0] = '\0';
			return;
		}
		std::strncpy(m_strCurrency, currency, sizeof(m_strCurrency) - 1);
		m_strCurrency[sizeof(m_strCurrency) - 1] = '\0';
	}

	inline void setDirection(YCTDirectionType dType) noexcept { m_direction = dType; }
	inline void setPrePosition(double prePos) noexcept { m_dPrePosition = prePos; }
	inline void setNewPosition(double newPos) noexcept { m_dNewPosition = newPos; }
	inline void setAvailPrePos(double availPos) noexcept { m_dAvailPrePos = availPos; }
	inline void setAvailNewPos(double availPos) noexcept { m_dAvailNewPos = availPos; }
	inline void setPositionCost(double cost) noexcept { m_dTotalPosCost = cost; }
	inline void setMargin(double margin) noexcept { m_dMargin = margin; }
	inline void setAvgPrice(double avgPrice) noexcept { m_dAvgPrice = avgPrice; }
	inline void setDynProfit(double profit) noexcept { m_dDynProfit = profit; }

	inline YCTDirectionType getDirection() const noexcept { return m_direction; }
	inline double getPrePosition() const noexcept { return m_dPrePosition; }
	inline double getNewPosition() const noexcept { return m_dNewPosition; }
	inline double getAvailPrePos() const noexcept { return m_dAvailPrePos; }
	inline double getAvailNewPos() const noexcept { return m_dAvailNewPos; }

	inline double getTotalPosition() const noexcept { return m_dPrePosition + m_dNewPosition; }
	inline double getAvailPosition() const noexcept { return m_dAvailPrePos + m_dAvailNewPos; }

	inline double getFrozenPosition() const noexcept { return getTotalPosition() - getAvailPosition(); }
	inline double getFrozenNewPos() const noexcept { return m_dNewPosition - m_dAvailNewPos; }
	inline double getFrozenPrePos() const noexcept { return m_dPrePosition - m_dAvailPrePos; }

	inline double getPositionCost() const noexcept { return m_dTotalPosCost; }
	inline double getMargin() const noexcept { return m_dMargin; }
	inline double getAvgPrice() const noexcept { return m_dAvgPrice; }
	inline double getDynProfit() const noexcept { return m_dDynProfit; }

	inline const char* getCode() const noexcept { return m_strCode; }
	inline const char* getCurrency() const noexcept { return m_strCurrency; }
	inline const char* getExchg() const noexcept { return m_strExchg; }

	inline void setBusinessType(YCTBusinessType bType) noexcept { m_businessType = bType; }
	inline YCTBusinessType getBusinessType() const noexcept { return m_businessType; }

	inline void setContractInfo(YCTContractInfo* cInfo) noexcept { m_pContract = cInfo; }
	inline YCTContractInfo* getContractInfo() const noexcept { return m_pContract; }
};

//账户信息
class YCTAccountInfo : public YCTPoolObject<YCTAccountInfo>
{
protected:
	char	m_strCurrency[8] = { 0 };

	double	m_dBalance;
	double	m_dPreBalance;
	double	m_dMargin;
	double	m_dCommission;
	double	m_dFrozenMargin;
	double	m_dFrozenCommission;
	double	m_dCloseProfit;
	double	m_dDynProfit;
	double	m_dDeposit;
	double	m_dWithdraw;
	double	m_dAvailable;

public:
	YCTAccountInfo()
		: m_dBalance(0)
		, m_dPreBalance(0)
		, m_dMargin(0)
		, m_dCommission(0)
		, m_dFrozenMargin(0)
		, m_dFrozenCommission(0)
		, m_dCloseProfit(0)
		, m_dDynProfit(0)
		, m_dDeposit(0)
		, m_dWithdraw(0)
		, m_dAvailable(0)
	{
		setCurrency("CNY");
	}

	virtual ~YCTAccountInfo() {}

	static inline YCTAccountInfo* create() noexcept
	{
		return YCTAccountInfo::allocate();
	}

	inline void setCurrency(const char* currency) noexcept
	{
		if (!currency)
		{
			m_strCurrency[0] = '\0';
			return;
		}
		std::strncpy(m_strCurrency, currency, sizeof(m_strCurrency) - 1);
		m_strCurrency[sizeof(m_strCurrency) - 1] = '\0';
	}

	inline void setBalance(double balance) noexcept { m_dBalance = balance; }
	inline void setPreBalance(double prebalance) noexcept { m_dPreBalance = prebalance; }
	inline void setMargin(double margin) noexcept { m_dMargin = margin; }
	inline void setFrozenMargin(double frozenmargin) noexcept { m_dFrozenMargin = frozenmargin; }
	inline void setCloseProfit(double closeprofit) noexcept { m_dCloseProfit = closeprofit; }
	inline void setDynProfit(double dynprofit) noexcept { m_dDynProfit = dynprofit; }
	inline void setDeposit(double deposit) noexcept { m_dDeposit = deposit; }
	inline void setWithdraw(double withdraw) noexcept { m_dWithdraw = withdraw; }
	inline void setCommission(double commission) noexcept { m_dCommission = commission; }
	inline void setFrozenCommission(double frozencommission) noexcept { m_dFrozenCommission = frozencommission; }
	inline void setAvailable(double available) noexcept { m_dAvailable = available; }

	inline double getBalance() const noexcept { return m_dBalance; }
	inline double getPreBalance() const noexcept { return m_dPreBalance; }
	inline double getMargin() const noexcept { return m_dMargin; }
	inline double getFrozenMargin() const noexcept { return m_dFrozenMargin; }
	inline double getCloseProfit() const noexcept { return m_dCloseProfit; }
	inline double getDynProfit() const noexcept { return m_dDynProfit; }
	inline double getDeposit() const noexcept { return m_dDeposit; }
	inline double getWithdraw() const noexcept { return m_dWithdraw; }
	inline double getCommission() const noexcept { return m_dCommission; }
	inline double getFrozenCommission() const noexcept { return m_dFrozenCommission; }
	inline double getAvailable() const noexcept { return m_dAvailable; }

	inline const char* getCurrency() const noexcept { return m_strCurrency; }
};

NS_YCT_END