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
	//���ر��(Ŀǰֻ֧�ֵ�����)
	m_iGateNO = nGateNO;
	int nLen = 0;

	//MO����
	nLen = min(strlen(szWbsLocalPath), MAX_PATH);
	memcpy(m_szMofilePathLocal, szWbsLocalPath, nLen);
	m_szMofilePathLocal[nLen] = '\0';

	//RPT����
	nLen = min(strlen(szWbsLocalPath), MAX_PATH);
	memcpy(m_szRptfilePathLocal, szWbsLocalPath, nLen);
	m_szRptfilePathLocal[nLen] = '\0';

	//MO����
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

		//�������·��δ���� ������·��ȡ����·��
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
	int nQueSize = RptQue.size();        //�����е�RPT����
	if (nQueSize <= 0)
	{
		return 0;
	}
	try
	{
		if (!CAdapter::PathFileExists(m_szRptfilePathShare))
		{
			//��ȷ�Ĺ���·���ϵ��󱨾�
			if ( 0 != strlen(m_szRptfilePathShare))
			{
				//�澯
				//������־��ض���
				string s = "Rpt�ļ�����·��������,���������Ƿ�����!";
				char szValue[25] = {0};
				sprintf(szValue, "%d", 1);
                
				CPtMonitor::GetInstance().AddLogToMonQue((int)LOG_SYS_ERROR, s.c_str(), 10015, "CFileMoRpt::WriteRpttoFile", szValue, true);
			}

			//��������·��
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
				//�澯
				char szValue[25] = {0};
				sprintf(szValue, "%d", 0);
				
				CPtMonitor::GetInstance().CommonMonEvent((int)LOG_SYS_ERROR, "����RPTĿ¼�ָ�����", 10015, "CFileMoRpt::WriteRpttoFile", szValue, false);
			}
		}
		
		PACK_DELIVER rptPack = RptQue[0]; 
		FILEINFO rptInfo;                               //�ļ���Ϣ
		char szFileName[nMaxFilePathLen]  = {0};        //�ļ�����·��
		char szUsrFolder[nMaxFilePathLen] = {0};        //�ļ�����
		INT64 nlFileName                  = 0;          //�������ļ���
		int nFileCount                    = 0;          //�����ļ��е�RPT����
		
		if (!CAdapter::PathFileExists(m_szRptfilePathShare))
		{
			//д����
			if (strlen((char *)rptPack.szUsrId)>2)
			{
				sprintf(szUsrFolder, "%s/%s", m_szRptfilePathLocal, rptPack.szUsrId);    //�û�Ŀ¼
			}
			else
			{
				sprintf(szUsrFolder, "%s/%d", m_szRptfilePathLocal, rptPack.nUID);    //�û�Ŀ¼	
			}
			if (!CAdapter::PathFileExists(szUsrFolder))
			{
				CAdapter::CreateDirectory(szUsrFolder, NULL);
			}
		}
		else
		{
			//д����
 			if (strlen((char *)rptPack.szUsrId)>2)
 			{
 				sprintf(szUsrFolder, "%s/%s", m_szRptfilePathShare, rptPack.szUsrId);    //�û�Ŀ¼
 			}
 			else
 			{
				sprintf(szUsrFolder, "%s/%d", m_szRptfilePathShare, rptPack.nUID);    //�û�Ŀ¼	
			}
			if (!CAdapter::PathFileExists(szUsrFolder))
			{
				CAdapter::CreateDirectory(szUsrFolder, NULL);
			}
		}

		//С��80���Ĳ����ļ���Ϣ���У�׷�ӵ����е��ļ���
		if (nQueSize < RPT_FILE_NUM)
		{
			//��mapȡ���ļ���Ϣ�����û���¼�
			GetPathFromMap(rptPack.nUID, nFileCount, szUsrFolder, szFileName, nlFileName);		
			
			//ȡ�ļ���ʧ��
			if (strlen(szFileName) < 1)
			{
				nFileCount = 0;
				MakeFileName(szUsrFolder, szFileName, nlFileName);
			}

			//�ļ�����+��������<80����������һ���µļ�¼������map��
			if ((nFileCount+nQueSize) < RPT_FILE_NUM)
			{
				rptInfo.nlFilePath  = nlFileName;
				rptInfo.nFileCount  = nFileCount+nQueSize;
				rptInfo.nUid        = rptPack.nUID;
				rptInfo.timeFile    = time(NULL);
				AddToMap(rptPack.nUID, rptInfo);	
			}
		}
		//�պ�80��ֱ��д��һ���ļ���
		else     
		{
			nFileCount = 0;                           //���ļ�������Ϊ0
			MakeFileName(szUsrFolder, szFileName, nlFileName);
		}

		//дһ�����
		if (-1 == WriteRpt(RptQue, nFileCount, szFileName))
		{
			return -1;
		}

		//������ʣ�µ���дһ�µ��ļ�
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
		LOGF_ERROR("WriteRpttoFile �쳣!", "FileRpt.txt");
		return -1;
	}
	return 0;
}

