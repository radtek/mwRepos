// SocketPool.cpp: implementation of the CSocketPool class.
//
//////////////////////////////////////////////////////////////////////
#include "SocketPool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef maxx
#define maxx(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef minn
#define minn(a,b)            (((a) < (b)) ? (a) : (b))
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketPool::CSocketPool()
{
	memset(m_szSrvIP, 0, sizeof(m_szSrvIP));
	m_szSrvPort	= 8080;
	
	m_bUseProxy = FALSE;
	memset(m_szProxyServer, 0, sizeof(m_szProxyServer));
	memset(m_szProxyUser, 0, sizeof(m_szProxyUser));
	memset(m_szProxyPwd, 0, sizeof(m_szProxyPwd));
	memset(m_szProxyAuthenMsg, 0, sizeof(m_szProxyAuthenMsg));	
	m_iProxyPort = 80;
	m_iOvertime = 60;
	//////////////////////////////////////////////////////////////////////////	
	memset(m_szHttpOk, 0, sizeof(m_szHttpOk));
	sprintf(m_szHttpOk, "HTTP/1.1 200");

	memset(m_szHttpOkEx, 0, sizeof(m_szHttpOkEx));
	sprintf(m_szHttpOkEx, "HTTP/1.0 200");
	
	memset(m_szContentType, 0, sizeof(m_szContentType));
	sprintf(m_szContentType, "Content-Type: text/xml; charset=utf-8");
	
	memset(m_szContentLength, 0, sizeof(m_szContentLength));
	sprintf(m_szContentLength, "Content-Length:");

	memset(m_szXmlHead, 0, sizeof(m_szXmlHead));
	sprintf(m_szXmlHead, "<\?xml version=\"1.0\" encoding=\"UTF-8\"\?>");

	memset(m_szSoapEnvelopeBeg, 0, sizeof(m_szSoapEnvelopeBeg));
	sprintf(m_szSoapEnvelopeBeg, "<soap:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">");

	memset(m_szSoapEnvelopeEnd, 0, sizeof(m_szSoapEnvelopeEnd));
	sprintf(m_szSoapEnvelopeEnd, "</soap:Envelope>");

	memset(m_szSoapBodyBeg, 0, sizeof(m_szSoapBodyBeg));
	sprintf(m_szSoapBodyBeg, "<soap:Body>");

	memset(m_szSoapBodyEnd, 0, sizeof(m_szSoapBodyEnd));
	sprintf(m_szSoapBodyEnd, "</soap:Body>");
}

CSocketPool::~CSocketPool()
{

}

void CSocketPool::SetParams(LPCTSTR lpSvrIP, unsigned short nSvrPort)
{
	if (NULL==lpSvrIP || strlen(lpSvrIP)<=0 || nSvrPort<=0) 
	{
		return;
	}
	memset(m_szSrvIP, 0, sizeof(m_szSrvIP));
	strncpy(m_szSrvIP, lpSvrIP, sizeof(m_szSrvIP)-1);
	m_szSrvPort = nSvrPort;
}

int CSocketPool::ReadSocket(SOCKET hSock, char *bufRecv, UINT *RecvLen)
{
	int iRet = 0;
	if (INVALID_SOCKET != hSock )
	{
		iRet = WaitSomeTime( hSock, 60, RECV );
		if ( iRet < 1 )
		{
			DWORD err = 0;
#ifdef WIN32
			err = GetLastError();
#endif
			CAdapter::closesocket( hSock );
			hSock = INVALID_SOCKET;
			return (-1)*(104*100000 + err);
		}
		iRet = recv( hSock, bufRecv, *RecvLen, 0);
		if ( iRet == SOCKET_ERROR || iRet == 0 )
		{
			DWORD err = 0;
#ifdef WIN32
			err = GetLastError();
#endif
			CAdapter::closesocket( hSock );
			hSock = INVALID_SOCKET;
			return (-1)*(105*100000 + err);
		}
		else
		{
			*RecvLen = iRet;
			bufRecv[*RecvLen] = 0;
			iRet = RETURN_SUCCESS;
		}
	}
	else
	{
		CAdapter::Sleep(2000);
		iRet = RETURN_NULL_SOCK;		
	}
	return iRet;
}

int CSocketPool::WriteSocket(SOCKET hSock, LPCTSTR bufSend, int SendLen)
{
	int iRet = 0;
	if (INVALID_SOCKET != hSock )
	{
		iRet = send( hSock, bufSend, SendLen, 0);
		if ( iRet == SOCKET_ERROR || iRet == 0 )
		{
			DWORD err = 0;
#ifdef WIN32
			err = GetLastError();
#endif
			CAdapter::closesocket( hSock );
			hSock = INVALID_SOCKET;
			iRet = (-1)*(106*100000 + err);
		}
		else
		{
			iRet = RETURN_SUCCESS;
		}
	}
	else
	{
		CAdapter::Sleep(2000);
		iRet = RETURN_NULL_SOCK;
	}
	return iRet;
}

SOCKET CSocketPool::GetSocket()
{
	SOCKET hSock = INVALID_SOCKET;
	while (m_socklist.GetCount() > 0)
	{
		int ierr = -1;
		SocketKey sockkey = m_socklist.GetHead(ierr);
		if (0 == ierr)
		{
			time_t tLast = sockkey.tTick;
			time_t tNow = CAdapter::GetTickCount();
			if (INVALID_SOCKET != sockkey.hSock && (tNow-tLast) < 90*1000)
			{
				hSock = sockkey.hSock;
				break;
			}
		}
		else
		{
			break;
		}
	}

	if (INVALID_SOCKET == hSock)
	{
		//网络连接
		if (0 == m_bUseProxy)	//非代理
		{
			hSock = ConnectHttpServer( m_szSrvIP, m_szSrvPort);
		}
		else if (1 == m_bUseProxy)	//sock5
		{
			hSock = ConnectServerBySocks5Proxy( m_szSrvIP, (DWORD)m_szSrvPort, m_szProxyServer, m_iProxyPort, m_szProxyUser, m_szProxyPwd );
		}
		else if (2 == m_bUseProxy)	//sock4
		{
			hSock = ConnectServerBySocks4Proxy( m_szSrvIP, (DWORD)m_szSrvPort, m_szProxyServer, m_iProxyPort, m_szProxyUser, m_szProxyPwd );
		}
		else if (3 == m_bUseProxy)	//http
		{
			hSock = ConnectHttpServer( m_szProxyServer, m_iProxyPort);
		}
		else
		{
			hSock = ConnectHttpServer( m_szSrvIP, m_szSrvPort);
		}	
	}
	return hSock;
}

void CSocketPool::FreeSocket(SOCKET sk)
{
	SocketKey sockkey;
	sockkey.hSock = sk;
	sockkey.tTick = CAdapter::GetTickCount();
	m_socklist.AddTail(0, sockkey);
}

void CSocketPool::CloseSocket()
{
	while (m_socklist.GetCount() > 0)
	{
		int ierr = 0;
		SocketKey sockkey = m_socklist.GetHead(ierr);
		if (0 == ierr)
		{
			CAdapter::closesocket(sockkey.hSock);
		}
		else
		{
			break;
		}		
	}
}

SOCKET CSocketPool::ConnectHttpServer(LPCTSTR lpszServer,unsigned short usPort)
{
	SOCKET hSock;
	struct sockaddr_in their_addr; // connector's address information 
	struct hostent *he;	
	if ((he = gethostbyname(lpszServer)) == NULL)  // get the host info
	{ 
		//DbgMessageBox("1001\r\n","note");
		return INVALID_SOCKET;
	}	
	if ((hSock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		//DbgMessageBox("1002\r\n","note");
		return INVALID_SOCKET;
	}
	their_addr.sin_family = AF_INET; // host byte order
	their_addr.sin_port = htons( usPort ); // short, network byte order
	their_addr.sin_addr = *((struct in_addr *)he->h_addr); 	
	memset(their_addr.sin_zero,0,sizeof(their_addr.sin_zero)); // zero the rest of the struct 
	if (connect(hSock, (struct sockaddr *)&their_addr,sizeof(struct sockaddr)) == -1)
	{ 
		CAdapter::closesocket( hSock );	
		hSock = INVALID_SOCKET;
		//DbgMessageBox("1003\r\n", "note");
	}		
	return hSock;
}

int  CSocketPool::ConnectServerBySocks4Proxy(
								LPCTSTR lpszServerAddr,
								DWORD   nServerPort,
								LPCTSTR pszProxyIP,
								DWORD	nProxyPort,
								LPCTSTR pszProxyAccount,
								LPCTSTR pszProxyPwd
								)
{
	char buf[1024]={0},httpbuf[1025]={0},reg[100]={0};
	int cbRecv = 0;
	int cbSend = 0;
	struct hostent* pHostInfo = NULL;
	struct sockaddr_in svrAddr;
	
	int nUserLen = strlen(pszProxyAccount); 
	int nPassLen = strlen(pszProxyPwd); 
	DWORD tmpLong = inet_addr(lpszServerAddr);
	unsigned short port = ntohs((unsigned short)nServerPort);
	SOCKET sock = 0;
	
	if (lpszServerAddr == NULL || pszProxyIP == NULL || pszProxyAccount == NULL || pszProxyPwd == NULL )
		return -1;
	
	if (strlen(pszProxyAccount) > 64 || strlen(pszProxyPwd) > 64)
		return -2;
	
	pHostInfo = gethostbyname(pszProxyIP);
	if (pHostInfo == NULL)
	{
		pHostInfo = gethostbyaddr(pszProxyIP, strlen(pszProxyIP), AF_INET);
		if (pHostInfo == NULL)
		{
			return -3;
		}
	}
	svrAddr.sin_family = AF_INET;
	memcpy(&svrAddr.sin_addr, pHostInfo->h_addr_list[0], pHostInfo->h_length);
	svrAddr.sin_port = htons((unsigned short)nProxyPort);
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)   
		return -4;
	
	if (connect(sock,(struct sockaddr*)&svrAddr, sizeof(svrAddr)) ==-1)
		return -5;
	
	memset(buf, 0, sizeof(buf));
	sock4req1 *m_proxyreq;   
	m_proxyreq = (sock4req1 *)buf;   
	m_proxyreq-> VN = 4;   
	m_proxyreq-> CD = 1;   
	m_proxyreq-> Port = ntohs((unsigned short)nServerPort);   
	m_proxyreq-> IPAddr = inet_addr(lpszServerAddr);   
	if (WaitSomeTime(sock, m_iOvertime, SEND) ==1)
	{
		cbSend = send(sock, buf, 9, 0);
	}
	else
	{
		return -101;
	}
	if (cbSend < 1){
		CAdapter::closesocket(sock);
		return -102;
	}
	
	sock4ans1 *m_proxyans;   
	m_proxyans = (sock4ans1 *)buf;   
	memset(buf, 0, sizeof(buf)); 
	if (WaitSomeTime(sock, m_iOvertime, RECV) == 1)
	{
		cbRecv = recv(sock, buf, 100, 0);
	}
	else
	{
		CAdapter::closesocket(sock);
		return (-103); 
	}

	if (cbRecv<1) 
	{
		return (-104);
	}

	if(m_proxyans-> VN != 0 || m_proxyans-> CD != 90)
	//if(m_proxyans-> VN != 0 || m_proxyans-> CD != 93)	//CCProxy
	{   
		CAdapter::closesocket(sock); 
		return (-6); 
	}
	return (sock);
}

int CSocketPool::ConnectServerBySocks5Proxy(
								LPCTSTR lpszServerAddr,
								DWORD   nServerPort,
								LPCTSTR pszProxyIP,
								DWORD	iProxyPort,
								LPCTSTR pszProxyAccount,
								LPCTSTR pszProxyPwd
								)
{
	char buf[1024]={0},httpbuf[1025]={0},reg[100]={0};
	int cbRecv = 0;
	int cbSend = 0;
	struct hostent* pHostInfo = NULL;
	struct sockaddr_in svrAddr;
	
	LPREQ1_SOCK5 m_proxyreq1; 
	LPREQ2_SOCK5 m_proxyreq2; 
	LPANS1_SOCK5 m_proxyans1; 
	LPANS2_SOCK5 m_proxyans2; 	
	LPANS_AUTHEN_SOCK5 m_authans;
		
	int nUserLen = strlen(pszProxyAccount); 
	int nPassLen = strlen(pszProxyPwd); 
	DWORD tmpLong = inet_addr(lpszServerAddr);
	unsigned short port = ntohs((unsigned short)nServerPort);
	SOCKET sock = 0;
	
	if (lpszServerAddr == NULL || pszProxyIP == NULL || pszProxyAccount == NULL || pszProxyPwd == NULL )
		return -1;
	
	if (strlen(pszProxyAccount) > 64 || strlen(pszProxyPwd) > 64)
		return -2;
	
	pHostInfo = gethostbyname(pszProxyIP);
	if (pHostInfo == NULL)
	{
		pHostInfo = gethostbyaddr(pszProxyIP, strlen(pszProxyIP), AF_INET);
		if (pHostInfo == NULL)
		{
			return -3;
		}
	}
	svrAddr.sin_family = AF_INET;
	memcpy(&svrAddr.sin_addr, pHostInfo->h_addr_list[0], pHostInfo->h_length);
	svrAddr.sin_port = htons((unsigned short)iProxyPort);
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)   
		return -4;
	
	if (connect(sock,(struct sockaddr*)&svrAddr, sizeof(svrAddr)) ==-1)
		return -5;
	
	m_proxyreq1 = (LPREQ1_SOCK5)buf; 
	m_proxyreq1->Ver = 5; 
	m_proxyreq1->nMethods = 2;
	m_proxyreq1->Methods[0] = 0; 
	m_proxyreq1->Methods[1] = 2;

	if (WaitSomeTime(sock, m_iOvertime, SEND) ==1)
	{
		cbSend = send(sock, buf, 4, 0);
	}
	else
	{
		return -101;
	}
	if (cbSend < 1){
		CAdapter::closesocket(sock);
		return -102;
	}
	
	m_proxyans1 = (LPANS1_SOCK5)buf; 
	memset(buf, 0, sizeof(buf));
	if (WaitSomeTime(sock, m_iOvertime, RECV) == 1)
		cbRecv = recv(sock, buf, 100,0);  
	else{
		CAdapter::closesocket(sock);
		return -103; 
	}
	if (cbRecv<1) {
		return -104;
	}
	
	if(m_proxyans1->Ver != 5||(m_proxyans1->Method!=0 && m_proxyans1->Method!=2)) 	
	{ 	
		CAdapter::closesocket(sock);
		return -6; 
	}
	if(m_proxyans1->Method == 2) 	
	{ 	  
		memset(buf,0,sizeof(buf));
		sprintf((char *)buf,"  %s %s",pszProxyAccount,pszProxyPwd);
		buf[0]=1;
		buf[1]=nUserLen;
		buf[2+nUserLen]=nPassLen;
		cbSend = send(sock, buf,strlen(buf), 0);
		if (cbSend < 1){
			CAdapter::closesocket(sock);
			return -110;
		}
		m_authans = (LPANS_AUTHEN_SOCK5)buf; 
		memset(buf,0,sizeof(buf)); 
		if (WaitSomeTime(sock, m_iOvertime, RECV) == 1)
			cbRecv = recv(sock, buf, 100,0);  
		else{
			CAdapter::closesocket(sock);
			return -111; 
		}
		
		if(m_authans->Ver != 1 || m_authans->Status != 0)
		{ 
			CAdapter::closesocket(sock);		  
			return -112;  
		} 
	} 
	
	m_proxyreq2 = (LPREQ2_SOCK5)buf; 
	m_proxyreq2->Ver = 5; 
	m_proxyreq2->Cmd = 1; 
	m_proxyreq2->Rsv = 0; 
	m_proxyreq2->Atyp = 1;
	memcpy(m_proxyreq2->other,&tmpLong,4); 
	memcpy(m_proxyreq2->other+4,&port,2);
	cbSend = send(sock, buf,4+4+2, 0);
	if (cbSend < 1){
		CAdapter::closesocket(sock);
		return -113;
	}
	
	m_proxyans2 = (LPANS2_SOCK5)buf; 	
	memset(buf,0,sizeof(buf)); 	
	if (WaitSomeTime(sock, m_iOvertime, RECV) == 1)
		cbRecv = recv(sock, buf, 100,0);  
	else{
		CAdapter::closesocket(sock);
		return -114; 
	}
	
	if(m_proxyans2->Ver != 5 || m_proxyans2->Rep != 0) 
	{ 
		CAdapter::closesocket(sock);
		return -115;
	}
	
	return (sock);
}

