// FileMoRpt.h: interface for the CFileMoRpt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEMORPT_H__416B06C7_57B2_4B50_AC8C_09FF585E0B63__INCLUDED_)
#define AFX_FILEMORPT_H__416B06C7_57B2_4B50_AC8C_09FF585E0B63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>
#include <vector>
#include "wbs_config.h"
#include "TypeDef.h"
#include "Global.h"

#define MAX_MO_BUFLEN  2048        //Mo�ļ�һ���ĳ���
#define MAX_RPT_BUFLEN 1024        //RPT�ļ�һ���ĳ���
#define MAX_SEQ_LEN    48          //���к��ַ�������
#define MAX_FILE_PATH_LEN   521
const int g_nMaxSize = 1000000;      //�����������100��

typedef struct fileInfo
{
	INT64  nlFilePath;
	int    nUid;
	int    nFileCount;
	time_t timeFile;
	
	fileInfo()
	{
		nlFilePath      = 0;
		nUid            = 0;
		nFileCount      = 0;
		timeFile        = 0;
		//memset(this, 0, sizeof(fileInfo));
	}

	fileInfo& operator= (const fileInfo &other)
	{
		if (this == &other)
		{
			return *this;
		}

		this->nlFilePath  = other.nlFilePath;
		this->nUid        = other.nUid;
		this->nFileCount  = other.nFileCount;
		this->timeFile    = other.timeFile;
// 		memset(this, 0, sizeof(fileInfo));
// 		memcpy(this, &other, sizeof(fileInfo));
		return *this;
	}
}FILEINFO;
 
class CFileMoRpt  
{
public:
	CFileMoRpt();
	virtual ~CFileMoRpt();

	//��ʼ���ļ�·��
	void InitFilePath(char* szWbsLocalPath, char* szWbsSharePath, int nGateNO);

	int WriteRpttoFile(std::vector<PACK_DELIVER> &RptQue);      //��RPTд���ļ�
	int ReadRptFile(const int nUid, const char* szpUsrName, std::vector<PACK_DELIVER> &RptPackArr);

 	int WriteMotoFile(std::vector<PACK_DELIVER> &MoQue);        //��Moд���ļ�
 	int ReadMoFile(const int nUid, const char* szpUsrName, std::vector<PACK_DELIVER> &MoPackArr);

	int GetFileCount(const int nUid);

	int GetMoRptWaitInfo();

	//ͳ���ļ�����
	int CountFileLines(char *filename, int n);

	//************************************
	// Description:��ȡ����MO/RPT��Ч�ļ�
	// Parameter: [in]nUid�û�UID,[in]szpUsrName�û�userid,[out]szValidFile��Ч�ļ�,[in]szValidPath�����ļ�·��
	// Returns:   -1��ȡʧ��,0��ȡ�ɹ�
	// Author:	  sunzj
	//************************************
	int GetWbsValidFile(const int nUid, const char* szpUsrName, char* szValidFile, char* szValidPath);

public:
//	int GetSubFileCount(const int nUid, bool bType/*trueΪRpt,falseΪMo*/);       //���ÿ���û�������ļ�����
	
	void makeRptString(const PACK_DELIVER &RptPack, char *szbuffer);
	void makeMoString(const PACK_DELIVER &MoPack, char *szbuffer);

	BOOL ResolveRptString(const char *szRptbuf, const int nUid, PACK_DELIVER &rptPack);
	BOOL ResolveMoString(const char *szRptbuf, const int nUid, PACK_DELIVER &MoPack);

	void AddToMap(int key, const FILEINFO &rptInfo);
	void AddToMoMap(int key, const FILEINFO &MoInfo);
	
	void GetPathFromMap(int nUid, int &nCount,const char* szUsrFolder, char *szFilePath, INT64 &nlFileName);
	void GetPathFromMoMap(int nUid, int &nCount,const char* szUsrFolder, char *szFilePath, INT64 &nlFileName);

	int WriteRpt(std::vector<PACK_DELIVER> &RptQue, int nFileCount,const char *szFileName);
	int WriteMo(std::vector<PACK_DELIVER> &MoQue, int nFileCount,const char *szFileName);
	
	//�γ��ļ�����ȫ��Ψһ
	int MakeFileName(const char *szUsrFolder, char *szFileName, INT64 &nlFileName);
	//�����ļ�Ŀ¼
	BOOL CreateWbsFolder(char *szFolder, char* szFilePath, char* szPathBuf, int nLen);
	BOOL GetWbsFolder(char *szFolder, char* szFilePath, char* szPathBuf, int nLen);
	BOOL CreateFolder(char *szFolder, char *szFilePathShare, char *szFilePathLocal);
	BOOL IsPathOk(char *szpath);
	BOOL GetValidFile(const char *strFolder, char *strFilePath);
	//ʵ���ж���һ�ζ�һ��
	// char *ReadString(char *string, int n, FILE *stream);
	// int ReadString(char *string, int n, int fd);
	// dengyu 2017-6-9
	// ����RPT/MOĿ¼�����˻�ͳ������
	void ProcessDirectory(const std::string& dirPath, const std::string& dirSrcPath,int nFlag,int nDepth=1);
public:
	std::multimap<int, FILEINFO> m_mapRpt;
	std::multimap<int, FILEINFO> m_mapMo;

	DerivedLock  m_lockRpt;
	DerivedLock  m_lockRptQue;

	DerivedLock  m_lockMo;
	DerivedLock  m_lockMoQue;

	char m_szRptfilePathShare[521];
	char m_szMofilePathShare[521];
	char m_szRptfilePathLocal[521];
	char m_szMofilePathLocal[521];

	CGlobal	 m_global;

	int      m_iGateNO;
	
	char m_szRemoteName[MAX_PATH];
	char m_szUserName[MAX_PATH];
	char m_szPassword[MAX_PATH];

	int  m_nLocalPathFlag;   //�Ƿ��Ǳ���·��
};

#endif // !defined(AFX_FILEMORPT_H__416B06C7_57B2_4B50_AC8C_09FF585E0B63__INCLUDED_)
