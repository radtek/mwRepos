// MemPool.h: interface for the CMemPool class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __MEMPOOL_H__
#define __MEMPOOL_H__

#include <deque>
#include <list>
#include <vector>
#include "DerivedLock.h"
#include "SafeLock.h"
//#include "LockFreeQueue.h"

template <class T, int COUNT>
class CMemPool  
{
public:
	CMemPool();
	virtual ~CMemPool();
public:
	T*		GetMem();
	void	RecycleMem(T** pMem);
	void	RecycleMem(T* pMem);
	void	GetMemCount(int &nBusy, int& nIdle);
	void	ResetMemPoolSize(int nMinSize, int nMaxSize);
	void	ShrinkMemPool();
private:
	std::deque<T*> m_MemPool;
	DerivedLock m_cs;
	int m_nMinSize;
	int m_nMaxSize;
	int m_nCount;
};

template <class T, int COUNT>
CMemPool<T, COUNT>::CMemPool()
{
	T *lp_t = NULL;
	SafeLock safelock(&m_cs);
	m_nCount = 0;
	m_nMinSize = m_nMaxSize = COUNT;
    for (int i = 0; i < COUNT; ++i)
    {
        lp_t = new T;
        if (NULL != lp_t)
        {
            m_MemPool.push_back(lp_t);
			++m_nCount;
        }
    }
}

template <class T, int COUNT>
CMemPool<T, COUNT>::~CMemPool()
{
	
}

template <class T, int COUNT>
T* CMemPool<T, COUNT>::GetMem()
{
	T* lp_t = NULL;
	SafeLock safelock(&m_cs);
	if (!m_MemPool.empty())
	{
		lp_t = m_MemPool.front();
		m_MemPool.pop_front();
	}
	else
	{
		lp_t = new T;
		++m_nCount;
	}
	return lp_t;
}

template <class T, int COUNT>
void CMemPool<T, COUNT>::RecycleMem(T** pMem)
{
	if (NULL != *pMem)
	{
		{
			SafeLock safelock(&m_cs);
			m_MemPool.push_back(*pMem);
		}
		*pMem = NULL;
	}
}

template <class T, int COUNT>
void CMemPool<T, COUNT>::RecycleMem(T* pMem)
{
	if (NULL != pMem)
	{
		SafeLock safelock(&m_cs);
		m_MemPool.push_back(pMem);
	}
}

template <class T, int COUNT>
void CMemPool<T, COUNT>::ResetMemPoolSize(int nMinSize, int nMaxSize)
{
	SafeLock safelock(&m_cs);
	m_nMinSize = nMinSize;
	m_nMaxSize = nMaxSize;
}

template <class T, int COUNT>
void CMemPool<T, COUNT>::ShrinkMemPool()
{	
	T* lp_t = NULL;
	typename std::vector<T*> vTmp;
	//如果发现当前空闲的大于最大值则收缩，否则不收缩
	{
		SafeLock safelock(&m_cs);
		int nIdleSize = m_MemPool.size(); 
		while (--nIdleSize >= m_nMaxSize)
		{
			lp_t = m_MemPool.front();
			if (NULL != lp_t) vTmp.push_back(lp_t);//delete[] lp_t;
			m_MemPool.pop_front();
			--m_nCount;
		}
	}

	typename std::vector<T*>::iterator it = vTmp.begin();
	for (it; it != vTmp.end(); ++it)
	{
		delete[] *it;
	}
}

template <class T, int COUNT>
void CMemPool<T, COUNT>::GetMemCount(int &nBusy, int& nIdle)
{
	//SafeLock safelock(&m_cs);
	//nIdle  = m_MemPool.size();
	nBusy  = m_nCount - (nIdle = m_MemPool.size());
}

