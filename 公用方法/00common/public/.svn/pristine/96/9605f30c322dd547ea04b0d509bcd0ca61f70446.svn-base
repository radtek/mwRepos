// GetMem.h: interface for the CGetMem class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __GETMEM_H__
#define __GETMEM_H__

#include "MemPool.h"

template <class T, int COUNT>
class CGetMem
{
public:
    CGetMem();
    CGetMem(CMemPool<T,COUNT> *pMemPool, T*& pBuf);
    virtual ~CGetMem();
private:
    CMemPool<T,COUNT> *m_pMemPool;
    T* m_pBuf;
};

template <class T, int COUNT>
CGetMem<T,COUNT>::CGetMem()
{
    m_pMemPool = NULL;
    m_pBuf     = NULL;
}

template <class T, int COUNT>
CGetMem<T,COUNT>::CGetMem(CMemPool<T,COUNT> *pMemPool, T*& pBuf)
{
    if (NULL != pMemPool)
    {
        m_pMemPool = pMemPool;
        pBuf = m_pMemPool->GetMem();
        if (NULL != pBuf)
        {
            m_pBuf = pBuf;
        }
    }
}

template <class T, int COUNT>
CGetMem<T,COUNT>::~CGetMem()
{
    if (NULL != m_pBuf)
    {
        if (NULL != m_pMemPool)
        {
            m_pMemPool->RecycleMem(m_pBuf);
        }
    }
}

#endif

