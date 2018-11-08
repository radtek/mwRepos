// LogMgr.h: interface for the CLogMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMGR_H__79EC757B_3D21_4BEF_BED2_33C4A1700FF6__INCLUDED_)
#define AFX_LOGMGR_H__79EC757B_3D21_4BEF_BED2_33C4A1700FF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TypeDef.h"
#include <algorithm>
#include <wbs_config.h>

namespace CurrentThread
{

extern __thread int t_cachedTid;
extern __thread int t_tidStringLength;
extern __thread char t_tidString[32];

void cacheTid();
inline int tid()
{
	if (__builtin_expect(t_cachedTid == 0, 0))
	{
		cacheTid();
	}
	return t_cachedTid;
}

inline const char* tidString()
{
	return t_tidString;
}

inline int tidStringLength()
{
	return t_tidStringLength;
}

} // end CurrentThread

struct LOGFILE
{
	char szLogName[MAX_PATH + 1];	//日志文件名
	FILE *fp;						//日志文件指针
	DerivedLock cs;					//日志文件锁
	int nRefCnt;					//引用记数标记
	bool bNeedNextClose;			//是否需要下次关闭并删除
	size_t nFileSize;				//文件大小
	LOGFILE()
	{
		memset(szLogName, 0, sizeof(szLogName));
		fp  = NULL;
		nRefCnt = 0;
		bNeedNextClose = false;
		nFileSize = 0;
	}
	
	void IncRefCnt() 
	{
		++nRefCnt;
	}

	void DecRefCnt()
	{
		--nRefCnt;
	}

	bool IsInUse() 
	{ 
		return 0 != nRefCnt; 
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
		nRefCnt = other.nRefCnt;
		return *this;
	}
};
typedef std::map<std::string, LOGFILE*> MAP_LOGFILE;

struct LOGS
{
	char* LogPtr;						//日志内容
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
    typedef enum LogLevel
    {//枚举值越大，打印级别越高。高级别日志会打印，所有低于高级别的，低级别的日志。
        LOG_TRACE = 0,//0
        LOG_DEBUG,
        LOG_INFO,
        LOG_WARN,
        LOG_ERROR,
        LOG_FATAL,
        NUM_LOG_LEVELS
    } LogLevel;

	static LogLevel logLevel();
	static void SetLogLevel(LogLevel level);
	static void SetLogLevel(UINT nLevel);

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
	// Parameter: [IN] szLog		: 日志内容
	//			  [IN] szLogName	: 日志文件名
	// Returns:   void
	//************************************
    void WriteLog(const char *szLog, const char *szLogName = "log.txt");

	void WriteLog(const char *szLog, 
				  const char *szLogName, 
				  LogLevel level, 
				  const char* fileName, 
				  int lineNum, 
				  const char* funcName);
			
	void WriteLog(const string& strLog, 
				  const char *szLogName, 
				  LogLevel level, 
				  const char* fileName, 
				  int lineNum, 
				  const char* funcName);
    static void CLogWrite(const char *szLogName, CLogMgr::LogLevel level, const char* fileName, int lineNum, const char* funcName, const char *fmt, ...);
	
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
	static void	DelLogs(int nLogSaveDays = 7);

	//变参格式化日志输出
	void	WriteErrorLog(const char *szLogFmt, ...);

	//设置日志路径
	void	SetLogPath(const char *szLogPath);

	string GetLogPath() const;

	//设置单个日志的大小以M为单位
	void	SetLogSize(int nLogSize = 10);

	//设置日志级别
	//void	SetLogLevel(UINT nLogLevel = 0);

	//开启线程模式
	void	OpenThrMode(bool bThrMode);

	//获取线程日志缓冲大小
	int		GetLogBufSize();

	//获取日志文件个数
	int		GetLogFileNum();

	static void EnumFileEx(string strFolderName, int nDay, std::vector<string>& vFileName);

	//获取MORPT共享目录
	static BOOL GetMoRptModule(char* szFileMoShare, char* szFileRptShare);

	void SetCurrentProcessIndex(int nPIndex);

    int GetLogFolderPath(string& strFolderPath);
    int OverwriteFile(const char* szFolderAndFileName, const char* pLogStr);

private:
	CLogMgr();
	int		RenameLog(const char *oldname, const char *newname);
	int		GetLogSize(const char *szLogName);

	//写普通日志
    void WriteLog0(const char *szLog, const char *szLogName);
	void WriteLog0(const char *szLog, 
				   const char *szLogName,
				   LogLevel level, 
				   const char* fileName, 
				   int lineNum, 
				   const char* funcName);

	//清空日志队列，并关闭文件指针
	void	DelAllLogFile();

	//添加到日志队列
	void	WriteLogFile(const char *szPreFix, /*ref_ptr<char*>*/const char* LogPtr, const char *szPostFix, int nLogLen, int nLogSize, const char *szLogName, const char *szNewLogName, const char *szLogPath, int nLogLevel = 1);

	//************************************
	// Description:新建一个文件夹
	// Parameter: [IN] const char *szDirName : 文件夹名称
	// Returns:   BOOL						 0:失败 1:成功
	//************************************
	BOOL	CreateDir(const char *szDirName);

private:
	//清除日志线程
	static THREAD_RETURN __STDCALL  ThreadDelLog(LPVOID pParam);
private:
	char m_szLogPath[MAX_PATH];
	DerivedLock	cs_xml;
	DerivedLock cs_logfile;
	CMemList<string>	m_innerLog;
	MAP_LOGFILE m_logfile;
	static int m_nLogSaveDays;	//日志保存天数
	static pid_t g_nPid;
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
	// UINT m_nLogLevel;


