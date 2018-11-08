//////////////////////////////////////////////////////////////////////////
/*
	MemListEx.h
	该模板类实现各种数据类型的element信息的添加。

	author:jhb
*/
//////////////////////////////////////////////////////////////////////////

#ifndef _MEMLISTEX_H_
#define _MEMLISTEX_H_


#include <deque>
#include <map>
#include <vector>

#include "Global.h"
//zhubo 2017-4-8
//#include "process_stat.h"
#include "SafeLock.h"
#include "DerivedLock.h"
#include "LogMgr.h"

using namespace std;

//#include "TypeDef.h"
//////////////////////////////////////////////////////////////////////////
					/* CMemListEx类声明与实现部分 */
//////////////////////////////////////////////////////////////////////////



template <class T>
class CMemListEx
{
	public:
		CMemListEx();
		virtual ~CMemListEx();

		//************************************************************************/
		// Description:以nKey为主键将element加入映射表尾部
		// Returns:   void
		// Parameter: [IN]int nKey 主键 [IN]const T& element 键值
		//************************************************************************/
		void	AddTail(int nKey, const T& element, const char *szpInoke);

		//************************************************************************/
		// Description:以nKey为主键将element加入映射表头部
		// Returns:   void
		// Parameter: [IN]int nKey 主键 [IN]const T& element 键值
		//************************************************************************/
		void	AddHead(int nKey, const T& element);

		//************************************************************************/
		// Description:取nKey主键下的第一个元素，bIsDel为真删除该元素，为假不删除
		// Returns:   T  nKey下的头元素
		// Parameter: [IN]int nKey 主键 [IN]bool bIsDel 删除标志
		//************************************************************************/
		T		GetHead(int& nErr, int nKey, bool bIsDel = TRUE);

		//************************************************************************/
		// Description:取映射表的第一个元素，bIsDel为真删除该元素，为假不删除
		// Returns:   T  头元素
		// Parameter: [IN]int nKey 主键 [IN]bool bIsDel 删除标志
		//************************************************************************/
		T		GetHead(int& nErr, bool bIsDel = TRUE);

		//************************************************************************/
		// Description:取映射表中按主键排在最后结点的头元素
		// Returns:   T  头元素
		// Parameter: [IN]int& nErr 返回码 [IN]bool bIsDel 删除标志
		//************************************************************************/
		T		GetTail(int& nErr, bool bIsDel = true);

		//************************************************************************/
		// Description:取nKey主键下第一个元素，取到则返回0，没取到返回-1
		// Returns:   int  0:取到第一个元素 -1:失败
		// Parameter: [IN]int nKey 主键 [OUT]T& element 返回取到的值
		//************************************************************************/
		int		GetHeadEx(int nKey, T& element);

		//************************************************************************/
		// Description:取nKey的下一个主键，若映射表为空返回-1，若已到了映射表尾返回首key
		// Returns:   int  -1:没有取到下个主键  非0:下一个Key
		// Parameter: [IN]int nKey  当前Key
		//************************************************************************/
		int		GetNextKey(int nKey);

		//************************************************************************/
		// Description:从队首开始依次往下取
		// Returns:   int  -1:没有取到下个主键  非0:下一个Key
		// Parameter: [IN]int nKey  当前Key
		//************************************************************************/
		int		GetNextKeyEx(/*int nKey*/);

		//************************************************************************/
		// Description:取nkey主键的data，并返回下一个nkey, 取到返回data，没取到返回-1
		// Returns:   int  -1:没有取到  非0:下一个Key
		// Parameter: [IN]int nKey  当前Key [OUT]T& element  nKey对应的键值
		//************************************************************************/
		int		GetNextKeyData(int nKey, T& element);

		//************************************************************************/
		// Description:删除nkey下第npos个元素
		// Returns:   void
		// Parameter: [IN]int nKey  当前Key [IN]int nPos 元素位置
		//************************************************************************/
		void	RemoveAt(int nKey, int nPos = 0);

