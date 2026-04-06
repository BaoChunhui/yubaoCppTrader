/*!
 * \file YCTBaseDataMgr.cpp
 * \project	yubaoCppTrader
 *
 * \author yubao
 * \date 2026/04/06
 *
 * \brief 基础数据管理器实现
 */
#include "YCTBaseDataMgr.h"
#include "../YCTUtils/YCTCfgLoader.h"
#include "YCTLogger.h"

#include "../Includes/YCTContractInfo.hpp"
#include "../Includes/YCTSessionInfo.hpp"
#include "../Includes/YCTVariant.hpp"
#include "../Includes/YCTTypes.h"

#include "../Share/StrUtil.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/fmtlib.h"

#include <cstring>
#include <ctime>
#include <string>

static const char* DEFAULT_HOLIDAY_TPL = "CHINA";

namespace {

static void parseCommodity(YCTCommodityInfo* pCommInfo, YCTVariant* jPInfo)
{
	pCommInfo->setPriceTick(jPInfo->getDouble("pricetick"));
	pCommInfo->setVolScale(jPInfo->getUInt32("volscale"));

	if (jPInfo->has("category"))
		pCommInfo->setCategory(static_cast<ContractCategory>(jPInfo->getUInt32("category")));
	else
		pCommInfo->setCategory(ContractCategory::CC_Future);

	pCommInfo->setCoverMode(static_cast<CoverMode>(jPInfo->getUInt32("covermode")));
	pCommInfo->setPriceMode(static_cast<PriceMode>(jPInfo->getUInt32("pricemode")));

	if (jPInfo->has("trademode"))
		pCommInfo->setTradingMode(static_cast<TradingMode>(jPInfo->getUInt32("trademode")));
	else
		pCommInfo->setTradingMode(TradingMode::TM_Both);

	double lotsTick = 1;
	double minLots = 1;
	if (jPInfo->has("lotstick"))
		lotsTick = jPInfo->getDouble("lotstick");
	if (jPInfo->has("minlots"))
		minLots = jPInfo->getDouble("minlots");
	pCommInfo->setLotsTick(lotsTick);
	pCommInfo->setMinLots(minLots);
}

} // namespace

YCTBaseDataMgr::YCTBaseDataMgr()
	: m_mapExchgContract(NULL)
	, m_mapSessions(NULL)
	, m_mapCommodities(NULL)
	, m_mapContracts(NULL)
{
	m_mapExchgContract = YCTExchgContract::create();
	m_mapSessions = YCTSessionMap::create();
	m_mapCommodities = YCTCommodityMap::create();
	m_mapContracts = YCTContractMap::create();
}

YCTBaseDataMgr::~YCTBaseDataMgr()
{
	if (m_mapExchgContract)
	{
		m_mapExchgContract->release();
		m_mapExchgContract = NULL;
	}

	if (m_mapSessions)
	{
		m_mapSessions->release();
		m_mapSessions = NULL;
	}

	if (m_mapCommodities)
	{
		m_mapCommodities->release();
		m_mapCommodities = NULL;
	}

	if (m_mapContracts)
	{
		m_mapContracts->release();
		m_mapContracts = NULL;
	}
}

YCTCommodityInfo* YCTBaseDataMgr::getCommodity(const char* exchgpid)
{
	if (exchgpid == NULL || m_mapCommodities == NULL)
		return NULL;

	return (YCTCommodityInfo*)m_mapCommodities->get(exchgpid);
}

YCTCommodityInfo* YCTBaseDataMgr::getCommodity(const char* exchg, const char* pid)
{
	if (m_mapCommodities == NULL || exchg == NULL || pid == NULL)
		return NULL;

	char key[64] = { 0 };
	fmtutil::format_to(key, sizeof(key), "{}.{}", exchg, pid);

	return (YCTCommodityInfo*)m_mapCommodities->get(key);
}

