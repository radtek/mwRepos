// LogMgr.cpp: implementation of the CLogMgr class.
//
//////////////////////////////////////////////////////////////////////


//zhubo 2017-4-7
//#include "cmppgw.h"
#include "LogMgr.h"
#include <unistd.h>
#include <sys/syscall.h>
//#include "AdapterExWbsLinux.h"
#include "Global.h"
#include <time.h>


//zhubo 2017-4-7
//#include "process.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace CurrentThread
{

__thread int t_cachedTid = 0;
__thread int t_tidStringLength;
__thread char t_tidString[32];


void cacheTid()
{
	if (t_cachedTid == 0)
	{
		t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
		t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
	}
}

} // end CurrentThread

void afterFork()
{
	CurrentThread::t_cachedTid = 0;
	CurrentThread::tid();
}

class ThreadInitializer
{
public:
	ThreadInitializer()
	{
		CurrentThread::tid();
		pthread_atfork(NULL, NULL, &afterFork);
	}
};

ThreadInitializer init;

CLogMgr::LogLevel InitLogLevel()
{
	if (::getenv("WBS_LOG_TRACE"))
		return CLogMgr::LOG_TRACE;
	else if (::getenv("WBS_LOG_DEBUG"))
		return CLogMgr::LOG_DEBUG;
	else
		return CLogMgr::LOG_INFO;
}

CLogMgr::LogLevel g_logLevel = InitLogLevel();
pid_t CLogMgr::g_nPid = ::getpid();

const char* LogLevelName[CLogMgr::NUM_LOG_LEVELS] =
{
	"LOG_TRACE ",
	"LOG_DEBUG ",
	"LOG_INFO  ",
	"LOG_WARN  ",
	"LOG_ERROR ",
	"LOG_FATAL ",
};

int CLogMgr::m_nLogSaveDays  = 7;	//7天
CLogMgr& CLogMgr::GetInstance()
{
	static CLogMgr instance;
	return instance;
}

CLogMgr::CLogMgr()
{
	m_bClear   = true;
	m_bThrMode = false;	//默认不启动线程写日志
	m_bThrStart= false;
	memset(m_szLogPath, 0, sizeof(m_szLogPath));
	m_nLogSize = 10;	//10M

    m_nCurrentProcessIndex = 1;
}

CLogMgr::~CLogMgr()
{

}

void CLogMgr::SetLogLevel(CLogMgr::LogLevel level)
{
	g_logLevel = level;
}

void CLogMgr::SetLogLevel(UINT nLevel)
{
	switch (nLevel)
	{
	case 0:
		g_logLevel = CLogMgr::LOG_TRACE;
		break;
	case 1:
		g_logLevel = CLogMgr::LOG_DEBUG;
		break;
	case 2:
		g_logLevel = CLogMgr::LOG_INFO;
		break;
	case 3:
		g_logLevel = CLogMgr::LOG_WARN;
		break;
	case 4:
		g_logLevel = CLogMgr::LOG_ERROR;
		break;
	case 5:
		g_logLevel = CLogMgr::LOG_FATAL;
		break;
	default:
		g_logLevel = CLogMgr::LOG_DEBUG;
		break;
	}
}

//设置日志路径
void CLogMgr::SetLogPath(const char *szLogPath)
{	
	if (NULL != szLogPath)
	{
		memset(m_szLogPath, 0, sizeof(m_szLogPath));
		strcpy(m_szLogPath, szLogPath);
	}
	//自定义的目录不存在则取当前目录
	if (!CAdapter::PathFileExists(m_szLogPath))
	{
		char szTemp[MAX_PATH+1] = {0};
        //zhubo 2017-4-7
		CAdapter::GetCurrentPath(szTemp, MAX_PATH);

        /*******************zhubo 2017-4-12**********************/
        /*
		CString strPath(szTemp);
		strPath = strPath.Left(strPath.ReverseFind('\\') + 1);
		memset(m_szLogPath, 0, sizeof(m_szLogPath));
		memcpy(m_szLogPath, strPath, strPath.GetLength());
		*/
        /*******************zhubo 2017-4-12**********************/
        memcpy(m_szLogPath, szTemp, strlen(szTemp));
	}
}

//设置日志大小
void CLogMgr::SetLogSize(int nLogSize)
{
	//日志大小至少为10
	nLogSize<10?nLogSize=10:nLogSize;
	m_nLogSize = nLogSize;

	//大日志文件为小日志的5倍，但不超过100M
	m_nLargeLogSize = 5*nLogSize;
	m_nLargeLogSize>100?m_nLargeLogSize=100:1;
}

