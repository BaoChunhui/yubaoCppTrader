/*!
 * \file FasterDefs.h
 * \project	yubaoCppTrader
 *
 * \author yubao
 * \date 2026/03/09
 * 
 * \brief 集中定义yubaoCppTrader项目用的高性能 map/set 和字符串哈希策略
 */
#pragma once
#include <string.h>
#include "YCTMacros.h"
#include "../FasterLibs/tsl/robin_map.h"
#include "../FasterLibs/tsl/robin_set.h"
#include "../FasterLibs/ankerl/unordered_dense.h"

NS_YCT_BEGIN

struct string_hash
{
    //BKDRHash算法
    std::size_t operator()(const std::string& key) const
    {
        size_t seed = 131;
        size_t hash = 0;

        char* str = (char*)key.c_str();
        while (*str)
        {
            hash = hash * seed + (*str++);
        }

        return (hash & 0x7FFFFFFF);
    }
};

template<class Key, class T>
class fastest_hashmap : public tsl::robin_map<Key, T>
{
public:
	typedef tsl::robin_map<Key, T> Container;
	fastest_hashmap():Container(){}
};

template<class T>
class fastest_hashmap<std::string, T> : public tsl::robin_map<std::string, T, string_hash>
{
public:
	typedef tsl::robin_map<std::string, T, string_hash> Container;
	fastest_hashmap() :Container() {}
};

template<class Key>
class fastest_hashset : public tsl::robin_set<Key>
{
public:
	typedef tsl::robin_set<Key>	Container;
	fastest_hashset() :Container() {}
};

template<>
class fastest_hashset<std::string> : public tsl::robin_set<std::string, string_hash>
{
public:
	typedef tsl::robin_set<std::string, string_hash> Container;
	fastest_hashset() :Container() {}
};

typedef fastest_hashset<std::string> CodeSet;

//下面使用unordered_dense

template<class Key, class T, class Hash = std::hash<Key>>
class yct_hashmap : public ankerl::unordered_dense::map<Key, T, Hash>
{
public:
	typedef ankerl::unordered_dense::map<Key, T, Hash> Container;
	yct_hashmap() :Container() {}
};

template<class T>
class yct_hashmap<std::string, T, string_hash> : public ankerl::unordered_dense::map<std::string, T, string_hash>
{
public:
	typedef ankerl::unordered_dense::map<std::string, T, string_hash> Container;
	yct_hashmap() :Container() {}
};

template<class Key, class Hash = std::hash<Key>>
class yct_hashset : public ankerl::unordered_dense::set<Key, Hash>
{
public:
	typedef ankerl::unordered_dense::set<Key, Hash>	Container;
	yct_hashset() :Container() {}
};

template<>
class yct_hashset<std::string, string_hash> : public ankerl::unordered_dense::set<std::string, string_hash>
{
public:
	typedef ankerl::unordered_dense::set<std::string, string_hash> Container;
	yct_hashset() :Container() {}
};

NS_YCT_END