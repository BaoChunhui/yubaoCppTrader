/*!
 * \file fmtlib.h
 * \project	yubaoCppTrader
 *
 * \author yubao
 * \date 2026/03/12
 * 
 * \brief fmtlib的封装，实现fmt::format和fmt::format_to的封装
 */
#pragma once

#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY
#endif
#include <spdlog/fmt/bundled/format.h>
#include <cstddef>

namespace fmtutil
{
	template<typename... Args>
	inline char* format_to(char* buffer, const char* format, const Args& ...args)
	{
		char* s = fmt::format_to(buffer, format, args...);
		*s = '\0';
		return s;
	}

	// 带缓冲区长度的安全版本：保证在末尾写入 '\0'（超出长度则在末尾截断）
	template<typename... Args>
	inline char* format_to(char* buffer, std::size_t buf_size, const char* format, const Args& ...args)
	{
		if (buf_size == 0)
			return buffer;

		char* s = fmt::format_to(buffer, format, args...);
		*s = '\0';
		buffer[buf_size - 1] = '\0';

		// 若 fmt 写超出了 buf_size，返回值也夹紧到 buffer 末尾，避免返回野指针
		return (s < buffer + buf_size) ? s : (buffer + buf_size - 1);
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