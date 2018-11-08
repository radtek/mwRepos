//CompatibleEvent.cpp

#include "StdAfx.h"

#include "CompatibleEvent.h"
#include <iostream>
using namespace std;


////////////////////////////////////////////////////////////////////////////
//         ������       ����WINDOWS��linuxƽ̨                            //
//                                                                        //
//                      WINDOWS���뻷������                               //
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


#if (defined  MW_WINDOWS) || (defined  WIN32)

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
//Description: ����WINDOWSƽ̨��ͬ����(�¼�����)
//             WINDOWSʹ��event
//             linuxʹ����������
// Parameter:  void
// Returns:    bool  ���źŷ���true�����򷵻�false
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
//Description: ��ʼ��WINDOWSƽ̨��ͬ����(�¼�����)
//             WINDOWSʹ��event
//             linuxʹ����������
// Parameter:  void
// Returns:    bool  �ɹ�����TRUE  ���򷵻�FALSE
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
//Description: ����ͬ���¼�Ϊ���ź�״̬(�¼�����)
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
//Description: �жϵ�ǰͬ���¼��Ƿ����ź�(�¼�����)
// Parameter:  void
// Returns:    bool  ���źŷ���true�����򷵻�false
//************************************
bool CCompatibleEvent::IsSignaled()
{
    return false;       //windows��������Ҫ�õ��ú���
}

//**********************************
//Description: ��ȡ��ǰ�����е��¼���Ա(�¼�����)
// Parameter:  void
// Returns:    �¼�EVENT window����ΪHANDLE linux����Ϊpthread_cond_t
//************************************
EVENT& CCompatibleEvent::GetEvent()
{
    return m_Event;
}

//**********************************
//Description: ������������Ϊ���ź�
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
//Description: �����Ƿ���Ҫ�ֶ����źű�־
// Parameter:  void
// Returns:    bool  ��Ҫ����true�����򷵻�false
//************************************
bool CCompatibleEvent::IsManualReset() const
{
	return m_bManualReset;
}

//**********************************
//Description: ���õȶ���¼���Աm_mEvent
// Parameter:  pEvent ָ��CCompatibleEvent�����ָ��
// Returns:    bool ԭ�¼�Ϊ���ź�״̬ʱ����true 
//************************************
bool CCompatibleEvent::SetMEvent(CCompatibleEvent *pEvent, unsigned int nNo)
{
    return false;
}

//**********************************
//Description: ��ȡ�ȶ���¼���Աm_mEvent
// Parameter:  void
// Returns:    MEVENT*
//************************************
MEVENT* CCompatibleEvent::GetMEvent()
{
    return NULL;
}



////////////////////////////////////////////////////////////////////////////
//         ������       ����WINDOWS��linuxƽ̨                            //
//                                                                        //
//                      linux���뻷������                                 //
////////////////////////////////////////////////////////////////////////////
#elif defined LINUX

CCompatibleEvent::CCompatibleEvent()
{
	m_bInit		   = false;
    m_bManualReset = false;
    //��ӱ�־����ʾ��ǰ��������δ����ʼ��
    m_nSignalCnt = -1;
	m_MEvent.me_nNo = 0;
	m_MEvent.me_pMEvent = NULL;
}

CCompatibleEvent::~CCompatibleEvent()
{
    UnInit();
}

//**********************************
//Description: ����linuxƽ̨��ͬ����(��������)
//             WINDOWSʹ��event
//             linuxʹ����������
// Parameter:  void
// Returns:    bool  ���źŷ���true�����򷵻�false
//************************************
void CCompatibleEvent::UnInit()
{
    SafeLock safelock(&m_cs);
	//�����ǰ״̬���ѳ�ʼ��,������,��״̬Ϊ������,��ʲôҲ����
    if (m_bInit) 
	{
		pthread_cond_destroy(&m_Event);
		pthread_condattr_destroy(&m_attr);
		pthread_mutex_destroy(&m_mutex);
	}
    m_nSignalCnt = -1;
	m_bInit = false;

	return;
}