//获取线程日志缓冲大小
int	CLogMgr::GetLogBufSize()
{
	return m_LogQue.GetCount();
}

//获取日志文件个数
int	CLogMgr::GetLogFileNum()
{
	return m_logfile.size();
}

//是否开启日志线程模式
void CLogMgr::OpenThrMode(bool bThrMode)
{
	m_bThrMode = bThrMode;
}

//取日志路径
string CLogMgr::GetLogPath() const
{
	return m_szLogPath;
}

//重命名文件
int CLogMgr::RenameLog(const char *oldname, const char *newname)
{
	int nRet;
	nRet = rename(oldname, newname);
	return nRet;
}

//取文件大小
int CLogMgr::GetLogSize(const char *szLogName)
{
    struct _stat buf;
    int nRet = _stat(szLogName, &buf);
    if(nRet == 0)
    {
        return buf.st_size;
    }
	return 0;
}


void CLogMgr::WriteLog(const char *szLog, const char *szLogName)
{
    WriteLog0(szLog, szLogName);
}


void CLogMgr::WriteLog(const char *szLog, const char *szLogName, LogLevel level, const char* fileName, int lineNum, const char* funcName)
{
	if (level >= g_logLevel)
	{
		WriteLog0(szLog, szLogName, level, fileName, lineNum, funcName);
	}
}


void	CLogMgr::WriteLog(const string& strLog, const char *szLogName, LogLevel level, const char* fileName, int lineNum, const char* funcName)
{
	if (level >= g_logLevel)
	{
		WriteLog0(strLog.c_str(), szLogName, level, fileName, lineNum, funcName);
	}
   
}

void CLogMgr::WriteLog0(const char *szLog, const char *szLogName)
{
    char szParam[MAX_PATH] = { 0 };
    char szTemp[MAX_PATH] = { 0 };
    char szNewName[MAX_PATH] = { 0 };
    char szOldName[MAX_PATH] = { 0 };
    DWORD dwLen = MAX_PATH;
    try
    {
        string strTime("");
        string strDate("");
        string strDate2("");
        string strNewName("");

        SYSTEMTIME_EX tt;
        CAdapter::GetLocalTime(&tt);
        SYSTEMTIME_EX_LINUX t(tt);

        strDate = cstr::format("%02d-%02d-%02d ", t.wYear, t.wMonth, t.wDay);//2017-06-28 
        strDate2 = cstr::format("%02d-%02d-%02d", t.wYear, t.wMonth, t.wDay);//2017-06-29
        strTime = cstr::format("%02d:%02d:%02d:%03d", t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);//14:40:18:009
        strNewName = cstr::format("%02d-%02d-%02d-%03d ", t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);//15-26-29-015
        strNewName = strDate + strNewName;//  2017-06-28 15-26-29-015
        strTime = strDate + strTime;//2017-06-28 14:40:18:009
        strTime += " ---- ";//2017-06-28 14:40:18:009 ---- 

        //自定义的目录不存在则取当前目录
        if (!CAdapter::PathFileExists(m_szLogPath))
        {//  /out/bin
			CAdapter::GetCurrentPath(szTemp, MAX_PATH);
            /*******************zhubo 2017-4-12**********************/
            /*
			CString strPath(szTemp);
			strPath = strPath.Left(strPath.ReverseFind('\\') + 1);
			memset(szTemp, 0, sizeof(szTemp));
			strcpy(szTemp, strPath);
			*/
            /*******************zhubo 2017-4-12**********************/
			//SetLogPath(szTemp);
        }
        else
        {//   /out/bin
            strcpy(szTemp, m_szLogPath);
        }

        strcat(szTemp, "SysLogs");//   /out/bin/SysLogs
        strcat(szOldName, "SysLogs");//   /SysLogs
        if (!CAdapter::PathFileExists(szTemp))
        {
            CAdapter::CreateDirectory(szTemp, NULL);//   /out/bin/SysLogs
        }
        strcat(szTemp, "/");//   /out/bin/SysLogs/
        strcat(szTemp, strDate2.c_str());//   /out/bin/SysLogs/2017-06-29
        strcat(szOldName, "/");//   /SysLogs/
        strcat(szOldName, strDate2.c_str());//   /SysLogs/2017-06-29
        if (!CAdapter::PathFileExists(szTemp))
        {
            CAdapter::CreateDirectory(szTemp, NULL);//   /out/bin/SysLogs/2017-06-29
        }
        strcat(szTemp, "/");//   /out/bin/SysLogs/2017-06-29/
        strcat(szOldName, "/");//   /SysLogs/2017-06-29/

        strcat(szNewName, szOldName);////   SysLogs/2017-06-29/
        strcat(szNewName, strNewName.c_str());// SysLogs/2017-06-29/2017-06-29 5-26-29-015
        strcat(szNewName, szLogName);// SysLogs/2017-06-29/2017-06-29 15-26-29-015 log.txt
        strcat(szOldName, szLogName);//  SysLogs/2017-06-29/log.txt

        //szNewName=SysLogs/2017-06-29/2017-06-29 15-26-29-015 log.txt 7
        //szOldName=//  SysLogs/2017-06-29/log.txt 6
        //strTime =//2017-06-28 14:40:18:009 ---- 
        //m_szLogPath= /out/bin/
        WriteLogFile(strTime.c_str(), szLog, "\r\n", /*strlen(szLog)*/0, m_nLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_DEFAULT);
    }
    catch (...)
    {
    }
}