int  CFileMoRpt::WriteMotoFile(std::vector<PACK_DELIVER> &MoQue)        //��Moд���ļ�
{
	int nQueSize = MoQue.size();        //�����е�RPT����
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
				//�澯
				//������־��ض���
				string s = "Mo�ļ�����·��������,���������Ƿ�����!";
				char szValue[25] = {0};
				sprintf(szValue, "%d", 1);

				CPtMonitor::GetInstance().AddLogToMonQue((int)LOG_SYS_ERROR, s.c_str(), 10014, "CFileMoRpt::WriteMotoFile", szValue, true);
			}

			//��������·��
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
				//�澯
				char szValue[25] = {0};
				sprintf(szValue, "%d", 0);
				
				CPtMonitor::GetInstance().CommonMonEvent((int)LOG_SYS_ERROR, "����MoĿ¼�ָ�����", 10014, "CFileMoRpt::WriteMotoFile", szValue, false);
			}
		}
		
		PACK_DELIVER MoPack = MoQue[0]; 
		FILEINFO MoInfo;                               //�ļ���Ϣ
		char szFileName[nMaxFilePathLen]  = {0};        //�ļ�����·��
		char szUsrFolder[nMaxFilePathLen] = {0};        //�ļ�����
		INT64 nlFileName                  = 0;          //�������ļ���
		int nFileCount                    = 0;          //�����ļ��е�RPT����
		if (!CAdapter::PathFileExists(m_szMofilePathShare))
		{
			//д����
			if (strlen((char *)MoPack.szUsrId)>2)
			{
				sprintf(szUsrFolder, "%s/%s", m_szMofilePathLocal, MoPack.szUsrId);    //�û�Ŀ¼
			}
			else
			{
				sprintf(szUsrFolder, "%s/%d", m_szMofilePathLocal, MoPack.nUID);    //�û�Ŀ¼	
			}
			if (!CAdapter::PathFileExists(szUsrFolder))
			{
				CAdapter::CreateDirectory(szUsrFolder, NULL);
			}
		}
		else
		{
			//д����
			if (strlen((char *)MoPack.szUsrId)>2)
			{
				sprintf(szUsrFolder, "%s/%s", m_szMofilePathShare, MoPack.szUsrId);    //�û�Ŀ¼
			}
			else
			{
				sprintf(szUsrFolder, "%s/%d", m_szMofilePathShare, MoPack.nUID);    //�û�Ŀ¼	
			}
			if (!CAdapter::PathFileExists(szUsrFolder))
			{
				CAdapter::CreateDirectory(szUsrFolder, NULL);
			}
		}
		
		//С��80���Ĳ����ļ���Ϣ���У�׷�ӵ����е��ļ���
		if (nQueSize < MO_FILE_NUM)
		{
			//��mapȡ���ļ���Ϣ�����û���¼�
			GetPathFromMoMap(MoPack.nUID, nFileCount, szUsrFolder, szFileName, nlFileName);		
			
			//ȡ�ļ���ʧ��
			if (strlen(szFileName) < 1)
			{
				nFileCount = 0;
				MakeFileName(szUsrFolder, szFileName, nlFileName);
			}
			
			//�ļ�����+��������<25����������һ���µļ�¼������map��
			if ((nFileCount + nQueSize) < MO_FILE_NUM)
			{
				MoInfo.nlFilePath  = nlFileName;
				MoInfo.nFileCount  = nFileCount + nQueSize;
				MoInfo.nUid        = MoPack.nUID;
				MoInfo.timeFile    = time(NULL);
				AddToMoMap(MoPack.nUID, MoInfo);	
			}
		}
		//�պ�80��ֱ��д��һ���ļ���
		else     
		{
			nFileCount = 0;                     //���ļ�������Ϊ0
			MakeFileName(szUsrFolder, szFileName, nlFileName);
		}
		
		//дһ�����
		if (-1 == WriteMo(MoQue, nFileCount, szFileName))
		{
			return -1;
		}
		
		//������ʣ�µ���дһ�µ��ļ�
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
		LOGF_ERROR("WriteMotoFile �쳣!", "FileMo.txt");
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
		//���˺�תΪ��д
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
			//����·�������ڣ��жϱ���·��
			if (!CAdapter::PathFileExists(m_szMofilePathLocal))
			{
				return -1;
			}
			else
			{
				//������
				if (0 != GetWbsValidFile(nUid, szpUsrName, szFilePath, m_szMofilePathLocal))
				{
                                 //CLOG_DEBUG("quit fail GetWbsValidFile nUid=%d szpUsrName=%s ", nUid, szpUsrName);
					return -1;
				}
			}
		}
		else
		{
			//����·������
			if (0 != GetWbsValidFile(nUid, szpUsrName, szFilePath, m_szMofilePathShare))
			{
				//�����Ƿ����
				if (!CAdapter::PathFileExists(m_szMofilePathLocal))
				{
		             return -1;
				}
				else
				{
					//������
					if (0 != GetWbsValidFile(nUid, szpUsrName, szFilePath, m_szMofilePathLocal))
					{
						return -1;
					}
				}
			}
		}

		//������
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
				//��������ȥ
				fin.close();
				int ret = rename(szFilePathNew, szFilePath);
			}
		}
	}
	catch (...)
	{
	    char s[128] = { 0 };
		snprintf(s,sizeof(s),"ReadMoFile �쳣! nUid=%d \r\n", nUid);
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

		// �ĳ���ʱ���ļ�,д����ٸĳ�txt�ļ�
		sprintf(szFileName, "%s/%lld_n0.tmp", szUsrFolder, nlFileName);	
	}
	catch (...)
	{
		string s = "MakeFileName �쳣!";
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
						// ���ַ���д�뵽�ļ�
						fout.write(szbuffer, strlen(szbuffer));
					}
				}
				catch(...)
				{
					char s[128] = { 0 };	
					snprintf(s,sizeof(s),"WriteRpt �쳣! �ļ�·��:%s", szFileName);
					LOGF_ERROR(s, "FileRpt.txt");
				}
			}
			
			fout.close();
			
			if (itQue != RptQue.begin())
			{
				RptQue.erase(RptQue.begin(), RptQue.begin() + nCount);
			}

			//�����ļ�������Ϊ���������ļ���
			const char *p1 =strstr(szFileName, "_n");
			char szFilePathNew[256] = { 0 };
			char szFm[256] = { 0 };
			if (p1 != NULL)
			{
				//����
				strncpy(szFm, szFileName, strlen(szFileName) - strlen(p1));
			}
			else
			{
				strncpy(szFm, szFileName, strlen(szFileName) - 4);
			}
			sprintf(szFilePathNew, "%s_n%d.txt", szFm, nFileCount);
			int ret = rename(szFileName, szFilePathNew);   //ʧ����ʱ������
		}
		else
		{
			//����򲻿�����Ϊ���ļ�
			if(strstr(szFileName,".tmp") != NULL)
			{
				remove(szFileName);
			}
		}
	}
	catch (...)
	{
		fout.close();
		
		//ɾ������д���쳣�������ļ�
		if(strstr(szFileName,".tmp") != NULL)
		{
			remove(szFileName);
		}
		
		char s[128] = { 0 };	
		snprintf(s,sizeof(s),"WriteRpt �쳣! �ļ�·��:%s", szFileName);
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
						// ���ַ���д�뵽�ļ�
						fout.write(szbuffer, strlen(szbuffer));
					}
				}
				catch(...)
				{
					char s[128] = { 0 };	
					snprintf(s,sizeof(s),"WriteMo �쳣! �ļ�·��:%s", szFileName);
					LOGF_ERROR(s, "FileMo.txt");
				}
			}
			
			fout.close();
			
			if (itMo != MoQue.begin())
			{
				MoQue.erase(MoQue.begin(), MoQue.begin() + nCount);
			}

			//�����ļ�������Ϊ���������ļ���
			const char *p1 = strstr(szFileName, "_n");
			char szFilePathNew[256] = { 0 };
			char szFm[256] = { 0 };
			
			if (p1 != NULL)
			{
				//����
				strncpy(szFm, szFileName, strlen(szFileName) - strlen(p1));
			}
			else
			{
				strncpy(szFm, szFileName, strlen(szFileName) - 4);
			}
			
			sprintf(szFilePathNew, "%s_n%d.txt", szFm, nFileCount);
			int ret = rename(szFileName, szFilePathNew);	//ʧ����ʱ������
		}
		else
		{
			//����򲻿�����Ϊ���ļ�
			if(strstr(szFileName,".tmp") !=NULL)
			{
				remove(szFileName);
			}
		}
	}
	catch (...)
	{
		fout.close();
		
		//ɾ������д���쳣�������ļ�
		if(strstr(szFileName,".tmp") != NULL)
		{
			remove(szFileName);
		}
		
		char s[128] = { 0 };
		snprintf(s,sizeof(s),"WriteMo �쳣! filename=%s", szFileName);
		LOGF_ERROR(s, "FileMo.txt");
		return -1;	
	}
	
	return 0;
}	