//**********************************
//Description: ��ʼ��linuxƽ̨��ͬ����(��������)
//             WINDOWSʹ��event
//             linuxʹ����������
// Parameter:  void
// Returns:    bool  ���źŷ���true�����򷵻�false
//************************************
bool CCompatibleEvent::Init(bool bManualReset)
{
    SafeLock safelock(&m_cs);
    m_bManualReset = bManualReset;
    m_nSignalCnt = 0;

	if(0 == pthread_condattr_init(&m_attr))
	{
		if(pthread_condattr_setclock(&m_attr, CLOCK_MONOTONIC) != 0)
		{
			pthread_condattr_destroy(&m_attr);
			return m_bInit = false;
		}
	}

	if (0 != pthread_cond_init(&m_Event, &m_attr)
		|| 0 != pthread_mutex_init(&m_mutex, NULL))
    {
        return m_bInit=false;
    }
    else
    {
        return m_bInit=true;
    }
}

//**********************************
//Description: ����ͬ���¼�Ϊ���ź�״̬(��������)
// Parameter:  void
// Returns:    void
//************************************
void CCompatibleEvent::SetCompatibleEvent()
{
    SafeLock safelock(&m_cs);
	pthread_mutex_lock(&m_mutex);
	++m_nSignalCnt;
    pthread_cond_signal(&m_Event);
	pthread_mutex_unlock(&m_mutex);
    //pthread_cond_broadcast(&m_Event);
    
    //DEBUG_PRINTF("m_MEvent.me_pMEvent ", m_pMEvent->me_pMEvent);
	//�ж��Ƿ��д����¼���ȴ����¼����ڣ����н�����Ϊ���ź�
	//����ָ���ÿգ���ֹ�ظ����¼�
    if(m_MEvent.me_pMEvent != NULL)
    {
        m_MEvent.me_pMEvent->SetMEvent(NULL, m_MEvent.me_nNo);
        m_MEvent.me_pMEvent->SetCompatibleEvent();
		m_MEvent.me_pMEvent = NULL;
    }
    return;
}

//**********************************
//Description: �жϵ�ǰͬ���¼��Ƿ����ź�(��������)
// Parameter:  void
// Returns:    bool  ���źŷ���true�����򷵻�false
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
//Description: ��ȡ��ǰ�����е��¼���Ա(��������)
// Parameter:  void
// Returns:    �¼�EVENT window����ΪHANDLE linux����Ϊpthread_cond_t
//************************************
EVENT& CCompatibleEvent::GetEvent()
{
    return m_Event;
}

//**********************************
//Description: ������������Ϊ���ź�
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
//Description: �����Ƿ���Ҫ�ֶ����źű�־
// Parameter:  void
// Returns:    bool  ��Ҫ����true�����򷵻�false
//************************************
bool CCompatibleEvent::IsManualReset() const
{
    return m_bManualReset;
}

//**********************************
//Description: ���õȶ���¼���Աm_mEvent
// Parameter:  pEvent ָ��CCompatibleEvent�����ָ��
// Returns:    bool ԭ�¼�Ϊ���ź�״̬ʱ����true 
//************************************
bool CCompatibleEvent::SetMEvent(CCompatibleEvent *pEvent, unsigned int nNo)
{
	bool bRet = false;
	SafeLock safelock(&m_cs);
    m_MEvent.me_pMEvent = pEvent;
    m_MEvent.me_nNo = nNo;

	if (NULL != pEvent && 0 < m_nSignalCnt)
	{
		bRet = true;
	}

	return bRet;
}

//**********************************
//Description: ��ȡ�ȶ���¼���Աm_mEvent
// Parameter:  void
// Returns:    MEVENT*
//************************************
MEVENT* CCompatibleEvent::GetMEvent()
{
    return &m_MEvent;
}

//**********************************
//Description: ��ȡʱ�����Ļ������
// Parameter:  void
// Returns:    linux����Ϊpthread_mutex_t window��������˺���
//************************************
pthread_mutex_t& CCompatibleEvent::GetMutex()
{
	return m_mutex;
}

#endif

