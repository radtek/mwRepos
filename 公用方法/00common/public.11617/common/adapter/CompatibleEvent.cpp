//CompatibleEvent.cpp

#include "StdAfx.h"

#include "CompatibleEvent.h"
#include <iostream>
using namespace std;


////////////////////////////////////////////////////////////////////////////
//         适配器       兼容WINDOWS和linux平台                            //
//                                                                        //
//                      WINDOWS编译环境代码                               //
////////////////////////////////////////////////////////////////////////////

CCompatibleEvent& CCompatibleEvent::operator=(CCompatibleEvent &event)
{
    if(this != &event)
    {
        m_Event = event.GetEvent();
        m_bManualReset = event.m_bManualReset;

    #ifdef LINUX
        const MEVENT *mEvent = event.GetMEvent();
        m_MEvent.me_pMEvent = mEvent->me_pMEvent;
        m_MEvent.me_nNo = mEvent->me_nNo;
    #endif
    }
    return *this;
}


#ifdef WIN32

CCompatibleEvent::CCompatibleEvent()
{
	m_bManualReset = false;
    m_Event = NULL;
}

CCompatibleEvent::~CCompatibleEvent()
{
    UnInit();
}


//**********************************
//Description: 销毁WINDOWS平台的同步量(事件对象)
//             WINDOWS使用event
//             linux使用条件变量
// Parameter:  void
// Returns:    bool  有信号返回true，否则返回false
//************************************
void CCompatibleEvent::UnInit()
{
    if(m_Event != NULL)
    {
        CloseHandle(m_Event);
        m_Event = NULL;
    }
    return;
}


//**********************************
//Description: 初始化WINDOWS平台的同步量(事件对象)
//             WINDOWS使用event
//             linux使用条件变量
// Parameter:  void
// Returns:    bool  成功返回TRUE  否则返回FALSE
//************************************
bool CCompatibleEvent::Init(bool bManualReset)
{
    bool bRet = FALSE;
	m_bManualReset = bManualReset;
    if(NULL == m_Event)
    {
        m_Event = CreateEvent(NULL, m_bManualReset, FALSE, NULL);
        if(NULL == m_Event)
        {
            bRet = FALSE;
        }
        else
        {
            bRet = TRUE;
        }
    }
    return bRet;
}


//**********************************
//Description: 设置同步事件为有信号状态(事件对象)
// Parameter:  void
// Returns:    void
//************************************
void CCompatibleEvent::SetCompatibleEvent()
{
    if(NULL != m_Event)
    {
        SetEvent(m_Event);
    }
    return;
}

//**********************************
//Description: 判断当前同步事件是否有信号(事件对象)
// Parameter:  void
// Returns:    bool  有信号返回true，否则返回false
//************************************
bool CCompatibleEvent::IsSignaled()
{
    return false;       //windows环境不需要用到该函数
}

//**********************************
//Description: 获取当前对象中的事件成员(事件对象)
// Parameter:  void
// Returns:    事件EVENT window环境为HANDLE linux环境为pthread_cond_t
//************************************
EVENT& CCompatibleEvent::GetEvent()
{
    return m_Event;
}

//**********************************
//Description: 设置条件变量为无信号
// Parameter:  void
// Returns:    void
//************************************
void CCompatibleEvent::ResetSignalCnt()
{
	if (m_bManualReset)
	{
		::ResetEvent(m_Event);
	}
    return;
}

//**********************************
//Description: 返回是否需要手动清信号标志
// Parameter:  void
// Returns:    bool  需要返回true，否则返回false
//************************************
bool CCompatibleEvent::IsManualReset() const
{
	return m_bManualReset;
}

//**********************************
//Description: 设置等多个事件成员m_mEvent
// Parameter:  pEvent 指向CCompatibleEvent对象的指针
// Returns:    void
//************************************
void CCompatibleEvent::SetMEvent(CCompatibleEvent *pEvent, unsigned int nNo)
{
    return;
}

//**********************************
//Description: 获取等多个事件成员m_mEvent
// Parameter:  void
// Returns:    MEVENT*
//************************************
MEVENT* CCompatibleEvent::GetMEvent()
{
    return NULL;
}



////////////////////////////////////////////////////////////////////////////
//         适配器       兼容WINDOWS和linux平台                            //
//                                                                        //
//                      linux编译环境代码                                 //
////////////////////////////////////////////////////////////////////////////
#elif defined LINUX