YCTContractInfo* YCTBaseDataMgr::getContract(const char* code, const char* exchg /* = "" */, uint32_t uDate /* = 0 */)
{
	if (code == NULL || m_mapContracts == NULL || m_mapExchgContract == NULL)
		return NULL;

	const char* exPtr = (exchg != NULL) ? exchg : "";
	auto lKey = std::string(code);

	if (strlen(exPtr) == 0)
	{
		auto it = m_mapContracts->find(lKey);
		if (it == m_mapContracts->end())
			return NULL;

		YCTArray* ayInst = (YCTArray*)it->second;
		if (ayInst == NULL || ayInst->size() == 0)
			return NULL;

		for (std::size_t i = 0; i < ayInst->size(); i++)
		{
			YCTContractInfo* cInfo = (YCTContractInfo*)ayInst->at(i);
			if (uDate == 0 || (cInfo->getOpenDate() <= uDate && cInfo->getExpireDate() >= uDate))
				return cInfo;
		}
		return NULL;
	}
	else
	{
		auto sKey = std::string(exPtr);
		auto it = m_mapExchgContract->find(sKey);
		if (it != m_mapExchgContract->end())
		{
			YCTContractList* contractList = (YCTContractList*)it->second;
			auto it2 = contractList->find(lKey);
			if (it2 != contractList->end())
			{
				YCTContractInfo* cInfo = (YCTContractInfo*)it2->second;
				if (uDate == 0 || (cInfo->getOpenDate() <= uDate && cInfo->getExpireDate() >= uDate))
					return cInfo;
			}

			return NULL;
		}
	}

	return NULL;
}

uint32_t YCTBaseDataMgr::getContractSize(const char* exchg /* = "" */, uint32_t uDate /* = 0 */)
{
	if (m_mapExchgContract == NULL)
		return 0;

	uint32_t ret = 0;
	const char* exPtr = (exchg != NULL) ? exchg : "";
	if (strlen(exPtr) > 0)
	{
		auto it = m_mapExchgContract->find(std::string(exPtr));
		if (it != m_mapExchgContract->end())
		{
			YCTContractList* contractList = (YCTContractList*)it->second;
			auto it2 = contractList->begin();
			for (; it2 != contractList->end(); it2++)
			{
				YCTContractInfo* cInfo = (YCTContractInfo*)it2->second;
				if (uDate == 0 || (cInfo->getOpenDate() <= uDate && cInfo->getExpireDate() >= uDate))
					ret++;
			}
		}
	}
	else
	{
		auto it = m_mapExchgContract->begin();
		for (; it != m_mapExchgContract->end(); it++)
		{
			YCTContractList* contractList = (YCTContractList*)it->second;
			auto it2 = contractList->begin();
			for (; it2 != contractList->end(); it2++)
			{
				YCTContractInfo* cInfo = (YCTContractInfo*)it2->second;
				if (uDate == 0 || (cInfo->getOpenDate() <= uDate && cInfo->getExpireDate() >= uDate))
					ret++;
			}
		}
	}

	return ret;
}

YCTArray* YCTBaseDataMgr::getContracts(const char* exchg /* = "" */, uint32_t uDate /* = 0 */)
{
	YCTArray* ay = YCTArray::create();
	if (m_mapExchgContract == NULL)
		return ay;

	const char* exPtr = (exchg != NULL) ? exchg : "";
	if (strlen(exPtr) > 0)
	{
		auto it = m_mapExchgContract->find(std::string(exPtr));
		if (it != m_mapExchgContract->end())
		{
			YCTContractList* contractList = (YCTContractList*)it->second;
			auto it2 = contractList->begin();
			for (; it2 != contractList->end(); it2++)
			{
				YCTContractInfo* cInfo = (YCTContractInfo*)it2->second;
				if (uDate == 0 || (cInfo->getOpenDate() <= uDate && cInfo->getExpireDate() >= uDate))
					ay->append(cInfo, true);
			}
		}
	}
	else
	{
		auto it = m_mapExchgContract->begin();
		for (; it != m_mapExchgContract->end(); it++)
		{
			YCTContractList* contractList = (YCTContractList*)it->second;
			auto it2 = contractList->begin();
			for (; it2 != contractList->end(); it2++)
			{
				YCTContractInfo* cInfo = (YCTContractInfo*)it2->second;
				if (uDate == 0 || (cInfo->getOpenDate() <= uDate && cInfo->getExpireDate() >= uDate))
					ay->append(cInfo, true);
			}
		}
	}

	return ay;
}

