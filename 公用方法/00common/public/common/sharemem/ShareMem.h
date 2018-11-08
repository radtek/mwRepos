/*************************************************************************
��  �ܣ������ڴ�ģ���࣬ʵ���ڴ�����/���ʼ���/��д������ɺ���Ϣ�Ĵ���
        ��ģ������ʱʵ��һ������д�룬һ�����̶�ȡ�����ڴ�Ĳ���������
        �ڴ���ʷ�ʽ���ƶ��У�ͷȡβ�塣д����д�����ݺ��ͨ��SIGUSR1��
        ��֪ͨ�����̽��ж�ȡ���������ڳ�ʼ�������ڴ�ʱ�贫�����ID���ź�
        ������ָ�롣����֧��SHM_MAX_PIDS�����̵Ķ�д����
��  �ߣ����鲩
************************************************************************/
#ifndef __SHAREMEM_H__
#define __SHAREMEM_H__

#include "Semaphore.h"
#include <semaphore.h>
#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include "assert.h"

//�źŴ�����
typedef void (*SIGHANDLER)(int, siginfo_t *, void *);

#pragma pack(1)
//�����̷�����
const int SHM_MAX_PIDS  = 16;

//��ȡ����Ȩ�޿���
enum
{
    O_W_ONLY=0,     //ֻд
    O_R_ONLY        //ֻ��
};
struct ProcPriv
{
    pid_t nPid;             //��ȡ���ݵĽ���PID������д���ݽ��̸ò�����-1
    SIGHANDLER sighandler;  //�źŴ�����

    ProcPriv()
    {
        sighandler  = NULL;
        nPid        = -1;
    }
};
//�����ڴ��˽����Ϣ(�����ڸö��ڴ�ͷ��)
struct ShmPriv
{
    size_t nItems;  //�����ڴ��е�ǰ��Ԫ�ظ���(��д����)
    size_t nROffSet; //��ǰ��ָ����ڴ�ͷ��ƫ����
    size_t nWOffSet; //��ǰдָ����ڴ�ͷ��ƫ����
    ProcPriv procpriv[SHM_MAX_PIDS];//��ǰ���ʸ��ڴ�Ľ���
	size_t nProcCnt; //��ǰ���ʸö��ڴ�Ľ�����
    ShmPriv()
    {
        memset(this, 0, sizeof(ShmPriv));
    }
};
#pragma pack()
//ͷ��Ϣ����(��Ϣ�������������ƫ����)
const size_t OFFSET     = 1024*1024*10;
template<class T, size_t shemsize, key_t shemkey>
class CShareMem
{
public:
    CShareMem();
    ~CShareMem();
    //��ʼ�������ڴ�(O_W_ONLY��ָ��ʱ�����������ڴ�,�������ʾʹ�øö��ڴ�)
    //(O_R_ONLYʱ��Ҫ�������PID���źŴ�������ַָ��)
    bool InitShareMem(int flags, pid_t pid=-1, SIGHANDLER sighandler=NULL);
    //�����ڴ����������
    bool PushBack(T* pMem);
    //�ӹ����ڴ���ȡ����
    bool PopFront(T& Mem);
    //��ȡ�����ڴ������ݸ���
    int  GetCount();
    //ɾ���öι����ڴ�
    bool DelShareMem();
protected:
    //����ö��ڴ�
    ShmPriv* LockShareMem();
    //�뿪���ڴ�
    bool     UnLockShareMem();
    //��ȡ�ڴ�����С
    //size_t   GetFreeMemSize();
    //֪ͨ��ȡ
    int      SignalToRead();
    //���ö�������Ϣ
    int      SetProcPriv(ProcPriv& priv);
    //ȡ��������Ϣ
    int      GetPorcPriv(int i, ProcPriv& priv);
    char *m_pMem;           //�ڴ�ͷָ��
    char *m_pBegin;			//��ȡ��ָ��
    char *m_pEnd;           //�ڴ�βָ��
    ShmPriv *m_pShmPriv;    //�����ڴ�ͷ
    size_t m_nTSize;        //�ڴ�Ҫ��ŵ���������T�Ĵ�С
    size_t m_nMemSize;      //���뵽���ڴ�ռ�
    bool m_bLocked;         //�ڴ��Ƿ�����
    key_t m_tKey;           //���뵽�ļ�ֵ
    int m_iShmId;           //��ʶ�ö��ڴ��ID
    int m_nFlags;           //��/дȨ�ޱ�־
    int m_nCapacity;        //�������
    CSemaphore m_semLock;   //��
    sem_t sem;              //�ź���
    size_t m_nCurProcPos;	//��ǰ�����ڹ����ڴ���б��е��±�
};

