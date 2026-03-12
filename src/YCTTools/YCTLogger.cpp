/*!
 * \file YCTLogger.cpp
 * \project	yubaoCppTrader
 *
 * \author yubao
 * \date 2026/03/12
 * 
 * \brief 日志模块定义，cpp实现
 */
#include <stdio.h>
#include <iostream>
#include <sys/timeb.h>
#ifdef _MSC_VER
#include <time.h>
#else
#include <sys/time.h>
#endif

#include "YCTLogger.h"
#include "../YCTUtils/YCTCfgLoader.h"
#include "../Includes/ILogHandler.h"
#include "../Includes/YCTVariant.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/TimeUtils.hpp"

#include <boost/filesystem.hpp>

#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/async.h>

const char* DYN_PATTERN = "dyn_pattern";

ILogHandler* YCTLogger::m_logHandler = NULL;
YCTLogLevel YCTLogger::m_logLevel = LL_NONE;
bool YCTLogger::m_bStopped = false;
bool YCTLogger::m_bInited = false;
bool YCTLogger::m_bTpInited = false;
SpdLoggerPtr YCTLogger::m_rootLogger = NULL;
YCTLogger::LogPatterns* YCTLogger::m_mapPatterns = NULL;
thread_local char YCTLogger::m_buffer[];
std::set<std::string> YCTLogger::m_setDynLoggers;

inline spdlog::level::level_enum str_to_level(const char* slvl)
{
	if (slvl == NULL)
		return spdlog::level::off;
	if(yubao_stricmp(slvl, "debug") == 0)
	{
		return spdlog::level::debug;
	}
	else if (yubao_stricmp(slvl, "info") == 0)
	{
		return spdlog::level::info;
	}
	else if (yubao_stricmp(slvl, "warn") == 0)
	{
		return spdlog::level::warn;
	}
	else if (yubao_stricmp(slvl, "error") == 0)
	{
		return spdlog::level::err;
	}
	else if (yubao_stricmp(slvl, "fatal") == 0)
	{
		return spdlog::level::critical;
	}
	else
	{
		return spdlog::level::off;
	}
}

inline YCTLogLevel str_to_ll(const char* slvl)
{
	if (slvl == NULL)
		return LL_NONE;
	if (yubao_stricmp(slvl, "debug") == 0)
	{
		return LL_DEBUG;
	}
	else if (yubao_stricmp(slvl, "info") == 0)
	{
		return LL_INFO;
	}
	else if (yubao_stricmp(slvl, "warn") == 0)
	{
		return LL_WARN;
	}
	else if (yubao_stricmp(slvl, "error") == 0)
	{
		return LL_ERROR;
	}
	else if (yubao_stricmp(slvl, "fatal") == 0)
	{
		return LL_FATAL;
	}
	else
	{
		return LL_NONE;
	}
}

inline void checkDirs(const char* filename)
{
	std::string s = StrUtil::standardisePath(filename, false);
	// 支持 '/' 与 '\\'，便于 Windows 路径
	std::size_t pos = s.find_last_of("/\\");

	if (pos == std::string::npos)
		return;

	pos++;

	if (!StdFile::exists(s.substr(0, pos).c_str()))
		boost::filesystem::create_directories(s.substr(0, pos).c_str());
}

// 向 stdout 打印本地时间戳 [YYYY.MM.DD HH:MM:SS]，可选尾部空格。使用线程安全接口。
inline void print_timetag(bool bWithSpace = true)
{
	uint64_t now = TimeUtils::getLocalTimeNow();
	time_t t = static_cast<time_t>(now / 1000);

	tm tNow;
#ifdef _WIN32
	localtime_s(&tNow, &t);
#else
	localtime_r(&t, &tNow);
#endif
	fmt::print("[{}.{:02d}.{:02d} {:02d}:{:02d}:{:02d}]",
		tNow.tm_year + 1900, tNow.tm_mon + 1, tNow.tm_mday,
		tNow.tm_hour, tNow.tm_min, tNow.tm_sec);
	if (bWithSpace)
		fmt::print(" ");
}

void YCTLogger::print_message(const char* buffer)
{
	print_timetag(true);
	fmt::print("{}", buffer != NULL ? buffer : "");
	fmt::print("\r\n");
}

