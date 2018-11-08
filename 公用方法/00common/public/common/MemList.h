//////////////////////////////////////////////////////////////////////////
/*
	MemList.h
	该模板类实现各种数据类型的element信息的添加。

	author:jhb
*/
//////////////////////////////////////////////////////////////////////////

#ifndef _MEMLIST_H_
#define _MEMLIST_H_

#include <deque>
#include <list>
#include <vector>
#include "SafeLock.h"
#include "DerivedLock.h"

//#include "TypeDef.h"
//////////////////////////////////////////////////////////////////////////
					/* CMemList类声明与实现部分 */
//////////////////////////////////////////////////////////////////////////
template <class T>
class CMemList
{
	public:
		CMemList();
		virtual ~CMemList();
		void	AddTail(const T& element);
		void	AddHead(const T& element);
        T       GetHead(/*T& element, */bool bIsDel = true);
		T		GetHead(int& nErr, bool bIsDel = true);
		int		GetHead_PoinType(T& element, bool bIsDel = true);

		int		GetHead(T& element);
		void	GetSomeValues(std::deque<T>& Values, int nCount);
		int		GetAt(
					T& element, 
					int nPos = 0, 
					int nTop = 0, 
					bool bIsDel = true
					);//nTop = 0则遍历全队列查找，不为0则只查找前nTop个
		void	RemoveAt(int nPos = 0);
		void	Clear();
		int		GetCount();
		bool	IsEmpty();
		void	ClearError();
        //void    WriteErrorLog(const char *szLogFmt, ...);
        //void    GetMemUsage();
	private:
		std::deque <T>		m_elementArr;
		//CRITICAL_SECTION	m_cs;
		DerivedLock			m_cs;
        //char                m_szLog[1024+1];
        //uint64_t            m_mem;
        //uint64_t            m_vmem;
};

template<class T>
CMemList<T>::CMemList()
{
	//InitializeCriticalSection(&m_cs);
}

template<class T>
CMemList<T>::~CMemList()
{
	//DeleteCriticalSection(&m_cs);
}

template<class T>
void CMemList<T>::ClearError()
{
	Clear();
}