void CLogMgr::WriteLog0(const char *szLog, const char *szLogName, LogLevel level, const char* fileName, int lineNum, const char* funcName)
{
	char szTemp[MAX_PATH] = {0};
	char szNewName[MAX_PATH] = {0};
	char szOldName[MAX_PATH] = {0};

	try
	{
		char buffer[128] = { 0 };
		string strPreFix("");
		string strTime("");
		string strDate("");
		string strDate2("");
		string strNewName("");

        string strTmp("");
        snprintf(buffer, sizeof(buffer), "%d", m_nCurrentProcessIndex);
		strTmp.append(buffer); 

        SYSTEMTIME_EX tt;
        CAdapter::GetLocalTime(&tt);
        SYSTEMTIME_EX_LINUX t(tt);
        
		snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d:%03d ", 
				t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
		strPreFix.append(buffer);

		snprintf(buffer, sizeof(buffer), "%02d-%02d-%02d", t.wYear, t.wMonth, t.wDay);
		strDate2.append(buffer);

		snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d_%02d-%02d-%02d-%03d_", 
				t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
		strNewName.append(buffer);

		if (CurrentThread::t_cachedTid == 0)
		{
			CurrentThread::tid();
		}

		strPreFix.append(CurrentThread::tidString());
		strPreFix.append(LogLevelName[level]);
		snprintf(buffer, sizeof(buffer), "[%s:%d] %s ", ::basename(fileName), lineNum, funcName);
		strPreFix.append(buffer);
		strPreFix.append("---- ");

		//自定义的目录不存在则取当前目录
		if (!CAdapter::PathFileExists(m_szLogPath))
		{
			CAdapter::GetCurrentPath(szTemp, MAX_PATH);
            /*******************zhubo 2017-4-12**********************/
            /*
			CString strPath(szTemp);
			strPath = strPath.Left(strPath.ReverseFind('\\') + 1);
			memset(szTemp, 0, sizeof(szTemp));
			strcpy(szTemp, strPath);
			*/
            /*******************zhubo 2017-4-12**********************/
			SetLogPath(szTemp);
		}
		else
		{
			strcpy(szTemp, m_szLogPath);
		}
		//strcat(szTemp, "\\SysLogs");
		//strcat(szOldName, "\\SysLogs");
		strcat(szTemp, "/SysLogs");
		strcat(szOldName, "/SysLogs");
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
		//strcat(szTemp, "\\");
		strcat(szTemp, "/");
		strcat(szTemp, strDate2.c_str());
		//strcat(szOldName, "\\");
		strcat(szOldName, "/");
		strcat(szOldName, strDate2.c_str());
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
		strcat(szTemp, "/");

        //***************zhubo 2017-5-24
        strcat(szTemp, strTmp.c_str());
        if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
        //***************zhubo 2017-5-24
		strcat(szOldName, "/");
		
        //***************zhubo 2017-5-24
        strcat(szOldName, strTmp.c_str());
        strcat(szOldName, "/");
        //***************zhubo 2017-5-24
        
		strcat(szNewName, szOldName);
		strcat(szNewName, strNewName.c_str());
		strcat(szNewName, szLogName);
		strcat(szOldName, szLogName);
        /*
        strTime        year-mon-day hour:min:sec:millsecond -----
        szLog          日志字符串
        szOldName      /SysLogs/year-mon-day/ngx_worker_index/adoerror.txt   
        szNewName      /SysLogs/year-mon-day/ngx_worker_index/year-mon-day hour-min-sec-millsecond/adoerror.txt
        m_szLogPath    
        */
		WriteLogFile(strPreFix.c_str(), szLog, "\r\n", /*strlen(szLog)*/0, m_nLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_DEFAULT);
	}
	catch (...)
	{
	}
} 

