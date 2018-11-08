// LogMgr.h: interface for the CLogMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMGR_H__79EC757B_3D21_4BEF_BED2_33C4A1700FF6__INCLUDED_)
#define AFX_LOGMGR_H__79EC757B_3D21_4BEF_BED2_33C4A1700FF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TypeDef.h"
#include <algorithm>
#include <wbs_config.h>

namespace CurrentThread
{

extern __thread int t_cachedTid;
extern __thread int t_tidStringLength;
extern __thread char t_tidString[32];

void cacheTid();
inline int tid()
{
	if (__builtin_expect(t_cachedTid == 0, 0))
	{
		cacheTid();
	}
	return t_cachedTid;
}

inline const char* tidString()
{
	return t_tidString;
}

inline int tidStringLength()
{
	return t_tidStringLength;
}

} // end CurrentThread

struct LOGFILE
{
	char szLogName[MAX_PATH + 1];	//��־�ļ���
	FILE *fp;						//��־�ļ�ָ��
	DerivedLock cs;					//��־�ļ���
	int nRefCnt;					//���ü������
	bool bNeedNextClose;			//�Ƿ���Ҫ�´ιرղ�ɾ��
	size_t nFileSize;				//�ļ���С
	LOGFILE()
	{
		memset(szLogName, 0, sizeof(szLogName));
		fp  = NULL;
		nRefCnt = 0;
		bNeedNextClose = false;
		nFileSize = 0;
	}
	
	void IncRefCnt() 
	{
		++nRefCnt;
	}

	void DecRefCnt()
	{
		--nRefCnt;
	}

	bool IsInUse() 
	{ 
		return 0 != nRefCnt; 
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
		nRefCnt = other.nRefCnt;
		return *this;
	}
};
typedef std::map<std::string, LOGFILE*> MAP_LOGFILE;

struct LOGS
{
	char* LogPtr;						//��־����
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
    typedef enum LogLevel
    {//ö��ֵԽ�󣬴�ӡ����Խ�ߡ��߼�����־���ӡ�����е��ڸ߼���ģ��ͼ������־��
        LOG_TRACE = 0,//0
        LOG_DEBUG,
        LOG_INFO,
        LOG_WARN,
        LOG_ERROR,
        LOG_FATAL,
        NUM_LOG_LEVELS
    } LogLevel;

	static LogLevel logLevel();
	static void SetLogLevel(LogLevel level);
	static void SetLogLevel(UINT nLevel);

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
	// Parameter: [IN] szLog		: ��־����
	//			  [IN] szLogName	: ��־�ļ���
	// Returns:   void
	//************************************
    void WriteLog(const char *szLog, const char *szLogName = "log.txt");

	void WriteLog(const char *szLog, 
				  const char *szLogName, 
				  LogLevel level, 
				  const char* fileName, 
				  int lineNum, 
				  const char* funcName);
			
	void WriteLog(const string& strLog, 
				  const char *szLogName, 
				  LogLevel level, 
				  const char* fileName, 
				  int lineNum, 
				  const char* funcName);
    static void CLogWrite(const char *szLogName, CLogMgr::LogLevel level, const char* fileName, int lineNum, const char* funcName, const char *fmt, ...);
	
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
	static void	DelLogs(int nLogSaveDays = 7);

	//��θ�ʽ����־���
	void	WriteErrorLog(const char *szLogFmt, ...);

	//������־·��
	void	SetLogPath(const char *szLogPath);

	string GetLogPath() const;

	//���õ�����־�Ĵ�С��MΪ��λ
	void	SetLogSize(int nLogSize = 10);

	//������־����
	//void	SetLogLevel(UINT nLogLevel = 0);

	//�����߳�ģʽ
	void	OpenThrMode(bool bThrMode);

	//��ȡ�߳���־�����С
	int		GetLogBufSize();

	//��ȡ��־�ļ�����
	int		GetLogFileNum();

	static void EnumFileEx(string strFolderName, int nDay, std::vector<string>& vFileName);

