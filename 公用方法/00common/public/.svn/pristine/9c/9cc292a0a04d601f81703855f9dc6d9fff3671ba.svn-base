//////////////////////////////////////////////////////////////////////////
/*
	MemListEx.h
	该模板类实现各种数据类型的element信息的添加。

	author:jhb
*/
//////////////////////////////////////////////////////////////////////////

#ifndef _MULITMAP_H_
#define _MULITMAP_H_

#include <map>
#include "SafeLock.h"
#include "DerivedLock.h"

//#include "TypeDef.h"
//////////////////////////////////////////////////////////////////////////
                    /* CMullitMap类声明与实现部分 */
//////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE>
class CMullitMap
{
	public:
		CMullitMap();
		virtual ~CMullitMap();

		//************************************************************************/
		// Description:以nKey为主键将element加入映射表尾部
		// Returns:   void
		// Parameter: [IN]int nKey 主键 [IN]const T& element 键值
		//************************************************************************/
		void	AddTail(KEY nKey, const VALUE& element);

		//************************************************************************/
		// Description:以nKey为主键将element加入映射表头部
		// Returns:   void
		// Parameter: [IN]int nKey 主键 [IN]const T& element 键值
		//************************************************************************/
		void	AddHead(KEY nKey, const VALUE& element);

		//************************************************************************/
		// Description:取nKey主键下的第一个元素，bIsDel为真删除该元素，为假不删除
		// Returns:   VALUE  nKey下的头元素
		// Parameter: [IN]int nKey 主键 [IN]bool bIsDel 删除标志
		//************************************************************************/
		VALUE		GetHead(int& nErr, KEY nKey, bool bIsDel = true);

		//************************************************************************/
		// Description:取映射表的第一个元素，bIsDel为真删除该元素，为假不删除
		// Returns:   VALUE  头元素
		// Parameter: [IN]int nKey 主键 [IN]bool bIsDel 删除标志
		//************************************************************************/
		VALUE		GetHead(int& nErr, bool bIsDel = true);

		//************************************************************************/
		// Description:取nKey主键下第一个元素，取到则返回0，没取到返回-1
		// Returns:   int  0:取到第一个元素 -1:失败
		// Parameter: [IN]int nKey 主键 [OUT]VALUE& element 返回取到的值
		//************************************************************************/
		int		GetHeadEx(KEY nKey, VALUE& element);

		//************************************************************************/
		// Description:取nKey的下一个主键，若映射表为空返回-1，若已到了映射表尾返回首key
		// Returns:   int  -1:没有取到下个主键  非0:下一个Key
		// Parameter: [IN]int nKey  当前Key
		//************************************************************************/
		KEY		GetNextKey(KEY nKey);

		//************************************************************************/
		// Description:取nkey主键的data，并返回下一个nkey, 取到返回data，没取到返回-1
		// Returns:   int  -1:没有取到  非0:下一个Key
		// Parameter: [IN]int nKey  当前Key [OUT]VALUE& element  nKey对应的键值
		//************************************************************************/
		KEY		GetNextKeyData(KEY nKey, VALUE& element);

		//************************************************************************/
		// Description:删除nkey下第npos个元素
		// Returns:   void
		// Parameter: [IN]int nKey  当前Key [IN]int nPos 元素位置
		//************************************************************************/
		void	RemoveAt(KEY nKey, int nPos = 0);

		//************************************************************************/
		// Description:取nkey主键下共有多少data
		// Returns:   int 元素个数
		// Parameter: [IN]int nKey  当前Key 
		//************************************************************************/
		int		GetCount(KEY nKey);

		//************************************************************************/
		// Description:取映射表总元素数
		// Returns:   int 元素个数
		// Parameter: void
		//************************************************************************/
		int		GetCount();

		//************************************************************************/
		// Description:取映射表主键的数量
		// Returns:   int 主键个数
		// Parameter: void
		//************************************************************************/
		int		GetKeyCount();