void CLogMgr::WriteXml(const char *szXML, int nLen, int nUID, int nType)
{
	if (0 == nUID)
	{
		return ;
	}
	char szParam[MAX_PATH] = {0};
	char szTemp[MAX_PATH] = {0};
	char szNewName[MAX_PATH] = {0};
	DWORD dwLen = MAX_PATH;
	
	try
	{	
		string strFileName("");
		string strUID("");
		//zhubo 2017-4-7
		//SYSTEMTIME t;
		//GetLocalTime(&t);
        //zhubo 2017-4-7
        
        SYSTEMTIME_EX tt;
        CAdapter::GetLocalTime(&tt);
        SYSTEMTIME_EX_LINUX t(tt);

        
		strFileName = cstr::format("%02d%02d%02d%02d%02d%02d%03d.xml", 
						t.wYear, 
						t.wMonth, 
						t.wDay, 
						t.wHour, 
						t.wMinute, 
						t.wSecond, 
						t.wMilliseconds);
		string strDate("");
		strDate = cstr::format("/%02d-%02d-%02d_Data", t.wYear, t.wMonth, t.wDay);
		strUID =  cstr::format("/%d", nUID);
		//自定义的目录不存在则取当前目录
		if (!CAdapter::PathFileExists(m_szLogPath))
		{
			CAdapter::GetCurrentPath(szTemp, MAX_PATH);
            /*******************zhubo 2017-4-12**********************/

            /*
			CString strPath(szTemp);
			strPath = strPath.Left(strPath.ReverseFind('\\') + 1);
			memset(szTemp, 0, sizeof(szTemp));
			strcpy(szTemp, strPath);
			*/
			/*******************zhubo 2017-4-12**********************/
			//SetLogPath(szTemp);
		}
		else
		{
			strcpy(szTemp, m_szLogPath);
		}
		strcat(szTemp, "/SysLogs");
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
		strcat(szTemp, strDate.c_str());
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
		strcat(szTemp, strUID.c_str());
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
		strcat(szTemp, "/");
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
		/*
		WBS_GET_WSDL=0,
		WBS_MT_SINGLE,
		WBS_MT_EXNO,
		WBS_MT_LONGMSG,
		WBS_MO_GET,
		WBS_RPT_GET
		*/
		switch (nType)
		{
		case 0://下行回应
			{
				strcat(szTemp, "/MtReq_Rsp");
			}
			break;
		case 1://上行
			{
				strcat(szTemp, "/MoGet_Rsp");
			}
			break;
		case 2://rpt
			{
				strcat(szTemp, "/RptGet_Rsp");
			}
			break;
		case 3://wsdl
			{
				strcat(szTemp, "/WsdlGet_Rsp");
			}
			break;
		case 4:
			{
				strcat(szTemp, "/QueryFee_Rsp");
			}
		default:
			{
				strcat(szTemp, "/Unknown_Rsp");
			}
			break;
		}
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
		strcat(szTemp, "/");
		strcat(szTemp, strFileName.c_str());  //SP网关程序
		
		SafeLock safelock(&cs_xml);
		
		FILE *fp = NULL;
		if (fp = fopen(szTemp, "ab+"))
		{
			fwrite(szXML, 1, nLen, fp);
			fclose(fp);
		}
	}
	catch (...)
	{
	}
}

BOOL CLogMgr::CreateDir(const char *szDirName)
{
	int bRet = TRUE;
	char szParam[MAX_PATH] = {0};
	char szTemp[MAX_PATH] = {0};
	char szNewName[MAX_PATH] = {0};
	DWORD dwLen = MAX_PATH;
	
	CAdapter::GetCurrentPath(szTemp, MAX_PATH);

    /*******************zhubo 2017-4-12**********************/
    /*
	CString strPath(szTemp);
	strPath = strPath.Left(strPath.ReverseFind('\\') + 1);
	strcpy(szTemp, strPath);
	strcat(szTemp, "\\");
	strcat(szTemp, szDirName);
	*/
    /*******************zhubo 2017-4-12**********************/
    strcat(szTemp, szDirName);
	if (!CAdapter::PathFileExists(szTemp))
	{
		bRet = CAdapter::CreateDirectory(szTemp, NULL);
	}

	return bRet;
}

