// LogMgr.h: interface for the CLogMgrBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMGR_H__5DBCBB13_CC66_4EC7_BE14_95CF95953475__INCLUDED_)
#define AFX_LOGMGR_H__5DBCBB13_CC66_4EC7_BE14_95CF95953475__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>

#ifdef DLIB_PROJECT
#ifndef DLIB_API
#define DLIB_API __declspec(dllexport)
#endif
#else
#ifndef NOUSE_DLLEXPORT
#ifndef DLIB_API
#define DLIB_API __declspec(dllimport)
#endif
#else
#ifndef DLIB_API
#define DLIB_API
#endif 
#endif
#endif

class DLIB_API CLogMgrBase  
{
public:
	CLogMgrBase();
	virtual ~CLogMgrBase();

public:
	//************************************
	// Description:����־������ȡһ������
	// Parameter: void
	// Returns:   std::string һ����־
	//************************************
	virtual std::string	GetInnerLog()=0;

	//************************************
	// Description:����־�������Ƿ�Ϊ��
	// Parameter: void
	// Returns:   BOOL 0:�ǿ� 1:��
	//************************************
	virtual bool IsInnerLogEmpty()=0;

	//ȡ��־����
	virtual int	GetInnerLogCount()=0;

	//************************************
	// Description:д��־
	// Parameter: [IN] const char *szLog		: ��־����
	//			  [IN] const char *szLogName	: ��־�ļ���
	// Returns:   void
	//************************************
	virtual void	WriteLog(const char *szLog, const char *szLogName = "log.txt")=0;

	//************************************
	// Description:д��־�������ļ���Ҫ��
	// Parameter: [IN] const char *szSrc	: ��־����
	//			  [IN] int nLen				: ��־����
	//			  [IN] const char *szSrcAddr: �ĸ��û�����־
	//			  [IN] const char *szLogName	: ��־����
	// Returns:   void
	//************************************
	virtual void	WriteLogEx(/*ref_ptr<char*>*/const char* LogPtr, int nLogLen, const char *szSrcAddr, 
								const char *szLogName = "logEx.txt")=0;

	//************************************
	// Description:д��־(�ĸ�sockid��Ӧ���ĸ��û����������͵Ľ��յ��Ļ����������͵���־)
	// Parameter: [IN] const char *szSrc	: ��־����
	//			  [IN] int nLen				: ��־����
	//			  [IN] int nSockId			: �ĸ�SockId����־
	//			  [IN] int nCmdId			: �������͵���־
	//			  [IN] int nRecvSendFlag	: �ǽ��յĻ��Ƿ��͵���־
	//			  [IN] const char *szSourceAddr:�ĸ��û�����־
	//			  [IN] const char *szLogName	: ��־����
	// Returns:   void
	//************************************
	virtual void	WriteLogExEx(/*ref_ptr<char*>*/const char* LogPtr, 
								int nLogLen, 
								int nSockId, 
								int nCmdId, 
								int nRecvSendFlag, 
								const char *szSourceAddr, 
								const char *szLogName = "logExEx.txt")=0;

	//��¼ԭʼ�����ֽڰ�
	virtual void	WriteLogExExEx(/*ref_ptr<char*>*/const char* LogPtr, int nLogLen, int nSockId, 
									int nRecvSendFlag, const char *szLogName="logExExEx.txt")=0;

	//************************************
	// Description:��ӵ���־����
	// Parameter: [IN] const char *szLog		: ��־����
	//			  [IN] const char *szLogName	: ��־�ļ���
	// Returns:   void
	//************************************
	virtual void	AddInnerLog(const char *szLog, const char *szLogName = "log.txt")=0;

	//������־·��
	virtual void	SetLogPath(const char *szLogPath)=0;

	virtual const char* GetLogPath() const=0;

	//���õ�����־�Ĵ�С��MΪ��λ
	virtual void	SetLogSize(int nLogSize=10)=0;

	//������־����
	virtual void	SetLogLevel(int nLogLevel=0)=0;

	//�����߳�ģʽ
	virtual void	OpenThrMode(bool bThrMode)=0;

	//��ȡ�߳���־�����С
	virtual int		GetLogBufSize()=0;

	//��ȡ��־�ļ�����
	virtual int		GetLogFileNum()=0;

	//ɾ��N��ǰ����־
	virtual void	DelLogs(int nLogSaveDays=7)=0;
};

/************************************************************************/
/* ȡ����������                                                         */
/************************************************************************/
class DLIB_API CLogMgr
{
public:
	static CLogMgrBase& GetInstance();
};


#endif // !defined(AFX_LOGMG_H__5DBCBB13_CC66_4EC7_BE14_95CF95953475__INCLUDED_)
