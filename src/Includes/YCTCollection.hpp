/*!
 * \file YCTCollection.hpp
 * \project	yubaoCppTrader
 *
 * \author yubao
 * \date 2026/03/10
 * 
 * \brief yubaoCppTrader集合组件定义文件
 */
#pragma once
#include "YCTMacros.h"
#include "YCTObject.hpp"
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include "FasterDefs.h"

#include <deque>

NS_YCT_BEGIN

//////////////////////////////////////////////////////////////////////////
//YCTArray

/*
 *	yubaoCppTrader数组容器
 *	内部使用vector实现
 *	数据使用YCTObject指针对象
 *	所有YCTObject的派生类都可以使用
 *	用于yubaoCppTrader项目内使用
 */
class YCTArray : public YCTObject
{
public:
	/*
	 *	数组迭代器
	 */
    typedef std::vector<YCTObject*>::iterator Iterator;
    typedef std::vector<YCTObject*>::const_iterator ConstIterator;
 
    typedef std::vector<YCTObject*>::reverse_iterator ReverseIterator;
    typedef std::vector<YCTObject*>::const_reverse_iterator ConstReverseIterator;

    typedef std::function<bool(YCTObject*, YCTObject*)>	SortFunc;

	/*
	 *	创建数组对象
	 */
	inline static YCTArray* create()
	{
		YCTArray* pRet = new YCTArray();
		return pRet;
	}

	/*
	 *	读取数组长度
	 */
	inline uint32_t size() const noexcept { return (uint32_t)_vec.size(); }

	/*
	 *	清空数组,并重新分配空间
	 *	调用该函数会预先分配长度
	 *	预先分配好的数据都是NULL
	 */
	inline void resize(uint32_t _size)
	{
		if(!_vec.empty())
			clear();

		_vec.resize(_size, NULL);
	}

	/*
	 *	读取数组指定位置的数据
	 *	对比grab接口,at接口只取得数据
	 *	不增加数据的引用计数
	 *	grab接口读取数据以后,增加引用计数
	 */
	inline YCTObject* at(uint32_t idx) noexcept
	{
		if(idx <0 || idx >= _vec.size())
			return NULL;

		YCTObject* pRet = _vec.at(idx);
		return pRet;
	}

	inline uint32_t idxOf(YCTObject* obj) noexcept
	{
		if (obj == NULL)
			return -1;

		uint32_t idx = 0;
		auto it = _vec.begin();
		for (; it != _vec.end(); it++, idx++)
		{
			if (obj == (*it))
				return idx;
		}

		return -1;
	}

	template<typename T> 
	inline T* at(uint32_t idx) noexcept
	{
		if(idx <0 || idx >= _vec.size())
			return NULL;

		YCTObject* pRet = _vec.at(idx);
		return static_cast<T*>(pRet);
	}

	/*
	 *	[]操作符重载
	 *	用法同at函数
	 */
	inline YCTObject* operator [](uint32_t idx) noexcept
	{
		if(idx <0 || idx >= _vec.size())
			return NULL;

		YCTObject* pRet = _vec.at(idx);
		return pRet;
	}

	/*
	 *	读取数组指定位置的数据
	 *	增加引用计数
	 */
	inline YCTObject* grab(uint32_t idx)
	{
		if(idx <0 || idx >= _vec.size())
			return NULL;

		YCTObject* pRet = _vec.at(idx);
		if (pRet)
			pRet->retain();

		return pRet;
	}

	/*
	 *	数组末尾追加数据
	 *	数据自动增加引用计数
	 */
	inline void append(YCTObject* obj, bool bAutoRetain = true)
	{
		if (bAutoRetain && obj)
			obj->retain();

		_vec.emplace_back(obj);
	}

	/*
	 *	设置指定位置的数据
	 *	如果该位置已有数据,则释放掉
	 *	新数据引用计数增加
	 */
	inline void set(uint32_t idx, YCTObject* obj, bool bAutoRetain = true)
	{
		if(idx >= _vec.size() || obj == NULL)
			return;

		if(bAutoRetain)
			obj->retain();

		YCTObject* oldObj = _vec.at(idx);
		if(oldObj)
			oldObj->release();

		_vec[idx] = obj;
	}

	inline void append(YCTArray* ay)
	{
		if(ay == NULL)
			return;

		_vec.insert(_vec.end(), ay->_vec.begin(), ay->_vec.end());
		ay->_vec.clear();
	}

