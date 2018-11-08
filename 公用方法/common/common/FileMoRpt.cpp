// FileMoRpt.cpp: implementation of the CFileMoRpt class.
//
//////////////////////////////////////////////////////////////////////


#include "FileMoRpt.h"
#include "StatsMgr.h"
#include "LogMgr.h"
#include "PtMonitor.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//extern const char *g_szRptContent;
const int nMaxFilePathLen = 512;

using std::string;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileMoRpt::CFileMoRpt()
{
	memset(m_szRptfilePathShare, 0, sizeof(m_szRptfilePathShare));
	memset(m_szMofilePathShare, 0, sizeof(m_szMofilePathShare));
	memset(m_szRptfilePathLocal, 0, sizeof(m_szRptfilePathLocal));
	memset(m_szMofilePathLocal, 0, sizeof(m_szMofilePathLocal));
    m_iGateNO = 0;
}

CFileMoRpt::~CFileMoRpt()
{

}

void CFileMoRpt::InitFilePath(char* szWbsLocalPath, char* szWbsSharePath, int nGateNO)
{
	//网关编号(目前只支持单进程)
	m_iGateNO = nGateNO;
	int nLen = 0;

	//MO本地
	nLen = min(strlen(szWbsLocalPath), MAX_PATH);
	memcpy(m_szMofilePathLocal, szWbsLocalPath, nLen);
	m_szMofilePathLocal[nLen] = '\0';

	//RPT本地
	nLen = min(strlen(szWbsLocalPath), MAX_PATH);
	memcpy(m_szRptfilePathLocal, szWbsLocalPath, nLen);
	m_szRptfilePathLocal[nLen] = '\0';

	//MO共享
	nLen = min(strlen(szWbsSharePath), MAX_PATH);
	memcpy(m_szMofilePathShare, szWbsSharePath, nLen);
	m_szMofilePathShare[nLen] = '\0';
		
	//RPT
	nLen = min(strlen(szWbsSharePath), MAX_PATH);
	memcpy(m_szRptfilePathShare, szWbsSharePath, nLen);
	m_szRptfilePathShare[nLen] = '\0';

	if (strlen(szWbsLocalPath) <= 0)
	{
		LOG_INFO("CFileMoRpt::Init: waring LocalPath is NULL");
	}
	if (strlen(szWbsSharePath) <= 0)
	{
		LOG_INFO("CFileMoRpt::Init: waring SharePath is NULL");

		//如果共享路径未配置 ，则共享路径取本地路径
		nLen = min(strlen(m_szMofilePathLocal), MAX_PATH);
		memcpy(m_szMofilePathShare, m_szMofilePathLocal, nLen);
		m_szMofilePathShare[nLen] = '\0';

		nLen = min(strlen(m_szMofilePathLocal), MAX_PATH);
		memcpy(m_szRptfilePathShare, m_szRptfilePathLocal, nLen);
		m_szRptfilePathShare[nLen] = '\0';
	}

	CreateFolder((char*)"FILEMO", m_szMofilePathShare, m_szMofilePathLocal);
	CreateFolder((char*)"FILERPT", m_szRptfilePathShare, m_szRptfilePathLocal);
	CLOG_INFO("CFileMoRpt::Init success!\r\nLocalPath=%s\r\nSharePath=%s ", m_szMofilePathLocal, m_szMofilePathShare);
}

int CFileMoRpt::WriteRpttoFile(std::vector<PACK_DELIVER> &RptQue)
{
	int nQueSize = RptQue.size();        //队列中的RPT条数
	if (nQueSize <= 0)
	{
		return 0;
	}
	try
	{
		if (!CAdapter::PathFileExists(m_szRptfilePathShare))
		{
			//正确的共享路径断掉后报警
			if ( 0 != strlen(m_szRptfilePathShare))
			{
				//告警
				//加入日志监控队列
				string s = "Rpt文件共享路径不存在,请检查网络是否连接!";
				char szValue[25] = {0};
				sprintf(szValue, "%d", 1);
                
				CPtMonitor::GetInstance().AddLogToMonQue((int)LOG_SYS_ERROR, s.c_str(), 10015, "CFileMoRpt::WriteRpttoFile", szValue, true);
			}

			//创建本地路径
			if (0 == strlen(m_szRptfilePathLocal))
			{
				char szPathBuf[nMaxFilePathLen] = {0};
				CAdapter::GetCurrentPath(szPathBuf, 512);
                strcat(szPathBuf, "FILERPT");
			}

			if (!CAdapter::PathFileExists(m_szRptfilePathLocal))
			{
				CAdapter::CreateDirectory(m_szRptfilePathLocal, NULL);
			}
		}
		else
		{
			if (!CAdapter::PathFileExists(m_szRptfilePathShare))
			{
				CAdapter::CreateDirectory(m_szRptfilePathShare, NULL);
			}

            //zhubo 2017-4-8
			if (CPtMonitor::GetInstance().bLastCommonIsError(10015))
			{
				//告警
				char szValue[25] = {0};
				sprintf(szValue, "%d", 0);
				
				CPtMonitor::GetInstance().CommonMonEvent((int)LOG_SYS_ERROR, "共享RPT目录恢复正常", 10015, "CFileMoRpt::WriteRpttoFile", szValue, false);
			}
		}
		
		PACK_DELIVER rptPack = RptQue[0]; 
		FILEINFO rptInfo;                               //文件信息
		char szFileName[nMaxFilePathLen]  = {0};        //文件整体路径
		char szUsrFolder[nMaxFilePathLen] = {0};        //文件夹名
		INT64 nlFileName                  = 0;          //大整型文件名
		int nFileCount                    = 0;          //现有文件中的RPT条数
		
		if (!CAdapter::PathFileExists(m_szRptfilePathShare))
		{
			//写本地
			if (strlen((char *)rptPack.szUsrId)>2)
			{
				sprintf(szUsrFolder, "%s/%s", m_szRptfilePathLocal, rptPack.szUsrId);    //用户目录
			}
			else
			{
				sprintf(szUsrFolder, "%s/%d", m_szRptfilePathLocal, rptPack.nUID);    //用户目录	
			}
			if (!CAdapter::PathFileExists(szUsrFolder))
			{
				CAdapter::CreateDirectory(szUsrFolder, NULL);
			}
		}
		else
		{
			//写共享
 			if (strlen((char *)rptPack.szUsrId)>2)
 			{
 				sprintf(szUsrFolder, "%s/%s", m_szRptfilePathShare, rptPack.szUsrId);    //用户目录
 			}
 			else
 			{
				sprintf(szUsrFolder, "%s/%d", m_szRptfilePathShare, rptPack.nUID);    //用户目录	
			}
			if (!CAdapter::PathFileExists(szUsrFolder))
			{
				CAdapter::CreateDirectory(szUsrFolder, NULL);
			}
		}

		//小于80条的查找文件信息队列，追加到现有的文件中
		if (nQueSize < RPT_FILE_NUM)
		{
			//查map取出文件信息，如果没有新加
			GetPathFromMap(rptPack.nUID, nFileCount, szUsrFolder, szFileName, nlFileName);		
			
			//取文件名失败
			if (strlen(szFileName) < 1)
			{
				nFileCount = 0;
				MakeFileName(szUsrFolder, szFileName, nlFileName);
			}

			//文件条数+队列条数<80条，刚生成一个新的记录，存入map中
			if ((nFileCount+nQueSize) < RPT_FILE_NUM)
			{
				rptInfo.nlFilePath  = nlFileName;
				rptInfo.nFileCount  = nFileCount+nQueSize;
				rptInfo.nUid        = rptPack.nUID;
				rptInfo.timeFile    = time(NULL);
				AddToMap(rptPack.nUID, rptInfo);	
			}
		}
		//刚好80条直接写到一个文件中
		else     
		{
			nFileCount = 0;                           //新文件，条数为0
			MakeFileName(szUsrFolder, szFileName, nlFileName);
		}

		//写一批入件
		if (-1 == WriteRpt(RptQue, nFileCount, szFileName))
		{
			return -1;
		}

		//队列中剩下的再写一新的文件
		nQueSize = RptQue.size();
		if (nQueSize > 0)
		{
			MakeFileName(szUsrFolder, szFileName, nlFileName);
			rptInfo.nlFilePath  = nlFileName;
			rptInfo.nFileCount  = nQueSize;
			rptInfo.nUid        = rptPack.nUID;
			rptInfo.timeFile    = time(NULL);
			AddToMap(rptPack.nUID, rptInfo);
			
			if (-1 == WriteRpt(RptQue, 0, szFileName))
			{
				return -1;
			}
		}		
	}
	catch (...)
	{
		LOGF_ERROR("WriteRpttoFile 异常!", "FileRpt.txt");
		return -1;
	}
	return 0;
}