	//zhubo 2017-05-26
	int m_nCurrentProcessIndex;
    DerivedLock		 m_lockOverwrite;//1

};

extern CLogMgr::LogLevel g_logLevel;

inline CLogMgr::LogLevel CLogMgr::logLevel()
{
	return g_logLevel;
}



//最低日志等级为 LOG_TRACE，LOGF_TRACE
//调试时的记录等级为 LOG_LOG_DEBUG，LOGF_DEBUG
//正常的日志打印: LOG_INFO / LOGF_INFO
//警告日志输出到 LOG_LOG_WARN / LOGF_LOG_WARN
//错误的信息输出到LOG_ERROR / LOGF_ERROR，
//线程退出程序退出：LOG_FATAL / LOGF_FATAL
//上线程序默认级别为 LOG_INFO

//最低日志等级为 LOG_TRACE，LOGF_TRACE
#define LOG_TRACE(x) if (CLogMgr::logLevel() <= CLogMgr::LOG_TRACE) \
	CLogMgr::GetInstance().WriteLog((x), "log.txt", CLogMgr::LOG_TRACE, __FILE__, __LINE__, __func__)

//调试时的记录等级为 LOG_DEBUG，LOGF_DEBUG
#define LOG_DEBUG(x) if (CLogMgr::logLevel() <= CLogMgr::LOG_DEBUG) \
	CLogMgr::GetInstance().WriteLog((x), "log.txt", CLogMgr::LOG_DEBUG, __FILE__, __LINE__, __func__)

//正常的日志打印: LOG_INFO / LOGF_INFO
#define LOG_INFO(x) if (CLogMgr::logLevel() <= CLogMgr::LOG_INFO) \
	CLogMgr::GetInstance().WriteLog((x), "log.txt", CLogMgr::LOG_INFO, __FILE__, __LINE__, __func__)

//警告日志输出到 LOG_LOG_WARN / LOGF_WARN
#define LOG_WARN(x) CLogMgr::GetInstance().WriteLog((x), "log.txt", CLogMgr::LOG_WARN, __FILE__, __LINE__, __func__)

//错误的信息输出到LOG_ERROR / LOGF_ERROR，
#define LOG_ERROR(x) CLogMgr::GetInstance().WriteLog((x), "log.txt", CLogMgr::LOG_ERROR, __FILE__, __LINE__, __func__)

//线程退出程序退出：LOG_FATAL / LOGF_FATAL
#define LOG_FATAL(x) CLogMgr::GetInstance().WriteLog((x), "log.txt", CLogMgr::LOG_FATAL, __FILE__, __LINE__, __func__)


//宏函数名加个F是指宏函数的第二个参数为指定文件名打印，而不是默认的log.txt
#define LOGF_TRACE(x, fileName) if (CLogMgr::logLevel() <= CLogMgr::LOG_TRACE) \
	CLogMgr::GetInstance().WriteLog((x), (fileName), CLogMgr::LOG_TRACE, __FILE__, __LINE__, __func__)
#define LOGF_DEBUG(x, fileName) if (CLogMgr::logLevel() <= CLogMgr::LOG_DEBUG) \
	CLogMgr::GetInstance().WriteLog((x), (fileName), CLogMgr::LOG_DEBUG, __FILE__, __LINE__, __func__)
#define LOGF_INFO(x, fileName) if (CLogMgr::logLevel() <= CLogMgr::LOG_INFO) \
	CLogMgr::GetInstance().WriteLog((x), (fileName), CLogMgr::LOG_INFO, __FILE__, __LINE__, __func__)
#define LOGF_WARN(x, fileName) CLogMgr::GetInstance().WriteLog((x), (fileName), CLogMgr::LOG_WARN, __FILE__, __LINE__, __func__)
#define LOGF_ERROR(x, fileName) CLogMgr::GetInstance().WriteLog((x), (fileName), CLogMgr::LOG_ERROR, __FILE__, __LINE__, __func__)
#define LOGF_FATAL(x, fileName) CLogMgr::GetInstance().WriteLog((x), (fileName), CLogMgr::LOG_FATAL, __FILE__, __LINE__, __func__)


//调试时的记录等级为 LOG_LOG_DEBUG，LOGF_DEBUG
#define CLOG_DEBUG( fmt , ...)                      CLogMgr::CLogWrite("log.txt", CLogMgr::LOG_DEBUG, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__ )
//正常的日志打印: LOG_INFO / LOGF_INFO
#define CLOG_INFO( fmt , ...)                       CLogMgr::CLogWrite("log.txt", CLogMgr::LOG_INFO, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__ )
//错误的信息输出到LOG_ERROR / LOGF_ERROR，
#define CLOG_ERROR( fmt , ...)                      CLogMgr::CLogWrite("log.txt", CLogMgr::LOG_ERROR, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__ )
//宏函数名加个F是指宏函数的第二个参数为指定文件名打印，而不是默认的log.txt
#define CLOGF_DEBUG(fileName , fmt , ... )         CLogMgr::CLogWrite((fileName), CLogMgr::LOG_DEBUG, __FILE__, __LINE__, __func__, fmt ,##__VA_ARGS__)
#define CLOGF_INFO(fileName , fmt , ... )          CLogMgr::CLogWrite((fileName), CLogMgr::LOG_INFO, __FILE__, __LINE__, __func__, fmt ,##__VA_ARGS__)
#define CLOGF_ERROR(fileName , fmt , ... )         CLogMgr::CLogWrite((fileName), CLogMgr::LOG_ERROR, __FILE__, __LINE__, __func__, fmt ,##__VA_ARGS__)



#endif // !defined(AFX_LOGMGR_H__79EC757B_3D21_4BEF_BED2_33C4A1700FF6__INCLU
