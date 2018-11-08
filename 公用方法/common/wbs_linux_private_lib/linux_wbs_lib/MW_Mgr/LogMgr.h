// LogMgr.h: interface for the CLogMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMGR_H__79EC757B_3D21_4BEF_BED2_33C4A1700FF6__INCLUDED_)
#define AFX_LOGMGR_H__79EC757B_3D21_4BEF_BED2_33C4A1700FF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TypeDef.h"
#include "adapter/CompatibleThread.h"
#include "adapter/CompatibleEvent.h"
//zhubo 2017-4-7 
//#include "shlwapi.h"
#include <algorithm>

#include "DerivedLock.h"
#include "MemList.h"
#include "MemListEx.h"
//zhubo 2017-4-7 
//#include "sys/stat.h"
#include "DoubleListEx.h"
#include "smartptr.h"

struct LOGFILE 
{
	char szLogName[MAX_PATH+1]; //��־�ļ���
	FILE *fp;				  //��־�ļ�ָ��
	LOGFILE()
	{
		memset(szLogName, 0, sizeof(szLogName));
		fp  = NULL;
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
		return *this;
	}
};
typedef std::map<std::string, LOGFILE> MAP_LOGFILE;

struct LOGS
{
	/*ref_ptr<char*>*/char* LogPtr;						//��־����
	//std::string strLog;								//��־����
	std::string strPreFix;								//��־����ǰ׺
	std::string	strPostFix;								//��־���ݺ�׺
	/*std::string*/ char szLogName[MAX_PATH];			//��־�ļ���
	/*std::string*/ char szNewLogName[MAX_PATH];		//����־�ļ���
	/*std::string*/ char szLogPath[MAX_PATH];			//����־·��
	/*std::string*/ char szNewLogPath[MAX_PATH];		//����־·��
	int nLogSize;										//������־���������
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
	static CLogMgr& GetInstance();//����ʵ����ȡ�ӿ�
	virtual ~CLogMgr();
public:
	//************************************
	// Description:����־������ȡһ������
	// Parameter: void
	// Returns:   CString һ����־
	//************************************
	string	GetInnerLog();

	//************************************
	// Description:����־�������Ƿ�Ϊ��
	// Parameter: void
	// Returns:   BOOL 0:�ǿ� 1:��
	//************************************
	BOOL	IsInnerLogEmpty();

	//ȡ��־����
	int		GetInnerLogCount();

	//************************************
	// Description:д��־
	// Parameter: [IN] CString szLog		: ��־����
	//			  [IN] CString szLogName	: ��־�ļ���
	// Returns:   void
	//************************************
	//void	WriteLog(CString szLog, CString szLogName = "log.txt");
	void	WriteLog(const char *szLog, const char *szLogName = "log.txt");

	//д��־������ʱ��
	void	WriteLogNoTime(const char *szLog, const char *szLogName = "log.txt");

	//************************************
	// Description:д��־�������ļ���Ҫ��
	// Parameter: [IN] const char *szSrc	: ��־����
	//			  [IN] int nLen				: ��־����
	//			  [IN] const char *szSrcAddr: �ĸ��û�����־
	//			  [IN] CString szLogName	: ��־����
	// Returns:   void
	//************************************
	void	WriteLogEx(/*ref_ptr<char*>*/const char* LogPtr, int nLen, const char *szSrcAddr, const char *szLogName = "logEx.txt");

	//************************************
	// Description:д��־(�ĸ�sockid��Ӧ���ĸ��û����������͵Ľ��յ��Ļ����������͵���־)
	// Parameter: [IN] const char *szSrc	: ��־����
	//			  [IN] int nLen				: ��־����
	//			  [IN] int nSockId			: �ĸ�SockId����־
	//			  [IN] int nCmdId			: �������͵���־
	//			  [IN] int nRecvSendFlag	: �ǽ��յĻ��Ƿ��͵���־
	//			  [IN] const char *szSourceAddr:�ĸ��û�����־
	//			  [IN] CString szLogName	: ��־����
	// Returns:   void
	//************************************
	void	WriteLogExEx(/*ref_ptr<char*>*/const char* LogPtr, 
								int nLen, 
								int nSockId, 
								int nCmdId, 
								int nRecvSendFlag, 
								const char *szSourceAddr, 
								const char *szLogName = "logExEx.txt");

	//��¼ԭʼ�����ֽڰ�
	void	WriteLogExExEx(/*ref_ptr<char*>*/const char* LogPtr, int nLogLen, const char *szLogName = "logExExEx.txt");

	//************************************
	// Description:��ӵ���־����
	// Parameter: [IN] CString szLog		: ��־����
	//			  [IN] CString szLogName	: ��־�ļ���
	// Returns:   void
	//************************************
	//void	AddInnerLog(CString szLog, CString szLogName = "log.txt");
	void	AddInnerLog(const char *szLog, const char *szLogName = "log.txt");

	//************************************
	// Description:Ϊָ����UID����ָ��nType��xml�ĵ�
	// Parameter: [IN] CString strXml		: �ļ�����
	//			  [IN] int nUID				: �û�UID
	//			  [IN] int nType			: ����(���У����У�״̬����)
	// Returns:   void
	//************************************
	void	WriteXml(const char *szXML, int nLen, int nUID, int nType);

	//************************************
	// Description:ɾ���ļ����ļ���
	// Parameter: [IN] CString strFilePath	: �ļ����ļ���·��
	// Returns:   BOOL						
	//************************************
	static BOOL	ShellDeleteFiles(string strFilePath);

