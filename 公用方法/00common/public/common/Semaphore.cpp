/*************************************************************************
��  �ܣ��ź����࣬ʵ�ּ���/����/���³�ʼ����������
��  ������ʼֵΪ1�ĵ����ź���������ʵ���ļ�
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

    //�Ȼ�ȡ����ȡʧ���򴴽�
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
            //����һ�����̷��� semVal
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

    //����һ�����̷���   semVal
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

    struct sembuf sem_lock={0,-1,/*IPC_NOWAIT|*/SEM_UNDO}; //�����Ľṹ
    int nRet = semop(m_semID,&sem_lock,1);
    if (nRet < 0) //����Ӧ�ø���error��ֵ����
    {
        printf("CSemaphore::Lock semop error, errno=[%d][%s]\r\n", errno, strerror(errno));
        return 1; //���Լ������Լ���
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

    struct sembuf sem_lock={0,1,/*IPC_NOWAIT|*/SEM_UNDO}; //�����ṹ
    int nRet = semop(m_semID,&sem_lock,1);
    if (nRet < 0) //����Ӧ�ø���error��ֵ����
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

