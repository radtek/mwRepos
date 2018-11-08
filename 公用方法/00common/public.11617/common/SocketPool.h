// SocketPool.h: interface for the CSocketPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__SOCKETPOOL_H__)
#define __SOCKETPOOL_H__

#include "./adapter/Adapter.h"
#include "MemListEx.h"

class CSocketPool  
{
public:
	CSocketPool();
	virtual ~CSocketPool();

private:	
	typedef struct
	{   
		char   VN;   
		char   CD;   
		unsigned   short   Port;   
		unsigned   long   IPAddr;   
		char   other[1];   
	}sock4req1;   
	
	typedef struct
	{   
		char   VN;   
		char   CD;   
	}sock4ans1;
	
	typedef struct  
	{
		BYTE Ver;
		BYTE nMethods;
		BYTE Methods[255];
	}REQ1_SOCK5,*LPREQ1_SOCK5;
	typedef struct  TagANS1_SOCK5
	{
		BYTE Ver;	
		BYTE Method;
		TagANS1_SOCK5()
		{
			memset(this, 0, sizeof(ANS1_SOCK5));
		}
	}ANS1_SOCK5,*LPANS1_SOCK5;
	
	
	typedef struct  
	{
		BYTE Ver;	
		BYTE Status;
	}ANS_AUTHEN_SOCK5,*LPANS_AUTHEN_SOCK5;
	
	typedef struct  
	{
		BYTE Ver;
		BYTE Cmd;
		BYTE Rsv;
		BYTE Atyp;
		//BYTE other;
		char other[16];
	}REQ2_SOCK5,*LPREQ2_SOCK5;
	typedef struct  
	{
		BYTE Ver;
		BYTE Rep;
		BYTE nMethods;
		BYTE Methods[2];
	}ANS2_SOCK5,*LPANS2_SOCK5;

private:
	typedef struct	TagSocketKey
	{
		SOCKET hSock;
		time_t tTick;
		TagSocketKey()
		{
			memset(this, 0, sizeof(TagSocketKey));
		}
	}SocketKey, *LPSocketKey;
	
	CMemListEx<SocketKey>	m_socklist;

public:	
	void	SetParams(LPCTSTR lpSvrIP, u_short nSvrPort);
	BOOL	ConnectServer();
	BOOL	TestNetState();
	int		ReadSocket(SOCKET hSock, char *bufRecv, UINT *RecvLen);
	int		WriteSocket(SOCKET hSock, LPCTSTR bufSend, int SendLen);
	SOCKET	GetSocket();
	void	FreeSocket(SOCKET sock);
	void	CloseSocket();

	enum	TWaitFlag{	RECV, SEND };
	int		WaitSomeTime(SOCKET fd, int sec, enum TWaitFlag flags);
	
	SOCKET	ConnectHttpServer(LPCTSTR lpszServer,u_short usPort);
	int  ConnectServerBySocks4Proxy(
								LPCTSTR		lpszServerAddr,
								DWORD		nServerPort,
								LPCTSTR		pszProxyIP,
								DWORD		nProxyPort,
								LPCTSTR		pszProxyAccount,
								LPCTSTR		pszProxyPwd
								);
	int ConnectServerBySocks5Proxy(
								LPCTSTR		lpszServerAddr,
								DWORD		nServerPort,
								LPCTSTR		pszProxyIP,
								DWORD		iProxyPort,
								LPCTSTR		pszProxyAccount,
								LPCTSTR		pszProxyPwd
								);
	int __SetProxyParameter(		
		int			iProxyType,		//0=不使用  1=socks5 2=socks4 3=http(此DLL仅代理3)
		LPCTSTR	lpszProxyAddr,
		int			iProxyPort,
		LPCTSTR	lpszProxyUser,
		LPCTSTR	lpszProxyPwd
		);
	
	void ProxyAuthenMsg(
		LPCTSTR account, 
		LPCTSTR password
		);
	
