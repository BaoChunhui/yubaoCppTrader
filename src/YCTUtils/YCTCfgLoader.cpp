/*!
 * \file YCTCfgLoader.cpp
 * \project	yubaoCppTrader
 *
 * \author yubao
 * \date 2026/03/11
 * 
 * \brief 从 JSON/YAML 读取配置文件的封装cpp实现
 */
#include "YCTCfgLoader.h"
#include "../Share/StrUtil.hpp"
#include "../Share/StdUtils.hpp"

#include "../Share/charconv.hpp"

#include "../Includes/YCTVariant.hpp"
#include <rapidjson/document.h>

#include "../YCTUtils/yamlcpp/yaml.h"
namespace rj = rapidjson;


bool json_to_variant(const rj::Value& root, YCTVariant* params)
{
	if (root.IsObject() && params->type() != YCTVariant::VT_Object)
		return false;

	if (root.IsArray() && params->type() != YCTVariant::VT_Array)
		return false;

	// root 必须是 Object 或 Array，否则 GetObject()/GetArray() 未定义
	if (!root.IsObject() && !root.IsArray())
		return false;

    if (root.IsObject())
    {
        for (auto& m : root.GetObject())
		{
			const char* key = m.name.GetString();
			const rj::Value& item = m.value;
			switch (item.GetType())
			{
			case rj::kObjectType:
			{
				YCTVariant* subObj = YCTVariant::createObject();
				if (json_to_variant(item, subObj))
					params->append(key, subObj, false);
				else
					subObj->release();
			}
				break;
			case rj::kArrayType:
			{
				YCTVariant* subAy = YCTVariant::createArray();
				if (json_to_variant(item, subAy))
					params->append(key, subAy, false);
				else
					subAy->release();
			}
				break;
			case rj::kNumberType:
				if (item.IsInt())
					params->append(key, item.GetInt());
				else if (item.IsUint())
					params->append(key, item.GetUint());
				else if (item.IsInt64())
					params->append(key, item.GetInt64());
				else if (item.IsUint64())
					params->append(key, item.GetUint64());
				else if (item.IsDouble())
					params->append(key, item.GetDouble());
				break;
			case rj::kStringType:
				params->append(key, item.GetString());
				break;
			case rj::kTrueType:
			case rj::kFalseType:
				params->append(key, item.GetBool());
				break;
			default:
				break;
			}
		}
    }
    else
    {
		for (auto& item : root.GetArray())
		{
			switch (item.GetType())
			{
			case rj::kObjectType:
			{
				YCTVariant* subObj = YCTVariant::createObject();
				if (json_to_variant(item, subObj))
					params->append(subObj, false);
				else
					subObj->release();
			}
			break;
			case rj::kArrayType:
			{
				YCTVariant* subAy = YCTVariant::createArray();
				if (json_to_variant(item, subAy))
					params->append(subAy, false);
				else
					subAy->release();
			}
			break;
			case rj::kNumberType:
				if (item.IsInt())
					params->append(item.GetInt());
				else if (item.IsUint())
					params->append(item.GetUint());
				else if (item.IsInt64())
					params->append(item.GetInt64());
				else if (item.IsUint64())
					params->append(item.GetUint64());
				else if (item.IsDouble())
					params->append(item.GetDouble());
				break;
			case rj::kStringType:
				params->append(item.GetString());
				break;
			case rj::kTrueType:
			case rj::kFalseType:
				params->append(item.GetBool());
				break;
			default:
				break;
			}
		}
    }
    return true;
}

YCTVariant* YCTCfgLoader::load_from_json(const char* content)
{
	rj::Document root;
	root.Parse(content);

	if (root.HasParseError())
		return NULL;

	YCTVariant* ret = YCTVariant::createObject();
	if (!json_to_variant(root, ret))
	{
		ret->release();
		return NULL;
	}

	return ret;
}

bool yaml_to_variant(const YAML::Node& root, YCTVariant* params)
{
	if (root.IsNull() && params->type() != YCTVariant::VT_Object)
		return false;

	if (root.IsSequence() && params->type() != YCTVariant::VT_Array)
		return false;

	bool isMap = root.IsMap();
	for (auto& m : root)
	{
		std::string key = isMap ? m.first.as<std::string>() : "";
		const YAML::Node& item = isMap ? m.second : m;
		switch (item.Type())
		{
		case YAML::NodeType::Map:
		{
			YCTVariant* subObj = YCTVariant::createObject();
			if (yaml_to_variant(item, subObj))
			{
				if(isMap)
					params->append(key.c_str(), subObj, false);
				else
					params->append(subObj, false);
			}
			else
			{
				subObj->release();
			}
		}
			break;
		case YAML::NodeType::Sequence:
		{
			YCTVariant* subAy = YCTVariant::createArray();
			if (yaml_to_variant(item, subAy))
			{
				if (isMap)
					params->append(key.c_str(), subAy, false);
				else
					params->append(subAy, false);
			}
			else
			{
				subAy->release();
			}
		}
			break;
		case YAML::NodeType::Scalar:
			if (isMap)
				params->append(key.c_str(), item.as<std::string>().c_str());
			else
				params->append(item.as<std::string>().c_str());
			break;
		default:
			break;
		}
	}

	return true;
}

YCTVariant* YCTCfgLoader::load_from_yaml(const char* content)
{
	YAML::Node root = YAML::Load(content);

	if (root.IsNull())
		return NULL;

	YCTVariant* ret = YCTVariant::createObject();
	if (!yaml_to_variant(root, ret))
	{
		ret->release();
		return NULL;
	}

	return ret;
}

YCTVariant* YCTCfgLoader::load_from_content(const std::string& content, bool isYaml /* = false */)
{
	//加一个自动检测编码的逻辑
	bool isUTF8 = EncodingHelper::isUtf8((unsigned char*)content.data(), content.size());

	std::string buffer;
	//Linux下得是UTF8
	//Win下得是GBK
#ifdef _WIN32
	if (isUTF8)
		buffer = UTF8toChar(content);
#else
	if (!isUTF8)
		buffer = ChartoUTF8(content);
#endif

	if (buffer.empty())
		buffer = content;

	if (isYaml)
		return load_from_yaml(buffer.c_str());
	else
		return load_from_json(buffer.c_str());
}

YCTVariant* YCTCfgLoader::load_from_file(const char* filename)
{
	if (!StdFile::exists(filename))
		return NULL;

	std::string content;
	StdFile::read_file_content(filename, content);
	if (content.empty())
		return NULL;

	//加一个自动检测编码的逻辑
	bool isUTF8 = EncodingHelper::isUtf8((unsigned char*)content.data(), content.size());

	//By Wesley @ 2022.01.07
	//Linux下得是UTF8
	//Win下得是GBK
#ifdef _WIN32
	if(isUTF8)
		content = UTF8toChar(content);
#else
	if (!isUTF8)
		content = ChartoUTF8(content);
#endif

	if (StrUtil::endsWith(filename, ".json"))
		return load_from_json(content.c_str());
	else if (StrUtil::endsWith(filename, ".yaml") || StrUtil::endsWith(filename, ".yml"))
		return load_from_yaml(content.c_str());

	return NULL;
}