		//************************************************************************/
		// Description:nkey下有没有元素
		// Returns:   bool 
		// Parameter: [IN]int nKey  当前Key 
		//************************************************************************/
		bool	IsEmpty(KEY nKey);

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
		void	Clear(KEY nKey);

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
		bool	Find(KEY nKey);

		//************************************************************************/
		// Description:返回主键字符串
		// Returns:   void 
		// Parameter: [OUT]char *szKey
		//************************************************************************/
		//void	GetKeyStr(char *szKey);

		//************************************************************************/
		// Description:取当前nkey下npos位置上的值，成功返回0，不成功返回-1
		// Returns:   int 0:成功  -1:失败
		// Parameter: [IN]int nKey 当前Key [OUT]VALUE& element 当前Key的键值 [IN]int nPos 元素位置
		//************************************************************************/
		//int		GetKeyData(int nKey, VALUE& element, int nPos=0);

		//************************************************************************/
		// Description:删除nkey下npos位置上的值，成功返回0，不成功返回-1 
		// Returns:   int 0:成功  -1:失败
		// Parameter: [IN]int nKey 当前Key [IN]const VALUE element 当前Key的键值 [IN]int nPos 元素位置
		//************************************************************************/
		//int		DeleteKeyData(int nKey, const VALUE& element, int nPos=0);

		//************************************************************************/
		// Description:更新nkey下npos位置上的值，成功返回0，不成功返回-1
		// Returns:   int 0:成功  -1:失败
		// Parameter: [IN]int nKey 当前Key [IN]const VALUE element 当前Key的键值 [IN]int nPos 元素位置
		//************************************************************************/
		//int		UpdateKeyData(int nKey, const VALUE& element, int nPos=0);

		void	ClearError();

        //void    WriteErrorLog(const char *szLogFmt, ...);

        //void    GetMemUsage();
	private:
		typedef std::multimap <KEY, VALUE> MULTIMAP_T;
		MULTIMAP_T				m_elementArr;
		//CRITICAL_SECTION		m_cs;
		DerivedLock				m_cs;
        //char                    m_szLog[1024+1];
        //uint64_t                m_mem;
        //uint64_t                m_vmem;
};

template <class KEY, class VALUE>
CMullitMap<KEY,VALUE>::CMullitMap()
{
	//InitializeCriticalSection(&m_cs);
}

template <class KEY, class VALUE>
CMullitMap<KEY,VALUE>::~CMullitMap()
{
	//DeleteCriticalSection(&m_cs);
}

template <class KEY, class VALUE>
void CMullitMap<KEY,VALUE>::ClearError()
{
	Clear();
}

template <class KEY, class VALUE>
inline bool CMullitMap<KEY,VALUE>::IsEmpty()
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
        //GetMemUsage();
        //WriteErrorLog("CMullitMap<KEY,VALUE>::IsEmpty() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);

	return bIsEmpty;
}

template <class KEY, class VALUE>
inline bool CMullitMap<KEY,VALUE>::IsEmpty(KEY nKey)
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
        //GetMemUsage();
        //WriteErrorLog("CMullitMap<KEY,VALUE>::IsEmpty(int nKey) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);

	return bIsEmpty;
}

template <class KEY, class VALUE>
inline bool	CMullitMap<KEY,VALUE>::Find(KEY nKey)
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
        //GetMemUsage();
        //WriteErrorLog("CMullitMap<KEY,VALUE>::Find(int nKey) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);
	
	return bFind;
}