string CLogMgr::GetInnerLog()
{
	return m_innerLog.GetHead();
}

BOOL CLogMgr::IsInnerLogEmpty()
{
	return m_innerLog.IsEmpty();
}

int	CLogMgr::GetInnerLogCount()
{
	return m_innerLog.GetCount();
}

void CLogMgr::AddInnerLog(const char *szLog, const char *szLogName)
{
    //zhubo 2017-5-25
	//m_innerLog.AddTail(string(szLog));
	// WriteLog(szLog, szLogName);
}

//shell命令删除文件


BOOL CLogMgr::ShellDeleteFiles(string strFilePath)
{
    return CAdapter::ShellDeleteFiles(strFilePath.c_str());
}


void CLogMgr::EnumDir(string strFolderName, std::vector<string>& vFileName)
{   
    /*******************zhubo 2017-4-12**********************/
    CAdapter::EnumDir(strFolderName.c_str(),vFileName);
    /*******************zhubo 2017-4-12**********************/
}

void CLogMgr::EnumDirEx(string strFolderName, int nDay, std::vector<string>& vFileName)
{
    CAdapter::EnumDirEx(strFolderName.c_str(),nDay,vFileName);
}

void CLogMgr::EnumFileEx(string strFolderName, int nDay, std::vector<string>& vFileName)
{
    DIR						*dir;
	struct dirent			*ptr;
    string                   fpath;
    char                    *fname;

    dir = opendir(strFolderName.c_str());
	if (NULL == dir)
	{
		return;
	}
    
	while ((ptr = readdir(dir)) != NULL)
	{
	    fname = ptr->d_name;
	    if (('.' == fname[0] && 1 == strlen(fname))
               || ('.' == fname[0] && '.' == fname[1] && 2 == strlen(fname)))
        {
            continue;
        }
        fpath = strFolderName;
        fpath += "/";
        fpath += fname;

        //zhubo 2018-01-29
        //if(DT_DIR  == ptr->d_type)
        if(CAdapter::IsDir(fpath))
        {
            //文件夹
            fpath += "/";

            EnumFileEx(fpath,nDay,vFileName);            
        }
        //zhubo 2018-01-29
        //else if(DT_REG == ptr->d_type)
        else if(CAdapter::IsRegularFile(fpath))
        {  
           //文件
           if(TRUE == CAdapter::IsFileModifyBeforeDay(fpath.c_str(),nDay))
           {
               vFileName.push_back(fpath);//加入文件名  路径+文件名
           }
        }
    }

	closedir(dir);
}

void CLogMgr::DelDir(std::vector<string>& vFolderName)
{
	std::vector<string>::iterator it = vFolderName.begin();
	for (it; it != vFolderName.end(); ++it)
	{
		ShellDeleteFiles(*it);
	}
}

void CLogMgr::DelLogs(int nLogSaveDays)
{
	m_nLogSaveDays = nLogSaveDays;

    CAdapter::CreateCompatibleThread(ThreadDelLog, (LPVOID)&m_nLogSaveDays);
}

