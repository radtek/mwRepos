#include <stdio.h>
#include "stdlib.h"
#include "SQLConnection.h"
#include "SQLException.h"
#include "SQLResult.h"
#include "time.h"

using namespace ODBC_SQLEXCEPTION_NSAMESPACE;
using namespace ODBC_SQLCONNECTION_NSAMESPACE;
using namespace ODBC_SQLRESULT_NSAMESPACE;
using namespace ODBC_SQLVALUE_NSAMESPACE;

// AccessOracle.h: interface for the CAccessOracle class.
//
//////////////////////////////////////////////////////////////////////

namespace odbcwapper{
	
	struct DBPARAMS_DEF
	{
		std::string strHost;                //数据库地址
		std::string strUser;                //数据库用户名
		std::string strPwd;                 //数据库密码
		std::string strDBName;              //数据库名称
		std::string strConStr;				//数据库连接字符串
		int             nPort;              //数据库端口
		int             nDBType;            //数据库类型
		int             CommandTimeout;     //执行超时时长
		void* pInvoker;
		DBPARAMS_DEF()
		{
			strHost     = "";
			strUser     = "";
			strPwd      = "";
			strDBName   = "";
			strConStr	= "";
			nPort       = 1433;
			nDBType     = 0;
			CommandTimeout = 60;
			pInvoker    = NULL;
		}
		DBPARAMS_DEF& operator=(const DBPARAMS_DEF& other)
		{
			if (this != &other)
			{
				strHost     = other.strHost;
				strUser     = other.strUser;
				strPwd      = other.strPwd;
				strDBName   = other.strDBName;
				strConStr	= other.strConStr;
				nPort       = other.nPort;
				nDBType     = other.nDBType;
				CommandTimeout = other.CommandTimeout;
				pInvoker    = other.pInvoker;
			}
			return *this;
		}
	};
	
	class DBConnection
	{
	public:
		DBConnection()
		{
			bValid = false;
		}
		virtual ~DBConnection(){Close();};
	private:
		bool bValid;
		ODBC_SQLCONNECTION_NSAMESPACE::CSQLConnection sqlconn;
	public:
		ODBC_SQLTYPES_NSAMESPACE::ConnectionPtr GetConnection()
		{
			return (ODBC_SQLTYPES_NSAMESPACE::ConnectionPtr)sqlconn;
		}
		ODBC_SQLTYPES_NSAMESPACE::EnvironmentPtr GetEnvironment() const
		{
			return sqlconn.GetEnvironment();
		}
		bool IsValid()
		{
			return bValid;
		}
		bool Open(const DBPARAMS_DEF& params)
		{
			return (bValid = sqlconn.Open(params.strHost.c_str(),params.strDBName.c_str(),params.strUser.c_str(),params.strPwd.c_str(),params.nPort));
		}
		bool Close()
		{
			bValid = false;
			sqlconn.Close();
			return bValid;
		}
		bool SetConnInvalid()
		{
			bValid = false;
			return bValid;
		}
	};
	
	//字段值(可以设计为泛型)
	class SQLVALUE
	{
	public:
		SQLVALUE() {};
		virtual ~SQLVALUE() {};
	public:
		SQLVALUE& operator=(const SQLVALUE& other)
		{
			if (this == &other)
			{
				return *this;
			}
			value = other.value;
			return *this;
		}
		
		inline operator const char*() const
		{
			return (const char*)value;
		}
		
		inline operator char() const
		{
			return (char)value;
		}
		
		inline operator short() const
		{
			return (short)value;
		}
		
		inline operator int() const
		{
			return (int)value;
		}
		
		inline operator long() const
		{
			return (long)value;
		}
		
		inline operator __int64() const
		{
			return (__int64)value;
		}
		
		inline operator float() const
		{
			return (float)value;
		}
		
		inline operator double() const
		{
			return (double)value;
		}
		
		inline unsigned long GetLength() const
		{
			return value.GetLength();
		}
		
		/*inline SQLType::e_Type GetType() const
		{
        return value.GetType();
    }*/
		
		inline bool IsNull() const
		{
			return value.IsNull();
		}
		
		/*inline bool IsBLOB() const
		{
        return value.IsBLOB();
    }*/
		
		inline const char* GetData() const
		{
			return value.GetData();
		}
	public:
		ODBC_SQLVALUE_NSAMESPACE::CSQLValue value;
	};
	