BOOL CSocketPool::ConnectServer()
{
	SOCKET hSocket = GetSocket();
	if( INVALID_SOCKET != hSocket && (int)hSocket > 0 )
	{
		FreeSocket(hSocket);
		return TRUE;
	}
	return FALSE;
}

BOOL CSocketPool::TestNetState()
{
	SOCKET hSock = INVALID_SOCKET;
	
	if (0 == m_bUseProxy)	//非代理
	{
		hSock = ConnectHttpServer( m_szSrvIP, m_szSrvPort);
	}
	else if (1 == m_bUseProxy)	//sock5
	{
		hSock = ConnectServerBySocks5Proxy( m_szSrvIP, (DWORD)m_szSrvPort, m_szProxyServer, m_iProxyPort, m_szProxyUser, m_szProxyPwd );
	}
	else if (2 == m_bUseProxy)	//sock4
	{
		hSock = ConnectServerBySocks4Proxy( m_szSrvIP, (DWORD)m_szSrvPort, m_szProxyServer, m_iProxyPort, m_szProxyUser, m_szProxyPwd );
	}
	else if (3 == m_bUseProxy)	//http
	{
		hSock = ConnectHttpServer( m_szProxyServer, m_iProxyPort);
	}
	else
	{
		hSock = ConnectHttpServer( m_szSrvIP, m_szSrvPort);
	}

	if (INVALID_SOCKET == hSock || SOCKET_ERROR == hSock)
	{
		return FALSE;
	} 
	else
	{
		CAdapter::closesocket(hSock);
		return TRUE;
	}
}

