// MonInfoWriter.h: interface for the CPtMonitor class.
//
//////////////////////////////////////////////////////////////////////
/*
* 文件名称：MonInfoWriter.h
* 摘   要：平台监控类，实现日志监控、帐号监控、
*			通道监控、线程监控、系统信息监控等监控、输出监控信息
* 当前版本：1.0
*/

#ifndef __MONINFOWRITER_H__
#define __MONINFOWRITER_H__

#include "TypeDef.h"
#include <stack>

//动态库及接口命名信息
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
										int eventType,      /*【入参】事件类型*/  
										int eventLevel,/*【入参】事件级别*/										
										const char* eventContent,     /* 【入参】根据不同事件类型定义不同的xml标签格式，xml编码格式为GBK,不需带标签“CONTENT”，不需要标签头 (必填)*/
										const char* eventWhere,         
										const char* eventWho,
										const char* eventOther
										);

//根据新的报文协议编写的
typedef int (FAR_STDCALL *_WriteMonXmlV2)(
										  int nEventType,			 /*事件类型*/
										  int nEventId,				 /*事件ID*/
										  const char* szEventTm,	 /*事件产生时间,格式:YYYY-MM-DD HH:mm:SS.FFF*/
										  const char* szEventContent /*事件内容(JSON格式)*/
										 );

//写监控信息类调用流程及方法
/*
1、获取对象：直接调用 CMonInfoWriter::GetInstance()
2、初始化: CMonInfoWriter::GetInstance.Init(...), 为使用其他接口的先决条件
3、输出监控信息
3.1、应用启动成功调用：CMonInfoWriter::GetInstance.WriteAppStartOkInfo(...)
3.2、应用启动失败调用：CMonInfoWriter::GetInstance.WriteAppStartErInfo(...)
3.3、应用被关闭时调用：CMonInfoWriter::GetInstance.WriteAppClosedInfo(...)
3.4、应用发生网络异常(或恢复)时调用：CMonInfoWriter::GetInstance.WriteAppNetErrInfo(...)
3.5、应用发生数据库异常(或恢复)时调用：CMonInfoWriter::GetInstance.WriteAppDbErrInfo(...)
3.6、应用发生其他异常(或恢复)时调用：CMonInfoWriter::GetInstance.WriteAppOtherErrInfo(...)
3.7、应用线程信息输出调用：CMonInfoWriter::GetInstance.WriteAppThreadInfo(...)
3.8、应用系统信息(CPU/MEM)输出调用(需要调用者在特定线程定时调用)：CMonInfoWriter::GetInstance.WriteAppSysInfo(...)
3.9、应用自定义事件输出时调用：CMonInfoWriter::GetInstance.WriteAppOtherInfo(...)
4、参数填写格式参照《梦网监控平台相关类型定义1.doc》
5、头文件根据工程路径调整
6、DEBUG模式下预定义_TEST_CLASS_可以脱离DLL进行测试
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
		MON_EVENT_APP_COMMON_ERR = 6000    //公共类事件	
};

//异常级别分段
#define MON_EVENT_TYPE_ERROR			10			//10 错误类
#define MON_EVENT_TYPE_ERROR_OK			19			//19 错误恢复
#define MON_EVENT_TYPE_WARN				20			//20 警告类
#define MON_EVENT_TYPE_WARN_OK			29			//29 警告恢复
#define MON_EVENT_TYPE_BASEINFO			30			//30 基本信息类型（实时批量送出的数据，比如：资源占有情况/程序启动等）
#define MON_EVENT_TYPE_BUSSINESSINFO	99			//99 业务相关信息类（帐号相关信息/通道相关的信息等与业务相关的信息）

//
class CMonInfoWriter
{
	//接口库及导出接口
	int m_nLastDllErr;
	HINSTANCE m_hMonitorDll;
	_InitMonitor m_pInitMonitor;
	_ExitMonitor m_pExitMonitor;
	_WriteMonXml m_pWriteMonXml;
	_WriteMonXmlV2 m_pWriteMonXmlV2;


protected:

	//构造
	CMonInfoWriter();

	std::string m_strAppPath;		//已经转义APP当前路径
	std::string m_strAppVersion;	//已经转义APP版本信息
	std::string m_strProVersion;	//已经转义APP版本信息
	
	std::string GetCurrentFormatTime(BOOL bAddMilliscd=TRUE, int nFreqSW=0);

	//
	//对动态库接口调用的封装
	//
	bool WriteMonXml(		
		int eventLevel,         /*【入参】事件级别*/
		int eventType,    /*【入参】事件类型*/
		const char* eventWhere,         
		const char* eventWho,
		const char* eventOther,
		const char* eventContent     /* 【入参】根据不同事件类型定义不同的xml标签格式，xml编码格式为GBK,不需带标签“CONTENT”，不需要标签头 (必填)*/
		);

	bool WriteMonXmlV2(
		int nEventType, /*事件类型*/
		int nEventId,   /*事件ID*/
		// 		const char* szEventTm,      /*事件产生时间,格式:YYYY-MM-DD HH:mm:SS.FFF*/
		const char* szEventContent  /*事件内容(JSON格式)*/
		);
protected:
	int m_nMonFreq;