THREAD_RETURN __STDCALL CLogMgr::ThreadDelLog(LPVOID pParam)
{
	int nLogSaveDays = *((int*)pParam);
	nLogSaveDays<2?nLogSaveDays=7:nLogSaveDays;
	nLogSaveDays>90?nLogSaveDays=90:nLogSaveDays;
	char szSysLog[MAX_PATH] = {0};
	char szUsrLog[MAX_PATH] = {0};
	char szBatchMt[MAX_PATH]= {0};
	char szFileRptLocal[MAX_PATH]= {0};
	char szFileMoLocal[MAX_PATH] = {0};
	char szFileRptShare[MAX_PATH] = { 0 };
	char szFileMoShare[MAX_PATH] = { 0 };
	char szDataMt[MAX_PATH] = { 0 };
    char szSmeiLog[MAX_PATH] = {0};
	//自定义的目录不存在则取当前目录
	if (!CAdapter::PathFileExists(CLogMgr::GetInstance().GetLogPath().c_str()))
	{
		CAdapter::GetCurrentPath(szSysLog, MAX_PATH);
        /*******************zhubo 2017-4-12**********************/
        /*
		CString strPath(szSysLog);
		strPath = strPath.Left(strPath.ReverseFind('\\') + 1);
		memset(szSysLog, 0, sizeof(szSysLog));
		strcpy(szSysLog, strPath);
		*/
        /*******************zhubo 2017-4-12**********************/
	}
	else
	{
		strcpy(szSysLog, CLogMgr::GetInstance().GetLogPath().c_str());
	}
	std::vector<string> vLogs;

	//获取MO/RPT共享目录
	GetMoRptModule(szFileMoShare, szFileRptShare);

	strcpy(szUsrLog, szSysLog);
	strcpy(szBatchMt, szSysLog);
    //zhubo 2017-12-28
    strcpy(szSmeiLog, szSysLog);
    //zhubo 2017-12-28

	strcpy(szFileRptLocal, szSysLog);
	strcpy(szFileMoLocal, szSysLog);
	strcat(szFileRptLocal, "/FILERPT");
	strcat(szFileMoLocal, "/FILEMO");

	//如果共享路径存在
	if (CAdapter::PathFileExists(szFileMoShare) && CAdapter::PathFileExists(szFileRptShare))
	{
		//strcat(szFileRptShare, "/*.*");
		//strcat(szFileMoShare, "/*.*");
		strcat(szFileRptShare, "/");
        strcat(szFileMoShare, "/");
		vLogs.clear();
		EnumFileEx(szFileRptShare, nLogSaveDays, vLogs);
		DelDir(vLogs);

		vLogs.clear();
		EnumFileEx(szFileMoShare, nLogSaveDays, vLogs);
		DelDir(vLogs);
	}

	//如果本地路径存在
	if (CAdapter::PathFileExists(szFileMoLocal) && CAdapter::PathFileExists(szFileRptLocal))
	{
		//strcat(szFileRptLocal, "/*.*");
		//strcat(szFileMoLocal, "/*.*");
		strcat(szFileRptLocal, "/");
		strcat(szFileMoLocal, "/");
		vLogs.clear();
		EnumFileEx(szFileRptLocal, nLogSaveDays, vLogs);
		DelDir(vLogs);

		vLogs.clear();
		EnumFileEx(szFileMoLocal, nLogSaveDays, vLogs);
		DelDir(vLogs);
	}


	strcpy(szDataMt, szSysLog);
//	strcat(szSysLog, "/SysLogs/*.*");
	strcat(szSysLog, "/SysLogs");
//	strcat(szUsrLog, "/Userlogs/*.*");
    strcat(szUsrLog, "/Userlogs");
//	strcat(szBatchMt,"/BATCH_MT_REQUEST/*.*");
    strcat(szBatchMt,"/BATCH_MT_REQUEST");
//	strcat(szDataMt, "/DATA_MT/*.*");
    strcat(szDataMt, "/DATA_MT");
    //zhubo 2017-12-28
    strcat(szSmeiLog, "/smeidll_log");
    //zhubo 2017-12-28

    

	
	EnumDirEx(szUsrLog, nLogSaveDays, vLogs);
	DelDir(vLogs);

	vLogs.clear();
	EnumDirEx(szSysLog, nLogSaveDays, vLogs);
	DelDir(vLogs);

	vLogs.clear();
	EnumDirEx(szBatchMt, nLogSaveDays, vLogs);
	DelDir(vLogs);

	vLogs.clear();
	EnumFileEx(szDataMt, nLogSaveDays, vLogs);
	DelDir(vLogs);

    //zhubo 2017-12-28
    vLogs.clear();
	EnumFileEx(szSmeiLog, nLogSaveDays, vLogs);
	DelDir(vLogs);
    //zhubo 2017-12-28

	return 0;
}