template<class T, size_t shemsize, key_t shemkey>
CShareMem<T,shemsize,shemkey>::CShareMem()
{
    m_nTSize   = sizeof(T);
    m_nMemSize = shemsize;
    m_tKey     = shemkey;
    m_bLocked  = false;
    m_pShmPriv = NULL;
    m_pMem     = NULL;
	m_pBegin   = NULL;
    m_pEnd     = NULL;
    m_iShmId   = -1;
	m_nCurProcPos = 0;
    m_nCapacity= (m_nMemSize-OFFSET)/m_nTSize;  //��������
    sem_init(&sem,0,0);                         //��ʼ���ź���
}

template<class T, size_t shemsize, key_t shemkey>
CShareMem<T,shemsize,shemkey>::~CShareMem()
{
	//����Ƕ�ȡ����,�˳�ʱ�����Ѵӷ����б��������
	if (O_R_ONLY == m_nFlags)
	{
		ShmPriv* p = LockShareMem();
		if (p)
		{
	   		if (m_nCurProcPos >= 0 && m_nCurProcPos < SHM_MAX_PIDS)
	    	{
	        	p->procpriv[m_nCurProcPos].nPid = -1;
	    		p->procpriv[m_nCurProcPos].sighandler = NULL;
				--p->nProcCnt;
	    	}
			UnLockShareMem();
		}
	}
}

template<class T, size_t shemsize, key_t shemkey>
bool CShareMem<T,shemsize,shemkey>::InitShareMem(int flags, pid_t pid, SIGHANDLER sighandler)
{
    bool bRet  = true;
    //3����Ϊ1000�ֽ�,4����Ϊ2000�ֽ�,5����Ϊ3000�ֽ�, 6����Ϊ5000�ֽ�
    m_tKey     = shemkey;
    m_nMemSize = shemsize;
    m_bLocked  = false;
    m_nFlags   = flags;

    //�����ڴ� ʹ����Դ�� IPC_EXCL ������ڣ������
    if (O_W_ONLY == flags)
    {
        //���д������뱣֤������ʣ����Գ�ʼ�������ź���
        m_iShmId = shmget(m_tKey, shemsize, IPC_CREAT|0666);
    }
    else if (O_R_ONLY == flags)
    {
        m_iShmId = shmget(m_tKey, shemsize, IPC_ALLOC);
    }

    if (m_iShmId >= 0)
    {
        if ((m_pMem = (char*)shmat(m_iShmId, NULL, 0)) != (void *)-1)
        {
            //��ȡ�ź���
            if (m_semLock.Init(m_tKey) < 0)
            {
                 printf("%s:Init Semaphore error\r\n", __func__);
                 bRet = false;
            }
            else
            {
            	m_pBegin				= m_pMem+OFFSET;
                m_pEnd                  = m_pBegin+(m_nCapacity*m_nTSize);
                m_pShmPriv              = (ShmPriv*)m_pMem;
                ShmPriv* p = LockShareMem();
                if (p)
                {
                    //������Ը�����Ҫ�Ƿ���в���
                    if (O_W_ONLY == flags)
                    {
                    }
                    else if (O_R_ONLY == flags)
                    {
                        ProcPriv priv;
                        priv.nPid       = pid;
                        priv.sighandler = sighandler;
                        if (0 != SetProcPriv(priv))
                        {
                            bRet = false;
                            printf("%s:SetProcPriv error, pid[%d]sighandler[%0x]\r\n", __func__, pid, sighandler);
                        }
                        else
                        {
                            //ע����Ϣ������
                            struct sigaction act;
                            sigemptyset(&act.sa_mask);
                            act.sa_flags     = SA_SIGINFO;
                            act.sa_sigaction = sighandler;
                            if(sigaction(SIGUSR1, &act, NULL))
                            {
                                printf("%s:sigaction SIGUSR1 error, errno[%d][%s]\r\n", __func__, errno, strerror(errno));
                                bRet = false;
                            }
                        }
                    }
                    printf("m_pMem=%0x nItems=%d nROffSet=%d nWOffSet=%d\r\n",
                          m_pMem, m_pShmPriv->nItems, m_pShmPriv->nROffSet, m_pShmPriv->nWOffSet);
                    UnLockShareMem();
                }
                else
                {
                    bRet = false;
                    printf("%s:LockShareMem error\r\n", __func__);
                }
            }
        }
        else
        {
            printf("%s:shmat error[%d][%s]\r\n", __func__, errno, strerror(errno));
            bRet = false;
        }
    }
    else
    {
        printf("%s:shmget error[%d][%s]\r\n", __func__, errno, strerror(errno));
        bRet = false;
    }
    return bRet;
}

