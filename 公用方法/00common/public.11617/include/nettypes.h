#ifndef _NETTYPES_H_
#define _NETTYPES_H_

#include <vector>
#include <map>
#include <string>

//�����С
#ifndef  MAX_RECBUF_LEN
#define  MAX_RECBUF_LEN              30*1024     //�����ջ���(����������С)
#endif

#ifndef  BUFFER_SIZE
#define  BUFFER_SIZE                 8*1024      //��ɶ˿�һ�ν��յĴ�С
#endif

#ifndef  MAX_PACK_LEN
#define  MAX_PACK_LEN                16*1024     //10*1024     //һ������������ڴ�ֵ
#endif

#ifndef  MAX_SPID_LEN
#define  MAX_SPID_LEN                6           //��ҵSP������󳤶�(�ֽ�)
#endif

#ifndef  MAX_IPADDR_LEN
#define  MAX_IPADDR_LEN              18          //IP��ַ��󳤶�
#endif

//
//��־SOCKET�ĵ�ǰ״̬
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
//��ɶ˿���ɵ��¼�
//
typedef enum
{
    IOCP_COMPLETE_READ,
		IOCP_COMPLETE_ACCEPT_READ,
		IOCP_COMPLETE_WRITE,
		IOCP_COMPLETE_ACCEPT
} IOCP_COMPLETE_STATE;

//
//�Զ���ö���������ͣ�������ʶ�׽���IO��������
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
//�Զ���ö���������ͣ�������ʶ�׽���IO��������
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

//socket״̬
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

//��ɶ˿ڽ��ջ�����
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

//windows������ɶ˿�
#ifdef WIN32
//////////////////////////////////////////////////////////////////////////
//
//��IO���ݣ���չ��WSAOVERLAPPED
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
//�Զ���ṹ��������ɼ���(���������)
//
struct IOCPKEY
{
    SOCKET          socket;
    int             socketid;
    /*TINYINT*/unsigned char   nUsrType;
    int             nUserPrivilege;     //�Ա���ˮ��Ȩ�� 0-�ޣ���0-��
    int             nUID;
    time_t          tLastActive;
    /*TINYINT*/unsigned char         nVer;
	/*TINYINT*/unsigned char		   nProtocol;
    int             state;
    UINT            nPort;
    UCHAR           cLogin;             //�Ƿ��ѵ�½��־
    UCHAR           szSrcAddr[MAX_SPID_LEN+1];
    char            szIP[MAX_IPADDR_LEN+1];
    int             nSeqId;         //��ˮ��
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

//�ص�����
typedef INT (CALLBACK* DEALRECVCALLBACK)(IOCP_IO_PTR pIo, IOCP_KEY_PTR pKey, int nRead, void* pInvokePtr, BOOL bDisconnect=FALSE);
//////////////////////////////////////////////////////////////////////////
//linux����epoll
#elif defined LINUX
//socket״̬
enum
{
    S_IDLE,
        S_PENDING,
};
/*User data variable*/
struct epolldata_key
{
    int socketid;                   //�ļ�������ID
    int socket;                     //�ļ�������
    //void (*call_back)(int socket, int events, void *arg);//�¼�������
    int events;                     //�¼�
    void *arg;                      //�ṹ��ַ
    int status;                     //1:��epoll�ȴ����� 0:���ڶ���
    int sstate;                     //socket״̬
    unsigned int nRemain;           //buf�����������ݳ���
    time_t tLastActive;             //���һ�μ���ʱ��
    EPOLL_OPERATION operation;
    short           m_nUsrType;
    int             nUserPrivilege;     //�Ա���ˮ��Ȩ�� 0-�ޣ���0-��
    int             m_nUID;
    UCHAR           m_ucVer;
	UCHAR			m_ucProtocol;
    UINT            nPort;
    UCHAR           cLogin;             //�Ƿ��ѵ�½��־
    UCHAR           m_szSrcAddr[MAX_SPID_LEN+1];
    char            m_szSpGate[MAX_CHANNEL_LEN+1];
    char            m_szIP[MAX_IPADDR_LEN+1];
    char            m_szTotalRecv[MAX_RECBUF_LEN+1];     //���ջ���
    int             nSeqId;         //��ˮ��

    epolldata_key()
    {
        memset(this, 0, sizeof(epolldata_key));
        m_nUsrType = TYPE_UNKNOWN;
        cLogin     = 0;
        time(&tLastActive);
    }
};
typedef epolldata_key* IOCP_KEY_PTR;
//�ص�����
typedef int (*DEALRECVCALLBACK)(epolldata_key* pKey, int nRead, void* pInvokePtr, bool bDisconnect);
//////////////////////////////////////////////////////////////////////////
#endif

#endif
