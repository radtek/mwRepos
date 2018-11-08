// PtMonitor.cpp: implementation of the CPtMonitor class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "MonInfoWriter.h"
#include "../../common/Global.h"
#include "../../public/common/process_stat.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//构造：获取应用地球路径
CMonInfoWriter::CMonInfoWriter()
{	
	m_nLastDllErr = 0;
	m_hMonitorDll = 0;
	m_pInitMonitor = NULL;
	m_pExitMonitor = NULL;
	m_pWriteMonXml = NULL;
}

//析构:
CMonInfoWriter::~CMonInfoWriter()
{
	if (m_hMonitorDll)
	{
		try
		{
			m_pExitMonitor();
		}
		catch(...)
		{
			m_nLastDllErr = 98;
		}
		CAdapter::FreeLibrary(m_hMonitorDll);
	}
}


CMonInfoWriter& CMonInfoWriter::GetInstance()
{
	static CMonInfoWriter MonInfoMaker;
	return MonInfoMaker;
}

std::string CMonInfoWriter::Init(const char* appVersion, int nFiltime, const char* Reserve)
{	

	std::string strErr;

	if (NULL != m_hMonitorDll)
	{
		return strErr;
	}

	do 
	{		
		char szAppPath[512] = {0};
		memset(szAppPath, 0, sizeof(szAppPath));
		CAdapter::GetCurrentPath(szAppPath, sizeof(szAppPath));
		ConverToXmlValue(szAppPath, m_strAppPath);

		if (m_strAppVersion.empty() && appVersion != NULL)
		{
			ConverToXmlValue(appVersion, m_strAppVersion);
		}	
		
		if (m_hMonitorDll = CAdapter::LoadLibrary(MON_DLL_NAME) )
		{
			m_pInitMonitor = (_InitMonitor)CAdapter::GetProcAddress(m_hMonitorDll, MON_DLL_IF_INIT);
			m_pExitMonitor = (_ExitMonitor)CAdapter::GetProcAddress(m_hMonitorDll, MON_DLL_IF_EXIT);
			m_pWriteMonXml = (_WriteMonXml)CAdapter::GetProcAddress(m_hMonitorDll, MON_DLL_IF_WRITE);
			if (m_pInitMonitor == NULL || m_pExitMonitor == NULL || m_pWriteMonXml == NULL)
			{
				ASSERT(FALSE);
				CAdapter::FreeLibrary(m_hMonitorDll);				
				strErr = "加载API接口失败";
				break;
			}
						
			try
			{				
				m_nLastDllErr = m_pInitMonitor(szAppPath, nFiltime,	0);
				if (0 != m_nLastDllErr)
				{
					char szNum[16];
					strErr = "调用接口 InitXMLMon 失败返回:";
					strErr += itoa(m_nLastDllErr, szNum,  10);					
				}
			}
			catch(...)
			{
				strErr = "调用接口 InitXMLMon 异常";				
			}			
		}
		else
		{
			strErr = "加载动态库MonXmlDll.dll失败";			
		}
	} while (0);
	
	if (!strErr.empty() && NULL != m_hMonitorDll)
	{
		CAdapter::FreeLibrary(m_hMonitorDll);
		m_hMonitorDll = NULL;
		m_pInitMonitor = NULL;
		m_pExitMonitor = NULL;
		m_pWriteMonXml = NULL;
	}
#ifdef DEBUG
#ifdef _TEST_CLASS_
	return "";
#endif
	#endif
	return strErr;
	
}

void CMonInfoWriter::ConverToXmlValue(const char* pValue, std::string &XmlValue)
{
	if (pValue && *pValue)
	{
		XmlValue = pValue;
		ConverToXmlValue(XmlValue);
	}	
}

std::string& CMonInfoWriter::ConverToXmlValue(std::string &strValue)
{
	cstr::replace(strValue, "<", "&lt;");
	cstr::replace(strValue, ">", "&gt;");
	cstr::replace(strValue, "\"", "&quot;");
	cstr::replace(strValue, "&", "&amp;");
	cstr::replace(strValue, "\'", "&apos;");
	return strValue;
}

std::string CMonInfoWriter::ConverToXmlValue(const char* pValue)
{
	std::string strValue(pValue?pValue:"");
	return ConverToXmlValue(strValue);
}