template<class T, size_t shemsize, key_t shemkey>
ShmPriv* CShareMem<T,shemsize,shemkey>::LockShareMem()
{
	//return m_pShmPriv;
    ShmPriv* p = NULL;
    int nRet = m_semLock.Lock();
    int nCount = 0;
    while (nRet > 0)
    {
        usleep(1);
        nRet = m_semLock.Lock();
        if (++nCount > 1000*1000*3)
        {
            printf("%s:Warnning:can't lock during [%d] microseconds....\r\n",__func__, nCount);
            break;
        }
    }
    if (0 == nRet)
    {
        m_bLocked = true;
        p = m_pShmPriv;
    }
    else
    {
         printf("%s:Warnning:have locked sharemem but the lock not during locksharemem(bool) create...\r\n",__func__);
    }
    return p;
}

template<class T, size_t shemsize, key_t shemkey>
bool CShareMem<T,shemsize,shemkey>::UnLockShareMem()
{
	//return true;
    bool bRet = false;
    if (!m_semLock.UnLock())
    {
        m_bLocked = false;
        bRet = true;
    }
    return bRet;
}

/*
template<class T, size_t shemsize, key_t shemkey>
size_t CShareMem<T,shemsize,shemkey>::GetFreeMemSize()
{
    return (m_nMemSize-OFFSET-m_pShmPriv->nItems*m_nTSize);
}
*/

//����֪ͨ���з��ʸöι����ڴ�Ľ���
template<class T, size_t shemsize, key_t shemkey>
int CShareMem<T,shemsize,shemkey>::SignalToRead()
{
    int nRet = -1;
    for (int i = 0; i < m_pShmPriv->nProcCnt; ++i)
    {
        if (-1 != m_pShmPriv->procpriv[i].nPid && NULL != m_pShmPriv->procpriv[i].sighandler)
        {
            union sigval val;
            val.sival_int = 0;
            if(0 == sigqueue(m_pShmPriv->procpriv[i].nPid, SIGUSR1, val))
            {
                nRet = 0;
            }
            else
            {
                printf("%s:sigqueue SIGUSR1 error, pid[%d],errno[%d][%s]\r\n", __func__, m_pShmPriv->procpriv[i].nPid, errno, strerror(errno));
                m_pShmPriv->procpriv[i].nPid        = -1;
                m_pShmPriv->procpriv[i].sighandler  = NULL;
            }
        }
    }
    return nRet;
}

//д�����ڴ�
template<class T, size_t shemsize, key_t shemkey>
bool CShareMem<T,shemsize,shemkey>::PushBack(T* pMem)
{
    assert(m_nFlags == O_W_ONLY);
    bool bRet = false;
    //�������ڴ���ѱ��þ����ȴ�ȡ�����ڴ��̵߳�ȡ���źţ��Ա�֤�ڴ治������
	int nWaitCnt = 2;//�ڴ治��ʱ�ȶ��ٴ�
	int nWaitSec = 3;//�ڴ治��ÿ�εȶ�����
	if (0)
    {
MEMNOTENOUGH:

		if (--nWaitCnt < 0) return bRet = false;
		
		printf("mem is not enough....\n");
        struct timespec ts;
        ts.tv_sec = time(NULL) + nWaitSec;
        //usleep(100);
        //sem_wait(&sem);
        sem_timedwait(&sem, &ts);
    }
    ShmPriv* p = LockShareMem();
    if (p)
    {
        char *pShMem = m_pBegin+p->nWOffSet;
        //�ж��ڴ��Ƿ���,�Լ�дָ���Ƿ�д��һȦ�ص��˶�ȡָ��
        if (pShMem+m_nTSize<=m_pEnd)
        {
        	//���д��һȦ,����û�б���ȡ��Ҫ�ȴ������ߺ���д
            if (p->nWOffSet == p->nROffSet && p->nItems > 0)
            {
            	printf("write a circle...\r\n");
            	bRet = false;
            }
			else
			{
	            //��������
	            memcpy(pShMem, pMem, m_nTSize);
	            //���Ӹ���
	            ++p->nItems;
	            //����ƫ����
	            p->nWOffSet += m_nTSize;
				//д��β�Ժ�,��ͷ��д
				if (m_pBegin+p->nWOffSet == m_pEnd)
				{
					p->nWOffSet = 0;
				}
	            bRet = true;
			}
        }
        else
        {
	        bRet = false;
            printf("no enough memory space...\r\n");			
        }
        //printf("nItems=%d p->nWOffSet=%d\r\n", p->nItems, p->nWOffSet);

		//֪ͨ��ȡ
		SignalToRead();
		//����
		UnLockShareMem();
		//������ڴ��ȴ�
		if (!bRet) goto MEMNOTENOUGH;
    }
    return bRet;
}

