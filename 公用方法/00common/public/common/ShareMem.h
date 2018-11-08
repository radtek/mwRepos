/*************************************************************************
功  能：共享内存模板类，实现内存申请/访问加锁/读写动作完成后消息的传递
        该模板类暂时实现一个进程写入，一个进程读取共享内存的操作，共享
        内存访问方式类似队列，头取尾插。写进程写入数据后会通过SIGUSR1信
        号通知读进程进行读取。读进程在初始化共享内存时需传入进程ID和信号
        处理函数指针。可以支持SHM_MAX_PIDS个进程的读写操作
作  者：姜洪博
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

//信号处理函数
typedef void (*SIGHANDLER)(int, siginfo_t *, void *);

#pragma pack(1)
//最大进程访问数
const int SHM_MAX_PIDS  = 8;

//读取访问权限控制
enum
{
    O_W_ONLY=0,     //只写
    O_R_ONLY        //只读
};
struct ProcPriv
{
    pid_t nPid;             //读取数据的进程PID，若是写数据进程该参数赋-1
    SIGHANDLER sighandler;  //信号处理函数

    ProcPriv()
    {
        sighandler  = NULL;
        nPid        = -1;
    }
};
//共享内存段私有信息(放置在该段内存头部)
struct ShmPriv
{
    size_t nItems;  //共享内存中当前的元素个数(读写共用)
    size_t nROffSet; //当前读指针距内存头的偏移量
    size_t nWOffSet; //当前写指针距内存头的偏移量
    ProcPriv procpriv[SHM_MAX_PIDS];//当前访问该内存的进程

    ShmPriv()
    {
        memset(this, 0, sizeof(ShmPriv));
    }
};
#pragma pack()
//头信息长度(信息区与数据区间的偏移量)
const size_t OFFSET     = sizeof(ShmPriv);
template<class T, size_t shemsize, key_t shemkey>
class CShareMem
{
public:
    CShareMem();
    ~CShareMem();
    //初始化共享内存(O_W_ONLY被指定时将创建共享内存,其他则表示使用该段内存)
    //(O_R_ONLY时需要传入进程PID和信号处理函数地址指针)
    bool InitShareMem(int flags, pid_t pid=-1, SIGHANDLER sighandler=NULL);
    //向共享内存中添加数据
    bool PushBack(T* pMem);
    //从共享内存中取数据
    bool PopFront(T& Mem);
    //获取共享内存中数据个数
    int  GetCount();
    //删除该段共享内存
    bool DelShareMem();
protected:
    //进入该段内存
    ShmPriv* LockShareMem();
    //离开该内存
    bool     UnLockShareMem();
    //获取内存空余大小
    size_t   GetFreeMemSize();
    //通知读取
    int      SignalToRead();
    //设置读进程信息
    int      SetProcPriv(ProcPriv& priv);
    //取读进程信息
    int      GetPorcPriv(int i, ProcPriv& priv);
    char *m_pMem;           //内存头指针
    char *m_pEnd;           //内存尾指针
    ShmPriv *m_pShmPriv;    //共享内存头
    size_t m_nTSize;        //内存要存放的数据类型T的大小
    size_t m_nMemSize;      //申请到的内存空间
    bool m_bLocked;         //内存是否被锁定
    key_t m_tKey;           //申请到的键值
    int m_iShmId;           //标识该段内存的ID
    int m_nFlags;           //读/写权限标志
    int m_nCapacity;        //最大容量
    CSemaphore m_semLock;   //锁
    sem_t sem;              //信号量
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
    m_pEnd     = NULL;
    m_iShmId   = -1;
    m_nCapacity= (m_nMemSize-OFFSET)/m_nTSize;  //计算容量
    sem_init(&sem,0,0);                         //初始化信号量
}

template<class T, size_t shemsize, key_t shemkey>
bool CShareMem<T,shemsize,shemkey>::InitShareMem(int flags, pid_t pid, SIGHANDLER sighandler)
{
    bool bRet  = true;
    //3长度为1000字节,4长度为2000字节,5长度为3000字节, 6长度为5000字节
    m_tKey     = shemkey;
    m_nMemSize = shemsize;
    m_bLocked  = false;
    m_nFlags   = flags;

    //共享内存 使用资源池 IPC_EXCL 如果存在，则出错
    if (O_W_ONLY == flags)
    {
        //如果写，则必须保证互斥访问，所以初始化互斥信号量
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
            //获取信号量
            if (m_semLock.Init(m_tKey) < 0)
            {
                 printf("%s:Init Semaphore error\r\n", __func__);
                 bRet = false;
            }
            else
            {
                m_pEnd                  = m_pMem+shemsize-1;
                m_pShmPriv              = (ShmPriv*)m_pMem;
                ShmPriv* p = LockShareMem();
                if (p)
                {
                    //如果是写操作则初始化头,如果是读操作则不能初始化,只需读取即可
                    if (O_W_ONLY == flags)
                    {
                        if (p->nWOffSet < OFFSET)
                        {
                            p->nWOffSet = OFFSET;
                        }
                        if (p->nItems < 0)
                        {
                            p->nItems  = 0;
                        }
                    }
                    else if (O_R_ONLY == flags)
                    {
                        if (p->nROffSet < OFFSET)
                        {
                            p->nROffSet = OFFSET;
                        }
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
                            //注册消息处理函数
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
CShareMem<T,shemsize,shemkey>::~CShareMem()
{
/*
    if (m_bLocked)
    {
        UnLockShareMem();
    }
    if (m_pMem)
    {
        shmdt(m_pMem);
        m_pMem      = NULL;
        m_pShmPriv  = NULL;
    }
*/
}

