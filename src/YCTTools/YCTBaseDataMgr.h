/*!
 * \file YCTBaseDataMgr.h
 * \project	yubaoCppTrader
 *
 * \author yubao
 * \date 2026/04/06
 * 
 * \brief 基础数据管理器实现
 */
#pragma once
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/YCTCollection.hpp"
#include "../Includes/FasterDefs.h"

USING_NS_YCT;

typedef yct_hashmap<std::string, TradingDayTpl>	TradingDayTplMap;

typedef YCTHashMap<std::string>		YCTContractList;
typedef YCTHashMap<std::string>		YCTExchgContract;
typedef YCTHashMap<std::string>		YCTContractMap;

typedef YCTHashMap<std::string>		YCTSessionMap;
typedef YCTHashMap<std::string>		YCTCommodityMap;

typedef yct_hashmap<std::string, CodeSet> SessionCodeMap;

class YCTBaseDataMgr : public IBaseDataMgr
{
public:
	YCTBaseDataMgr();
	~YCTBaseDataMgr();

	virtual YCTCommodityInfo*	getCommodity(const char* stdPID) override;
	virtual YCTCommodityInfo*	getCommodity(const char* exchg, const char* pid) override;

	virtual YCTContractInfo*	getContract(const char* code, const char* exchg = "", uint32_t uDate = 0) override;
	virtual YCTArray*			getContracts(const char* exchg = "", uint32_t uDate = 0) override;

	virtual YCTSessionInfo*		getSession(const char* sid) override;
	virtual YCTSessionInfo*		getSessionByCode(const char* code, const char* exchg = "") override;
	virtual YCTArray*			getAllSessions() override;
	virtual bool				isHoliday(const char* stdPID, uint32_t uDate, bool isTpl = false) override;

	virtual uint32_t			calcTradingDate(const char* stdPID, uint32_t uDate, uint32_t uTime, bool isSession = false) override;
	virtual uint64_t			getBoundaryTime(const char* stdPID, uint32_t tDate, bool isSession = false, bool isStart = true) override;

	virtual uint32_t			getContractSize(const char* exchg = "", uint32_t uDate = 0) override;

	void		release();

	bool		loadSessions(const char* filename);
	bool		loadCommodities(const char* filename);
	bool		loadContracts(const char* filename);
	bool		loadHolidays(const char* filename);

	uint32_t	getTradingDate(const char* stdPID, uint32_t uOffDate = 0, uint32_t uOffMinute = 0, bool isTpl = false);
	uint32_t	getNextTDate(const char* stdPID, uint32_t uDate, int days = 1, bool isTpl = false);
	uint32_t	getPrevTDate(const char* stdPID, uint32_t uDate, int days = 1, bool isTpl = false);
	bool		isTradingDate(const char* stdPID, uint32_t uDate, bool isTpl = false);
	void		setTradingDate(const char* stdPID, uint32_t uDate, bool isTpl = false);

	CodeSet*	getSessionComms(const char* sid);

private:
	const char* getTplIDByPID(const char* stdPID);

	TradingDayTplMap	m_mapTradingDay;

	SessionCodeMap		m_mapSessionCode;

	YCTExchgContract*	m_mapExchgContract;
	YCTSessionMap*		m_mapSessions;
	YCTCommodityMap*	m_mapCommodities;
	YCTContractMap*		m_mapContracts;
};