int  CFileMoRpt::WriteMotoFile(std::vector<PACK_DELIVER> &MoQue)        //将Mo写入文件
{
	int nQueSize = MoQue.size();        //队列中的RPT条数
	if (nQueSize <= 0)
	{
		return 0;
	}
	try
	{
		if (!CAdapter::PathFileExists(m_szMofilePathShare))
		{
			if ( 0 != strlen(m_szMofilePathShare))
			{
				//告警
				//加入日志监控队列
				string s = "Mo文件共享路径不存在,请检查网络是否连接!";
				char szValue[25] = {0};
				sprintf(szValue, "%d", 1);

				CPtMonitor::GetInstance().AddLogToMonQue((int)LOG_SYS_ERROR, s.c_str(), 10014, "CFileMoRpt::WriteMotoFile", szValue, true);
			}

			//创建本地路径
			if ( 0 == strlen(m_szMofilePathLocal))
			{
				char szPathBuf[nMaxFilePathLen] = {0};
				CAdapter::GetCurrentPath(szPathBuf, 512);
                strcat(m_szMofilePathLocal, "FILEMO");
			}
			
			if (!CAdapter::PathFileExists(m_szMofilePathLocal))
			{
				CAdapter::CreateDirectory(m_szMofilePathLocal, NULL);
			}
		}
		else
		{
			if (!CAdapter::PathFileExists(m_szMofilePathShare))
			{
				CAdapter::CreateDirectory(m_szMofilePathShare, NULL);
			}

            //zhubo 2017-4-8
			if (CPtMonitor::GetInstance().bLastCommonIsError(10014))
			{
				//告警
				char szValue[25] = {0};
				sprintf(szValue, "%d", 0);
				
				CPtMonitor::GetInstance().CommonMonEvent((int)LOG_SYS_ERROR, "共享Mo目录恢复正常", 10014, "CFileMoRpt::WriteMotoFile", szValue, false);
			}
		}
		
		PACK_DELIVER MoPack = MoQue[0]; 
		FILEINFO MoInfo;                               //文件信息
		char szFileName[nMaxFilePathLen]  = {0};        //文件整体路径
		char szUsrFolder[nMaxFilePathLen] = {0};        //文件夹名
		INT64 nlFileName                  = 0;          //大整型文件名
		int nFileCount                    = 0;          //现有文件中的RPT条数
		if (!CAdapter::PathFileExists(m_szMofilePathShare))
		{
			//写本地
			if (strlen((char *)MoPack.szUsrId)>2)
			{
				sprintf(szUsrFolder, "%s/%s", m_szMofilePathLocal, MoPack.szUsrId);    //用户目录
			}
			else
			{
				sprintf(szUsrFolder, "%s/%d", m_szMofilePathLocal, MoPack.nUID);    //用户目录	
			}
			if (!CAdapter::PathFileExists(szUsrFolder))
			{
				CAdapter::CreateDirectory(szUsrFolder, NULL);
			}
		}
		else
		{
			//写共享
			if (strlen((char *)MoPack.szUsrId)>2)
			{
				sprintf(szUsrFolder, "%s/%s", m_szMofilePathShare, MoPack.szUsrId);    //用户目录
			}
			else
			{
				sprintf(szUsrFolder, "%s/%d", m_szMofilePathShare, MoPack.nUID);    //用户目录	
			}
			if (!CAdapter::PathFileExists(szUsrFolder))
			{
				CAdapter::CreateDirectory(szUsrFolder, NULL);
			}
		}
		
		//小于80条的查找文件信息队列，追加到现有的文件中
		if (nQueSize < MO_FILE_NUM)
		{
			//查map取出文件信息，如果没有新加
			GetPathFromMoMap(MoPack.nUID, nFileCount, szUsrFolder, szFileName, nlFileName);		
			
			//取文件名失败
			if (strlen(szFileName) < 1)
			{
				nFileCount = 0;
				MakeFileName(szUsrFolder, szFileName, nlFileName);
			}
			
			//文件条数+队列条数<25条，刚生成一个新的记录，存入map中
			if ((nFileCount + nQueSize) < MO_FILE_NUM)
			{
				MoInfo.nlFilePath  = nlFileName;
				MoInfo.nFileCount  = nFileCount + nQueSize;
				MoInfo.nUid        = MoPack.nUID;
				MoInfo.timeFile    = time(NULL);
				AddToMoMap(MoPack.nUID, MoInfo);	
			}
		}
		//刚好80条直接写到一个文件中
		else     
		{
			nFileCount = 0;                     //新文件，条数为0
			MakeFileName(szUsrFolder, szFileName, nlFileName);
		}
		
		//写一批入件
		if (-1 == WriteMo(MoQue, nFileCount, szFileName))
		{
			return -1;
		}
		
		//队列中剩下的再写一新的文件
		nQueSize = MoQue.size();
		if (nQueSize > 0)
		{
			MakeFileName(szUsrFolder, szFileName, nlFileName);
			MoInfo.nlFilePath  = nlFileName;
			MoInfo.nFileCount  = nQueSize;
			MoInfo.nUid        = MoPack.nUID;
			MoInfo.timeFile    = time(NULL);
			AddToMoMap(MoPack.nUID, MoInfo);
			
			if (-1 == WriteMo(MoQue, 0, szFileName))
			{
				return -1;
			}
		}		
	}
	catch (...)
	{
		LOGF_ERROR("WriteMotoFile 异常!", "FileMo.txt");
		return -1;
	}
	return 0;
}

int CFileMoRpt::GetWbsValidFile(const int nUid, const char* szpUsrName, char* szValidFile, char* szValidPath)
{
	char szUsrFolder[nMaxFilePathLen]   = {0};
	char szUsrFolder2[nMaxFilePathLen]  = {0};
	char szName[MAX_SPID_LEN+1]         = {0}; 
	
	sprintf(szUsrFolder, "%s/%d", szValidPath, nUid);
	if (NULL != szpUsrName)
	{
		//将账号转为大写
		string strUserName = cstr::to_upper_s(szpUsrName);
		memcpy(szName, strUserName.c_str(), MAX_SPID_LEN);
		sprintf(szUsrFolder2, "%s/%s", szValidPath, szName);
	}
	
	if (!CAdapter::PathFileExists(szUsrFolder) && !CAdapter::PathFileExists(szUsrFolder2))
	{
        CLOG_DEBUG("quit 1 . nUid=%d szpUsrName=%s", nUid, szpUsrName);
		return -1;
	}


    if (GetValidFile(szUsrFolder2, szValidFile))
    {
        //CLOG_DEBUG("quit succeed userid . nUid=%d szpUsrName=%s", nUid, szpUsrName);
        return 0;
    }

    if (GetValidFile(szUsrFolder, szValidFile))
    {
        //CLOG_DEBUG("quit succeed . nUid=%d szpUsrName=%s", nUid, szpUsrName);
        return 0;
    }

    return -1;
}

