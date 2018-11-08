/*************************************************************************
功  能：共享内存模板类，实现内存申请/访问加锁/读写动作完成后消息的传递
        该模板类暂时实现一个进程写入，一个进程读取共享内存的操作，共享
        内存访问方式类似队列，头取尾插。写进程写入数据后会通过SIGUSR1信
        号通知读进程进行读取。读进程在初始化共享内存时需传入进程ID和信号
        处理函数指针。可以支持SHM_MAX_PIDS个进程的读写操作
作  者：姜洪博
************************************************************************/
#ifndef __SHAREMEMEX_H__
#define __SHAREMEMEX_H__

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
//最大读取进程数
const int SHM_MAX_RD_PIDS  = 1;

//读取访问权限控制
enum
{
    O_W_ONLY=0,     //只写
    O_R_ONLY        //只读
};
enum
{
	O_W=0,
	O_R
};
enum
{
	O_W_OK=0,     //写OK
	O_W_ING=1, 	  //正在写
};
enum
{
	O_SWITCH_OK=0, //切换OK
	O_SWITCH_ING   //正在切换
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

//每个共享内存段私有信息(放置在该段内存头部)
struct ShmPrivEach
{
    size_t nItems;  //共享内存中当前的元素个数
    size_t nWRoffset;//写操作偏移量
	size_t nRDoffset;//读操作偏移量
	volatile bool bWRStatus; //当前的写状态0:写OK 1:正在写
    ShmPrivEach()
    {
        memset(this, 0, sizeof(ShmPrivEach));
		bWRStatus = true;
    }
};

struct ShmMemInfo
{
	char *m_pMem;					//内存头指针
	char *m_pBegin;		 		 	//读取首指针
	char *m_pEnd; 		  			//内存尾指针
	key_t m_tKey;					//共享内存的KEY
	ShmPrivEach *m_pShmPriv;	  	//共享内存头
	int m_iShmId; 		  			//标识该段内存的ID
	ShmMemInfo()
	{
		memset(this, 0, sizeof(ShmMemInfo));
	}
};

//独立与参与共享的内存段之外的一段控制内容，用来控制双缓冲交换
struct ShmCtrl
{
    volatile char cCurRD;//当前读(取值范围0/1)
    volatile char cCurWR;//当前写(取值范围0/1)
	volatile bool bSwitchFlag;//读写缓冲切换标记 0:切换完成/不需切换 1:正在切换
    ShmCtrl()
    {
        memset(this, 0, sizeof(ShmCtrl));
		cCurRD = 255;
		cCurWR = 255;
    }
};

#pragma pack()
//头信息长度(信息区与数据区间的偏移量)
const size_t OFFSET     = 1024*1024*1;
template<class T, size_t shemsize, int optflag>
class CShareMem
{
public:
    CShareMem();
    ~CShareMem();
    //初始化共享内存(O_W_ONLY被指定时将创建共享内存,其他则表示使用该段内存)
    //(O_R_ONLY时需要传入进程PID和信号处理函数地址指针)
	//生成共享内存key的种子,0~100.每段共享内存要不一样,不能重复
    bool InitShareMem(int keyseed=0,pid_t pid=-1, SIGHANDLER sighandler=NULL);
    //向共享内存中添加数据
    bool PushBack(T* pMem);
    //从共享内存中取数据
    bool PopFront(T& Mem);
    //获取共享内存中数据个数
private:
    //交换读写缓冲
    void     SwapRdWr();
private:
	ShmMemInfo m_ShmInfo[2];
	ShmCtrl* m_pShmCtrl;
	key_t m_nShmCtrlKey;	//控制块共享内存的key
	int m_nShmCtrlId;		//控制块共享内存的id
	int m_nFlags; 		  //读/写权限标志
	char* m_pMemCtrl;
	size_t m_nTSize;
	size_t m_nMemSize;
	size_t m_nCapacity;
	sem_t sem_write_over;           //写OK信号量
	sem_t sem_switch_over;          //切换OK信号量
};

template<class T, size_t shemsize, int optflag>
CShareMem<T,shemsize,optflag>::CShareMem()
{
	for (int i = 0; i < 2; ++i)
	{
	    m_ShmInfo[i].m_pShmPriv = NULL;
	    m_ShmInfo[i].m_pMem     = NULL;
		m_ShmInfo[i].m_pBegin   = NULL;
	    m_ShmInfo[i].m_pEnd     = NULL;
	    m_ShmInfo[i].m_iShmId   = -1;
		m_ShmInfo[i].m_tKey   	= -1;
	}
	m_nTSize 	= sizeof(T);
	m_nMemSize 	= shemsize;
	m_nCapacity = (m_nMemSize-OFFSET)/m_nTSize;
	m_nFlags 	= optflag;
	m_pShmCtrl 	= NULL;
	m_pMemCtrl  = NULL;
	m_nShmCtrlKey	=-1;
	m_nShmCtrlId 	=-1;
	sem_init(&sem_write_over,0,0);
	sem_init(&sem_switch_over,0,0);
}

template<class T, size_t shemsize, int optflag>
CShareMem<T,shemsize,optflag>::~CShareMem()
{
	//把WRStatus的状态是正在写的改成已写完
	if (O_W_ONLY == m_nFlags && m_pShmCtrl) 
	{
		ShmPrivEach* p = m_ShmInfo[m_pShmCtrl->cCurWR].m_pShmPriv;
		if (p)
		{
			p->bWRStatus = O_W_OK;
			//发写完成信号
			sem_post(&sem_write_over);
		}
	}
	else if (O_R_ONLY == m_nFlags && m_pShmCtrl)
	{
		//置为切换完成
		m_pShmCtrl->bSwitchFlag = O_SWITCH_OK;
		//发切换完成信号
		sem_post(&sem_switch_over);
	}
}

template<class T, size_t shemsize, int optflag>
bool CShareMem<T,shemsize,optflag>::InitShareMem(int keyseed, pid_t pid, SIGHANDLER sighandler)
{
    bool bRet  = true;

	int i = 0;
	for (i = 0; i < 2; ++i)
	{
		m_ShmInfo[i].m_tKey = ftok("/dev/null", keyseed*100+i);
	}
	
	m_nShmCtrlKey = ftok("/dev/null", keyseed*100+i);

	i = 0;
    m_ShmInfo[i].m_iShmId = shmget(m_ShmInfo[i].m_tKey, m_nMemSize, (O_W_ONLY == m_nFlags)?(IPC_CREAT|0666):IPC_ALLOC);
	if (m_ShmInfo[i].m_iShmId >= 0)
	{
		if ((m_ShmInfo[i].m_pMem = (char*)shmat(m_ShmInfo[i].m_iShmId, NULL, 0)) != (void *)-1)
		{
			m_ShmInfo[i].m_pBegin		= m_ShmInfo[i].m_pMem+OFFSET;
			m_ShmInfo[i].m_pEnd         = m_ShmInfo[i].m_pBegin+(m_nCapacity*m_nTSize);
			m_ShmInfo[i].m_pShmPriv     = (ShmPrivEach*)m_ShmInfo[i].m_pMem;

			++i;
		    m_ShmInfo[i].m_iShmId = shmget(m_ShmInfo[i].m_tKey, m_nMemSize, (O_W_ONLY == m_nFlags)?(IPC_CREAT|0666):IPC_ALLOC);	
			if (m_ShmInfo[i].m_iShmId >= 0)
			{
				if ((m_ShmInfo[i].m_pMem = (char*)shmat(m_ShmInfo[i].m_iShmId, NULL, 0)) != (void *)-1)
				{
					m_ShmInfo[i].m_pBegin		= m_ShmInfo[i].m_pMem+OFFSET;
					m_ShmInfo[i].m_pEnd         = m_ShmInfo[i].m_pBegin+(m_nCapacity*m_nTSize);
					m_ShmInfo[i].m_pShmPriv     = (ShmPrivEach*)m_ShmInfo[i].m_pMem;

					++i;
					m_nShmCtrlId = shmget(m_nShmCtrlKey, OFFSET, (O_W_ONLY == m_nFlags)?(IPC_CREAT|0666):IPC_ALLOC);
					if (m_nShmCtrlId >= 0)
					{
						if ((m_pMemCtrl = (char*)shmat(m_nShmCtrlId, NULL, 0)) != (void *)-1)
						{
							m_pShmCtrl = (ShmCtrl*)m_pMemCtrl;
							if (0xFF == m_pShmCtrl->cCurRD || 0xFF == m_pShmCtrl->cCurWR || m_pShmCtrl->cCurWR == m_pShmCtrl->cCurRD)
							{
								m_pShmCtrl->cCurRD = O_R;
								m_pShmCtrl->cCurWR = O_W;
							}
						}
						else
						{
							printf("%s:shmat_3 error[%d][%s]\r\n", __func__, errno, strerror(errno));
            				bRet = false;
						}
					}
					else
					{
						printf("%s:shmget_3 error[%d][%s]\r\n", __func__, errno, strerror(errno));
    					bRet = false;
					}
				}
				else
				{
					printf("%s:shmat_2 error[%d][%s]\r\n", __func__, errno, strerror(errno));
            		bRet = false;
				}
			}
			else
			{
				printf("%s:shmget_2 error[%d][%s]\r\n", __func__, errno, strerror(errno));
    			bRet = false;
			}
		}
		else
		{
			printf("%s:shmat_1 error[%d][%s]\r\n", __func__, errno, strerror(errno));
            bRet = false;
		}
	}
	else
	{
		printf("%s:shmget_1 error[%d][%s]\r\n", __func__, errno, strerror(errno));
    	bRet = false;
	}
    return bRet;
}

//写共享内存
template<class T, size_t shemsize, int optflag>
bool CShareMem<T,shemsize,optflag>::PushBack(T* pMem)
{
	bool bRet = false;
	if (m_pShmCtrl)
	{
		//等待切换完成
		int n = 0;
		bool bOk = true;
		while (O_SWITCH_ING == m_pShmCtrl->bSwitchFlag)
		{
			struct timespec ts;
			ts.tv_sec = time(NULL) + 1;
			sem_timedwait(&sem_switch_over, &ts);
			if (++n >= 3)
			{
				bOk = false;
				break;
			}
		}
		//切换OK/无需切换
		if (bOk)
		{
			char cCurWR = m_pShmCtrl->cCurWR;
			ShmMemInfo* pShmInfo = &m_ShmInfo[cCurWR];
			ShmPrivEach* p = pShmInfo->m_pShmPriv;
    		if (p)
    		{
    			//先置正在写标志
    			p->bWRStatus = O_W_ING;
				//写。。。。
				char *pShMem = pShmInfo->m_pBegin+p->nWRoffset;
		        //判断内存是否够用
		        if (pShMem+m_nTSize <= pShmInfo->m_pEnd)
		        {
		            //拷入数据
		            memcpy(pShMem, pMem, m_nTSize);
		            //增加个数
		            ++p->nItems;
		            //增加偏移量
		            p->nWRoffset += m_nTSize;

		            bRet = true;
				}
				else
				{
					printf("mem is not enough....\n");
				}
				
				//置写完成标志
				p->bWRStatus = O_W_OK;
				//发写完成信号
				sem_post(&sem_write_over);
    		}
		}
	}

    return bRet;
}

//取共享内存
template<class T, size_t shemsize, int optflag>
bool CShareMem<T,shemsize,optflag>::PopFront(T& Mem)
{
    bool bRet = false;
	char cCurRD = m_pShmCtrl->cCurRD;
	ShmMemInfo* pShmInfo = &m_ShmInfo[cCurRD];
	//先读取，发现无数据，启动切换
	ShmPrivEach* p = pShmInfo->m_pShmPriv;
    //指针有效&&有数据
    if (p)
    {
        char *pShMem = pShmInfo->m_pBegin+p->nRDoffset;
        if (p->nItems > 0)
        {
            //copy内存
            memcpy(&Mem, pShMem, m_nTSize);
            //清空内存
            //memset(pShMem, '\0', m_nTSize);
            //移动读指针
            p->nRDoffset += m_nTSize;
			//减少计数
			--p->nItems;
            bRet = true;
        }
		if (p->nItems <= 0)
		{
			SwapRdWr();
		}
    }
    else
    {
        printf("PopFront null ShmPrivEachPtr \r\n");
    }
    return bRet;
}

//交换读写缓冲
template<class T, size_t shemsize, int optflag>
void CShareMem<T,shemsize,optflag>::SwapRdWr()
{
	if (O_R_ONLY == m_nFlags && m_pShmCtrl)
	{ 
		char cCurWR = m_pShmCtrl->cCurWR;
		ShmMemInfo* pShmInfo = &m_ShmInfo[cCurWR];
		if (pShmInfo && pShmInfo->m_pShmPriv)
		{
			//先置切换标记
			m_pShmCtrl->bSwitchFlag = O_SWITCH_ING;
			//等待当前的写操作完成
			int n = 0;
			bool bOk = true;
			while (O_W_ING == pShmInfo->m_pShmPriv->bWRStatus)
			{
				struct timespec ts;
				ts.tv_sec = time(NULL) + 1;
				sem_timedwait(&sem_write_over, &ts);
				if (++n >= 3)
				{
					bOk = false;
					break;
				}
			}
			
			if (bOk)
			{
				char cCurWR = m_pShmCtrl->cCurWR;
				char cCurRD = m_pShmCtrl->cCurRD;
				//切换
				m_pShmCtrl->cCurWR = cCurRD;
				//写偏移量归0
				m_ShmInfo[m_pShmCtrl->cCurWR].m_pShmPriv->nWRoffset = 0;
				
				m_pShmCtrl->cCurRD = cCurWR;
				//读偏移量归0
				m_ShmInfo[m_pShmCtrl->cCurRD].m_pShmPriv->nRDoffset = 0;
				//置切换OK标记
				m_pShmCtrl->bSwitchFlag = O_SWITCH_OK;
				//发写切换完成信号
				sem_post(&sem_switch_over);
			}
		}
	}
}
#endif

