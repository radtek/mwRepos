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
	/* �������ܣ���ȡSOCKET ״̬��                                          */
	/* ����˵����SOCKET ID													*/
	/* �������أ�SOCKET_STATE												*/
	/************************************************************************/
	virtual int  GetSockState(int nSockId)=0;
	
	
	/************************************************************************/
	/*�������ܣ���ʼ����ɶ˿ڼ���ص����ж���.								*/
	/*����˵��: dwPtr������this,lpCallBackFun��ɶ˿ڻص�����				*/
	/*�������أ��ɹ���TRUE��ʧ�ܣ�FALSE										*/
	/************************************************************************/
	virtual BOOL Init(void* pInvokePtr, DEALRECVCALLBACK lpCallBackFun)=0;
	
	
	/************************************************************************/
	/* �������ܣ����ӷ�������������Ϣ										*/
	/* ����˵����															*/
	/* �������أ�TRUE/FALSE													*/
	/************************************************************************/
	virtual BOOL ConnectSend(const char *szAddr, int nPort, int *pSockId, UINT *pSocket, 
							const char * msg, int nMsgLen, int nSeqId)=0;
	
	
	/************************************************************************/
	/* �������ܣ����ӷ�������������Ϣ										*/
	/* ����˵����															*/
	/* �������أ�TRUE/FALSE													*/
	/************************************************************************/
	virtual BOOL ConnectSendEx(const char *szAddr, int nPort, int *pSockId, UINT *pSocket, 
								const char * msg, int nMsgLen, int nSeqId, const char* szSrcAddr)=0;
	
	
	/************************************************************************/
	/* �������ܣ�������Ϣ													*/
	/* ����˵����															*/
	/* �������أ�����š�													*/
	/************************************************************************/
	virtual int  SendIocp(int nSockId, int nSocket, const char *szSendBuf, int nLen, int nState=SOCK_STATE_CONNECTED)=0;
	
	
	/************************************************************************/
	/* �������ܣ�������Ϣ													*/
	/* ����˵����															*/
	/* �������أ�����š�													*/
	/************************************************************************/
	virtual int  SendIocpEx(int nSockId, int nSocket, const char *szSendBuf, int nLen, int nSeqId, int nState=SOCK_STATE_CONNECTED)=0;
	
	
	/************************************************************************/
	/* �ر��̣߳�SOCKET���ͷ���Դ��                                         */
	/************************************************************************/
	virtual void Close()=0;
	
	
	/************************************************************************/
	/* ��ȡ IO KEY ��Ϣ��												    */
	/************************************************************************/
	virtual void ReportKeyInfo(int& nBusy, int& nIdle)=0;
	
	
	/************************************************************************/
	/* ��ȡ IO ��Ϣ��														*/
	/************************************************************************/
	virtual void ReportIoInfo(int& nBusy, int& nIdle)=0;
	
	/************************************************************************/
	/* �ر�socket��ɾ��IO,IO KEY                                            */
	/************************************************************************/
	virtual void CloseSocket(int nSockId, SOCKET& sock)=0;

};


/************************************************************************/
/* ����CIocpClient�����                                                */
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
