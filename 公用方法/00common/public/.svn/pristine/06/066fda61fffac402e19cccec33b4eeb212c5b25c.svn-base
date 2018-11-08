#ifndef _NETTYPES_H_
#define _NETTYPES_H_

#include <vector>
#include <map>
#include <string.h>
#include <time.h>


//#pragma pack(1)
/////////////////////////////////////////////////////////////////////////////////////////////////
//下面这段原则上不允许修改,除非在编译时,为不同的项目编译不同的库
//缓冲大小
//由于不同的程序需要不同的IOCP读取缓冲大小,所以这里根据不同的程序定义不同的宏来定义缓冲大小
//为每个不同的项目添加DLL文件描述(必须填,以用来区分DLL)
//CCS
#define DLL_FILEDESC_CCS "CCS专用IocpServer库\0"
//DFC
#define DLL_FILEDESC_DFC "DFC专用IocpServer库\0"
//STD
#define DLL_FILEDESC_STD "标准IocpServer库\0"

//每次编译时根据不同的项目替换不同的宏(必须填写)
#define DLL_MEMO    DLL_FILEDESC_STD
/////////////////////////////////////////////////////////////////////////////////////////////////

//最大接收缓冲(三个单包大小)
#if defined IOCP_CCS  //CCS专用的大小
	#ifndef  MAX_RECBUF_LEN
		#define  MAX_RECBUF_LEN              32*1024     
	#endif
#elif defined IOCP_DFC //DFC专用大小
	#ifndef  MAX_RECBUF_LEN
		#define  MAX_RECBUF_LEN              30*1024     
	#endif
#else //标准大小
	#ifndef  MAX_RECBUF_LEN
		#define  MAX_RECBUF_LEN              30*1024     
	#endif
#endif

#ifndef  READ_BUFFER_SIZE
#define  READ_BUFFER_SIZE            8*1024      //完成端口一次接收的大小
#endif

#ifndef  EPOLL_BUFFER_SIZE
#define  EPOLL_BUFFER_SIZE            8*1024      //完成端口一次接收的大小
#endif

#if defined IOCP_CCS  //CCS专用的大小
	#ifndef  WRITE_BUFFER_SIZE
		#define  WRITE_BUFFER_SIZE           300*1024      
	#endif
#elif defined IOCP_DFC //DFC专用大小
	#ifndef  WRITE_BUFFER_SIZE
		#define  WRITE_BUFFER_SIZE           32*1024      
	#endif
#else //标准大小
	#ifndef  WRITE_BUFFER_SIZE
		#define  WRITE_BUFFER_SIZE           8*1024      
	#endif
#endif

//鉴权帐号的最大长度
#if defined IOCP_DFC //DFC的帐号长度为16
	#ifndef  MAX_SPID_LEN
		#define  MAX_SPID_LEN                16           
	#endif
#else //标准大小
	#ifndef  MAX_SPID_LEN
		#define  MAX_SPID_LEN                6           
	#endif
#endif

//IP地址最大长度
#ifndef  MAX_IPADDR_LEN
#define  MAX_IPADDR_LEN              18         
#endif

//完成端口一次接收的大小
#ifndef  READ_BUFFER_SIZE
#define  READ_BUFFER_SIZE            8*1024      //完成端口一次接收的大小 IOCP
#endif

#ifndef  EPOLL_BUFFER_SIZE
#define  EPOLL_BUFFER_SIZE            8*1024      //完成端口一次接收的大小 EPOLL
#endif

//一个单包的最大内存值
#ifndef  MAX_PACK_LEN
#define  MAX_PACK_LEN                16*1024 
#endif

#ifdef LINUX
	#ifdef USESPNO
		//通道号长度，epoll用的
		#ifndef  MAX_CHANNEL_LEN
			#define  MAX_CHANNEL_LEN                21
		#endif
	#endif
#endif

//
//标志SOCKET的当前状态
//
enum
{
    SOCKET_STATE_NOT_CONNECT,
		SOCKET_STATE_CONNECT,
		SOCKET_STATE_CONNECT_AND_READ,
		SOCKET_STATE_TIMEOUT,
		SOCKET_WAIT_CLOSE
};