//添加到日志队列
void CLogMgr::WriteLogFile(const char *szPreFix, const char* LogPtr, const char *szPostFix, int nLogLen, int nLogSize, const char *szLogName, const char *szNewLogName, const char *szLogPath, int nLogLevel)
{
	char szOldName[MAX_PATH+1] = {0};
	char szNewName[MAX_PATH+1] = {0};
	sprintf(szOldName, "%s%s", szLogPath, szLogName);
	sprintf(szNewName, "%s%s", szLogPath, szNewLogName);

	//每天0时关闭前一天的文件指针
	//SYSTEMTIME systime;
	//GetLocalTime(&systime);

    SYSTEMTIME_EX tt;
    CAdapter::GetLocalTime(&tt);
    SYSTEMTIME_EX_LINUX systime(tt);
        
	if (0 == systime.wHour && m_bClear)
	{
		DelAllLogFile();
		m_bClear = false;
	}
	if (0 != systime.wHour)
	{
		m_bClear = true;
	}
    
	LOGFILE* pLogFile = NULL;
	{
		// 加锁找出MAP中的LOGFILE中的信息
		SafeLock safelock(&cs_logfile);
		MAP_LOGFILE::iterator it = m_logfile.find(szLogName);
		//存在则判断日志文件大小，重命名/重新生成文件
		if (it != m_logfile.end())
		{
			if (it->second == NULL)
				return;
			pLogFile = it->second;

			bool bNeedSwitch = false;
			LOGFILE* pNeedDelPtr = NULL;
			{
				SafeLock safelock(&pLogFile->cs);
				if (!pLogFile->IsInUse() && (pLogFile->bNeedNextClose || pLogFile->nFileSize > 1048576 * nLogSize))
				{
					//关闭文件
					fclose(pLogFile->fp);
					//置需要切换标记
					bNeedSwitch = true;
					//保存需要删除的指针
					pNeedDelPtr = pLogFile;
					m_logfile.erase(it);

					//重命名日志
					RenameLog(szOldName, szNewName);

					//重新生成日志
					pLogFile = new LOGFILE();
					memcpy(pLogFile->szLogName, szLogName, strlen(szLogName));
					pLogFile->fp = fopen(szOldName, "ab+");
					if (pLogFile->fp)
					{
						++pLogFile->nRefCnt;//增加引用计数
						m_logfile.insert(std::make_pair(std::string(pLogFile->szLogName), pLogFile));
					}
					else
					{
						delete pLogFile;
						return;
					}
				}
				else
				{
					pLogFile->IncRefCnt();//增加引用计数
				}
			}
			//删除已切换的日志文件指针
			if (pNeedDelPtr) delete pNeedDelPtr;
		}
		//不存在则插入一项记录，并生成新的文件指针
		else
		{
			//重新生成日志
			pLogFile = new LOGFILE();
			memcpy(pLogFile->szLogName, szLogName, strlen(szLogName));
			pLogFile->fp = fopen(szOldName, "ab+");
			if (pLogFile->fp)
			{
				++pLogFile->nRefCnt;//增加引用计数
				m_logfile.insert(std::make_pair(std::string(pLogFile->szLogName), pLogFile));
			}
			else
			{
				delete pLogFile;
				return;
			}
		}
	}

	if (pLogFile && pLogFile->fp)
	{
		SafeLock safelock(&pLogFile->cs);

		int nSize = fprintf(pLogFile->fp, "%s%s%s", szPreFix, LogPtr, szPostFix);
		nSize > 0 ? pLogFile->nFileSize += nSize : 1;

		fflush(pLogFile->fp);

		pLogFile->DecRefCnt(); //减少引用计数
	}
}

//清空日志队列，并关闭文件指针
void CLogMgr::DelAllLogFile()
{
	SafeLock safelock(&cs_logfile);
	MAP_LOGFILE::iterator it = m_logfile.begin();
	for (it; it != m_logfile.end();)
	{
		if (it->second)
		{
			SafeLock safelock(&it->second->cs);
			//正在占用中的暂不关闭
			if (!it->second->IsInUse())
			{
				if (NULL != it->second->fp)
				{
					fclose(it->second->fp);
					m_logfile.erase(it++);
				}
			}
			else
			{
				//标记为下次关闭并删除
				it->second->bNeedNextClose = true;
			}
		}
		else
		{
			m_logfile.erase(it++);
		}
	}
}


