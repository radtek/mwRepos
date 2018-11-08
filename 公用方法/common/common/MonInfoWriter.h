// MonInfoWriter.h: interface for the CPtMonitor class.
//
//////////////////////////////////////////////////////////////////////
/*
* �ļ����ƣ�MonInfoWriter.h
* ժ   Ҫ��ƽ̨����࣬ʵ����־��ء��ʺż�ء�
*			ͨ����ء��̼߳�ء�ϵͳ��Ϣ��صȼ�ء���������Ϣ
* ��ǰ�汾��1.0
*/

#ifndef __MONINFOWRITER_H__
#define __MONINFOWRITER_H__

#include "TypeDef.h"
#include <stack>

//��̬�⼰�ӿ�������Ϣ
#ifdef _WIN64
#define MON_DLL_NAME	"MonWriter64.dll"
#elif defined WIN32
#define MON_DLL_NAME	"MonWriter.dll"
#elif defined LINUX
#define MON_DLL_NAME "libMonWriter64.so"
#endif
#define MON_DLL_IF_INIT "InitXMLMon"
#define MON_DLL_IF_WRITE "WriteXMLMon"
#define MON_DLL_IF_EXIT "ExitXmlMon"
#define MON_DLL_IF_WRITEV2 "WriteXMLMonV2"

typedef int (FAR_STDCALL *_InitMonitor)(const char* AppDir, const char* szVersion, int filtime, void* pv);
typedef bool (FAR_STDCALL *_ExitMonitor)();
typedef int (FAR_STDCALL *_WriteMonXml)(
										int eventType,      /*����Ρ��¼�����*/  
										int eventLevel,/*����Ρ��¼�����*/										
										const char* eventContent,     /* ����Ρ����ݲ�ͬ�¼����Ͷ��岻ͬ��xml��ǩ��ʽ��xml�����ʽΪGBK,�������ǩ��CONTENT��������Ҫ��ǩͷ (����)*/
										const char* eventWhere,         
										const char* eventWho,
										const char* eventOther
										);

//�����µı���Э���д��
typedef int (FAR_STDCALL *_WriteMonXmlV2)(
										  int nEventType,			 /*�¼�����*/
										  int nEventId,				 /*�¼�ID*/
										  const char* szEventTm,	 /*�¼�����ʱ��,��ʽ:YYYY-MM-DD HH:mm:SS.FFF*/
										  const char* szEventContent /*�¼�����(JSON��ʽ)*/
										 );

//д�����Ϣ��������̼�����
/*
1����ȡ����ֱ�ӵ��� CMonInfoWriter::GetInstance()
2����ʼ��: CMonInfoWriter::GetInstance.Init(...), Ϊʹ�������ӿڵ��Ⱦ�����
3����������Ϣ
3.1��Ӧ�������ɹ����ã�CMonInfoWriter::GetInstance.WriteAppStartOkInfo(...)
3.2��Ӧ������ʧ�ܵ��ã�CMonInfoWriter::GetInstance.WriteAppStartErInfo(...)
3.3��Ӧ�ñ��ر�ʱ���ã�CMonInfoWriter::GetInstance.WriteAppClosedInfo(...)
3.4��Ӧ�÷��������쳣(��ָ�)ʱ���ã�CMonInfoWriter::GetInstance.WriteAppNetErrInfo(...)
3.5��Ӧ�÷������ݿ��쳣(��ָ�)ʱ���ã�CMonInfoWriter::GetInstance.WriteAppDbErrInfo(...)
3.6��Ӧ�÷��������쳣(��ָ�)ʱ���ã�CMonInfoWriter::GetInstance.WriteAppOtherErrInfo(...)
3.7��Ӧ���߳���Ϣ������ã�CMonInfoWriter::GetInstance.WriteAppThreadInfo(...)
3.8��Ӧ��ϵͳ��Ϣ(CPU/MEM)�������(��Ҫ���������ض��̶߳�ʱ����)��CMonInfoWriter::GetInstance.WriteAppSysInfo(...)
3.9��Ӧ���Զ����¼����ʱ���ã�CMonInfoWriter::GetInstance.WriteAppOtherInfo(...)
4��������д��ʽ���ա��������ƽ̨������Ͷ���1.doc��
5��ͷ�ļ����ݹ���·������
6��DEBUGģʽ��Ԥ����_TEST_CLASS_��������DLL���в���
*/

