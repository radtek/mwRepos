#ifndef __COMPATIBLETHREAD__H__
#define __COMPATIBLETHREAD__H__

#include "Adapter.h"

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
private:
};


#endif