	//************************************
	// Description:ö��ָ���ļ����ļ����ļ���
	// Parameter: [IN] CString strFolderName: ָ�����ļ���·��
	//			  [OUT]std::vector<CString>& vFileName:����ö�ٵ����ļ�����(ȫ·��)
	// Returns:   BOOL						
	//************************************
	static void	EnumDir(string strFolderName, std::vector<string>& vFileName);

	//************************************
	// Description:ö��ָ���ļ����ļ����ļ���(�������ھ��ʱ�䳬��nDay���)
	// Parameter: [IN] CString strFolderName: ָ�����ļ���·��
	//			  [IN] int nDay				: �������ھ��ʱ��(��)
	//			  [OUT]std::vector<CString>& vFileName:����ö�ٵ����ļ�����(ȫ·��)
	// Returns:   BOOL						
	//************************************
	static void	EnumDirEx(string strFolderName, int nDay, std::vector<string>& vFileName);

	//************************************
	// Description:ɾ���ļ��м��ļ�������������
	// Parameter: [OUT]std::vector<CString>& vFolderName:�ļ���ȫ·��
	// Returns:   void						
	//************************************
	static void	DelDir(std::vector<string>& vFolderName);

	//************************************
	// Description:ɾ����־
	// Parameter: void
	// Returns:   void						
	//************************************
	static void	DelLogs(int nLogSaveDays=7);

	//��θ�ʽ����־���
	void	WriteErrorLog(const char *szLogFmt, ...);

	//������־·��
	void	SetLogPath(const char *szLogPath);

	string GetLogPath() const;

	//���õ�����־�Ĵ�С��MΪ��λ
	void	SetLogSize(int nLogSize=10);

	//������־����
	void	SetLogLevel(UINT nLogLevel=0);

	//�����߳�ģʽ
	void	OpenThrMode(bool bThrMode);

	//��ȡ�߳���־�����С
	int		GetLogBufSize();

	//��ȡ��־�ļ�����
	int		GetLogFileNum();

	static void EnumFileEx(string strFolderName, int nDay, std::vector<string>& vFileName);

	//ȡ��һ����С���ڴ�ռ�
	ref_ptr<char*>	GetBuffer(UINT nSize);

	//��ȡMORPT����Ŀ¼
	static BOOL GetMoRptModule(char* szFileMoShare, char* szFileRptShare);
private:
	CLogMgr();
	int		RenameLog(const char *oldname, const char *newname);
	int		GetLogSize(const char *szLogName);
	
	//д��ͨ��־
	void	WriteLog0(const char *szLog, const char *szLogName = "log.txt");

	//���ʺ�д��־
	void	WriteLog1(/*ref_ptr<char*>*/const char* LogPtr, int nLen, const char *szSrcAddr, const char *szLogName = "logEx.txt");

	//��ĳ��һ�ʺŵ�ͨ�������
	void	WriteLog2(/*ref_ptr<char*>*/const char* LogPtr, 
								int nLen, 
								int nSockId, 
								int nCmdId, 
								int nRecvSendFlag, 
								const char *szSourceAddr, 
								const char *szLogName = "logExEx.txt");
	//д��ϸ������־��
	void	WriteLog3(/*ref_ptr<char*>*/const char* LogPtr, int nLogLen, const char *szLogName = "logExExEx.txt");

	//������־�ļ�����־�ļ�ָ��Ĵ򿪺͹ر�
	//��ӵ���־�ļ������в����Ӷ�Ӧ���ļ�ָ��
	FILE*	AddLogFile1(const char *szLogName);

	//������־�ļ��Ƿ�����ڶ����У��������򷵻���Чָ�룬�����ڷ���NULL
	FILE*	FindLogFile(const char *szLogName);

	//����־������ɾ������־�ļ������ر��ļ�ָ��
	void	DelLogFile(const char *szLogName);

	//�����־���У����ر��ļ�ָ��
	void	DelAllLogFile();

	//��ӵ���־����
	void	WriteLogFile(const char *szPreFix, /*ref_ptr<char*>*/const char* LogPtr, const char *szPostFix, int nLogLen, int nLogSize, const char *szLogName, const char *szNewLogName, const char *szLogPath, int nLogLevel=1);

	//
	void	WriteLogFileEx(const char *szLog, int nLogLen, int nLogSize, const char *szLogName, const char *szNewLogName, const char *szLogPath, int nLogLevel=1);

	//д��־�������ݴ�С������
	void	WriteLogFile(LOGS& log);

	//************************************
	// Description:�½�һ���ļ���
	// Parameter: [IN] const char *szDirName : �ļ�������
	// Returns:   BOOL						 0:ʧ�� 1:�ɹ�
	//************************************
	BOOL	CreateDir(const char *szDirName);

private:
	//�����־�߳�
	static THREAD_RETURN __STDCALL  ThreadDelLog(LPVOID pParam); 
	//д��־�߳�
	static unsigned int __STDCALL ThreadWriteLog(LPVOID pParam);
private:
	char m_szLogPath[MAX_PATH];
	DerivedLock	cs_xml;
	DerivedLock cs_logfile;
	CMemList<string>	m_innerLog;
	MAP_LOGFILE m_logfile;
	static int m_nLogSaveDays;	//��־��������
	int m_nLogSize;	//������־�ļ���С
	int m_nLargeLogSize;
	//LOG_LIST m_LogList;
	CMemList<LOGS> m_LogQue;

	
	//zhubo 2017-4-7
	//HANDLE m_hCanWrite;
	CCompatibleEvent  m_hCanWrite;
	BOOL m_bThrMode;		//�Ƿ������߳�ģʽ
	BOOL m_bThrStart;		//�߳�������־
	BOOL m_bClear;		
	UINT m_nLogLevel;
};

#endif // !defined(AFX_LOGMGR_H__79EC757B_3D21_4BEF_BED2_33C4A1700FF6__INCLUDED_)