	/*
	 *	数组清空
	 *	数组内所有数据释放引用
	 */
	inline void clear()
	{
		{
			std::vector<YCTObject*>::iterator it = _vec.begin();
			for (; it != _vec.end(); it++)
			{
				YCTObject* obj = (*it);
				if (obj)
					obj->release();
			}
		}
		
		_vec.clear();
	}

	/*
	 *	释放数组对象,用法如YCTObject
	 *	不同的是,如果引用计数为1时
	 *	释放所有数据
	 */
	virtual void release()
	{
		if (m_uRefs == 0)
			return;

		try
		{
			m_uRefs--;
			if (m_uRefs == 0)
			{
				clear();
				delete this;
			}
		}
		catch(...)
		{

		}
	}

	/*
	 *	取得数组对象起始位置的迭代器
	 */
	inline Iterator begin() noexcept
	{
		return _vec.begin();
	}

	inline ConstIterator begin() const noexcept
	{
		return _vec.begin();
	}

	inline ReverseIterator rbegin() noexcept
	{
		return _vec.rbegin();
	}

	inline ConstReverseIterator rbegin() const noexcept
	{
		return _vec.rbegin();
	}

	/*
	 *	取得数组对象末尾位置的迭代器
	 */
	inline Iterator end() noexcept
	{
		return _vec.end();
	}

	inline ConstIterator end() const noexcept
	{
		return _vec.end();
	}

	inline ReverseIterator rend() noexcept
	{
		return _vec.rend();
	}

	inline ConstReverseIterator rend() const noexcept
	{
		return _vec.rend();
	}

	inline void sort(SortFunc func)
	{
		std::sort(_vec.begin(), _vec.end(), func);
	}

protected:
	YCTArray():_holding(false){}
	virtual ~YCTArray(){}

	std::vector<YCTObject*>	_vec;
	std::atomic<bool> _holding;
};


/*
 *	map容器
 *	内部采用std:map实现
 *	模版类型为key类型
 *	数据使用YCTObject指针对象
 *	所有YCTObject的派生类都适用
 */
template <class T>
class YCTMap : public YCTObject
{
public:
	/*
	 *	容器迭代器的定义
	 */
	typedef typename std::map<T, YCTObject*> _YCTMapType;
	typedef typename _YCTMapType::iterator Iterator;
	typedef typename _YCTMapType::const_iterator ConstIterator;
	typedef typename _YCTMapType::reverse_iterator ReverseIterator;
	typedef typename _YCTMapType::const_reverse_iterator ConstReverseIterator;

	/*
	 *	创建map容器
	 */
	inline static YCTMap<T>* create()
	{
		YCTMap<T>* pRet = new YCTMap<T>();
		return pRet;
	}

	/*
	 *	返回map容器的大小
	 */
	inline uint32_t size() const noexcept { return (uint32_t)_map.size(); }

	/*
	 *	读取指定key对应的数据
	 *	不增加数据的引用计数
	 *	没有则返回NULL
	 */
	inline YCTObject* get(const T &_key)
	{
		Iterator it = _map.find(_key);
		if(it == _map.end())
			return NULL;

		YCTObject* pRet = it->second;
		return pRet;
	}

	/*
	 *	[]操作符重载
	 *	用法同get函数
	 */
	inline YCTObject* operator[](const T &_key)
	{
		Iterator it = _map.find(_key);
		if(it == _map.end())
			return NULL;

		YCTObject* pRet = it->second;
		return pRet;
	}

	/*
	 *	读取指定key对应的数据
	 *	增加数据的引用计数
	 *	没有则返回NULL
	 */
	inline YCTObject* grab(const T &_key)
	{
		Iterator it = _map.find(_key);
		if(it == _map.end())
			return NULL;

		YCTObject* pRet = it->second;
		if (pRet)
			pRet->retain();

		return pRet;
	}

	/*
	 *	新增一个数据,并增加数据引用计数
	 *	如果key存在,则将原有数据释放
	 */
	inline void add(T _key, YCTObject* obj, bool bAutoRetain = true)
	{
		if(bAutoRetain && obj)
			obj->retain();

		YCTObject* pOldObj = NULL;
		Iterator it = _map.find(_key);
		if(it != _map.end())
		{
			pOldObj = it->second;
		}

		_map[_key] = obj;

		if (pOldObj) pOldObj->release();
	}