	int SendMessage(UINT ReSend, LPCTSTR bufSend, int BufLength, LPCTSTR lpszResultBeg, LPCTSTR lpszResultEnd, char* szResult, UINT* ResultLen);

private:
	char	m_szSrvIP[64+1];
	u_short	m_szSrvPort;
	BOOL	m_bUseProxy;				//是否通过代理服务器连接到服务器
	char	m_szProxyServer[64+1];		//代理服务器的IP地址
	int		m_iProxyPort;				//代理服务器的端口号
	char	m_szProxyUser[32+1];		//能够访问代理服务器的用户名
	char	m_szProxyPwd[16+1];			//与上面用户名对应的密码
	char	m_szProxyAuthenMsg[1024];
	int		m_iOvertime;
private:
	char	m_szHttpOk[32+1];
	char	m_szHttpOkEx[32+1];
	char	m_szContentType[64+1];
	char	m_szContentLength[32+1];
	
	char	m_szXmlHead[64+1];
	char	m_szSoapEnvelopeBeg[1024+1];
	char	m_szSoapEnvelopeEnd[32+1];
	char	m_szSoapBodyBeg[32+1];
	char	m_szSoapBodyEnd[32+1];
private:
//错误码
#define RETURN_SUCCESS			0
#define RETURN_NULL_SOCK		-1000
#define RETURN_SEND_ERROR		-1001
#define RETURN_RECV_ERROR		-1002
#define RETURN_RECV_TIME_OUT	-1003
#define RETURN_HTTP_HEAD_ERROR	-1004
#define RETURN_RECV_BUF_OUT		-1005
#define HTTP_HEAD_NO_CONT_LEN	-1006
#define BODY_NO_RESULT_BEG		-1007
#define BODY_NO_RESULT_END		-1008
#define BODY_NO_SOAP_BEG		-1009
#define BODY_NO_SOAP_END		-1010
#define RESULT_IS_ZERO			-1011
#define NOT_INIT_IF				-1012
#define RETURN_PACK_BIG			-1013
#define RETURN_DEST_ERROR		-1014
#define RETURN_MSG_TOO_LONG		-1015
//////////////////////////////////////////////////////////////////////////
#define REQ_MO_NULL_MSG			1001
//////////////////////////////////////////////////////////////////////////
#define	MAX_RVBUF_LEN			(20*1024)
#define	MAX_CALLER				50   //SOCKET连接池
//////////////////////////////////////////////////////////////////////////
private:
	int Base64Encode(unsigned char* pbSrc, unsigned long SrcLen, unsigned char* pbDes)
	{
		const unsigned char base64_alphabet[] = {
			'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
				'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
				'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
				'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/',
				'='
		};
		
		unsigned char* pbOut = pbDes;
		unsigned long dwSrc;
		if( SrcLen<=0 ) return 0;
		
		unsigned long iSrc=0;
		for(iSrc=0;iSrc<SrcLen;iSrc+=3 ){
			
			dwSrc =  *pbSrc++;
			dwSrc <<= 8;
			dwSrc += *pbSrc++;
			dwSrc <<= 8;
			dwSrc += *pbSrc++;
			
			*pbOut++ = base64_alphabet[ (dwSrc>>18) & 0x3F ];
			*pbOut++ = base64_alphabet[ (dwSrc>>12) & 0x3F ];
			*pbOut++ = base64_alphabet[ (dwSrc>>6)  & 0x3F ];
			*pbOut++ = base64_alphabet[ (dwSrc)     & 0x3F ];
			
		}
		
		iSrc -= SrcLen;
		
		if( iSrc==2 ){//需要补2位
			*(pbOut-1) = *(pbOut-2) = '=';
		}else if( iSrc==1 ){//需要补1位
			*(pbOut-1) = '=';
		}
		*pbOut = 0;
		iSrc = (int)(pbOut-pbDes);
		return  iSrc;
	}
};

#endif // !defined(__SOCKETPOOL_H__)