YCTArray* YCTBaseDataMgr::getAllSessions()
{
	YCTArray* ay = YCTArray::create();
	if (m_mapSessions == NULL)
		return ay;

	for (auto it = m_mapSessions->begin(); it != m_mapSessions->end(); it++)
	{
		ay->append(it->second, true);
	}
	return ay;
}

YCTSessionInfo* YCTBaseDataMgr::getSession(const char* sid)
{
	if (sid == NULL || m_mapSessions == NULL)
		return NULL;

	return (YCTSessionInfo*)m_mapSessions->get(sid);
}

YCTSessionInfo* YCTBaseDataMgr::getSessionByCode(const char* code, const char* exchg /* = "" */)
{
	if (code == NULL)
		return NULL;

	YCTContractInfo* ct = getContract(code, exchg != NULL ? exchg : "");
	if (ct == NULL)
		return NULL;

	YCTCommodityInfo* comm = ct->getCommInfo();
	if (comm == NULL)
		return NULL;

	return comm->getSessionInfo();
}

bool YCTBaseDataMgr::isHoliday(const char* pid, uint32_t uDate, bool isTpl /* = false */)
{
	uint32_t wd = TimeUtils::getWeekDay(uDate);
	if (wd == 0 || wd == 6)
		return true;

	std::string tplid;
	if (isTpl)
	{
		if (pid == NULL || pid[0] == '\0')
			return false;
		tplid.assign(pid);
	}
	else
	{
		tplid.assign(getTplIDByPID(pid));
	}

	auto it = m_mapTradingDay.find(tplid);
	if (it != m_mapTradingDay.end())
	{
		const TradingDayTpl& tpl = it->second;
		return (tpl._holidays.find(uDate) != tpl._holidays.end());
	}

	return false;
}

void YCTBaseDataMgr::release()
{
	if (m_mapExchgContract)
	{
		m_mapExchgContract->release();
		m_mapExchgContract = NULL;
	}

	if (m_mapSessions)
	{
		m_mapSessions->release();
		m_mapSessions = NULL;
	}

	if (m_mapCommodities)
	{
		m_mapCommodities->release();
		m_mapCommodities = NULL;
	}
}

bool YCTBaseDataMgr::loadSessions(const char* filename)
{
	if (!StdFile::exists(filename))
	{
		YCTLogger::error("Trading sessions configuration file {} not exists", filename);
		return false;
	}

	YCTVariant* root = YCTCfgLoader::load_from_file(filename);
	if (root == NULL)
	{
		YCTLogger::error("Loading session config file {} failed", filename);
		return false;
	}

	for (const std::string& id : root->memberNames())
	{
		YCTVariant* jVal = root->get(id);

		const char* name = jVal->getCString("name");
		int32_t offset = jVal->getInt32("offset");

		YCTSessionInfo* sInfo = YCTSessionInfo::create(id.c_str(), name, offset);

		if (jVal->has("auction"))
		{
			YCTVariant* jAuc = jVal->get("auction");
			sInfo->setAuctionTime(jAuc->getUInt32("from"), jAuc->getUInt32("to"));
		}
		else if (jVal->has("auctions"))
		{
			YCTVariant* jAucs = jVal->get("auctions");
			for (uint32_t i = 0; i < jAucs->size(); i++)
			{
				YCTVariant* jSec = jAucs->get(i);
				sInfo->addAuctionTime(jSec->getUInt32("from"), jSec->getUInt32("to"));
			}
		}

		YCTVariant* jSecs = jVal->get("sections");
		if (jSecs == NULL || !jSecs->isArray())
			continue;

		for (uint32_t i = 0; i < jSecs->size(); i++)
		{
			YCTVariant* jSec = jSecs->get(i);
			sInfo->addTradingSection(jSec->getUInt32("from"), jSec->getUInt32("to"));
		}

		m_mapSessions->add(id.c_str(), sInfo);
	}

	root->release();

	return true;
}

