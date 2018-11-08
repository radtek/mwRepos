// PtMonitor.h: interface for the CPtMonitor class.
//
//////////////////////////////////////////////////////////////////////
/*
* �ļ����ƣ�PtMonitor.h
* ժ   Ҫ��ƽ̨����࣬ʵ����־��ء��ʺż�ء�
*			ͨ����ء��̼߳�ء�ϵͳ��Ϣ��صȼ��
* ��ǰ�汾��1.0
* ��   �ߣ�jianghb
* ������ڣ�2011��11��01��
*
* ȡ���汾��1.0 
* ԭ���� ��jianghb
* ������ڣ�2011��11��01��
*/

#ifndef __PTMONITOR_H__
#define __PTMONITOR_H__

#include "MonInfoWriter.h"
//#include "DerivedLock.h"
//#include "SafeLock.h"
#include "wbs_config.h"
#include <list>

//MGATEWAY�¼�����
enum 
{
	MON_EVENT_MGATEWAY_ALL_USER = 1500,
	MON_EVENT_MGATEWAY_USER_ONLINE = 1501,	     //�û��������
	MON_EVENT_MGATEWAY_USER_FEE = 1502,		     //�����û��������	
	MON_EVENT_MGATEWAY_SAMPLE = 1508,		     //�����¼�	
	MON_EVENT_MGATEWAY_ALL_SPGATE = 1510,	     //ͨ���˺�������Ϣ2
	MON_EVENT_MGATEWAY_SPGATE_ONLINE = 1511,     //ͨ���ʺ��������
	MON_EVENT_MGATEWAY_SYS_BUFFER = 1520,		 //���ػ�����Ϣ
	MON_EVENT_MGATEWAY_ALL_SPGATETWO = 1521,	 //ͨ��������Ϣ2
};


//�������¼�:�澯���ܵ����
enum{
	COMEVENT_BUFINFO = 1001    //������Ϣ
};

//�쳣����ֶ�
#define MON_EVENT_TYPE_ERROR			10			//10 ������
#define MON_EVENT_TYPE_ERROR_OK			19			//19 ����ָ�
#define MON_EVENT_TYPE_WARN				20			//20 ������
#define MON_EVENT_TYPE_WARN_OK			29			//29 ����ָ�
#define MON_EVENT_TYPE_BASEINFO			30			//30 ������Ϣ���ͣ�ʵʱ�����ͳ������ݣ����磺��Դռ�����/���������ȣ�
#define MON_EVENT_TYPE_SAMPLINGINFO		40			//40 ������Ϣ
#define MON_EVENT_TYPE_SAMPLINGPERIODIC	90			//90 �����Բɼ�
#define MON_EVENT_TYPE_BUSSINESSINFO	99			//99 ҵ�������Ϣ�ࣨ�ʺ������Ϣ/ͨ����ص���Ϣ����ҵ����ص���Ϣ��������

struct CommEventRecord{
	int nMODCLS;//���
	char nLastErrType;//10/19
	time_t tLastMonTime;//���һ���ϱ�ʱ��
	
	CommEventRecord()
	{
		nMODCLS = 0;
		nLastErrType = MON_EVENT_TYPE_ERROR_OK;
		tLastMonTime = 0;
	}
};

struct LastMonTime{
	time_t t_LastBufferMonTime; //����������Ϣ
	time_t t_LastUserMonTime;   //�û�������Ϣ
	time_t t_LastMonSampleTime; //����Ƶ��
	time_t t_LastMonSpgateTime; //ͨ���˺�������Ϣ
	time_t t_LastMonGateTime;   //ͨ��������Ϣ
	time_t t_LastMonProcessTime;//����������Ϣ
	
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

#define MAX_EVENT_MODCLS_BASE_NUM 10001   //�����¼������ʼλ��
#define MAX_EVENT_MODCLS_NUM  10101  //��󹫹��¼�ID.ÿ���������Ķ�����Ķ����ֵ

class CPtMonitor
{
public:
    static CPtMonitor& GetInstance();//����ʵ����ȡ�ӿ�
    virtual ~CPtMonitor();
private:
    CPtMonitor();
	//CMonXmlMaker m_XmlMaker;
	
