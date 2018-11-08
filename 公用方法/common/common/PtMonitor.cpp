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

//����ϵͳ��Ϣ�����Ϣ
int	CPtMonitor::MakeSysMonInfo(int nFlag)
{
	//���ϵͳ��Ϣ��XML��ʽ���ļ�
	CMonInfoWriter::GetInstance().WriteAppSysInfo(nFlag);

	return 0;
}

//ȡ�����־���д�С
int	CPtMonitor::GetMonLogQueSize()
{
	SafeLock safelock(&m_csMonLog);
	
	return m_MonLogQue.size();
}

//����ϵͳ��������Ϣ
int	CPtMonitor::MakeBufMonInfo(DEQUE_MO& BufMonQue)
{
	return 0;
}

//���ƽ̨��־����־��ض���
int	CPtMonitor::AddLogToMonQue(int nLogType, const char* szLog, int nMODCLS, const char* szWho, const char *szValue, bool bIsErrInfo)
{
	//�����ࣺ���ݿ��ࡢϵͳ��æ��
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
					//�ָ�����Ҫ�ظ���
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
					if (time(NULL) - tCommEventRecord->tLastMonTime < 10)//�ݶ�Ϊ10s
					{
						break;
					}
				}
				else
				{
					if (time(NULL) - tCommEventRecord->tLastMonTime < 3)//�ָ��澯���ʱ��С��3s���Ȳ��澯��
					{
						break;
					}
				}
			}

			time(&tCommEventRecord->tLastMonTime);
			tCommEventRecord->nLastErrType = nErrType;
		}

		//���ݿ��쳣��־����
		if (LOG_DB_OPT_FAILED == nLogType)
		{
			CMonInfoWriter::GetInstance().WriteAppDbErrInfo(bIsErrInfo, nMODCLS, szLog, szWho, szValue);
		}
		//ϵͳ�ڲ���һЩҵ���쳣����
		else if (LOG_SYS_BUSSINESS_ERROR == nLogType)
		{
			CMonInfoWriter::GetInstance().WriteAppOtherErrInfo(bIsErrInfo, nMODCLS, szLog, szWho, szValue);
		}
		//ϵͳ�ڲ��߳�ִ��/����ִ���쳣
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
	
	//���ݼ������ȷ����ѯ������
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
		//���������͵ļ�������Ƿ��ع����߼�ؼ��
		for (it; it != m_mapMonitorHis.end() && nCount > 0; ++it, --nCount)
		{
			//�Ƿ���ͬ�ļ����Ϣ(���ͼ�����)
			if (MonExceptionInfo == it->second)
			{
				//������ͬ�ٿ�Ƶ��
				if ((MonExceptionInfo.MonTime - it->second.MonTime) > m_nMonExceptionFreq)
				{
					//��ʱ�����ʱ�䡢����������в�����true
					it->second.MonTime = MonExceptionInfo.MonTime;
					m_MonitorTask.push_back(MonExceptionInfo);
					return true;			 
				}
				else
				{	
					//Ƶ��̫��
					return false;				
				}
			}
		}
		
		//û�м�ع�,���������ʷ
		m_mapMonitorHis.insert(std::make_pair(nKey, MonExceptionInfo));
		//����������
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

//�ڴ����ɴ�����Ϣ
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

//�����û��ʺż����Ϣ
int	CPtMonitor::MakeUserMonInfo(DEQUE_MO& UserMonQue)
{
	return 0;
}

int CPtMonitor::MakeUserMonInfo(/*in/out*/DEQUE_MO& UserMonQue, const int nUID, MON_EXCEPTION_INFO MonExceptionInfo)
{
	return 0;
}

//���ü��״̬
void CPtMonitor::SetMonStatus(bool bCycle, bool bOther)
{
	m_bCycle = bCycle;
	m_bOther = bOther;
}