CCompatibleEvent::CCompatibleEvent()
{
	m_bInit		   = false;
    m_bManualReset = false;
    //添加标志，表示当前条件变量未被初始化
    m_nSignalCnt = -1;
	m_MEvent.me_nNo = 0;
	m_MEvent.me_pMEvent = NULL;
}

CCompatibleEvent::~CCompatibleEvent()
{
    UnInit();
}

//**********************************
//Description: 销毁linux平台的同步量(条件变量)
//             WINDOWS使用event
//             linux使用条件变量
// Parameter:  void
// Returns:    bool  有信号返回true，否则返回false
//************************************
void CCompatibleEvent::UnInit()
{
    SafeLock safelock(&m_cs);
	//如果当前状态是已初始化,则销毁,若状态为已销毁,则什么也不做
    if (m_bInit) 
	{
		pthread_cond_destroy(&m_Event);
	}
    m_nSignalCnt = -1;
	m_bInit = false;

	return;
}

//**********************************
//Description: 初始化linux平台的同步量(条件变量)
//             WINDOWS使用event
//             linux使用条件变量
// Parameter:  void
// Returns:    bool  有信号返回true，否则返回false
//************************************
bool CCompatibleEvent::Init(bool bManualReset)
{
    SafeLock safelock(&m_cs);
    m_bManualReset = bManualReset;
    m_nSignalCnt = 0;
    if (0 != pthread_cond_init(&m_Event, NULL))
    {
        return m_bInit=false;
    }
    else
    {
        return m_bInit=true;
    }
}

//**********************************
//Description: 设置同步事件为有信号状态(条件变量)
// Parameter:  void
// Returns:    void
//************************************
void CCompatibleEvent::SetCompatibleEvent()
{
    SafeLock safelock(&m_cs);
    pthread_cond_signal(&m_Event);
    //pthread_cond_broadcast(&m_Event);
    ++m_nSignalCnt;

    //DEBUG_PRINTF("m_MEvent.me_pMEvent ", m_pMEvent->me_pMEvent);
	//判断是否有代替事件组等待的事件存在，若有将其置为有信号
	//并将指针置空，防止重复置事件
    if(m_MEvent.me_pMEvent != NULL)
    {
        m_MEvent.me_pMEvent->SetMEvent(NULL, m_MEvent.me_nNo);
        m_MEvent.me_pMEvent->SetCompatibleEvent();
		m_MEvent.me_pMEvent = NULL;
    }
    return;
}

//**********************************
//Description: 判断当前同步事件是否有信号(条件变量)
// Parameter:  void
// Returns:    bool  有信号返回true，否则返回false
//************************************
bool CCompatibleEvent::IsSignaled()
{
    bool bRet = false;
    SafeLock safelock(&m_cs);
    bRet = m_nSignalCnt>0;
    m_nSignalCnt = 0;
	return bRet;
}

//**********************************
//Description: 获取当前对象中的事件成员(条件变量)
// Parameter:  void
// Returns:    事件EVENT window环境为HANDLE linux环境为pthread_cond_t
//************************************
EVENT& CCompatibleEvent::GetEvent()
{
    return m_Event;
}

//**********************************
//Description: 设置条件变量为无信号
// Parameter:  void
// Returns:    void
//************************************
void CCompatibleEvent::ResetSignalCnt()
{
    SafeLock safelock(&m_cs);
    m_nSignalCnt = 0;

	return;
}

//**********************************
//Description: 返回是否需要手动清信号标志
// Parameter:  void
// Returns:    bool  需要返回true，否则返回false
//************************************
bool CCompatibleEvent::IsManualReset() const
{
    return m_bManualReset;
}

//**********************************
//Description: 设置等多个事件成员m_mEvent
// Parameter:  pEvent 指向CCompatibleEvent对象的指针
// Returns:    void
//************************************
void CCompatibleEvent::SetMEvent(CCompatibleEvent *pEvent, unsigned int nNo)
{
	SafeLock safelock(&m_cs);
    m_MEvent.me_pMEvent = pEvent;
    m_MEvent.me_nNo = nNo;
}

//**********************************
//Description: 获取等多个事件成员m_mEvent
// Parameter:  void
// Returns:    MEVENT*
//************************************
MEVENT* CCompatibleEvent::GetMEvent()
{
    return &m_MEvent;
}


#endif