//
//完成端口完成的事件
//
typedef enum
{
    IOCP_COMPLETE_READ,
		IOCP_COMPLETE_ACCEPT_READ,
		IOCP_COMPLETE_WRITE,
		IOCP_COMPLETE_ACCEPT
} IOCP_COMPLETE_STATE;

//
//自定义枚举数据类型，用来标识套接字IO动作类型
//
typedef enum
{
    IOCP_INIT,
		IOCP_ACCEPT, //AcceptEx/accept
		IOCP_CONNECT,
		IOCP_CONNWRITE,
		IOCP_READ,   //WSARecv/recv/ReadFile
		IOCP_WRITE,   //WSASend/send/WriteFile
		IOCP_WRITE_OVER,
		IOCP_READ_OVER,
		IOCP_END
} IOCP_OPERATION, *IOCP_OPERATION_PTR;

//
//自定义枚举数据类型，用来标识套接字IO动作类型
//
typedef enum
{
    EPOLL_INIT,
        EPOLL_ACCEPT,   //accept
        EPOLL_CONNECT,  //connect
        EPOLL_CONNWRITE,
        EPOLL_READ,     //recv
        EPOLL_WRITE,    //send
        EPOLL_WRITE_OVER,
        EPOLL_READ_OVER,
        EPOLL_END
} EPOLL_OPERATION, *EPOLL_OPERATION_PTR;

//socket状态
typedef enum
{
    SOCK_STATE_INIT=0x0000,
		SOCK_STATE_CONNECTING=0x0001,
		SOCK_STATE_CONNECTED=0x0002,
		SOCK_STATE_READED=0x0004,
		SOCK_STATE_LOGINING=0x0008,
		SOCK_STATE_LOGINED=0x0010,
		SOCK_STATE_NEEDCLOSE=0x0020,
		SOCK_STATE_OK=0x0040,
		SOCK_STATE_PENDING=0x0080,
		SOCK_STATE_END=0x0100
} SOCKET_STATE;

//windows下是完成端口
#ifdef WIN32

//完成端口接收缓冲区
struct IOCP_RECV_BUF
{
	unsigned int nRemain;
	char szBuf[MAX_RECBUF_LEN + 1];

	IOCP_RECV_BUF()
	{
		memset(this, 0, sizeof(IOCP_RECV_BUF));
	}

	IOCP_RECV_BUF& operator=(const IOCP_RECV_BUF& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(IOCP_RECV_BUF));
		memcpy(this, &other, sizeof(IOCP_RECV_BUF));
		return *this;
	}
};
typedef IOCP_RECV_BUF* IOCP_RECV_BUF_PTR;

//////////////////////////////////////////////////////////////////////////
//
//单IO数据，扩展的WSAOVERLAPPED
//
struct IOCPIO
{
    WSAOVERLAPPED               ol;
    char                        buf[WRITE_BUFFER_SIZE];
    WSABUF                      wsaBuf;
    SOCKET                      socket;
    IOCP_OPERATION              operation;
    volatile int                state;
    int                         nSockId;
	int							nSeqId;
    IOCPIO()
    {
        memset(this, 0, sizeof(IOCPIO));
    }
};
typedef IOCPIO IOCP_IO;
typedef IOCPIO* IOCP_IO_PTR;

/*
typedef struct
{
    WSAOVERLAPPED               ol;
    char                        buf[BUFFER_SIZE];
    WSABUF                      wsaBuf;
    SOCKET                      socket;
    IOCP_OPERATION              operation;
    volatile int                state;
} IO_SD, *LP_IO_SD;


typedef struct
{
    WSAOVERLAPPED               ol;
    char                        buf[BUFFER_SIZE];
    WSABUF                      wsaBuf;
    SOCKET                      socket;
    IOCP_OPERATION              operation;
    volatile int                state;
} IO_RD, *LP_IO_RD;
*/

