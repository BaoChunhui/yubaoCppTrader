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

	// 带缓冲区长度的安全版本：最多写 buf_size-1 个字符，并保证以 '\0' 结尾（超出则截断）
	template<typename... Args>
	inline char* format_to(char* buffer, std::size_t buf_size, const char* format, const Args& ...args)
	{
		if (buf_size == 0)
			return buffer;

		auto result = fmt::format_to_n(buffer, buf_size - 1, format, args...);
		*result.out = '\0';
		return result.out;
	}

	template<int BUFSIZE=512, typename... Args>
	inline const char* format(const char* format, const Args& ...args)
	{
		thread_local static char buffer[BUFSIZE];
		auto result = fmt::format_to_n(buffer, BUFSIZE - 1, format, args...);
		*result.out = '\0';
		return buffer;
	}
}