template <class KEY, class VALUE>
int CMullitMap<KEY,VALUE>::GetKeyCount()
{
	SafeLock safelock(&m_cs);
	
	int nCount = 0;

	try
	{
		//EnterCriticalSection(&m_cs);
        typename MULTIMAP_T::iterator it = m_elementArr.begin();
		for (it; it != m_elementArr.end(); ++it)
		{
			++nCount;
		}
	}
	catch (...)
	{
        //GetMemUsage();
        //WriteErrorLog("CMullitMap<KEY,VALUE>::GetKeyStr(char *szKey) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);

	return nCount;
}

// template <class VALUE>
// void CMullitMap<KEY,VALUE>::GetKeyStr(char *szKey)
// {
// 	SafeLock safelock(&m_cs);
// 
// 	try
// 	{
// 		//EnterCriticalSection(&m_cs);
//         typename MULTIMAP_T::iterator it = m_elementArr.begin();
// 		int i		= 0;
// 		int nRet	= 0;
// 		for (it; it != m_elementArr.end(); ++it,++i)
// 		{
// 			if (i%10 == 0 && 0 != i)
// 			{
// 				nRet += sprintf(szKey+nRet, ",\r\n%d", it->first);
// 			}
// 			else
// 			{
// 				nRet += sprintf(szKey+nRet, ",%d", it->first);
// 			}
// 		}
// 	}
// 	catch (...)
// 	{
//         //GetMemUsage();
//         //WriteErrorLog("CMullitMap<KEY,VALUE>::GetKeyStr(char *szKey) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
// 	}
// 	//LeaveCriticalSection(&m_cs);
// }

template <class KEY, class VALUE>
inline void CMullitMap<KEY,VALUE>::AddHead(KEY nKey, const VALUE& element)
{
	SafeLock safelock(&m_cs);

	try
	{
		//EnterCriticalSection(&m_cs);
		m_elementArr.insert(m_elementArr.find(nKey).begin(), std::make_pair(nKey, element));
	}
	catch (...)
	{
        //GetMemUsage();
        //WriteErrorLog("CMullitMap<KEY,VALUE>::AddHead(int nKey, VALUE element) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);
}

template <class KEY, class VALUE>
inline void CMullitMap<KEY,VALUE>::AddTail(KEY nKey, const VALUE& element)
{
	SafeLock safelock(&m_cs);

	try
	{
		//EnterCriticalSection(&m_cs);
		m_elementArr.insert(std::make_pair(nKey, element));
	}
	catch (...)
	{
        //GetMemUsage();
        //WriteErrorLog("CMullitMap<KEY,VALUE>::AddTail(int nKey, VALUE element) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);
}

template <class KEY, class VALUE>
inline VALUE CMullitMap<KEY,VALUE>::GetHead(int& nErr, KEY nKey, bool bIsDel)
{
	SafeLock safelock(&m_cs);
	VALUE element;
	nErr = -1;
	try
	{
		//EnterCriticalSection(&m_cs);
        typename MULTIMAP_T::iterator it = m_elementArr.find(nKey);
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
        //GetMemUsage();
        //WriteErrorLog("CMullitMap<KEY,VALUE>::GetHead(int nKey, bool bIsDel) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);
	return element;
}

template <class KEY, class VALUE>
int	CMullitMap<KEY,VALUE>::GetHeadEx(KEY nKey, VALUE& element)
{
	SafeLock safelock(&m_cs);

	int nRet = -1;
	try
	{
		//EnterCriticalSection(&m_cs);
        typename MULTIMAP_T::iterator it = m_elementArr.find(nKey);
		if (it != m_elementArr.end())
		{
			element = it->second;
			nRet = 0;
		}
	}
	catch (...)
	{
        //GetMemUsage();
        //WriteErrorLog("CMullitMap<KEY,VALUE>::GetHeadEx(int nKey, VALUE& element) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);
	
	return nRet;
}

template <class KEY, class VALUE>
KEY CMullitMap<KEY,VALUE>::GetNextKey(KEY nKey)
{
	SafeLock safelock(&m_cs);

	KEY nRetKey ;
	try
	{
		//EnterCriticalSection(&m_cs);
		if (!m_elementArr.empty())
		{
            typename MULTIMAP_T::iterator it = m_elementArr.find(nKey);
			if (it != m_elementArr.end())
			{
				it = m_elementArr.upper_bound(nKey);
				if (it != m_elementArr.end())
				{
					nRetKey = it->first;
				}
				else
				{
					nRetKey = (m_elementArr.begin())->first;
				}
			}
			else
			{
				nRetKey = (m_elementArr.begin())->first;
			}
		}
	}
	catch (...)
	{
        //GetMemUsage();
        //WriteErrorLog("CMullitMap<KEY,VALUE>::GetNextKey(int nKey) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);
	return nRetKey;
}

template <class KEY, class VALUE>
KEY CMullitMap<KEY,VALUE>::GetNextKeyData(KEY nKey, VALUE& element)
{
	SafeLock safelock(&m_cs);

	KEY nRetKey ;
	try
	{
		//EnterCriticalSection(&m_cs);
		if (!m_elementArr.empty())
		{
            typename MULTIMAP_T::iterator it = m_elementArr.find(nKey);
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
        //GetMemUsage();
        //WriteErrorLog("CMullitMap<KEY,VALUE>::GetNextKeyData(VALUE& element) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);
	return nRetKey;
}

// template <class KEY, class VALUE>
// int	CMullitMap<KEY,VALUE>::UpdateKeyData(int nKey, const VALUE& element, int nPos)
// {
// 	SafeLock safelock(&m_cs);
// 
// 	int nRet = -1;
// 	try
// 	{
// 		//EnterCriticalSection(&m_cs);
//         typename MULTIMAP_T::iterator it = m_elementArr.find(nKey);
// 		if (it != m_elementArr.end())
// 		{
// 			if (0 == nPos)
// 			{
// 				if (0 != memcmp(&element, &it->second, sizeof(element)))
// 				{
// 					it->second = element;
// 					nRet = 0;
// 				}
// 			}
// 			else
// 			{
// 				int nSize = m_elementArr.count(nKey);
// 				if (nPos > 0 && nPos <= nSize)
// 				{
// 					int nCount = 0;
// 					for (nCount = 0; nCount < nSize; ++it, ++nCount)
// 					{
// 						if (nPos == nCount)
// 						{
// 							if (0 != memcmp(&element, &it->second, sizeof(element)))
// 							{
// 								it->second = element;
// 								nRet = 0;
// 							}
// 							break;
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
// 	catch (...)
// 	{
//         //GetMemUsage();
//         //WriteErrorLog("CMullitMap<KEY,VALUE>::UpdateKeyData(int nKey, const VALUE element, int nPos) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
// 	}
// 	//LeaveCriticalSection(&m_cs);
// 	return nRet;
// }

// template <class KEY, class VALUE>
// int CMullitMap<KEY,VALUE>::DeleteKeyData(int nKey, const VALUE& element, int nPos)
// {
// 	SafeLock safelock(&m_cs);
// 
// 	int nRet = -1;
// 	try
// 	{
// 		//EnterCriticalSection(&m_cs);
//         typename MULTIMAP_T::iterator it = m_elementArr.find(nKey);
// 		if (it != m_elementArr.end())
// 		{
// 			if (0 == nPos)
// 			{
// 				if (0 == memcmp(&element, &it->second, sizeof(element)))
// 				{
// 					m_elementArr.erase(it);
// 					nRet = 0;
// 				}
// 			}
// 			else
// 			{
// 				int nSize = m_elementArr.count(nKey);
// 				if (nPos > 0 && nPos <= nSize)
// 				{
// 					int nCount = 0;
// 					for (nCount = 0; nCount < nSize; ++it, ++nCount)
// 					{
// 						if (nPos == nCount)
// 						{
// 							if (0 == memcmp(&element, &it->second, sizeof(element)))
// 							{
// 								m_elementArr.erase(it);
// 								nRet = 0;
// 							}
// 							break;
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
// 	catch (...)
// 	{
//         //GetMemUsage();
//         //WriteErrorLog("CMullitMap<KEY,VALUE>::DeleteKeyData(int nKey, const VALUE element, int nPos) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
// 	}
// 	//LeaveCriticalSection(&m_cs);
// 	return nRet;
// }

// template <class KEY, class VALUE>
// int	CMullitMap<KEY,VALUE>::GetKeyData(int nKey, VALUE& element, int nPos)
// {
// 	SafeLock safelock(&m_cs);
// 
// 	int nRet = -1;
// 	try
// 	{
// 		//EnterCriticalSection(&m_cs);
// 		if (!m_elementArr.empty())
// 		{
//             typename MULTIMAP_T::iterator it = m_elementArr.find(nKey);
// 			if (it != m_elementArr.end())
// 			{
// 				if (0 == nPos)
// 				{
// 					element = it->second;
// 					nRet = 0;
// 				}
// 				else
// 				{
// 					int nSize = m_elementArr.count(nKey);
// 					if (nPos > 0 && nPos <= nSize)
// 					{
// 						int nCount = 0;
// 						for (nCount = 0; nCount < nSize; ++it, ++nCount)
// 						{
// 							if (nPos == nCount)
// 							{
// 								element = it->second;
// 								nRet = 0;
// 								break;
// 							}
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
// 	catch (...)
// 	{
//         //GetMemUsage();
//         //WriteErrorLog("CMullitMap<KEY,VALUE>::GetKeyData(int nKey, VALUE& element, int nPos) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
// 	}
// 	//LeaveCriticalSection(&m_cs);
// 	return nRet;
// }

template <class KEY, class VALUE>
inline VALUE CMullitMap<KEY,VALUE>::GetHead(int& nErr, bool bIsDel/*=true*/)
{
	SafeLock safelock(&m_cs);
	VALUE element;
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
        //GetMemUsage();
        //WriteErrorLog("CMullitMap<KEY,VALUE>::GetHead(bool bIsDel) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}	
	//LeaveCriticalSection(&m_cs);
	return element;
}

template <class KEY, class VALUE>
void CMullitMap<KEY,VALUE>::RemoveAt(KEY nKey, int nPos)
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
        //GetMemUsage();
        //WriteErrorLog("CMullitMap<KEY,VALUE>::RemoveAt(int nKey, int nPos) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);
}


template <class KEY, class VALUE>
inline int CMullitMap<KEY,VALUE>::GetCount(KEY nKey)
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
        //GetMemUsage();
        //WriteErrorLog("CMullitMap<KEY,VALUE>::GetCount(int nKey) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);

	return nCount;
}

template <class KEY, class VALUE>
inline int CMullitMap<KEY,VALUE>::GetCount()
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
        //GetMemUsage();
        //WriteErrorLog("CMullitMap<KEY,VALUE>::GetCount() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);

	return nCount;
}

template <class KEY, class VALUE>
inline void CMullitMap<KEY,VALUE>::Clear()
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
        //WriteErrorLog("CMullitMap<KEY,VALUE>::Clear() error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);
}

template <class KEY, class VALUE>
inline void CMullitMap<KEY,VALUE>::Clear(KEY nKey)
{
	SafeLock safelock(&m_cs);

	try
	{
		//EnterCriticalSection(&m_cs);
		m_elementArr.erase(nKey);
	}
	catch (...)
	{
        //GetMemUsage();
        //WriteErrorLog("CMullitMap<KEY,VALUE>::Clear(int nKey) error! Mem:%I64d VMem:%I64d", m_mem, m_vmem);
	}
	//LeaveCriticalSection(&m_cs);
}

/*
template <class KEY, class VALUE>
void CMullitMap<KEY,VALUE>::GetMemUsage()
{
    m_mem = 0;
    m_vmem = 0;
    get_memory_usage(&m_mem, &m_vmem);
    m_mem  = m_mem/(1024*1024);
    m_vmem = m_vmem/(1024*1024);
}
*/
/*
template <class KEY, class VALUE>
void CMullitMap<KEY,VALUE>::WriteErrorLog(const char *szLogFmt, ...)
{
    va_list vl;
    int nMaxSize = sizeof(m_szLog);
    memset(m_szLog, 0, nMaxSize);
    //以Printf方式组织输出
    va_start(vl, szLogFmt);
    _vsntprintf(m_szLog, nMaxSize, szLogFmt, vl);
    va_end(vl);
    CLogMgr::GetInstance().WriteLog(m_szLog, "CMemListExError.txt");
}
*/
#endif
