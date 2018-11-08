// IocpClient.h: interface for the CIocpClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOCPCLIENT_H__9A0FF28B_D15C_4EC5_AB63_B5F1E0E65B07__INCLUDED_)
#define AFX_IOCPCLIENT_H__9A0FF28B_D15C_4EC5_AB63_B5F1E0E65B07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef DLIB_PROJECT
#ifndef DLIB_API
#define DLIB_API __declspec(dllexport)
#endif
#else
#ifndef NOUSE_DLLEXPORT
#ifndef DLIB_API
#define DLIB_API __declspec(dllimport)
#endif
#else
#ifndef DLIB_API
#define DLIB_API
#endif 
#endif
#endif

#include "nettypes.h"

/************************************************************************/
/* IOCP CLIENT															*/                                                                  
/************************************************************************/
class DLIB_API CIocpClient  
{
public:
	CIocpClient();
	virtual ~CIocpClient();

public:
	/************************************************************************/
	/* 函数功能：获取SOCKET 状态。                                          */
	/* 参数说明：SOCKET ID													*/
	/* 函数返回：SOCKET_STATE												*/
	/************************************************************************/
	virtual int  GetSockState(int nSockId)=0;
	
	
	/************************************************************************/
	/*函数功能：初始化完成端口及相关的所有东西.								*/
	/*参数说明: dwPtr调用类this,lpCallBackFun完成端口回调函数				*/
	/*函数返回：成功，TRUE；失败，FALSE										*/
	/************************************************************************/
	virtual BOOL Init(void* pInvokePtr, DEALRECVCALLBACK lpCallBackFun)=0;
	
	
	/************************************************************************/
	/* 函数功能：连接服务器并发送信息										*/
	/* 参数说明：															*/
	/* 函数返回：TRUE/FALSE													*/
	/************************************************************************/
	virtual BOOL ConnectSend(const char *szAddr, int nPort, int *pSockId, UINT *pSocket, 
							const char * msg, int nMsgLen, int nSeqId)=0;
	
	
	/************************************************************************/
	/* 函数功能：连接服务器并发送信息										*/
	/* 参数说明：															*/
	/* 函数返回：TRUE/FALSE													*/
	/************************************************************************/
	virtual BOOL ConnectSendEx(const char *szAddr, int nPort, int *pSockId, UINT *pSocket, 
								const char * msg, int nMsgLen, int nSeqId, const char* szSrcAddr)=0;
	
	
	/************************************************************************/
	/* 函数功能：发送信息													*/
	/* 参数说明：															*/
	/* 函数返回：错误号。													*/
	/************************************************************************/
	virtual int  SendIocp(int nSockId, int nSocket, const char *szSendBuf, int nLen, int nState=SOCK_STATE_CONNECTED)=0;
	
	
	/************************************************************************/
	/* 函数功能：发送信息													*/
	/* 参数说明：															*/
	/* 函数返回：错误号。													*/
	/************************************************************************/
	virtual int  SendIocpEx(int nSockId, int nSocket, const char *szSendBuf, int nLen, int nSeqId, int nState=SOCK_STATE_CONNECTED)=0;
	
	
	/************************************************************************/
	/* 关闭线程，SOCKET，释放资源。                                         */
	/************************************************************************/
	virtual void Close()=0;
	
	
	/************************************************************************/
	/* 获取 IO KEY 信息。												    */
	/************************************************************************/
	virtual void ReportKeyInfo(int& nBusy, int& nIdle)=0;
	
	
	/************************************************************************/
	/* 获取 IO 信息。														*/
	/************************************************************************/
	virtual void ReportIoInfo(int& nBusy, int& nIdle)=0;
	
	/************************************************************************/
	/* 关闭socket并删除IO,IO KEY                                            */
	/************************************************************************/
	virtual void CloseSocket(int nSockId, SOCKET& sock)=0;

};


/************************************************************************/
/* 产生CIocpClient类对象                                                */
/************************************************************************/
class DLIB_API CIocpClientFactory
{
public:
	CIocpClientFactory();
	virtual ~CIocpClientFactory();

public:
	static CIocpClient* New();
	static void Destroy(CIocpClient* pBase);
};


#endif // !defined(AFX_IocpClientBase_H__9A0FF28B_D15C_4EC5_AB63_B5F1E0E65B07__INCLUDED_)
