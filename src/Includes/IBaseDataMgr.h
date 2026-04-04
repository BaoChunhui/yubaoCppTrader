/*!
 * \file IBaseDataMgr.h
 * \project	yubaoCppTrader
 *
 * \author yubao
 * \date 2026/04/04
 *
 * \brief 基础数据管理器接口定义
 */
#pragma once
#include <stdint.h>

#include "YCTMacros.h"
#include "FasterDefs.h"

NS_YCT_BEGIN

typedef CodeSet ContractSet;

class YCTContractInfo;
class YCTArray;
class YCTSessionInfo;
class YCTCommodityInfo;

typedef yct_hashset<uint32_t> HolidaySet;

typedef struct _TradingDayTpl
{
	uint32_t	_cur_tdate;
	HolidaySet	_holidays;

	_TradingDayTpl() : _cur_tdate(0) {}
} TradingDayTpl;

class IBaseDataMgr
{
public:
	virtual ~IBaseDataMgr() = default;

	virtual YCTCommodityInfo*	getCommodity(const char* exchgpid) = 0;
	virtual YCTCommodityInfo*	getCommodity(const char* exchg, const char* pid) = 0;

	virtual YCTContractInfo*	getContract(const char* code, const char* exchg = "", uint32_t uDate = 0) = 0;
	virtual YCTArray*			getContracts(const char* exchg = "", uint32_t uDate = 0) = 0;

	virtual YCTSessionInfo*		getSession(const char* sid) = 0;
	virtual YCTSessionInfo*		getSessionByCode(const char* code, const char* exchg = "") = 0;
	virtual YCTArray*			getAllSessions() = 0;

	virtual bool				isHoliday(const char* pid, uint32_t uDate, bool isTpl = false) = 0;

	virtual uint32_t			calcTradingDate(const char* stdPID, uint32_t uDate, uint32_t uTime, bool isSession = false) = 0;
	virtual uint64_t			getBoundaryTime(const char* stdPID, uint32_t tDate, bool isSession = false, bool isStart = true) = 0;

	virtual uint32_t			getContractSize(const char* exchg = "", uint32_t uDate = 0) { return 0; }
};

NS_YCT_END
