// PtMonitor.cpp: implementation of the CPtMonitor class.
//
//////////////////////////////////////////////////////////////////////


#include "PtMonitor.h"
//#include "WbsMsgHandle.h"
//#include "StatsMgr.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPtMonitor& CPtMonitor::GetInstance()
{
    static CPtMonitor instance;	
    return instance;
}

CPtMonitor::CPtMonitor()
{
	m_nMtRemained = 0;
	m_nMoRemained = 0;
	m_nRptRemained = 0;
	m_bCycle = false;
	m_bOther = false;
}

CPtMonitor::~CPtMonitor()
{
	
}

int	CPtMonitor::MakePtMonInfo(void* pInvokePtr, int nMasterOrSlave, int tMonFreqParams, int nFlag)
{
	m_pInvokePtr = pInvokePtr;

	time_t t_CurTime = time(NULL);
	static LastMonTime tLastMonTime;

	if((tMonFreqParams >= MW_PARAM_MIN_MONFREQ && tMonFreqParams <= MW_PARAM_MAX_MONFREQ) 
		&& t_CurTime - tLastMonTime.t_LastMonProcessTime >= tMonFreqParams)
	{
		MakeSysMonInfo(nFlag);	
		time(&tLastMonTime.t_LastMonProcessTime);
	}
	return 0;
}

//生成系统信息监控信息
int	CPtMonitor::MakeSysMonInfo(int nFlag)
{
	//输出系统信息以XML格式到文件
	CMonInfoWriter::GetInstance().WriteAppSysInfo(nFlag);

	return 0;
}

//取监控日志队列大小
int	CPtMonitor::GetMonLogQueSize()
{
	SafeLock safelock(&m_csMonLog);
	
	return m_MonLogQue.size();
}

//生成系统缓冲监控信息
int	CPtMonitor::MakeBufMonInfo(DEQUE_MO& BufMonQue)
{
	return 0;
}

//添加平台日志至日志监控队列
int	CPtMonitor::AddLogToMonQue(int nLogType, const char* szLog, int nMODCLS, const char* szWho, const char *szValue, bool bIsErrInfo)
{
	//其他类：数据库类、系统繁忙类
	if (m_bOther && (LOG_DB_OPT_FAILED == nLogType || LOG_SYS_BUSSINESS_ERROR == nLogType))
	{
		return 0;
	}

	if (NULL == szLog || '\0' == szLog[0])
	{
		return 0;
	}

	SafeLock safelock(&m_csMonLog);
	CommonMonEvent(nLogType, szLog, nMODCLS, szWho, szValue, bIsErrInfo);

	return 0;
}

bool CPtMonitor::bLastCommonIsError(int nMODCLS)
{
	SafeLock safelock(&m_csCommonLock);

	if (nMODCLS < MAX_EVENT_MODCLS_BASE_NUM ||
		nMODCLS > MAX_EVENT_MODCLS_NUM)
	{
		return false;
	}

	return m_tCommEventRecord[nMODCLS-MAX_EVENT_MODCLS_BASE_NUM].nLastErrType == MON_EVENT_TYPE_ERROR;
}

int	CPtMonitor::CommonMonEvent(int nLogType, const char* szLog, int nMODCLS, const char* szWho, const char *szValue, bool bIsErrInfo)
{
	do 
	{
		if (nMODCLS < MAX_EVENT_MODCLS_BASE_NUM ||
			nMODCLS > MAX_EVENT_MODCLS_NUM)
		{
			break;
		}

		{
			SafeLock safelock(&m_csCommonLock);
		
			CommEventRecord *tCommEventRecord = &m_tCommEventRecord[nMODCLS-MAX_EVENT_MODCLS_BASE_NUM];
			if (!tCommEventRecord)
			{
				break;
			}
			
			int nErrType = bIsErrInfo?MON_EVENT_TYPE_ERROR:MON_EVENT_TYPE_ERROR_OK;
			
			if (tCommEventRecord->nLastErrType == MON_EVENT_TYPE_ERROR_OK)
			{
				if (nErrType == MON_EVENT_TYPE_ERROR_OK)
				{
					//恢复不需要重复报
					break;
				}
				else
				{
					if (time(NULL) - tCommEventRecord->tLastMonTime < 3)
					{
						break;
					}
				}
			}
			else
			{
				if (nErrType == MON_EVENT_TYPE_ERROR)
				{
					if (time(NULL) - tCommEventRecord->tLastMonTime < 10)//暂定为10s
					{
						break;
					}
				}
				else
				{
					if (time(NULL) - tCommEventRecord->tLastMonTime < 3)//恢复告警如果时间小于3s就先不告警。
					{
						break;
					}
				}
			}

			time(&tCommEventRecord->tLastMonTime);
			tCommEventRecord->nLastErrType = nErrType;
		}

		//数据库异常日志报警
		if (LOG_DB_OPT_FAILED == nLogType)
		{
			CMonInfoWriter::GetInstance().WriteAppDbErrInfo(bIsErrInfo, nMODCLS, szLog, szWho, szValue);
		}
		//系统内部的一些业务异常报警
		else if (LOG_SYS_BUSSINESS_ERROR == nLogType)
		{
			CMonInfoWriter::GetInstance().WriteAppOtherErrInfo(bIsErrInfo, nMODCLS, szLog, szWho, szValue);
		}
		//系统内部线程执行/函数执行异常
		else if (LOG_SYS_ERROR == nLogType)
		{
			CMonInfoWriter::GetInstance().WriteAppOtherErrInfo(bIsErrInfo, nMODCLS, szLog, szWho, szValue);
		}
		
	} while (false);

	return 0;
}