template<class T, size_t shemsize, key_t shemkey>
ShmPriv* CShareMem<T,shemsize,shemkey>::LockShareMem()
{
    ShmPriv* p = NULL;
    int nRet = m_semLock.Lock();
    int nCount = 0;
    while (nRet > 0)
    {
        sleep(1);
        nRet = m_semLock.Lock();
        if (++nCount > 10)
        {
            printf("%s:Warnning:can't lock during [%d] seconds...\r\n",__func__, nCount);
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
    bool bRet = false;
    if (!m_semLock.UnLock())
    {
        m_bLocked = false;
        bRet = true;
    }
    return bRet;
}

template<class T, size_t shemsize, key_t shemkey>
size_t CShareMem<T,shemsize,shemkey>::GetFreeMemSize()
{
    return (m_nMemSize-OFFSET-m_pShmPriv->nItems*m_nTSize);
}

//批量通知所有访问该段共享内存的进程
template<class T, size_t shemsize, key_t shemkey>
int CShareMem<T,shemsize,shemkey>::SignalToRead()
{
    int nRet = -1;
    for (int i = 0; i < SHM_MAX_PIDS; ++i)
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

//写共享内存
template<class T, size_t shemsize, key_t shemkey>
bool CShareMem<T,shemsize,shemkey>::PushBack(T* pMem)
{
    assert(m_nFlags == O_W_ONLY);
    bool bRet = false;
    //若共享内存段已被用尽，等待取共享内存线程的取出信号，以保证内存不被覆盖
    /*while*/if (GetCount() >= m_nCapacity)
    {
        struct timespec ts;
        ts.tv_sec = time(NULL) + 5;
        //usleep(100);
        //sem_wait(&sem);
        sem_timedwait(&sem, &ts);
    }
    ShmPriv* p = LockShareMem();
    if (p)
    {
        char *pShMem = m_pMem+OFFSET+p->nWOffSet;
        //判断内存是否够用
        if (pShMem && GetFreeMemSize() >= m_nTSize)
        {
            if (p->nWOffSet == p->nROffSet && p->nItems > 0)
            {
                //sleep(5000);
            }
            //写指针已到尾，偏移量归位
            if (pShMem+m_nTSize > m_pEnd)
            {
                pShMem      = m_pMem+OFFSET;
                p->nWOffSet = OFFSET;
                printf("write end..........................write end\r\n");
            }
            //拷入数据
            memcpy(pShMem, pMem, m_nTSize);
            //增加个数
            ++p->nItems;
            //增加偏移量
            p->nWOffSet += m_nTSize;
            bRet = true;
        }
        else
        {
            printf("no enough memory space...\r\n");
        }
        printf("nItems=%d p->nWOffSet=%d\r\n", p->nItems, p->nWOffSet);
        SignalToRead();
        UnLockShareMem();
    }
    return bRet;
}

//取共享内存
template<class T, size_t shemsize, key_t shemkey>
bool CShareMem<T,shemsize,shemkey>::PopFront(T& Mem)
{
    bool bRet = false;
    ShmPriv* p = LockShareMem();
    //指针有效&&有数据
    if (p)
    {
        char *pShMem = m_pMem+OFFSET+p->nROffSet;
        if (pShMem && p->nItems > 0)
        {
            //读指针已到尾，偏移量归位
            if (pShMem+m_nTSize > m_pEnd)
            {
                pShMem      = m_pMem+OFFSET;
                p->nROffSet = OFFSET;
                printf("read end..........................read end\r\n");
            }
            //copy内存
            memcpy(&Mem, pShMem, m_nTSize);
            //清空内存
            memset(pShMem, '\0', m_nTSize);
            //减少计数
            --p->nItems;
            //移动读指针
            p->nROffSet += m_nTSize;
            bRet = true;
        }
        printf("nItems=%d p->nROffSet=%d\r\n", p->nItems, p->nROffSet);
        UnLockShareMem();
    }
    else
    {
        printf("PopFront lock error \r\n");
        //sleep(5000);
    }
    //发送数据已被取出信号
    sem_post(&sem);
    return bRet;
}

//获取内存中存的数据数
template<class T, size_t shemsize, key_t shemkey>
int CShareMem<T,shemsize,shemkey>::GetCount()
{
    int nCount = 0;
    ShmPriv* p = LockShareMem();
    //指针有效
    if (p)
    {
        nCount = p->nItems;
        UnLockShareMem();
    }
    return nCount;
}

//保存进程私有信息(进程PID以及消息处理函数)
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
            nRet = 0;
            break;
        }
    }
    return nRet;
}

//取进程私有信息
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

//删除该段共享内存
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
    //删除锁
    m_semLock.Del();
    //销毁信号量
    sem_destroy(&sem);
    return bRet;
}
#endif

