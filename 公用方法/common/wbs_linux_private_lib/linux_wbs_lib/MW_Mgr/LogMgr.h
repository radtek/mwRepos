// LogMgr.h: interface for the CLogMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMGR_H__79EC757B_3D21_4BEF_BED2_33C4A1700FF6__INCLUDED_)
#define AFX_LOGMGR_H__79EC757B_3D21_4BEF_BED2_33C4A1700FF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TypeDef.h"
#include "adapter/CompatibleThread.h"
#include "adapter/CompatibleEvent.h"
//zhubo 2017-4-7 
//#include "shlwapi.h"
#include <algorithm>

#include "DerivedLock.h"
#include "MemList.h"
#include "MemListEx.h"
//zhubo 2017-4-7 
//#include "sys/stat.h"
#include "DoubleListEx.h"
#include "smartptr.h"

struct LOGFILE 
{
	char szLogName[MAX_PATH+1]; //日志文件名
	FILE *fp;				  //日志文件指针
	LOGFILE()
	{
		memset(szLogName, 0, sizeof(szLogName));
		fp  = NULL;
	}
	
	LOGFILE& operator=(const LOGFILE& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(szLogName, 0, sizeof(szLogName));
		memcpy(szLogName, other.szLogName, sizeof(szLogName));
		fp  = other.fp;
		return *this;
	}
};
typedef std::map<std::string, LOGFILE> MAP_LOGFILE;

struct LOGS
{
	/*ref_ptr<char*>*/char* LogPtr;						//日志内容
	//std::string strLog;								//日志内容
	std::string strPreFix;								//日志内容前缀
	std::string	strPostFix;								//日志内容后缀
	/*std::string*/ char szLogName[MAX_PATH];			//日志文件名
	/*std::string*/ char szNewLogName[MAX_PATH];		//新日志文件名
	/*std::string*/ char szLogPath[MAX_PATH];			//旧日志路径
	/*std::string*/ char szNewLogPath[MAX_PATH];		//新日志路径
	int nLogSize;										//单个日志的最大容量
	LOGS()
	{
		memset(this, 0, sizeof(LOGS));
		nLogSize = 10;
	}
	
	LOGS& operator=(const LOGS& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(LOGS));
		this->LogPtr     = other.LogPtr;
		this->strPreFix  = other.strPreFix;
		this->strPostFix = other.strPostFix;
		this->nLogSize   = other.nLogSize;
		memset(this->szLogName, 0, sizeof(this->szLogName));
		strcpy(this->szLogName, other.szLogName);
		memset(this->szNewLogName, 0, sizeof(this->szNewLogName));
		strcpy(this->szNewLogName, other.szNewLogName);
		memset(this->szLogPath, 0, sizeof(this->szLogPath));
		strcpy(this->szLogPath, other.szLogPath);
		memset(this->szNewLogPath, 0, sizeof(this->szNewLogPath));
		strcpy(this->szNewLogPath, other.szNewLogPath);
		return *this;
	}
};
//typedef CDoubleListEx<LOGS,	500> LOG_LIST;

class CLogMgr  
{
public:
	static CLogMgr& GetInstance();//单件实例获取接口
	virtual ~CLogMgr();
public:
	//************************************
	// Description:从日志队列中取一条出来
	// Parameter: void
	// Returns:   CString 一条日志
	//************************************
	string	GetInnerLog();

	//************************************
	// Description:看日志队列中是否为空
	// Parameter: void
	// Returns:   BOOL 0:非空 1:空
	//************************************
	BOOL	IsInnerLogEmpty();

	//取日志条数
	int		GetInnerLogCount();

	//************************************
	// Description:写日志
	// Parameter: [IN] CString szLog		: 日志内容
	//			  [IN] CString szLogName	: 日志文件名
	// Returns:   void
	//************************************
	//void	WriteLog(CString szLog, CString szLogName = "log.txt");
	void	WriteLog(const char *szLog, const char *szLogName = "log.txt");

	//写日志，不带时间
	void	WriteLogNoTime(const char *szLog, const char *szLogName = "log.txt");

