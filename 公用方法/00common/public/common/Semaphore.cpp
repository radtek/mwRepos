/*************************************************************************
功  能：信号量类，实现加锁/解锁/重新初始化三个函数
描  述：初始值为1的单个信号量操作的实现文件
************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "errno.h"
#include <iostream>
#include <string.h>
#include "Semaphore.h"

CSemaphore::CSemaphore()
{
    m_nStatus   = -1;
    m_nKey      = -1;
    m_semID     = -1;
}

CSemaphore::CSemaphore(int nKey)
{
    Init(nKey);
}

CSemaphore::~CSemaphore()
{
//    Del();
}

int CSemaphore::Init(key_t nKey)
{
    m_nStatus   = 0;
    m_nKey      = nKey;
    m_semID     = -1;

    //先获取，获取失败则创建
    m_semID = semget(m_nKey,0,IPC_ALLOC);
    if (m_semID < 0)
    {
        m_semID = semget(m_nKey,1,IPC_CREAT|0666);
        if (m_semID < 0)
        {
            m_nStatus = -1;
            printf("CSemaphore::Init semget IPC_CREAT error, errno=[%d][%s]\r\n", errno, strerror(errno));
        }
        else
        {
            //printf("CSemaphore::Init semget IPC_CREAT, Key=[%d]ID=[%d] Semaphore success\r\n", m_nKey, m_semID);
            //运行一个进程访问 semVal
            int nRet = semctl(m_semID,0,SETVAL,1);
            if (nRet < 0)
            {
                m_nStatus = -1;
                printf("CSemaphore::Init semctl SETVAL error, errno=[%d][%s]\r\n", errno, strerror(errno));
            }
        }
    }
    else
    {
        //printf("CSemaphore::Init semget IPC_ALLOC, Key=[%d]ID=[%d] Semaphore success\r\n", m_nKey, m_semID);
    }

    return m_nStatus;
}


int CSemaphore::Reset()
{
    if (m_nStatus != 0 || m_semID < 0)
    {
        printf("CSemaphore::Lock error, this CSemaphore has not been constructed normally\r\n");
        return -1;
    }

    //运行一个进程访问   semVal
    int nRet = semctl(m_semID,0,SETVAL,1);
    if (nRet < 0)
    {
        return -1;
    }

    //printf("CSemaphore::Reset success Reset");
    return 0;
}

int CSemaphore::Lock()
{
    if (m_nStatus != 0 || m_semID < 0)
    {
        printf("CSemaphore::Lock error, this CSemaphore has not been constructed normally\r\n");
        return -1;
    }

    struct sembuf sem_lock={0,-1,/*IPC_NOWAIT|*/SEM_UNDO}; //加锁的结构
    int nRet = semop(m_semID,&sem_lock,1);
    if (nRet < 0) //这里应该根据error的值返回
    {
        printf("CSemaphore::Lock semop error, errno=[%d][%s]\r\n", errno, strerror(errno));
        return 1; //可以继续尝试加锁
    }
    //printf("CSemaphore::Lock success\n");

    return 0;
}

int CSemaphore::UnLock()
{
    if (m_nStatus != 0 || m_semID < 0)
    {
        printf("CSemaphore::UnLock error, this CSemaphore has not been constructed normally\r\n");
        return -1;
    }

    struct sembuf sem_lock={0,1,/*IPC_NOWAIT|*/SEM_UNDO}; //解锁结构
    int nRet = semop(m_semID,&sem_lock,1);
    if (nRet < 0) //这里应该根据error的值返回
    {
        printf("CSemaphore::UnLock semop error, errno=[%d][%s]\r\n", errno, strerror(errno));
        return 1;
    }

    return 0;
}

void CSemaphore::Del()
{
    if (m_semID >= 0 && 0 == m_nStatus)
    {
        if (semctl(m_semID, 0, IPC_RMID, NULL) < 0)
        {
            printf("CSemaphore::Del semctl IPC_RMID error, errno=[%d][%s]\r\n", errno, strerror(errno));
        }
        else
        {
            m_semID     = -1;
            m_nStatus   = -1;
        }
    }
}