	//��ȡMORPT����Ŀ¼
	static BOOL GetMoRptModule(char* szFileMoShare, char* szFileRptShare);

	void SetCurrentProcessIndex(int nPIndex);

    int GetLogFolderPath(string& strFolderPath);
    int OverwriteFile(const char* szFolderAndFileName, const char* pLogStr);

private:
	CLogMgr();
	int		RenameLog(const char *oldname, const char *newname);
	int		GetLogSize(const char *szLogName);

	//д��ͨ��־
    void WriteLog0(const char *szLog, const char *szLogName);
	void WriteLog0(const char *szLog, 
				   const char *szLogName,
				   LogLevel level, 
				   const char* fileName, 
				   int lineNum, 
				   const char* funcName);

	//�����־���У����ر��ļ�ָ��
	void	DelAllLogFile();

	//��ӵ���־����
	void	WriteLogFile(const char *szPreFix, /*ref_ptr<char*>*/const char* LogPtr, const char *szPostFix, int nLogLen, int nLogSize, const char *szLogName, const char *szNewLogName, const char *szLogPath, int nLogLevel = 1);

	//************************************
	// Description:�½�һ���ļ���
	// Parameter: [IN] const char *szDirName : �ļ�������
	// Returns:   BOOL						 0:ʧ�� 1:�ɹ�
	//************************************
	BOOL	CreateDir(const char *szDirName);

private:
	//�����־�߳�
	static THREAD_RETURN __STDCALL  ThreadDelLog(LPVOID pParam);
private:
	char m_szLogPath[MAX_PATH];
	DerivedLock	cs_xml;
	DerivedLock cs_logfile;
	CMemList<string>	m_innerLog;
	MAP_LOGFILE m_logfile;
	static int m_nLogSaveDays;	//��־��������
	static pid_t g_nPid;
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
	// UINT m_nLogLevel;


	//zhubo 2017-05-26
	int m_nCurrentProcessIndex;
    DerivedLock		 m_lockOverwrite;//1

};

extern CLogMgr::LogLevel g_logLevel;

inline CLogMgr::LogLevel CLogMgr::logLevel()
{
	return g_logLevel;
}



//�����־�ȼ�Ϊ LOG_TRACE��LOGF_TRACE
//����ʱ�ļ�¼�ȼ�Ϊ LOG_LOG_DEBUG��LOGF_DEBUG
//��������־��ӡ: LOG_INFO / LOGF_INFO
//������־����� LOG_LOG_WARN / LOGF_LOG_WARN
//�������Ϣ�����LOG_ERROR / LOGF_ERROR��
//�߳��˳������˳���LOG_FATAL / LOGF_FATAL
//���߳���Ĭ�ϼ���Ϊ LOG_INFO

//�����־�ȼ�Ϊ LOG_TRACE��LOGF_TRACE
#define LOG_TRACE(x) if (CLogMgr::logLevel() <= CLogMgr::LOG_TRACE) \
	CLogMgr::GetInstance().WriteLog((x), "log.txt", CLogMgr::LOG_TRACE, __FILE__, __LINE__, __func__)

//����ʱ�ļ�¼�ȼ�Ϊ LOG_DEBUG��LOGF_DEBUG
#define LOG_DEBUG(x) if (CLogMgr::logLevel() <= CLogMgr::LOG_DEBUG) \
	CLogMgr::GetInstance().WriteLog((x), "log.txt", CLogMgr::LOG_DEBUG, __FILE__, __LINE__, __func__)

//��������־��ӡ: LOG_INFO / LOGF_INFO
#define LOG_INFO(x) if (CLogMgr::logLevel() <= CLogMgr::LOG_INFO) \
	CLogMgr::GetInstance().WriteLog((x), "log.txt", CLogMgr::LOG_INFO, __FILE__, __LINE__, __func__)

