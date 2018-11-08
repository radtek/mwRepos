//////////////////////////////////////////////////////////////////////////
/*
	MemListEx.h
	��ģ����ʵ�ָ����������͵�element��Ϣ����ӡ�

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
                    /* CMullitMap��������ʵ�ֲ��� */
//////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE>
class CMullitMap
{
	public:
		CMullitMap();
		virtual ~CMullitMap();

		//************************************************************************/
		// Description:��nKeyΪ������element����ӳ���β��
		// Returns:   void
		// Parameter: [IN]int nKey ���� [IN]const T& element ��ֵ
		//************************************************************************/
		void	AddTail(KEY nKey, const VALUE& element);

		//************************************************************************/
		// Description:��nKeyΪ������element����ӳ���ͷ��
		// Returns:   void
		// Parameter: [IN]int nKey ���� [IN]const T& element ��ֵ
		//************************************************************************/
		void	AddHead(KEY nKey, const VALUE& element);

		//************************************************************************/
		// Description:ȡnKey�����µĵ�һ��Ԫ�أ�bIsDelΪ��ɾ����Ԫ�أ�Ϊ�ٲ�ɾ��
		// Returns:   VALUE  nKey�µ�ͷԪ��
		// Parameter: [IN]int nKey ���� [IN]bool bIsDel ɾ����־
		//************************************************************************/
		VALUE		GetHead(int& nErr, KEY nKey, bool bIsDel = true);

		//************************************************************************/
		// Description:ȡӳ���ĵ�һ��Ԫ�أ�bIsDelΪ��ɾ����Ԫ�أ�Ϊ�ٲ�ɾ��
		// Returns:   VALUE  ͷԪ��
		// Parameter: [IN]int nKey ���� [IN]bool bIsDel ɾ����־
		//************************************************************************/
		VALUE		GetHead(int& nErr, bool bIsDel = true);

		//************************************************************************/
		// Description:ȡnKey�����µ�һ��Ԫ�أ�ȡ���򷵻�0��ûȡ������-1
		// Returns:   int  0:ȡ����һ��Ԫ�� -1:ʧ��
		// Parameter: [IN]int nKey ���� [OUT]VALUE& element ����ȡ����ֵ
		//************************************************************************/
		int		GetHeadEx(KEY nKey, VALUE& element);

		//************************************************************************/
		// Description:ȡnKey����һ����������ӳ���Ϊ�շ���-1�����ѵ���ӳ���β������key
		// Returns:   int  -1:û��ȡ���¸�����  ��0:��һ��Key
		// Parameter: [IN]int nKey  ��ǰKey
		//************************************************************************/
		KEY		GetNextKey(KEY nKey);

		//************************************************************************/
		// Description:ȡnkey������data����������һ��nkey, ȡ������data��ûȡ������-1
		// Returns:   int  -1:û��ȡ��  ��0:��һ��Key
		// Parameter: [IN]int nKey  ��ǰKey [OUT]VALUE& element  nKey��Ӧ�ļ�ֵ
		//************************************************************************/
		KEY		GetNextKeyData(KEY nKey, VALUE& element);

		//************************************************************************/
		// Description:ɾ��nkey�µ�npos��Ԫ��
		// Returns:   void
		// Parameter: [IN]int nKey  ��ǰKey [IN]int nPos Ԫ��λ��
		//************************************************************************/
		void	RemoveAt(KEY nKey, int nPos = 0);

		//************************************************************************/
		// Description:ȡnkey�����¹��ж���data
		// Returns:   int Ԫ�ظ���
		// Parameter: [IN]int nKey  ��ǰKey 
		//************************************************************************/
		int		GetCount(KEY nKey);

		//************************************************************************/
		// Description:ȡӳ�����Ԫ����
		// Returns:   int Ԫ�ظ���
		// Parameter: void
		//************************************************************************/
		int		GetCount();

		//************************************************************************/
		// Description:ȡӳ�������������
		// Returns:   int ��������
		// Parameter: void
		//************************************************************************/
		int		GetKeyCount();

		//************************************************************************/
		// Description:nkey����û��Ԫ��
		// Returns:   bool 
		// Parameter: [IN]int nKey  ��ǰKey 
		//************************************************************************/
		bool	IsEmpty(KEY nKey);

		//************************************************************************/
		// Description:ӳ����Ƿ�Ϊ��
		// Returns:   bool 
		// Parameter: void 
		//************************************************************************/
		bool	IsEmpty();

		//************************************************************************/
		// Description:���������������Ԫ��
		// Returns:   void 
		// Parameter: [IN]int nKey  ��ǰKey 
		//************************************************************************/
		void	Clear(KEY nKey);

		//************************************************************************/
		// Description:���ӳ���
		// Returns:   void 
		// Parameter: void
		//************************************************************************/
		void	Clear();

		//************************************************************************/
		// Description:����nkey�Ƿ����
		// Returns:   bool 
		// Parameter: [IN]int nKey  ��ǰKey
		//************************************************************************/
		bool	Find(KEY nKey);

		//************************************************************************/
		// Description:���������ַ���
		// Returns:   void 
		// Parameter: [OUT]char *szKey
		//************************************************************************/
		//void	GetKeyStr(char *szKey);

		//************************************************************************/
		// Description:ȡ��ǰnkey��nposλ���ϵ�ֵ���ɹ�����0�����ɹ�����-1
		// Returns:   int 0:�ɹ�  -1:ʧ��
		// Parameter: [IN]int nKey ��ǰKey [OUT]VALUE& element ��ǰKey�ļ�ֵ [IN]int nPos Ԫ��λ��
		//************************************************************************/
		//int		GetKeyData(int nKey, VALUE& element, int nPos=0);

		//************************************************************************/
		// Description:ɾ��nkey��nposλ���ϵ�ֵ���ɹ�����0�����ɹ�����-1 
		// Returns:   int 0:�ɹ�  -1:ʧ��
		// Parameter: [IN]int nKey ��ǰKey [IN]const VALUE element ��ǰKey�ļ�ֵ [IN]int nPos Ԫ��λ��
		//************************************************************************/
		//int		DeleteKeyData(int nKey, const VALUE& element, int nPos=0);

		//************************************************************************/
		// Description:����nkey��nposλ���ϵ�ֵ���ɹ�����0�����ɹ�����-1
		// Returns:   int 0:�ɹ�  -1:ʧ��
		// Parameter: [IN]int nKey ��ǰKey [IN]const VALUE element ��ǰKey�ļ�ֵ [IN]int nPos Ԫ��λ��
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
    //��Printf��ʽ��֯���
    va_start(vl, szLogFmt);
    _vsntprintf(m_szLog, nMaxSize, szLogFmt, vl);
    va_end(vl);
    CLogMgr::GetInstance().WriteLog(m_szLog, "CMemListExError.txt");
}
*/
#endif
