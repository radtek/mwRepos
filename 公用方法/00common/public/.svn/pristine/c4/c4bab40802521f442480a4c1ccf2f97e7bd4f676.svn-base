#ifndef __COMPATIBLETHREAD__H__
#define __COMPATIBLETHREAD__H__

#include "Adapter.h"

#ifndef uintptr_t
    #ifdef  LINUX
		#ifdef  _X64_
			//typedef unsigned long int    uintptr_t;
		#else
			//typedef unsigned int   uintptr_t;
		#endif
	#else
		#ifdef  _WIN64
			typedef unsigned __int64    uintptr_t;
		#else
			typedef unsigned int   uintptr_t;
		#endif
	#endif
#endif

//兼容WINDOWS和linux的创建线程类
class CCompatibleThread
{
	friend class CAdapter;
public:
    CCompatibleThread();
    ~CCompatibleThread();
private:
    //**********************************
    //Description: 创建兼容WINDOWS和linux的线程
    // Parameter:  THREAD_RETURN(*start_address)(void *)  创建线程运行函数的入口地址
    //              void*arg 为传入线程使用的参数
    // Returns:    int
    //************************************
    static int CreateThread(THREAD_RETURN(__STDCALL *start_address)(void *), void *arg);
    static int CreateThread(THREAD_RETURN(__STDCALL *start_address)(void *), void *arg, unsigned int stack_size);
	  static int CreateThread(THREAD_RETURN(__STDCALL *start_address)(void *), void *arg, pthread_t& nThreadId);
private:
};


#endif

