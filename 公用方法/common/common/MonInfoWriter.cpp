// PtMonitor.cpp: implementation of the CPtMonitor class.
//
//////////////////////////////////////////////////////////////////////


#include "MonInfoWriter.h"
#include "Global.h"


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
	m_nMonFreq = 10;
}

//析构:
CMonInfoWriter::~CMonInfoWriter()
{

}


CMonInfoWriter& CMonInfoWriter::GetInstance()
{
	static CMonInfoWriter MonInfoMaker;
	return MonInfoMaker;
}

std::string CMonInfoWriter::Init(const char* appVersion, const char* proVersion, int nFiltime, const char* Reserve)
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
//		GetCurrentPath(szAppPath, sizeof(szAppPath));
		CAdapter::GetModuleFileName(szAppPath, sizeof(szAppPath));
		std::string strPath(szAppPath);
		size_t found = strPath.find_last_of("/\\");
		memset(szAppPath+found+1, 0, strPath.size()-found-1);
		int nLen = strlen(szAppPath);
		for (int i = 0; i < nLen; ++i)
		{
			if (szAppPath[i] == '\\')
			{
				szAppPath[i] = '/';
			}
		}
		
		ConverToXmlValue(szAppPath, m_strAppPath);

		if (m_strAppVersion.empty() && appVersion != NULL)
		{
			ConverToXmlValue(appVersion, m_strAppVersion);
		}		
		
		if (m_strProVersion.empty() && proVersion != NULL)
		{
			ConverToXmlValue(proVersion, m_strProVersion);
		}	

		#ifdef _WIN64
    		m_hMonitorDll = CAdapter::LoadLibrary(MON_DLL_NAME);
		#else
    		m_hMonitorDll = CAdapter::LoadLibrary(MON_DLL_NAME);
    		char* perr = dlerror();
		#endif


	    if (NULL != m_hMonitorDll)
	    {
			m_pInitMonitor = (_InitMonitor)CAdapter::GetProcAddress(m_hMonitorDll, MON_DLL_IF_INIT);
			m_pExitMonitor = (_ExitMonitor)CAdapter::GetProcAddress(m_hMonitorDll, MON_DLL_IF_EXIT);
			m_pWriteMonXml = (_WriteMonXml)CAdapter::GetProcAddress(m_hMonitorDll, MON_DLL_IF_WRITE);
  			m_pWriteMonXmlV2 = (_WriteMonXmlV2)CAdapter::GetProcAddress(m_hMonitorDll, MON_DLL_IF_WRITEV2);
			if (m_pInitMonitor == NULL || m_pExitMonitor == NULL || m_pWriteMonXml == NULL || m_pWriteMonXmlV2 == NULL)
			{
				//ASSERT(FALSE);
				CAdapter::FreeLibrary(m_hMonitorDll);				
				strErr = "加载API接口失败";
				break;
			}
						
			try
			{				
				m_nLastDllErr = m_pInitMonitor(szAppPath, m_strAppVersion.c_str(), nFiltime,	0);
				if (0 != m_nLastDllErr)
				{
					char szNum[16];
					strErr = "调用接口 InitXMLMon 失败返回:";
					strErr += CAdapter::itoa(m_nLastDllErr, szNum,  10);					
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


void  CMonInfoWriter::UnInit()
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

void CMonInfoWriter::SetFreq(int nMonFreq)
{
	m_nMonFreq = nMonFreq;
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

/************************************************************************/
/* 取当前时间                                                           */
/************************************************************************/
std::string CMonInfoWriter::GetCurrentFormatTime(BOOL bAddMilliscd, int nFreqSW)
{
	//当前时间
	SYSTEMTIME_EX tNow;
	GetWbsLocalTime(&tNow);
	tm temptm = {tNow.m_iSecond, 	
		tNow.m_iMinute, 	
		tNow.m_iHour, 	
		tNow.m_iDay, 	
		tNow.m_iMonth - 1, 	
		tNow.m_iYear - 1900, 		
		tNow.m_iDayOfWeek, 		
		0, 		
		0};
	
	time_t tAllSeconds = mktime(&temptm);
	
	char szCurrentTm[256]={0};
	
	//调整输出时间
	int nSecond = tNow.m_iSecond;
	if (nFreqSW > 0)
	{
		//总的秒数 - 总的秒数%周期 = 格式化后的。
		tAllSeconds = tAllSeconds - tAllSeconds%nFreqSW;
	}
	
	struct tm *outTime = localtime(&tAllSeconds);
	//格式化时间
	if (bAddMilliscd)
	{
		sprintf(szCurrentTm,"%d-%02d-%02d %02d:%02d:%02d.%03d",
			outTime->tm_year+1900, outTime->tm_mon+1, outTime->tm_mday, 
			outTime->tm_hour, outTime->tm_min, outTime->tm_sec,tNow.m_iMilliseconds);
	}
	else
	{
		sprintf(szCurrentTm,"%d-%02d-%02d %02d:%02d:%02d",
			outTime->tm_year+1900, outTime->tm_mon+1, outTime->tm_mday, 
			outTime->tm_hour, outTime->tm_min, outTime->tm_sec);
	}
	
	return std::string(szCurrentTm);
}

bool CMonInfoWriter::WriteMonXml(int eventType, int eventLevel, const char* eventWhere, const char* eventWho, const char* eventOther, const char* eventContent)
{
	bool bRet = false;
	if (NULL != m_pWriteMonXml)
	{		
		try
		{
			int nLastDllErr = m_pWriteMonXml(eventType, eventLevel, eventContent,  eventWhere, eventWho, eventOther);
			//ASSERT(0 == nLastDllErr);
			bRet = 0==nLastDllErr;
		}
		catch(...)
		{
			//ASSERT(false);
		}
	}
	
	return bRet;
}

bool CMonInfoWriter::WriteMonXmlV2(int nEventType, int nEventId, /*const char* szEventTm,*/ const char* szEventContent)
{
	bool bRet = false;
	if (NULL != m_pWriteMonXmlV2)
	{		
		try
		{
			std::string szTime;
			
			switch(nEventId)
			{
// 			case MON_EVENT_MGATEWAY_SYS_BUFFER:
// 			case MON_EVENT_MGATEWAY_ALL_USER:
// 			case MON_EVENT_MGATEWAY_ALL_SPGATE:
// 			case MON_EVENT_MGATEWAY_ALL_SPGATETWO:
			case MON_EVENT_APP_SYS_INFO:
				szTime = GetCurrentFormatTime(TRUE, m_nMonFreq);
				break;
				
// 			case MON_EVENT_MGATEWAY_SAMPLE://抽样
// 				szTime = GetCurrentFormatTime(TRUE, m_nSampleFreq);
// 				break;
				
			default:
				szTime = GetCurrentFormatTime(TRUE, 0);
				break;
			}
			
			int nLastDllErr = m_pWriteMonXmlV2(nEventType, nEventId, szTime.c_str(), szEventContent);
			//ASSERT(0 == nLastDllErr);
			bRet = 0==nLastDllErr;
		}
		catch(...)
		{
			//ASSERT(false);
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

		std::string s;
		std::string sTemp = cstr::format(" \"STATUS\":\"0\", \"VER\":\"%s\", \"PATH\":\"%s\",  \"INFO\":\"%s\" ", m_strProVersion.c_str(), m_strAppPath.c_str(), strStateInfo.c_str());
		AddJsonContext(s, sTemp, TRUE);
		bRet = WriteMonXmlV2(MON_EVENT_TYPE_ERROR_OK, MON_EVENT_APP_START_OK, s.c_str());
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

		std::string s;
		std::string sTemp = cstr::format(" \"STATUS\":\"1\", \"VER\":\"%s\", \"PATH\":\"%s\",  \"INFO\":\"%s\" ", m_strProVersion.c_str(), m_strAppPath.c_str(), strStateInfo.c_str());
		AddJsonContext(s, sTemp, TRUE);
		bRet = WriteMonXmlV2(MON_EVENT_TYPE_ERROR, MON_EVENT_APP_START_OK, s.c_str());//AAAAAA
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

		std::string s;
		std::string sTemp = cstr::format(" \"STATUS\":\"1\", \"VER\":\"%s\", \"PATH\":\"%s\",  \"INFO\":\"%s\" ", m_strProVersion.c_str(), m_strAppPath.c_str(), strStateInfo.c_str());
		AddJsonContext(s, sTemp, TRUE);
		bRet = WriteMonXmlV2(MON_EVENT_TYPE_ERROR, MON_EVENT_APP_START_OK, s.c_str());//AAAAAA

		//std::string s = cstr::format("<INFO VER:\"%s\" PATH:\"%s\"  INFO:\"%s\" />", m_strAppVersion.c_str(), m_strAppPath.c_str(), strStateInfo.c_str());		
		//bRet = WriteMonXml(MON_EVENT_APP_CLOSED, MON_EVENT_TYPE_WARN, "", "", "", s.c_str());
	}
	catch(...)
	{
		
	}	
	return bRet;
}

//写应用所占用的系统资源信息
bool CMonInfoWriter::WriteAppSysInfo(int nFlag)
{
	MON_SYSINFO sysinfo;
	sysinfo.nCpuUsage = get_cpu_usage();
	sysinfo.nCpuUsage<0?sysinfo.nCpuUsage=0:1;
	uint64_t I64Mem = 0;
	uint64_t I64vMem= 0;
	get_memory_usage(&I64Mem, &I64vMem);
	sysinfo.nMemUsage  = I64Mem/(1024*1024);
	sysinfo.nVmemUsage = I64vMem/(1024*1024);	
	sysinfo.nDiskFreeSpace = get_disk_free_space(m_strAppPath.c_str());
	return WriteAppSysInfo(sysinfo, nFlag);
}

bool CMonInfoWriter::WriteAppSysInfo(const MON_SYSINFO &sysinfo, int nFlag)
{
	bool bRet = false;
	try
	{
		std::string s;
		std::string sTemp;
		switch (nFlag)
		{
		case 0:
		{
			sTemp = cstr::format(" \"VER\":\"%s\", \"CPU\":\"%d\", \"MEM\":\"%d\", \"VMEM\":\"%d\", \"DISKFREE\":\"%d\", \"STATUS\":\"0\", \"MSG\":\"正常\" ", m_strProVersion.c_str(), sysinfo.nCpuUsage, sysinfo.nMemUsage, sysinfo.nVmemUsage, sysinfo.nDiskFreeSpace);
			AddJsonContext(s, sTemp, TRUE);
		}
			break;
		case 1:
		{
			sTemp = cstr::format(" \"VER\":\"%s\", \"CPU\":\"%d\", \"MEM\":\"%d\", \"VMEM\":\"%d\", \"DISKFREE\":\"%d\", \"STATUS\":\"1\", \"MSG\":\"业务数据长时间未更新\" ", m_strProVersion.c_str(), sysinfo.nCpuUsage, sysinfo.nMemUsage, sysinfo.nVmemUsage, sysinfo.nDiskFreeSpace);
			AddJsonContext(s, sTemp, TRUE);
		}
			break;
		case 2:
		{
			sTemp = cstr::format(" \"VER\":\"%s\", \"CPU\":\"%d\", \"MEM\":\"%d\", \"VMEM\":\"%d\", \"DISKFREE\":\"%d\", \"STATUS\":\"2\", \"MSG\":\"存在严重告警\" ", m_strProVersion.c_str(), sysinfo.nCpuUsage, sysinfo.nMemUsage, sysinfo.nVmemUsage, sysinfo.nDiskFreeSpace);
			AddJsonContext(s, sTemp, TRUE);
		}
			break;
		default:
			break;
		}

		bRet = WriteMonXmlV2(MON_EVENT_TYPE_BUSSINESSINFO, MON_EVENT_APP_SYS_INFO, s.c_str());//AAAAAA
	}
	catch (...)
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
		
		std::string s;
		std::string sTemp = cstr::format(
			" ID:\"%u\" NAME:\"%s\" STATUS:\"%d\" ERRCNT:\"%d\"  LASTERTM:\"%s\"  LASTRUNTM:\"%s\"  THRLOGS:\"%s\" "
			,ThreadId, ThreadName, Status, ErrCnt,  LastErrTime,  LastRunTime, strThreadLog.c_str());
		
		AddJsonContext(s, sTemp, TRUE);
		bRet = WriteMonXmlV2(MON_EVENT_TYPE_BASEINFO, MON_EVENT_APP_THREAD_INFO, s.c_str());//AAAAAAA文档上没有1034也没用线程方面的内容,而且函数没被调用。。。

	}
	catch(...)
	{
		
	}	
	return bRet;
}


bool CMonInfoWriter::WriteAppNetErrInfo(bool bIsErrInfo, int nMODCLS, const char* Msg, const char* szWho, const char *szValue)//AAAAAA函数没被调用。。。
{
	bool bRet = false;
	try
	{
		if (!Msg || !szWho || !szValue)
		{
			return bRet;
		}

		std::string strMsg;	
		ConverToXmlValue(Msg, strMsg);

		std::string s;
		std::string sTemp = cstr::format(" \"MODCLS\":\"%d\", \"WHO\":\"%s\", \"VALUE\":\"%s\", \"MSG\":\"%s\" ", nMODCLS, szWho, szValue, strMsg.c_str());
		AddJsonContext(s, sTemp, TRUE);

		bRet = WriteMonXmlV2(bIsErrInfo?MON_EVENT_TYPE_ERROR:MON_EVENT_TYPE_ERROR_OK, MON_EVENT_APP_COMMON_ERR, s.c_str());
	}
	catch(...)
	{
		
	}	
	return bRet;
}


bool CMonInfoWriter::WriteAppDbErrInfo(bool bIsErrInfo, int nMODCLS, const char* Msg, const char* szWho, const char *szValue)
{
	bool bRet = false;
	try
	{
		if (!Msg || !szWho || !szValue)
		{
			return bRet;
		}
		
		std::string strMsg;				
		ConverToXmlValue(Msg, strMsg);
		
		std::string s;
		std::string sTemp = cstr::format(" \"MODCLS\":\"%d\", \"WHO\":\"%s\", \"VALUE\":\"%s\", \"MSG\":\"%s\" ", nMODCLS, szWho, szValue, strMsg.c_str());
		AddJsonContext(s, sTemp, TRUE);
		
		WriteMonXmlV2(bIsErrInfo?MON_EVENT_TYPE_ERROR:MON_EVENT_TYPE_ERROR_OK, MON_EVENT_APP_COMMON_ERR, s.c_str());
		
	}
	catch(...)
	{
		
	}	
	return bRet;
}

//写应用程序其他异常信息
bool CMonInfoWriter::WriteAppOtherErrInfo(bool bIsErrInfo, int nMODCLS, const char* Msg, const char* szWho, const char *szValue)
{
	bool bRet = false;
	try
	{	
		if (!Msg || !szWho || !szValue)
		{
			return bRet;
		}
		
		std::string strMsg;
		ConverToXmlValue(Msg, strMsg);
		
		std::string s;
		std::string sTemp = cstr::format(" \"MODCLS\":\"%d\", \"WHO\":\"%s\", \"VALUE\":\"%s\", \"MSG\":\"%s\" ", nMODCLS, szWho, szValue, strMsg.c_str());
		AddJsonContext(s, sTemp, TRUE);
		
		WriteMonXmlV2(bIsErrInfo?MON_EVENT_TYPE_ERROR:MON_EVENT_TYPE_ERROR_OK, MON_EVENT_APP_COMMON_ERR, s.c_str());
		
	}
	catch(...)
	{
		
	}	
	return bRet;
}

//写应用程序其他信息
bool CMonInfoWriter::WriteAppOtherInfo(int eventLevel, int eventType, const char* xmlMsg)
{
	bool bRet = WriteAppOtherInfoV2(eventLevel, eventType, xmlMsg);
	return bRet;	
}

//替换目标
//把"<M_REPLACEV2 "部分去掉， 把"/>"去掉
//!!!如果出现多对需用大括号替换!!!
void ReplaceSpecifyXml(std::string &strXml)
{
	char szOldHead[1024+1] = {0};
	char szOldHead2[1024+1] = {0};
	const char* szOldHeadTmp		   = "<M_REPLACEV2";
	const char* szOldHead2Tmp       = "<";
	memcpy(szOldHead, szOldHeadTmp, min(strlen(szOldHeadTmp), sizeof(szOldHead)-1));
	memcpy(szOldHead2, szOldHead2Tmp, min(strlen(szOldHead2Tmp), sizeof(szOldHead2)-1));
	const char* szOldEnd   = "/>";
	const char* szNewHead  = "{";
	const char* szNewHead2 = ",{";
	const char* szNewEnd   = "}";
	
	int nOldHeadLen  = strlen(szOldHead);
	int nOldHeadLen2 = strlen(szOldHead2);
	int nOldHeadEnd  = strlen(szOldEnd);
	int nNewHeadLen	 = strlen(szNewHead);
	int nNewHeadLen2 = strlen(szNewHead2);
	int nNewEndLen	 = strlen(szNewEnd);
	
	int nCntEnd		 = 0;//统计有多少结尾符
	bool bFoundHead  = true;//交替查找替换 
	size_t found  	 = strXml.find(szOldHead);
	if (found == std::string::npos)
	{
		//szOldHead   = szOldHead2;
		memcpy(szOldHead,   szOldHead2, min(strlen(szOldHead2), sizeof(szOldHead)-1));
		nOldHeadLen = nOldHeadLen2;
		//用标准的找
		found = strXml.find(szOldHead);
	}
	
	while (found != std::string::npos)
	{
		if (bFoundHead)
		{
			if(nCntEnd > 0)//有多组的
			{
				strXml.replace(found, nOldHeadLen, szNewHead2);
				found += nNewHeadLen2;
			}
			else
			{
				strXml.replace(found, nOldHeadLen, szNewHead);
				found += nNewHeadLen;
			}
		}
		else
		{
			strXml.replace(found, nOldHeadEnd, szNewEnd);
			found += nNewEndLen;
			nCntEnd++;
		}
		bFoundHead = !bFoundHead;
		
		if(bFoundHead)
			found = strXml.find(szOldHead, found);
		else
			found = strXml.find(szOldEnd, found);
	}
	
	if(nCntEnd > 1)
	{
		//组合方括号
		strXml = "[" + strXml;//<<<---Start
		strXml += "]";//---End>>>
	}
	else if (nCntEnd < 1)
	{
		//一个标签都没有组合大括号
		strXml = "{" + strXml;//<<<---Start
		strXml += "}";//---End>>>
	}
	
	//dll里会自动加
	// 	strXml = "<EVTCONT> " + strXml;//<<<---Start
	// 	strXml += " </EVTCONT>";//---End>>>
}

//写应用程序其他信息V2
bool CMonInfoWriter::WriteAppOtherInfoV2(int eventType, int eventID, const char* xmlMsg)
{
	if (!xmlMsg)
	{
		return false;
	}
	
	std::string strXmlMsg = xmlMsg;
	//老的接口统一在这替换，nomi
	ReplaceSpecifyXml(strXmlMsg);
	
	bool bRet = WriteMonXmlV2(eventType, eventID, strXmlMsg.c_str());
	return bRet;	
}

//增加内容
void CMonInfoWriter::AddJsonContext(std::string &strContent, const std::string strInsertContent, BOOL bLastInfo, BOOL bLastBracet)
{
	strContent += strInsertContent;
	if (!bLastInfo)
	{
		//每条非结尾信息
		strContent += ",";
	}
	else
	{
		if(strContent.find("},") != std::string::npos)
		{
			//补全大括号
			strContent += "}";//---End>>>
		}
		else
		{
			//第一组大括号
			strContent = "{" + strContent;//<<<---Start
			strContent += "}";//---End>>>
		}
		
		//非最后一组信息
		if (!bLastBracet)
		{
			strContent += ",";
			strContent += "{";//<<<---Start
		}
		else
		{
			//最后一组信息判断是否要加大括号
			if(strContent.find("},") != std::string::npos)
			{
				//方括号
				strContent = "[" + strContent;//<<<---Start
				strContent += "]";//---End>>>
			}
			
			//json标签
			//dll里会自动加
			// 			strContent = "<EVTCONT> " + strContent;//<<<---Start
			// 			strContent += " </EVTCONT>";//---End>>>
		}
	}
}


//**********************************
//Description: 获取当前的系统时间
// Parameter:  LPSYSTEMTIME_EX lpSystemTime_ex 自定义的拓展系统时间
//              和windows下SYSTEMTIME成员一致，数据类型改为32位int
// Returns:    void
//
//************************************
void CMonInfoWriter::GetWbsLocalTime(LPSYSTEMTIME_EX lpSystemTime_ex)
{
	//WINDOWS编译环境
#ifdef WIN32
    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);
    lpSystemTime_ex->m_iYear = sysTime.wYear;
    lpSystemTime_ex->m_iMonth = sysTime.wMonth;
    lpSystemTime_ex->m_iDayOfWeek = sysTime.wDayOfWeek;
    lpSystemTime_ex->m_iDay = sysTime.wDay;
    lpSystemTime_ex->m_iHour = sysTime.wHour;
    lpSystemTime_ex->m_iMinute = sysTime.wMinute;
    lpSystemTime_ex->m_iSecond = sysTime.wSecond;
    lpSystemTime_ex->m_iMilliseconds = sysTime.wMilliseconds;
	//linux编译环境
#elif defined LINUX
    struct timeval tv;
    struct tm      t;
    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &t);
    //linuxAPI mktime的年份需要加1900
    lpSystemTime_ex->m_iYear = t.tm_year + 1900;
    //linux获取的月份是从0-11，windows为1-12，为兼容做加1操作
    lpSystemTime_ex->m_iMonth = t.tm_mon + 1;
    lpSystemTime_ex->m_iDayOfWeek = t.tm_wday;
    lpSystemTime_ex->m_iDay = t.tm_mday;
    lpSystemTime_ex->m_iHour = t.tm_hour;
    lpSystemTime_ex->m_iMinute = t.tm_min;
    lpSystemTime_ex->m_iSecond = t.tm_sec;
    lpSystemTime_ex->m_iMilliseconds = (int)(tv.tv_usec)/1000;
	
#endif

}