enum MON_EVENT_TYPE
{
	MON_EVENT_APP_START_OK = 1300,
		MON_EVENT_APP_START_ER = 1301,
		MON_EVENT_APP_CLOSED = 1302,
		MON_EVENT_APP_SYS_INFO = 1303,
		MON_EVENT_APP_THREAD_INFO = 1304,
		MON_EVENT_APP_NET_ERR = 1305,
		MON_EVENT_APP_DB_ERR = 1306,
		MON_EVENT_APP_OTHER_ERR = 1307,		
		MON_EVENT_APP_COMMON_ERR = 6000    //�������¼�	
};

//�쳣����ֶ�
#define MON_EVENT_TYPE_ERROR			10			//10 ������
#define MON_EVENT_TYPE_ERROR_OK			19			//19 ����ָ�
#define MON_EVENT_TYPE_WARN				20			//20 ������
#define MON_EVENT_TYPE_WARN_OK			29			//29 ����ָ�
#define MON_EVENT_TYPE_BASEINFO			30			//30 ������Ϣ���ͣ�ʵʱ�����ͳ������ݣ����磺��Դռ�����/���������ȣ�
#define MON_EVENT_TYPE_BUSSINESSINFO	99			//99 ҵ�������Ϣ�ࣨ�ʺ������Ϣ/ͨ����ص���Ϣ����ҵ����ص���Ϣ��

//
class CMonInfoWriter
{
	//�ӿڿ⼰�����ӿ�
	int m_nLastDllErr;
	HINSTANCE m_hMonitorDll;
	_InitMonitor m_pInitMonitor;
	_ExitMonitor m_pExitMonitor;
	_WriteMonXml m_pWriteMonXml;
	_WriteMonXmlV2 m_pWriteMonXmlV2;


protected:

	//����
	CMonInfoWriter();

	std::string m_strAppPath;		//�Ѿ�ת��APP��ǰ·��
	std::string m_strAppVersion;	//�Ѿ�ת��APP�汾��Ϣ
	std::string m_strProVersion;	//�Ѿ�ת��APP�汾��Ϣ
	
	std::string GetCurrentFormatTime(BOOL bAddMilliscd=TRUE, int nFreqSW=0);

	//
	//�Զ�̬��ӿڵ��õķ�װ
	//
	bool WriteMonXml(		
		int eventLevel,         /*����Ρ��¼�����*/
		int eventType,    /*����Ρ��¼�����*/
		const char* eventWhere,         
		const char* eventWho,
		const char* eventOther,
		const char* eventContent     /* ����Ρ����ݲ�ͬ�¼����Ͷ��岻ͬ��xml��ǩ��ʽ��xml�����ʽΪGBK,�������ǩ��CONTENT��������Ҫ��ǩͷ (����)*/
		);

	bool WriteMonXmlV2(
		int nEventType, /*�¼�����*/
		int nEventId,   /*�¼�ID*/
		// 		const char* szEventTm,      /*�¼�����ʱ��,��ʽ:YYYY-MM-DD HH:mm:SS.FFF*/
		const char* szEventContent  /*�¼�����(JSON��ʽ)*/
		);
protected:
	int m_nMonFreq;

public:
	void SetFreq(int nMonFreq);

	//���ַ���ֵת��ΪXML��ʽ
	static void ConverToXmlValue(const char* pValue, std::string &XmlValue);
	static std::string& ConverToXmlValue(std::string &strValue);
	static std::string ConverToXmlValue(const char* pValue);


	//����Ψһʵ��ģʽ�����в������ɸýӿڷ��صĶ������
	static CMonInfoWriter& GetInstance();

	//����:�Ƴ���ʼ����ж��DLL
	virtual ~CMonInfoWriter();

	//��ʼ��:
	//[IN]const char* appVersion��Ӧ�ó���汾��
	//[IN]int nFiltime:	��ͬ�����¼��Ӽ�������������(��)
	//[IN]const char* Reserve: ����
	//REURN: ���ؿ��ַ���Ϊ�ɹ�������Ϊ��������
	std::string Init(const char* appVersion, const char* proVersion, int nFiltime, const char* Reserve);

	void  UnInit();
	
	//дӦ�������ɹ���Ϣ:	��Ӧ�������ɹ�֮�����
	//[IN]const char* stateInfo:	�����ɹ���Ϣ
	//REURN: true=�ɹ� false=ʧ��
	bool WriteAppStartOkInfo(const char* stateInfo);
	
	//дӦ������ʧ����Ϣ:	��Ӧ������ʧ��
	//[IN]const char* stateInfo:	����ʧ����Ϣ
	//REURN: true=�ɹ� false=ʧ��
	bool WriteAppStartErInfo(const char* stateInfo);

	//дӦ�ñ��ر�ʱ����Ϣ:	��Ӧ�ñ��ر�ʱ����
	//[IN]const char* stateInfo:	���ر�ʱ����Ϣ	
	//REURN: true=�ɹ� false=ʧ��
	bool WriteAppClosedInfo(const char* stateInfo);