	/*
	 *	根据key删除一个数据
	 *	如果key存在,则对应数据引用计数-1
	 */
	inline void remove(T _key)
	{
		Iterator it = _map.find(_key);
		if(it != _map.end())
		{
			YCTObject* obj = it->second;
			_map.erase(it);
			if (obj) obj->release();
		}
	}

	/*
	 *	获取容器起始位置的迭代器
	 */
	inline Iterator begin() noexcept
	{
		return _map.begin();
	}

	inline ConstIterator begin() const noexcept
	{
		return _map.begin();
	}

	/*
	 *	获取容器末尾位置的迭代器
	 */
	inline Iterator end() noexcept
	{
		return _map.end();
	}

	inline ConstIterator end() const noexcept
	{
		return _map.end();
	}

	/*
	 *	获取容器起始位置的迭代器
	 */
	inline ReverseIterator rbegin() noexcept
	{
		return _map.rbegin();
	}

	inline ConstReverseIterator rbegin() const noexcept
	{
		return _map.rbegin();
	}

	/*
	 *	获取容器末尾位置的迭代器
	 */
	inline ReverseIterator rend() noexcept
	{
		return _map.rend();
	}

	inline ConstReverseIterator rend() const noexcept
	{
		return _map.rend();
	}

	inline Iterator find(const T& key)
	{
		return _map.find(key);
	}

	inline ConstIterator find(const T& key) const
	{
		return _map.find(key);
	}

	inline void erase(ConstIterator it) noexcept
	{
		_map.erase(it);
	}

	inline void erase(Iterator it) noexcept
	{
		_map.erase(it);
	}

	inline void erase(T key)
	{
		_map.erase(key);
	}

	inline Iterator lower_bound(const T& key)
	{
		 return _map.lower_bound(key);
	}

	inline ConstIterator lower_bound(const T& key) const
	{
		return _map.lower_bound(key);
	}

	inline Iterator upper_bound(const T& key)
	{
	 	 return _map.upper_bound(key);
	}

	inline ConstIterator upper_bound(const T& key) const
	{
		return _map.upper_bound(key);
	}

	inline YCTObject* last() noexcept
	{
		if(_map.empty())
			return NULL;
		
		return _map.rbegin()->second;
	}

	/*
	 *	清空容器
	 *	容器内所有数据引用计数-1
	 */
	inline void clear()
	{
		Iterator it = _map.begin();
		for(; it != _map.end(); it++)
		{
			it->second->release();
		}
		_map.clear();
	}

	/*
	 *	释放容器对象
	 *	如果容器引用计数为1,则清空所有数据
	 */
	virtual void release()
	{
		if (m_uRefs == 0)
			return;

		try
		{
			m_uRefs--;
			if (m_uRefs == 0)
			{
				clear();
				delete this;
			}
		}
		catch(...)
		{

		}
	}

protected:
	YCTMap(){}
	~YCTMap(){}

	_YCTMapType	_map;
};

/*
 *	unordered_map容器
 *	内部采用yct_hashmap实现
 *	模版类型为key类型
 *	数据使用YCTObject指针对象
 *	所有YCTObject的派生类都适用
 */
template <typename T, class Hash = std::hash<T>>
class YCTHashMap : public YCTObject
{
public:
	/*
	 *	容器迭代器的定义
	 */
	typedef yct_hashmap<T, YCTObject*, Hash> _YCTHashMapType;
	typedef typename _YCTHashMapType::const_iterator ConstIterator;

	/*
	 *	创建map容器
	 */
	inline static YCTHashMap<T, Hash>*	create()
	{
		YCTHashMap<T, Hash>* pRet = new YCTHashMap<T, Hash>();
		return pRet;
	}

	/*
	 *	返回map容器的大小
	 */
	inline uint32_t size() const noexcept { return (uint32_t)_map.size(); }

	/*
	 *	读取指定key对应的数据
	 *	不增加数据的引用计数
	 *	没有则返回NULL
	 */
	inline YCTObject* get(const T &_key)
	{
		auto it = _map.find(_key);
		if(it == _map.end())
			return NULL;

		YCTObject* pRet = it->second;
		return pRet;
	}

	/*
	 *	读取指定key对应的数据
	 *	增加数据的引用计数
	 *	没有则返回NULL
	 */
	inline YCTObject* grab(const T &_key)
	{
		auto it = _map.find(_key);
		if(it == _map.end())
			return NULL;

		YCTObject* pRet = it->second;
		if (pRet)
			pRet->retain();
		return pRet;
	}