//
//自定义结构，即“完成键”(单句柄数据)
//
struct IOCPKEY
{
    SOCKET          socket;
    int             socketid;
    /*TINYINT*/unsigned char   nUsrType;
    int             nUserPrivilege;     //自编流水号权限 0-无，非0-有
    int             nUID;
    time_t          tLastActive;
    /*TINYINT*/unsigned char         nVer;
	/*TINYINT*/unsigned char		   nProtocol;
    int             state;
    UINT            nPort;
    UCHAR           cLogin;             //是否已登陆标志
    UCHAR           szSrcAddr[MAX_SPID_LEN+1];
    char            szIP[MAX_IPADDR_LEN+1];
    int             nSeqId;         //流水号
	IOCP_RECV_BUF   IocpRecvBuf;
	IOCP_IO_PTR		pIo;
    IOCPKEY()
    {
        memset(this, 0, sizeof(IOCPKEY));
        nUsrType = 2;
        cLogin   = 0;
        time(&tLastActive);
    }
};
typedef IOCPKEY IOCP_KEY;
typedef IOCPKEY* IOCP_KEY_PTR;

//回调函数
typedef INT (CALLBACK* DEALRECVCALLBACK)(IOCP_IO_PTR pIo, IOCP_KEY_PTR pKey, int nRead, void* pInvokePtr, BOOL bDisconnect);
//////////////////////////////////////////////////////////////////////////
//linux下是epoll
#elif defined LINUX

//EPOLL接收缓冲区
struct EPOLL_RECV_BUF
{
	unsigned int nRemain;
	char szBuf[MAX_RECBUF_LEN + 1];

	EPOLL_RECV_BUF()
	{
		memset(this, 0, sizeof(EPOLL_RECV_BUF));
	}

	EPOLL_RECV_BUF& operator=(const EPOLL_RECV_BUF& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(EPOLL_RECV_BUF));
		memcpy(this, &other, sizeof(EPOLL_RECV_BUF));
		return *this;
	}
};
typedef EPOLL_RECV_BUF* EPOLL_RECV_BUF_PTR;

//socket状态
enum
{
	S_IDLE,
	S_PENDING,
};
/*User data variable*/
struct epolldata_key
{
	int socketid;                   //文件描述符ID
	int socket;                     //文件描述符
	//void (*call_back)(int socket, int events, void *arg);//事件处理函数
	int events;                     //事件
	void *arg;                      //结构地址
	int status;                     //1:在epoll等待队列 0:不在队列
	int sstate;                     //socket状态
// 	unsigned int nRemain;           //buf中尚留的数据长度
	time_t tLastActive;             //最后一次激活时间
	EPOLL_OPERATION operation;
	unsigned char   nUsrType;
	int             nUserPrivilege;     //自编流水号权限 0-无，非0-有
	int             nUID;
	unsigned char   nVer;
	unsigned char	nProtocol;
	unsigned int    nPort;
	unsigned char   cLogin;             //是否已登陆标志
	unsigned char   szSrcAddr[MAX_SPID_LEN + 1];

#ifdef USESPNO
	char            m_szSpGate[MAX_CHANNEL_LEN + 1];
#endif

	char            szIP[MAX_IPADDR_LEN + 1];
// 	char            m_szTotalRecv[MAX_RECBUF_LEN + 1];     //接收缓冲
	int             nSeqId;         //流水号
	EPOLL_RECV_BUF  epollRecvBuf;

	// 	char*			m_pRecvData;
	// 	BOOL			m_bMultixMtReq;
	// 	BOOL			m_bLarge;
	// 	PLARGE_PACK     m_pLargePack;
#ifdef USESHORTCONN
	BOOL			bKeepConnction;	//是否为长连接
#endif

	epolldata_key()
	{
		memset(this, 0, sizeof(epolldata_key));
		cLogin = 0;
		time(&tLastActive);
	}
};

typedef epolldata_key* IOCP_KEY_PTR;
//回调函数
typedef int(*DEALRECVCALLBACK)(epolldata_key* pKey, int nRead, void* pInvokePtr, bool bDisconnect);
//////////////////////////////////////////////////////////////////////////
#endif
//#pragma pack()
#endif