	//结果集(可以设计成泛型)
	class RecordSet
	{
	public:
		RecordSet(){};
		virtual ~RecordSet(){/*rs.EndFetch();*/};
	public:
		bool EndOfFile;
	private:
		ODBC_SQLVALUE_NSAMESPACE::SQLValueArray values;
		ODBC_SQLRESULT_NSAMESPACE::CSQLResult rs;
	public:
		//执行后，如果需要记录集，指针将直接移至记录集的第一条记录，movefirst函数将不再做任何操作
		//注意：记录集的操作是单向的，只能前向迭代，不能往回操作。当使用了movenext后，再使用movefirst是无效的,没有一点用
		int Open(DBConnection& dbconn, const char *szSql, int nCMDDelay=60,
			ODBC_SQLTYPES_NSAMESPACE::SQLSTRTYPE nType=ODBC_SQLTYPES_NSAMESPACE::SQLSTRTYPE_NORMAL, bool bNeedRecordSet=true)
		{
			rs.Init(dbconn.GetConnection(), dbconn.GetEnvironment());
			int nRet = rs.Query(szSql, nType, bNeedRecordSet);
			EndOfFile = bNeedRecordSet&&TryMoveFirst();
			return nRet;
		}
		void GetValue(const char* szItemName, SQLVALUE& value)
		{
			int nIndex = rs.GetItemIndex(szItemName);
			value.value = values[nIndex];
		}
		//不做任何操作,open函数中已完成此功能
		void MoveFirst()
		{
			//EndOfFile = !rs.Fetch(values);
		}
		//movenext后，将无法再movefirst
		void MoveNext()
		{
			EndOfFile = !rs.Fetch(values);
		}
		void Close()
		{
			rs.EndFetch();
			EndOfFile = true;
		}
	public:
		//////////////////////////////////////////////////////////
		//绑定变量相关
		//1.设置SQL语句
		void SetSQL(DBConnection& dbconn, const char* szSql)
		{
			rs.Init(dbconn.GetConnection(), dbconn.GetEnvironment());
			rs.SetSQL(szSql);
		}
		//2.设置迭代次数
		void SetMaxIterations(unsigned int nMaxIterations=1)
		{
			rs.SetMaxIterations(nMaxIterations);
		}
		/*
		//3.设置参数个数
		void SetParamNum(unsigned int nParamNum=1)
		{
			rs.SetParamNum(nParamNum);
		}
		*/
		//3.设置参数属性
		void SetParamAttr(unsigned int nParamIndex, int sql_type, unsigned int nMaxSize=0, bool bTmstrToTm=false)
		{
			rs.SetParamAttr(nParamIndex, sql_type, nMaxSize, bTmstrToTm);
		}
		//4.设置参数的值
		void SetParamValue(unsigned int nParamIndex, unsigned int nValueLen, const void* pValue)
		{
			rs.SetParamValue(nParamIndex, nValueLen, pValue);
		}
		//5.完成绑定
		void EndParamBind()
		{
			rs.EndParamBind();
		}
		//6.执行绑定
		void ExecuteParamBind()
		{
			rs.ExecuteParamBind();
		}
		
		/////////////////////////////////////////////////////////
	private:
		//尝试移到头，并判断是否有记录集
		bool TryMoveFirst()
		{
			bool bRet = false;
			if (!rs.IsEmpty())
			{
				bRet = !rs.Fetch(values);
			}
			return bRet;
		}
	};
	