	//************************************
	// Description:写日志，但有文件等要求
	// Parameter: [IN] const char *szSrc	: 日志内容
	//			  [IN] int nLen				: 日志长度
	//			  [IN] const char *szSrcAddr: 哪个用户的日志
	//			  [IN] CString szLogName	: 日志名称
	// Returns:   void
	//************************************
	void	WriteLogEx(/*ref_ptr<char*>*/const char* LogPtr, int nLen, const char *szSrcAddr, const char *szLogName = "logEx.txt");

	//************************************
	// Description:写日志(哪个sockid对应的哪个用户的哪种类型的接收到的还是主动发送的日志)
	// Parameter: [IN] const char *szSrc	: 日志内容
	//			  [IN] int nLen				: 日志长度
	//			  [IN] int nSockId			: 哪个SockId的日志
	//			  [IN] int nCmdId			: 哪种类型的日志
	//			  [IN] int nRecvSendFlag	: 是接收的还是发送的日志
	//			  [IN] const char *szSourceAddr:哪个用户的日志
	//			  [IN] CString szLogName	: 日志名称
	// Returns:   void
	//************************************
	void	WriteLogExEx(/*ref_ptr<char*>*/const char* LogPtr, 
								int nLen, 
								int nSockId, 
								int nCmdId, 
								int nRecvSendFlag, 
								const char *szSourceAddr, 
								const char *szLogName = "logExEx.txt");

	//记录原始网络字节包
	void	WriteLogExExEx(/*ref_ptr<char*>*/const char* LogPtr, int nLogLen, const char *szLogName = "logExExEx.txt");

	//************************************
	// Description:添加到日志队列
	// Parameter: [IN] CString szLog		: 日志内容
	//			  [IN] CString szLogName	: 日志文件名
	// Returns:   void
	//************************************
	//void	AddInnerLog(CString szLog, CString szLogName = "log.txt");
	void	AddInnerLog(const char *szLog, const char *szLogName = "log.txt");

	//************************************
	// Description:为指定的UID生成指定nType的xml文档
	// Parameter: [IN] CString strXml		: 文件内容
	//			  [IN] int nUID				: 用户UID
	//			  [IN] int nType			: 类型(上行，下行，状态报告)
	// Returns:   void
	//************************************
	void	WriteXml(const char *szXML, int nLen, int nUID, int nType);

	//************************************
	// Description:删除文件或文件夹
	// Parameter: [IN] CString strFilePath	: 文件或文件夹路径
	// Returns:   BOOL						
	//************************************
	static BOOL	ShellDeleteFiles(string strFilePath);

	//************************************
	// Description:枚举指定文夹下文件或文件夹
	// Parameter: [IN] CString strFolderName: 指定的文件夹路径
	//			  [OUT]std::vector<CString>& vFileName:返回枚举到的文件名称(全路径)
	// Returns:   BOOL						
	//************************************
	static void	EnumDir(string strFolderName, std::vector<string>& vFileName);

	//************************************
	// Description:枚举指定文夹下文件或文件夹(创建日期距今时间超过nDay天的)
	// Parameter: [IN] CString strFolderName: 指定的文件夹路径
	//			  [IN] int nDay				: 创建日期距今时间(天)
	//			  [OUT]std::vector<CString>& vFileName:返回枚举到的文件名称(全路径)
	// Returns:   BOOL						
	//************************************
	static void	EnumDirEx(string strFolderName, int nDay, std::vector<string>& vFileName);

	//************************************
	// Description:删除文件夹及文件夹下所有内容
	// Parameter: [OUT]std::vector<CString>& vFolderName:文件夹全路径
	// Returns:   void						
	//************************************
	static void	DelDir(std::vector<string>& vFolderName);

	//************************************
	// Description:删除日志
	// Parameter: void
	// Returns:   void						
	//************************************
	static void	DelLogs(int nLogSaveDays=7);

	//变参格式化日志输出
	void	WriteErrorLog(const char *szLogFmt, ...);

	//设置日志路径
	void	SetLogPath(const char *szLogPath);

	string GetLogPath() const;