int CSocketPool::WaitSomeTime(SOCKET fd, int sec, enum TWaitFlag flags) 
{
	fd_set rwset; // = {1,{fd}};
	struct timeval timeout = {sec,0};
	int iret;	
	
	FD_SET(fd, &rwset);

	if (flags == RECV)
	{
		iret=(select(fd+1, &rwset, NULL, NULL, &timeout));
	}
	else if (flags == SEND)
	{ 
		iret=(select(fd+1, NULL, &rwset, NULL, &timeout));
	}
	else
	{
		iret=-1;
	}
	return iret;
}

int CSocketPool::__SetProxyParameter(
								   int		iProxyType,		//0=不使用  1=socks5 2=socks4 3=http(此DLL仅代理3)
								   LPCTSTR	lpszProxyAddr,
								   int		iProxyPort,
								   LPCTSTR	lpszProxyUser,
								   LPCTSTR	lpszProxyPwd)
{	
	m_bUseProxy = (BOOL)iProxyType;
	if( m_bUseProxy )
	{		
		if( lpszProxyAddr )
		{
			strncpy(m_szProxyServer, lpszProxyAddr, sizeof(m_szProxyServer) );
			m_szProxyServer[sizeof(m_szProxyServer)-1] = 0;
		}
		
		m_iProxyPort = iProxyPort;
		
		if( lpszProxyUser )
		{
			memcpy( m_szProxyUser, lpszProxyUser, sizeof(m_szProxyUser) );
			m_szProxyUser[sizeof(m_szProxyUser)-1] = 0;
		}
		
		if( lpszProxyPwd )
		{
			memcpy( m_szProxyPwd, lpszProxyPwd, sizeof(m_szProxyPwd) );
			m_szProxyPwd[sizeof(m_szProxyPwd)-1] = 0;
		}
		
		if (3 == iProxyType)
		{
			ProxyAuthenMsg(lpszProxyUser, lpszProxyPwd);
		}
		return 0;
	}
	else
	{
		m_bUseProxy = FALSE;
		return (-1);
	}	
}

