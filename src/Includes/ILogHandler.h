/*!
 * \file ILogHandler.h
 * \project	yubaoCppTrader
 *
 * \author yubao
 * \date 2026/03/12
 * 
 * \brief 日志转发模块接口定义
 */
#pragma once
#include "YCTMacros.h"
#include "YCTTypes.h"

NS_YCT_BEGIN
class ILogHandler
{
public:
    virtual void handleLogAppend(YCTLogLevel ll, const char* msg) = 0;
};
NS_YCT_END