	//�ϲ������ָ��
	void*			m_pInvokePtr;

	
	int		MakeSysMonInfo(int nFlag);

	//ȡ�����־���д�С
	int		GetMonLogQueSize();
	
	//����ϵͳ��������Ϣ
	int		MakeBufMonInfo(/*in/out*/DEQUE_MO& BufMonQue);

	//�����û��ʺż����Ϣ
	int		MakeUserMonInfo(/*in/out*/DEQUE_MO& UserMonQue);
	//�����ض��ʺ��ض����͵Ĵ�����Ϣ�������ߡ����ߡ�Ƿ�ѣ����ü��V1.0��V2.0
	int		MakeUserMonInfo(/*in/out*/DEQUE_MO& UserMonQue, const int nUID, MON_EXCEPTION_INFO MonExceptionInfo);

	//�����͵ļ����Ϣ�Ƿ���Ҫ�ٴη���֪ͨ	
	bool	CanNotifyMonitor(const int nKey, const MON_EXCEPTION_INFO& MonExceptionInfo);

	//�Ѽ�������Ƶ�ָ����������
	void	RemoveMonExceptionTask(std::list<MON_EXCEPTION_INFO>& vMonTask);

	bool m_bCycle;    //������
	bool m_bOther;    //������

public:

	//���ü��״̬
	void SetMonStatus(bool bCycle, bool bOther);

	//���V2.0��V1.0������ʱ����
	int		MakePtMonInfo(void* pInvokePtr, int nMasterOrSlave, int tMonFreqParams, int nFlag);
	//int		MakePtMonInfo(/*in*/void* pInvokePtr);

	//���ƽ̨��־����־��ض���
	int		AddLogToMonQue(/*in*/int nLogType, /*in*/const char* szLog, int nMODCLS, const char* szWho, const char *szValue, bool bIsErrInfo);

	//�ж����һ�θ澯�ǲ��Ǵ���澯
	bool	bLastCommonIsError(int nMODCLS);

	//�����¼��澯
	int		CommonMonEvent(int nLogType, const char* szLog, int nMODCLS, const char* szWho, const char *szValue, bool bIsErrInfo);

	//���ܣ��Ƿ��д����ͼ����Ϣ
	//����ֵ��false/true
	bool HasMonExceptionTask();

	//����쳣�����Ϣ
	//[IN]MonExceptionInfo���µļ����Ϣ
	//����ֵ��false=������ȫ��ͬ���ҹ�Ƶ����Է��أ�true=�ɹ���Ӳ�����
	bool AddMonExceptionInfo(const MON_EXCEPTION_INFO& MonExceptionInfo);
	
	//�Ѽ���������Ϣ�������MonQue	
	int	MakeExceptionMonInfo(DEQUE_MO& MonQue, /*in*/void* pInvokePtr);

private:
	DerivedLock		m_csMonLog;  //��־������
	DerivedLock		m_csCommonLock; //�����¼���
	std::multimap<int, MON_LOGINFO> m_MonLogQue; //�����־����
	CommEventRecord m_tCommEventRecord[MAX_EVENT_MODCLS_NUM-MAX_EVENT_MODCLS_BASE_NUM+1]; //�����¼�

	int m_nMtRemained;  //��¼�����û���MT������
	int m_nMoRemained;  //��¼�����û���MO������
	int m_nRptRemained; //��¼�����û���RPT������

	//�쳣��Ϣ�����ʷ����Ϊǰ���û�Ƿ���쳣���ߺ���û������쳣������λUID���������Ϳɶ���Ϊ��ֵ������
	std::multimap<int, MON_EXCEPTION_INFO> m_mapMonitorHis;
	//�����͵��쳣��Ϣ
	std::list<MON_EXCEPTION_INFO> m_MonitorTask;
	//�쳣��Ϣ���ݽṹ��
	DerivedLock m_csMonExceptionInfo;
	//��ͬ�����쳣��Ϣ���Ƶ��
	int m_nMonExceptionFreq;
};






#endif // !defined(AFX_PTMONITOR_H__8D87476A_F4DE_44C6_AAFF_649E4C02BB6F__INCLUDED_)
