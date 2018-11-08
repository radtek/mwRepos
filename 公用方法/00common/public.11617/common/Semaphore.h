/*************************************************************************
��  �ܣ��ź����࣬ʵ�ּ���/����/���³�ʼ����������
��  ������ʼֵΪ1�ĵ����ź���������ʵ����ͷ�ļ�
��  �ߣ����鲩
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
    //��ʼ���ź���
    int Init(key_t nKey);
    //����
    int Lock();
    //����
    int UnLock();
    //��λ ��ֵΪ1
    int Reset();
    //ɾ���ź���
    void Del();
protected:
    key_t m_nKey;
    int m_nStatus;
    int m_semID;
};

#endif