bool YCTBaseDataMgr::loadCommodities(const char* filename)
{
	if (!StdFile::exists(filename))
	{
		YCTLogger::error("Commodities configuration file {} not exists", filename);
		return false;
	}

	YCTVariant* root = YCTCfgLoader::load_from_file(filename);
	if (root == NULL)
	{
		YCTLogger::error("Loading commodities config file {} failed", filename);
		return false;
	}

	for (const std::string& exchg : root->memberNames())
	{
		YCTVariant* jExchg = root->get(exchg);

		for (const std::string& pid : jExchg->memberNames())
		{
			YCTVariant* jPInfo = jExchg->get(pid);

			const char* name = jPInfo->getCString("name");
			const char* sid = jPInfo->getCString("session");
			const char* hid = jPInfo->getCString("holiday");

			if (strlen(sid) == 0)
			{
				YCTLogger::warn("No session configured for {}.{}", exchg.c_str(), pid.c_str());
				continue;
			}

			YCTCommodityInfo* pCommInfo = YCTCommodityInfo::create(pid.c_str(), name, exchg.c_str(), sid, hid);
			parseCommodity(pCommInfo, jPInfo);

			YCTSessionInfo* sInfo = getSession(sid);
			pCommInfo->setSessionInfo(sInfo);

			std::string key = fmt::format("{}.{}", exchg.c_str(), pid.c_str());
			if (m_mapCommodities == NULL)
				m_mapCommodities = YCTCommodityMap::create();

			m_mapCommodities->add(key, pCommInfo, false);

			m_mapSessionCode[sid].insert(key);
		}
	}

	YCTLogger::info("Commodities configuration file {} loaded", filename);
	root->release();
	return true;
}

