/************************************************************************
*
* Description : 线程类
*
* Created Date : 2017 / 6 / 1
*
* Copyright(c) ShenZhen Montnets Technology, Inc.All rights reserved.
*
************************************************************************/
#include "utility/common/mwcore_export.h"


namespace UTI{
namespace COM{

#if defined(MW_WIN) || defined(WIN32)
#include   <windows.h>  
#include   <process.h>  
#elif defined(MW_LINUX) || defined(LINUX)
#include <unistd.h>
#include <pthread.h>
void UTILITY_Export Sleep(unsigned long  ulMilliseconds)
{
    sleep(ulMilliseconds/1000);
}
#endif

/************************************************************************
*  功能: 线程基类
************************************************************************/
class UTILITY_Export MThread
{
private:
	explicit MThread(const MThread & rhs);

public:
	MThread();
	~MThread(void);

	/************************************************************************
	*  功能: 开始运行线程
	************************************************************************/
	bool Start();

	/************************************************************************
	*  功能: 运行的线程函数，需使用派生类重写此函数
	************************************************************************/
	virtual void Run() = 0;

	/************************************************************************
	*  功能: 当前执行此函数线程等待线程结束
	*  参数: timeout 等待超时时间，如果为负数，等待无限时长
	************************************************************************/
	void Join(int timeout = -1);

	/************************************************************************
	*  功能: 终止线程的执行
	*  参数: ExitCode 返回的值
	************************************************************************/
	bool Terminate(unsigned long ExitCode);

private:
	/************************************************************************
	*  功能: 线程的执行函数
	*  参数: arg 线程参数，为调用对象指针
	************************************************************************/
#ifdef _WINDOWS
	HANDLE m_handle;
	static unsigned int WINAPI StaticThreadFunc(void * arg);
#else
	pthread_t m_handle;
    static void* StaticThreadFunc(void *arg);
#endif
	/************************************************************************
	*  功能: 线程是否运行
	************************************************************************/
	/*volatile*/ bool m_bRun;
};

}}  //end namespace
