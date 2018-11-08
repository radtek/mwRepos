
#include "FileBufferManage.h"
//#include <io.h>
#include <time.h>
#include <sys/timeb.h>
#include <errno.h>
//#include <SHLWAPI.H>
#include <dirent.h>

#include "../../../../00common/public/common/adapter/Adapter.h"
#include "../../../../common/common/LogMgr.h"

#define  FILE_ERROR -1

#ifdef _WIN64

	#ifdef _DEBUG
		#pragma comment(lib, "../../../00common/public/lib/LogMgr64d.lib")
	#else
		#pragma comment(lib, "../../../00common/public/lib/LogMgr64.lib")
	#endif

#else
	#pragma comment(lib, "../../../00common/public/lib/LogMgr.lib")
#endif


char header_v2[2] = { 0x6D, 0x77 };

CFileBufferManage::CFileBufferManage()
{
	m_nCurrDataCnt = 0;
	m_nMaxDataCnt = 500;
	m_nTimeOutSwitch = 20;
	m_nIndexId = 0;
	m_nDataTypeSize = 1;
	m_fpCurrfile = NULL;
	m_tLastSwitch = time(NULL);
	m_nAllDataCnt = 0;
	m_nWGNO = 0;

	memset(m_szCurrFileDir,0,sizeof(m_szCurrFileDir));
	memset(m_szVersion, 0, sizeof(m_szVersion));
}

CFileBufferManage::~CFileBufferManage(void)
{

}

bool CFileBufferManage::InitFileBuffer(const char* szDirName, int nMaxDataCount, int nTimeOut, int nDataTypeSize, const int nWGNO, const char *szVersion)
{
	if(!szDirName||!szVersion||nDataTypeSize<=0||nMaxDataCount<=0)
	{
		return false;
	}

	bool bRet = false;
	m_nMaxDataCnt = nMaxDataCount;
	m_nTimeOutSwitch = nTimeOut;
	m_nDataTypeSize = nDataTypeSize;
	m_nWGNO = nWGNO;

	size_t nMinLen = sizeof(m_szVersion)-1 > strlen(szVersion) ? strlen(szVersion) : sizeof(m_szVersion)-1;
	memcpy(m_szVersion, szVersion, nMinLen);

	CAdapter::GetCurrentPath(m_szCurrFileDir,MAX_PATH-1);
	strcat(m_szCurrFileDir, szDirName);
	CAdapter::CreateDirectory(m_szCurrFileDir, 0);
	
	//判断文件夹是否创建成功，不管之前创建是否失败只要认为存在该目录就视为目录创建成功
	if (CAdapter::PathFileExists(m_szCurrFileDir))
	{
		bRet = true;
		//重命名所有临时文件
		SwitchAllFile();
		//把文件ID加入映射表
		LoadFilePathToMap();
		//获取当前文件夹下有多少满足条件的结构体
		m_nAllDataCnt = GetAllDataSize();
	}
	
	return bRet;
}

bool CFileBufferManage::CloseCurFile()
{
	if (m_fpCurrfile)
	{
		fclose(m_fpCurrfile);
		m_fpCurrfile = NULL;

		//重命名
		char szOkFileName[MAX_PATH]={0};
		char szCurFileName[MAX_PATH] = {0};
		sprintf(szCurFileName, "%s/%s", m_szCurrFileDir, m_szCurrFileName);
		sprintf(szOkFileName, "%s/%s_ok", m_szCurrFileDir, m_szCurrFileName);	
				
		char szNewData[32] = {0};
		sprintf(szNewData, "_%d_", m_nCurrDataCnt);
		
		std::string strOkFileName = szOkFileName;
		ReplaceStr(strOkFileName, "_0_", szNewData);

		rename(szCurFileName, strOkFileName.c_str());
	}

	return true;
}

