//CompatibleThread.cpp

#include  "StdAfx.h"

#include "CompatibleThread.h"


CCompatibleThread::CCompatibleThread()
{

}

CCompatibleThread::~CCompatibleThread()
{

}
#if (defined  MW_WINDOWS) || (defined  WIN32)
//**********************************
//Description: 创建兼容WINDOWS和linux的线程
// Parameter:  THREAD_RETURN(*start_address)(void *)  创建线程运行函数的入口地址
//              void*arg 为传入线程使用的参数
// Returns:    int 返回0启动线程成功 非0表示失败
//************************************
int CCompatibleThread::CreateThread(THREAD_RETURN(__STDCALL *start_address)(void *), void *arg)
{
    int iRet = 0;
    if(NULL == start_address)
    {
        return -1;
    }

	uintptr_t nRet = _beginthreadex(NULL, NULL, start_address, arg, 0, NULL);
    if(0 == nRet )
    {
        iRet = -1;
    }
	else
	{
		CloseHandle((HANDLE)nRet);
	}

    return iRet;
}

int CCompatibleThread::CreateThread(THREAD_RETURN(__STDCALL *start_address)(void *), void *arg, unsigned int stack_size)
{
    int iRet = 0;
    if(NULL == start_address)
    {
        return -1;
    }

	uintptr_t nRet = _beginthreadex(NULL, stack_size, start_address, arg, 0, NULL);
    if(0 == nRet )
    {
        iRet = -1;
    }
	else
	{
		CloseHandle((HANDLE)nRet);
	}

    return iRet;
}
#elif defined LINUX

//**********************************
//Description: 创建兼容WINDOWS和linux的线程
// Parameter:  THREAD_RETURN(*start_address)(void *)  创建线程运行函数的入口地址
//              void*arg 为传入线程使用的参数
// Returns:    int 返回0启动线程成功 非0表示失败
//************************************
int CCompatibleThread::CreateThread(THREAD_RETURN(*start_address)(void *), void *arg)
{
    int iRet = -1;
    if(NULL == start_address)
    {
        return -1;
    }
    
	pthread_attr_t attr;
	pthread_attr_t* pattr = &attr;
	if (0 == pthread_attr_init(pattr))
	{
		if (0 == pthread_attr_setstacksize(pattr,0x600000))
		{
			pthread_t pthId;
			if (0 == pthread_create(&pthId, pattr, start_address, arg))
			{
				iRet = 0;
			}
			pthread_detach(pthId);
		}
		pthread_attr_destroy(pattr);
	}

    return iRet;
}

int CCompatibleThread::CreateThread(THREAD_RETURN(__STDCALL *start_address)(void *), void *arg, unsigned int stack_size)
{
    int iRet = -1;
    if(NULL == start_address)
    {
        return -1;
    }
    
	pthread_attr_t attr;
	pthread_attr_t* pattr = &attr;
	if (0 == pthread_attr_init(pattr))
	{
		if (0 == pthread_attr_setstacksize(pattr, stack_size))
		{
			pthread_t pthId;
			if (0 == pthread_create(&pthId, pattr, start_address, arg))
			{
				iRet = 0;
			}
			pthread_detach(pthId);
		}
		pthread_attr_destroy(pattr);
	}

    return iRet;
}
#endif