void CFileMoRpt::AddToMap(int key, const FILEINFO &rptInfo)
{
	try
	{
		SafeLock slock(&m_lockRptQue);    //����
		if (m_mapRpt.size() < g_nMaxSize)
		{
			m_mapRpt.insert(std::make_pair(key, rptInfo));
		}
		else
		{
			//���������ʱ�����������
			try
			{
				FILEINFO fInfo;
				const int CLEANTIME = 43200;    //����12Сʱǰ����Ϣ 12*60*60
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
				LOGF_ERROR("�������ʱ�쳣! \r\n", "FileRpt.txt");
			}		
		}
	}
	catch (...)
	{
		char s[128] = { 0 };	
		snprintf(s,sizeof(s),"AddToMap ����m_mapRptʱ�쳣! m_mapRpt.size()=%d;\r\n", m_mapRpt.size());
		LOGF_ERROR(s, "FileRpt.txt");
	}
}

void CFileMoRpt::AddToMoMap(int key, const FILEINFO &MoInfo)
{
	try
	{
		SafeLock slock(&m_lockMoQue);    //����
		if (m_mapMo.size() < g_nMaxSize)
		{
			m_mapMo.insert(std::make_pair(key, MoInfo));
		}
		else
		{
			//���������ʱ�����������
			try
			{
				FILEINFO fInfo;
				const int CLEANTIME = 43200;    //����12Сʱǰ����Ϣ 12*60*60
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
				LOGF_ERROR("����Mo����ʱ�쳣! \r\n", "FileRpt.txt");
			}		
		}
	}
	catch (...)
	{
		char s[128] = { 0 };
		snprintf(s,sizeof(s),"AddToMoMap ����m_mapMoʱ�쳣! m_mapMo.size()=%d;\r\n", m_mapMo.size());
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

		//��wbs5.0�е�custid��exdata����base64����(���ܻẬ�ж���)
		char szCustId[200] = { 0 };
		char szExData[200] = { 0 };
		m_global.Base64Encode((LPBYTE)RptPack.szCustId, strlen(RptPack.szCustId), (LPBYTE)szCustId);
		m_global.Base64Encode((LPBYTE)RptPack.szExData, strlen(RptPack.szExData), (LPBYTE)szExData);

		//��ʽΪ��ʱ��,��ˮ�ţ�ͨ���ţ��ֻ��ţ��û��Ա���ˮ�ţ�״ֵ̬,
		//wbs5.0�û��Ա���ˮ��,��ǰ����������������չ�Ӻţ�����ʱ�䣬����ʱ�䣬����״̬����չ����
		sprintf(szbuffer, "%s,%lld,%s,%s,%lld,%s,%s,%d,%d,%s,%s,%s,%d,%s\r\n", szTime, 
			m_global.TranMsgIdCharToI64(szMsgId),
			szDestId,
			szPhone,      //�ֻ���
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
		LOGF_ERROR("makeRptString �쳣! \r\n", "FileRpt.txt");
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
		//��ʽΪ��ʱ��,����Դ����,����Ŀ��ͨ����,��Ϣ����,��Ϣ��ʽ,��չ��,ǩ��,��Ϣ����
		sprintf(szbuffer, "%s,%s,%s,%d,%d,%s,*,%s,%s,%lld\r\n", szTime,
			szSrcId,  //�ֻ��� 
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
		LOGF_ERROR("makeMoString �쳣! \r\n", "FileRpt.txt");
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
        
		//wbs5.0�û��Ա���ˮ��
		if (-1 == strBuf.find(','))
		{
			return FALSE; 
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));		
		strncpy(szCustId, strTmp.c_str(), min(strTmp.length(), sizeof(szCustId) - 1));
		//5.0��ȡ4.0�ļ�����������ȡĬ���ֶ� custid,exdataֻ֧����ĸ�����֣��»��ߣ�����
		if ('*' != szCustId[0])
		{
            memset(rptPack.szCustId,0,sizeof(rptPack.szCustId)-1);
		    //memset(rptPack.szCustId,0,sizeof(rptPack.szCustId)-1);
			//BASE64����
			m_global.Base64Decode((LPBYTE)szCustId, strlen(szCustId), (LPBYTE)rptPack.szCustId);
		}
		//strBuf = strBuf.Right(strBuf.length() - strBuf.Find(',') - 1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);
        
		//��ǰ����
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

		//������
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

		//��չ�Ӻ�
		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));		
		strncpy((char*)rptPack.szSpNumEx, strTmp.c_str(), min(strTmp.length(), sizeof(rptPack.szSpNumEx) - 1));
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') - 1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);

		//����ʱ��
		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));		
		strncpy(rptPack.szSendTime, strTmp.c_str(), min(strTmp.length(), sizeof(rptPack.szSendTime) - 1));
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') - 1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);

		//����ʱ��
		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));	
		strncpy(rptPack.szTime, strTmp.c_str(), min(strTmp.length(), sizeof(rptPack.szTime) - 1));
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') - 1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);

		//����״̬
		if (-1 == strBuf.find(','))
		{
			return FALSE;
		}
		//strTmp = strBuf.Left(strBuf.find(','));
		strTmp = cstr::left(strBuf,strBuf.find(','));
		//strBuf = strBuf.Right(strBuf.length() - strBuf.find(',') - 1);
		strBuf = cstr::right(strBuf,strBuf.length() - strBuf.find(',') -1);

		//��չ����
		//ȥ��ĩβ��\r\n 
		cstr::trimright(strBuf, "\n");
		cstr::trimright(strBuf, "\r");
		strncpy(szExData, strBuf.c_str(), min(strBuf.length(), sizeof(szExData) - 1)); 
		
		//strncpy(szExData, strBuf.c_str(), min(strBuf.length()-2, sizeof(szExData) - 1)); //-2 "/r/n"
		
		//BASE64����
		m_global.Base64Decode((LPBYTE)szExData, strlen(szExData), (LPBYTE)rptPack.szExData);
	}
	catch (...)
	{
		LOGF_ERROR("ResolveRptString  �����ַ���ʱ�쳣!", "FileRpt.txt");
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
		LOGF_ERROR("ResolveRptString  �����ַ���ʱ�쳣!", "FileRpt.txt");
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
			//����·�������ڣ��жϱ���·��
			if (!CAdapter::PathFileExists(m_szRptfilePathLocal))
			{
				return -1;
			}
			else
			{
				//������
				if (0 != GetWbsValidFile(nUid, szpUsrName, szFilePath, m_szRptfilePathLocal))
				{
					return -1;
				}
			}
		}
		else
		{
			//����·������
			if (0 != GetWbsValidFile(nUid, szpUsrName, szFilePath, m_szRptfilePathShare))
			{
				//�����Ƿ����
				if (!CAdapter::PathFileExists(m_szRptfilePathLocal))
				{
					return -1;
				}
				else
				{
					//������
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
		snprintf(s,sizeof(s),"ReadRptFile �쳣! nUid=%d \r\n", nUid);
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

		SafeLock slock(&m_lockRptQue);    //����
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
			
				//�γ��ļ��� dengyu 2017-6-13
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
		snprintf(s,sizeof(s),"GetPathFromMap �쳣!  nUid=%d  \r\n", nUid);
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
		
		SafeLock slock(&m_lockMoQue);    //����.
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
				
				//�γ��ļ���
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
		snprintf(s,sizeof(s),"GetPathFromMoMap �쳣!  nUid=%d  \r\n", nUid);
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
		snprintf(s,sizeof(s),"IsPathOk �쳣! szpath=%s \r\n", szpath);
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
		snprintf(s,sizeof(s),"GetValidFile �쳣! strFolder=%s; \r\n", strFolder);
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
// 				SafeLock slock(&m_lockRpt);    //����
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
				// 				SafeLock slock(&m_lockRpt);    //����
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
			//��¼����·��
			GetWbsFolder(szFolder, szFilePathShare, szPathBuf, 512);
		}
		else
		{
			//��������Ŀ¼
			bRet = CreateWbsFolder(szFolder, szFilePathShare, szPathBuf, 512);

			//��¼����·��
			GetWbsFolder(szFolder, szFilePathShare, szPathBuf, 512);
		}

		memset(szPathBuf, 0, sizeof(szPathBuf));
		sprintf(szPathBuf, "%s/", szFilePathLocal);
		if (!CAdapter::PathFileExists(szPathBuf))
		{
			//��¼����·��
			GetWbsFolder(szFolder, szFilePathLocal, szPathBuf, 512);
		}
		else
		{
			//��������Ŀ¼
			bRet = CreateWbsFolder(szFolder, szFilePathLocal, szPathBuf, 512);

			//��¼����·��
			GetWbsFolder(szFolder, szFilePathLocal, szPathBuf, 512);
		}

		return bRet;
	}
	catch (...)
	{
		char s[128] = { 0 };
		snprintf(s, sizeof(s), "CreateFolder �쳣! szFolder=%s; \r\n", szFolder);
		LOGF_ERROR(s, "FileRpt.txt");

		return FALSE;
	}
}