void YCTLogger::initLogger(const char* catName, YCTVariant* cfgLogger)
{
	if (cfgLogger == NULL)
		return;
	bool bAsync = cfgLogger->getBoolean("async");

	YCTVariant* cfgSinks = cfgLogger->get("sinks");
	std::vector<spdlog::sink_ptr> sinks;
	if (cfgSinks != NULL && cfgSinks->type() == YCTVariant::VT_Array)
	{
		for (uint32_t idx = 0; idx < cfgSinks->size(); idx++)
		{
			YCTVariant* cfgSink = cfgSinks->get(idx);
			if (cfgSink == NULL)
				continue;
			const char* type = cfgSink->getCString("type");
			if (type == NULL)
				type = "";
			if (strcmp(type, "daily_file_sink") == 0)
			{
				std::string filename = cfgSink->getString("filename");
				StrUtil::replace(filename, "%s", catName);
				checkDirs(filename.c_str());
				auto sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(filename, 0, 0);
				sink->set_pattern(cfgSink->getCString("pattern"));
				sinks.emplace_back(sink);
			}
			else if (strcmp(type, "basic_file_sink") == 0)
			{
				std::string filename = cfgSink->getString("filename");
				StrUtil::replace(filename, "%s", catName);
				checkDirs(filename.c_str());
				auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename, cfgSink->getBoolean("truncate"));
				sink->set_pattern(cfgSink->getCString("pattern"));
				sinks.emplace_back(sink);
			}
			else if (strcmp(type, "console_sink") == 0)
			{
				auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
				sink->set_pattern(cfgSink->getCString("pattern"));
				sinks.emplace_back(sink);
			}
			else if (strcmp(type, "ostream_sink") == 0)
			{
				auto sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(std::cout, true);
				sink->set_pattern(cfgSink->getCString("pattern"));
				sinks.emplace_back(sink);
			}
		}
	}

	if (!bAsync)
	{
		auto logger = std::make_shared<spdlog::logger>(catName, sinks.begin(), sinks.end());
		logger->set_level(str_to_level(cfgLogger->getCString("level")));
		spdlog::register_logger(logger);
	}
	else
	{
		if(!m_bTpInited)
		{
			spdlog::init_thread_pool(8192, 2);
			m_bTpInited = true;
		}

		auto logger = std::make_shared<spdlog::async_logger>(catName, sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
		logger->set_level(str_to_level(cfgLogger->getCString("level")));
		spdlog::register_logger(logger);
	}

	if(strcmp(catName, "root")==0)
	{
		m_logLevel = str_to_ll(cfgLogger->getCString("level"));
	}
}

void YCTLogger::init(const char* propFile /* = "logcfg.json" */, bool isFile /* = true */, ILogHandler* handler /* = NULL */)
{
	if (m_bInited)
		return;

	m_bStopped = false;  // 支持 stop() 后再次 init() 时恢复打日志

	if (isFile && !StdFile::exists(propFile))
		return;

	YCTVariant* cfg = isFile ? YCTCfgLoader::load_from_file(propFile) : YCTCfgLoader::load_from_content(propFile, false);
	if (cfg == NULL)
		return;

	auto keys = cfg->memberNames();
	for (std::string& key : keys)
	{
		YCTVariant* cfgItem = cfg->get(key.c_str());
		if (cfgItem == NULL)
			continue;
		if (key == DYN_PATTERN)
		{
			if (cfgItem->type() != YCTVariant::VT_Object)
				continue;
			auto pkeys = cfgItem->memberNames();
			for(std::string& pkey : pkeys)
			{
				YCTVariant* cfgPattern = cfgItem->get(pkey.c_str());
				if (cfgPattern == NULL)
					continue;
				if (m_mapPatterns == NULL)
					m_mapPatterns = LogPatterns::create();

				m_mapPatterns->add(pkey.c_str(), cfgPattern, true);
			}
			continue;
		}

		if (cfgItem->type() != YCTVariant::VT_Object)
			continue;
		initLogger(key.c_str(), cfgItem);
	}

	m_rootLogger = getLogger("root");
	if(m_rootLogger == NULL)
	{
		throw std::runtime_error("root logger can not be null, please check the config file");
	}
	spdlog::set_default_logger(m_rootLogger);
	spdlog::flush_every(std::chrono::seconds(2));

	m_logHandler = handler;

	m_bInited = true;
}

void YCTLogger::registerHandler(ILogHandler* handler /* = NULL */)
{
	m_logHandler = handler;
}

void YCTLogger::stop()
{
	m_bStopped = true;
	if (m_mapPatterns)
	{
		m_mapPatterns->release();
		m_mapPatterns = NULL;
	}
	spdlog::shutdown();
	m_rootLogger.reset();
	m_bInited = false;
}

void YCTLogger::debug_imp(SpdLoggerPtr logger, const char* message)
{
	const char* msg = (message != NULL) ? message : "";
	if (logger)
		logger->debug(msg);

	if (logger != m_rootLogger && m_rootLogger)
		m_rootLogger->debug(msg);

	if (m_logHandler)
		m_logHandler->handleLogAppend(LL_DEBUG, msg);
}

