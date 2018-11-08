/*************************************************************************
功  能：信号量类，实现加锁/解锁/重新初始化三个函数
描  述：初始值为1的单个信号量操作的实现类头文件
作  者：姜洪博
************************************************************************/
#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

//from unix->linux
#ifdef linux
#define IPC_ALLOC  0
#endif

class CSemaphore
{
public:
    CSemaphore(key_t nKey);
    CSemaphore();
    ~CSemaphore();
    //初始化信号量
    int Init(key_t nKey);
    //加锁
    int Lock();
    //解锁
    int UnLock();
    //复位 赋值为1
    int Reset();
    //删除信号量
    void Del();
protected:
    key_t m_nKey;
    int m_nStatus;
    int m_semID;
};

#endif