//ȡ�����ڴ�
template<class T, size_t shemsize, key_t shemkey>
bool CShareMem<T,shemsize,shemkey>::PopFront(T& Mem)
{
    bool bRet = false;
    ShmPriv* p = LockShareMem();
    //ָ����Ч&&������
    if (p)
    {
        char *pShMem = m_pBegin+p->nROffSet;
        if (p->nItems > 0)
        {
            //copy�ڴ�
            memcpy(&Mem, pShMem, m_nTSize);
            //����ڴ�
            //memset(pShMem, '\0', m_nTSize);
            //���ټ���
            --p->nItems;
            //�ƶ���ָ��
            p->nROffSet += m_nTSize;
			//������ĩβ
			if (m_pBegin+p->nROffSet == m_pEnd)
			{
				p->nROffSet = 0;
			}
            bRet = true;
        }
        //printf("nItems=%d p->nROffSet=%d\r\n", p->nItems, p->nROffSet);
        UnLockShareMem();
    }
    else
    {
        printf("PopFront lock error \r\n");
        //sleep(5000);
    }
    //���������ѱ�ȡ���ź�
    sem_post(&sem);
    return bRet;
}

//��ȡ�ڴ��д��������
template<class T, size_t shemsize, key_t shemkey>
int CShareMem<T,shemsize,shemkey>::GetCount()
{
/*
    int nCount = 0;
    ShmPriv* p = LockShareMem();
    //ָ����Ч
    if (p)
    {
        nCount = p->nItems;
        UnLockShareMem();
    }
    return nCount;
*/
	return m_pShmPriv?m_pShmPriv->nItems:0;
}

//�������˽����Ϣ(����PID�Լ���Ϣ������)
template<class T, size_t shemsize, key_t shemkey>
int CShareMem<T,shemsize,shemkey>::SetProcPriv(ProcPriv& priv)
{
    int nRet = -1;
    for (int i = 0; i < SHM_MAX_PIDS; ++i)
    {
        if (m_pShmPriv->procpriv[i].nPid <= 0)
        {
            m_pShmPriv->procpriv[i].nPid        = priv.nPid;
            m_pShmPriv->procpriv[i].sighandler  = priv.sighandler;
			++m_pShmPriv->nProcCnt;
			m_nCurProcPos = i;
            nRet = 0;
            break;
        }
    }
    return nRet;
}

//ȡ����˽����Ϣ
template<class T, size_t shemsize, key_t shemkey>
int CShareMem<T,shemsize,shemkey>::GetPorcPriv(int i, ProcPriv& priv)
{
    int nRet = 0;
    if (i < 0 || i >= SHM_MAX_PIDS)
    {
        nRet = -1;
    }
    priv.nPid       = m_pShmPriv->procpriv[i].nPid;
    priv.sighandler = m_pShmPriv->procpriv[i].sighandler;
    return nRet;
}

//ɾ���öι����ڴ�
template<class T, size_t shemsize, key_t shemkey>
bool CShareMem<T,shemsize,shemkey>::DelShareMem()
{
    bool bRet = true;
    if (m_bLocked)
    {
        UnLockShareMem();
    }
    ShmPriv* p = LockShareMem();
    if (p)
    {
        shmdt(m_pMem);
        m_pMem      = NULL;
        m_pShmPriv  = NULL;
    }
    UnLockShareMem();
    if (0 != shmctl(m_iShmId, IPC_RMID, NULL))
    {
        printf("shmctl IPC_RMID failed...shmid=%d\r\n", m_iShmId);
        bRet = false;
    }
    //ɾ����
    m_semLock.Del();
    //�����ź���
    sem_destroy(&sem);
    return bRet;
}
#endif