int  CFileMoRpt::ReadMoFile(const int nUid, const char* szpUsrName, std::vector<PACK_DELIVER> &MoPackArr)
{
    char szUsrFolderTmp[nMaxFilePathLen] = { 0 };
    char szUsrFolderTmp2[nMaxFilePathLen]  = {0};
    char szNameTmp[MAX_SPID_LEN+1]         = {0}; 
    char szFilePath[nMaxFilePathLen]    = {0};         
    PACK_DELIVER MoPack;

	try
	{
		if (!CAdapter::PathFileExists(m_szMofilePathShare))
		{
			//共享路径不存在，判断本地路径
			if (!CAdapter::PathFileExists(m_szMofilePathLocal))
			{
				return -1;
			}
			else
			{
				//读本地
				if (0 != GetWbsValidFile(nUid, szpUsrName, szFilePath, m_szMofilePathLocal))
				{
                                 //CLOG_DEBUG("quit fail GetWbsValidFile nUid=%d szpUsrName=%s ", nUid, szpUsrName);
					return -1;
				}
			}
		}
		else
		{
			//共享路径存在
			if (0 != GetWbsValidFile(nUid, szpUsrName, szFilePath, m_szMofilePathShare))
			{
				//本地是否存在
				if (!CAdapter::PathFileExists(m_szMofilePathLocal))
				{
		             return -1;
				}
				else
				{
					//读本地
					if (0 != GetWbsValidFile(nUid, szpUsrName, szFilePath, m_szMofilePathLocal))
					{
						return -1;
					}
				}
			}
		}

		//重命名
		char szFilePathNew[256] = {0};
		char szFm[256] = {0};
		strncpy(szFm, szFilePath, strlen(szFilePath)-4);
		sprintf(szFilePathNew, "%s_bak_%d.tmp", szFm, m_iGateNO);
		int ret = rename(szFilePath, szFilePathNew);
		if (ret == 0)
		{
			ifstream fin(szFilePathNew);
			if (fin.is_open())
			{
				char szData[MAX_MO_BUFLEN + 1] = {0};
				int nLine = 0;

				while (fin.getline(szData, MAX_MO_BUFLEN + 1))
				{
					szData[MAX_MO_BUFLEN] = 0;
					memset(&MoPack, 0, sizeof(MoPack));
					if (ResolveMoString(szData, nUid, MoPack))
					{
						MoPack.nReportFlag = FLAG_MO;
						memcpy(MoPack.szUsrId, szpUsrName, min(strlen(szpUsrName), MAX_SPID_LEN));
						MoPackArr.push_back(MoPack);
						nLine++;
					}
					memset(szData, 0, sizeof(szData));
				}

				fin.close();
				if (0 != remove(szFilePathNew))
				{
					int errCode = errno;
					char s[128] = { 0 };
					snprintf(s,sizeof(s),"remove file error! fileName=%s,errno=%d \r\n", szFilePathNew, errCode);
					LOGF_ERROR(s, "FileRpt.txt");
                                    CLOG_DEBUG("quit remove fail .nUid=%d szpUsrName=%s szFilePathNew=%s", nUid, szpUsrName, szFilePathNew);
					return -1;
				}
				return nLine;
			}
			else
			{
				//重命名回去
				fin.close();
				int ret = rename(szFilePathNew, szFilePath);
			}
		}
	}
	catch (...)
	{
	    char s[128] = { 0 };
		snprintf(s,sizeof(s),"ReadMoFile 异常! nUid=%d \r\n", nUid);
		LOGF_ERROR(s, "FileRpt.txt");

		return -1;
	}

	return -1;
}

int  CFileMoRpt::MakeFileName(const char *szUsrFolder, char *szFileName, INT64 &nlFileName)
{
	try
	{
		unsigned char szMsgId[MAX_MSGID_LEN+1] = {0};   
		m_global.MakeMsgId(szMsgId, 1, m_iGateNO);
		nlFileName = m_global.TranMsgIdCharToI64(szMsgId);

		// 改成临时的文件,写完后再改成txt文件
		sprintf(szFileName, "%s/%lld_n0.tmp", szUsrFolder, nlFileName);	
	}
	catch (...)
	{
		string s = "MakeFileName 异常!";
		LOGF_ERROR(s.c_str(), "FileRpt.txt");
		return -1;
	}
	
	return 0;
}

int  CFileMoRpt::WriteRpt(std::vector<PACK_DELIVER> &RptQue, int nFileCount,const char *szFileName)
{
	int nQueSize = RptQue.size();
	if (nQueSize <= 0 || NULL == szFileName)
	{
		return -1;
	}
	
	ofstream fout(szFileName, ios::out | ios::app);
	try
	{
		int nCount = 0;
		std::vector<PACK_DELIVER> ::iterator itQue;
		itQue = RptQue.begin();
		if (fout.is_open())
		{
			char szbuffer[MAX_RPT_BUFLEN+1] = {0};
			while (nFileCount < RPT_FILE_NUM && itQue != RptQue.end())
			{
				try
				{	
					++nFileCount;
					++nCount;
					memset(szbuffer, 0, sizeof(szbuffer));
					
					makeRptString(*itQue, szbuffer);
					++itQue;
					szbuffer[MAX_RPT_BUFLEN] = 0;
					 
					if (strlen(szbuffer) > 1 && strlen(szbuffer) < MAX_RPT_BUFLEN)
					{
						// 将字符串写入到文件
						fout.write(szbuffer, strlen(szbuffer));
					}
				}
				catch(...)
				{
					char s[128] = { 0 };	
					snprintf(s,sizeof(s),"WriteRpt 异常! 文件路径:%s", szFileName);
					LOGF_ERROR(s, "FileRpt.txt");
				}
			}
			
			fout.close();
			
			if (itQue != RptQue.begin())
			{
				RptQue.erase(RptQue.begin(), RptQue.begin() + nCount);
			}

			//将该文件重命名为带条数的文件名
			const char *p1 =strstr(szFileName, "_n");
			char szFilePathNew[256] = { 0 };
			char szFm[256] = { 0 };
			if (p1 != NULL)
			{
				//更新
				strncpy(szFm, szFileName, strlen(szFileName) - strlen(p1));
			}
			else
			{
				strncpy(szFm, szFileName, strlen(szFileName) - 4);
			}
			sprintf(szFilePathNew, "%s_n%d.txt", szFm, nFileCount);
			int ret = rename(szFileName, szFilePathNew);   //失败暂时不处理
		}
		else
		{
			//如果打不开并且为新文件
			if(strstr(szFileName,".tmp") != NULL)
			{
				remove(szFileName);
			}
		}
	}
	catch (...)
	{
		fout.close();
		
		//删掉这批写入异常的数据文件
		if(strstr(szFileName,".tmp") != NULL)
		{
			remove(szFileName);
		}
		
		char s[128] = { 0 };	
		snprintf(s,sizeof(s),"WriteRpt 异常! 文件路径:%s", szFileName);
		LOGF_ERROR(s, "FileRpt.txt");
		return -1;	
	}

	return 0;
}

int  CFileMoRpt::WriteMo(std::vector<PACK_DELIVER> &MoQue, int nFileCount,const char *szFileName)
{
	int nQueSize = MoQue.size();
	if (nQueSize <= 0 || NULL == szFileName)
	{
		return -1;
	}
	
	ofstream fout(szFileName, ios::out | ios::app);
	try
	{
		int nCount = 0;
		std::vector<PACK_DELIVER> ::iterator itMo;
		itMo = MoQue.begin();
		
		if (fout.is_open())
		{
			char szbuffer[MAX_MO_BUFLEN + 1] = {0};
			while (nFileCount < MO_FILE_NUM && itMo != MoQue.end())
			{
				try
				{
					++nFileCount;
					++nCount;
					memset(szbuffer, 0, sizeof(szbuffer));
				
					makeMoString(*itMo, szbuffer);
					szbuffer[MAX_MO_BUFLEN] = 0;
					++itMo;
					
					if (strlen(szbuffer) > 1 && strlen(szbuffer) < MAX_MO_BUFLEN)
					{
						// 将字符串写入到文件
						fout.write(szbuffer, strlen(szbuffer));
					}
				}
				catch(...)
				{
					char s[128] = { 0 };	
					snprintf(s,sizeof(s),"WriteMo 异常! 文件路径:%s", szFileName);
					LOGF_ERROR(s, "FileMo.txt");
				}
			}
			
			fout.close();
			
			if (itMo != MoQue.begin())
			{
				MoQue.erase(MoQue.begin(), MoQue.begin() + nCount);
			}

			//将该文件重命名为带条数的文件名
			const char *p1 = strstr(szFileName, "_n");
			char szFilePathNew[256] = { 0 };
			char szFm[256] = { 0 };
			
			if (p1 != NULL)
			{
				//更新
				strncpy(szFm, szFileName, strlen(szFileName) - strlen(p1));
			}
			else
			{
				strncpy(szFm, szFileName, strlen(szFileName) - 4);
			}
			
			sprintf(szFilePathNew, "%s_n%d.txt", szFm, nFileCount);
			int ret = rename(szFileName, szFilePathNew);	//失败暂时不处理
		}
		else
		{
			//如果打不开并且为新文件
			if(strstr(szFileName,".tmp") !=NULL)
			{
				remove(szFileName);
			}
		}
	}
	catch (...)
	{
		fout.close();
		
		//删掉这批写入异常的数据文件
		if(strstr(szFileName,".tmp") != NULL)
		{
			remove(szFileName);
		}
		
		char s[128] = { 0 };
		snprintf(s,sizeof(s),"WriteMo 异常! filename=%s", szFileName);
		LOGF_ERROR(s, "FileMo.txt");
		return -1;	
	}
	
	return 0;
}	