		//************************************************************************/
		// Description:取nkey主键下共有多少data
		// Returns:   int 元素个数
		// Parameter: [IN]int nKey  当前Key 
		//************************************************************************/
		int		GetCount(int nKey);

		//************************************************************************/
		// Description:取映射表总元素数
		// Returns:   int 元素个数
		// Parameter: void
		//************************************************************************/
		int		GetCount();

		//************************************************************************/
		// Description:nkey下有没有元素
		// Returns:   bool 
		// Parameter: [IN]int nKey  当前Key 
		//************************************************************************/
		bool	IsEmpty(int nKey);

		//************************************************************************/
		// Description:映射表是否为空
		// Returns:   bool 
		// Parameter: void 
		//************************************************************************/
		bool	IsEmpty();

		//************************************************************************/
		// Description:清除该主键下所有元素
		// Returns:   void 
		// Parameter: [IN]int nKey  当前Key 
		//************************************************************************/
		void	Clear(int nKey);

		//************************************************************************/
		// Description:清空映射表
		// Returns:   void 
		// Parameter: void
		//************************************************************************/
		void	Clear();

		//************************************************************************/
		// Description:查找nkey是否存在
		// Returns:   bool 
		// Parameter: [IN]int nKey  当前Key
		//************************************************************************/
		bool	Find(int nKey);

		//************************************************************************/
		// Description:返回主键字符串
		// Returns:   void 
		// Parameter: [OUT]char *szKey
		//************************************************************************/
		void	GetKeyStr(char *szKey);

		//************************************************************************/
		// Description:取当前nkey下npos位置上的值，成功返回0，不成功返回-1
		// Returns:   int 0:成功  -1:失败
		// Parameter: [IN]int nKey 当前Key [OUT]T& element 当前Key的键值 [IN]int nPos 元素位置
		//************************************************************************/
		int		GetKeyData(int nKey, T& element, int nPos=0);

		//************************************************************************/
		// Description:删除nkey下npos位置上的值，成功返回0，不成功返回-1 
		// Returns:   int 0:成功  -1:失败
		// Parameter: [IN]int nKey 当前Key [IN]const T element 当前Key的键值 [IN]int nPos 元素位置
		//************************************************************************/
		int		DeleteKeyData(int nKey, const T& element, int nPos=0);

		//************************************************************************/
		// Description:更新nkey下npos位置上的值，成功返回0，不成功返回-1
		// Returns:   int 0:成功  -1:失败
		// Parameter: [IN]int nKey 当前Key [IN]const T element 当前Key的键值 [IN]int nPos 元素位置
		//************************************************************************/
		int		UpdateKeyData(int nKey, const T& element, int nPos=0);

		void	ClearError();

		void	WriteErrorLog(const char *szLogFmt, ...);

		void	GetMemUsage();
	public:
		typedef std::multimap<int, T> 			MULTIMAP_T;
		MULTIMAP_T				m_elementArr;
		//CRITICAL_SECTION		m_cs;
		DerivedLock				m_cs;
		char					m_szLog[1024+1];
		uint64_t				m_mem;
		uint64_t				m_vmem; 
};

template<class T>
CMemListEx<T>::CMemListEx()
{
	//InitializeCriticalSection(&m_cs);
}

template<class T>
CMemListEx<T>::~CMemListEx()
{
	//DeleteCriticalSection(&m_cs);
}

template<class T>
void CMemListEx<T>::ClearError()
{
	Clear();
}

template<class T>
inline bool CMemListEx<T>::IsEmpty()
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
		WriteErrorLog("CMemListEx<T>::IsEmpty() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);

	return bIsEmpty;
}

template<class T>
inline bool CMemListEx<T>::IsEmpty(int nKey)
{
	SafeLock safelock(&m_cs);

	bool bIsEmpty = false;
	try
	{
		//EnterCriticalSection(&m_cs);
		bIsEmpty = (m_elementArr.find(nKey) == m_elementArr.end()?true:false);
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemListEx<T>::IsEmpty(int nKey) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);

	return bIsEmpty;
}