bool CMonInfoWriter::WriteMonXml(int eventType, int eventLevel, const char* eventWhere, const char* eventWho, const char* eventOther, const char* eventContent)
{
	bool bRet = false;
	if (NULL != m_pWriteMonXml)
	{		
		try
		{
			int nLastDllErr = m_pWriteMonXml(eventType, eventLevel, eventContent,  eventWhere, eventWho, eventOther);
			ASSERT(0 == nLastDllErr);
			bRet = 0==nLastDllErr;
		}
		catch(...)
		{
			ASSERT(false);
		}
	}
	
	return bRet;
}

//写应用启动成功信息
bool CMonInfoWriter::WriteAppStartOkInfo(const char* stateInfo)
{
	bool bRet = false;
	try
	{
		std::string strStateInfo;		
		ConverToXmlValue(stateInfo, strStateInfo);
		std::string s = cstr::format("<INFO VER=\"%s\" PATH=\"%s\"  INFO=\"%s\" />", m_strAppVersion.c_str(), m_strAppPath.c_str(), strStateInfo.c_str());
		bRet = WriteMonXml(MON_EVENT_APP_START_OK, MON_EVENT_TYPE_BASEINFO, "", "", "", s.c_str());
	}
	catch(...)
	{
		
	}	
	return bRet;
}

//写应用启动失败信息
bool CMonInfoWriter::WriteAppStartErInfo(const char* stateInfo)
{
	bool bRet = false;
	try
	{
		std::string strAppPath, strStateInfo;		
		ConverToXmlValue(stateInfo, strStateInfo);
		std::string s = cstr::format("<INFO VER=\"%s\" PATH=\"%s\"  INFO=\"%s\" />", m_strAppVersion.c_str(), m_strAppPath.c_str(), strStateInfo.c_str());
		bRet = WriteMonXml(MON_EVENT_APP_START_ER, MON_EVENT_TYPE_WARN, "", "", "", s.c_str());
	}
	catch(...)
	{
		
	}	
	return bRet;
}

//写应用被关闭时的信息
bool CMonInfoWriter::WriteAppClosedInfo(const char* stateInfo)
{
	bool bRet = false;
	try
	{
		std::string strStateInfo;		
		ConverToXmlValue(stateInfo, strStateInfo);
		std::string s = cstr::format("<INFO VER=\"%s\" PATH=\"%s\"  INFO=\"%s\" />", m_strAppVersion.c_str(), m_strAppPath.c_str(), strStateInfo.c_str());		
		bRet = WriteMonXml(MON_EVENT_APP_CLOSED, MON_EVENT_TYPE_WARN, "", "", "", s.c_str());
	}
	catch(...)
	{
		
	}	
	return bRet;
}

//写应用所占用的系统资源信息
bool CMonInfoWriter::WriteAppSysInfo()
{
	MON_SYSINFO sysinfo;
	sysinfo.nCpuUsage = get_cpu_usage();
	sysinfo.nCpuUsage<0?sysinfo.nCpuUsage=0:1;
	unsigned __int64 I64Mem = 0;
	unsigned __int64 I64vMem= 0;
	get_memory_usage(&I64Mem, &I64vMem);
	sysinfo.nMemUsage  = I64Mem/(1024*1024);
	sysinfo.nVmemUsage = I64vMem/(1024*1024);	
	sysinfo.nDiskFreeSpace = get_disk_free_space(m_strAppPath.c_str());
	return WriteAppSysInfo(sysinfo);
}

bool CMonInfoWriter::WriteAppSysInfo(const MON_SYSINFO &sysinfo)
{
	bool bRet = false;
	try
	{
		std::string s = cstr::format("<INFO CPU=\"%d\" MEM=\"%d\" VMEM=\"%d\" DISKFREE=\"%d\" />", sysinfo.nCpuUsage, sysinfo.nMemUsage, sysinfo.nVmemUsage, sysinfo.nDiskFreeSpace);
		bRet = WriteMonXml(MON_EVENT_APP_SYS_INFO, MON_EVENT_TYPE_BASEINFO, "", "", "", s.c_str());
	}
	catch(...)
	{
		
	}	
	return bRet;
}

