// LogMgr.h: interface for the CLogMgrBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMGR_H__5DBCBB13_CC66_4EC7_BE14_95CF95953475__INCLUDED_)
#define AFX_LOGMGR_H__5DBCBB13_CC66_4EC7_BE14_95CF95953475__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>

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

class DLIB_API CLogMgrBase  
{
public:
	CLogMgrBase();
	virtual ~CLogMgrBase();

public:
	//************************************
	// Description:从日志队列中取一条出来
	// Parameter: void
	// Returns:   std::string 一条日志
	//************************************
	virtual std::string	GetInnerLog()=0;

	//************************************
	// Description:看日志队列中是否为空
	// Parameter: void
	// Returns:   BOOL 0:非空 1:空
	//************************************
	virtual bool IsInnerLogEmpty()=0;

	//取日志条数
	virtual int	GetInnerLogCount()=0;

	//************************************
	// Description:写日志
	// Parameter: [IN] const char *szLog		: 日志内容
	//			  [IN] const char *szLogName	: 日志文件名
	// Returns:   void
	//************************************
	virtual void	WriteLog(const char *szLog, const char *szLogName = "log.txt")=0;

	//************************************
	// Description:写日志，但有文件等要求
	// Parameter: [IN] const char *szSrc	: 日志内容
	//			  [IN] int nLen				: 日志长度
	//			  [IN] const char *szSrcAddr: 哪个用户的日志
	//			  [IN] const char *szLogName	: 日志名称
	// Returns:   void
	//************************************
	virtual void	WriteLogEx(/*ref_ptr<char*>*/const char* LogPtr, int nLogLen, const char *szSrcAddr, 
								const char *szLogName = "logEx.txt")=0;

	//************************************
	// Description:写日志(哪个sockid对应的哪个用户的哪种类型的接收到的还是主动发送的日志)
	// Parameter: [IN] const char *szSrc	: 日志内容
	//			  [IN] int nLen				: 日志长度
	//			  [IN] int nSockId			: 哪个SockId的日志
	//			  [IN] int nCmdId			: 哪种类型的日志
	//			  [IN] int nRecvSendFlag	: 是接收的还是发送的日志
	//			  [IN] const char *szSourceAddr:哪个用户的日志
	//			  [IN] const char *szLogName	: 日志名称
	// Returns:   void
	//************************************
	virtual void	WriteLogExEx(/*ref_ptr<char*>*/const char* LogPtr, 
								int nLogLen, 
								int nSockId, 
								int nCmdId, 
								int nRecvSendFlag, 
								const char *szSourceAddr, 
								const char *szLogName = "logExEx.txt")=0;

	//记录原始网络字节包
	virtual void	WriteLogExExEx(/*ref_ptr<char*>*/const char* LogPtr, int nLogLen, int nSockId, 
									int nRecvSendFlag, const char *szLogName="logExExEx.txt")=0;

	//************************************
	// Description:添加到日志队列
	// Parameter: [IN] const char *szLog		: 日志内容
	//			  [IN] const char *szLogName	: 日志文件名
	// Returns:   void
	//************************************
	virtual void	AddInnerLog(const char *szLog, const char *szLogName = "log.txt")=0;

	//设置日志路径
	virtual void	SetLogPath(const char *szLogPath)=0;

	virtual const char* GetLogPath() const=0;

	//设置单个日志的大小以M为单位
	virtual void	SetLogSize(int nLogSize=10)=0;

	//设置日志级别
	virtual void	SetLogLevel(int nLogLevel=0)=0;

	//开启线程模式
	virtual void	OpenThrMode(bool bThrMode)=0;

	//获取线程日志缓冲大小
	virtual int		GetLogBufSize()=0;

	//获取日志文件个数
	virtual int		GetLogFileNum()=0;

	//删除N天前的日志
	virtual void	DelLogs(int nLogSaveDays=7)=0;
};

/************************************************************************/
/* 取单例类引用                                                         */
/************************************************************************/
class DLIB_API CLogMgr
{
public:
	static CLogMgrBase& GetInstance();
};


#endif // !defined(AFX_LOGMG_H__5DBCBB13_CC66_4EC7_BE14_95CF95953475__INCLUDED_)