template<class T>
inline bool CMemList<T>::IsEmpty()
{
	bool bIsEmpty = false;

	SafeLock safelock(&m_cs); 

	try
	{
		//EnterCriticalSection(&m_cs);
		bIsEmpty = m_elementArr.empty();
	}
	catch (...)
	{
        //GetMemUsage();
        //WriteErrorLog("CMemList<T>::IsEmpty() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);

	return bIsEmpty;
}

template <class T>
inline void CMemList<T>::AddHead(const T& element)
{
	SafeLock safelock(&m_cs); 

	try
	{
		//EnterCriticalSection(&m_cs);
		m_elementArr.push_front(element);
	}
	catch (...)
	{
        //GetMemUsage();
        //WriteErrorLog("CMemList<T>::AddHead() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);
}

template <class T>
inline void CMemList<T>::AddTail(const T& element)
{
	SafeLock safelock(&m_cs); 

	try
	{
		//EnterCriticalSection(&m_cs);
		m_elementArr.push_back(element);
	}
	catch (...)
	{
        //GetMemUsage();
        //WriteErrorLog("CMemList<T>::AddTail() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);
}

template<class T>
void CMemList<T>::GetSomeValues(std::deque<T>& Values, int nCount)
{
	SafeLock safelock(&m_cs); 
	try
	{
		while (!m_elementArr.empty() && --nCount >= 0)
		{
			Values.push_back(m_elementArr.front());
			m_elementArr.pop_front();
		}	
	}
	catch (...)
	{
        //GetMemUsage();
        //WriteErrorLog("CMemList<T>::GetHead() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
}

template<class T>
int CMemList<T>::GetHead(T& element)
{
	int nRet = -1;

	SafeLock safelock(&m_cs); 

	try
	{
		if (!m_elementArr.empty())
		{
			element = m_elementArr.front();
			m_elementArr.pop_front();
			nRet = 0;
		}
		/*
        typename std::deque<T>::iterator it = m_elementArr.begin();
		if (it != m_elementArr.end())
		{
			element = *it;
			m_elementArr.erase(it);
			--m_nCount;
			nRet = 0;
		}
		*/
	}
	catch (...)
	{
        //GetMemUsage();
        //WriteErrorLog("CMemList<T>::GetHead() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	return nRet;
}

template<class T>
inline T CMemList<T>::GetHead(/*T& element, */bool bIsDel)
{
	T element;

	SafeLock safelock(&m_cs); 

	try
	{
		//EnterCriticalSection(&m_cs);
		if (!m_elementArr.empty())
		{
			element = m_elementArr.front();
			if (bIsDel)
			{
				m_elementArr.pop_front();
			}
		}	
	}
	catch (...)
	{
        //GetMemUsage();
        //WriteErrorLog("CMemList<T>::GetHead() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);
	return element;
}

template<class T>
inline T CMemList<T>::GetHead(int& nErr, bool bIsDel)
{
	nErr = -1;
	T element;
	SafeLock safelock(&m_cs); 
	try
	{
		//EnterCriticalSection(&m_cs);
		if (!m_elementArr.empty())
		{
			element = m_elementArr.front();
			if (bIsDel)
			{
				m_elementArr.pop_front();
			}
			nErr = 0;
		}	
	}
	catch (...)
	{
		//GetMemUsage();
		//WriteErrorLog("CMemList<T>::GetHead() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
	}
	//LeaveCriticalSection(&m_cs);
	return element;
}

template<class T>
inline int CMemList<T>::GetHead_PoinType(T& element, bool bIsDel)
{
	int nErr = -1;

	SafeLock safelock(&m_cs); 
	try
	{
		//EnterCriticalSection(&m_cs);
		if (!m_elementArr.empty())
		{
			element = m_elementArr.front();
			if (bIsDel)
			{
				m_elementArr.pop_front();
			}
			nErr = 0;
		}	
	}
	catch (...)
	{
		//GetMemUsage();
		//WriteErrorLog("CMemList<T>::GetHead() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
	}
	//LeaveCriticalSection(&m_cs);
	return nErr;
}

template<class T>
int CMemList<T>::GetAt(T& element, int nPos, int nTop, bool bIsDel)
{
	int nRet = -1;

	SafeLock safelock(&m_cs); 

	try
	{
		//EnterCriticalSection(&m_cs);
		if (!m_elementArr.empty())
		{
			if (nPos < 0 || nPos >= m_elementArr.size())//小于0返回失败//大于当前容量返回失败
			{
				nRet = -1;
				//LeaveCriticalSection(&m_cs);
				return nRet;
			}
			else if (0 == nPos)
			{
				element = m_elementArr.front();
				if (bIsDel)
				{
					m_elementArr.pop_front();
				}
				nRet = 0;
			}
			else
			{
                typename std::deque <T>::iterator it = m_elementArr.begin();
				int nCount = 0;
				for (it; it != m_elementArr.end(); ++it)
				{
					if (nPos == nCount)
					{
						element = *it;
						if (bIsDel)
						{
							m_elementArr.erase(it);
						}
						nRet = 0;
						break;
					}
					else if (nTop > 0 && nCount == nTop)
					{
						nRet = -1;
						break;
					}
					else 
					{
						++nCount;
						continue;
					}
				}
			}
		}
	}
	catch (...)
	{
        //GetMemUsage();
        //WriteErrorLog("CMemList<T>::GetAt() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);
	return nRet;
}

template<class T>
void CMemList<T>::RemoveAt(int nPos)
{
	SafeLock safelock(&m_cs); 

	try
	{
	//	EnterCriticalSection(&m_cs);
		if (!m_elementArr.empty())
		{
			if (nPos < 0 || nPos >= m_elementArr.size())//小于0返回失败//大于当前容量返回失败
			{
				//LeaveCriticalSection(&m_cs);
				return;
			}
			else if (0 == nPos)
			{
				m_elementArr.pop_front();
			}
			else
			{
                typename std::deque <T>::iterator it = m_elementArr.begin();
				int nCount = 0;
				for (it; it != m_elementArr.end(); ++it, ++nCount)
				{
					if (nPos == nCount)
					{
						m_elementArr.erase(it);
						break;
					}
				}
			}
		}
	}
	catch (...)
	{
        //GetMemUsage();
        //WriteErrorLog("CMemList<T>::RemoveAt() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}	
	//LeaveCriticalSection(&m_cs);
}


template<class T>
inline int CMemList<T>::GetCount()
{
	int nCount = 0;

	SafeLock safelock(&m_cs); 

	try
	{
		//EnterCriticalSection(&m_cs);
		nCount = m_elementArr.size();
	}
	catch (...)
	{
        //GetMemUsage();
        //WriteErrorLog("CMemList<T>::GetCount() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);

	return nCount;
}

template<class T>
inline void CMemList<T>::Clear()
{
	SafeLock safelock(&m_cs); 

	try
	{
		//EnterCriticalSection(&m_cs);
		m_elementArr.clear();
	}
	catch (...)
	{
        //GetMemUsage();
        //WriteErrorLog("CMemList<T>::Clear() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);
}
/*
template<class T>
void CMemList<T>::GetMemUsage()
{
    m_mem = 0;
    m_vmem = 0;
    get_memory_usage(&m_mem, &m_vmem);
    m_mem  = m_mem/(1024*1024);
    m_vmem = m_vmem/(1024*1024);
}
*/
/*
template<class T>
void CMemList<T>::WriteErrorLog(const char *szLogFmt, ...)
{
    va_list vl;
    int nMaxSize = sizeof(m_szLog);
    memset(m_szLog, 0, nMaxSize);
    //以Printf方式组织输出
    va_start(vl, szLogFmt);
    _vsntprintf(m_szLog, nMaxSize, szLogFmt, vl);
    va_end(vl);
    CLogMgr::GetInstance().WriteLog(m_szLog, "CMemListError.txt");
}
*/
#endif