void CFileMoRpt::AddToMap(int key, const FILEINFO &rptInfo)
{
	try
	{
		SafeLock slock(&m_lockRptQue);    //加锁
		if (m_mapRpt.size() < g_nMaxSize)
		{
			m_mapRpt.insert(std::make_pair(key, rptInfo));
		}
		else
		{
			//清队列里面时间过长的数据
			try
			{
				FILEINFO fInfo;
				const int CLEANTIME = 43200;    //清理12小时前的信息 12*60*60
				std::multimap<int, FILEINFO> ::iterator itRpt;

				itRpt = m_mapRpt.begin();
				while (itRpt != m_mapRpt.end())
				{
					fInfo = (FILEINFO)itRpt->second;
					time_t tnow = time(NULL);
					
					if ((tnow-fInfo.timeFile) > CLEANTIME)
					{
						m_mapRpt.erase(itRpt++);
					}
					else
					{
						++itRpt;
					}
				}
			}
			catch (...)
			{
				LOGF_ERROR("清理队列时异常! \r\n", "FileRpt.txt");
			}		
		}
	}
	catch (...)
	{
		char s[128] = { 0 };	
		snprintf(s,sizeof(s),"AddToMap 加入m_mapRpt时异常! m_mapRpt.size()=%d;\r\n", m_mapRpt.size());
		LOGF_ERROR(s, "FileRpt.txt");
	}
}

void CFileMoRpt::AddToMoMap(int key, const FILEINFO &MoInfo)
{
	try
	{
		SafeLock slock(&m_lockMoQue);    //加锁
		if (m_mapMo.size() < g_nMaxSize)
		{
			m_mapMo.insert(std::make_pair(key, MoInfo));
		}
		else
		{
			//清队列里面时间过长的数据
			try
			{
				FILEINFO fInfo;
				const int CLEANTIME = 43200;    //清理12小时前的信息 12*60*60
				std::multimap<int, FILEINFO> ::iterator itMo;
				itMo = m_mapMo.begin();
				while (itMo != m_mapMo.end())
				{
					fInfo = (FILEINFO)itMo->second;
					time_t tnow = time(NULL);
					
					if ((tnow-fInfo.timeFile) > CLEANTIME)
					{
						m_mapMo.erase(itMo++);
					}
					else
					{
						++itMo;
					}
				}
			}
			catch (...)
			{
				LOGF_ERROR("清理Mo队列时异常! \r\n", "FileRpt.txt");
			}		
		}
	}
	catch (...)
	{
		char s[128] = { 0 };
		snprintf(s,sizeof(s),"AddToMoMap 加入m_mapMo时异常! m_mapMo.size()=%d;\r\n", m_mapMo.size());
		LOGF_ERROR(s, "FileRpt.txt");
	}
}

void CFileMoRpt::makeRptString(const PACK_DELIVER &RptPack, char *szbuffer)
{
	char szTime[MAX_TIME_LEN+1]        = {0};
	UCHAR szMsgId[MAX_MSGID_LEN+1]     = {0};
	char szDestId[MAX_DESTID_LEN+1]    = {0};
	char szPhone[MAX_DESTTMID30_LEN+1] = {0};
	char szState[MAX_ERSTAT_LEN+1]     = {0};

	try
	{
		if (NULL == szbuffer)
		{
			return;
		}

		memcpy(szTime, RptPack.szTime, MAX_TIME_LEN);
		memcpy(szMsgId, RptPack.msgcontent.szMsgId, MAX_MSGID_LEN);
		memcpy(szDestId, RptPack.szDestId, MAX_DESTID_LEN);
		memcpy(szPhone, RptPack.msgcontent.szDestTerminalId, MAX_DESTTMID30_LEN);
		memcpy(szState, RptPack.msgcontent.szState, MAX_ERSTAT_LEN);

		//对wbs5.0中的custid和exdata进行base64编码(可能会含有逗号)
		char szCustId[200] = { 0 };
		char szExData[200] = { 0 };
		m_global.Base64Encode((LPBYTE)RptPack.szCustId, strlen(RptPack.szCustId), (LPBYTE)szCustId);
		m_global.Base64Encode((LPBYTE)RptPack.szExData, strlen(RptPack.szExData), (LPBYTE)szExData);

		//格式为：时间,流水号，通道号，手机号，用户自编流水号，状态值,
		//wbs5.0用户自编流水号,当前条数，总条数，扩展子号，下行时间，返回时间，发送状态，扩展数据
		sprintf(szbuffer, "%s,%lld,%s,%s,%lld,%s,%s,%d,%d,%s,%s,%s,%d,%s\r\n", szTime, 
			m_global.TranMsgIdCharToI64(szMsgId),
			szDestId,
			szPhone,      //手机号
			RptPack.nUsrMsgId,
			RptPack.msgcontent.szState,
			szCustId,
			RptPack.nPknum,
			RptPack.nPkTotal,
			RptPack.szSpNumEx,
			RptPack.szSendTime,
			RptPack.szTime,
			//zhubo 2017-4-8
			_strnicmp((LPCSTR)RptPack.msgcontent.szState, "DELIVRD", 7) == 0 ? 0 : 2,
			szExData
			);
	//	strcpy(szbuffer, s);
	}
	catch (...)
	{
		LOGF_ERROR("makeRptString 异常! \r\n", "FileRpt.txt");
	}
}

void CFileMoRpt::makeMoString(const PACK_DELIVER &MoPack, char *szbuffer)
{
	char szTime[MAX_DONETM_LEN+1]      = {0};
	char szSrcId[MAX_SRCTMID30_LEN+1]  = {0};
	char szDestId[MAX_DESTID_LEN+1]    = {0};
	char szMsg[2*MAX_MSG_LEN+1]        = {0};
	char szHexMsg[4*MAX_MSG_LEN+1]     = {0};
	char szSpNumEx[MAX_DESTID_LEN+1]   = {0};
	try
	{
		if (NULL == szbuffer)
		{
			return;
		}
		
		memcpy(szTime, MoPack.msgcontent.szDoneTime,min(strlen((const char *)MoPack.msgcontent.szDoneTime), MAX_DONETM_LEN));
		memcpy(szSrcId, MoPack.szSrcTerminalId, MAX_SRCTMID30_LEN);
		memcpy(szDestId, MoPack.szDestId, MAX_DESTID_LEN);
		int nCopyLen = min(2 * MAX_MSG_LEN, MoPack.nMsgLen);
		memcpy(szMsg, MoPack.szMsgContent, min(2 * MAX_MSG_LEN, nCopyLen));
		memcpy(szSpNumEx, MoPack.szSpNumEx, MAX_DESTID_LEN);
		CGlobal::MemBytesToHexStr((const unsigned char *)szMsg, nCopyLen, szHexMsg);

		__int64		I64MsgId = 0;
		I64MsgId = m_global.TranMsgIdCharToI64(MoPack.szMsgId);
		//格式为：时间,上行源号码,上行目标通道号,信息长度,信息格式,扩展号,签名,信息内容
		sprintf(szbuffer, "%s,%s,%s,%d,%d,%s,*,%s,%s,%lld\r\n", szTime,
			szSrcId,  //手机号 
			szDestId,
			MoPack.nMsgLen,
			MoPack.nMsgFmt,
			szSpNumEx,
			MoPack.szSignature,
			szHexMsg,
			I64MsgId
			);
	}
	catch (...)
	{
		LOGF_ERROR("makeMoString 异常! \r\n", "FileRpt.txt");
	}
}

