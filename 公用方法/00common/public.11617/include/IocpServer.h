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
	/*	函数功能：设置服务参数    											*/
	/*	参数说明：nMaxConNum	最大连接数									*/
	/*			  nUnlinkTime	空连接超时时间								*/
	/*			  nCheckFreq	空连接检测频率								*/
	/*			  nThrCnt		完成端口线程数								*/
	/*			  bShowLogs		是否显示日志								*/
	/*	函数返回：															*/
	/************************************************************************/
	virtual void				SetParams(int nMaxConNum, int nUnlinkTime, int nCheckFreq, int nThrCnt, bool bShowLogs=1)=0;

	/************************************************************************/
	/*	函数功能：初始化完成端口及相关的所有东西，并发出每一个10个连接.     */
	/*	函数说明：*/
	/*	函数返回：成功，TRUE；失败，FALSE  
	/************************************************************************/
	virtual BOOL				Init(long nPort, void* pInvokePtr, DEALRECVCALLBACK lpCallBackFun)=0;

	/************************************************************************/
	/* 关闭服务，退出线程                                                   */
	/************************************************************************/
	virtual void				Close()=0;

	/************************************************************************/
	/* 向完成端口投递发送数据请求                                           */
	/************************************************************************/
	virtual int					SendIocp(int nSockId, int nSocket, const char *buf, int len, int nState=SOCK_STATE_CONNECTED)=0;

	/************************************************************************/
	/* 设置KEY信息。                                                       */
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
	/* 获取KEY信息：InUseCount,IdleCount.                                  */
	/************************************************************************/
	virtual void				ReportKeyInfo(int& nBusy, int& nIdle)=0;

	/************************************************************************/
	/* 获取IO信息                                                           */
	/************************************************************************/
	virtual void				ReportIoInfo(int& nBusy, int& nIdle)=0;

	/************************************************************************/
	/* 添加IP至黑名单。														*/
	/************************************************************************/
	static int					AddBlackIP(const char *szIp);

	/************************************************************************/
	/* 将IP从黑名单中删除。													*/
	/************************************************************************/
	static void					DelBlackIP(const char *szIp);

	/************************************************************************/
	/* 获取IP黑名单。														*/
	/************************************************************************/
	static std::map<std::string,int> GetBlackIP();

	/************************************************************************/
	/* 查找SOCKET ID。                                                      */
	/************************************************************************/
	virtual bool				FindSockId(int nSockId)=0;

	/************************************************************************/
	/* 返回SOCKET ID 总数。                                                 */
	/************************************************************************/
	virtual int					GetSockIdNum()=0;

	/************************************************************************/
	/* 获取SOCKET 状态。                                                    */
	/************************************************************************/
	virtual int					GetSockState(int nSockId)=0;

	/************************************************************************/
	/* 获取连接总次数。														*/
	/************************************************************************/
	virtual int					GetAcceptConNum() const=0;

	/************************************************************************/
	/* 重置连接总次数。														*/
	/************************************************************************/
	virtual void				ResetAcceptConNum()=0;

	/************************************************************************/
	/* 关闭SOCKET,清除相关资源。                                            */
	/************************************************************************/
	virtual void				CloseSocketEx(SOCKET& sock, int nSocketId=0, IOCP_KEY_PTR pKey=NULL, IOCP_IO_PTR pIo=NULL, 
									BOOL bRemove=FALSE, BOOL bForceClose=TRUE, int nLine=0) =0;
};


/************************************************************************/
/* 产生IOCP 类对象                                                       */
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