template <class T>
inline bool	CMemListEx<T>::Find(int nKey)
{
	SafeLock safelock(&m_cs);

	bool bFind = false;
	try
	{
		//EnterCriticalSection(&m_cs);
		bFind = (m_elementArr.find(nKey) == m_elementArr.end()?false:true);
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemListEx<T>::Find(int nKey) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
	
	return bFind;
}

template <class T>
void CMemListEx<T>::GetKeyStr(char *szKey)
{
	SafeLock safelock(&m_cs);

	try
	{
		//EnterCriticalSection(&m_cs);
		typename MULTIMAP_T::iterator it = m_elementArr.begin();
		int i		= 0;
		int nRet	= 0;
		for (it; it != m_elementArr.end(); ++it,++i)
		{
			if (i%10 == 0 && 0 != i)
			{
				nRet += sprintf(szKey+nRet, ",\r\n%d", it->first);
			}
			else
			{
				nRet += sprintf(szKey+nRet, ",%d", it->first);
			}
		}
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemListEx<T>::GetKeyStr(char *szKey) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
}

template <class T>
inline void CMemListEx<T>::AddHead(int nKey, const T& element)
{
	SafeLock safelock(&m_cs);

	try
	{
		//EnterCriticalSection(&m_cs);
		m_elementArr.insert(m_elementArr.find(nKey).begin(), std::make_pair(nKey, element));
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemListEx<T>::AddHead(int nKey, T element) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
}

template <class T>
inline void CMemListEx<T>::AddTail(int nKey, const T& element, const char *szpInoke)
{
	SafeLock safelock(&m_cs);

	try
	{
		//EnterCriticalSection(&m_cs);
		m_elementArr.insert(std::make_pair(nKey, element));
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemListEx<T>::AddTail(%d, T element) error! Mem:%I64d VMem:%I64d InvokeStr%s", nKey, m_mem, m_vmem, szpInoke);	
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
}

template<class T>
inline T CMemListEx<T>::GetHead(int& nErr, int nKey, bool bIsDel)
{
	SafeLock safelock(&m_cs);
	T element;
	nErr = -1;
	try
	{
		//EnterCriticalSection(&m_cs);
		typename std::multimap<int, T>::iterator it = m_elementArr.find(nKey);
		if (it != m_elementArr.end())
		{
			element = it->second;
			if (bIsDel)
			{
				m_elementArr.erase(it);
			}
			nErr = 0;
		}
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemListEx<T>::GetHead(int nKey, bool bIsDel) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
	return element;
}

template<class T>
inline T CMemListEx<T>::GetTail(int& nErr, bool bIsDel)
{
	SafeLock safelock(&m_cs);
	T element;
	nErr = -1;
	try
	{
		//EnterCriticalSection(&m_cs);
		if (m_elementArr.begin() != m_elementArr.end())
		{
			typename MULTIMAP_T::iterator it = m_elementArr.find(m_elementArr.rbegin()->first);
			element = it->second;
			if (bIsDel)
			{
				m_elementArr.erase(it);
			}
			nErr = 0;
		}
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemListEx<T>::GetTail(int nKey, bool bIsDel) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
	return element;
}

template<class T>
int	CMemListEx<T>::GetHeadEx(int nKey, T& element)
{
	SafeLock safelock(&m_cs);

	int nRet = -1;
	try
	{
		//EnterCriticalSection(&m_cs);
		typename std::multimap<int, T>::iterator it = m_elementArr.find(nKey);
		if (it != m_elementArr.end())
		{
			element = it->second;
			nRet = 0;
		}
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemListEx<T>::GetHeadEx(int nKey, T& element) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
	
	return nRet;
}

template<class T>
int CMemListEx<T>::GetNextKey(int nKey)
{
	SafeLock safelock(&m_cs);

	int nRetKey = -1;
	try
	{
		//EnterCriticalSection(&m_cs);
		if (!m_elementArr.empty())
		{
			typename std::multimap<int, T>::iterator it = m_elementArr.upper_bound(nKey);
			if (it != m_elementArr.end())
			{
				nRetKey = it->first;
			}
 			else
 			{
 				nRetKey = (m_elementArr.begin())->first;
 			}
		}
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemListEx<T>::GetNextKey(int nKey) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
	return nRetKey;
}

template<class T>
int CMemListEx<T>::GetNextKeyEx(/*int nKey*/)
{
	SafeLock safelock(&m_cs);
	
	int nRetKey = -1;
	try
	{
		//EnterCriticalSection(&m_cs);
		if (!m_elementArr.empty())
		{
// 			std::multimap<int, T>::iterator it = m_elementArr.find(nKey);
// 			if (it != m_elementArr.end())
// 			{
// 				nRetKey = it->first;
// 			}
// 			else
//			{
			nRetKey = (m_elementArr.begin())->first;
//			}
		}
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemListEx<T>::GetNextKey(int nKey) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
	return nRetKey;
}

template<class T>
int CMemListEx<T>::GetNextKeyData(int nKey, T& element)
{
	SafeLock safelock(&m_cs);

	int nRetKey = -1;
	try
	{
		//EnterCriticalSection(&m_cs);
		if (!m_elementArr.empty())
		{
			typename std::multimap<int, T>::iterator it = m_elementArr.find(nKey);
			if (it != m_elementArr.end())
			{
				element = it->second;
				it = m_elementArr.upper_bound(nKey);
				if (it != m_elementArr.end())
				{
					nRetKey = it->first;
				}
			}
		}
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemListEx<T>::GetNextKeyData(T& element) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
	return nRetKey;
}

template<class T>
int	CMemListEx<T>::UpdateKeyData(int nKey, const T& element, int nPos)
{
	SafeLock safelock(&m_cs);

	int nRet = -1;
	try
	{
		//EnterCriticalSection(&m_cs);
		typename std::multimap<int, T>::iterator it = m_elementArr.find(nKey);
		if (it != m_elementArr.end())
		{
			if (0 == nPos)
			{
				if (0 != memcmp(&element, &it->second, sizeof(element)))
				{
					it->second = element;
					nRet = 0;
				}
			}
			else
			{
				int nSize = m_elementArr.count(nKey);
				if (nPos > 0 && nPos <= nSize)
				{
					int nCount = 0;
					for (nCount = 0; nCount < nSize; ++it, ++nCount)
					{
						if (nPos == nCount)
						{
							if (0 != memcmp(&element, &it->second, sizeof(element)))
							{
								it->second = element;
								nRet = 0;
							}
							break;
						}
					}
				}
			}
		}
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemListEx<T>::UpdateKeyData(int nKey, const T element, int nPos) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
	return nRet;
}

template<class T>
int CMemListEx<T>::DeleteKeyData(int nKey, const T& element, int nPos)
{
	SafeLock safelock(&m_cs);

	int nRet = -1;
	try
	{
		//EnterCriticalSection(&m_cs);
		typename std::multimap<int, T>::iterator it = m_elementArr.find(nKey);
		if (it != m_elementArr.end())
		{
			if (0 == nPos)
			{
				if (0 == memcmp(&element, &it->second, sizeof(element)))
				{
					m_elementArr.erase(it);
					nRet = 0;
				}
			}
			else
			{
				int nSize = m_elementArr.count(nKey);
				if (nPos > 0 && nPos <= nSize)
				{
					int nCount = 0;
					for (nCount = 0; nCount < nSize; ++it, ++nCount)
					{
						if (nPos == nCount)
						{
							if (0 == memcmp(&element, &it->second, sizeof(element)))
							{
								m_elementArr.erase(it);
								nRet = 0;
							}
							break;
						}
					}
				}
			}
		}
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemListEx<T>::DeleteKeyData(int nKey, const T element, int nPos) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
	return nRet;
}

template<class T>
int	CMemListEx<T>::GetKeyData(int nKey, T& element, int nPos)
{
	SafeLock safelock(&m_cs);

	int nRet = -1;
	try
	{
		//EnterCriticalSection(&m_cs);
		if (!m_elementArr.empty())
		{
			typename std::multimap<int, T>::iterator it = m_elementArr.find(nKey);
			if (it != m_elementArr.end())
			{
				if (0 == nPos)
				{
					element = it->second;
					nRet = 0;
				}
				else
				{
					int nSize = m_elementArr.count(nKey);
					if (nPos > 0 && nPos <= nSize)
					{
						int nCount = 0;
						for (nCount = 0; nCount < nSize; ++it, ++nCount)
						{
							if (nPos == nCount)
							{
								element = it->second;
								nRet = 0;
								break;
							}
						}
					}
				}
			}
		}
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemListEx<T>::GetKeyData(int nKey, T& element, int nPos) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
	return nRet;
}

template<class T>
inline T CMemListEx<T>::GetHead(int& nErr, bool bIsDel)
{
	SafeLock safelock(&m_cs);
	T element;
	nErr = -1;
	try
	{
		//EnterCriticalSection(&m_cs);
		typename MULTIMAP_T::iterator it = m_elementArr.begin();
		if (it != m_elementArr.end())
		{
			element = it->second;
			if (bIsDel)
			{
				m_elementArr.erase(it);
			}
			nErr = 0;
		}
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemListEx<T>::GetHead(bool bIsDel) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);	
		//g_bMemError = TRUE;
	}	
	//LeaveCriticalSection(&m_cs);
	return element;
}

template<class T>
void CMemListEx<T>::RemoveAt(int nKey, int nPos)
{
	SafeLock safelock(&m_cs);

	try
	{
		//EnterCriticalSection(&m_cs);
		if (!m_elementArr.empty())
		{
			int nSize = m_elementArr.count(nKey);
			if (nPos < 0 || nPos >= nSize)
			{
				//LeaveCriticalSection(&m_cs);
				return;
			}
			else if (0 == nPos)
			{
				m_elementArr.erase(m_elementArr.begin());
			}
			else
			{
				typename MULTIMAP_T::iterator it = m_elementArr.find(nKey);
				int nCount = 0;
				for (nCount = 0; nCount < nSize; ++it, ++nCount)
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
		GetMemUsage();
		WriteErrorLog("CMemListEx<T>::RemoveAt(int nKey, int nPos) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);		
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
}


template<class T>
inline int CMemListEx<T>::GetCount(int nKey)
{
	SafeLock safelock(&m_cs);

	int nCount = 0;
	try
	{
		//EnterCriticalSection(&m_cs);
		nCount = m_elementArr.count(nKey);
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemListEx<T>::GetCount(int nKey) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);		
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);

	return nCount;
}

template<class T>
inline int CMemListEx<T>::GetCount()
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
		WriteErrorLog("CMemListEx<T>::GetCount() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);			
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);

	return nCount;
}

template<class T>
inline void CMemListEx<T>::Clear()
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
		WriteErrorLog("CMemListEx<T>::Clear() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);			
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
}

template<class T>
inline void CMemListEx<T>::Clear(int nKey)
{
	SafeLock safelock(&m_cs);

	try
	{
		//EnterCriticalSection(&m_cs);
		m_elementArr.erase(nKey);
	}
	catch (...)
	{
		GetMemUsage();
		WriteErrorLog("CMemListEx<T>::Clear(int nKey) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);			
		//g_bMemError = TRUE;
	}
	//LeaveCriticalSection(&m_cs);
}

template<class T>
void CMemListEx<T>::GetMemUsage()
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
void CMemListEx<T>::WriteErrorLog(const char *szLogFmt, ...)
{
	/*
	va_list	vl;
	int nMaxSize = sizeof(m_szLog);
	memset(m_szLog, 0, nMaxSize);
	//以Printf方式组织输出
	va_start(vl, szLogFmt);
	_vsntprintf(m_szLog, nMaxSize, szLogFmt, vl);
	va_end(vl);
	CLogMgr::GetInstance().WriteLog(m_szLog, "CMemListExError.txt");
	*/
}

#endif