bool YCTBaseDataMgr::loadContracts(const char* filename)
{
	if (!StdFile::exists(filename))
	{
		YCTLogger::error("Contracts configuration file {} not exists", filename);
		return false;
	}

	YCTVariant* root = YCTCfgLoader::load_from_file(filename);
	if (root == NULL)
	{
		YCTLogger::error("Loading contracts config file {} failed", filename);
		return false;
	}

	for (const std::string& exchg : root->memberNames())
	{
		YCTVariant* jExchg = root->get(exchg);

		for (const std::string& code : jExchg->memberNames())
		{
			YCTVariant* jcInfo = jExchg->get(code);

			YCTCommodityInfo* commInfo = NULL;
			std::string pid;
			if (jcInfo->has("product"))
			{
				pid = jcInfo->getCString("product");
				commInfo = getCommodity(jcInfo->getCString("exchg"), pid.c_str());
			}
			else if (jcInfo->has("rules"))
			{
				pid = code.c_str();
				YCTVariant* jPInfo = jcInfo->get("rules");
				const char* name = jcInfo->getCString("name");
				std::string sid = jPInfo->getCString("session");
				std::string hid;
				if (jPInfo->has("holiday"))
					hid = jPInfo->getCString("holiday");

				if (sid.empty())
					sid = "ALLDAY";

				commInfo = YCTCommodityInfo::create(pid.c_str(), name, exchg.c_str(), sid.c_str(), hid.c_str());
				parseCommodity(commInfo, jPInfo);
				YCTSessionInfo* sInfo = getSession(sid.c_str());
				commInfo->setSessionInfo(sInfo);

				std::string key = fmt::format("{}.{}", exchg.c_str(), pid.c_str());
				if (m_mapCommodities == NULL)
					m_mapCommodities = YCTCommodityMap::create();

				m_mapCommodities->add(key, commInfo, false);

				m_mapSessionCode[sid].insert(key);

				YCTLogger::debug("Commodity {} has been automatically added", key.c_str());
			}

			if (commInfo == NULL)
			{
				YCTLogger::warn("Commodity {}.{} not found, contract {} skipped", jcInfo->getCString("exchg"), jcInfo->getCString("product"), code.c_str());
				continue;
			}

			YCTContractInfo* cInfo = YCTContractInfo::create(code.c_str(),
				jcInfo->getCString("name"),
				jcInfo->getCString("exchg"),
				pid.c_str());

			cInfo->setCommInfo(commInfo);

			uint32_t maxMktQty = 1000000;
			uint32_t maxLmtQty = 1000000;
			uint32_t minMktQty = 1;
			uint32_t minLmtQty = 1;
			if (jcInfo->has("maxmarketqty"))
				maxMktQty = jcInfo->getUInt32("maxmarketqty");
			if (jcInfo->has("maxlimitqty"))
				maxLmtQty = jcInfo->getUInt32("maxlimitqty");
			if (jcInfo->has("minmarketqty"))
				minMktQty = jcInfo->getUInt32("minmarketqty");
			if (jcInfo->has("minlimitqty"))
				minLmtQty = jcInfo->getUInt32("minlimitqty");
			cInfo->setVolumeLimits(maxMktQty, maxLmtQty, minMktQty, minLmtQty);

			uint32_t opendate = 0;
			uint32_t expiredate = 0;
			if (jcInfo->has("opendate"))
				opendate = jcInfo->getUInt32("opendate");
			if (jcInfo->has("expiredate"))
				expiredate = jcInfo->getUInt32("expiredate");
			cInfo->setDates(opendate, expiredate);

			double lMargin = 0;
			double sMargin = 0;
			if (jcInfo->has("longmarginratio"))
				lMargin = jcInfo->getDouble("longmarginratio");
			if (jcInfo->has("shortmarginratio"))
				sMargin = jcInfo->getDouble("shortmarginratio");
			cInfo->setMarginRatios(lMargin, sMargin);

			YCTContractList* contractList = (YCTContractList*)m_mapExchgContract->get(std::string(cInfo->getExchg()));
			if (contractList == NULL)
			{
				contractList = YCTContractList::create();
				m_mapExchgContract->add(std::string(cInfo->getExchg()), contractList, false);
			}
			contractList->add(std::string(cInfo->getCode()), cInfo, false);

			commInfo->addCode(code.c_str());

			std::string key = std::string(cInfo->getCode());
			YCTArray* ayInst = (YCTArray*)m_mapContracts->get(key);
			if (ayInst == NULL)
			{
				ayInst = YCTArray::create();
				m_mapContracts->add(key, ayInst, false);
			}

			ayInst->append(cInfo, true);
		}
	}

	YCTLogger::info("Contracts configuration file {} loaded, {} exchanges", filename, m_mapExchgContract->size());
	root->release();
	return true;
}

bool YCTBaseDataMgr::loadHolidays(const char* filename)
{
	if (!StdFile::exists(filename))
	{
		YCTLogger::error("Holidays configuration file {} not exists", filename);
		return false;
	}

	YCTVariant* root = YCTCfgLoader::load_from_file(filename);
	if (root == NULL)
	{
		YCTLogger::error("Loading holidays config file {} failed", filename);
		return false;
	}

	for (const std::string& hid : root->memberNames())
	{
		YCTVariant* jHolidays = root->get(hid);
		if (!jHolidays->isArray())
			continue;

		TradingDayTpl& trdDayTpl = m_mapTradingDay[hid];
		for (uint32_t i = 0; i < jHolidays->size(); i++)
		{
			YCTVariant* hItem = jHolidays->get(i);
			trdDayTpl._holidays.insert(hItem->asUInt32());
		}
	}

	root->release();

	return true;
}