void CFileBufferManage::AddToFilePathMap(const char *szFilePath)
{
	if (!szFilePath && strlen(szFilePath) > 0)
	{
		SafeLock saflock(&m_csFilePathMap);
		m_mFileName.insert(make_pair(szFilePath,0));	
		if(m_mFileName.size() >= MAPMAXFILE)	
		{
			m_mFileName.erase(m_mFileName.begin());//挤掉最开始那个
		}
	}
}

void CFileBufferManage::ClearFilePathMap()
{
	SafeLock saflock(&m_csFilePathMap);
	m_mFileName.clear();
}

bool CFileBufferManage::WriteData(const void *data, int nDataSize)
{
	bool bRet = false;
	if (!data)
	{
		return bRet;
	}
	
	//锁定当前文件
	SafeLock saflock(&m_csCurWriteFile);
	
	if (!m_fpCurrfile)
	{
		char sFileName[MAX_PATH]={0};
		SYSTEMTIME_EX t;
		CAdapter::GetLocalTime(&t);
		
		if (m_nIndexId>99999)
		{
			m_nIndexId = 0;
		}
		m_nIndexId++;

		//网关编号_条数_现有文件名_版本号。
		sprintf(m_szCurrFileName, "%d_%d_%02d%02d%02d%02d%02d%03d%05d_%s", m_nWGNO, 0, t.m_iMonth,t.m_iDay,t.m_iHour,t.m_iMinute,t.m_iSecond,t.m_iMilliseconds,m_nIndexId,m_szVersion);
        sprintf(sFileName, "%s/%s",m_szCurrFileDir, m_szCurrFileName);

		if((m_fpCurrfile = fopen(sFileName,"w+b"))==NULL)
		{
			bool bOtherRet = false;
			if (!CAdapter::PathFileExists(m_szCurrFileDir))
			{
				//文件夹被删了
				CAdapter::CreateDirectory(m_szCurrFileDir, 0);
				//再检测次
				if (CAdapter::PathFileExists(m_szCurrFileDir))
				{
					if((m_fpCurrfile = fopen(sFileName,"w+b")) != NULL)
					{
						bOtherRet = true;
					}
				}
			}

			if (!bOtherRet)
			{
				char szErrorLog[128] = {0};
				//sprintf(szErrorLog, "fopen error GetLastError() = %d", GetLastError());
				sprintf(szErrorLog, "fopen error GetLastError() = %d", errno);
				CLogMgr::GetInstance().WriteLog(szErrorLog, "WriteFileLog.txt");

				return bRet;
			}
		}
		//记录文件生成的时间
		time(&m_tLastSwitch);
		/*
		//文件版本V2
		if (0 == memcmp(VERSION_V2, m_szVersion, sizeof(VERSION_V2)-1))
		{
			//zhubo 2018-01-31  增加文件头
			size_t nwtSize = fwrite(header_v2, sizeof(char), sizeof(header_v2), m_fpCurrfile);
			if (nwtSize != sizeof(header_v2))
			{
				return errno;
			}
		}
		*/
	}
	
	size_t nWriteSize = fwrite(data, sizeof(char), nDataSize, m_fpCurrfile);
	if ((size_t)nDataSize == nWriteSize)
	{
		bRet = true;
		fflush(m_fpCurrfile);
		//达到最大值或超时未切换
		if (++m_nCurrDataCnt >= m_nMaxDataCnt || time(NULL)-m_tLastSwitch > m_nTimeOutSwitch)
		{
			//关闭临时文件，生OK文件
			CloseCurFile();
			//加入文件映射
			AddToFilePathMap(m_szCurrFileName);

			m_nCurrDataCnt = 0;
			memset(m_szCurrFileName, 0, sizeof(m_szCurrFileName));
			time(&m_tLastSwitch);
		}

		CAdapter::InterlockedExchangeAdd((long*)&m_nAllDataCnt, nDataSize/m_nDataTypeSize);
	}
	else
	{
		char szErrorLog[128] = {0};
		//sprintf(szErrorLog, "fwrite error nWriteSize=%d, GetLastError()=%d", nWriteSize, GetLastError());
		sprintf(szErrorLog, "fwrite error nWriteSize=%d, GetLastError()=%d", nWriteSize, errno);
		
		CLogMgr::GetInstance().WriteLog(szErrorLog, "WriteFileLog.txt");

		//写入失败的情况
		if (m_fpCurrfile)
		{
			//关闭临时文件，生OK文件
			CloseCurFile();
			//加入文件映射
			AddToFilePathMap(m_szCurrFileName);
			
			m_nCurrDataCnt = 0;
			memset(m_szCurrFileName, 0, sizeof(m_szCurrFileName));
			time(&m_tLastSwitch);

			//重新写下
			return RetryWriteData(data, nDataSize);
		}

		if (!CAdapter::PathFileExists(m_szCurrFileDir))
		{
			//文件夹被删了
			CAdapter::CreateDirectory(m_szCurrFileDir, 0);
		}
	}
	return bRet;
}

