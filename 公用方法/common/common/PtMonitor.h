// PtMonitor.h: interface for the CPtMonitor class.
//
//////////////////////////////////////////////////////////////////////
/*
* 文件名称：PtMonitor.h
* 摘   要：平台监控类，实现日志监控、帐号监控、
*			通道监控、线程监控、系统信息监控等监控
* 当前版本：1.0
* 作   者：jianghb
* 完成日期：2011年11月01日
*
* 取代版本：1.0 
* 原作者 ：jianghb
* 完成日期：2011年11月01日
*/

#ifndef __PTMONITOR_H__
#define __PTMONITOR_H__

#include "MonInfoWriter.h"
//#include "DerivedLock.h"
//#include "SafeLock.h"
#include "wbs_config.h"
#include <list>

//MGATEWAY事件类型
enum 
{
	MON_EVENT_MGATEWAY_ALL_USER = 1500,
	MON_EVENT_MGATEWAY_USER_ONLINE = 1501,	     //用户在线情况
	MON_EVENT_MGATEWAY_USER_FEE = 1502,		     //接入用户费用情况	
	MON_EVENT_MGATEWAY_SAMPLE = 1508,		     //抽样事件	
	MON_EVENT_MGATEWAY_ALL_SPGATE = 1510,	     //通道账号周期信息2
	MON_EVENT_MGATEWAY_SPGATE_ONLINE = 1511,     //通道帐号在线情况
	MON_EVENT_MGATEWAY_SYS_BUFFER = 1520,		 //网关缓冲信息
	MON_EVENT_MGATEWAY_ALL_SPGATETWO = 1521,	 //通道周期信息2
};


//公共类事件:告警功能点分类
enum{
	COMEVENT_BUFINFO = 1001    //缓冲信息
};

//异常级别分段
#define MON_EVENT_TYPE_ERROR			10			//10 错误类
#define MON_EVENT_TYPE_ERROR_OK			19			//19 错误恢复
#define MON_EVENT_TYPE_WARN				20			//20 警告类
#define MON_EVENT_TYPE_WARN_OK			29			//29 警告恢复
#define MON_EVENT_TYPE_BASEINFO			30			//30 基本信息类型（实时批量送出的数据，比如：资源占有情况/程序启动等）
#define MON_EVENT_TYPE_SAMPLINGINFO		40			//40 抽样信息
#define MON_EVENT_TYPE_SAMPLINGPERIODIC	90			//90 周期性采集
#define MON_EVENT_TYPE_BUSSINESSINFO	99			//99 业务相关信息类（帐号相关信息/通道相关的信息等与业务相关的信息）公共类

struct CommEventRecord{
	int nMODCLS;//编号
	char nLastErrType;//10/19
	time_t tLastMonTime;//最后一次上报时间
	
	CommEventRecord()
	{
		nMODCLS = 0;
		nLastErrType = MON_EVENT_TYPE_ERROR_OK;
		tLastMonTime = 0;
	}
};

struct LastMonTime{
	time_t t_LastBufferMonTime; //缓冲周期信息
	time_t t_LastUserMonTime;   //用户周期信息
	time_t t_LastMonSampleTime; //抽样频率
	time_t t_LastMonSpgateTime; //通道账号周期信息
	time_t t_LastMonGateTime;   //通道周期信息
	time_t t_LastMonProcessTime;//进程周期信息
	
	LastMonTime()
	{
		t_LastBufferMonTime  = time(NULL); 
		t_LastUserMonTime    = time(NULL);   
		t_LastMonSampleTime  = time(NULL); 
		t_LastMonSpgateTime  = time(NULL); 
		t_LastMonGateTime    = time(NULL);   
		t_LastMonProcessTime = time(NULL);
	}
};

#define MAX_EVENT_MODCLS_BASE_NUM 10001   //公共事件编号起始位置
#define MAX_EVENT_MODCLS_NUM  10101  //最大公共事件ID.每次有新增的都必须改动这个值

