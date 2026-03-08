/*!
 * \file ObjectPool.hpp
 * \project	yubaoCppTrader
 *
 * \author yubao
 * \date 2026/03/08
 * 
 * \brief 提供了一个通用型对象池（Object Pool）模板类，
 * 基于 Boost 库的 boost::pool 内存池核心，高效管理 T 类型对象的内存分配 / 释放
 */
#pragma once
#include <boost/pool/pool.hpp>

template <typename T>
class ObjectPool
{
public:
    ObjectPool() :_pool(sizeof(T)) {}
    virtual ~ObjectPool() {}

	T* construct()
	{
		void * mem = _pool.malloc();
		if (!mem)
			return nullptr;

		T* pobj = new(mem) T();
		return pobj;
	}

	void destroy(T* pobj)
	{
		pobj->~T();
		_pool.free(pobj);
	}

	//手动释放未使用的内存
	void release()
	{
		_pool.release_memory();
	}

private:
    boost::pool<> _pool;
};