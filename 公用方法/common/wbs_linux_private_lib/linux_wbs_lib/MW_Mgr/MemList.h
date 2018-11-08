
#ifndef _MEMLIST_H_
#define _MEMLIST_H_


#include <deque>
#include "Global.h"
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
		
		void	AddTail(const T& element , bool &nErr);

		void	AddHead(const T& element);
		T		GetHead(/*T& element, */bool bIsDel = TRUE);
		T		GetHead(int& nErr, bool bIsDel = TRUE);
		int		GetHead(T& element);
		int		GetAt(
					T& element, 
					int nPos = 0, 
					int nTop = 0, 
					bool bIsDel = TRUE
					);//nTop = 0则遍历全队列查找，不为0则只查找前nTop个
		void	RemoveAt(int nPos = 0);
		void	Clear();
		int		GetCount();
		bool	IsEmpty();
		void	ClearError();
		void	WriteErrorLog(const char *szLogFmt, ...);
		void	GetMemUsage();
	private:
		std::deque <T>		m_elementArr;
		//CRITICAL_SECTION	m_cs;
		DerivedLock			m_cs;
		int					m_nCount;
		char				m_szLog[1024+1];
		uint64_t			m_mem;
		uint64_t			m_vmem; 
};

template<class T>
CMemList<T>::CMemList()
{
	//InitializeCriticalSection(&m_cs);
	m_nCount = 0;
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
	m_nCount = 0;
}

template<class T>
inline bool CMemList<T>::IsEmpty()
{
	SafeLock safelock(&m_cs); 

	bool bIsEmpty = false;
	try
	{
		//EnterCriticalSection(&m_cs);
		bIsEmpty = m_elementArr.empty();
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemList<T>::IsEmpty() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);		
		//g_bMemError = TRUE;
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
		++m_nCount;
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemList<T>::AddHead() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
	//	g_bMemError = TRUE;
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
		++m_nCount;
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemList<T>::AddTail() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
	//	g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
}
 
template <class T>
inline void CMemList<T>::AddTail(const T& element , bool &nErr)
{
	nErr = false;
	SafeLock safelock(&m_cs); 
	
	try
	{
		//EnterCriticalSection(&m_cs);
		m_elementArr.push_back(element);
		++m_nCount;
		nErr = true ;
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemList<T>::AddTail() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
	return  ;
}

template<class T>
int CMemList<T>::GetHead(T& element)
{
	SafeLock safelock(&m_cs); 
	int nRet = -1;
	try
	{
		typename std::deque<T>::iterator it = m_elementArr.begin();
		if (it != m_elementArr.end())
		{
			element = *it;
			m_elementArr.erase(it);
			--m_nCount;
			nRet = 0;
		}	
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemList<T>::GetHead() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
	//	g_bMemError = TRUE;
	}
	return nRet;
}

template<class T>
inline T CMemList<T>::GetHead(/*T& element, */bool bIsDel)
{
	SafeLock safelock(&m_cs); 

	T element;
	try
	{
		//EnterCriticalSection(&m_cs);
		if (!m_elementArr.empty())
		{
			element = m_elementArr.at(0);
			if (bIsDel)
			{
				m_elementArr.pop_front();
				--m_nCount;
			}
		}	
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemList<T>::GetHead() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
	return element;
}

template<class T>
inline T CMemList<T>::GetHead(int& nErr, bool bIsDel)
{
	SafeLock safelock(&m_cs); 
	nErr = -1;
	T element;
	try
	{
		//EnterCriticalSection(&m_cs);
		if (!m_elementArr.empty())
		{
			element = m_elementArr.at(0);
			if (bIsDel)
			{
				m_elementArr.pop_front();
				--m_nCount;
			}
			nErr = 0;
		}	
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemList<T>::GetHead() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
	return element;
}

template<class T>
int CMemList<T>::GetAt(T& element, int nPos, int nTop, bool bIsDel)
{
	SafeLock safelock(&m_cs); 

	int nRet = -1;
	try
	{
		//EnterCriticalSection(&m_cs);
		if (!m_elementArr.empty())
		{
			if (nPos < 0 || nPos >= m_nCount)//小于0返回失败//大于当前容量返回失败
			{
				nRet = -1;
				//LeaveCriticalSection(&m_cs);
				return nRet;
			}
			else if (0 == nPos)
			{
				element = m_elementArr.at(0);
				if (bIsDel)
				{
					m_elementArr.pop_front();
					--m_nCount;
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
							--m_nCount;
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
		GetMemUsage();
		WriteErrorLog("CMemList<T>::GetAt() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
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
			if (nPos < 0 || nPos >= m_nCount)//小于0返回失败//大于当前容量返回失败
			{
				//LeaveCriticalSection(&m_cs);
				return;
			}
			else if (0 == nPos)
			{
				m_elementArr.pop_front();
				--m_nCount;
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
						--m_nCount;
						break;
					}
				}
			}
		}
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemList<T>::RemoveAt() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}	
	//LeaveCriticalSection(&m_cs);
}


template<class T>
inline int CMemList<T>::GetCount()
{
	SafeLock safelock(&m_cs); 

	int nCount = 0;
	try
	{
		//EnterCriticalSection(&m_cs);
		nCount = m_elementArr.size();
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemList<T>::GetCount() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
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
		GetMemUsage();
		WriteErrorLog("CMemList<T>::Clear() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
}

template<class T>
void CMemList<T>::GetMemUsage()
{
	/*
	m_mem = 0;
	m_vmem = 0; 
	get_memory_usage(&m_mem, &m_vmem);
	m_mem  = m_mem/(1024*1024);
	m_vmem = m_vmem/(1024*1024);
	*/
}

template<class T>
void CMemList<T>::WriteErrorLog(const char *szLogFmt, ...)
{
	/*

	va_list	vl;
	int nMaxSize = sizeof(m_szLog);
	memset(m_szLog, 0, nMaxSize);
	//以Printf方式组织输出
	va_start(vl, szLogFmt);
	_vsntprintf(m_szLog, nMaxSize, szLogFmt, vl);
	va_end(vl);
	CLogMgr::GetInstance().WriteLog(m_szLog, "CMemListError.txt");
	*/
}

#endif