BOOL CFileMoRpt::ResolveRptString(const char *szRptbuf, const int nUid, PACK_DELIVER &rptPack)
{
	if (NULL == szRptbuf)
	{
		return FALSE;
	}
	try
	{
		char szTime[MAX_TIME_LEN+1]        = {0};
		char szSeq[MAX_SEQ_LEN+1]          = {0};
		char szPhone[MAX_DESTTMID30_LEN+1] = {0}; 
		char szDestId[MAX_DESTID_LEN+1]    = {0};
		char szUsrSeq[MAX_SEQ_LEN+1]       = {0};
		char szState[MAX_ERSTAT_LEN+1]     = {0};

		char szCustId[200] = { 0 };
		char szExData[200] = { 0 };

		string strBuf = szRptbuf;
		string strTmp;
		
		rptPack.nUID = nUid;

		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		strTmp = cstr::left(strBuf,strBuf.find(','));
		strncpy(szTime, strTmp.c_str(), min(strTmp.length(), sizeof(szTime)-1));
		strcpy((char *)rptPack.szTime, szTime);

        SYSTEMTIME_EX tt;
        CAdapter::GetLocalTime(&tt);
        SYSTEMTIME_EX_LINUX t(tt);
    
		if (rptPack.szTime[0]== '\0')
		{
			sprintf(rptPack.szTime, "%04d-%02d-%02d %02d:%02d:%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);	
		}
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') -1);	
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);	

		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
//		strTmp = strBuf.Left(strBuf.find(','));
        strTmp = cstr::left(strBuf,strBuf.find(','));
		strncpy(szSeq, strTmp.c_str(), min(strTmp.length(), sizeof(szSeq)-1));
		INT64 nMsgId = _atoi64(szSeq);
		memset(rptPack.msgcontent.szMsgId, 0, sizeof(rptPack.msgcontent.szMsgId));
		memset(rptPack.szMsgId, 0, sizeof(rptPack.szMsgId));
		CGlobal::TranMsgIdI64ToChar(rptPack.msgcontent.szMsgId, nMsgId);
		memcpy(rptPack.szMsgId, rptPack.msgcontent.szMsgId, MAX_MSGID_LEN);
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') -1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);

		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));
		strncpy(szDestId, strTmp.c_str(), min(strTmp.length(), sizeof(szDestId)-1));
		strcpy((char *)rptPack.szDestId, szDestId);
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') -1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);

		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));
		strncpy(szPhone, strTmp.c_str(), min(strTmp.length(), sizeof(szPhone)-1));
		strcpy((char *)rptPack.msgcontent.szDestTerminalId, szPhone);
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') -1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);
        

		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));
		//strncpy(szUsrSeq, strTmp, min(strTmp.GetLength(), sizeof(szUsrSeq)-1));
		//rptPack.nUsrMsgId = _atoi64(szUsrSeq);
		strncpy(rptPack.szCustId, strTmp.c_str(), min(strTmp.length(), sizeof(rptPack.szCustId) - 1));
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') -1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);
        
		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));
		strncpy(szState, strTmp.c_str(), min(strTmp.length(), sizeof(szState)-1));
		strcpy((char *)rptPack.msgcontent.szState, szState);
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') - 1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);
        
		//wbs5.0用户自编流水号
		if (-1 == strBuf.find(','))
		{
			return FALSE; 
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));		
		strncpy(szCustId, strTmp.c_str(), min(strTmp.length(), sizeof(szCustId) - 1));
		//5.0读取4.0文件，读不到的取默认字段 custid,exdata只支持字母，数字，下划线，减号
		if ('*' != szCustId[0])
		{
            memset(rptPack.szCustId,0,sizeof(rptPack.szCustId)-1);
		    //memset(rptPack.szCustId,0,sizeof(rptPack.szCustId)-1);
			//BASE64解码
			m_global.Base64Decode((LPBYTE)szCustId, strlen(szCustId), (LPBYTE)rptPack.szCustId);
		}
		//strBuf = strBuf.Right(strBuf.length() - strBuf.Find(',') - 1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);
        
		//当前条数
		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));		
		
		char szPknum[10] = { 0 };
		strncpy(szPknum, strTmp.c_str(), min(strTmp.length(), sizeof(szPknum)-1));
		if ('*' != szPknum[0])
		{
			rptPack.nPknum = atoi(szPknum);
		}
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') - 1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);

		//总条数
		if (-1 == strBuf.find(','))
		{
			char szPkTotal[10] = { 0 };
			strncpy(szPkTotal, strBuf.c_str(), min(strBuf.length(), sizeof(szPkTotal)-1));
			if ('*' != szPknum[0])
			{
				rptPack.nPkTotal = atoi(szPkTotal);
			}
			return TRUE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));		
        
		char szPkTotal[10] = { 0 };
		strncpy(szPkTotal, strTmp.c_str(), min(strTmp.length(), sizeof(szPkTotal)-1));
		if ('*' != szPknum[0])
		{
			rptPack.nPkTotal = atoi(szPkTotal);
		}
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') - 1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);

		//扩展子号
		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));		
		strncpy((char*)rptPack.szSpNumEx, strTmp.c_str(), min(strTmp.length(), sizeof(rptPack.szSpNumEx) - 1));
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') - 1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);

		//下行时间
		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));		
		strncpy(rptPack.szSendTime, strTmp.c_str(), min(strTmp.length(), sizeof(rptPack.szSendTime) - 1));
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') - 1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);

		//返回时间
		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));	
		strncpy(rptPack.szTime, strTmp.c_str(), min(strTmp.length(), sizeof(rptPack.szTime) - 1));
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') - 1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);

		//发送状态
		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') - 1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);

		//扩展数据
		//去掉末尾的\r\n 
		cstr::trimright(strBuf, "\n");
		cstr::trimright(strBuf, "\r");
		strncpy(szExData, strBuf.c_str(), min(strBuf.length(), sizeof(szExData) - 1)); 
		
		//strncpy(szExData, strBuf.c_str(), min(strBuf.length()-2, sizeof(szExData) - 1)); //-2 "/r/n"
		
		//BASE64解码
		m_global.Base64Decode((LPBYTE)szExData, strlen(szExData), (LPBYTE)rptPack.szExData);
	}
	catch (...)
	{
		LOGF_ERROR("ResolveRptString  解析字符串时异常!", "FileRpt.txt");
		return FALSE;
	}
	
	return TRUE;
}	