	class CDataBaseMgr
	{
	public:
		CDataBaseMgr(){};
		virtual ~CDataBaseMgr()
		{
			CloseDataBase();
		};
	public:
		//设置参数 连接字符串/数据库类型
		void SetParams(const DBPARAMS_DEF& params)
		{
			m_dbparams = params;
		};
		//打开数据库
		int OpenDataBase()
		{
			int nRet = 0;
			char szBuf[1024+1] = {0};
			
			try
			{
				m_PrimaryCon.Open(m_dbparams);
				nRet = 0;
			}
			catch (ODBC_SQLEXCEPTION_NSAMESPACE::CSQLException e)
			{
				nRet = 97;
				
				m_PrimaryCon.SetConnInvalid();
				
				_snprintf(szBuf, sizeof(szBuf)-1,
					"CDataBaseMgr::OpenDatabase() -- ErrorMsg:%s, Description:%s",
					e.ErrorMessage(), e.Description());
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			catch (...)
			{
				nRet = 98;
				
				sprintf(szBuf, "CDataBaseMgr::OpenDatabase() -- Error! ConPtr=%d", &m_PrimaryCon);
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			
			return nRet;
		}
		//关闭数据库
		int CloseDataBase()
		{
			int nRet = 0;
			char szBuf[1024+1] = {0};
			
			try
			{
				if (m_PrimaryCon.IsValid())
				{
					m_PrimaryCon.Close();
				}
				nRet = 0;
			}
			catch (ODBC_SQLEXCEPTION_NSAMESPACE::CSQLException e)
			{
				nRet = 97;
				
				_snprintf(szBuf, sizeof(szBuf)-1,
					"CDataBaseMgr::CloseDatabase() -- ErrorMsg:%s, Description:%s",
					e.ErrorMessage(), e.Description());
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			catch (...)
			{
				nRet = 98;
				
				sprintf(szBuf, "CDataBaseMgr::CloseDatabase() -- Error!");
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			
			return nRet;
		}
	public:
		//主连接执行sql语句并返回记录集 返回值0:正常 -1:参数有问题或重新打数据库失败 97:执行SQL异常 98:函数内部执行异常
		int Execute(const char *szSql, RecordSet& rs, int nCMDDelay=60,
			ODBC_SQLTYPES_NSAMESPACE::SQLSTRTYPE nType=ODBC_SQLTYPES_NSAMESPACE::SQLSTRTYPE_NORMAL, bool bNeedRecordSet=true)
		{
			if (NULL == szSql)
			{
				return -1;
			}
			
			int nRet = 0;
			char szBuf[2048+1] = {0};
			
			try
			{
				if (m_PrimaryCon.IsValid())
				{
					//nRet = 0 || nRet = -1;
					nRet = rs.Open(m_PrimaryCon, szSql, nCMDDelay, nType, bNeedRecordSet);
				}
				else
				{
					nRet = 97;
				}
			}
			catch (ODBC_SQLEXCEPTION_NSAMESPACE::CSQLException e)
			{
				nRet = 97;
				
				m_PrimaryCon.SetConnInvalid();
				
				_snprintf(szBuf, sizeof(szBuf)-1,
					"CDataBaseMgr::Execute() -- ErrorMsg:%s, Description:%s, SQL:%s",
					e.ErrorMessage(), e.Description(), szSql);
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			catch (...)
			{
				nRet = 97;
				
				_snprintf(szBuf, sizeof(szBuf)-1,
					"CDataBaseMgr::Execute() -- Error! nRet=%d SQL:%s",
					nRet, szSql);
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			/*
			if (0 != nRet)
			{
            memset(szBuf, 0, sizeof(szBuf));
            if (0 == OpenDataBase())
            {
			sprintf(szBuf, "CDataBaseMgr::Execute() Error and ReOpen DB Success!");
			CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
            }
            else
            {
			sprintf(szBuf, "CDataBaseMgr::Execute() Error and ReOpen DB Failed!");
			CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
            }
        }*/
			return nRet;
		}
	public:
		/////////////////////////////////////////////
		//绑定变量相关
		//1.设置SQL语句(同时申请stmt)
		int SetSQL(RecordSet& rs, const char* szSql=NULL)
		{
			int nRet = 0;
			char szBuf[2048+1] = {0};
			
			try
			{
				if (m_PrimaryCon.IsValid())
				{
					rs.SetSQL(m_PrimaryCon, szSql);
				}
				else
				{
					nRet = 97;
				}
			}
			catch (ODBC_SQLEXCEPTION_NSAMESPACE::CSQLException e)
			{
				nRet = 97;
				
				m_PrimaryCon.SetConnInvalid();
				
				_snprintf(szBuf, sizeof(szBuf)-1,
					"CDataBaseMgr::SetSQL() -- ErrorMsg:%s, Description:%s, SQL:%s",
					e.ErrorMessage(), e.Description(), szSql);
				printf("%s\r\n", szBuf);
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			catch (...)
			{
				nRet = 97;
				
				_snprintf(szBuf, sizeof(szBuf)-1,
					"CDataBaseMgr::SetSQL() -- Error! nRet=%d", nRet);
				printf("%s\r\n", szBuf);
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			return nRet;
		}
		//2.设置迭代次数(如实填写，不能多也不能少，少了会丢，多了会插入空行)
		int SetIterations(RecordSet& rs, unsigned int nMaxIterations=1)
		{
			int nRet = 0;
			char szBuf[2048+1] = {0};
			
			try
			{
				rs.SetMaxIterations(nMaxIterations);
			}
			catch (ODBC_SQLEXCEPTION_NSAMESPACE::CSQLException e)
			{
				nRet = 97;
				
				m_PrimaryCon.SetConnInvalid();
				
				_snprintf(szBuf, sizeof(szBuf)-1,
					"CDataBaseMgr::SetIterations() -- ErrorMsg:%s, Description:%s",
					e.ErrorMessage(), e.Description());
				printf("%s\r\n", szBuf);
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			catch (...)
			{
				nRet = 97;
				
				_snprintf(szBuf, sizeof(szBuf)-1,
					"CDataBaseMgr::SetIterations() -- Error! nRet=%d", nRet);
				printf("%s\r\n", szBuf);
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			return nRet;
		}
		/*
		//3.设置参数个数(如实填写，不能多也不能少)
		int SetParamNum(RecordSet& rs, unsigned int nParamNum=1)
		{
			int nRet = 0;
			char szBuf[2048+1] = {0};
			
			try
			{
				rs.SetParamNum(nParamNum);
			}
			catch (ODBC_SQLEXCEPTION_NSAMESPACE::CSQLException e)
			{
				nRet = 97;
				
				m_PrimaryCon.SetConnInvalid();
				
				_snprintf(szBuf, sizeof(szBuf)-1,
					"CDataBaseMgr::SetParamNum() -- ErrorMsg:%s, Description:%s",
					e.ErrorMessage(), e.Description());
				printf("%s\r\n", szBuf);
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			catch (...)
			{
				nRet = 97;
				
				_snprintf(szBuf, sizeof(szBuf)-1,
					"CDataBaseMgr::SetParamNum() -- Error! nRet=%d", nRet);
				printf("%s\r\n", szBuf);
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			return nRet;
		}
		*/
		//3.设置每个参数的属性及对应的值
		//paramIndex从1开始,对应SQL语句中字段的顺序
		//sql_type:该字段在数据库中的类型(SQL_VARCHAR,SQL_CHAR,SQL_INTEGER,SQL_SMALLINT,SQL_BIGINT,SQL_NUMERIC,SQL_TYPE_TIMESTAMP)
		//nMaxSize的取值请使用sizeof,如果是字符串,需要将'\0'的长度算在内
		//当nMaxSize取0时表示取sql_type类型的长度(当该类型的长度为定长时可填0)
		//bTmstrToTm:是否将时间字符串转换为时间结构体(当数据库类型为mssql和db2时不需要,oracle时需要)
		//注:BIGINT也就是INT64类型被为NUMERIC来处理，所以可以使用SQL_BIGINT和SQL_NUMERIC
		//注:mssql中的datetime,db2中的timestamp,oracle中的timestamp,请格式化为标准时间"yyyy-mm-dd hh-mm-ss.fff"或"yyyy-mm-dd hh-mm-ss"
		//并使用类型SQL_TYPE_TIMESTAMP,且传入正确的nMaxSize值,mssql/db2时bTmstrToTm=false,oracle时bTmstrToTm=true
		int SetParamAttr(RecordSet& rs, unsigned int nParamIndex, int sql_type, unsigned int nMaxSize=0, bool bTmstrToTm=false)
		{
			int nRet = 0;
			char szBuf[2048+1] = {0};
			
			try
			{
				rs.SetParamAttr(nParamIndex, sql_type, nMaxSize, bTmstrToTm);
			}
			catch (ODBC_SQLEXCEPTION_NSAMESPACE::CSQLException e)
			{
				nRet = 97;
				
				m_PrimaryCon.SetConnInvalid();
				
				_snprintf(szBuf, sizeof(szBuf)-1,
					"CDataBaseMgr::SetParamAttr() -- ErrorMsg:%s, Description:%s",
					e.ErrorMessage(), e.Description());
				printf("%s\r\n", szBuf);
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			catch (...)
			{
				nRet = 97;
				
				_snprintf(szBuf, sizeof(szBuf)-1,
					"CDataBaseMgr::SetParamAttr() -- Error! nRet=%d", nRet);
				printf("%s\r\n", szBuf);
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			return nRet;
		}
		//4.设置参数的值
		//paramIndex从1开始
		int SetParamValue(RecordSet& rs, unsigned int nParamIndex, unsigned int nValueLen, const void* pValue)
		{
			int nRet = 0;
			char szBuf[2048+1] = {0};
			
			try
			{
				rs.SetParamValue(nParamIndex, nValueLen, pValue);
			}
			catch (ODBC_SQLEXCEPTION_NSAMESPACE::CSQLException e)
			{
				nRet = 97;
				
				m_PrimaryCon.SetConnInvalid();
				
				_snprintf(szBuf, sizeof(szBuf)-1,
					"CDataBaseMgr::SetParamValue() -- ErrorMsg:%s, Description:%s",
					e.ErrorMessage(), e.Description());
				printf("%s\r\n", szBuf);
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			catch (...)
			{
				nRet = 97;
				
				_snprintf(szBuf, sizeof(szBuf)-1,
					"CDataBaseMgr::SetParamValue() -- Error! nRet=%d", nRet);
				printf("%s\r\n", szBuf);
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			return nRet;
		}
		//5.完成绑定
		int EndParamBind(RecordSet& rs)
		{
			int nRet = 0;
			char szBuf[2048+1] = {0};
			
			try
			{
				rs.EndParamBind();
			}
			catch (ODBC_SQLEXCEPTION_NSAMESPACE::CSQLException e)
			{
				nRet = 97;
				
				m_PrimaryCon.SetConnInvalid();
				
				_snprintf(szBuf, sizeof(szBuf)-1,
					"CDataBaseMgr::EndParamBind() -- ErrorMsg:%s, Description:%s",
					e.ErrorMessage(), e.Description());
				printf("%s\r\n", szBuf);
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			catch (...)
			{
				nRet = 97;
				
				_snprintf(szBuf, sizeof(szBuf)-1,
					"CDataBaseMgr::EndParamBind() -- Error! nRet=%d", nRet);
				printf("%s\r\n", szBuf);
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			return nRet;
		}
		//6.执行绑定
		int ExecuteParamBind(RecordSet& rs)
		{
			int nRet = 0;
			char szBuf[2048+1] = {0};
			
			try
			{
				rs.ExecuteParamBind();
			}
			catch (ODBC_SQLEXCEPTION_NSAMESPACE::CSQLException e)
			{
				nRet = 97;
				
				m_PrimaryCon.SetConnInvalid();
				
				_snprintf(szBuf, sizeof(szBuf)-1,
					"CDataBaseMgr::ExecuteParamBind() -- ErrorMsg:%s, Description:%s",
					e.ErrorMessage(), e.Description());
				printf("%s\r\n", szBuf);
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			catch (...)
			{
				nRet = 97;
				
				_snprintf(szBuf, sizeof(szBuf)-1,
					"CDataBaseMgr::ExecuteParamBind() -- Error! nRet=%d", nRet);
				printf("%s\r\n", szBuf);
				//CLogMgr::GetInstance().WriteLog(szBuf, "AdoError.txt");
				//CGlobal::BeepEx(2000, 1000);
			}
			return nRet;
		}
		
		/////////////////////////////////////////////
private:
    DBPARAMS_DEF        m_dbparams;        //数据库参数
    DBConnection    m_PrimaryCon;       //主连接
};
};

int GetMsgId(int Num, bool &bFlow)
{
    static int g_iMsgId = 0;
    int iFirstId = 0;
    try
    {
        //cs_MsgId.Lock();
        iFirstId = g_iMsgId + 1;
        g_iMsgId += Num;
        //32位int
        //unsigned int 4294967295
        //int   2147483647
        if (g_iMsgId > 1000000)
        {
            g_iMsgId = 0;
            iFirstId = 0;
            g_iMsgId += Num;
            bFlow = TRUE;
        }
        //cs_MsgId.Unlock();
        return iFirstId;
    }
    catch (...)
    {
        //cs_MsgId.Unlock();
    }
    return 0;
}

void MakeMsgId(unsigned char *MsgId, unsigned int Num, unsigned int Gate)
{
    static time_t tLastMake;
    unsigned int nMonth = 0;
    unsigned int nDay   = 0;
    unsigned int nHour  = 0;
    unsigned int nMin   = 0;
    unsigned int nSec   = 0;
    unsigned int nGate  = Gate;
    bool        bFlow   = FALSE;
    unsigned int nNo    = GetMsgId(Num, bFlow);
    if (NULL == MsgId)
    {
        return;
    }
    time_t tNow;
    time(&tNow);
    if (tNow == tLastMake && bFlow)
    {
        Sleep(1000);
    }
	
    time(&tLastMake);
    SYSTEMTIME t;
    ::GetLocalTime(&t);
    nMonth  = t.wMonth;
    nDay    = t.wDay;
    nHour   = t.wHour;
    nMin    = t.wMinute;
    nSec    = t.wSecond;
	
    INT64 j = 0;
    j |= (INT64)(nMonth & 0x0f)       << 60;
    j |= (INT64)(nDay   & 0x1f)       << 55;
    j |= (INT64)(nHour  & 0x1f)       << 50;
    j |= (INT64)(nMin   & 0x3f)       << 44;
    j |= (INT64)(nSec   & 0x3f)       << 38;
    j |= (INT64)(nGate  & 0x03ffff)   << 20;
    j |= (INT64)(nNo    & 0x0fffff);
	//nGate最大值只能取262144
	//nNo最大值只能取1048575
	
    //j |= (__int64)(nGate & 0x3fffff)<< 16;
    //j |= (__int64)(nNo & 0x0ffff);
	
    MsgId[0] = (unsigned char)((j >> 56) & 0xff);
    MsgId[1] = (unsigned char)((j >> 48) & 0xff);
    MsgId[2] = (unsigned char)((j >> 40) & 0xff);
    MsgId[3] = (unsigned char)((j >> 32) & 0xff);
    MsgId[4] = (unsigned char)((j >> 24) & 0xff);
    MsgId[5] = (unsigned char)((j >> 16) & 0xff);
    MsgId[6] = (unsigned char)((j >>  8) & 0xff);
    MsgId[7] = (unsigned char)((j)       & 0xff);
    return;
}


INT64 TranMsgIdCharToI64(const unsigned char *szMsgId)
{
    if (NULL == szMsgId)
    {
        return 0;
    }
	
    INT64 j = 0;
    const unsigned char* p = szMsgId;
    j += (INT64)(*p)     << 56;
    j += (INT64)*(p + 1) << 48;
    j += (INT64)*(p + 2) << 40;
    j += (INT64)*(p + 3) << 32;
    j += (INT64)*(p + 4) << 24;
    j += (INT64)*(p + 5) << 16;
    j += (INT64)*(p + 6) << 8;
    j += (INT64)*(p + 7);
    return j;
}

using namespace odbcwapper;

//const char *szSql = "INSERT INTO JHBTEST (PartId,Description) VALUES (?,?)";
//const char *szSql = "INSERT INTO JHBTEST (PartId,DESCRIPTION,TM,TESTID) VALUES (?,?,?,?)";
const char *szSql_Normal = "INSERT INTO RPT_WAIT_B(UID,PTMSGID,SPNUMBER,PHONE,SUBMITTIME,DONETIME,ERRORCODE,USERID,ECID,ORGUID,RESENDCNT,USERMSGID,"
							"SENDLEVEL,RECVTIME,SUPPMSGID,MTSENDTIME,SUBMITDATE,DONEDATE,MOBILETYPE,MOBILEAREA,PREGATENO,LOCALGATENO,NEXTGATENO,EXNO,JHBTEST) "
							"VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
const char *szSql_Proc = "EXEC S_WR_RPTWAITB ?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?";
//const char *szSql_Normal_Oracle = "INSERT /*+APPEND*/ INTO MT_TASK NOLOGGING (\"UID\",PTMSGID,ECID,SENDSTATUS,RETFLAG,PKNUMBER,PKTOTAL,FEEFLAG,SENDLEVEL,RECVMTTIME,USERID,SPGATE,CPNO,PHONE,MESSAGE,TASKID,ERRORCODE,TPUDHI,LONGMSGSEQ,MSGFMT,UNICOM,MOBILEAREA,SVRTYPE,P1,P2,P3,P4) SELECT ?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,? FROM DUAL";
const char *szSql_Normal_Oracle = "INSERT INTO MT_TASK (\"UID\",PTMSGID,ECID,SENDSTATUS,RETFLAG,PKNUMBER,PKTOTAL,FEEFLAG,SENDLEVEL,RECVMTTIME,USERID,SPGATE,CPNO,PHONE,MESSAGE,TASKID,ERRORCODE,TPUDHI,LONGMSGSEQ,MSGFMT,UNICOM,MOBILEAREA,SVRTYPE,P1,P2,P3,P4) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";

//,DESCRIPTION,TESTID
int main()
{
	int nMaxIterations = 1;
	int nLoopCount = 1;
	int nDBTYPE = 0;
	int nSqlType = 1;
	int nSrandNum = 1234;
	int nCurrentCnt = 0;
	bool bTmstrToTm = false;
	
	printf("Please input LoopCount:\r\n");
	scanf("%d", &nLoopCount);
	printf("Please input Interations:\r\n");
	scanf("%d", &nMaxIterations);
	printf("Please Select DBTYPE:\r\n0:MSSQL 1:DB2  2:ORACLE\r\n");
	scanf("%d", &nDBTYPE);
	printf("Please Select SQLTYPE:\r\n0:NORMAL 1:PROC\r\n");
	scanf("%d", &nSqlType);
	printf("Please input SrandNumber:\r\n");
	scanf("%d", &nSrandNum);

	odbcwapper::DBPARAMS_DEF db;
	char *pSql = NULL;
	if (0 == nDBTYPE)
	{
		db.strHost = "127.0.0.1";
		db.strDBName = "empsvr235";
		db.strUser = "sa";
		db.strPwd = "123zxcvbnm,./";
	}
	else if (1 == nDBTYPE)
	{
		db.strHost = "127.0.0.1";
		db.strDBName = "JHBTESTDB2";
		db.strUser = "DB2JHB";
		db.strPwd = "123456";
	}
	else if (2 == nDBTYPE)
	{
		bTmstrToTm = true;
		db.strHost = "127.0.0.1";
		db.strDBName = "JHBTESTORA";
		db.strUser = "EMPSVR";
		db.strPwd = "123456";
	}
	
	CDataBaseMgr DBMgr;
	DBMgr.SetParams(db);
	
	if (0 == nSqlType && 2 == nDBTYPE)
	{
		pSql = (char*)szSql_Normal;
	}
	else if (1 == nSqlType)
	{
		pSql = (char*)szSql_Proc;
	}
	else
	{
		pSql = (char*)szSql_Normal;
	}
	time_t tTmp = time(NULL);
	if (0 == nSqlType)
	{
		try
		{	
			if(0 == DBMgr.OpenDataBase())
			{
				time_t t1 = time(NULL);

				for (int kk = 0; kk < nLoopCount; ++kk)
				{
					RecordSet rs;
					int nIterations = nMaxIterations;
					
					DBMgr.SetSQL(rs, pSql);
					DBMgr.SetIterations(rs, nIterations);

					int nParamIndex = 0;
					int nUID = 100053;
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_INTEGER);//UID
					char szMsgId[20+1] = {"-8082710085533433855"};	
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_NUMERIC, sizeof(szMsgId));//PTMSGID
					char szSpNumber[21+1] = {"106571234567890"};
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_VARCHAR, sizeof(szSpNumber));//SPNUMBER
					char szPhone[21+1] = {"13265661403"};
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_VARCHAR, sizeof(szPhone));//PHONE
					char szSubmitTime[10+1] = {"1308281320"};
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_VARCHAR, sizeof(szSubmitTime));//SUBMITTIME
					char szDoneTime[10+1] = {"1308281320"};
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_VARCHAR, sizeof(szDoneTime));//DONETIME
					char szErrorCode[7+1] = {"DELIVRD"};
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_VARCHAR, sizeof(szErrorCode));//ERRORCODE
					char szUserId[11+1] = {"SP1234"};
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_VARCHAR, sizeof(szUserId));//USERID
					int nEcId = 123456;
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_INTEGER);//ECID
					int nOrgUID = 123456;
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_INTEGER);//ORGUID
					short nResendCnt = 0;
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_SMALLINT);//RESENDCNT
					char szUserMsgId[20+1] = {"1234567890123456789"};	
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_NUMERIC, sizeof(szUserMsgId));//USERMSGID
					short nSendLevel = 5;
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_SMALLINT);//SENDLEVEL

					char szRecvTime[25+1] = {0};
					SYSTEMTIME t;
					::GetLocalTime(&t);
					sprintf(szRecvTime, "%04d-%02d-%02d %02d:%02d:%02d.%03d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_TYPE_TIMESTAMP, sizeof(szRecvTime), bTmstrToTm);//RECVTIME
					char szSuppMsgId[20+1] = {"1234567890123456789"};	
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_NUMERIC, sizeof(szSuppMsgId));//SUPPMSGID
					char szMtSendTime[25+1] = {0};
					sprintf(szMtSendTime, "%04d-%02d-%02d %02d:%02d:%02d.%03d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_TYPE_TIMESTAMP, sizeof(szMtSendTime), bTmstrToTm);//MTSENDTIME
					char szMtSubmitDate[25+1] = {0};
					sprintf(szMtSubmitDate, "%04d-%02d-%02d %02d:%02d:%02d.%03d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_TYPE_TIMESTAMP, sizeof(szMtSubmitDate), bTmstrToTm);//SUBMITDATE
					char szDoneDate[25+1] = {0};
					sprintf(szDoneDate, "%04d-%02d-%02d %02d:%02d:%02d.%03d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_TYPE_TIMESTAMP, sizeof(szDoneDate), bTmstrToTm);//DONEDATE
					short nMobileType = 255;
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_SMALLINT);//MOBILETYPE
					short nMobileArea = 0;
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_SMALLINT);//MOBILEAREA
					int nPreGateNo = 0;
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_INTEGER);//PREGATENO
					int nLocalGateNo = 0;
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_INTEGER);//LOCALGATENO
					int nNextGateNo = 0;
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_INTEGER);//NEXTGATENO
					char szExNo[21+1] = {"456123"};
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_VARCHAR, sizeof(szExNo));//EXNO

					char szJhbtest[2048+1] = {0};
					DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_VARCHAR, sizeof(szJhbtest));//jhbtest

					for (int i = 0; i < nIterations; ++i)
					{
						++nCurrentCnt;

						int nParamIndex = 0;
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nUID), &nUID);//UID
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szMsgId), szMsgId);//PTMSGID
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szSpNumber), szSpNumber);//SPNUMBER
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szPhone), szPhone);//PHONE
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szSubmitTime), szSubmitTime);//SUBMITTIME
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szDoneTime), szDoneTime);//DONETIME
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szErrorCode), szErrorCode);//ERRORCODE
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szUserId), szUserId);//USERID
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nEcId), &nEcId);//ECID
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nOrgUID), &nOrgUID);//ORGUID
						unsigned char nResendCnt = 0;
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nResendCnt), &nResendCnt);//RESENDCNT
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szUserMsgId), szUserMsgId);//USERMSGID
						unsigned char nSendLevel = 0;
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nSendLevel), &nSendLevel);//SENDLEVEL
						
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szRecvTime), szRecvTime);//RECVTIME
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szUserMsgId), szUserMsgId);//SUPPMSGID
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szMtSendTime), szMtSendTime);//MTSENDTIME
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szMtSubmitDate), szMtSubmitDate);//SUBMITDATE
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szDoneDate), szDoneDate);//DONEDATE
						unsigned char nMobileType = 0;
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nMobileType), &nMobileType);//MOBILETYPE
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nMobileArea), &nMobileArea);//MOBILEAREA
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nPreGateNo), &nPreGateNo);//PREGATENO
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nLocalGateNo), &nLocalGateNo);//LOCALGATENO
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nNextGateNo), &nNextGateNo);//NEXTGATENO
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szExNo), szExNo);//EXNO	
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szJhbtest), szJhbtest);//Jhbtest
					}
					DBMgr.EndParamBind(rs);
					if (0 != DBMgr.ExecuteParamBind(rs))
					{
						break;
					}
					//每三秒显示一次当前插入的总量
					int nTm = time(NULL)-tTmp;
					if (nTm >= 2)
					{
						static int nTmpCnt = nCurrentCnt;
						printf("CurrentNum:%10d      TM: %d s      Speed: %d/s\r\n", nCurrentCnt, nTm, (nCurrentCnt-nTmpCnt)/nTm);
						tTmp = time(NULL);
						nTmpCnt = nCurrentCnt;
					}
				}
				time_t t2 = time(NULL);
				printf("-----------%d---------%d\r\n", t2-t1, (nMaxIterations*nLoopCount)/((t2-t1)==0?1:t2-t1));
			}
		}
		catch (exception& e)
		{
			printf(e.what());
			//printf("%s,%s", e.ErrorMessage(), e.Description());
			//CLogMgr::GetInstance().WriteLog(e.Description());
		}
	}
	else if (1 == nSqlType)
	{
		try
		{	
			if(0 == DBMgr.OpenDataBase())
			{
				RecordSet rs;
				int nIterations = nMaxIterations;
				
				DBMgr.SetSQL(rs, pSql);
				DBMgr.SetIterations(rs, nIterations);

				int nParamIndex = 0;
				int nUID = 100053;
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_INTEGER);//UID
				char szMsgId[20+1] = {"1234567890123456789"};	
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_NUMERIC, sizeof(szMsgId));//PTMSGID
				int nEcId = 123456;
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_INTEGER);//ECID
				char szSpNumber[21+1] = {"106571234567890"};
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_VARCHAR, sizeof(szSpNumber));//SPNUMBER
				char szPhone[21+1] = {"13265661403"};
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_VARCHAR, sizeof(szPhone));//PHONE
				char szSubmitTime[10+1] = {"1308281320"};
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_VARCHAR, sizeof(szSubmitTime));//SUBMITTIME
				char szDoneTime[10+1] = {"1308281320"};
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_VARCHAR, sizeof(szDoneTime));//DONETIME
				char szErrorCode[7+1] = {"DELIVRD"};
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_VARCHAR, sizeof(szErrorCode));//ERRORCODE
				char szUserId[11+1] = {"SP1234"};
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_VARCHAR, sizeof(szUserId));//USERID
				int nOrgUID = 123456;
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_INTEGER);//ORGUID
				short nResendCnt = 0;
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_SMALLINT);//RESENDCNT
				char szUserMsgId[20+1] = {"1234567890123456789"};	
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_NUMERIC, sizeof(szUserMsgId));//USERMSGID
				short nSendLevel = 5;
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_SMALLINT);//SENDLEVEL

				char szRecvTime[25+1] = {0};
				SYSTEMTIME t;
				::GetLocalTime(&t);
				sprintf(szRecvTime, "%04d-%02d-%02d %02d:%02d:%02d.%03d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_TYPE_TIMESTAMP, sizeof(szRecvTime), bTmstrToTm);//RECVTIME
				char szSuppMsgId[20+1] = {"1234567890123456789"};	
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_NUMERIC, sizeof(szSuppMsgId));//SUPPMSGID
				char szMtSendTime[25+1] = {0};
				sprintf(szMtSendTime, "%04d-%02d-%02d %02d:%02d:%02d.%03d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_TYPE_TIMESTAMP, sizeof(szMtSendTime), bTmstrToTm);//MTSENDTIME
				char szMtSubmitDate[25+1] = {0};
				sprintf(szMtSubmitDate, "%04d-%02d-%02d %02d:%02d:%02d.%03d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_TYPE_TIMESTAMP, sizeof(szMtSubmitDate), bTmstrToTm);//SUBMITDATE
				char szDoneDate[25+1] = {0};
				sprintf(szDoneDate, "%04d-%02d-%02d %02d:%02d:%02d.%03d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_TYPE_TIMESTAMP, sizeof(szDoneDate), bTmstrToTm);//DONEDATE
				short nMobileType = 255;
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_SMALLINT);//MOBILETYPE
				short nMobileArea = 0;
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_SMALLINT);//MOBILEAREA
				int nPreGateNo = 0;
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_INTEGER);//PREGATENO
				int nLocalGateNo = 0;
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_INTEGER);//LOCALGATENO
				int nNextGateNo = 0;
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_INTEGER);//NEXTGATENO
				char szExNo[21+1] = {"456123"};
				DBMgr.SetParamAttr(rs, ++nParamIndex, SQL_VARCHAR, sizeof(szExNo));//EXNO

				time_t t1 = time(NULL);
				for (int kk = 0; kk < nLoopCount; ++kk)
				{
					for (int i = 0; i < nIterations; ++i)
					{
						++nCurrentCnt;

						int nParamIndex = 0;
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nUID), &nUID);//UID
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szMsgId), szMsgId);//PTMSGID
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nEcId), &nEcId);//ECID
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szSpNumber), szSpNumber);//SPNUMBER
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szPhone), szPhone);//PHONE
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szSubmitTime), szSubmitTime);//SUBMITTIME
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szDoneTime), szDoneTime);//DONETIME
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szErrorCode), szErrorCode);//ERRORCODE
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szUserId), szUserId);//USERID
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nOrgUID), &nOrgUID);//ORGUID
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nResendCnt), &nResendCnt);//RESENDCNT
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szUserMsgId), szUserMsgId);//USERMSGID
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nSendLevel), &nSendLevel);//SENDLEVEL
						
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szRecvTime), szRecvTime);//RECVTIME
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szUserMsgId), szUserMsgId);//SUPPMSGID
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szMtSendTime), szMtSendTime);//MTSENDTIME
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szMtSubmitDate), szMtSubmitDate);//SUBMITDATE
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szDoneDate), szDoneDate);//DONEDATE
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nMobileType), &nMobileType);//MOBILETYPE
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nMobileArea), &nMobileArea);//MOBILEAREA
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nPreGateNo), &nPreGateNo);//PREGATENO
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nLocalGateNo), &nLocalGateNo);//LOCALGATENO
						DBMgr.SetParamValue(rs, ++nParamIndex, sizeof(nNextGateNo), &nNextGateNo);//NEXTGATENO
						DBMgr.SetParamValue(rs, ++nParamIndex, strlen(szExNo), szExNo);//EXNO	
					}
					DBMgr.EndParamBind(rs);
					if (0 != DBMgr.ExecuteParamBind(rs))
					{
						break;
					}
					
					//每三秒显示一次当前插入的总量
					int nTm = time(NULL)-tTmp;
					if (nTm >= 2)
					{
						static int nTmpCnt = nCurrentCnt;
						printf("CurrentNum:%10d      TM: %d s      Speed: %d/s\r\n", nCurrentCnt, nTm, (nCurrentCnt-nTmpCnt)/nTm);
						tTmp = time(NULL);
						nTmpCnt = nCurrentCnt;
					}
				}
				time_t t2 = time(NULL);
				printf("-----------%d---------%d\r\n", t2-t1, (nMaxIterations*nLoopCount)/((t2-t1)==0?1:t2-t1));
			}
		}
		catch (exception& e)
		{
			printf(e.what());
			//printf("%s,%s", e.ErrorMessage(), e.Description());
			//CLogMgr::GetInstance().WriteLog(e.Description());
		}
	}
	
	printf("exec over...\r\n");
	while (1)
	{
		Sleep(1000);
	}
    return 0;
}


