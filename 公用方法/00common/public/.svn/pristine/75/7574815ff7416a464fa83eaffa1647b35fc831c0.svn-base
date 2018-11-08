#include "utility/common/mwcorepch.h"
#include "utility/common/MThread.h"
using namespace ::UTI::COM;

#ifndef _WINDOWS
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
#endif

MThread::MThread(void) :
	m_bRun(false)
{
}

MThread::~MThread(void)
{
}
/************************************************************************
*  功能: 开始运行线程
************************************************************************/
bool MThread::Start()
{
	if (m_bRun)
	{
		return true;
	}
#ifdef _WINDOWS
	unsigned int uThreadID;
	m_handle = (HANDLE)_beginthreadex(NULL, 0, StaticThreadFunc, this, 0, &uThreadID);
	m_bRun = (NULL != m_handle);
#else 
    m_bRun = (pthread_create(&m_handle, NULL, StaticThreadFunc, this) == 0);
#endif
	return m_bRun;
}

/************************************************************************
*  功能: 当前执行此函数线程等待线程结束
*  参数: timeout 等待超时时间，如果为负数，等待无限时长
************************************************************************/
void MThread::Join(int timeout)
{
#ifdef _WINDOWS
	if (NULL == m_handle || !m_bRun)
	{
		return;
	}
	if (timeout <= 0)
	{
		timeout = INFINITE;
	}
	::WaitForSingleObject(m_handle, timeout);
#else
	if (m_bRun) {
		pthread_join(m_handle, NULL);
	}
#endif
}

/************************************************************************
*  功能: 终止线程的执行
*  参数: ExitCode 返回的值
************************************************************************/
bool MThread::Terminate(unsigned long ExitCode)
{
#ifdef _WINDOWS
	if (NULL == m_handle || !m_bRun)
	{
		return true;
	}
	if (::TerminateThread(m_handle, ExitCode))
	{
		::CloseHandle(m_handle);
		return true;
	}
#else
	if (m_bRun) {
		m_bRun = false;
		pthread_detach(m_handle);
		return true;
	}		
#endif
	return false;
}

/************************************************************************
*  功能: 线程的执行函数
*  参数: arg 线程参数，为调用对象指针
************************************************************************/
#ifdef _WINDOWS
unsigned int MThread::StaticThreadFunc(void * arg)
#else
void* MThread::StaticThreadFunc(void *arg)
#endif
{
	MThread * pThread = (MThread *)arg;
	pThread->Run();
	return 0;
}