BOOL CFileMoRpt::ResolveMoString(const char *szRptbuf, const int nUid, PACK_DELIVER &MoPack)
{
	if (NULL == szRptbuf)
	{
		return FALSE;
	}
	try
	{
		char szTime[MAX_TIME_LEN+1]        = {0};
		char szPhone[MAX_DESTTMID30_LEN+1] = {0};
		char szDestId[MAX_DESTID_LEN+1]    = {0};
		char szMsg[2*MAX_MSG_LEN+1]        = {0};
		char szHexMsg[4*MAX_MSG_LEN+1]     = {0};
		char szSpNumEx[MAX_DESTID_LEN+1]   = {0}; 
		char szTmp[7+1]                    = {0};
		
		string strBuf(szRptbuf);
		string strTmp("");
		
		memset(&MoPack, 0, sizeof(MoPack));

		MoPack.nUID = nUid;
		
		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
        strTmp = cstr::left(strBuf,strBuf.find(','));	
		strncpy(szTime, strTmp.c_str(), min(strTmp.length(), sizeof(szTime)-1));
		strcpy((char *)MoPack.msgcontent.szDoneTime, szTime);
		string strDateTime, strDate, strTime;

        SYSTEMTIME_EX tt;
        CAdapter::GetLocalTime(&tt);
        SYSTEMTIME_EX_LINUX t(tt);
        
		if (MoPack.msgcontent.szDoneTime[0] == '\0')
		{
			strTime = cstr::format("%04d%02d%02d%02d%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute);
			memcpy(MoPack.msgcontent.szDoneTime, strTime.c_str()+2, MAX_DONETM_LEN);
			sprintf(MoPack.szTime, "%04d-%02d-%02d %02d:%02d:%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
		}
		else
		{	
			string strSecond;
			strSecond = cstr::format(":%02d", t.wSecond);
			strDateTime = (char*)MoPack.msgcontent.szDoneTime;
//			strDate		= strDateTime.Left(6);
            strDate = cstr::left(strDateTime,6);
			strDate		= "20"+strDate;
            //zhubo 2017-4-8
            
			strDate.insert(4,"-");
			strDate.insert(7,"-");
//			strTime		= strDateTime.Right(4);
            strTime = cstr::right(strDateTime,4);
			strTime.insert(2,":");
			strTime		+= strSecond;
			
			
			strDate += " ";
			strDate+=	strTime;
			memcpy(MoPack.szTime, strDate.c_str(), min(sizeof(MoPack.szTime)-1,strDate.length() ) );
		}

		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') -1);
        strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);
        
		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.left(strBuf.find(','));
		 strTmp = cstr::left(strBuf,strBuf.find(','));
		strncpy(szPhone, strTmp.c_str(), min(strTmp.length(), sizeof(szPhone)-1));
		strcpy((char *)MoPack.szSrcTerminalId, szPhone);
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') -1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);
		
		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));
		strncpy(szDestId, strTmp.c_str(), min(strTmp.length(), sizeof(szDestId)-1));
		strcpy((char *)MoPack.szDestId, szDestId);
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') -1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);
        
		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));
		memset(szTmp, 0, sizeof(szTmp));
		strncpy(szTmp, strTmp.c_str(), min(strTmp.length(), sizeof(szTmp)-1));
		MoPack.nMsgLen = atoi(szTmp);
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') -1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);
		
		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));
		memset(szTmp, 0, sizeof(szTmp));
		strncpy(szTmp, strTmp.c_str(), min(strTmp.length(), sizeof(szTmp)-1));
		MoPack.nMsgFmt = atoi(szTmp);
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') -1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);

		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));
		strncpy((char *)MoPack.szSpNumEx, strTmp.c_str(), min(strTmp.length(), sizeof(MoPack.szSpNumEx)-1));
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') -1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);

		//*
		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		memset(szTmp, 0, sizeof(szTmp));
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));
		strncpy(szTmp, strTmp.c_str(), min(strTmp.length(), sizeof(szTmp)-1));
		if (0 != strcmp(szTmp,"*"))
		{
			return FALSE;
		}
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') -1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);
	
		//szSignature
		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));
		strncpy(MoPack.szSignature, strTmp.c_str(), min(strTmp.length(), sizeof(MoPack.szSignature)-1));
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') -1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);

		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));
		strncpy(szHexMsg, strTmp.c_str(), min(strTmp.length(), sizeof(szHexMsg)-1));

		//CGlobal::HexStrToMemBytes((unsigned char *)szMsg, szHexMsg, strlen(szHexMsg), sizeof(szMsg)-1);
		//strncpy((char *)MoPack.szMsgContent, szMsg, min(strlen(szMsg), (sizeof(MoPack.szMsgContent)-1)));
		//MoPack.nMsgLen = strlen((char*)MoPack.szMsgContent);//MoPack.nMsgLen>strlen(szMsg) ? strlen(szMsg):MoPack.nMsgLen;
		// dengyu 2017-6-9
		int nTmpLen = CGlobal::HexStrToMemBytes((unsigned char *)szMsg, szHexMsg, strlen(szHexMsg), sizeof(szMsg)-1);
		int nTmpCopyLen = min(nTmpLen, (sizeof(MoPack.szMsgContent) - 1));
		memcpy((char *)MoPack.szMsgContent, szMsg, nTmpCopyLen);
		MoPack.nMsgLen = nTmpCopyLen;//MoPack.nMsgLen>strlen(szMsg) ? strlen(szMsg):MoPack.nMsgLen;
		MoPack.szMsgContent[MoPack.nMsgLen] = '\0';
		
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') -1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);
		__int64		IPtMsgId = 0 ;
		sscanf(strBuf.c_str(),"%lld",&IPtMsgId);
		m_global.TranMsgIdI64ToChar(MoPack.szMsgId, IPtMsgId);

	}
	catch (...)
	{
		LOGF_ERROR("ResolveRptString  解析字符串时异常!", "FileRpt.txt");
		return FALSE;
	}
    //CLOG_DEBUG("quit . nUid=%d ", nUid);

	return TRUE;
	
}

int CFileMoRpt::ReadRptFile(const int nUid, const char* szpUsrName, std::vector<PACK_DELIVER> &RptPackArr)
{
	char szUsrFolderTmp[nMaxFilePathLen]   = {0};
	char szUsrFolderTmp2[nMaxFilePathLen]  = {0};
	char szNameTmp[MAX_SPID_LEN+1]         = {0}; 
	char szFilePath[nMaxFilePathLen]    = {0};    
	PACK_DELIVER RptPack;
	
	try
	{
		if (!CAdapter::PathFileExists(m_szRptfilePathShare))
		{
			//共享路径不存在，判断本地路径
			if (!CAdapter::PathFileExists(m_szRptfilePathLocal))
			{
				return -1;
			}
			else
			{
				//读本地
				if (0 != GetWbsValidFile(nUid, szpUsrName, szFilePath, m_szRptfilePathLocal))
				{
					return -1;
				}
			}
		}
		else
		{
			//共享路径存在
			if (0 != GetWbsValidFile(nUid, szpUsrName, szFilePath, m_szRptfilePathShare))
			{
				//本地是否存在
				if (!CAdapter::PathFileExists(m_szRptfilePathLocal))
				{
					return -1;
				}
				else
				{
					//读本地
					if (0 != GetWbsValidFile(nUid, szpUsrName, szFilePath, m_szRptfilePathLocal))
					{
						return -1;
					}
				}
			}
		}

		char szFilePathNew[256] = {0};
		char szFm[256] = {0};
		strncpy(szFm, szFilePath, strlen(szFilePath)-4);
		sprintf(szFilePathNew, "%s_bak_%d.tmp", szFm, m_iGateNO);
		int ret = rename(szFilePath, szFilePathNew);
		if (ret == 0)
		{
			ifstream fin(szFilePathNew);
			if (fin.is_open())
			{	
				char szData[MAX_RPT_BUFLEN+1] = {0};
				int nLine = 0;
				while (fin.getline(szData, MAX_RPT_BUFLEN + 1))
				{
					memset(&RptPack, 0, sizeof(RptPack));
					szData[MAX_RPT_BUFLEN] = 0;
					if (ResolveRptString(szData, nUid, RptPack))
					{
						RptPack.nReportFlag = FLAG_REPORT;
						memcpy(RptPack.szUsrId, szpUsrName, min(strlen(szpUsrName), MAX_SPID_LEN));
						RptPackArr.push_back(RptPack);
						nLine++;
					}	
					memset(szData, 0, sizeof(szData));
				}
				
				fin.close();
				if (0 != remove(szFilePathNew))
				{
					int errCode = errno;
					char s[128] = { 0 };
					snprintf(s,sizeof(s),"remove file error! fileName=%s,errno=%d \r\n", szFilePathNew, errCode);
					LOGF_ERROR(s, "FileRpt.txt");
					return -1;
				}
				
				return nLine;
			}
			else
			{
				fin.close();
				int ret = rename(szFilePathNew, szFilePath);
			}
		}
	}
	catch (...)
	{
		char s[128] = { 0 };
		snprintf(s,sizeof(s),"ReadRptFile 异常! nUid=%d \r\n", nUid);
		LOGF_ERROR(s, "FileRpt.txt");
		return -1;
	}

	return -1;
}

