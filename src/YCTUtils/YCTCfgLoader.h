/*!
 * \file YCTCfgLoader.h
 * \project	yubaoCppTrader
 *
 * \author yubao
 * \date 2026/03/11
 * 
 * \brief 从 JSON/YAML 读取配置文件的封装头文件
 */
#pragma once
#include "../Includes/YCTMacros.h"

#include <string>

NS_YCT_BEGIN
class YCTVariant;
NS_YCT_END

USING_NS_YCT;

class YCTCfgLoader
{
	static YCTVariant* load_from_json(const char* content);
	static YCTVariant* load_from_yaml(const char* content);

public:
	static YCTVariant* load_from_file(const char* filename);
	static YCTVariant* load_from_content(const std::string& content, bool isYaml = false);

	static YCTVariant* load_from_file(const std::string& filename)
	{
		return load_from_file(filename.c_str());
	}
};