// IocpServer.h: interface for the CIocpServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOCPSERVER_H__0F37B3DD_FCEA_4FBE_B6D9_A3F6F2F18CA8__INCLUDED_)
#define AFX_IOCPSERVER_H__0F37B3DD_FCEA_4FBE_B6D9_A3F6F2F18CA8__INCLUDED_

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

class DLIB_API CIocpServer  
{
public:
	CIocpServer();
	virtual ~CIocpServer();

public:
	/************************************************************************/
	/*	�������ܣ����÷������    											*/
	/*	����˵����nMaxConNum	���������									*/
	/*			  nUnlinkTime	�����ӳ�ʱʱ��								*/
	/*			  nCheckFreq	�����Ӽ��Ƶ��								*/
	/*			  nThrCnt		��ɶ˿��߳���								*/
	/*			  bShowLogs		�Ƿ���ʾ��־								*/
	/*	�������أ�															*/
	/************************************************************************/
	virtual void				SetParams(int nMaxConNum, int nUnlinkTime, int nCheckFreq, int nThrCnt, bool bShowLogs=1)=0;

	/************************************************************************/
	/*	�������ܣ���ʼ����ɶ˿ڼ���ص����ж�����������ÿһ��10������.     */
	/*	����˵����*/
	/*	�������أ��ɹ���TRUE��ʧ�ܣ�FALSE  
	/************************************************************************/
	virtual BOOL				Init(long nPort, void* pInvokePtr, DEALRECVCALLBACK lpCallBackFun)=0;

	/************************************************************************/
	/* �رշ����˳��߳�                                                   */
	/************************************************************************/
	virtual void				Close()=0;

	/************************************************************************/
	/* ����ɶ˿�Ͷ�ݷ�����������                                           */
	/************************************************************************/
	virtual int					SendIocp(int nSockId, int nSocket, const char *buf, int len, int nState=SOCK_STATE_CONNECTED)=0;

	/************************************************************************/
	/* ����KEY��Ϣ��                                                       */
	/************************************************************************/
	virtual int					UpdateKeyInfo(
									IOCP_KEY_PTR pKey, 
									int nSockId, 
									int nUID, 
									int nSockType, 
									int nUserPrivilege,
									UCHAR ucProtocol,
									UCHAR ucVer, 
									const unsigned char *szSrcAddr
									)=0;



	/************************************************************************/
	/* ��ȡKEY��Ϣ��InUseCount,IdleCount.                                  */
	/************************************************************************/
	virtual void				ReportKeyInfo(int& nBusy, int& nIdle)=0;

	/************************************************************************/
	/* ��ȡIO��Ϣ                                                           */
	/************************************************************************/
	virtual void				ReportIoInfo(int& nBusy, int& nIdle)=0;

	/************************************************************************/
	/* ���IP����������														*/
	/************************************************************************/
	static int					AddBlackIP(const char *szIp);

	/************************************************************************/
	/* ��IP�Ӻ�������ɾ����													*/
	/************************************************************************/
	static void					DelBlackIP(const char *szIp);

	/************************************************************************/
	/* ��ȡIP��������														*/
	/************************************************************************/
	static std::map<std::string,int> GetBlackIP();

	/************************************************************************/
	/* ����SOCKET ID��                                                      */
	/************************************************************************/
	virtual bool				FindSockId(int nSockId)=0;

	/************************************************************************/
	/* ����SOCKET ID ������                                                 */
	/************************************************************************/
	virtual int					GetSockIdNum()=0;

	/************************************************************************/
	/* ��ȡSOCKET ״̬��                                                    */
	/************************************************************************/
	virtual int					GetSockState(int nSockId)=0;

	/************************************************************************/
	/* ��ȡ�����ܴ�����														*/
	/************************************************************************/
	virtual int					GetAcceptConNum() const=0;

	/************************************************************************/
	/* ���������ܴ�����														*/
	/************************************************************************/
	virtual void				ResetAcceptConNum()=0;

	/************************************************************************/
	/* �ر�SOCKET,��������Դ��                                            */
	/************************************************************************/
	virtual void				CloseSocketEx(SOCKET& sock, int nSocketId=0, IOCP_KEY_PTR pKey=NULL, IOCP_IO_PTR pIo=NULL, 
									BOOL bRemove=FALSE, BOOL bForceClose=TRUE, int nLine=0) =0;
};


/************************************************************************/
/* ����IOCP �����                                                       */
/************************************************************************/
class DLIB_API CIocpServerFactory
{
public:
	CIocpServerFactory();
	virtual ~CIocpServerFactory();

public:
	static CIocpServer* New();
	static void Destroy(CIocpServer* pBase);
};




#endif // !defined(AFX_IOCPSERVER_H__0F37B3DD_FCEA_4FBE_B6D9_A3F6F2F18CA8__INCLUDED_)