void CSocketPool::ProxyAuthenMsg(LPCTSTR account, LPCTSTR password)
{
	char src[512] = {0};
	UINT nLen = sprintf(src, "%s:%s",account,password);
	char dst[512] = {0};
	Base64Encode((UCHAR*)src, nLen, (UCHAR*)dst);
	if (strlen(account) > 0)
	{
		sprintf(m_szProxyAuthenMsg,"Proxy-Authorization: Basic %s\r\n", dst);
	}	
}

int CSocketPool::SendMessage(UINT ReSend, LPCTSTR bufSend, int BufLength, LPCTSTR lpszResultBeg, LPCTSTR lpszResultEnd, char* szResult, UINT* ResultLen)
{
	int iRet = 0;
	char *RecvBuf = new char[2*MAX_RVBUF_LEN+1];
	UINT nRecvLen = 0;
	
	if (!RecvBuf)
	{
		return (-1);
	}
	memset(RecvBuf, 0, MAX_RVBUF_LEN+1);
	
	//发送信息	
	SOCKET hSock = INVALID_SOCKET;
	for (UINT i=0; i<ReSend; i++)
	{
		hSock = GetSocket();
		if (RETURN_SUCCESS == WriteSocket(hSock, bufSend, BufLength) )
		{
			UINT nReadLen = MAX_RVBUF_LEN - nRecvLen;
			iRet = ReadSocket(hSock, RecvBuf+nRecvLen, &nReadLen);
			if (RETURN_SUCCESS == iRet)
			{
				//接收成功
				nRecvLen += nReadLen;
				RecvBuf[nRecvLen] = 0;
				break;
			}
		}
	}	
	if (RETURN_SUCCESS != iRet)
	{
		delete [] RecvBuf;
		RecvBuf = NULL;
		return iRet;
	}
	
	//接收HTTP头
	while (nRecvLen < 15)
	{
		UINT nReadLen = MAX_RVBUF_LEN - nRecvLen;
		iRet = ReadSocket(hSock, RecvBuf+nRecvLen, &nReadLen);
		if (RETURN_SUCCESS != iRet)
		{
			delete [] RecvBuf;
			RecvBuf = NULL;
			return iRet;
		}
		nRecvLen += nReadLen;
		RecvBuf[nRecvLen] = 0;
	}
	
	//判断HTTP 200 OK
	if ( NULL == strstr( RecvBuf, m_szHttpOk) && NULL == strstr( RecvBuf, m_szHttpOkEx) )
	{
		CAdapter::closesocket( hSock );
		hSock = INVALID_SOCKET;
		delete [] RecvBuf;
		RecvBuf = NULL;
		return RETURN_HTTP_HEAD_ERROR;
	}
	
	//接收\r\n\r\n
	char *pContentBeg = NULL;
	char *pContentEnd = NULL;
	while ( NULL == (pContentBeg = strstr(RecvBuf, "\r\n\r\n")) )
	{
		UINT nReadLen = MAX_RVBUF_LEN - nRecvLen;
		iRet = ReadSocket(hSock, RecvBuf+nRecvLen, &nReadLen);
		if (RETURN_SUCCESS != iRet)
		{
			delete [] RecvBuf;
			RecvBuf = NULL;
			return iRet;
		}
		nRecvLen += nReadLen;
		RecvBuf[nRecvLen] = 0;
	}
	pContentBeg += 4; //\r\n\r\n
	
	//解析Http报文正文长度
	char *pContextLen = NULL;
	int nContentLen = 0;	
	if ( NULL == (pContextLen = strstr( RecvBuf, m_szContentLength)))
	{
		CAdapter::closesocket( hSock );
		hSock = INVALID_SOCKET;
		delete [] RecvBuf;
		RecvBuf = NULL;
		return RETURN_HTTP_HEAD_ERROR;
	}
	pContextLen += (strlen(m_szContentLength) + 1);
	nContentLen = atoi(pContextLen);
	if (pContentBeg - RecvBuf + nContentLen > MAX_RVBUF_LEN  || nContentLen < 0)
	{
		CAdapter::closesocket( hSock );
		hSock = INVALID_SOCKET;
		delete [] RecvBuf;
		RecvBuf = NULL;
		return RETURN_RECV_BUF_OUT;
	}
	
	//继续接收Http报文正文
	while ((nRecvLen-(pContentBeg-RecvBuf)) < nContentLen)
	{	
		UINT nReadLen = MAX_RVBUF_LEN - nRecvLen;
		iRet = ReadSocket(hSock, RecvBuf+nRecvLen, &nReadLen);
		if (RETURN_SUCCESS != iRet)
		{
			//接收失败
			delete [] RecvBuf;
			RecvBuf = NULL;
			return iRet;
		}
		nRecvLen += nReadLen;
		RecvBuf[nRecvLen] = 0;		
	}
	
	//回收Socket
	FreeSocket( hSock );
	
	//找到数据包	
	if (pContentBeg = strstr(pContentBeg, lpszResultBeg))
	{	
		pContentBeg += strlen(lpszResultBeg);
		//空上行<MongateGetDeliverResponse xmlns="http://tempuri.org/" />
		LPCTSTR p = " />";
		if (0 == strncmp(pContentBeg, p, strlen(p)))
		{
			//返回空信息
			*ResultLen = 0;
			delete [] RecvBuf;
			RecvBuf = NULL;
			return REQ_MO_NULL_MSG;
		}
		
		if (pContentEnd = strstr(pContentBeg, lpszResultEnd))
		{
			//找全标签头'>'
			if ((pContentBeg = strchr(pContentBeg,'>')) && ((++pContentBeg) < pContentEnd))
			{
				*ResultLen = minn((pContentEnd-pContentBeg), MAX_RVBUF_LEN);
				memcpy(szResult, pContentBeg, *ResultLen);				
			} 
			else
			{
				*ResultLen = 0;
				memcpy(szResult, pContentBeg, *ResultLen);
			}
			iRet = RETURN_SUCCESS;
		}
		else
		{
			delete [] RecvBuf;
			RecvBuf = NULL;
			return BODY_NO_RESULT_END;
		}
	}
	else
	{
		delete [] RecvBuf;
		RecvBuf = NULL;
		return BODY_NO_RESULT_END;
	}
	
	delete [] RecvBuf;
	RecvBuf = NULL;
	return iRet;
}