	//дӦ�ó��������쳣������������������쳣��Ϣ�������ʧ��/����ʧ��/����ʧ�ܵȣ�
	//[IN]bool bIsErrInfo:	true=�쳣��Ϣ, false=�쳣�ָ���Ϣ
	//[IN]int nMODCLS:	MODCLS�澯���ܵ����
	//[IN]const char* Msg:	������д�쳣�ָ��������ַ���	
	//REURN: true=�ɹ� false=ʧ��
	bool WriteAppNetErrInfo(bool bIsErrInfo, int nMODCLS, const char* Msg, const char* szWho, const char *szValue);
	
	//WriteAppDbErrInfo:дӦ�ó������ݿ�����쳣������ڲ������ݿ������쳣��Ϣ��������ݿ�/д��/����ʧ�ܵȣ�
	//[IN]bool bIsErrInfo:	true=�쳣��Ϣ, false=�쳣�ָ���Ϣ
	//[IN]int nMODCLS:	MODCLS�澯���ܵ����
	//[IN]const char* Msg:	������д�쳣�ָ��������ַ���	
	//REURN: true=�ɹ� false=ʧ��
	bool WriteAppDbErrInfo(bool bIsErrInfo, int nMODCLS, const char* Msg, const char* szWho, const char *szValue);
	
	//WriteAppOtherErrInfo:дӦ�ó��������쳣��Ϣ,�粶����쳣
	//[IN]bool bIsErrInfo:	true=�쳣��Ϣ, false=�쳣�ָ���Ϣ
	//[IN]int nMODCLS:	MODCLS�澯���ܵ����
	//[IN]const char* Msg:	������д�쳣�ָ��������ַ���	
	//REURN: true=�ɹ� false=ʧ��
	bool WriteAppOtherErrInfo(bool bIsErrInfo, int nMODCLS, const char* Msg, const char* szWho, const char *szValue);
	
	//дӦ����ռ�õ�ϵͳ��Դ��Ϣ: ��ĳһ���̶߳�ʱ����
	//REURN: true=�ɹ� false=ʧ��
	bool WriteAppSysInfo(int nFlag);
	bool WriteAppSysInfo(const MON_SYSINFO &sysinfo, int nFlag);

	//дӦ�ó����߳���Ϣ������߳�ID���߳�����״̬���쳣���������һ���쳣ʱ�䣬��һ���߳����е�ʱ�䣬�߳���־��	
	//[IN]:	unsigned int ThreadId
	//[IN]:	const char* ThreadName
	//[IN]:	int Status
	//[IN]:	unsigned int ErrCnt
	//[IN]:	const char* LastErrTime
	//[IN]:	const char* LastRunTime
	//[IN]:	const char* ThreadLog);
	//REURN: true=�ɹ� false=ʧ��
	bool WriteAppThreadInfo(unsigned int ThreadId, const char* ThreadName, int Status, unsigned int ErrCnt,  const char* LastErrTime,  const char* LastRunTime, const char* ThreadLog);

	

	
	//WriteAppOtherInfo:дӦ�ó����Զ����¼����͵���Ϣ
	//[IN]int eventLevel:	�¼�����
	//[IN]int eventType		�¼�����
	//[IN]const char* xmlMsg	�¼�����,���սӿ�Լ����ʽ��ʽ����GBK�����XML�ַ���(����ǩ<CONTENT>�����ݣ�������<CONTENT>),���뱣֤XML�ؼ��ַ���ת��(����ConverToXmlValueת��)
	//REURN: true=�ɹ� false=ʧ��
	bool WriteAppOtherInfo(int eventLevel, int eventType, const char* xmlMsg);
	
	bool WriteAppOtherInfoV2(int eventType, int eventID , const char* xmlMsg);

	//ƴ��JSON��Ϣ	
	//[IN]:	string strContent �ܵ���Ϣ
	//[IN]:	const string strInsertContent Ҫ��������Ϣ
	//[IN]:	BOOL bLastInfo �Ƿ��Ǳ������������һ����Ϣ
	//[IN]:	BOOL bLastBracet  �Ƿ������һ�������
	static void AddJsonContext(std::string &strContent, const std::string strInsertContent, BOOL bLastInfo=FALSE, BOOL bLastBracet=TRUE);

	void GetWbsLocalTime(LPSYSTEMTIME_EX lpSystemTime_ex);

};

#endif // !defined(AFX_PTMONITOR_H__8D87476A_F4DE_44C6_AAFF_649E4C02BB6F__INCLUDED_)
