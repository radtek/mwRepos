#ifndef __COMPATIBLEEVENT__H__
#define __COMPATIBLEEVENT__H__

#include "Adapter.h"

class CCompatibleEvent;

typedef struct MEvent
{
    CCompatibleEvent* me_pMEvent;       //指向等多个事件时代替事件组等待而使用的事件
    unsigned int      me_nNo;           //标记是自已是事件组中的第几个事件

    MEvent()
    {
        me_pMEvent = NULL;
        me_nNo = 0;
    }
}MEVENT, *PMEVENT;

class CCompatibleEvent
{
	friend class CAdapter;
public:
    CCompatibleEvent();
    ~CCompatibleEvent();

//对象的比较
bool operator==(CCompatibleEvent &event)
{
    bool bRet = false;
    if(this == &event)
    {
        bRet = true;
    }
    else
    {
    #if (defined  MW_WINDOWS) || (defined  WIN32)
        if(m_Event == event.GetEvent())
        {
            bRet = true;
        }
    #elif defined LINUX
        if(&m_Event == &(event.GetEvent()))
        {
            bRet = true;
        }
    #endif
    }
    return bRet;
}

//与空NULL的比较
bool operator==(void*event)
{
    if(NULL == event)
    {
    #if (defined  MW_WINDOWS) || (defined  WIN32)
        return m_Event == NULL;
    #elif defined LINUX
    //当m_nSignalCnt为-1时表示条件变量未被初始化(未调用Init())
        if(-1 == m_nSignalCnt)
        {
            return true;
        }
        else
        {
            return false;
        }
    #endif
    }
    return false;
}

bool operator!=(void*event)
{
    return !(*this == event);
}

CCompatibleEvent& operator=(void *event)
{
    if(NULL == event)
    {
        UnInit();
    }
    else
    {
        ;
    }
    return *this;
}


CCompatibleEvent& operator=(CCompatibleEvent &event);

private:
    //**********************************
    //Description: 销毁WINDOWS、linux平台兼容的同步量
    //             WINDOWS使用event
    //             linux使用条件变量
    // Parameter:  void
    // Returns:    bool  有信号返回true，否则返回false
    //************************************
    void UnInit();


    //**********************************
    //Description: 初始化WINDOWS、linux平台兼容的同步量
    //             WINDOWS使用event
    //             linux使用条件变量
    // Parameter:  void
    // Returns:    bool  有信号返回true，否则返回false
    //************************************
    bool Init(bool bManualReset=false);


    //**********************************
    //Description: 设置同步事件为有信号状态
    // Parameter:  void
    // Returns:    void
    //************************************
    void SetCompatibleEvent();

    //**********************************
    //Description: 判断当前同步事件是否有信号
    // Parameter:  void
    // Returns:    bool  有信号返回true，否则返回false

    //************************************
    bool IsSignaled();
    //**********************************
    //Description: 获取当前对象中的事件成员
    // Parameter:  void
    // Returns:    事件EVENT window环境为HANDLE linux环境为pthread_cond_t

    //************************************
    EVENT& GetEvent();

    //**********************************
    //Description: 设置条件变量为无信号
    // Parameter:  void
    // Returns:    void
    //************************************
    void ResetSignalCnt();

    //**********************************
    //Description: 返回是否需要手动清信号标志
    // Parameter:  void
    // Returns:    bool  需要返回true，否则返回false
    //************************************
	bool IsManualReset() const;

    //**********************************
    //Description: 设置等多个事件成员m_mEvent
    // Parameter:  pEvent 指向CCompatibleEvent对象的指针
    // Returns:    void
    //************************************
	bool SetMEvent(CCompatibleEvent *pMEvent, unsigned int nNo);

    //**********************************
    //Description: 获取等多个事件成员m_mEvent
    // Parameter:  void
    // Returns:    const MEVENT*
    //************************************
    MEVENT* GetMEvent();

private:
    EVENT m_Event;
	bool  m_bManualReset;			//是否是手动事件
#ifdef LINUX
    //**********************************
    //Description: 获取时间对象的互斥对象
    // Parameter:  void
    // Returns:    linux环境为pthread_mutex_t window环境无需此函数
    //************************************
    pthread_mutex_t& GetMutex();

    int         m_nSignalCnt;
    DerivedLock m_cs;
	pthread_mutex_t m_mutex;
	pthread_condattr_t m_attr;
    MEVENT      m_MEvent;			//有事件标志(用于等待多个事件)
	bool		m_bInit;			//是否已经初始化
#endif

};

#endif