class CPtMonitor
{
public:
    static CPtMonitor& GetInstance();//单件实例获取接口
    virtual ~CPtMonitor();
private:
    CPtMonitor();
	//CMonXmlMaker m_XmlMaker;
	
	//上层调用者指针
	void*			m_pInvokePtr;

	
	int		MakeSysMonInfo(int nFlag);

	//取监控日志队列大小
	int		GetMonLogQueSize();
	
	//生成系统缓冲监控信息
	int		MakeBufMonInfo(/*in/out*/DEQUE_MO& BufMonQue);

	//生成用户帐号监控信息
	int		MakeUserMonInfo(/*in/out*/DEQUE_MO& UserMonQue);
	//生成特定帐号特定类型的触发信息，如上线、下线、欠费，兼用监控V1.0和V2.0
	int		MakeUserMonInfo(/*in/out*/DEQUE_MO& UserMonQue, const int nUID, MON_EXCEPTION_INFO MonExceptionInfo);

	//该类型的监控信息是否需要再次发出通知	
	bool	CanNotifyMonitor(const int nKey, const MON_EXCEPTION_INFO& MonExceptionInfo);

	//把监控任务移到指定对象里面
	void	RemoveMonExceptionTask(std::list<MON_EXCEPTION_INFO>& vMonTask);

	bool m_bCycle;    //周期类
	bool m_bOther;    //其他类

public:

	//设置监控状态
	void SetMonStatus(bool bCycle, bool bOther);

	//监控V2.0，V1.0不在线时调用
	int		MakePtMonInfo(void* pInvokePtr, int nMasterOrSlave, int tMonFreqParams, int nFlag);
	//int		MakePtMonInfo(/*in*/void* pInvokePtr);

	//添加平台日志至日志监控队列
	int		AddLogToMonQue(/*in*/int nLogType, /*in*/const char* szLog, int nMODCLS, const char* szWho, const char *szValue, bool bIsErrInfo);

	//判断最后一次告警是不是错误告警
	bool	bLastCommonIsError(int nMODCLS);

	//公共事件告警
	int		CommonMonEvent(int nLogType, const char* szLog, int nMODCLS, const char* szWho, const char *szValue, bool bIsErrInfo);

	//功能：是否有待推送监控信息
	//返回值：false/true
	bool HasMonExceptionTask();

	//添加异常监控信息
	//[IN]MonExceptionInfo：新的监控信息
	//返回值：false=类型完全相同并且过频则忽略返回，true=成功添加并更新
	bool AddMonExceptionInfo(const MON_EXCEPTION_INFO& MonExceptionInfo);
	
	//把监控任务个信息组包放至MonQue	
	int	MakeExceptionMonInfo(DEQUE_MO& MonQue, /*in*/void* pInvokePtr);

private:
	DerivedLock		m_csMonLog;  //日志队列锁
	DerivedLock		m_csCommonLock; //公共事件锁
	std::multimap<int, MON_LOGINFO> m_MonLogQue; //监控日志队列
	CommEventRecord m_tCommEventRecord[MAX_EVENT_MODCLS_NUM-MAX_EVENT_MODCLS_BASE_NUM+1]; //公共事件

	int m_nMtRemained;  //记录所有用户的MT滞留量
	int m_nMoRemained;  //记录所有用户的MO滞留量
	int m_nRptRemained; //记录所有用户的RPT滞留量

	//异常信息监控历史，若为前端用户欠费异常或者后端用户运行异常则主键位UID，其他类型可定义为负值以区分
	std::multimap<int, MON_EXCEPTION_INFO> m_mapMonitorHis;
	//待推送的异常信息
	std::list<MON_EXCEPTION_INFO> m_MonitorTask;
	//异常信息数据结构所
	DerivedLock m_csMonExceptionInfo;
	//相同类型异常信息监控频率
	int m_nMonExceptionFreq;
};






#endif // !defined(AFX_PTMONITOR_H__8D87476A_F4DE_44C6_AAFF_649E4C02BB6F__INCLUDED_)