bool CFileBufferManage::RetryWriteData(const void *data, int nDataSize)
{
	bool bRet = false;
	if (!data)
	{
		return bRet;
	}
	
	if (!m_fpCurrfile)
	{
		char sFileName[MAX_PATH]={0};
		SYSTEMTIME_EX t;
		CAdapter::GetLocalTime(&t);
		
		if (m_nIndexId>99999)
		{
			m_nIndexId = 0;
		}
		m_nIndexId++;

		sprintf(m_szCurrFileName, "%d_%d_%02d%02d%02d%02d%02d%03d%05d_%s", m_nWGNO, 0, t.m_iMonth,t.m_iDay,t.m_iHour,t.m_iMinute,t.m_iSecond,t.m_iMilliseconds,m_nIndexId,m_szVersion);
        sprintf(sFileName, "%s/%s",m_szCurrFileDir, m_szCurrFileName);

		if((m_fpCurrfile = fopen(sFileName,"w+b"))==NULL)
		{
			bool bOtherRet = false;
			if (!CAdapter::PathFileExists(m_szCurrFileDir))
			{
				//文件夹被删了
				CAdapter::CreateDirectory(m_szCurrFileDir, 0);
				//再检测次
				if (CAdapter::PathFileExists(m_szCurrFileDir))
				{
					if((m_fpCurrfile = fopen(sFileName,"w+b")) != NULL)
					{
						bOtherRet = true;
					}
				}
			}

			if (!bOtherRet)
			{
				char szErrorLog[128] = {0};
				//sprintf(szErrorLog, "RetryWriteData fopen error GetLastError() = %d", GetLastError());
				sprintf(szErrorLog, "RetryWriteData fopen error GetLastError() = %d", errno);
				CLogMgr::GetInstance().WriteLog(szErrorLog, "WriteFileLog.txt");

				return bRet;
			}
		}
		//记录文件生成的时间
		time(&m_tLastSwitch);
	}
	
	size_t nWriteSize = fwrite(data, sizeof(char), nDataSize, m_fpCurrfile);
	if ((size_t)nDataSize == nWriteSize)
	{
		bRet = true;
		fflush(m_fpCurrfile);
		//达到最大值或超时未切换
		if (++m_nCurrDataCnt >= m_nMaxDataCnt || time(NULL)-m_tLastSwitch > m_nTimeOutSwitch)
		{
			//关闭临时文件，生OK文件
			CloseCurFile();
			//加入文件映射
			AddToFilePathMap(m_szCurrFileName);

			m_nCurrDataCnt = 0;
			memset(m_szCurrFileName, 0, sizeof(m_szCurrFileName));
			time(&m_tLastSwitch);
		}

		CAdapter::InterlockedExchangeAdd((long*)&m_nAllDataCnt, nDataSize/m_nDataTypeSize);
	}
	else
	{
		char szErrorLog[128] = {0};
		//sprintf(szErrorLog, "RetryWriteData fwrite error nWriteSize=%d, GetLastError()=%d", nWriteSize, GetLastError());
		sprintf(szErrorLog, "RetryWriteData fwrite error nWriteSize=%d, GetLastError()=%d", nWriteSize, errno);
		CLogMgr::GetInstance().WriteLog(szErrorLog, "WriteFileLog.txt");

		//写入失败的情况
		if (m_fpCurrfile)
		{
			fclose(m_fpCurrfile);
			m_fpCurrfile = NULL;
			
			//重命名为err
			char szErrFileName[MAX_PATH]={0};
 			char szCurFileName[MAX_PATH] = {0};
 			sprintf(szCurFileName, "%s/%s", m_szCurrFileDir, m_szCurrFileName);
			sprintf(szErrFileName, "%s/%s_err", m_szCurrFileDir, m_szCurrFileName);				
			rename(m_szCurrFileName, szErrFileName);
			
			m_nCurrDataCnt = 0;
			memset(m_szCurrFileName, 0, sizeof(m_szCurrFileName));
			time(&m_tLastSwitch);
		}
	}

	return bRet;
}