void YCTLogger::info_imp(SpdLoggerPtr logger, const char* message)
{
	const char* msg = (message != NULL) ? message : "";
	if (logger)
		logger->info(msg);

	if (logger != m_rootLogger && m_rootLogger)
		m_rootLogger->info(msg);

	if (m_logHandler)
		m_logHandler->handleLogAppend(LL_INFO, msg);
}

void YCTLogger::warn_imp(SpdLoggerPtr logger, const char* message)
{
	const char* msg = (message != NULL) ? message : "";
	if (logger)
		logger->warn(msg);

	if (logger != m_rootLogger && m_rootLogger)
		m_rootLogger->warn(msg);

	if (m_logHandler)
		m_logHandler->handleLogAppend(LL_WARN, msg);
}

void YCTLogger::error_imp(SpdLoggerPtr logger, const char* message)
{
	const char* msg = (message != NULL) ? message : "";
	if (logger)
		logger->error(msg);

	if (logger != m_rootLogger && m_rootLogger)
		m_rootLogger->error(msg);

	if (m_logHandler)
		m_logHandler->handleLogAppend(LL_ERROR, msg);
}

void YCTLogger::fatal_imp(SpdLoggerPtr logger, const char* message)
{
	const char* msg = (message != NULL) ? message : "";
	if (logger)
		logger->critical(msg);

	if (logger != m_rootLogger && m_rootLogger)
		m_rootLogger->critical(msg);

	if (m_logHandler)
		m_logHandler->handleLogAppend(LL_FATAL, msg);
}

void YCTLogger::log_raw(YCTLogLevel ll, const char* message)
{
	if (m_logLevel > ll || m_bStopped)
		return;

	if (!m_bInited)
	{
		print_message(message);
		return;
	}

	auto logger = m_rootLogger;

	if (logger)
	{
		switch (ll)
		{
		case LL_DEBUG:
			debug_imp(logger, message); break;
		case LL_INFO:
			info_imp(logger, message); break;
		case LL_WARN:
			warn_imp(logger, message); break;
		case LL_ERROR:
			error_imp(logger, message); break;
		case LL_FATAL:
			fatal_imp(logger, message); break;
		default:
			break;
		}
	}
}

void YCTLogger::log_raw_by_cat(const char* catName, YCTLogLevel ll, const char* message)
{
	if (m_logLevel > ll || m_bStopped)
		return;

	auto logger = getLogger(catName);
	if (logger == NULL)
		logger = m_rootLogger;

	if (!m_bInited)
	{
		print_timetag(true);
		fmt::print("{}", message != NULL ? message : "");
		fmt::print("\n");
		return;
	}

	if (logger)
	{
		switch (ll)
		{
		case LL_DEBUG:
			debug_imp(logger, message);
			break;
		case LL_INFO:
			info_imp(logger, message);
			break;
		case LL_WARN:
			warn_imp(logger, message);
			break;
		case LL_ERROR:
			error_imp(logger, message);
			break;
		case LL_FATAL:
			fatal_imp(logger, message);
			break;
		default:
			break;
		}
	}	
}

void YCTLogger::log_dyn_raw(const char* patttern, const char* catName, YCTLogLevel ll, const char* message)
{
	if (m_logLevel > ll || m_bStopped)
		return;

	auto logger = getLogger(catName, patttern);
	if (logger == NULL)
		logger = m_rootLogger;

	if (!m_bInited)
	{
		print_timetag(true);
		fmt::print("{}", message != NULL ? message : "");
		fmt::print("\n");
		return;
	}

	if (!logger)
		return;
	switch (ll)
	{
	case LL_DEBUG:
		debug_imp(logger, message);
		break;
	case LL_INFO:
		info_imp(logger, message);
		break;
	case LL_WARN:
		warn_imp(logger, message);
		break;
	case LL_ERROR:
		error_imp(logger, message);
		break;
	case LL_FATAL:
		fatal_imp(logger, message);
		break;
	default:
		break;
	}
}

SpdLoggerPtr YCTLogger::getLogger(const char* logger, const char* pattern /* = "" */)
{
	SpdLoggerPtr ret = spdlog::get(logger);
	if (ret == NULL && pattern != NULL && pattern[0] != '\0')
	{
		//当成动态的日志来处理
		if (m_mapPatterns == NULL)
			return SpdLoggerPtr();

		YCTVariant* cfg = (YCTVariant*)m_mapPatterns->get(pattern);
		if (cfg == NULL)
			return SpdLoggerPtr();

		initLogger(logger, cfg);

		m_setDynLoggers.insert(logger);

		return spdlog::get(logger);
	}

	return ret;
}

void YCTLogger::freeAllDynLoggers()
{
	for (const std::string& name : m_setDynLoggers)
	{
		auto loggerPtr = spdlog::get(name);
		if (!loggerPtr)
			continue;
		spdlog::drop(name);
	}
	m_setDynLoggers.clear();
}