//������־����� LOG_LOG_WARN / LOGF_WARN
#define LOG_WARN(x) CLogMgr::GetInstance().WriteLog((x), "log.txt", CLogMgr::LOG_WARN, __FILE__, __LINE__, __func__)

//�������Ϣ�����LOG_ERROR / LOGF_ERROR��
#define LOG_ERROR(x) CLogMgr::GetInstance().WriteLog((x), "log.txt", CLogMgr::LOG_ERROR, __FILE__, __LINE__, __func__)

//�߳��˳������˳���LOG_FATAL / LOGF_FATAL
#define LOG_FATAL(x) CLogMgr::GetInstance().WriteLog((x), "log.txt", CLogMgr::LOG_FATAL, __FILE__, __LINE__, __func__)


//�꺯�����Ӹ�F��ָ�꺯���ĵڶ�������Ϊָ���ļ�����ӡ��������Ĭ�ϵ�log.txt
#define LOGF_TRACE(x, fileName) if (CLogMgr::logLevel() <= CLogMgr::LOG_TRACE) \
	CLogMgr::GetInstance().WriteLog((x), (fileName), CLogMgr::LOG_TRACE, __FILE__, __LINE__, __func__)
#define LOGF_DEBUG(x, fileName) if (CLogMgr::logLevel() <= CLogMgr::LOG_DEBUG) \
	CLogMgr::GetInstance().WriteLog((x), (fileName), CLogMgr::LOG_DEBUG, __FILE__, __LINE__, __func__)
#define LOGF_INFO(x, fileName) if (CLogMgr::logLevel() <= CLogMgr::LOG_INFO) \
	CLogMgr::GetInstance().WriteLog((x), (fileName), CLogMgr::LOG_INFO, __FILE__, __LINE__, __func__)
#define LOGF_WARN(x, fileName) CLogMgr::GetInstance().WriteLog((x), (fileName), CLogMgr::LOG_WARN, __FILE__, __LINE__, __func__)
#define LOGF_ERROR(x, fileName) CLogMgr::GetInstance().WriteLog((x), (fileName), CLogMgr::LOG_ERROR, __FILE__, __LINE__, __func__)
#define LOGF_FATAL(x, fileName) CLogMgr::GetInstance().WriteLog((x), (fileName), CLogMgr::LOG_FATAL, __FILE__, __LINE__, __func__)


//����ʱ�ļ�¼�ȼ�Ϊ LOG_LOG_DEBUG��LOGF_DEBUG
#define CLOG_DEBUG( fmt , ...)                      CLogMgr::CLogWrite("log.txt", CLogMgr::LOG_DEBUG, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__ )
//��������־��ӡ: LOG_INFO / LOGF_INFO
#define CLOG_INFO( fmt , ...)                       CLogMgr::CLogWrite("log.txt", CLogMgr::LOG_INFO, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__ )
//�������Ϣ�����LOG_ERROR / LOGF_ERROR��
#define CLOG_ERROR( fmt , ...)                      CLogMgr::CLogWrite("log.txt", CLogMgr::LOG_ERROR, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__ )
//�꺯�����Ӹ�F��ָ�꺯���ĵڶ�������Ϊָ���ļ�����ӡ��������Ĭ�ϵ�log.txt
#define CLOGF_DEBUG(fileName , fmt , ... )         CLogMgr::CLogWrite((fileName), CLogMgr::LOG_DEBUG, __FILE__, __LINE__, __func__, fmt ,##__VA_ARGS__)
#define CLOGF_INFO(fileName , fmt , ... )          CLogMgr::CLogWrite((fileName), CLogMgr::LOG_INFO, __FILE__, __LINE__, __func__, fmt ,##__VA_ARGS__)
#define CLOGF_ERROR(fileName , fmt , ... )         CLogMgr::CLogWrite((fileName), CLogMgr::LOG_ERROR, __FILE__, __LINE__, __func__, fmt ,##__VA_ARGS__)



#endif // !defined(AFX_LOGMGR_H__79EC757B_3D21_4BEF_BED2_33C4A1700FF6__INCLU
