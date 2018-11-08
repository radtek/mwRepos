// LogMgr.cpp: implementation of the CLogMgr class.
//
//////////////////////////////////////////////////////////////////////


//zhubo 2017-4-7
//#include "cmppgw.h"
#include "adapter/Adapter.h"
#include "LogMgr.h"

//zhubo 2017-4-7
//#include "process.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

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

	//创建线程
/*	m_hCanWrite = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (NULL != m_hCanWrite)
	{
		if (0 != _beginthreadex(NULL, NULL, ThreadWriteLog, (void *)this, 0, NULL))
		{
			m_bThrStart = true;
		}
	}
*/
}

CLogMgr::~CLogMgr()
{

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
		CAdapter::GetModuleFileName(szTemp, MAX_PATH);

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

//设置日志级别
void CLogMgr::SetLogLevel(UINT nLogLevel)
{
	nLogLevel<MAX_LOG_LEVEL_DEFAULT?nLogLevel=MAX_LOG_LEVEL_DEFAULT:1;
	nLogLevel>MAX_LOG_LEVEL_HIGHEST?nLogLevel=MAX_LOG_LEVEL_HIGHEST:1;
	m_nLogLevel = nLogLevel;
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

//取得一定大小的内存空间
ref_ptr<char*> CLogMgr::GetBuffer(UINT nSize)
{
	char *pBuf = new char[nSize];
	if (NULL != pBuf)
	{
		memset(pBuf, 0, nSize);
	}
	return ref_ptr<char*>(pBuf);
}

void CLogMgr::WriteLog(const char *szLog, const char *szLogName)
{
	WriteLog0(szLog, szLogName);
}

//写日志不记时间
void CLogMgr::WriteLogNoTime(const char *szLog, const char *szLogName)
{
	char szParam[MAX_PATH] = {0};
	char szTemp[MAX_PATH] = {0};
	char szNewName[MAX_PATH] = {0};
	char szOldName[MAX_PATH] = {0};
	DWORD dwLen = MAX_PATH;
	
	try
	{	
	    
		string strTime("");
		string strDate("");
		string strDate2("");
		string strNewName("");
		
        //zhubo 2017-4-7
		//SYSTEMTIME t;
		//GetLocalTime(&t);
        //zhubo 2017-4-7
        
        SYSTEMTIME_EX tt;
        CAdapter::GetLocalTime(&tt);
        SYSTEMTIME_EX_LINUX t(tt);
        
		strDate = cstr::format("%02d-%02d-%02d ", t.wYear, t.wMonth, t.wDay);
		strDate2= cstr::format("%02d-%02d-%02d", t.wYear, t.wMonth, t.wDay);
		strTime = cstr::format("%02d:%02d:%02d:%03d",t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
		strNewName = cstr::format("%02d-%02d-%02d-%03d ", t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
		strNewName = strDate + strNewName;
		strTime = strDate + strTime;
		strTime += " ---- ";
		
		//自定义的目录不存在则取当前目录
		if (!CAdapter::PathFileExists(m_szLogPath))
		{
		   
			CAdapter::GetModuleFileName(szTemp, MAX_PATH);
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
		strcat(szTemp, "SysLogs");
		strcat(szOldName, "SysLogs");
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
		strcat(szTemp, "/");
		strcat(szTemp, strDate2.c_str());
		strcat(szOldName, "/");
		strcat(szOldName, strDate2.c_str());
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
		strcat(szTemp, "UrgentRecycle");
		strcat(szOldName, "UrgentRecycle");
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
		strcat(szTemp, "/");
		strcat(szOldName, "/");
		strcat(szNewName,szOldName);
		strcat(szNewName, strNewName.c_str());
	    strcat(szNewName, szLogName);
		strcat(szOldName, szLogName);

		WriteLogFileEx(szLog, strlen(szLog), m_nLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_DEFAULT);
	}
	catch (...)
	{
	}
}

void CLogMgr::WriteLog0(const char *szLog, const char *szLogName)
{		
	char szParam[MAX_PATH] = {0};
	char szTemp[MAX_PATH] = {0};
	char szNewName[MAX_PATH] = {0};
	char szOldName[MAX_PATH] = {0};
	DWORD dwLen = MAX_PATH;

	try
	{	
	    
		string strTime("");
		string strDate("");
		string strDate2("");
		string strNewName("");

        //zhubo 2017-4-7
		//SYSTEMTIME t;
		//GetLocalTime(&t);
        //zhubo 2017-4-7
        
        SYSTEMTIME_EX tt;
        CAdapter::GetLocalTime(&tt);
        SYSTEMTIME_EX_LINUX t(tt);
        
		strDate = cstr::format("%02d-%02d-%02d ", t.wYear, t.wMonth, t.wDay);
		strDate2= cstr::format("%02d-%02d-%02d", t.wYear, t.wMonth, t.wDay);
		strTime = cstr::format("%02d:%02d:%02d:%03d",t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
		strNewName = cstr::format("%02d-%02d-%02d-%03d ", t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
		strNewName = strDate+strNewName;
		strTime = strDate + strTime;
		strTime += " ---- ";

		//自定义的目录不存在则取当前目录
		if (!CAdapter::PathFileExists(m_szLogPath))
		{
			CAdapter::GetModuleFileName(szTemp, MAX_PATH);
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
		strcat(szOldName, "/");
		strcat(szNewName, szOldName);
		strcat(szNewName, strNewName.c_str());
		strcat(szNewName, szLogName);
		strcat(szOldName, szLogName);
		//WriteLogFileEx(strTime+szLog+"\r\n", strTime.GetLength()+strlen(szLog)+2, m_nLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_DEFAULT);
		WriteLogFile(strTime.c_str(), szLog, "\r\n", strlen(szLog), m_nLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_DEFAULT);
	}
	catch (...)
	{
	}
} 

void CLogMgr::WriteLogEx(/*ref_ptr<char*>*/const char* LogPtr, int nLen, const char *szSrcAddr, const char *szLogName)
{
	WriteLog1(LogPtr, nLen, szSrcAddr, szLogName);
}

void CLogMgr::WriteLog1(/*ref_ptr<char*>*/const char* LogPtr, int nLen, const char *szSrcAddr, const char *szLogName)
{		
	char szParam[MAX_PATH] = {0};
	char szTemp[MAX_PATH] = {0};
	char szNewName[MAX_PATH] = {0};
	char szOldName[MAX_PATH] = {0};
	DWORD dwLen = MAX_PATH;
	
	try
	{	
		string strTime("");
		string strDate("");
		string strDate2("");
		string strNewName("");
        
		//zhubo 2017-4-7
		//SYSTEMTIME t;
		//GetLocalTime(&t);
        //zhubo 2017-4-7
        
        SYSTEMTIME_EX tt;
        CAdapter::GetLocalTime(&tt);
        SYSTEMTIME_EX_LINUX t(tt);

        
		strDate = cstr::format("%02d-%02d-%02d ", t.wYear, t.wMonth, t.wDay);
		strDate2= cstr::format("%02d-%02d-%02d", t.wYear, t.wMonth, t.wDay);
		strTime = cstr::format("%02d:%02d:%02d:%03d",t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
		strNewName = cstr::format("%02d-%02d-%02d-%03d ", t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
		strNewName = strDate+strNewName;
		strTime = strDate + strTime;
		strTime += " ---- ";

		//自定义的目录不存在则取当前目录
		if (!CAdapter::PathFileExists(m_szLogPath))
		{
			CAdapter::GetModuleFileName(szTemp, MAX_PATH);
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
		strcat(szTemp, "/UserLogs");
		strcat(szOldName, "/UserLogs");
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
		strcat(szTemp, "/");
		strcat(szTemp, strDate2.c_str());
		strcat(szOldName, "/");
		strcat(szOldName, strDate2.c_str());
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
		strcat(szTemp, "/");
		strcat(szTemp, szSrcAddr);
		strcat(szOldName, "/");
		strcat(szOldName, szSrcAddr);
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
		strcat(szTemp, "/");
		strcat(szOldName, "/");
		strcat(szNewName , szOldName);
		strcat(szNewName , strNewName.c_str());
		strcat(szNewName , szLogName);
		strcat(szOldName, szLogName);
		WriteLogFile(strTime.c_str(), LogPtr, "\r\n", nLen, m_nLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_LOW);
	}
	catch (...)
	{
	}
}

void CLogMgr::WriteLogExEx(/*ref_ptr<char*>*/const char* LogPtr, int nLen, int nSockId, int nCmdId, int nRecvSendFlag, const char *szSourceAddr, const char *szLogName)
{
	WriteLog2(LogPtr, nLen, nSockId, nCmdId, nRecvSendFlag, szSourceAddr, szLogName);
}

void CLogMgr::WriteLog2(/*ref_ptr<char*>*/const char* LogPtr, int nLen, int nSockId, int nCmdId, int nRecvSendFlag, const char *szSourceAddr, const char *szLogName)
{		
	char szParam[MAX_PATH] = {0};
	char szTemp[MAX_PATH] = {0};
	char szNewName[MAX_PATH] = {0};
	char szOldName[MAX_PATH] = {0};
	DWORD dwLen = MAX_PATH;
	
	try
	{	
		string strTime("");
		string strDate("");
		string strDate2("");
		string strNewName("");
		//zhubo 2017-4-7
		//SYSTEMTIME t;
		//GetLocalTime(&t);
        //zhubo 2017-4-7
        
        SYSTEMTIME_EX tt;
        CAdapter::GetLocalTime(&tt);
        SYSTEMTIME_EX_LINUX t(tt);

        
		strDate = cstr::format("%02d-%02d-%02d ", t.wYear, t.wMonth, t.wDay);
		strDate2 = cstr::format("%02d-%02d-%02d", t.wYear, t.wMonth, t.wDay);
		strTime = cstr::format("%02d:%02d:%02d:%03d",t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
		strNewName = cstr::format("%02d-%02d-%02d-%03d ", t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
		strNewName = strDate+strNewName;
		strTime = strDate + strTime;
		strTime += " ---- ";
		
		//自定义的目录不存在则取当前目录
		if (!CAdapter::PathFileExists(m_szLogPath))
		{
			CAdapter::GetModuleFileName(szTemp, MAX_PATH);
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
		strcat(szTemp, "/Syslogs");
		strcat(szOldName, "/Syslogs");
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
		strcat(szTemp, "/");
		strcat(szTemp, strDate2.c_str());
		strcat(szOldName, "/");
		strcat(szOldName, strDate2.c_str());
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
		strcat(szTemp, "/RecvSend_All");
		strcat(szOldName, "/RecvSend_All");
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}		
		strcat(szTemp, "/");
		strcat(szTemp, szSourceAddr);
		strcat(szOldName, "/");
		strcat(szOldName, szSourceAddr);
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
		strcat(szTemp, "/");
		strcat(szOldName, "/");
		strcat(szNewName , szOldName);
		strcat(szNewName , strNewName.c_str());
		strcat(szNewName , szLogName);
		strcat(szOldName, szLogName);
		char szTmp[100] = {0};
		int nRet = 0;
		switch (nCmdId)
		{
		case CMPP_CONNECT:
			{
				nRet = sprintf(szTmp, "[%06d] [%s] [%s] [%s]\r\n", nSockId, szSourceAddr, "RECV", "CMPP_CONNECT");
				//strcat(szNewName, strNewName+szLogName);
				WriteLogFile((strTime+szTmp+"[").c_str(), LogPtr, "]\n", nLen, m_nLargeLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_MID);
			}
			break;
		case CMPP_CONNECT_RESP:
			{
				nRet = sprintf(szTmp, "[%06d] [%s] [%s] [%s]\r\n", nSockId, szSourceAddr, "SEND", "CMPP_CONNECT_RESP");
				//strcat(szNewName, strNewName+szLogName);
				WriteLogFile((strTime+szTmp+"[").c_str(), LogPtr, "]\n\n", nLen, m_nLargeLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_MID);
			}
			break;
		case CMPP_ACTIVE_TEST:
			{
				nRet = sprintf(szTmp, "[%06d] [%s] [%s] [%s]\r\n", nSockId, szSourceAddr, "RECV", "CMPP_ACTIVE_TEST");
				//strcat(szNewName, strNewName+szLogName);
				WriteLogFile((strTime+szTmp+"[").c_str(), LogPtr, "]\n", nLen, m_nLargeLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_MID);
			}
			break;
		case CMPP_ACTIVE_TEST_RESP:
			{
				nRet = sprintf(szTmp, "[%06d] [%s] [%s] [%s]\r\n", nSockId, szSourceAddr, "SEND", "CMPP_ACTIVE_TEST_RESP");
				//strcat(szNewName, strNewName+szLogName);
				WriteLogFile((strTime+szTmp+"[").c_str(), LogPtr, "]\n\n", nLen, m_nLargeLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_MID);
			}
			break;
		case CMPP_SUBMIT:
			{
				nRet = sprintf(szTmp, "[%06d] [%s] [%s] [%s]\r\n", nSockId, szSourceAddr, "RECV", "CMPP_SUBMIT");
				//strcat(szNewName, strNewName+szLogName);
				WriteLogFile((strTime+szTmp+"[").c_str(), LogPtr, "]\n", nLen, m_nLargeLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_MID);
			}
			break;
		case CMPP_SUBMIT_RESP:
			{
				nRet = sprintf(szTmp, "[%06d] [%s] [%s] [%s]\r\n", nSockId, szSourceAddr, "SEND", "CMPP_SUBMIT_RESP");
				//strcat(szNewName, strNewName+szLogName);
				WriteLogFile((strTime+szTmp+"[").c_str(), LogPtr, "]\n\n", nLen, m_nLargeLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_MID);
			}
			break;
		case CMPP_FWD:
			{
				//recv
				if (0 == nRecvSendFlag)
				{
					nRet = sprintf(szTmp, "[%06d] [%s] [%s] [%s]\r\n", nSockId, szSourceAddr, "RECV", "CMPP_FWD");
					//strcat(szNewName, strNewName+szLogName);
					WriteLogFile((strTime+szTmp+"[").c_str(), LogPtr, "]\n", nLen, m_nLargeLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_MID);
				}
				else
				{
					nRet = sprintf(szTmp, "[%06d] [%s] [%s] [%s]\r\n", nSockId, szSourceAddr, "SEND", "CMPP_FWD");
					//strcat(szNewName, strNewName+szLogName);
					WriteLogFile((strTime+szTmp+"[").c_str(), LogPtr, "]\n\n", nLen, m_nLargeLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_MID);
				}
			}
			break;
		case CMPP_FWD_RESP:
			{
				//recv
				if (0 == nRecvSendFlag)
				{
					nRet = sprintf(szTmp, "[%06d] [%s] [%s] [%s]\r\n", nSockId, szSourceAddr, "RECV", "CMPP_FWD_RESP");
					//strcat(szNewName, strNewName+szLogName);
					WriteLogFile((strTime+szTmp+"[").c_str(), LogPtr, "]\n", nLen, m_nLargeLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_MID);
				}
				else
				{
					nRet = sprintf(szTmp, "[%06d] [%s] [%s] [%s]\r\n", nSockId, szSourceAddr, "SEND", "CMPP_FWD_RESP");
					//strcat(szNewName, strNewName+szLogName);
					WriteLogFile((strTime+szTmp+"[").c_str(), LogPtr, "]\n\n", nLen, m_nLargeLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_MID);
				}
			}
			break;
		case CMPP_DELIVER:
			{
				nRet = sprintf(szTmp, "[%06d] [%s] [%s] [%s]\r\n", nSockId, szSourceAddr, "SEND", "CMPP_DELIVER");
				//strcat(szNewName, strNewName+szLogName);
				WriteLogFile((strTime+szTmp+"[").c_str(), LogPtr, "]\n\n", nLen, m_nLargeLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_MID);
			}
			break;
		case CMPP_DELIVER_RESP:
			{
				nRet = sprintf(szTmp, "[%06d] [%s] [%s] [%s]\r\n", nSockId, szSourceAddr, "RECV", "CMPP_DELIVER_RESP");
				//strcat(szNewName, strNewName+szLogName);
				WriteLogFile((strTime+szTmp+"[").c_str(), LogPtr, "]\n", nLen, m_nLargeLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_MID);
			}
			break;
		case CMPP_QUERY_USERFEE:
			{
				nRet = sprintf(szTmp, "[%06d] [%s] [%s] [%s]\r\n", nSockId, szSourceAddr, "RECV", "CMPP_QUERY_USERFEE");
				//strcat(szNewName, strNewName+szLogName);
				WriteLogFile((strTime+szTmp+"[").c_str(), LogPtr, "]\n\n", nLen, m_nLargeLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_MID);
			}
			break;
		case CMPP_QUERY_USERFEE_RESP:
			{
				nRet = sprintf(szTmp, "[%06d] [%s] [%s] [%s]\r\n", nSockId, szSourceAddr, "SEND", "CMPP_QUERY_USERFEE_RESP");
				//strcat(szNewName, strNewName+szLogName);
				WriteLogFile((strTime+szTmp+"[").c_str(), LogPtr, "]\n", nLen, m_nLargeLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_MID);
			}
			break;
		default:
			break;
		}		
	}
	catch (...)
	{
	}
}

void CLogMgr::WriteLogExExEx(/*ref_ptr<char*>*/const char* LogPtr, int nLogLen, const char *szLogName)
{
	WriteLog3(LogPtr, nLogLen, szLogName);
}

void CLogMgr::WriteLog3(/*ref_ptr<char*>*/const char* LogPtr, int nLogLen, const char *szLogName)
{		
	char szParam[MAX_PATH] = {0};
	char szTemp[MAX_PATH] = {0};
	char szNewName[MAX_PATH] = {0};
	char szOldName[MAX_PATH] = {0};
	DWORD dwLen = MAX_PATH;
	
	try
	{	
		string strTime("");
		string strDate("");
		string strDate2("");
		string strNewName("");
		//zhubo 2017-4-7
		//SYSTEMTIME t;
		//GetLocalTime(&t);
        //zhubo 2017-4-7
        
        SYSTEMTIME_EX tt;
        CAdapter::GetLocalTime(&tt);
        SYSTEMTIME_EX_LINUX t(tt);

        
		strDate = cstr::format("%02d-%02d-%02d ", t.wYear, t.wMonth, t.wDay);
		strDate2 = cstr::format("%02d-%02d-%02d", t.wYear, t.wMonth, t.wDay);
		strTime = cstr::format("%02d:%02d:%02d:%03d",t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
		strNewName = cstr::format("%02d-%02d-%02d-%03d ", t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
		strNewName = strDate+strNewName;
		strTime = strDate + strTime;
		strTime += " ---- ";
		
		//自定义的目录不存在则取当前目录
		if (!CAdapter::PathFileExists(m_szLogPath))
		{
			CAdapter::GetModuleFileName(szTemp, MAX_PATH);
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
		strcat(szTemp, "/Syslogs");
		strcat(szOldName, "/Syslogs");
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
		strcat(szTemp, "/");
		strcat(szTemp, strDate2.c_str());
		strcat(szOldName, "/");
		strcat(szOldName, strDate2.c_str());
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
		strcat(szTemp, "/RecvSend_All");
		strcat(szOldName, "/RecvSend_All");
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}
		strcat(szTemp, "/RecvSendDetails");
		strcat(szOldName, "/RecvSendDetails");
		if (!CAdapter::PathFileExists(szTemp))
		{
			CAdapter::CreateDirectory(szTemp, NULL);
		}		
		strcat(szTemp, "/");
		strcat(szOldName, "/");
		strcat(szNewName , szOldName);
		strcat(szNewName , strNewName.c_str());
		strcat(szNewName , szLogName);
		strcat(szOldName, szLogName);
		WriteLogFile(strTime.c_str(), LogPtr, "\r\n", nLogLen, m_nLargeLogSize, szOldName, szNewName, m_szLogPath, MAX_LOG_LEVEL_HIGH);
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
			CAdapter::GetModuleFileName(szTemp, MAX_PATH);
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
	
	CAdapter::GetModuleFileName(szTemp, MAX_PATH);

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
	m_innerLog.AddTail(string(szLog));
	WriteLog(szLog, szLogName);
}

//shell命令删除文件


BOOL CLogMgr::ShellDeleteFiles(string strFilePath)
{
    return CAdapter::ShellDeleteFiles(strFilePath.c_str());
    /*
    //zhubo 2017-4-7
	TCHAR src[MAX_PATH + 1];
	_stprintf(src, "%s", strFilePath);
	src[_tcslen(src)+1]=0;	
	SHFILEOPSTRUCT fileOp;
	memset(&fileOp, 0, sizeof(fileOp));   
	fileOp.wFunc = FO_DELETE ;   
	fileOp.fFlags |= FOF_SILENT;         //后台进行
	//fileOp.fFlags |= FOF_NOERRORUI;      //没有出错提示
	fileOp.fFlags |= FOF_NOCONFIRMATION; //没有确认提示
	fileOp.pFrom = src;
	fileOp.pTo = NULL; 
	if (::SHFileOperation(&fileOp))
	{
		return FALSE;
	}
	return TRUE;

	*/
}


void CLogMgr::EnumDir(string strFolderName, std::vector<string>& vFileName)
{  
    
    /*******************zhubo 2017-4-12**********************/
    CAdapter::EnumDir(strFolderName.c_str(),vFileName);
    /*******************zhubo 2017-4-12**********************/
    

    //copy CString => std::string
    
    //zhubo 2017-4-12
    //这里注释了
    // LPTSTR lpstr = strFolderName.GetBuffer(0);
    //CAdapter::EnumDir((const char*)lpstr,vFileName);
    /*
    //zhubo 2017-4-7
	CFileFind finder;
	BOOL bFileExsit = finder.FindFile(strFolderName);
	if(!bFileExsit)
	{
		return ;
	}
	CString strFilePath;
	
	while (bFileExsit)
	{   
		bFileExsit = finder.FindNextFile();
		if (finder.IsDirectory() 
			&& !finder.IsSystem() 
			&& !finder.IsHidden() 
			&& !finder.IsDots() 
			&& !finder.IsTemporary())
		{	
			strFilePath = finder.GetFilePath();
			vFileName.push_back(strFilePath);//加入文件名  路径+文件名      
		}
	}
	*/
}

void CLogMgr::EnumDirEx(string strFolderName, int nDay, std::vector<string>& vFileName)
{
    CAdapter::EnumDirEx(strFolderName.c_str(),nDay,vFileName);
    //copy CString => std::string

    
    //zhubo 2017-4-12
    /*
	CFileFind finder;
	BOOL bFileExsit = finder.FindFile(strFolderName);
	if(!bFileExsit)
	{
		return ;
	}
	CString strFilePath;
	CTime ct;
	CTimeSpan ts;
	while (bFileExsit)
	{   
		bFileExsit = finder.FindNextFile();
		if (finder.IsDirectory() 
			&& !finder.IsSystem() 
			&& !finder.IsHidden() 
			&& !finder.IsDots() 
			&& !finder.IsTemporary())
		{	
			strFilePath = finder.GetFilePath();
			if (finder.GetCreationTime(ct))
			{
				ts = CTime::GetCurrentTime() - ct;
				if (ts.GetDays() >= nDay)
				{ 
					vFileName.push_back(strFilePath);//加入文件名  路径+文件名
				}  
			}    
		}
	}
	*/
}

void CLogMgr::EnumFileEx(string strFolderName, int nDay, std::vector<string>& vFileName)
{
    //zhubo 2017-4-7
    //std::vector<std::string> vecFileNames;
   // CAdapter::EnumDirEx((LPCSTR)strFolderName,nDay,vecFileNames);
    
    //copy vecFileNames  => vFileName
    
    /*
	CFileFind finder;              //文件夹查找
	CFileFind findfolder;          //子文件查找
	BOOL bFileExsit = finder.FindFile(strFolderName);
	if(!bFileExsit)
	{
		return ;
	}

	CString   strFilePath;        //文件路径
	CString   strSubFolder;       //子文件夹名
	BOOL      bSubFolder;
	CTime     ct;
	CTimeSpan ts;
	while (bFileExsit)
	{   
		bFileExsit = finder.FindNextFile();
		if (finder.IsDirectory()
			&& !finder.IsDots()
			&& !finder.IsSystem() 
			&& !finder.IsHidden() 
			&& !finder.IsTemporary())
		{	
			strSubFolder = finder.GetFilePath();
			strSubFolder += "\\*.*";
			bSubFolder = findfolder.FindFile(strSubFolder);
 			if(!bSubFolder)
 			{
				continue;
 			}
			while (bSubFolder)
			{
				bSubFolder = findfolder.FindNextFile();
				if (!findfolder.IsDirectory() 
					&& !findfolder.IsSystem() 
					&& !findfolder.IsHidden() 
					&& !findfolder.IsDots() 
					&& !findfolder.IsTemporary())
				{	
					strFilePath = findfolder.GetFilePath();
					if (findfolder.GetCreationTime(ct))
					{
						ts = CTime::GetCurrentTime() - ct;
						if (ts.GetDays() >= nDay)
						{ 
							vFileName.push_back(strFilePath);//加入文件名  路径+文件名
						}  
					}    
				}
				
			}
		}
// 		if (!finder.IsDirectory() 
// 			&& !finder.IsSystem() 
// 			&& !finder.IsHidden() 
// 			&& !finder.IsDots() 
// 			&& !finder.IsTemporary())
// 		{	
// 			strFilePath = finder.GetFilePath();
// 			if (finder.GetCreationTime(ct))
// 			{
// 				ts = CTime::GetCurrentTime() - ct;
// 				if (ts.GetDays() >= nDay)
// 				{ 
// 					vFileName.push_back(strFilePath);//加入文件名  路径+文件名
// 				}  
// 			}    
// 		}
	}
	*/
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
    /*
     //zhubo 2017-4-7
	_beginthreadex(NULL, NULL, ThreadDelLog, (LPVOID)&m_nLogSaveDays, 0, NULL);
	*/
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
	//自定义的目录不存在则取当前目录
	if (!CAdapter::PathFileExists(CLogMgr::GetInstance().GetLogPath().c_str()))
	{
		CAdapter::GetModuleFileName(szSysLog, MAX_PATH);
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

	strcpy(szFileRptLocal, szSysLog);
	strcpy(szFileMoLocal, szSysLog);
	strcat(szFileRptLocal, "/FILERPT");
	strcat(szFileMoLocal, "/FILEMO");

	//如果共享路径存在
	if (CAdapter::PathFileExists(szFileMoShare) && CAdapter::PathFileExists(szFileRptShare))
	{
		strcat(szFileRptShare, "/*.*");
		strcat(szFileMoShare, "/*.*");
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
		strcat(szFileRptLocal, "/*.*");
		strcat(szFileMoLocal, "/*.*");
		vLogs.clear();
		EnumFileEx(szFileRptLocal, nLogSaveDays, vLogs);
		DelDir(vLogs);

		vLogs.clear();
		EnumFileEx(szFileMoLocal, nLogSaveDays, vLogs);
		DelDir(vLogs);
	}


	strcpy(szDataMt, szSysLog);
	strcat(szSysLog, "/Syslogs/*.*");
	strcat(szUsrLog, "/Userlogs/*.*");
	strcat(szBatchMt,"/BATCH_MT_REQUEST/*.*");
	strcat(szDataMt, "/DATA_MT/*.*");
	
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

	return 0;
}

//管理日志文件及日志文件指针的打开和关闭
//添加到日志文件名队列并返加对应的文件指针
FILE* CLogMgr::AddLogFile1(const char *szLogName)
{
	SafeLock safelock(&cs_logfile);
	FILE *fp = NULL;
	if (m_logfile.find(szLogName) == m_logfile.end())
	{
		LOGFILE log;
		memcpy(log.szLogName, szLogName, strlen(szLogName));
		log.fp  = fopen(szLogName, "ab+");
		if (log.fp)
		{
			m_logfile.insert(std::make_pair(std::string(szLogName), log));
		}
		fp = log.fp;
	}
	return fp;
}

//查找日志文件是否存在于队列中，若存在则返回有效指针，不存在返回NULL
FILE* CLogMgr::FindLogFile(const char *szLogName)
{
	SafeLock safelock(&cs_logfile);
	FILE *fp = NULL;
	MAP_LOGFILE::iterator it = m_logfile.find(szLogName);
	//存在则返回指针
	if (it != m_logfile.end())
	{
		fp = it->second.fp;
	}
	//不存在则插入一项记录，并生成新的文件指针
	else
	{
		LOGFILE log;
		memcpy(log.szLogName, szLogName, strlen(szLogName));
		log.fp  = fopen(szLogName, "ab+");
		if (log.fp)
		{
			m_logfile.insert(std::make_pair(std::string(szLogName), log));
		}
		fp = log.fp;
	}
	return fp;
}

//写日志，并根据大小重命名
void CLogMgr::WriteLogFile(LOGS& log)
{
	SafeLock safelock(&cs_logfile);

	FILE *fp = NULL;
	MAP_LOGFILE::iterator it = m_logfile.find(log.szLogName);
	//存在则判断日志文件大小，重命名/重新生成文件
	if (it != m_logfile.end())
	{
		fp = it->second.fp;
		if (ftell(fp) > 1024*1024*log.nLogSize)
		{
			//关闭文件
			fclose(fp);
			//删除日志文件队列
			m_logfile.erase(it);
			//重命名日志
			RenameLog(log.szLogPath, log.szNewLogPath);
			
			//重新生成日志
			LOGFILE logfile;
			memcpy(logfile.szLogName, log.szLogName, strlen(log.szLogName));
			logfile.fp  = fopen(log.szLogPath, "ab+");
			if (logfile.fp)
			{
				m_logfile.insert(std::make_pair(std::string(logfile.szLogName), logfile));
			}
			else
			{
				return;
			}
			fp = logfile.fp;
		}		
	}
	//不存在则插入一项记录，并生成新的文件指针
	else
	{
		//重新生成日志
		LOGFILE logfile;
		memcpy(logfile.szLogName, log.szLogName, strlen(log.szLogName));
		logfile.fp  = fopen(log.szLogPath, "ab+");
		if (logfile.fp)
		{
			m_logfile.insert(std::make_pair(std::string(logfile.szLogName), logfile));
		}
		else
		{
			return;
		}
		fp = logfile.fp;
	}
	if (fp)
	{
		fputs(log.strPreFix.c_str(), fp);
		fputs(log.LogPtr, fp);
		fputs(log.strPostFix.c_str(), fp);
		//fwrite(log.szLog.c_str(), 1, log.szLog.size(), fp);
		fflush(fp);
	}
}

//添加到日志队列
void CLogMgr::WriteLogFile(const char *szPreFix, /*ref_ptr<char*>*/const char* LogPtr, const char *szPostFix, int nLogLen, int nLogSize, const char *szLogName, const char *szNewLogName, const char *szLogPath, int nLogLevel)
{
	//启动线程模式且线程已启动且日志级别大于0
	if (0/*m_bThrMode && m_bThrStart && nLogLevel > MAX_LOG_LEVEL_DEFAULT*//* && m_LogQue.GetCount() < 10000*/)
	{
		LOGS log;
		log.strPreFix	= szPreFix;
		log.LogPtr		= (char*)LogPtr;
		log.strPostFix	= szPostFix;
		memcpy(log.szLogName, szLogName, strlen(szLogName));
		memcpy(log.szNewLogName, szNewLogName, strlen(szNewLogName));
		memcpy(log.szLogPath, szLogPath, strlen(szLogPath));
		strcat(log.szLogPath, szLogName);
		memcpy(log.szNewLogPath, szLogPath, strlen(szLogPath));
		strcat(log.szNewLogPath, szNewLogName);
		log.nLogSize = nLogSize;

		m_LogQue.AddTail(log);
		//if (m_hCanWrite)
		{
		    CAdapter::SetCompatibleEvent(m_hCanWrite);
			//SetEvent(m_hCanWrite);
		}
	}
	//普通模式
	else
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

		SafeLock safelock(&cs_logfile);

		FILE *fp = NULL;
		MAP_LOGFILE::iterator it = m_logfile.find(szLogName);
		//存在则判断日志文件大小，重命名/重新生成文件
		if (it != m_logfile.end())
		{
			fp = it->second.fp;
			if (ftell(fp) > 1024*1024*nLogSize)
			{
				//关闭文件
				fclose(fp);
				//删除日志文件队列
				m_logfile.erase(it);
				//重命名日志
				RenameLog(szOldName, szNewName);
				
				//重新生成日志
				LOGFILE logfile;
				memcpy(logfile.szLogName, szLogName, strlen(szLogName));
				logfile.fp  = fopen(szOldName, "ab+");
				if (logfile.fp)
				{
					m_logfile.insert(std::make_pair(std::string(logfile.szLogName), logfile));
				}
				else
				{
					return;
				}
				fp = logfile.fp;
			}		
		}
		//不存在则插入一项记录，并生成新的文件指针
		else
		{
			//重新生成日志
			LOGFILE logfile;
			memcpy(logfile.szLogName, szLogName, strlen(szLogName));
			logfile.fp  = fopen(szOldName, "ab+");
			if (logfile.fp)
			{
				m_logfile.insert(std::make_pair(std::string(logfile.szLogName), logfile));
			}
			else
			{
				return;
			}
			fp = logfile.fp;
		}
		if (fp)
		{
			fputs(szPreFix, fp);
			fputs(LogPtr, fp);
			fputs(szPostFix, fp);
			fflush(fp);
		}
	}
}

//添加到日志队列
void CLogMgr::WriteLogFileEx(const char *szLog, int nLogLen, int nLogSize, const char *szLogName, const char *szNewLogName, const char *szLogPath, int nLogLevel)
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

	SafeLock safelock(&cs_logfile);

	FILE *fp = NULL;
	MAP_LOGFILE::iterator it = m_logfile.find(szLogName);
	//存在则判断日志文件大小，重命名/重新生成文件
	if (it != m_logfile.end())
	{
		fp = it->second.fp;
		if (ftell(fp) > 1024*1024*nLogSize)
		{
			//关闭文件
			fclose(fp);
			//删除日志文件队列
			m_logfile.erase(it);
			//重命名日志
			RenameLog(szOldName, szNewName);
			
			//重新生成日志
			LOGFILE logfile;
			memcpy(logfile.szLogName, szLogName, strlen(szLogName));
			logfile.fp  = fopen(szOldName, "ab+");
			if (logfile.fp)
			{
				m_logfile.insert(std::make_pair(std::string(logfile.szLogName), logfile));
			}
			else
			{
				return;
			}
			fp = logfile.fp;
		}		
	}
	//不存在则插入一项记录，并生成新的文件指针
	else
	{
		//重新生成日志
		LOGFILE logfile;
		memcpy(logfile.szLogName, szLogName, strlen(szLogName));
		logfile.fp  = fopen(szOldName, "ab+");
		if (logfile.fp)
		{
			m_logfile.insert(std::make_pair(std::string(logfile.szLogName), logfile));
		}
		else
		{
			return;
		}
		fp = logfile.fp;
	}
	if (fp)
	{
		fputs(szLog, fp);
		//fwrite(strLog, 1, nLogLen, fp);
		fflush(fp);
	}
}

//从日志队列中删除该日志文件，并关闭文件指针
void CLogMgr::DelLogFile(const char *szLogName)
{
	SafeLock safelock(&cs_logfile);
	MAP_LOGFILE::iterator it = m_logfile.find(szLogName);
	if (it != m_logfile.end())
	{
		fclose(it->second.fp);
		m_logfile.erase(it);
	}
}

//清空日志队列，并关闭文件指针
void CLogMgr::DelAllLogFile()
{
	SafeLock safelock(&cs_logfile);
	MAP_LOGFILE::iterator it = m_logfile.begin();
	for (it; it != m_logfile.end();)
	{
		fclose(it->second.fp);
		m_logfile.erase(it++);
	}
}

//线程写日志
unsigned int __STDCALL CLogMgr::ThreadWriteLog(LPVOID pParam)
{
	CLogMgr *pLogMgr = (CLogMgr *)pParam;
	if (NULL == pLogMgr)
	{
		return 0;
	}
	
	bool bClear = true;
	while (1)
	{
		try
		{
			while (!pLogMgr->m_LogQue.IsEmpty())
			{
				LOGS Log;
				if (-1 != pLogMgr->m_LogQue.GetHead(Log))
				{
					//写日志
					pLogMgr->WriteLogFile(Log);
					
					//每天0时关闭前一天的文件指针
					//SYSTEMTIME systime;
					//GetLocalTime(&systime);
                     //zhubo 2017-4-7
            		//SYSTEMTIME t;
            		//GetLocalTime(&t);
                    //zhubo 2017-4-7
        
                    SYSTEMTIME_EX tt;
                    CAdapter::GetLocalTime(&tt);
                    SYSTEMTIME_EX_LINUX systime(tt);
                    
					if (0 == systime.wHour && bClear)
					{
						pLogMgr->DelAllLogFile();
						bClear = false;
					}
					if (0 != systime.wHour)
					{
						bClear = true;
					}
				}
            }
            CAdapter::WaitForCompatibleEvent(pLogMgr->m_hCanWrite, 60*1000);
            //zhubo 2017-4-7
            //CAdapter::WaitForCompatibleEvent(pLogMgr->m_hCanWrite, INFINITE);
			//WaitForSingleObject(pLogMgr->m_hCanWrite, INFINITE);
		}
		catch (...)
		{
		}
	}
	pLogMgr->m_bThrStart = false;
	return 0;
}

BOOL CLogMgr::GetMoRptModule(char* szFileMoShare, char* szFileRptShare)
{
	BOOL bRet = FALSE;
	char szTmp[MAX_PATH] = { 0 };
	char szPath[MAX_PATH] = { 0 };
	char szWbsLocalName[MAX_PATH] = { 0 };
	CAdapter::GetModuleFileName(szPath, MAX_PATH);
    /*******************zhubo 2017-4-12**********************/
    /*
	CString strPath(szPath);
	strPath = strPath.Left(strPath.ReverseFind('\\') + 1);
	strcpy(szPath, strPath);
	strcat(szPath, "\\cmppgw.ini");  //SP网关程序
	*/
	/*******************zhubo 2017-4-12**********************/
    strcat(szPath, "cmppgw.ini");  //SP网关程序

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
