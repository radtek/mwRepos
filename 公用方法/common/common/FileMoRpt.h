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

#define MAX_MO_BUFLEN  2048        //Mo文件一条的长度
#define MAX_RPT_BUFLEN 1024        //RPT文件一条的长度
#define MAX_SEQ_LEN    48          //序列号字符串长度
#define MAX_FILE_PATH_LEN   521
const int g_nMaxSize = 1000000;      //两个队列最大100万

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

	//初始化文件路径
	void InitFilePath(char* szWbsLocalPath, char* szWbsSharePath, int nGateNO);

	int WriteRpttoFile(std::vector<PACK_DELIVER> &RptQue);      //将RPT写入文件
	int ReadRptFile(const int nUid, const char* szpUsrName, std::vector<PACK_DELIVER> &RptPackArr);

 	int WriteMotoFile(std::vector<PACK_DELIVER> &MoQue);        //将Mo写入文件
 	int ReadMoFile(const int nUid, const char* szpUsrName, std::vector<PACK_DELIVER> &MoPackArr);

	int GetFileCount(const int nUid);

	int GetMoRptWaitInfo();

	//统计文件行数
	int CountFileLines(char *filename, int n);

	//************************************
	// Description:获取滞留MO/RPT有效文件
	// Parameter: [in]nUid用户UID,[in]szpUsrName用户userid,[out]szValidFile有效文件,[in]szValidPath滞留文件路径
	// Returns:   -1获取失败,0获取成功
	// Author:	  sunzj
	//************************************
	int GetWbsValidFile(const int nUid, const char* szpUsrName, char* szValidFile, char* szValidPath);

public:
//	int GetSubFileCount(const int nUid, bool bType/*true为Rpt,false为Mo*/);       //获得每个用户下面的文件个数
	
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
	
	//形成文件名，全局唯一
	int MakeFileName(const char *szUsrFolder, char *szFileName, INT64 &nlFileName);
	//创建文件目录
	BOOL CreateWbsFolder(char *szFolder, char* szFilePath, char* szPathBuf, int nLen);
	BOOL GetWbsFolder(char *szFolder, char* szFilePath, char* szPathBuf, int nLen);
	BOOL CreateFolder(char *szFolder, char *szFilePathShare, char *szFilePathLocal);
	BOOL IsPathOk(char *szpath);
	BOOL GetValidFile(const char *strFolder, char *strFilePath);
	//实现行读，一次读一行
	// char *ReadString(char *string, int n, FILE *stream);
	// int ReadString(char *string, int n, int fd);
	// dengyu 2017-6-9
	// 遍历RPT/MO目录，按账户统计行数
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

	int  m_nLocalPathFlag;   //是否是本地路径
};

#endif // !defined(AFX_FILEMORPT_H__416B06C7_57B2_4B50_AC8C_09FF585E0B63__INCLUDED_)
