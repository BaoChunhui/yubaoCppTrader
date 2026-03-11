/*!
 * \file fmtlib.h
 * \project	yubaoCppTrader
 *
 * \author yubao
 * \date 2026/03/11
 * 
 * \brief fmtlib的封装，实现fmt::format和fmt::format_to的封装
 */
#pragma once

#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY
#endif
#include <spdlog/fmt/bundled/format.h>

namespace fmtutil
{
	template<typename... Args>
	inline char* format_to(char* buffer, const char* format, const Args& ...args)
	{
		char* s = fmt::format_to(buffer, format, args...);
		*s = '\0';
		return s;
	}

	template<int BUFSIZE=512, typename... Args>
	inline const char* format(const char* format, const Args& ...args)
	{
		thread_local static char buffer[BUFSIZE];
		char* s = fmt::format_to(buffer, format, args...);
		*s = '\0';
		buffer[BUFSIZE - 1] = '\0';
		return buffer;
	}
}