void CFileMoRpt::GetPathFromMap(int nUid, int &nCount, const char* szUsrFolder, char *szFilePath, INT64 &nlFileName)
{
	if (NULL == szFilePath)
	{
		return;
	}

	try	
	{
		FILEINFO rptInfo;
		std::multimap<int, FILEINFO> ::iterator itRpt;

		SafeLock slock(&m_lockRptQue);    //加锁
		itRpt = m_mapRpt.find(nUid);
		int nMapSize = m_mapRpt.count(nUid);
		if (itRpt != m_mapRpt.end())
		{
			for (int n=0; n<nMapSize; n++)
			{
				memset(szFilePath, 0, sizeof(szFilePath));
				rptInfo = itRpt->second;
				nlFileName = rptInfo.nlFilePath;
				m_mapRpt.erase(itRpt++);
				
				if (0 == nlFileName)
				{
					continue;
				}
			
				//形成文件名 dengyu 2017-6-13
				sprintf(szFilePath, "%s/%lld_n%d.txt", szUsrFolder, nlFileName, rptInfo.nFileCount);
				
				if (!CAdapter::PathFileExists(szFilePath))
				{
					memset(szFilePath, 0, sizeof(szFilePath));
					continue;
				}
				else 
				{
					nCount = rptInfo.nFileCount;
					break;
				}
			}	
		}
	}
	catch (...)
	{
		char s[128] = { 0 };
		snprintf(s,sizeof(s),"GetPathFromMap 异常!  nUid=%d  \r\n", nUid);
		LOGF_ERROR(s, "FileRpt.txt");
	}
}

void CFileMoRpt::GetPathFromMoMap(int nUid, int &nCount,const char* szUsrFolder, char *szFilePath, INT64 &nlFileName)
{
	if (NULL == szFilePath)
	{
		return;
	}
	
	try	
	{
		FILEINFO MoInfo;
		std::multimap<int, FILEINFO> ::iterator itMo;
		
		SafeLock slock(&m_lockMoQue);    //加锁.
		itMo = m_mapMo.find(nUid);
		if (itMo != m_mapMo.end())
		{
			int nMapSize = m_mapMo.count(nUid);
			for (int n=0; n<nMapSize; ++n)
			{
				memset(szFilePath, 0, sizeof(szFilePath));
				MoInfo = itMo->second;
				nlFileName = MoInfo.nlFilePath;
				m_mapMo.erase(itMo++);
				
				if (0 == nlFileName)
				{
					continue;
				}
				
				//形成文件名
				sprintf(szFilePath, "%s/%lld_n%d.txt", szUsrFolder, nlFileName, MoInfo.nFileCount);
				
				if (!CAdapter::PathFileExists(szFilePath))
				{
					memset(szFilePath, 0, sizeof(szFilePath));
					nlFileName = 0;
					continue;
				}
				else 
				{
					nCount = MoInfo.nFileCount;
					break;
				}
			}	
		}
	}
	catch (...)
	{
		char s[128] = { 0 };
		snprintf(s,sizeof(s),"GetPathFromMoMap 异常!  nUid=%d  \r\n", nUid);
		LOGF_ERROR(s, "FileRpt.txt");
	}
}

BOOL CFileMoRpt::IsPathOk(char *szpath)
{
	try
	{
		char szFm[8] = {0};
		strcpy(szFm, szpath+(strlen(szpath)-4));
		if (CAdapter::StrStrI(".txt", szFm))
		{
			if (NULL == szpath)
			{
				return FALSE;
			}

			return CAdapter::PathFileExists(szpath);
		}
		else
		{
			return FALSE;
		}
	}
	catch (...)
	{
		char s[128] = { 0 };
		snprintf(s,sizeof(s),"IsPathOk 异常! szpath=%s \r\n", szpath);
		LOGF_ERROR(s, "FileRpt.txt");

		return FALSE;
	}	
}


BOOL CFileMoRpt::GetValidFile(const char *strFolder, char *strFilePath)
{

	if (NULL == strFolder)
	{
		return FALSE;
	}
	try
	{
        if (!CAdapter::PathFileExists(strFolder))
        {
            return FALSE;
        }
        //CLOG_DEBUG("EnumFile call begin");

        /*******************zhubo 2017-4-12**********************/
        std::vector<std::string> vecFileNames;
        CAdapter::EnumFile(strFolder,".txt",vecFileNames,1);
        //CLOG_DEBUG("EnumFile call end");

        if(vecFileNames.size() <= 0)
        {
            return FALSE;
        }
        std::string strPath = *(vecFileNames.begin());
        strcpy(strFilePath, strPath.c_str());
        /*******************zhubo 2017-4-12**********************/
        //CLOG_DEBUG("quit vecFileNames.size()=%d,strFolder=%s strFilePath=%s", vecFileNames.size(), strFolder,strFilePath);

        // add by dengyu 2017-5-10
        return TRUE;
        
        /*
		CFileFind fFile;
		CString strFileName;
		char   szFm[nMaxFilePathLen]         = {0};
		char   szTmpFolder[nMaxFilePathLen]  = {0};
		sprintf(szTmpFolder, "%s\\*.*", strFolder);
		BOOL bIsFile = fFile.FindFile(szTmpFolder,0);
		while (bIsFile)
		{
			bIsFile = fFile.FindNextFile();
			
			if (!fFile.IsDirectory() 
				&& !fFile.IsSystem() 
				&& !fFile.IsHidden() 
				&& !fFile.IsDots() 
				&& !fFile.IsTemporary())
			{
				strFileName = "";
				strFileName = fFile.GetFilePath();
				if (strFileName.GetLength() < 4)
				{
					continue;
				}

				if (-1 != strFileName.Find(".txt"))
				{
					if (PathFileExists(strFileName))
					{
						strcpy(strFilePath, strFileName);
						return TRUE;
					}
				}
			}	
		}
        */
        
	}
	catch (...)
	{
		char s[128] = { 0 };
		snprintf(s,sizeof(s),"GetValidFile 异常! strFolder=%s; \r\n", strFolder);
		LOGF_ERROR(s, "FileRpt.txt");
	}
	
	return FALSE;
}

int CFileMoRpt::GetFileCount(const int nUid)
{
    //zhubo 2017-4-8
   
	if (strlen(m_szRptfilePathShare) < 1 && strlen(m_szRptfilePathLocal) < 1)
	{
		CreateFolder((char*)"FILERPT", m_szRptfilePathShare, m_szRptfilePathLocal);
	}
	/*
	int    nFileCountShare  = 0;
	int    nFileCountLocal  = 0;
	CFileFind fFile;
	char   szName[nMaxFilePathLen] = {0};
	char   szFm[nMaxFilePathLen]   = {0};
	char   szTmpFolder[nMaxFilePathLen]  = {0};
	sprintf(szTmpFolder, "%s\\%d\\*.*", m_szRptfilePathShare, nUid);
	BOOL bIsFileShare = fFile.FindFile(szTmpFolder,0);

	sprintf(szTmpFolder, "%s\\%d\\*.*", m_szRptfilePathLocal, nUid);
	BOOL bIsFileLocal = fFile.FindFile(szTmpFolder,0);

	if(bIsFileShare)
	{
		while (bIsFileShare)
		{
			bIsFileShare = fFile.FindNextFile();
			memset(szName, 0, sizeof(szName));
			memset(szFm, 0, sizeof(szFm));
			strcpy(szName, fFile.GetFilePath());
			if (strlen(szName) < 4)
			{
				continue;
			}
			strcpy(szFm, szName+(strlen(szName)-4));
			if (CAdapter::StrStrI(".txt", szFm))
			{
				if (IsPathOk(szName))
				{
					nFileCountShare ++;
				}
// 				SafeLock slock(&m_lockRpt);    //加锁
// 				FILE *fp = fopen(szName, "r");
// 				if (fp)
// 				{
// 					fclose(fp);
// 					nFileCount ++;
// 				}
			}
		}
	}

	if(bIsFileLocal)
	{
		while (bIsFileLocal)
		{
			bIsFileLocal = fFile.FindNextFile();
			memset(szName, 0, sizeof(szName));
			memset(szFm, 0, sizeof(szFm));
			strcpy(szName, fFile.GetFilePath());
			if (strlen(szName) < 4)
			{
				continue;
			}
			strcpy(szFm, szName+(strlen(szName)-4));
			if (StrStrI(".txt", szFm))
			{
				if (IsPathOk(szName))
				{
					nFileCountLocal ++;
				}
				// 				SafeLock slock(&m_lockRpt);    //加锁
				// 				FILE *fp = fopen(szName, "r");
				// 				if (fp)
				// 				{
				// 					fclose(fp);
				// 					nFileCount ++;
				// 				}
			}
		}
	}
	
	return nFileCountShare + nFileCountLocal;
    */
    return -1;

}