//写应用程序线程信息（输出线程ID，线程名，状态，异常次数，最后一次异常时间，上一次线程运行的时间，线程日志）	
bool CMonInfoWriter::WriteAppThreadInfo(unsigned int ThreadId, const char* ThreadName, int Status, unsigned int ErrCnt,  const char* LastErrTime,  const char* LastRunTime, const char* ThreadLog)
{
	
	bool bRet = false;
	try
	{
		std::string strThreadLog;
		ConverToXmlValue(ThreadLog, strThreadLog);
		
		std::string s = cstr::format(
			"<INFO ID=\"%u\" NAME=\"%s\" STATUS=\"%d\" ERRCNT=\"%d\"  LASTERTM=\"%s\"  LASTRUNTM=\"%s\"  THRLOGS=\"%s\" />"
			,ThreadId, ThreadName, Status, ErrCnt,  LastErrTime,  LastRunTime, strThreadLog.c_str());
		bRet = WriteMonXml(MON_EVENT_APP_THREAD_INFO, MON_EVENT_TYPE_BASEINFO, "", "", "", s.c_str());
	}
	catch(...)
	{
		
	}	
	return bRet;
}


bool CMonInfoWriter::WriteAppNetErrInfo(bool bIsErrInfo, const char* Where, const char* Who, const char* Value, const char* Msg)
{
	bool bRet = false;
	try
	{
		std::string strWhere, strWho, strValue, strMsg;		
		ConverToXmlValue(Where, strWhere);
		ConverToXmlValue(Who, strWho);
		ConverToXmlValue(Value, strValue);
		ConverToXmlValue(Msg, strMsg);
		std::string s = cstr::format("<INFO WHERE=\"%s%\" WHO=\"%s%\" VALUE=\"%s%\" MSG=\"%s%\" />", strWhere.c_str(), strWho.c_str(), strValue.c_str(), strMsg.c_str());
		bRet = WriteMonXml(MON_EVENT_APP_NET_ERR, bIsErrInfo?MON_EVENT_TYPE_ERROR:MON_EVENT_TYPE_ERROR_OK, "", "", "", s.c_str());
	}
	catch(...)
	{
		
	}	
	return bRet;
}


bool CMonInfoWriter::WriteAppDbErrInfo(bool bIsErrInfo, const char* Where, const char* Who, const char* Value, const char* Msg)
{
	bool bRet = false;
	try
	{
		std::string strWhere, strWho, strValue, strMsg;		
		ConverToXmlValue(Where, strWhere);
		ConverToXmlValue(Who, strWho);
		ConverToXmlValue(Value, strValue);		
		ConverToXmlValue(Msg, strMsg);
		std::string s = cstr::format("<INFO WHERE=\"%s%\" WHO=\"%s%\" VALUE=\"%s%\" MSG=\"%s%\" />", strWhere.c_str(), strWho.c_str(), strValue.c_str(), strMsg.c_str());
		bRet = WriteMonXml(MON_EVENT_APP_DB_ERR, bIsErrInfo?MON_EVENT_TYPE_ERROR:MON_EVENT_TYPE_ERROR_OK, "", "", "", s.c_str());
	}
	catch(...)
	{
		
	}	
	return bRet;
}

//写应用程序其他异常信息
bool CMonInfoWriter::WriteAppOtherErrInfo(bool bIsErrInfo, const char* Where, const char* Who, const char* Value, const char* Msg)
{
	bool bRet = false;
	try
	{		
		std::string strWhere, strWho, strValue, strMsg;		
		ConverToXmlValue(Where, strWhere);
		ConverToXmlValue(Who, strWho);
		ConverToXmlValue(Value, strValue);		
		ConverToXmlValue(Msg, strMsg);
		std::string s = cstr::format("<INFO WHERE=\"%s%\" WHO=\"%s%\" VALUE=\"%s%\" MSG=\"%s%\" />", strWhere.c_str(), strWho.c_str(), strValue.c_str(), strMsg.c_str());		
		bRet = WriteMonXml(MON_EVENT_APP_OTHER_ERR, bIsErrInfo?MON_EVENT_TYPE_ERROR:MON_EVENT_TYPE_ERROR_OK, "", "", "", s.c_str());
	}
	catch(...)
	{
		
	}	
	return bRet;
}

//写应用程序其他信息
bool CMonInfoWriter::WriteAppOtherInfo(int eventLevel, int eventType, const char* xmlMsg)
{
	bool bRet = WriteMonXml(eventType, eventLevel, "", "", "", xmlMsg);	
	return bRet;	
}
