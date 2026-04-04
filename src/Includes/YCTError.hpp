/*!
 * \file YCTError.hpp
 * \project	yubaoCppTrader
 *
 * \author yubao
 * \date 2026/04/04
 *
 * \brief 错误对象定义
 */
#pragma once
#include <string>

#include "YCTObject.hpp"
#include "YCTTypes.h"

NS_YCT_BEGIN

class YCTError : public YCTObject
{
protected:
	YCTError()
		: m_errCode(YCTErroCode::EC_NONE)
	{
	}

	virtual ~YCTError() {}

public:
	static YCTError* create(YCTErroCode ec, const char* errmsg)
	{
		YCTError* pRet = new YCTError();
		pRet->m_errCode = ec;
		if (errmsg)
			pRet->m_strMsg = errmsg;
		return pRet;
	}

	const char* getMessage() const { return m_strMsg.c_str(); }
	YCTErroCode getErrorCode() const { return m_errCode; }

protected:
	YCTErroCode		m_errCode;
	std::string		m_strMsg;
};

NS_YCT_END
