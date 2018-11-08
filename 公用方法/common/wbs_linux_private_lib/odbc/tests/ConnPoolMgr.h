#ifndef __CONNPOOL_MGR_HH___
#define __CONNPOOL_MGR_HH___

#include <odbc/Connection.h>
#include <odbc/Mutex.h>
#include <odbc/Thread.h>
#include <map>
#include <list>
#include <vector>

#define MAX_DBCONNPOOL_SIZE		5					//���ɴ��������ӳ���
#define MIN_DBCONN_SIZE			1					//ÿ�����ӳ�����������С������
#define	DEFAUT_DBCONN_SIZE		5					//ÿ�����ӳ�Ĭ�ϴ�����������
#define MAX_DBCONN_SIZE			255					//ÿ�����ӳ������������������
#define MAX_DBCONN_ACTIVE_TM	3*60				//���Ӽ���ʱ��
#define DEFAULT_POOLNAME		"DBCONNECTPOOL"		//Ĭ�����ӳ�����
#define MIN_WAITCONN_TM			5					//�ȴ���ȡ���ӵĳ�ʱֵ��λ��
#define ALIVETHREADNAME			"dbAliveThread"		//������߳�����

#define DEFAULT_DBPOOL1  0
#define DEFAULT_DBPOOL2  1

#define MGX_MW_DB_ERROR_99 99
#define NGX_MW_OK          0

struct DBCONN
{
    ODBC::Connection* conn;
    time_t lastActive;      // �ϴ�ʹ�õ�ʱ��

    DBCONN()
    {
        conn = NULL;
		time(&lastActive);
    }
	
    DBCONN & operator = (const DBCONN & other)
    {
        if (this == &other)
        {
            return *this;
        }
        memset(this, 0, sizeof(DBCONN));
        memcpy(this, &other, sizeof(DBCONN));
        return *this;
    }
};

class CDBConnPoolMgr;

//���ݿ����ӳ���
class CDBConnPool
{
	friend class CDBConnPoolMgr;

public:
    CDBConnPool();
    virtual ~CDBConnPool();
private:  
	//��ʼ�����ӳ�,������nMinSize������,���������Ӷ�����ɹ�ʱ����true.
	bool InitPool(const char* dbDNSName, 
                  const char* userName, 
                  const char* passWord,
                  const char* szActiveSql = ""/*�������*/, 
                  int nMinSize = MIN_DBCONN_SIZE, 
                  int nMaxSize = MAX_DBCONN_SIZE);
                  
	//�ӳ����л�ȡһ������.��ָ�����ȴ�ʱ��(��λ:s),��ʱ���Զ�����NULL.���ȴ�ʱ����СֵΪ5s
	ODBC::Connection* GetConnFromPool(int nMaxTimeWait = MIN_WAITCONN_TM);
	//��������
	void RecycleConn(ODBC::Connection* pConn);
	//��Ҫ��������Ƿ���Ч
	bool IsConnValid(ODBC::Connection* pConn);
	//��ȡ���ӳ���Ϣ
	void GetConnPoolInfo(int& nMinSize, int& nMaxSize, int& nIdle, int& nBusy);
	//�������ӳش�С
	void ResetConnPoolSize(int nMinSize, int nMaxSize);
	
private:
	//���������Ƿ����
	bool TestConn(ODBC::Connection* pConn);
	//�������ӳ�,���ͷ���������
	void UnInitPool();
	//����������
	bool ApplayNewConn(DBCONN& conn);
	//���ʱ����е�����
	void ActiveIdleConn();
	
private:
	int m_nMinSize;
	int m_nMaxSize;
	std::string m_dbDNSName;
	std::string m_userName;
	std::string m_passWord;
	std::string m_strActiveSql;
	std::map<ODBC::Connection*, int> m_InUse;
	std::list<DBCONN> m_Idle;
    ODBC::MutexLock m_mutex;
};

//���ݿ����ӳع�����
class CDBConnPoolMgr
{
public:
    static CDBConnPoolMgr& GetInstance();//����ʵ����ȡ�ӿ�
    virtual ~CDBConnPoolMgr();
public: 
	//�������ӳز�ָ�����ӵ�����,�����������ִ�Сд
	bool CreateConnPool(const char* szPoolName, 
	                    const char* dbDNSName, 
                        const char* userName, 
                        const char* passWord,
                        const char* szActiveSql = ""/*�������*/, 
                        int nMinSize = 5, 
                        int nMaxSize = 50);
    
	//��ָ���ĳ����л�ȡһ������.��ָ�����ȴ�ʱ��(��λ:s),��ʱ���Զ�����NULL.���ȴ�ʱ����СֵΪ5s
	ODBC::Connection* GetConnFromPool(int index, int nMaxTimeWait=5);
	//�����ӻ�����ָ��������
	void RecycleConn(int index, ODBC::Connection* pConn);
	//��Ҫ��������Ƿ���Ч
	bool IsConnValid(int index, ODBC::Connection* pConn);
	
	//�������ӳ��еĿ�������
	void ActiveIdleConn();
	//���ݿ����ӱ����̺߳���
	static void ThreadFuncActive();
	//��ȡ���ӳ���Ϣ
	void GetConnPoolInfo(int index, int& nMinSize, int& nMaxSize, int& nIdle, int& nBusy);
	
	//�������ӳش�С
	void ResetConnPoolSize(int index, int nMinSize, int nMaxSize);

private:
    CDBConnPoolMgr();
    std::string& getPoolName(int index);
    
private:
	static std::string kNullString;
	std::map<std::string, CDBConnPool*> m_dbpool;
	std::vector<std::string> m_poolNames;
	ODBC::Thread threadActive_;
	//ODBC::MutexLock m_mutex;
	bool bActiveing_;
	bool m_bExit;
};

/**
 * ���ã���֤�����Զ��黹�����ӳأ�������ʹ������ʱ����Ŀռ�
 */
class CDBConnectionPtr
{
public:
	CDBConnectionPtr(int index, ODBC::Connection* conn) : index_(index), conn_(conn)
	{};
	virtual ~CDBConnectionPtr()
	{
		if (conn_ != NULL)
		{
			CDBConnPoolMgr::GetInstance().RecycleConn(index_, conn_);
			conn_ = NULL;
		}
	}

private:
    int index_;
	ODBC::Connection* conn_;
};

// Prevent misuse like:
// CDBConnectionPtr(poolName, conn);
// A tempory object doesn't hold the lock for long!
#define CDBConnectionPtr(x, y) error "Missing CDBConnectionPtr object name"

#endif // __CONNPOOL_MGR_HH___