BOOL CLogMgr::GetMoRptModule(char* szFileMoShare, char* szFileRptShare)
{
	BOOL bRet = FALSE;
	char szTmp[MAX_PATH] = { 0 };
	char szPath[MAX_PATH] = { 0 };
	char szWbsLocalName[MAX_PATH] = { 0 };
	CAdapter::GetCurrentPath(szPath, MAX_PATH);
    /*******************zhubo 2017-4-12**********************/
    /*
	CString strPath(szPath);
	strPath = strPath.Left(strPath.ReverseFind('\\') + 1);
	strcpy(szPath, strPath);
	strcat(szPath, "\\cmppgw.ini");  //SP网关程序
	*/
	/*******************zhubo 2017-4-12**********************/
    strcat(szPath, "wbsconf.ini");  //SP网关程序

	if (!CAdapter::PathFileExists(szPath))
	{
		bRet = FALSE;
	}
	else
	{
		//WbsLocalName
		CAdapter::GetPrivateProfileString("WebService", "WbsLocalName", "", szTmp, MAX_PATH, szPath);
		memcpy(szWbsLocalName, szTmp, MAX_PATH);

		memcpy(szFileMoShare, szWbsLocalName, strlen(szWbsLocalName) + 1);
		memcpy(szFileRptShare, szWbsLocalName, strlen(szWbsLocalName) + 1);
		strcat(szFileMoShare, "/FILEMO");
		strcat(szFileRptShare, "/FILERPT");

		bRet = TRUE;
	}
	
	return bRet;
}

void CLogMgr::SetCurrentProcessIndex(int nPIndex)
{
    m_nCurrentProcessIndex = nPIndex;
}

void CLogMgr::CLogWrite(const char *szLogName,
                CLogMgr::LogLevel level,
                const char* fileName,
                int lineNum,
                const char* funcName, const char *fmt, ...)
{
    if (level < g_logLevel)
    {
        return;
    }

    if (NULL == fmt)
    {
        return;
    }

	va_list marker;
	va_start(marker, fmt);
	std::string strResult = "";
	int nLength = vsnprintf(NULL, 0, fmt, marker);
    if (nLength < 0)
    {
        return;
    }
	va_start(marker, fmt);
	strResult.resize(nLength + 1);
	int nSize = vsnprintf((char*)strResult.data(), nLength + 1, fmt, marker);
	va_end(marker);
    if (nSize < 0)
    {
        char strLog[128];
        sprintf(strLog," error. vsnprintf nSize=%d \r\n",__func__,nSize);
        strResult = strLog;
    }
    else
    {
        strResult.resize(nSize);
    }


    // LOGF_INFO(strResult.c_str(), szLogName, level, fileName, lineNum, funcName);
    // WriteLog0(szLog, szLogName, level, fileName, lineNum, funcName);
    CLogMgr::GetInstance().WriteLog0(strResult.c_str(), szLogName, level, fileName, lineNum, funcName);
}

int CLogMgr::GetLogFolderPath(string& strFolderPath)
{
    char szTemp[512] = { 0 };
    char szDate[128] = { 0 };
    
    memset(szTemp, 0, sizeof(szTemp));
    CAdapter::GetCurrentPath(szTemp, sizeof(szTemp));
    strcat(szTemp, "/SysLogs/");
    //size_t strftime(char *str, size_t maxsize, const char *fmt, struct tm *time);

    time_t nowTime=time(NULL);
    //struct tm* tmDate = localtime(&nowTime);
    //memset(szDate, 0, sizeof(szDate));
    struct tm tmDate;
    localtime_r(&nowTime,&tmDate);
    strftime(szDate, sizeof(szDate), "%04Y-%02m-%02d", &tmDate);

    strcat(szTemp, szDate);
    strcat(szTemp, "/");

    memset(szDate, 0, sizeof(szDate));
    snprintf(szDate, sizeof(szDate), "%d", m_nCurrentProcessIndex);
    strcat(szTemp, szDate);

    //strcat(szTemp, "/");

    if (TRUE != CAdapter::PathFileExists(szTemp))
    {
        if (0 != CAdapter::CreateDirectory(szTemp, 0777))
        {
            return -1;
        }
    }
    strFolderPath = szTemp;
    return 0;

}

int CLogMgr::OverwriteFile(const char* szFolderAndFileName, const char* pLogStr)
{//重写文件
    SafeLock safelock(&m_lockOverwrite);

    //FILE * fp = fopen(szFolderAndFileName, "ab+");
    FILE * fp = fopen(szFolderAndFileName, "wb+");//创建一个用于读/写的二进制文件
    if (!fp || !szFolderAndFileName || !pLogStr)
    {
        CLOG_DEBUG("quit fail. fp=%x szFolderAndFileName=%s pLogStr=%s", fp, szFolderAndFileName, pLogStr);
        return -1;
    }

    fputs(pLogStr, fp);
    //fputs(LogPtr, fp);
    //fputs(szPostFix, fp);
    fflush(fp);
    fclose(fp);
    return 0;
}