	//设置单个日志的大小以M为单位
	void	SetLogSize(int nLogSize=10);

	//设置日志级别
	void	SetLogLevel(UINT nLogLevel=0);

	//开启线程模式
	void	OpenThrMode(bool bThrMode);

	//获取线程日志缓冲大小
	int		GetLogBufSize();

	//获取日志文件个数
	int		GetLogFileNum();

	static void EnumFileEx(string strFolderName, int nDay, std::vector<string>& vFileName);

	//取得一定大小的内存空间
	ref_ptr<char*>	GetBuffer(UINT nSize);

	//获取MORPT共享目录
	static BOOL GetMoRptModule(char* szFileMoShare, char* szFileRptShare);
private:
	CLogMgr();
	int		RenameLog(const char *oldname, const char *newname);
	int		GetLogSize(const char *szLogName);
	
	//写普通日志
	void	WriteLog0(const char *szLog, const char *szLogName = "log.txt");

	//按帐号写日志
	void	WriteLog1(/*ref_ptr<char*>*/const char* LogPtr, int nLen, const char *szSrcAddr, const char *szLogName = "logEx.txt");

	//记某个一帐号的通信网络包
	void	WriteLog2(/*ref_ptr<char*>*/const char* LogPtr, 
								int nLen, 
								int nSockId, 
								int nCmdId, 
								int nRecvSendFlag, 
								const char *szSourceAddr, 
								const char *szLogName = "logExEx.txt");
	//写详细网络日志包
	void	WriteLog3(/*ref_ptr<char*>*/const char* LogPtr, int nLogLen, const char *szLogName = "logExExEx.txt");

	//管理日志文件及日志文件指针的打开和关闭
	//添加到日志文件名队列并返加对应的文件指针
	FILE*	AddLogFile1(const char *szLogName);

	//查找日志文件是否存在于队列中，若存在则返回有效指针，不存在返回NULL
	FILE*	FindLogFile(const char *szLogName);

	//从日志队列中删除该日志文件，并关闭文件指针
	void	DelLogFile(const char *szLogName);

	//清空日志队列，并关闭文件指针
	void	DelAllLogFile();

	//添加到日志队列
	void	WriteLogFile(const char *szPreFix, /*ref_ptr<char*>*/const char* LogPtr, const char *szPostFix, int nLogLen, int nLogSize, const char *szLogName, const char *szNewLogName, const char *szLogPath, int nLogLevel=1);

	//
	void	WriteLogFileEx(const char *szLog, int nLogLen, int nLogSize, const char *szLogName, const char *szNewLogName, const char *szLogPath, int nLogLevel=1);

	//写日志，并根据大小重命名
	void	WriteLogFile(LOGS& log);

	//************************************
	// Description:新建一个文件夹
	// Parameter: [IN] const char *szDirName : 文件夹名称
	// Returns:   BOOL						 0:失败 1:成功
	//************************************
	BOOL	CreateDir(const char *szDirName);

private:
	//清除日志线程
	static THREAD_RETURN __STDCALL  ThreadDelLog(LPVOID pParam); 
	//写日志线程
	static unsigned int __STDCALL ThreadWriteLog(LPVOID pParam);
private:
	char m_szLogPath[MAX_PATH];
	DerivedLock	cs_xml;
	DerivedLock cs_logfile;
	CMemList<string>	m_innerLog;
	MAP_LOGFILE m_logfile;
	static int m_nLogSaveDays;	//日志保存天数
	int m_nLogSize;	//单个日志文件大小
	int m_nLargeLogSize;
	//LOG_LIST m_LogList;
	CMemList<LOGS> m_LogQue;

	
	//zhubo 2017-4-7
	//HANDLE m_hCanWrite;
	CCompatibleEvent  m_hCanWrite;
	BOOL m_bThrMode;		//是否启动线程模式
	BOOL m_bThrStart;		//线程启动标志
	BOOL m_bClear;		
	UINT m_nLogLevel;
};

#endif // !defined(AFX_LOGMGR_H__79EC757B_3D21_4BEF_BED2_33C4A1700FF6__INCLUDED_)
