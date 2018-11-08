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

//����WINDOWS��linux�Ĵ����߳���
class CCompatibleThread
{
	friend class CAdapter;
public:
    CCompatibleThread();
    ~CCompatibleThread();
private:
    //**********************************
    //Description: ��������WINDOWS��linux���߳�
    // Parameter:  THREAD_RETURN(*start_address)(void *)  �����߳����к�������ڵ�ַ
    //              void*arg Ϊ�����߳�ʹ�õĲ���
    // Returns:    int
    //************************************
    static int CreateThread(THREAD_RETURN(__STDCALL *start_address)(void *), void *arg);
    static int CreateThread(THREAD_RETURN(__STDCALL *start_address)(void *), void *arg, unsigned int stack_size);
	  static int CreateThread(THREAD_RETURN(__STDCALL *start_address)(void *), void *arg, pthread_t& nThreadId);
private:
};


#endif

