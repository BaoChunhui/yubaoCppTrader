/*!
 * \file YCTLogger.h
 * \project	yubaoCppTrader
 *
 * \author yubao
 * \date 2026/03/12
 * 
 * \brief 日志模块定义，头文件
 */
#pragma once
#include "../Includes/YCTTypes.h"
#include "../Includes/YCTCollection.hpp"
#include "../Share/fmtlib.h"

#include <memory>
#include <sstream>
#include <thread>
#include <set>
#include <cstring>

#include <spdlog/spdlog.h>

typedef std::shared_ptr<spdlog::logger> SpdLoggerPtr;

NS_YCT_BEGIN
class ILogHandler;
class YCTVariant;
NS_YCT_END

USING_NS_YCT;

#define MAX_LOG_BUF_SIZE 2048

class YCTLogger
{
private:
	static bool					m_bInited;
	static bool					m_bTpInited;
	static bool					m_bStopped;
	static ILogHandler*			m_logHandler;
	static YCTLogLevel			m_logLevel;

	static SpdLoggerPtr			m_rootLogger;

	typedef YCTHashMap<std::string>	LogPatterns;
	static LogPatterns*				m_mapPatterns;
	static std::set<std::string>	m_setDynLoggers;

	static thread_local char	m_buffer[MAX_LOG_BUF_SIZE];

	static void debug_imp(SpdLoggerPtr logger, const char* message);
	static void info_imp(SpdLoggerPtr logger, const char* message);
	static void warn_imp(SpdLoggerPtr logger, const char* message);
	static void error_imp(SpdLoggerPtr logger, const char* message);
	static void fatal_imp(SpdLoggerPtr logger, const char* message);

	static void initLogger(const char* catName, YCTVariant* cfgLogger);
	static SpdLoggerPtr getLogger(const char* logger, const char* pattern = "");

	static void print_message(const char* buffer);

public:
	/*
	 *	直接输出
	 */
	static void log_raw(YCTLogLevel ll, const char* message);

	/*
	 *	分类输出
	 */
	static void log_raw_by_cat(const char* catName, YCTLogLevel ll, const char* message);

	/*
	 *	动态分类输出
	 */
	static void log_dyn_raw(const char* patttern, const char* catName, YCTLogLevel ll, const char* message);

	// fmt::format风格接口
	template<typename... Args>
	static void debug(const char* format, const Args& ...args)
	{
		if (m_logLevel > LL_DEBUG || m_bStopped)
			return;

		fmtutil::format_to(m_buffer, MAX_LOG_BUF_SIZE, format, args...);

		if (!m_bInited)
		{
			print_message(m_buffer);
			return;
		}

		debug_imp(m_rootLogger, m_buffer);
	}

	template<typename... Args>
	static void info(const char* format, const Args& ...args)
	{
		if (m_logLevel > LL_INFO || m_bStopped)
			return;

		fmtutil::format_to(m_buffer, MAX_LOG_BUF_SIZE, format, args...);

		if (!m_bInited)
		{
			print_message(m_buffer);
			return;
		}

		info_imp(m_rootLogger, m_buffer);
	}

	template<typename... Args>
	static void warn(const char* format, const Args& ...args)
	{
		if (m_logLevel > LL_WARN || m_bStopped)
			return;

		fmtutil::format_to(m_buffer, MAX_LOG_BUF_SIZE, format, args...);

		if (!m_bInited)
		{
			print_message(m_buffer);
			return;
		}

		warn_imp(m_rootLogger, m_buffer);
	}

	template<typename... Args>
	static void error(const char* format, const Args& ...args)
	{
		if (m_logLevel > LL_ERROR || m_bStopped)
			return;

		fmtutil::format_to(m_buffer, MAX_LOG_BUF_SIZE, format, args...);

		if (!m_bInited)
		{
			print_message(m_buffer);
			return;
		}

		error_imp(m_rootLogger, m_buffer);
	}

	template<typename... Args>
	static void fatal(const char* format, const Args& ...args)
	{
		if (m_logLevel > LL_FATAL || m_bStopped)
			return;

		fmtutil::format_to(m_buffer, MAX_LOG_BUF_SIZE, format, args...);

		if (!m_bInited)
		{
			print_message(m_buffer);
			return;
		}

		fatal_imp(m_rootLogger, m_buffer);
	}

	template<typename... Args>
	static void log(YCTLogLevel ll, const char* format, const Args& ...args)
	{
		if (m_logLevel > ll || m_bStopped)
			return;

		fmtutil::format_to(m_buffer, MAX_LOG_BUF_SIZE, format, args...);

		log_raw(ll, m_buffer);
	}

	template<typename... Args>
	static void log_by_cat(const char* catName, YCTLogLevel ll, const char* format, const Args& ...args)
	{
		if (m_logLevel > ll || m_bStopped)
			return;

		fmtutil::format_to(m_buffer, MAX_LOG_BUF_SIZE, format, args...);

		log_raw_by_cat(catName, ll, m_buffer);
	}

	template<typename... Args>
	static void log_by_cat_prefix(const char* catName, YCTLogLevel ll, const char* format, const Args& ...args)
	{
		if (m_logLevel > ll || m_bStopped)
			return;

		if (catName == NULL)
			catName = "";

		m_buffer[0] = '[';
		std::size_t catLen = std::strlen(catName);
		if (catLen > MAX_LOG_BUF_SIZE - 4) // "[", "]", '\0' 以及至少 1 字节正文
			catLen = MAX_LOG_BUF_SIZE - 4;

		std::memcpy(m_buffer + 1, catName, catLen);
		m_buffer[catLen + 1] = ']';
		m_buffer[catLen + 2] = '\0';

		char* s = m_buffer + catLen + 2;
		fmtutil::format_to(s, MAX_LOG_BUF_SIZE - (catLen + 2), format, args...);
		log_raw_by_cat(catName, ll, m_buffer);
	}

	template<typename... Args>
	static void log_dyn(const char* patttern, const char* catName, YCTLogLevel ll, const char* format, const Args& ...args)
	{
		if (m_logLevel > ll || m_bStopped)
			return;

		if (catName == NULL)
			catName = "";

		fmtutil::format_to(m_buffer, MAX_LOG_BUF_SIZE, format, args...);

		log_dyn_raw(patttern, catName, ll, m_buffer);
	}

	template<typename... Args>
	static void log_dyn_prefix(const char* patttern, const char* catName, YCTLogLevel ll, const char* format, const Args& ...args)
	{
		if (m_logLevel > ll || m_bStopped)
			return;

		if (catName == NULL)
			catName = "";

		m_buffer[0] = '[';
		std::size_t catLen = std::strlen(catName);
		if (catLen > MAX_LOG_BUF_SIZE - 4)
			catLen = MAX_LOG_BUF_SIZE - 4;

		std::memcpy(m_buffer + 1, catName, catLen);
		m_buffer[catLen + 1] = ']';
		m_buffer[catLen + 2] = '\0';

		char* s = m_buffer + catLen + 2;
		fmtutil::format_to(s, MAX_LOG_BUF_SIZE - (catLen + 2), format, args...);
		log_dyn_raw(patttern, catName, ll, m_buffer);
	}

	static void init(const char* propFile = "logcfg.json", bool isFile = true, ILogHandler* handler = NULL);

	static void registerHandler(ILogHandler* handler = NULL);

	static void stop();

	static void freeAllDynLoggers();
};