void CFileBufferManage::ReplaceStr(std::string &strData, const char *szOldData, const char *szNewData)
{
	size_t found = strData.rfind(szOldData);
	if (found != std::string::npos)
	{
		strData.replace(found, strlen(szOldData), szNewData);
	}
}

std::string CFileBufferManage::GetFirstOkFilePath()
{
	std::string strPath;
	SafeLock saflock(&m_csFilePathMap);
	if (!m_mFileName.empty())
	{
		strPath = m_mFileName.begin()->first;
		m_mFileName.erase(m_mFileName.begin());
	}
	else
	{
		LoadFilePathToMap();
		if (!m_mFileName.empty())
		{
			strPath = m_mFileName.begin()->first;
			m_mFileName.erase(m_mFileName.begin());
		}
	}

	return strPath;
}

int CFileBufferManage::ReadData(void *readData, int nToBeRead)
{
	if (!readData)
	{
		return 0;
	}
	
	//锁定文件
	SafeLock saflock(&m_csCurReadFile);

	int nTrueRead = 0;
	INT64 nFileId =0;
	char szFilePath[MAX_PATH]={0};
	
	std::string strPath = GetFirstOkFilePath();
	if (strPath.size() > 0)
	{
		sprintf(szFilePath, "%s/%s", m_szCurrFileDir, strPath.c_str());	
		FILE *fp = fopen(szFilePath,"rb");
		if(fp)
		{
			/*
			//文件版本V2
			if (0 == memcmp(VERSION_V2, m_szVersion, sizeof(VERSION_V2)-1))
			{
				if (fseek(fp, 0, sizeof(header_v2)) < 0)
				{
					return 0;
				}
			}
			*/

			//RISK_1
			nTrueRead = (int)(fread(readData, sizeof(char), nToBeRead, fp));
			fclose(fp);
			fp = NULL;
	
			//if (CAdapter::ShellDeleteFiles(szFilePath))
			if (0 == remove(szFilePath))
			{
				CAdapter::InterlockedExchangeAdd((long*)&m_nAllDataCnt, (-1)*(nTrueRead/m_nDataTypeSize));
			}
			else
			{
				nTrueRead = 0;
			}
		}
	}
	return nTrueRead;
}

bool CFileBufferManage::SwitchFile()
{
	//若超时则切换
	if (time(NULL) - m_tLastSwitch > m_nTimeOutSwitch)
	{
		SafeLock saflock(&m_csCurWriteFile);
		CloseCurFile();
		m_nCurrDataCnt = 0;
		
		AddToFilePathMap(m_szCurrFileName);
		memset(m_szCurrFileName, 0, sizeof(m_szCurrFileName));

		time(&m_tLastSwitch);
	}
	return true;
}

int CFileBufferManage::GetDataRecordCount()
{
	return m_nAllDataCnt;
}