BOOL CFileMoRpt::CreateWbsFolder(char *szFolder, char* szFilePath, char* szPathBuf, int nLen)
{
	string strPathbuf; 
	strPathbuf = szPathBuf;
    /*******************zhubo 2017-4-12**********************/
    /*
	memset(szPathBuf, 0, nLen);
	strncpy(szPathBuf, strPathbuf.c_str(), strPathbuf.find_last_of('\\'));
	strPathbuf = szPathBuf;
	strPathbuf += "\\";
	strPathbuf += szFolder;
	strncpy(szFilePath, strPathbuf.c_str(), strPathbuf.length());
	*/
    /*******************zhubo 2017-4-12**********************/
    // dengyu 2017-6-13
    // strPathbuf += "/";
    strPathbuf += szFolder;
  	strncpy(szFilePath, strPathbuf.c_str(), min(strPathbuf.length(),nLen-1));
	if (!CAdapter::PathFileExists(szFilePath))
	{
		CAdapter::CreateDirectory(szFilePath, NULL);
	}
	return TRUE;
}

BOOL CFileMoRpt::GetWbsFolder(char *szFolder, char* szFilePath, char* szPathBuf, int nLen)
{
	string strPathbuf; 
	strPathbuf = szPathBuf;
	memset(szPathBuf, 0, nLen);
	strncpy(szPathBuf, strPathbuf.c_str(), strPathbuf.find_last_of('/'));
	strPathbuf = szPathBuf;
	strPathbuf += "/";
	strPathbuf += szFolder;
	strncpy(szFilePath, strPathbuf.c_str(), strPathbuf.length());
	return TRUE;
}

BOOL CFileMoRpt::CreateFolder(char *szFolder, char *szFilePathShare, char *szFilePathLocal)
{
	try
	{
		BOOL bRet = TRUE;
		char szPathBuf[nMaxFilePathLen] = { 0 };

		sprintf(szPathBuf, "%s/", szFilePathShare);
		if (!CAdapter::PathFileExists(szPathBuf))
		{
			//记录共享路径
			GetWbsFolder(szFolder, szFilePathShare, szPathBuf, 512);
		}
		else
		{
			//创建共享目录
			bRet = CreateWbsFolder(szFolder, szFilePathShare, szPathBuf, 512);

			//记录共享路径
			GetWbsFolder(szFolder, szFilePathShare, szPathBuf, 512);
		}

		memset(szPathBuf, 0, sizeof(szPathBuf));
		sprintf(szPathBuf, "%s/", szFilePathLocal);
		if (!CAdapter::PathFileExists(szPathBuf))
		{
			//记录本地路径
			GetWbsFolder(szFolder, szFilePathLocal, szPathBuf, 512);
		}
		else
		{
			//创建本地目录
			bRet = CreateWbsFolder(szFolder, szFilePathLocal, szPathBuf, 512);

			//记录本地路径
			GetWbsFolder(szFolder, szFilePathLocal, szPathBuf, 512);
		}

		return bRet;
	}
	catch (...)
	{
		char s[128] = { 0 };
		snprintf(s, sizeof(s), "CreateFolder 异常! szFolder=%s; \r\n", szFolder);
		LOGF_ERROR(s, "FileRpt.txt");

		return FALSE;
	}
}

int CFileMoRpt::GetMoRptWaitInfo()
{
	try
	{
		// 获取MO滞留量
		if (CAdapter::PathFileExists(m_szMofilePathShare))
		{
			ProcessDirectory(m_szMofilePathShare, m_szMofilePathShare,FLAG_MO);
		}
		else if (CAdapter::PathFileExists(m_szMofilePathLocal))
		{
			ProcessDirectory(m_szMofilePathLocal, m_szMofilePathLocal,FLAG_MO);
		}

		// 获取RPT滞留量
		if (CAdapter::PathFileExists(m_szRptfilePathShare))
		{
			ProcessDirectory(m_szRptfilePathShare, m_szRptfilePathShare,FLAG_REPORT);
		}
		else if (CAdapter::PathFileExists(m_szRptfilePathLocal))
		{
			ProcessDirectory(m_szRptfilePathLocal, m_szRptfilePathLocal,FLAG_REPORT);
		}
	}
	catch(...)
	{
		LOGF_ERROR("获取MO/RPT滞留量时系统异常!", "FileMoRpt.txt");
		return -1;
	}

	return 0;
}

// dengyu 2017-6-9
/**
 * 遍历指定目录，统计目录下的文件行数，会递归调用
 */
void CFileMoRpt::ProcessDirectory(const std::string& dirPath, const std::string& dirSrcPath,int nFlag,int nDepth)
{
	DIR* dp = NULL;
	struct dirent* entry = NULL;
	struct stat statbuf;

    //zhubo 2017-12-08 不遍历子文件夹下的子文件夹
    if(nDepth >= 3)
    {
        return;
    }    
    //zhubo 2017-12-08 不遍历子文件夹下的子文件夹

	if (NULL == (dp = opendir(dirPath.c_str())))
	{
		char s[128] = { 0 };
		snprintf(s, sizeof(s), "打开目录异常! dirPath=%s; \r\n", dirPath.c_str());
		LOGF_ERROR(s, "FileMoRpt.txt");
		return;
	}
    string strTempDir = "";
	
	int nFileCountsMo = 0;
	while (NULL != (entry = readdir(dp)))
	{
		// 过滤掉[.]和[..]目录
		if (strncmp(entry->d_name, ".", 1) == 0 || strncmp(entry->d_name, "..", 2) == 0)
			continue;

        strTempDir = dirPath;
        strTempDir += "/";
        strTempDir += entry->d_name;

		// 4: 表示文件夹 8: 表示文件
		//zhubo 2018-01-29
		//if (4 == entry->d_type)
		if(CAdapter::IsDir(strTempDir))
		{
			// 是文件夹直接递归
			ProcessDirectory(dirPath + '/' + entry->d_name, dirSrcPath,nFlag,++nDepth);
		}
        //zhubo 2018-01-29
		//else if (8 == entry->d_type)
		else if (CAdapter::IsRegularFile(strTempDir))
		{
			// 是文件则统计行数
			nFileCountsMo += CountFileLines(entry->d_name, 25);
		}
		else
		{
			continue;
		}
	}

	// 过滤掉要遍历的根目录
	if (dirPath != dirSrcPath)
	{
		size_t pos = dirPath.find_last_of('/');
		if (pos == std::string::npos)
		{
			//return;
			goto END;
		}
		std::string buffer = dirPath.substr(pos + 1);
		CStatsMgr_Http::GetInstance().GetMoRptWaitInfo(const_cast<char* >(buffer.c_str()), nFileCountsMo, nFlag);

	}

	// 这个很重要,递归调用时用到,必须要加chdir(..)
	// chdir("..");
END:
    closedir(dp);
    dp = NULL;
}


int CFileMoRpt::CountFileLines(char *filename, int n)
{
	//从文件名中截取条数"11111111111_n90.txt"
	int nCount = 0;

	try
	{
		char *p1 = NULL;
		char *p2 = NULL;
		char *p3 = NULL;               // 正在读的文件
		char *p4 = NULL;               // 正在写的文件
		p3 = strstr(filename, "_bak");
		if (p3 != NULL)
		{
			//如果能找到_bak表示正在读取的文件
			return n;
		}
		p4 = strstr(filename, ".tmp");
		if (p4 != NULL)
		{
			return 0;
		}
		p1 = strstr(filename, "_n");
		if (p1 == NULL)
		{
			//如果找不到'_n',表示是老文件
			nCount = n;
		}
		else
		{
			//按新文件命令方式获取条数
			p2 = strstr(filename, ".txt");
			int nLen = strlen(filename) - strlen(p1) + 2;
			char szCount[5] = { 0 };
			memcpy(szCount, filename + nLen, min(p2 - p1 - 2, 4));
			nCount = atoi(szCount);
		}
	}
	catch (...)
	{
	}
	return nCount;
}