	/*
	 *	新增一个数据,并增加数据引用计数
	 *	如果key存在,则将原有数据释放
	 */
	inline void add(const T &_key, YCTObject* obj, bool bAutoRetain = true)
	{
		if (bAutoRetain && obj)
			obj->retain();

		YCTObject* pOldObj = NULL;
		auto it = _map.find(_key);
		if (it != _map.end())
		{
			pOldObj = it->second;
		}

		_map[_key] = obj;

		if (pOldObj) pOldObj->release();
	}

	/*
	 *	根据key删除一个数据
	 *	如果key存在,则对应数据引用计数-1
	 */
	inline void remove(const T &_key)
	{
		auto it = _map.find(_key);
		if (it != _map.end())
		{
			YCTObject* obj = it->second;
			_map.erase(it);
			if (obj) obj->release();
		}
	}

	/*
	 *	获取容器起始位置的迭代器
	 */
	inline ConstIterator begin() const noexcept
	{
		return _map.begin();
	}
 
	/*
	 *	获取容器末尾位置的迭代器
	 */
	inline ConstIterator end() const noexcept
	{
		return _map.end();
	}

	inline ConstIterator find(const T& key) const
	{
		return _map.find(key);
	}

	/*
	 *	清空容器
	 *	容器内所有数据引用计数-1
	 *	不加 noexcept：会调用 it->second->release()，若子类 release() 抛异常会 std::terminate
	 */
	inline void clear()
	{
		ConstIterator it = _map.begin();
		for(; it != _map.end(); it++)
		{
			it->second->release();
		}
		_map.clear();
	}

	/*
	 *	释放容器对象
	 *	如果容器引用计数为1,则清空所有数据
	 */
	virtual void release()
	{
		if (m_uRefs == 0)
			return;

		try
		{
			m_uRefs--;
			if (m_uRefs == 0)
			{
				clear();
				delete this;
			}
		}
		catch (...)
		{

		}
	}

protected:
	YCTHashMap() {}
	virtual ~YCTHashMap() {}

	_YCTHashMapType	_map;
};

//////////////////////////////////////////////////////////////////////////
//YCTQueue
class YCTQueue : public YCTObject
{
public:
	typedef std::deque<YCTObject*>::iterator Iterator;
	typedef std::deque<YCTObject*>::const_iterator ConstIterator;

	inline static YCTQueue* create()
	{
		YCTQueue* pRet = new YCTQueue();
		return pRet;
	}

	inline void pop() noexcept
	{
		_queue.pop_front();
	}

	inline void push(YCTObject* obj, bool bAutoRetain = true)
	{
		if (obj && bAutoRetain)
			obj->retain();

		_queue.emplace_back(obj);
	}

	inline YCTObject* front(bool bRetain = true)
	{
		if(_queue.empty())
			return NULL;

		YCTObject* obj = _queue.front();
		if(bRetain)
			obj->retain();

		return obj;
	}

	inline YCTObject* back(bool bRetain = true)
	{
		if(_queue.empty())
			return NULL;

		YCTObject* obj = _queue.back();
		if(bRetain)
			obj->retain();

		return obj;
	}

	inline uint32_t size() const noexcept { return (uint32_t)_queue.size(); }

	inline bool empty() const noexcept { return _queue.empty(); }

	void release()
	{
		if (m_uRefs == 0)
			return;

		try
		{
			m_uRefs--;
			if (m_uRefs == 0)
			{
				clear();
				delete this;
			}
		}
		catch (...)
		{

		}
	}

	inline void clear()
	{
		Iterator it = begin();
		for(; it != end(); it++)
		{
			(*it)->release();
		}
		_queue.clear();
	}

	/*
	 *	取得数组对象起始位置的迭代器
	 */
	inline Iterator begin() noexcept
	{
		return _queue.begin();
	}

	inline ConstIterator begin() const noexcept
	{
		return _queue.begin();
	}

	inline void swap(YCTQueue* right) noexcept
	{
		_queue.swap(right->_queue);
	}

	/*
	 *	取得数组对象末尾位置的迭代器
	 */
	inline Iterator end() noexcept
	{
		return _queue.end();
	}

	inline ConstIterator end() const noexcept
	{
		return _queue.end();
	}

protected:
	YCTQueue(){}
	virtual ~YCTQueue(){}

	std::deque<YCTObject*> _queue;
};

NS_YCT_END