bool CPtMonitor::HasMonExceptionTask()
{
	SafeLock safeLock(&m_csMonExceptionInfo);
	return !m_MonitorTask.empty();
}

bool CPtMonitor::AddMonExceptionInfo(const MON_EXCEPTION_INFO& SysExceptionInfo)
{
	bool bRet = false;
	int nKey;
	
	//根据监控类型确定查询的主键
	switch(SysExceptionInfo.ExceptionType)
	{
	case MON_EXCEPTION_USERFEE_INDEBT:
		{
			nKey = SysExceptionInfo.UserFeeInfo.UID;
		}
		break;
	case MON_EXCEPTION_USERFEE_THRESHOLD:
		{
			nKey = SysExceptionInfo.UserFeeInfo.UID;
		}
		break;
	case MON_EXCEPTION_USER_OFFLINE:
		{
			nKey = SysExceptionInfo.UserOfflineInfo.UID;
			break;
		}
	default:
		return false;			
	}
	
	return CanNotifyMonitor(nKey, SysExceptionInfo);
}

bool CPtMonitor::CanNotifyMonitor(const int nKey, const MON_EXCEPTION_INFO& MonExceptionInfo)
{
	try
	{
		SafeLock safeLock(&m_csMonExceptionInfo);
		
		int nCount = m_mapMonitorHis.count(nKey);
		std::multimap<int, MON_EXCEPTION_INFO>::iterator it = m_mapMonitorHis.find(nKey);
		//遍历该类型的监控内容是否监控过或者监控间隔
		for (it; it != m_mapMonitorHis.end() && nCount > 0; ++it, --nCount)
		{
			//是否相同的监控信息(类型及内容)
			if (MonExceptionInfo == it->second)
			{
				//内容相同再看频率
				if ((MonExceptionInfo.MonTime - it->second.MonTime) > m_nMonExceptionFreq)
				{
					//超时则更新时间、加入任务队列并返回true
					it->second.MonTime = MonExceptionInfo.MonTime;
					m_MonitorTask.push_back(MonExceptionInfo);
					return true;			 
				}
				else
				{	
					//频率太快
					return false;				
				}
			}
		}
		
		//没有监控过,则加入监控历史
		m_mapMonitorHis.insert(std::make_pair(nKey, MonExceptionInfo));
		//加入监控任务
		m_MonitorTask.push_back(MonExceptionInfo);
		
		return true;
	}
	catch(...)
	{
		
	}
	return false;
}

void CPtMonitor::RemoveMonExceptionTask(std::list<MON_EXCEPTION_INFO>& MonTask)
{	
	SafeLock safeLock(&m_csMonExceptionInfo);
	m_MonitorTask.swap(MonTask);
}

//在此生成触发信息
int	CPtMonitor::MakeExceptionMonInfo(DEQUE_MO& MonQue, /*in*/void* pInvokePtr)
{
	m_pInvokePtr = pInvokePtr;
	std::list<MON_EXCEPTION_INFO> MonTask;
	RemoveMonExceptionTask(MonTask);
	
	for (std::list<MON_EXCEPTION_INFO>::iterator it=MonTask.begin(); it!=MonTask.end(); ++it)
	{
		MON_EXCEPTION_INFO &MonExceptionInfo = *it;
		
		switch(MonExceptionInfo.ExceptionType)
		{
		case  MON_EXCEPTION_USERFEE_INDEBT:
			MakeUserMonInfo(MonQue, MonExceptionInfo.UserFeeInfo.UID, MonExceptionInfo);
			break;
		case  MON_EXCEPTION_USERFEE_THRESHOLD:
			MakeUserMonInfo(MonQue, MonExceptionInfo.UserFeeInfo.UID, MonExceptionInfo);
			break;
		case  MON_EXCEPTION_USER_OFFLINE:
			MakeUserMonInfo(MonQue, MonExceptionInfo.UserOfflineInfo.UID, MonExceptionInfo);					
			break;
		default:
			break;
		}
	}		
	
	return 0;
}

//生成用户帐号监控信息
int	CPtMonitor::MakeUserMonInfo(DEQUE_MO& UserMonQue)
{
	return 0;
}

int CPtMonitor::MakeUserMonInfo(/*in/out*/DEQUE_MO& UserMonQue, const int nUID, MON_EXCEPTION_INFO MonExceptionInfo)
{
	return 0;
}

//设置监控状态
void CPtMonitor::SetMonStatus(bool bCycle, bool bOther)
{
	m_bCycle = bCycle;
	m_bOther = bOther;
}