// template <class T, int COUNT>
// class CMemPool  
// {
// public:
// 	CMemPool();
// 	virtual ~CMemPool();
// public:
// 	T*		GetMem();
// 	void	RecycleMem(T** pMem);
// 	void	RecycleMem(T* pMem);
// 	void	GetMemCount(int &nBusy, int& nIdle);
// 	void	ResetMemPoolSize(int nMinSize, int nMaxSize);
// 	void	ShrinkMemPool();
// private:
// 	//std::deque<T*> m_MemPool;
// 	LockFreeList<T*> m_MemPool;
// 	DerivedLock m_cs;
// 	int m_nMinSize;
// 	int m_nMaxSize;
// 	int m_nCount;
// };
// 
// template <class T, int COUNT>
// CMemPool<T, COUNT>::CMemPool()
// {
// 	T *lp_t = NULL;
// 	//SafeLock safelock(&m_cs);
// 	m_nCount = 0;
// 	m_nMinSize = m_nMaxSize = COUNT;
//     for (int i = 0; i < COUNT; ++i)
//     {
// 		list_node<T*> *pNode = new list_node<T*>;
// 		if (NULL != pNode)
// 		{
// 			lp_t = new T;
// 			if (NULL != lp_t)
// 			{
// 				pNode->val = lp_t;
// 				pNode->_next = NULL;
// 				m_MemPool.push(pNode);
// 				InterlockedIncrement((long*)&m_nCount);
// 			}
// 		}    
//     }
// }
// 
// template <class T, int COUNT>
// CMemPool<T, COUNT>::~CMemPool()
// {
// 	
// }
// 
// template <class T, int COUNT>
// T* CMemPool<T, COUNT>::GetMem()
// {
// 	T* lp_t = NULL;
// 	list_node<T*> *pNode = m_MemPool.pop();
// 	if (NULL == pNode)
// 	{
// 		lp_t = new T;
// 		InterlockedIncrement((long*)&m_nCount);
// 	}
// 	else
// 	{
// 		lp_t = pNode->val;
// 		delete[] pNode;
// 	}
// 	return lp_t;
// }
// 
// template <class T, int COUNT>
// void CMemPool<T, COUNT>::RecycleMem(T** pMem)
// {
// 	if (NULL != *pMem)
// 	{
// 		list_node<T*> *pNode = new list_node<T*>;
// 		if (NULL != pNode)
// 		{
// 			pNode->val = *pMem;
// 			pNode->_next = NULL;
// 			m_MemPool.push(pNode);
// 		}    
// 		*pMem = NULL;
// 	}
// }
// 
// template <class T, int COUNT>
// void CMemPool<T, COUNT>::RecycleMem(T* pMem)
// {
// 	if (NULL != pMem)
// 	{
// 		list_node<T*> pNode = new list_node<T*>;
// 		if (NULL != pNode)
// 		{
// 			pNode->val = pMem;
// 			pNode->_next = NULL;
// 			m_MemPool.push(pNode);
// 		}
// 	}
// }
// 
// template <class T, int COUNT>
// void CMemPool<T, COUNT>::ResetMemPoolSize(int nMinSize, int nMaxSize)
// {
// 	SafeLock safelock(&m_cs);
// 	m_nMinSize = nMinSize;
// 	m_nMaxSize = nMaxSize;
// }
// 
// template <class T, int COUNT>
// void CMemPool<T, COUNT>::ShrinkMemPool()
// {	
// // 	T* lp_t = NULL;
// // 	std::vector<T*> vTmp;
// // 	//如果发现当前空闲的大于最大值则收缩，否则不收缩
// // 	{
// // 		SafeLock safelock(&m_cs);
// // 		int nIdleSize = m_MemPool.size(); 
// // 		while (--nIdleSize >= m_nMaxSize)
// // 		{
// // 			lp_t = m_MemPool.front();
// // 			if (NULL != lp_t) vTmp.push_back(lp_t);//delete[] lp_t;
// // 			m_MemPool.pop_front();
// // 			--m_nCount;
// // 		}
// // 	}
// // 
// // 	std::vector<T*>::iterator it = vTmp.begin();
// // 	for (it; it != vTmp.end(); ++it)
// // 	{
// // 		delete[] *it;
// // 	}
// }
// 
// template <class T, int COUNT>
// void CMemPool<T, COUNT>::GetMemCount(int &nBusy, int& nIdle)
// {
// 	//SafeLock safelock(&m_cs);
// 	//nIdle  = m_MemPool.size();
// 	nBusy  = m_nCount - (nIdle = m_MemPool.size());
// }

#endif