uint64_t YCTBaseDataMgr::getBoundaryTime(const char* stdPID, uint32_t tDate, bool isSession /* = false */, bool isStart /* = true */)
{
	if (stdPID == NULL)
		return 0;

	if (tDate == 0)
		tDate = TimeUtils::getCurDate();

	std::string tplid = stdPID;
	bool isTpl = false;
	YCTSessionInfo* sInfo = NULL;
	if (isSession)
	{
		sInfo = getSession(stdPID);
		tplid = DEFAULT_HOLIDAY_TPL;
		isTpl = true;
	}
	else
	{
		YCTCommodityInfo* cInfo = getCommodity(stdPID);
		if (cInfo == NULL)
			return 0;

		sInfo = cInfo->getSessionInfo();
	}

	if (sInfo == NULL)
		return 0;

	uint32_t weekday = TimeUtils::getWeekDay(tDate);
	if (weekday == 6 || weekday == 0)
	{
		if (isStart)
			tDate = getNextTDate(tplid.c_str(), tDate, 1, isTpl);
		else
			tDate = getPrevTDate(tplid.c_str(), tDate, 1, isTpl);
	}

	if (sInfo->getOffsetMins() == 0)
	{
		if (isStart)
			return (uint64_t)tDate * 10000 + sInfo->getOpenTime();
		else
			return (uint64_t)tDate * 10000 + sInfo->getCloseTime();
	}

	if (sInfo->getOffsetMins() < 0)
	{
		if (isStart)
			return (uint64_t)tDate * 10000 + sInfo->getOpenTime();
		else
			return (uint64_t)TimeUtils::getNextDate(tDate) * 10000 + sInfo->getCloseTime();
	}
	else
	{
		if (!isStart)
			return (uint64_t)tDate * 10000 + sInfo->getCloseTime();

		tDate = getPrevTDate(tplid.c_str(), tDate, 1, isTpl);
		return (uint64_t)tDate * 10000 + sInfo->getOpenTime();
	}
}

uint32_t YCTBaseDataMgr::calcTradingDate(const char* stdPID, uint32_t uDate, uint32_t uTime, bool isSession /* = false */)
{
	if (uDate == 0)
	{
		TimeUtils::getDateTime(uDate, uTime);
		uTime /= 100000;
	}

	if (stdPID == NULL)
		return uDate;

	std::string tplid = stdPID;
	bool isTpl = false;
	YCTSessionInfo* sInfo = NULL;
	if (isSession)
	{
		sInfo = getSession(stdPID);
		tplid = DEFAULT_HOLIDAY_TPL;
		isTpl = true;
	}
	else
	{
		YCTCommodityInfo* cInfo = getCommodity(stdPID);
		if (cInfo == NULL)
			return uDate;

		sInfo = cInfo->getSessionInfo();
	}

	if (sInfo == NULL)
		return uDate;

	uint32_t offMin = sInfo->offsetTime(uTime, true);
	uint32_t total_mins = sInfo->getTradingMins();
	if (total_mins == 1440)
	{
		if (sInfo->getOffsetMins() > 0 && uTime > offMin)
		{
			return TimeUtils::getNextDate(uDate, 1);
		}
		else if (sInfo->getOffsetMins() < 0 && uTime < offMin)
		{
			return TimeUtils::getNextDate(uDate, -1);
		}

		return uDate;
	}

	uint32_t weekday = TimeUtils::getWeekDay(uDate);
	if (sInfo->getOffsetMins() > 0)
	{
		if (uTime > offMin)
		{
			return getNextTDate(tplid.c_str(), uDate, 1, isTpl);
		}
		else if (weekday == 6 || weekday == 0)
		{
			return getNextTDate(tplid.c_str(), uDate, 1, isTpl);
		}
	}
	else if (sInfo->getOffsetMins() < 0)
	{
		if (uTime < offMin)
		{
			return getPrevTDate(tplid.c_str(), uDate, 1, isTpl);
		}
		else if (weekday == 6 || weekday == 0)
		{
			return getNextTDate(tplid.c_str(), uDate, 1, isTpl);
		}
	}
	else if (weekday == 6 || weekday == 0)
	{
		return getNextTDate(tplid.c_str(), uDate, 1, isTpl);
	}

	return uDate;
}

