// MemPool.h: interface for the CMemPool class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __MEMPOOL_H__
#define __MEMPOOL_H__

#include <time.h>
#include <deque>
#include <map>
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
	int		RecycleZombieMem(int nSec);
private:
	//空闲池
	std::deque<T*> m_MemPool_Idle;
	//正在使用的池
	std::map<T*, time_t> m_MemPool_InUse;
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
            m_MemPool_Idle.push_back(lp_t);
			++m_nCount;
        }
    }
}

template <class T, int COUNT>
CMemPool<T, COUNT>::~CMemPool()
{
#ifdef _DEBUG
	//暂时先这样，要不调试一大把的泄露输出，本代码不上传nomi_change
	SafeLock safelock(&m_cs);
	for (std::deque<T*>::iterator deIter = m_MemPool_Idle.begin(); deIter != m_MemPool_Idle.end(); ++deIter)
	{
		delete *deIter;
		*deIter = NULL;
	}

	for (std::map<T*, time_t>::iterator mapIter = m_MemPool_InUse.begin(); mapIter != m_MemPool_InUse.end(); )
	{
		delete mapIter->first;
		m_MemPool_InUse.erase(mapIter++);
	}
#endif
}

template <class T, int COUNT>
T* CMemPool<T, COUNT>::GetMem()
{
	T* lp_t = NULL;
	SafeLock safelock(&m_cs);
	if (!m_MemPool_Idle.empty())
	{
		lp_t = m_MemPool_Idle.front();
		m_MemPool_Idle.pop_front();
	}
	else
	{
		lp_t = new T;
		if (lp_t)
		{
			++m_nCount;
		}
	}
	if (lp_t)
	{
		m_MemPool_InUse.insert(std::make_pair(lp_t, time(NULL)));
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
			m_MemPool_Idle.push_back(*pMem);
			m_MemPool_InUse.erase(*pMem);
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
		m_MemPool_Idle.push_back(pMem);
		m_MemPool_InUse.erase(pMem);
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
	//如果发现当前空闲的大于最小值则收缩，否则不收缩
	{
		SafeLock safelock(&m_cs);
		int nIdleSize = m_MemPool_Idle.size(); 
		while (--nIdleSize >= m_nMinSize)
		{
			lp_t = m_MemPool_Idle.front();
			if (NULL != lp_t) vTmp.push_back(lp_t);//delete[] lp_t;
			m_MemPool_Idle.pop_front();
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
int CMemPool<T, COUNT>::RecycleZombieMem(int nSec)
{	
	int nZombieMemCnt = 0;
	SafeLock safelock(&m_cs);
	typename std::map<T*, time_t>::iterator it = m_MemPool_InUse.begin();
	for (it; it != m_MemPool_InUse.end(); )
	{
		if (time(NULL) - it->second > nSec)
		{
			if (NULL != it->first)
			{
				m_MemPool_Idle.push_back(it->first);
			}
			m_MemPool_InUse.erase(it++);
			++nZombieMemCnt;
		}
		else
		{
			++it;
		}
	}
	return nZombieMemCnt;
}

template <class T, int COUNT>
void CMemPool<T, COUNT>::GetMemCount(int &nBusy, int& nIdle)
{
	SafeLock safelock(&m_cs);
	nBusy = m_MemPool_InUse.size();
	nIdle = m_MemPool_Idle.size();
	//nBusy  = m_nCount - (nIdle = m_MemPool_Idle.size());
}


//////////////////////////////////////////////////////////////////////////
//直接new delete,不预分配
template <class T, int COUNT>
class CMemPoolEx  
{
public:
	CMemPoolEx();
	virtual ~CMemPoolEx();
public:
	T*		GetMem();
	void	RecycleMem(T** pMem);
	void	RecycleMem(T* pMem);
	void	GetMemCount(int &nBusy, int& nIdle);
	void	ResetMemPoolSize(int nMinSize, int nMaxSize);
	void	ShrinkMemPool();
	int		RecycleZombieMem(int nSec);
private:
	//空闲池
	std::deque<T*> m_MemPool_Idle;
	//正在使用的池
	std::map<T*, time_t> m_MemPool_InUse;
	DerivedLock m_cs;
	int m_nMinSize;
	int m_nMaxSize;
	int m_nCount;
};

template <class T, int COUNT>
CMemPoolEx<T, COUNT>::CMemPoolEx()
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
            m_MemPool_Idle.push_back(lp_t);
			++m_nCount;
        }
    }
}

template <class T, int COUNT>
CMemPoolEx<T, COUNT>::~CMemPoolEx()
{
	
}

template <class T, int COUNT>
T* CMemPoolEx<T, COUNT>::GetMem()
{
	T* lp_t = NULL;

	SafeLock safelock(&m_cs);

	if (!m_MemPool_Idle.empty())
	{
		lp_t = m_MemPool_Idle.front();
		m_MemPool_Idle.pop_front();
	}
	else
	{
		lp_t = new T;
		if (NULL != lp_t)
		{
			++m_nCount;
		}
	}
	if (NULL != lp_t)
	{
		m_MemPool_InUse.insert(std::make_pair(lp_t, time(NULL)));
	}

	return lp_t;
}

template <class T, int COUNT>
void CMemPoolEx<T, COUNT>::RecycleMem(T** pMem)
{
	if (NULL != *pMem)
	{
		SafeLock safelock(&m_cs);

		m_MemPool_Idle.push_back(*pMem);
		m_MemPool_InUse.erase(*pMem);
		*pMem = NULL;
	}
}

template <class T, int COUNT>
void CMemPoolEx<T, COUNT>::RecycleMem(T* pMem)
{
	if (NULL != pMem)
	{
		SafeLock safelock(&m_cs);
		m_MemPool_Idle.push_back(pMem);
		m_MemPool_InUse.erase(pMem);
	}
}

template <class T, int COUNT>
void CMemPoolEx<T, COUNT>::ResetMemPoolSize(int nMinSize, int nMaxSize)
{
	SafeLock safelock(&m_cs);
	m_nMinSize = nMinSize;
	m_nMaxSize = nMaxSize;
}

template <class T, int COUNT>
void CMemPoolEx<T, COUNT>::ShrinkMemPool()
{	
	T* lp_t = NULL;
	typename std::vector<T*> vTmp;
	//如果发现当前空闲的大于最小值则收缩，否则不收缩
	{
		SafeLock safelock(&m_cs);
		int nIdleSize = m_MemPool_Idle.size(); 
		while (--nIdleSize >= m_nMinSize)
		{
			lp_t = m_MemPool_Idle.front();
			if (NULL != lp_t) vTmp.push_back(lp_t);//delete[] lp_t;
			m_MemPool_Idle.pop_front();
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
int CMemPoolEx<T, COUNT>::RecycleZombieMem(int nSec)
{	
	return 0;

	int nZombieMemCnt = 0;
	SafeLock safelock(&m_cs);
	typename std::map<T*, time_t>::iterator it = m_MemPool_InUse.begin();
	for (it; it != m_MemPool_InUse.end(); )
	{
		if (time(NULL) - it->second > nSec)
		{
			if (NULL != it->first)
			{
				m_MemPool_Idle.push_back(it->first);
			}
			m_MemPool_InUse.erase(it++);
			++nZombieMemCnt;
		}
		else
		{
			++it;
		}
	}
	return nZombieMemCnt;
}

template <class T, int COUNT>
void CMemPoolEx<T, COUNT>::GetMemCount(int &nBusy, int& nIdle)
{
	SafeLock safelock(&m_cs);
	nBusy = m_MemPool_InUse.size();
	nIdle = m_MemPool_Idle.size();
	//nBusy  = m_nCount - (nIdle = m_MemPool_Idle.size());
}

//////////////////////////////////////////////////////////////////////////
//直接new不delete,超过设定的最小值时再delete,不预分配
template <class T, int COUNT>
class CMemPoolExEx  
{
public:
	CMemPoolExEx();
	virtual ~CMemPoolExEx();
public:
	T*		GetMem();
	void	RecycleMem(T** pMem);
	void	RecycleMem(T* pMem);
	void	GetMemCount(int &nBusy, int& nIdle);
	void	ResetMemPoolSize(int nMinSize, int nMaxSize);
	void	ShrinkMemPool();
	int		RecycleZombieMem(int nSec);
private:
	//空闲池
	//std::deque<T*> m_MemPool_Idle;
	//正在使用的池
	std::map<T*, bool> m_MemPool_InUse;
	DerivedLock m_cs;
	int m_nMinSize;
	int m_nMaxSize;
	int m_nCount;
};

template <class T, int COUNT>
CMemPoolExEx<T, COUNT>::CMemPoolExEx()
{
	T *lp_t = NULL;
	SafeLock safelock(&m_cs);
	m_nCount = 0;
	m_nMinSize = m_nMaxSize = COUNT;
	/*
    for (int i = 0; i < COUNT; ++i)
    {
        lp_t = new T;
        if (NULL != lp_t)
        {
            m_MemPool_Idle.push_back(lp_t);
			++m_nCount;
        }
    }
	*/
}

template <class T, int COUNT>
CMemPoolExEx<T, COUNT>::~CMemPoolExEx()
{
	
}

template <class T, int COUNT>
T* CMemPoolExEx<T, COUNT>::GetMem()
{
	T* lp_t = new T;
	if (lp_t)
	{
		SafeLock safelock(&m_cs);

		++m_nCount;
		m_MemPool_InUse.insert(std::make_pair(lp_t, true));
	}

	return lp_t;
}

template <class T, int COUNT>
void CMemPoolExEx<T, COUNT>::RecycleMem(T** pMem)
{
	if (NULL != *pMem)
	{
		bool bFind = false;
		{	
			SafeLock safelock(&m_cs);
			if (m_MemPool_InUse.erase(*pMem) > 0)
			{
				--m_nCount;
				bFind = true;
			}
		}

		if (bFind) delete [] *pMem;
		*pMem = NULL;
	}
}

template <class T, int COUNT>
void CMemPoolExEx<T, COUNT>::RecycleMem(T* pMem)
{
	if (NULL != pMem)
	{
		bool bFind = false;
		{
			SafeLock safelock(&m_cs);
			if (m_MemPool_InUse.erase(pMem) > 0)
			{			
				--m_nCount;
				bFind = true;
			}
		}
		if (bFind) delete [] pMem;
	}
}

template <class T, int COUNT>
void CMemPoolExEx<T, COUNT>::ResetMemPoolSize(int nMinSize, int nMaxSize)
{
	SafeLock safelock(&m_cs);
	m_nMinSize = nMinSize;
	m_nMaxSize = nMaxSize;
}

template <class T, int COUNT>
void CMemPoolExEx<T, COUNT>::ShrinkMemPool()
{	
	return;
/*
	T* lp_t = NULL;
	typename std::vector<T*> vTmp;
	//如果发现当前空闲的大于最小值则收缩，否则不收缩
	{
		SafeLock safelock(&m_cs);
		int nIdleSize = m_MemPool_Idle.size(); 
		int nMaxSize = m_nMinSize/2;
		while (--nIdleSize >= nMaxSize)
		{
			lp_t = m_MemPool_Idle.front();
			if (NULL != lp_t) vTmp.push_back(lp_t);//delete[] lp_t;
			m_MemPool_Idle.pop_front();
			--m_nCount;
		}
	}

	typename std::vector<T*>::iterator it = vTmp.begin();
	for (it; it != vTmp.end(); ++it)
	{
		delete[] *it;
	}
	*/
}

template <class T, int COUNT>
int CMemPoolExEx<T, COUNT>::RecycleZombieMem(int nSec)
{	
	return 0;
/*
	int nZombieMemCnt = 0;
	SafeLock safelock(&m_cs);
	typename std::map<T*, time_t>::iterator it = m_MemPool_InUse.begin();
	for (it; it != m_MemPool_InUse.end(); )
	{
		if (time(NULL) - it->second > nSec)
		{
			if (NULL != it->first)
			{
				m_MemPool_Idle.push_back(it->first);
			}
			m_MemPool_InUse.erase(it++);
			++nZombieMemCnt;
		}
		else
		{
			++it;
		}
	}
	return nZombieMemCnt;
*/
}

template <class T, int COUNT>
void CMemPoolExEx<T, COUNT>::GetMemCount(int &nBusy, int& nIdle)
{
	nBusy = m_nCount;
	nIdle = 0;
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
// 	//std::deque<T*> m_MemPool_Idle;
// 	LockFreeList<T*> m_MemPool_Idle;
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
// 				m_MemPool_Idle.push(pNode);
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
// 	list_node<T*> *pNode = m_MemPool_Idle.pop();
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
// 			m_MemPool_Idle.push(pNode);
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
// 			m_MemPool_Idle.push(pNode);
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
// // 		int nIdleSize = m_MemPool_Idle.size(); 
// // 		while (--nIdleSize >= m_nMaxSize)
// // 		{
// // 			lp_t = m_MemPool_Idle.front();
// // 			if (NULL != lp_t) vTmp.push_back(lp_t);//delete[] lp_t;
// // 			m_MemPool_Idle.pop_front();
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
// 	//nIdle  = m_MemPool_Idle.size();
// 	nBusy  = m_nCount - (nIdle = m_MemPool_Idle.size());
// }

#endif