int CFileBufferManage::GetMaxDataCount()
{
	return m_nMaxDataCnt; 
}

void CFileBufferManage::SwitchAllFile()
{

	DIR						*dir;
	struct dirent			*ptr;
	size_t					len;
	char					*name;
	struct stat				info;

	char szFilePath[MAX_PATH] = { 0 };
	sprintf(szFilePath, m_szCurrFileDir);

	dir = opendir(szFilePath);
	if (NULL == dir)
	{
		return;
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		name = ptr->d_name;

		/*
		// DT_REG = 8,
		if (ptr->d_type != DT_REG)
			continue;
		*/
		string strTmp(szFilePath);
		strTmp += "/";
		strTmp += ptr->d_name;
		if (!CAdapter::IsRegularFile(strTmp))
		{
			continue;
		}
		
		if (strstr(ptr->d_name, "_ok") || strstr(ptr->d_name, "_err"))
			continue;

		//重命名
		char szOkFileName[MAX_PATH] = { 0 };
		char szCurFileName[MAX_PATH] = { 0 };

		sprintf(szCurFileName, "%s/%s", m_szCurrFileDir, name);
		sprintf(szOkFileName, "%s/%s_ok", m_szCurrFileDir, name);

		if (stat((const char *)szCurFileName, &info) != FILE_ERROR)
		{
			char szNewData[32] = { 0 };
			sprintf(szNewData, "_%d_", info.st_mode / m_nDataTypeSize);

			std::string strOkFileName = szOkFileName;
			ReplaceStr(strOkFileName, "_0_", szNewData);
			rename(szCurFileName, strOkFileName.c_str());
		}
	}
	closedir(dir);


}

void CFileBufferManage::LoadFilePathToMap()
{


	DIR						*dir;
	struct dirent			*ptr;
	size_t					len;
	char					*name;
	struct stat				info;

	char szFilePath[MAX_PATH] = { 0 };
	sprintf(szFilePath, m_szCurrFileDir);

	dir = opendir(szFilePath);
	if (NULL == dir)
	{
		return;
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		// DT_REG = 8,
		/*
		if (ptr->d_type != DT_REG)
			continue;
		*/
		string strTmp(szFilePath);
		strTmp += "/";
		strTmp += ptr->d_name;
		if (!CAdapter::IsRegularFile(strTmp))
		{
			continue;
		}
		
		if (strstr(ptr->d_name, "_ok"))
		{
			m_mFileName.insert(make_pair(ptr->d_name, 0));
			if (m_mFileName.size() >= MAPMAXFILE)
			{
				m_mFileName.erase(m_mFileName.begin());//挤掉最开始那个
			}
		}
	}

	closedir(dir);
}

int CFileBufferManage::GetAllDataSize()
{
	//zhubo 2017-05-26

	//zhubo 2017-05-26
	int nDataCount = 0;

	DIR						*dir;
	struct dirent			*ptr;
	size_t					len;
	struct stat				info;


	char szFilePath[MAX_PATH] = { 0 };
	sprintf(szFilePath, m_szCurrFileDir);

	dir = opendir(szFilePath);
	if (NULL == dir)
	{
		return nDataCount;
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		// DT_REG = 8,
		/*
		if (ptr->d_type != DT_REG)
			continue;
			*/
		string strTmp(szFilePath);
		strTmp += "/";
		strTmp += ptr->d_name;
		if (!CAdapter::IsRegularFile(strTmp))
		{
			continue;
		}

		if (strstr((char*)ptr->d_name, "_ok"))
		{
			string strTmp(szFilePath);
			strTmp += "/";
			strTmp += ptr->d_name;
			if (stat((const char *)strTmp.c_str(), &info) != FILE_ERROR 
				&& info.st_size > 0)
			{
				nDataCount += info.st_size / m_nDataTypeSize;
			}
		}
	}

	closedir(dir);

	return nDataCount;


}