int CFileMoRpt::GetMoRptWaitInfo()
{
	try
	{
		// ��ȡMO������
		if (CAdapter::PathFileExists(m_szMofilePathShare))
		{
			ProcessDirectory(m_szMofilePathShare, m_szMofilePathShare,FLAG_MO);
		}
		else if (CAdapter::PathFileExists(m_szMofilePathLocal))
		{
			ProcessDirectory(m_szMofilePathLocal, m_szMofilePathLocal,FLAG_MO);
		}

		// ��ȡRPT������
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
		LOGF_ERROR("��ȡMO/RPT������ʱϵͳ�쳣!", "FileMoRpt.txt");
		return -1;
	}

	return 0;
}

// dengyu 2017-6-9
/**
 * ����ָ��Ŀ¼��ͳ��Ŀ¼�µ��ļ���������ݹ����
 */
void CFileMoRpt::ProcessDirectory(const std::string& dirPath, const std::string& dirSrcPath,int nFlag,int nDepth)
{
	DIR* dp = NULL;
	struct dirent* entry = NULL;
	struct stat statbuf;

    //zhubo 2017-12-08 ���������ļ����µ����ļ���
    if(nDepth >= 3)
    {
        return;
    }    
    //zhubo 2017-12-08 ���������ļ����µ����ļ���

	if (NULL == (dp = opendir(dirPath.c_str())))
	{
		char s[128] = { 0 };
		snprintf(s, sizeof(s), "��Ŀ¼�쳣! dirPath=%s; \r\n", dirPath.c_str());
		LOGF_ERROR(s, "FileMoRpt.txt");
		return;
	}
    string strTempDir = "";
	
	int nFileCountsMo = 0;
	while (NULL != (entry = readdir(dp)))
	{
		// ���˵�[.]��[..]Ŀ¼
		if (strncmp(entry->d_name, ".", 1) == 0 || strncmp(entry->d_name, "..", 2) == 0)
			continue;

        strTempDir = dirPath;
        strTempDir += "/";
        strTempDir += entry->d_name;

		// 4: ��ʾ�ļ��� 8: ��ʾ�ļ�
		//zhubo 2018-01-29
		//if (4 == entry->d_type)
		if(CAdapter::IsDir(strTempDir))
		{
			// ���ļ���ֱ�ӵݹ�
			ProcessDirectory(dirPath + '/' + entry->d_name, dirSrcPath,nFlag,++nDepth);
		}
        //zhubo 2018-01-29
		//else if (8 == entry->d_type)
		else if (CAdapter::IsRegularFile(strTempDir))
		{
			// ���ļ���ͳ������
			nFileCountsMo += CountFileLines(entry->d_name, 25);
		}
		else
		{
			continue;
		}
	}

	// ���˵�Ҫ�����ĸ�Ŀ¼
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

	// �������Ҫ,�ݹ����ʱ�õ�,����Ҫ��chdir(..)
	// chdir("..");
END:
    closedir(dp);
    dp = NULL;
}


int CFileMoRpt::CountFileLines(char *filename, int n)
{
	//���ļ����н�ȡ����"11111111111_n90.txt"
	int nCount = 0;

	try
	{
		char *p1 = NULL;
		char *p2 = NULL;
		char *p3 = NULL;               // ���ڶ����ļ�
		char *p4 = NULL;               // ����д���ļ�
		p3 = strstr(filename, "_bak");
		if (p3 != NULL)
		{
			//������ҵ�_bak��ʾ���ڶ�ȡ���ļ�
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
			//����Ҳ���'_n',��ʾ�����ļ�
			nCount = n;
		}
		else
		{
			//�����ļ����ʽ��ȡ����
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
