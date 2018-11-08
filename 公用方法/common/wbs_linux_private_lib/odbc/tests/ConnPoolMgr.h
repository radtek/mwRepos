#ifndef __CONNPOOL_MGR_HH___
#define __CONNPOOL_MGR_HH___

#include <odbc/Connection.h>
#include <odbc/Mutex.h>
#include <odbc/Thread.h>
#include <map>
#include <list>
#include <vector>

#define MAX_DBCONNPOOL_SIZE		5					//最大可创建的连接池数
#define MIN_DBCONN_SIZE			1					//每个连接池允许创建的最小连接数
#define	DEFAUT_DBCONN_SIZE		5					//每个连接池默认创建的连接数
#define MAX_DBCONN_SIZE			255					//每个连接池允许创建的最大连接数
#define MAX_DBCONN_ACTIVE_TM	3*60				//连接激活时间
#define DEFAULT_POOLNAME		"DBCONNECTPOOL"		//默认连接池名称
#define MIN_WAITCONN_TM			5					//等待获取连接的超时值单位秒
#define ALIVETHREADNAME			"dbAliveThread"		//保活的线程名称

#define DEFAULT_DBPOOL1  0
#define DEFAULT_DBPOOL2  1

#define MGX_MW_DB_ERROR_99 99
#define NGX_MW_OK          0

struct DBCONN
{
    ODBC::Connection* conn;
    time_t lastActive;      // 上次使用的时间

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

//数据库连接池类
class CDBConnPool
{
	friend class CDBConnPoolMgr;

public:
    CDBConnPool();
    virtual ~CDBConnPool();
private:  
	//初始化连接池,并申请nMinSize个连接,当所有连接都申请成功时返回true.
	bool InitPool(const char* dbDNSName, 
                  const char* userName, 
                  const char* passWord,
                  const char* szActiveSql = ""/*激活语句*/, 
                  int nMinSize = MIN_DBCONN_SIZE, 
                  int nMaxSize = MAX_DBCONN_SIZE);
                  
	//从池子中获取一个连接.可指定最大等待时间(单位:s),超时将自动返回NULL.最大等待时间最小值为5s
	ODBC::Connection* GetConnFromPool(int nMaxTimeWait = MIN_WAITCONN_TM);
	//回收连接
	void RecycleConn(ODBC::Connection* pConn);
	//需要检测连接是否还有效
	bool IsConnValid(ODBC::Connection* pConn);
	//获取连接池信息
	void GetConnPoolInfo(int& nMinSize, int& nMaxSize, int& nIdle, int& nBusy);
	//重置连接池大小
	void ResetConnPoolSize(int nMinSize, int nMaxSize);
	
private:
	//测试连接是否可用
	bool TestConn(ODBC::Connection* pConn);
	//销毁连接池,并释放所有连接
	void UnInitPool();
	//申请新连接
	bool ApplayNewConn(DBCONN& conn);
	//激活长时间空闲的连接
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

//数据库连接池管理类
class CDBConnPoolMgr
{
public:
    static CDBConnPoolMgr& GetInstance();//单件实例获取接口
    virtual ~CDBConnPoolMgr();
public: 
	//创建连接池并指定池子的名称,池子名称区分大小写
	bool CreateConnPool(const char* szPoolName, 
	                    const char* dbDNSName, 
                        const char* userName, 
                        const char* passWord,
                        const char* szActiveSql = ""/*激活语句*/, 
                        int nMinSize = 5, 
                        int nMaxSize = 50);
    
	//从指定的池子中获取一个连接.可指定最大等待时间(单位:s),超时将自动返回NULL.最大等待时间最小值为5s
	ODBC::Connection* GetConnFromPool(int index, int nMaxTimeWait=5);
	//将连接回收至指定池子中
	void RecycleConn(int index, ODBC::Connection* pConn);
	//需要检测连接是否还有效
	bool IsConnValid(int index, ODBC::Connection* pConn);
	
	//激活连接池中的空闲连接
	void ActiveIdleConn();
	//数据库连接保活线程函数
	static void ThreadFuncActive();
	//获取连接池信息
	void GetConnPoolInfo(int index, int& nMinSize, int& nMaxSize, int& nIdle, int& nBusy);
	
	//重置连接池大小
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
 * 作用：保证连接自动归还给连接池，并清理使用连接时申请的空间
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