public:
	void SetFreq(int nMonFreq);

	//把字符串值转换为XML格式
	static void ConverToXmlValue(const char* pValue, std::string &XmlValue);
	static std::string& ConverToXmlValue(std::string &strValue);
	static std::string ConverToXmlValue(const char* pValue);


	//采用唯一实例模式，所有操作均由该接口返回的对象承载
	static CMonInfoWriter& GetInstance();

	//析构:推出初始化并卸载DLL
	virtual ~CMonInfoWriter();

	//初始化:
	//[IN]const char* appVersion：应用程序版本号
	//[IN]int nFiltime:	相同类型事件接及内容屡重周期(秒)
	//[IN]const char* Reserve: 保留
	//REURN: 返回空字符串为成功，否则为错误描述
	std::string Init(const char* appVersion, const char* proVersion, int nFiltime, const char* Reserve);

	void  UnInit();
	
	//写应用启动成功信息:	在应用启动成功之后调用
	//[IN]const char* stateInfo:	启动成功信息
	//REURN: true=成功 false=失败
	bool WriteAppStartOkInfo(const char* stateInfo);
	
	//写应用启动失败信息:	在应用启动失败
	//[IN]const char* stateInfo:	启动失败信息
	//REURN: true=成功 false=失败
	bool WriteAppStartErInfo(const char* stateInfo);

	//写应用被关闭时的信息:	在应用被关闭时调用
	//[IN]const char* stateInfo:	被关闭时的信息	
	//REURN: true=成功 false=失败
	bool WriteAppClosedInfo(const char* stateInfo);

	//写应用程序网络异常（输出由网络层产生的异常信息，如监听失败/发送失败/接收失败等）
	//[IN]bool bIsErrInfo:	true=异常信息, false=异常恢复信息
	//[IN]int nMODCLS:	MODCLS告警功能点分类
	//[IN]const char* Msg:	这里填写异常恢复描述的字符串	
	//REURN: true=成功 false=失败
	bool WriteAppNetErrInfo(bool bIsErrInfo, int nMODCLS, const char* Msg, const char* szWho, const char *szValue);
	
	//WriteAppDbErrInfo:写应用程序数据库操作异常（输出在操作数据库层面的异常信息，如打开数据库/写库/读库失败等）
	//[IN]bool bIsErrInfo:	true=异常信息, false=异常恢复信息
	//[IN]int nMODCLS:	MODCLS告警功能点分类
	//[IN]const char* Msg:	这里填写异常恢复描述的字符串	
	//REURN: true=成功 false=失败
	bool WriteAppDbErrInfo(bool bIsErrInfo, int nMODCLS, const char* Msg, const char* szWho, const char *szValue);
	
	//WriteAppOtherErrInfo:写应用程序其他异常信息,如捕获的异常
	//[IN]bool bIsErrInfo:	true=异常信息, false=异常恢复信息
	//[IN]int nMODCLS:	MODCLS告警功能点分类
	//[IN]const char* Msg:	这里填写异常恢复描述的字符串	
	//REURN: true=成功 false=失败
	bool WriteAppOtherErrInfo(bool bIsErrInfo, int nMODCLS, const char* Msg, const char* szWho, const char *szValue);
	
	//写应用所占用的系统资源信息: 由某一个线程定时调用
	//REURN: true=成功 false=失败
	bool WriteAppSysInfo(int nFlag);
	bool WriteAppSysInfo(const MON_SYSINFO &sysinfo, int nFlag);

	//写应用程序线程信息（输出线程ID，线程名，状态，异常次数，最后一次异常时间，上一次线程运行的时间，线程日志）	
	//[IN]:	unsigned int ThreadId
	//[IN]:	const char* ThreadName
	//[IN]:	int Status
	//[IN]:	unsigned int ErrCnt
	//[IN]:	const char* LastErrTime
	//[IN]:	const char* LastRunTime
	//[IN]:	const char* ThreadLog);
	//REURN: true=成功 false=失败
	bool WriteAppThreadInfo(unsigned int ThreadId, const char* ThreadName, int Status, unsigned int ErrCnt,  const char* LastErrTime,  const char* LastRunTime, const char* ThreadLog);

	

	
	//WriteAppOtherInfo:写应用程序自定义事件类型的信息
	//[IN]int eventLevel:	事件级别
	//[IN]int eventType		事件类型
	//[IN]const char* xmlMsg	事件内容,按照接口约定格式格式化的GBK编码的XML字符流(即标签<CONTENT>的内容，但不含<CONTENT>),必须保证XML关键字符被转义(调用ConverToXmlValue转换)
	//REURN: true=成功 false=失败
	bool WriteAppOtherInfo(int eventLevel, int eventType, const char* xmlMsg);
	
	bool WriteAppOtherInfoV2(int eventType, int eventID , const char* xmlMsg);

	//拼接JSON信息	
	//[IN]:	string strContent 总的信息
	//[IN]:	const string strInsertContent 要新增的信息
	//[IN]:	BOOL bLastInfo 是否是本大括号内最后一条信息
	//[IN]:	BOOL bLastBracet  是否是最后一组大括号
	static void AddJsonContext(std::string &strContent, const std::string strInsertContent, BOOL bLastInfo=FALSE, BOOL bLastBracet=TRUE);

	void GetWbsLocalTime(LPSYSTEMTIME_EX lpSystemTime_ex);

};

#endif // !defined(AFX_PTMONITOR_H__8D87476A_F4DE_44C6_AAFF_649E4C02BB6F__INCLUDED_)