uint32_t YCTBaseDataMgr::getTradingDate(const char* pid, uint32_t uOffDate /* = 0 */, uint32_t uOffMinute /* = 0 */, bool isTpl /* = false */)
{
	uint32_t curDate = TimeUtils::getCurDate();

	std::string tplKey;
	if (isTpl)
	{
		if (pid == NULL || pid[0] == '\0')
			return curDate;
		tplKey.assign(pid);
	}
	else
	{
		tplKey.assign(getTplIDByPID(pid));
	}

	auto it = m_mapTradingDay.find(tplKey);
	if (it == m_mapTradingDay.end())
	{
		return curDate;
	}

	TradingDayTpl* tpl = (TradingDayTpl*)&it->second;
	if (tpl->_cur_tdate != 0 && uOffDate == 0)
		return tpl->_cur_tdate;

	if (uOffDate == 0)
		uOffDate = curDate;

	uint32_t weekday = TimeUtils::getWeekDay(uOffDate);

	if (weekday == 6 || weekday == 0)
	{
		tpl->_cur_tdate = getNextTDate(tplKey.c_str(), uOffDate, 1, true);
		uOffDate = tpl->_cur_tdate;
	}

	return uOffDate;
}

uint32_t YCTBaseDataMgr::getNextTDate(const char* pid, uint32_t uDate, int days /* = 1 */, bool isTpl /* = false */)
{
	uint32_t curDate = uDate;
	int left = days;
	while (true)
	{
		tm t;
		memset(&t, 0, sizeof(tm));
		t.tm_year = curDate / 10000 - 1900;
		t.tm_mon = (curDate % 10000) / 100 - 1;
		t.tm_mday = curDate % 100;
		time_t ts = mktime(&t);
		ts += 86400;

		tm* newT = localtime(&ts);
		curDate = (newT->tm_year + 1900) * 10000 + (newT->tm_mon + 1) * 100 + newT->tm_mday;
		if (newT->tm_wday != 0 && newT->tm_wday != 6 && !isHoliday(pid, curDate, isTpl))
		{
			left--;
			if (left == 0)
				return curDate;
		}
	}
}

uint32_t YCTBaseDataMgr::getPrevTDate(const char* pid, uint32_t uDate, int days /* = 1 */, bool isTpl /* = false */)
{
	uint32_t curDate = uDate;
	int left = days;
	while (true)
	{
		tm t;
		memset(&t, 0, sizeof(tm));
		t.tm_year = curDate / 10000 - 1900;
		t.tm_mon = (curDate % 10000) / 100 - 1;
		t.tm_mday = curDate % 100;
		time_t ts = mktime(&t);
		ts -= 86400;

		tm* newT = localtime(&ts);
		curDate = (newT->tm_year + 1900) * 10000 + (newT->tm_mon + 1) * 100 + newT->tm_mday;
		if (newT->tm_wday != 0 && newT->tm_wday != 6 && !isHoliday(pid, curDate, isTpl))
		{
			left--;
			if (left == 0)
				return curDate;
		}
	}
}

bool YCTBaseDataMgr::isTradingDate(const char* pid, uint32_t uDate, bool isTpl /* = false */)
{
	if (pid == NULL)
		return false;

	uint32_t wd = TimeUtils::getWeekDay(uDate);
	if (wd != 0 && wd != 6 && !isHoliday(pid, uDate, isTpl))
	{
		return true;
	}

	return false;
}

void YCTBaseDataMgr::setTradingDate(const char* pid, uint32_t uDate, bool isTpl /* = false */)
{
	if (pid == NULL)
		return;

	std::string tplID = pid;
	if (!isTpl)
		tplID.assign(getTplIDByPID(pid));

	auto it = m_mapTradingDay.find(tplID);
	if (it == m_mapTradingDay.end())
		return;

	TradingDayTpl* tpl = (TradingDayTpl*)&it->second;
	tpl->_cur_tdate = uDate;
}

CodeSet* YCTBaseDataMgr::getSessionComms(const char* sid)
{
	if (sid == NULL)
		return NULL;

	auto it = m_mapSessionCode.find(sid);
	if (it == m_mapSessionCode.end())
		return NULL;

	return (CodeSet*)&it->second;
}

const char* YCTBaseDataMgr::getTplIDByPID(const char* pid)
{
	if (pid == NULL || pid[0] == '\0')
		return "";

	const StringVector& ay = StrUtil::split(pid, ".");
	if (ay.size() < 2)
		return "";

	YCTCommodityInfo* commInfo = getCommodity(ay[0].c_str(), ay[1].c_str());
	if (commInfo == NULL)
		return "";

	return commInfo->getTradingTpl();
}
