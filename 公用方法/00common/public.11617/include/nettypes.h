#ifndef _NETTYPES_H_
#define _NETTYPES_H_

#include <vector>
#include <map>
#include <string>

//缓冲大小
#ifndef  MAX_RECBUF_LEN
#define  MAX_RECBUF_LEN              30*1024     //最大接收缓冲(三个单包大小)
#endif

#ifndef  BUFFER_SIZE
#define  BUFFER_SIZE                 8*1024      //完成端口一次接收的大小
#endif

#ifndef  MAX_PACK_LEN
#define  MAX_PACK_LEN                16*1024     //10*1024     //一个单包的最大内存值
#endif

#ifndef  MAX_SPID_LEN
#define  MAX_SPID_LEN                6           //企业SP代码最大长度(字节)
#endif

#ifndef  MAX_IPADDR_LEN
#define  MAX_IPADDR_LEN              18          //IP地址最大长度
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

//完成端口接收缓冲区
struct IOCP_RECV_BUF
{
	unsigned int nRemain;
    char szBuf[MAX_RECBUF_LEN+1];
	
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

//windows下是完成端口
#ifdef WIN32
//////////////////////////////////////////////////////////////////////////
//
//单IO数据，扩展的WSAOVERLAPPED
//
struct IOCPIO
{
    WSAOVERLAPPED               ol;
    char                        buf[BUFFER_SIZE];
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
	IOCP_RECV_BUF_PTR pIocpRecvBuf;
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
typedef INT (CALLBACK* DEALRECVCALLBACK)(IOCP_IO_PTR pIo, IOCP_KEY_PTR pKey, int nRead, void* pInvokePtr, BOOL bDisconnect=FALSE);
//////////////////////////////////////////////////////////////////////////
//linux下是epoll
#elif defined LINUX
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
    unsigned int nRemain;           //buf中尚留的数据长度
    time_t tLastActive;             //最后一次激活时间
    EPOLL_OPERATION operation;
    short           m_nUsrType;
    int             nUserPrivilege;     //自编流水号权限 0-无，非0-有
    int             m_nUID;
    UCHAR           m_ucVer;
	UCHAR			m_ucProtocol;
    UINT            nPort;
    UCHAR           cLogin;             //是否已登陆标志
    UCHAR           m_szSrcAddr[MAX_SPID_LEN+1];
    char            m_szSpGate[MAX_CHANNEL_LEN+1];
    char            m_szIP[MAX_IPADDR_LEN+1];
    char            m_szTotalRecv[MAX_RECBUF_LEN+1];     //接收缓冲
    int             nSeqId;         //流水号

    epolldata_key()
    {
        memset(this, 0, sizeof(epolldata_key));
        m_nUsrType = TYPE_UNKNOWN;
        cLogin     = 0;
        time(&tLastActive);
    }
};
typedef epolldata_key* IOCP_KEY_PTR;
//回调函数
typedef int (*DEALRECVCALLBACK)(epolldata_key* pKey, int nRead, void* pInvokePtr, bool bDisconnect);
//////////////////////////////////////////////////////////////////////////
#endif

#endif
