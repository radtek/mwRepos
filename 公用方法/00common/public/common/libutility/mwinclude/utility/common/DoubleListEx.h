// DoubleListEx.h: interface for the DoubleListEx class.
// Author: JiangHongbo
//////////////////////////////////////////////////////////////////////
#ifndef __DOUBLE_LISTEX_H_
#define __DOUBLE_LISTEX_H_

///////////////////////////////////////////////////////////////
//#define	MSG(msg) { cout << (msg) << endl; }
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
#include <map>
#include <list>
#include "SafeLock.h"
#include "DerivedLock.h"
#include "utility/common/mwcore_export.h"

///////////////////////////////////////////////////////////////

int UTILITY_Export GetGlobalKey();

namespace UTI{
namespace COM{

template<class T, int COUNT>
class UTILITY_Export CDoubleListEx
{
public:
    typedef std::map<size_t, T*> MAP;
    typedef std::list<T*> LIST;
    //typename MAP::iterator ITINUSE;
    //typename LIST::iterator ITIDLE;
public:
    CDoubleListEx();
    virtual ~CDoubleListEx();
private:
    //CRITICAL_SECTION  m_critical_section;
    DerivedLock m_cs;
    int m_nMaxSize;
protected:
    MAP                 m_InUse;    //正在使用的
    LIST                m_Idle;     //空闲的
private:
    void                Init(int nCount);
    void                RemoveList();
    void                RemoveMap();
public:
    void				InitList(int nCount);
    void				ReSetMaxItemCnt(int nMaxCount);
    int                 GetInUseCount();
    int                 GetIdleCount();
    void                RecyleIdleMem();
    T*                  FindKey(size_t nKey);
    T					GetKeyData(size_t nKey);
    T*					GetKeyData(size_t nKey, T& KeyData);
    T*                  GetBlank(int& nKey);
    T*                  GetBlank();
    void                RemoveAt(size_t nKey);
    void                RemoveAtEx(size_t nKey);
    T*                  GetHead();
    T*                  GetNext(size_t nKey);
    size_t				GetNextKey(size_t nKey);
    size_t              GetHeadKey();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*-------------------------------------------------------------
--------------------
功能：构造函数
说明：
返回：
---------------------------------------------------------------------------------*/
template<class T, int COUNT>
CDoubleListEx<T, COUNT>::CDoubleListEx()
{
    m_nMaxSize = COUNT;
    Init(COUNT);
}

template<class T, int COUNT>
void CDoubleListEx<T, COUNT>::InitList(int nCount)
{
    RemoveList();
    RemoveMap();
    Init(nCount);
}

template<class T, int COUNT>
void CDoubleListEx<T, COUNT>::ReSetMaxItemCnt(int nMaxCount)
{
    SafeLock safelock(&m_cs);

    m_nMaxSize = nMaxCount;
}

/*---------------------------------------------------------------------------------
功能：析构函数
说明：
返回：
---------------------------------------------------------------------------------*/
template<class T, int COUNT>
CDoubleListEx<T, COUNT>::~CDoubleListEx()
{
    RemoveList();
    RemoveMap();
    //::DeleteCriticalSection(&m_critical_section);
}

/*---------------------------------------------------------------------------------
功能：初始化空闲链表，并申请所需的内存
说明：
返回：
---------------------------------------------------------------------------------*/
template<class T, int COUNT>
void CDoubleListEx<T, COUNT>::Init(int nCount)
{
    SafeLock safelock(&m_cs);

    m_nMaxSize = nCount;

    T *lp_t = NULL;
    try
    {
        for (int i = 0; i < nCount; ++i)
        {
            lp_t = new T;
            if (NULL != lp_t)
            {
                m_Idle.push_back(lp_t);
            }
        }
    }
    catch (...)
    {
    }
}

/*---------------------------------------------------------------------------------
功能：删除一个空闲链表中所有元素，并释放内存
说明：
返回：
---------------------------------------------------------------------------------*/
template<class T, int COUNT>
void CDoubleListEx<T, COUNT>::RemoveList()
{
    SafeLock safelock(&m_cs);

    try
    {
        for (/*ITIDLE*/typename LIST::iterator it = m_Idle.begin(); it != m_Idle.end(); ++it)
        {
            //删除内存空间
            if (*it != NULL)
            {
                delete *it;
                *it = NULL;
            }
        }

        //清空链表
        m_Idle.clear();
    }
    catch (...)
    {
    }
}


/*---------------------------------------------------------------------------------
功能：删除一个使用表中所有元素，并释放内存
说明：
返回：
---------------------------------------------------------------------------------*/
template<class T, int COUNT>
void CDoubleListEx<T, COUNT>::RemoveMap()
{
    SafeLock safelock(&m_cs);

    try
    {
        for (/*ITINUSE*/typename MAP::iterator it = m_InUse.begin(); it != m_InUse.end(); ++it)
        {
            //删除内存空间
            if (it->second != NULL)
            {
                delete it->second;
                it->second = NULL;
            }
        }

        //清空链表
        m_InUse.clear();
    }
    catch (...)
    {
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---------------------------------------------------------------------------------
功能：得到使用表的结点数量
说明：
返回：
---------------------------------------------------------------------------------*/
template<class T, int COUNT>
int CDoubleListEx<T, COUNT>::GetInUseCount()
{
    SafeLock safelock(&m_cs);

    int nCount = 0;

    try
    {
        nCount = m_InUse.size();
    }
    catch (...)
    {
    }

    return nCount;
}

/*---------------------------------------------------------------------------------
功能：清空空表内存
说明：
返回：
---------------------------------------------------------------------------------*/
template<class T, int COUNT>
void CDoubleListEx<T, COUNT>::RecyleIdleMem()
{
    SafeLock safelock(&m_cs);

    try
    {
        /*ITIDLE*/typename LIST::iterator it = m_Idle.begin();
        for (it; it != m_Idle.end();)
        {
            if (NULL != *it)
            {
                delete *it;
                *it = NULL;
            }
            m_Idle.erase(it++);
        }
    }
    catch (...)
    {
    }

}

/*---------------------------------------------------------------------------------
功能：得到空表的结点数量
说明：
返回：
---------------------------------------------------------------------------------*/
template<class T, int COUNT>
int CDoubleListEx<T, COUNT>::GetIdleCount()
{
    SafeLock safelock(&m_cs);

    int nCount = 0;

    try
    {
        nCount = m_Idle.size();
    }
    catch (...)
    {
    }

    return nCount;
}

/*---------------------------------------------------------------------------------
功能：从空表，或者向系统申请一个结点
说明：如果空表已经无结点，且系统已无可用内存，此函数才会失败。
返回：成功，返回申请的结点；失败，NULL
---------------------------------------------------------------------------------*/
template<class T, int COUNT>
T* CDoubleListEx<T, COUNT>::GetBlank(int& nKey)
{
    SafeLock safelock(&m_cs);

    T*	lp_t = NULL;

    try
    {
        /*ITIDLE*/typename LIST::iterator it = m_Idle.begin();
        if (it == m_Idle.end())
        {
            lp_t = new T;
        }
        else
        {
            lp_t = *it;
            m_Idle.pop_front();
        }

        //因为有可能内存不足而申请内存失败
        if (NULL != lp_t)
        {
            //取全局KEY
            nKey = GetGlobalKey();
            //若该Key仍存在则重新取key至到找到不存的key为止
            while (m_InUse.find(nKey) != m_InUse.end() || 0 == nKey)
            {
                nKey = GetGlobalKey();
            }
            //加入映射表中
            memset(lp_t, 0, sizeof(T));
            m_InUse.insert(std::make_pair(nKey, lp_t));
        }
    }
    catch (...)
    {
    }

    return lp_t;
}

template<class T, int COUNT>
T* CDoubleListEx<T, COUNT>::GetBlank()
{
    SafeLock safelock(&m_cs);

    T*  lp_t = NULL;

    try
    {
        /*ITIDLE*/typename LIST::iterator it = m_Idle.begin();
        if (it == m_Idle.end())
        {
            lp_t = new T;
        }
        else
        {
            lp_t = *it;
            m_Idle.pop_front();
        }

        //因为有可能内存不足而申请内存失败
        if (NULL != lp_t)
        {
            //若该地址不存在则加入队列
            if (m_InUse.find((size_t)lp_t) == m_InUse.end())
            {
                memset(lp_t, 0, sizeof(T));
                m_InUse.insert(std::make_pair((size_t)lp_t, lp_t));
            }
        }
    }
    catch (...)
    {
    }

    return lp_t;
}

/*---------------------------------------------------------------------------------
功能：删除使用表中的一个结点
说明：
返回：
---------------------------------------------------------------------------------*/
template<class T, int COUNT>
void CDoubleListEx<T, COUNT>::RemoveAt(size_t nKey)
{
    SafeLock safelock(&m_cs);

    try
    {
        /*ITINUSE*/typename MAP::iterator it = m_InUse.find(nKey);
        if (it != m_InUse.end())
        {
            T *lp_t = it->second;

            m_InUse.erase(it);

            if (NULL != lp_t)
            {
                //memset(lp_t, 0, sizeof(T));
                m_Idle.push_back(lp_t);

                //限制队列最大size为m_nCount
                if (/*m_Idle.size()+m_InUse.size() > COUNT*2 || */m_Idle.size() > m_nMaxSize)
                {
                    typename LIST::iterator it = m_Idle.begin();
                    if (it != m_Idle.end() && *it != lp_t)
                    {
                        T *pBegin = *it;
                        m_Idle.erase(it);
                        if (NULL != pBegin)
                        {
                            delete pBegin;
                            pBegin = NULL;
                        }
                    }
                }
            }
            /*
            if (NULL != lp_t)
            {
            memset(lp_t, 0, sizeof(T));
            m_Idle.push_back(lp_t);
            }
            //限制队列最大size为m_nCount
            if (m_Idle.size() > m_nMaxSize)
            {
            typename LIST::iterator it = m_Idle.begin();
            if (it != m_Idle.end())
            {
            T *pBegin = *it;
            m_Idle.erase(it);
            if (NULL != pBegin)
            {
            delete pBegin;
            pBegin = NULL;
            }
            }
            }
            */
        }
    }
    catch (...)
    {
    }

}

/*---------------------------------------------------------------------------------
功能：删除使用表中的节点，并清空内存
说明：
返回：
---------------------------------------------------------------------------------*/
template<class T, int COUNT>
void CDoubleListEx<T, COUNT>::RemoveAtEx(size_t nKey)
{
    SafeLock safelock(&m_cs);

    try
    {
        /*ITINUSE*/typename MAP::iterator it = m_InUse.find(nKey);
        if (it != m_InUse.end())
        {
            T *lp_t = it->second;

            m_InUse.erase(it);

            if (NULL != lp_t)
            {
                if (NULL != lp_t)
                {
                    delete lp_t;
                    lp_t = NULL;
                }
            }
        }
    }
    catch (...)
    {
    }
}

/*---------------------------------------------------------------------------------
功能：得到指定key的结点
说明：
返回：
---------------------------------------------------------------------------------*/
template<class T, int COUNT>
T* CDoubleListEx<T, COUNT>::FindKey(size_t nKey)
{
    SafeLock safelock(&m_cs);

    T* p = NULL;

    try
    {
        /*ITINUSE*/typename MAP::iterator it = m_InUse.find(nKey);
        if (it != m_InUse.end())
        {
            p = it->second;
        }
    }
    catch (...)
    {
    }

    return p;
}

/*---------------------------------------------------------------------------------
功能：得到指定key下指针所指内容
说明：
返回：
---------------------------------------------------------------------------------*/
template<class T, int COUNT>
T CDoubleListEx<T, COUNT>::GetKeyData(size_t nKey)
{
    SafeLock safelock(&m_cs);

    T data;

    try
    {
        /*ITINUSE*/typename MAP::iterator it = m_InUse.find(nKey);
        if (it != m_InUse.end())
        {
            if (NULL != it->second)
            {
                data = *it->second;
            }
        }
    }
    catch (...)
    {
    }

    return data;
}

/*---------------------------------------------------------------------------------
功能：得到指定key下信息
说明：
返回：KEY的指针,以及数据的拷贝
---------------------------------------------------------------------------------*/
template<class T, int COUNT>
T* CDoubleListEx<T, COUNT>::GetKeyData(size_t nKey, T& KeyData)
{
    SafeLock safelock(&m_cs);

    T* p = NULL;

    try
    {
        /*ITINUSE*/typename MAP::iterator it = m_InUse.find(nKey);
        if (it != m_InUse.end())
        {
            if (NULL != (p = it->second)) KeyData = *p;
        }
    }
    catch (...)
    {
    }

    return p;
}

/*---------------------------------------------------------------------------------
功能：得到使用表头结点
说明：
返回：
---------------------------------------------------------------------------------*/
template<class T, int COUNT>
T* CDoubleListEx<T, COUNT>::GetHead()
{
    SafeLock safelock(&m_cs);

    T* p = NULL;

    try
    {
        /*ITINUSE*/typename MAP::iterator it = m_InUse.begin();
        if (it != m_InUse.end())
        {
            p = it->second;
        }
    }
    catch (...)
    {
    }

    return p;
}

/*---------------------------------------------------------------------------------
功能：返回当前Key的下一个Key的内容,若当前key已经是最后一个则返回NULL,若链表为空则返回NULL
说明：如果返回NULL，说明已经到了表尾
返回：
---------------------------------------------------------------------------------*/
template<class T, int COUNT>
T* CDoubleListEx<T, COUNT>::GetNext(size_t nKey)
{
    SafeLock safelock(&m_cs);

    T*  lp_t = NULL;

    try
    {
        /*ITINUSE*/typename MAP::iterator it = m_InUse.find(nKey);
        if (it != m_InUse.end())
        {
            ++it;
            if (it != m_InUse.end())
            {
                lp_t = it->second;
            }
        }
    }
    catch (...)
    {
    }

    return lp_t;
}

/*---------------------------------------------------------------------------------
功能：由当前Key取得下一个Key,如果当前已是最后一个,返回-1
说明：如果返回-1,说明已经到了表尾
返回：
---------------------------------------------------------------------------------*/
template<class T, int COUNT>
size_t CDoubleListEx<T, COUNT>::GetNextKey(size_t nKey)
{
    SafeLock safelock(&m_cs);

    size_t nNextKey = 0;

    try
    {
        /*ITINUSE*/typename MAP::iterator it = m_InUse.find(nKey);
        if (it != m_InUse.end())
        {
            ++it;
            if (it != m_InUse.end())
            {
                nNextKey = it->first;
            }
        }
    }
    catch (...)
    {
    }

    return nNextKey;
}

/*---------------------------------------------------------------------------------
功能：得到使用表的头结点的key,若无头结点返回-1
说明：
返回：
---------------------------------------------------------------------------------*/
template<class T, int COUNT>
size_t CDoubleListEx<T, COUNT>::GetHeadKey()
{
    SafeLock safelock(&m_cs);

    size_t nKey = 0;

    try
    {
        /*ITINUSE*/typename MAP::iterator it = m_InUse.begin();
        if (it != m_InUse.end())
        {
            nKey = it->first;
        }
    }
    catch (...)
    {
    }

    return nKey;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
}} // end namespace


#endif
