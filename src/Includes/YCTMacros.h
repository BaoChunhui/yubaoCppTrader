/*!
 * \file YCTMarcos.h
 * \project	yubaoCppTrader
 *
 * \author yubao
 * \date 2026/03/08
 * 
 * \brief yubaoCppTrader基础宏(Marco)定义文件
 */
#pragma once
#include <limits.h>
#include <string.h>

#ifndef NOMINMAX
#define NOMINMAX
#endif

#define MAX_INSTRUMENT_LENGTH	32
#define MAX_EXCHANGE_LENGTH		16

#define STATIC_CONVERT(x,T)		static_cast<T>(x)

#ifndef DBL_MAX
#define DBL_MAX 1.7976931348623158e+308
#endif

#ifndef FLT_MAX
#define FLT_MAX 3.402823466e+38F
#endif

#ifdef _MSC_VER
#define INVALID_DOUBLE		DBL_MAX
#define INVALID_INT32		INT_MAX
#define INVALID_UINT32		UINT_MAX
#define INVALID_INT64		_I64_MAX
#define INVALID_UINT64		_UI64_MAX
#else
#define INVALID_DOUBLE		1.7976931348623158e+308
#define INVALID_INT32		2147483647
#define INVALID_UINT32		0xffffffffUL
#define INVALID_INT64		9223372036854775807LL
#define INVALID_UINT64		0xffffffffffffffffULL
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#define NS_YCT_BEGIN	namespace yct{
#define NS_YCT_END	}//namespace yct
#define	USING_NS_YCT	using namespace yct

#ifndef EXPORT_FLAG
#ifdef _MSC_VER
#define EXPORT_FLAG __declspec(dllexport)
#else
#define EXPORT_FLAG __attribute__((__visibility__("default")))
#endif
#endif

#ifndef PORTER_FLAG
#ifdef _MSC_VER
#define PORTER_FLAG _cdecl
#else
#define PORTER_FLAG 
#endif
#endif

typedef unsigned int		yubaoUInt32;
typedef unsigned long long	yubaotUInt64;
typedef const char*			yubaoString;

#ifdef _MSC_VER
#define yubao_stricmp _stricmp
#else
#define yubao_stricmp strcasecmp
#endif