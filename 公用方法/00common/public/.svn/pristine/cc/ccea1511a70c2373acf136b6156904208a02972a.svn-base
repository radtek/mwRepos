#include "StdAfx.h"
#include "SQLException.h"
#include "SQLResult.h"
#include "SQLValue.h"
#include "SQLField.h"
ODBC_SQLRESULT_NSAMESPACE::CSQLResult::CSQLResult(ODBC_SQLTYPES_NSAMESPACE::ConnectionPtr pCon)
   : ODBC_SQLBASE_NSAMESPACE::CSQLBase(pCon),m_hStmt(NULL)
{
}

ODBC_SQLRESULT_NSAMESPACE::CSQLResult::CSQLResult():m_hStmt(NULL)
{
	m_nMaxIterations = 0;
	m_nParamNum = 0;
	m_nDbType = 0;
}


void ODBC_SQLRESULT_NSAMESPACE::CSQLResult::Init(ODBC_SQLTYPES_NSAMESPACE::ConnectionPtr pCon, ODBC_SQLTYPES_NSAMESPACE::EnvironmentPtr pEnv, int nDbType)
{
    m_hCon    = pCon;
	m_nDbType = nDbType;
}

ODBC_SQLRESULT_NSAMESPACE::CSQLResult::~CSQLResult()
{
   FreeParamBind();
   EndFetch();
}
/*
std::string FormatSql(const char* szProc, bool bAppendix)
{
    std::string strProc = "";
    if (NULL != szProc && '\0' != szProc[0])
    {
        int nLen = strlen(szProc);
        if ('}' == szProc[nLen-1])
        {
            --nLen;
        }
        if ('{' == szProc[0])
        {
            --nLen;
            strProc.assign(szProc + 1, nLen);
        }
        else
        {
            strProc.assign(szProc, nLen);
        }
        int nPos = 0;
        if (bAppendix)
        {
            nPos = strProc.find_last_of(')');
            if (0 != nPos)
            {
                strProc.insert(nPos, ",:1");
            }
        }
        nPos = strProc.find_first_of('(');
        if (0 != nPos)
        {
            if (',' == strProc[nPos+1])
            {
                strProc.erase(nPos+1,1);
            }
        }
    }
    return strProc;
}
*/

//清空参数绑定相关的内存
void ODBC_SQLRESULT_NSAMESPACE::CSQLResult::FreeParamBind()
{
	std::vector<COLUM_ARR_INFO>::iterator it = m_vColumArrInfo.begin();
	for (it; it != m_vColumArrInfo.end(); ++it)
	{
		if (NULL != it->pIndArr)
		{
			delete it->pIndArr;
			it->pIndArr = NULL;
		}
		if (NULL != it->pValueArr)
		{
			delete it->pValueArr;
			it->pValueArr = NULL;
		}
	}
	m_vColumArrInfo.clear();
}

//调整列信息数组大
void ODBC_SQLRESULT_NSAMESPACE::CSQLResult::ResizeColumArr(int nParamIndex)
{
	if (nParamIndex > m_nParamNum)
	{
		int nOffSet = nParamIndex-m_nParamNum;
		m_nParamNum = nParamIndex;
#if (defined  _X64_) && (defined  LINUX)
		if (2 == m_nDbType)//如果为oracle
		{
			for (int i = 0; i < nOffSet; ++i)
			{
				COLUM_ARR_INFO ColumInfo;
				ColumInfo.pIndArr = new long[m_nMaxIterations];
				memset(ColumInfo.pIndArr, 0, sizeof(long)*m_nMaxIterations);
				m_vColumArrInfo.push_back(ColumInfo);
			}
		}
		else
		{
			for (int i = 0; i < nOffSet; ++i)
			{
				COLUM_ARR_INFO ColumInfo;
				ColumInfo.pIndArr = new SQLLEN[m_nMaxIterations];
				memset(ColumInfo.pIndArr, 0, sizeof(SQLLEN)*m_nMaxIterations);
				m_vColumArrInfo.push_back(ColumInfo);
			}
		}
#else
		for (int i = 0; i < nOffSet; ++i)
			{
				COLUM_ARR_INFO ColumInfo;
				ColumInfo.pIndArr = new SQLLEN[m_nMaxIterations];
				memset(ColumInfo.pIndArr, 0, sizeof(SQLLEN)*m_nMaxIterations);
				m_vColumArrInfo.push_back(ColumInfo);
			}
#endif
	}
}

//重置参数定相关的内存中的内容
void ODBC_SQLRESULT_NSAMESPACE::CSQLResult::ResetParamBind(bool bClearMem)
{
	std::vector<COLUM_ARR_INFO>::iterator it = m_vColumArrInfo.begin();
	for (it; it != m_vColumArrInfo.end(); ++it)
	{
		if (bClearMem)
		{
			//清空内容
			if (it->pValueArr) memset(it->pValueArr, 0, it->nMemSize);
			it->sql_c_type = 0;			
			it->sql_type = 0;			
			it->cbColDef = 0;				
		}
		else
		{
			//清空内容
			if (it->pValueArr) memset(it->pValueArr, 0, it->nMemSize);
		}
		it->nArrSize = 0;
	}
}

//1.设置SQL语句(同时申请stmt)
void ODBC_SQLRESULT_NSAMESPACE::CSQLResult::SetSQL(const void* szSql, bool bWchar)
{
	//检查数据库是否已打开
    ThrowExceptionIfNotOpen();

    try
    {
        m_hStmt = NULL;
        /*Alloc Statement Handle*/
        SQLRETURN nSqlRet = SQLAllocHandle(SQL_HANDLE_STMT, m_hCon, &m_hStmt);
        if (nSqlRet != SQL_SUCCESS && nSqlRet != SQL_SUCCESS_WITH_INFO)
        {
            HandleError(SQL_HANDLE_DBC, m_hCon);
        }
		else
		{
			/*Prepare SQL*/
			if (bWchar)
			{
				nSqlRet = SQLPrepareW(m_hStmt, (SQLWCHAR*)szSql, SQL_NTS);
			}
			else
			{
				nSqlRet = SQLPrepare(m_hStmt, (SQLCHAR*)szSql, SQL_NTS);
			}

			if (nSqlRet != SQL_SUCCESS && nSqlRet != SQL_SUCCESS_WITH_INFO)
			{
				HandleError(SQL_HANDLE_STMT, m_hStmt);
			}
		}
		//设置列绑定
		SQLSetStmtAttr(m_hStmt, SQL_ATTR_PARAM_BIND_TYPE, (SQLPOINTER)SQL_PARAM_BIND_BY_COLUMN, 0);
    }
    catch (exception& e)
    {
        ThrowException(0, e.what());
    }
}

//2.设置最大迭代次数(请如实填写，尽量不要比实际的大，因为这个操作会预先分配内存)
void ODBC_SQLRESULT_NSAMESPACE::CSQLResult::SetMaxIterations(unsigned int nMaxIterations)
{
	//检查数据库是否已打开
    ThrowExceptionIfNotOpen();

	try
    {	
		const int nIterations = m_nMaxIterations;
		//若新的迭代值大于之前，则需要重新分配一些内存空间
		if (nIterations > 0 && nMaxIterations > nIterations)
		{
			m_nMaxIterations = nMaxIterations;
			std::vector<COLUM_ARR_INFO>::iterator it = m_vColumArrInfo.begin();
			for (it; it != m_vColumArrInfo.end(); ++it)
			{
				if (NULL != it->pIndArr)
				{
#if (defined  _X64_) && (defined  LINUX)
					if (2 == m_nDbType)//oracle
					{
						delete it->pIndArr;
						it->pIndArr = new long[m_nMaxIterations];
						memset(it->pIndArr, 0, sizeof(long)*m_nMaxIterations);
					}
					else
					{
						delete it->pIndArr;
						it->pIndArr = new SQLLEN[m_nMaxIterations];
						memset(it->pIndArr, 0, sizeof(SQLLEN)*m_nMaxIterations);
					}					
#else
					delete it->pIndArr;
					it->pIndArr = new SQLLEN[m_nMaxIterations];
					memset(it->pIndArr, 0, sizeof(SQLLEN)*m_nMaxIterations);
#endif

				}

				if (NULL != it->pValueArr)
				{
					delete it->pValueArr;
					unsigned int nMemSize = it->cbValueMax*m_nMaxIterations;
					it->nMemSize = nMemSize;
					it->pValueArr = new SQLCHAR[nMemSize];
					memset(it->pValueArr, 0, nMemSize);
				}
			}
		}
		//第一次设置
		else if (0 == nIterations)
		{
			m_nMaxIterations = nMaxIterations;
		}
		else
		{
			//新迭代值，比之前的小，则不需处理
		}
	}
    catch (exception& e)
    {
        ThrowException(0, e.what());
    }
	/*
	try
    {	
		//设置迭代次数
		SQLSetStmtAttr(m_hStmt, SQL_ATTR_PARAMSET_SIZE, (SQLPOINTER)m_nMaxIterations, 0);

		//设置列绑定
		SQLSetStmtAttr(m_hStmt, SQL_ATTR_PARAM_BIND_TYPE, (SQLPOINTER)SQL_PARAM_BIND_BY_COLUMN, 0);
	}
    catch (exception& e)
    {
        ThrowException(0, e.what());
    }
	*/
}

/*
//3.设置列数(如实填写，不能多也不能少)
void ODBC_SQLRESULT_NSAMESPACE::CSQLResult::SetParamNum(unsigned int nParamNum)
{
	//检查数据库是否已打开
    ThrowExceptionIfNotOpen();

	try
    {		
		m_nMaxParamNum = nParamNum;
		for (int i = 0; i < m_nMaxParamNum; ++i)
		{
			COLUM_ARR_INFO columarrinfo;
			columarrinfo.pValueArr = NULL;
			columarrinfo.pIndArr = new SQLLEN[m_nMaxIterations];
			memset(columarrinfo.pIndArr, 0, sizeof(SQLLEN)*m_nMaxIterations);
			columarrinfo.cbColDef = 0;
			columarrinfo.cbValueMax = 0;
			columarrinfo.nArrSize = 0;
			m_vColumArrInfo.push_back(columarrinfo);
		}
	}
	catch (exception& e)
	{
		ThrowException(0, e.what());
	}
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
void ODBC_SQLRESULT_NSAMESPACE::CSQLResult::SetParamAttr(unsigned int nParamIndex, int sql_type, unsigned int nMaxSize, bool bTmstrToTm)
{	
	//检查数据库是否已打开
    ThrowExceptionIfNotOpen();
	
	try
    {	
		//对类型做下调整以适应各种数据库
		SQLSMALLINT sql_c_type = SQL_C_CHAR;
		SQLLEN  sql_indicator = 0;
		SQLULEN cbColDef = 0;
		SQLLEN  cbValueMax = 0;
		switch (sql_type)
		{
		case SQL_BIGINT:
			sql_type = SQL_NUMERIC;
		case SQL_NUMERIC:
		case SQL_CHAR:	
		case SQL_VARCHAR:
			sql_c_type = SQL_C_CHAR;
			sql_indicator = SQL_NTS;
			cbColDef = nMaxSize-1;
			cbValueMax = nMaxSize;
			break;
		case SQL_INTEGER:
			sql_c_type = SQL_C_LONG;
			sql_indicator = 0;
			cbColDef = 0;
			cbValueMax = sizeof(SQLINTEGER/*SQLLEN*/);
			break;
		case SQL_SMALLINT:
			sql_c_type = SQL_C_SHORT;
			sql_indicator = 0;
			cbColDef = 0;
			cbValueMax = sizeof(SQLSMALLINT);
			break;
		/*
		case SQL_TINYINT:
			sql_c_type = SQL_C_TINYINT;
			sql_indicator = 0;
			cbColDef = 0;
			cbValueMax = sizeof(SQLCHAR);
			break;
		*/
		case SQL_WVARCHAR:
			sql_c_type = SQL_C_WCHAR;
			sql_type = SQL_WVARCHAR;
			sql_indicator = SQL_NTSL;
			cbColDef = nMaxSize - 1;
			cbValueMax = nMaxSize;
			break;

		case SQL_TYPE_TIMESTAMP:
			{
				if (bTmstrToTm)
				{
					sql_c_type = SQL_C_TYPE_TIMESTAMP;
					sql_indicator = 0;
					cbColDef = 0;
					cbValueMax = sizeof(TIMESTAMP_STRUCT);
				}
				else
				{
					sql_type = SQL_VARCHAR;
					sql_c_type = SQL_C_CHAR;
					sql_indicator = SQL_NTS;
					cbColDef = nMaxSize-1;
					cbValueMax = nMaxSize;
				}
			}
			break;
		default:
			sql_c_type = SQL_C_CHAR;
			sql_indicator = SQL_NTS;
			cbColDef = nMaxSize-1;
			cbValueMax = nMaxSize;
			break;
		}
		
		//设置列属性
		int nIndex = nParamIndex-1;
		nIndex<0?nIndex=0:1;
		//动态调整参数个数
		ResizeColumArr(nParamIndex);
		m_vColumArrInfo[nIndex].cbColDef = cbColDef;
		m_vColumArrInfo[nIndex].sql_c_type = sql_c_type;
		m_vColumArrInfo[nIndex].sql_type = sql_type;
		for (int i = 0; i < m_nMaxIterations; ++i)
		{
#if (defined  _X64_) && (defined  LINUX)
			if (2 == m_nDbType)
			{
				((long*)(m_vColumArrInfo[nIndex].pIndArr))[i] = sql_indicator;
			}
			else
			{
				((SQLLEN*)(m_vColumArrInfo[nIndex].pIndArr))[i] = sql_indicator;
			}			
#else
			((SQLLEN*)(m_vColumArrInfo[nIndex].pIndArr))[i] = sql_indicator;
#endif

		}
		//预分配空间
		//1.内存空间不存在,直接重新分配
		unsigned int nMemSize = cbValueMax*m_nMaxIterations;
		if (NULL == m_vColumArrInfo[nIndex].pValueArr)
		{
			m_vColumArrInfo[nIndex].nMemSize = nMemSize;
			m_vColumArrInfo[nIndex].cbValueMax = cbValueMax;
			m_vColumArrInfo[nIndex].pValueArr = new SQLCHAR[nMemSize];
		}
		//2.内存空间存在,这时需判断空间是否充足,若不充足需重新分配
		else 
		{
			if (cbValueMax > m_vColumArrInfo[nIndex].cbValueMax)
			{
				delete m_vColumArrInfo[nIndex].pValueArr;
				m_vColumArrInfo[nIndex].nMemSize = nMemSize;
				m_vColumArrInfo[nIndex].cbValueMax = cbValueMax;
				m_vColumArrInfo[nIndex].pValueArr = new SQLCHAR[nMemSize];
			}
		}
		memset(m_vColumArrInfo[nIndex].pValueArr, 0, m_vColumArrInfo[nIndex].nMemSize);
	}
    catch (exception& e)
    {
        ThrowException(0, e.what());
    }
}

//4.设置参数的值
//paramIndex从1开始
void ODBC_SQLRESULT_NSAMESPACE::CSQLResult::SetParamValue(unsigned int nParamIndex, unsigned int nValueLen, const void* pValue)
{
	//检查数据库是否已打开
    ThrowExceptionIfNotOpen();
	
	try
    {
		int nIndex = nParamIndex-1;
		nIndex<0?nIndex=0:1;

		unsigned int nValueMax = m_vColumArrInfo[nIndex].cbValueMax;
		unsigned int nPos = ++m_vColumArrInfo[nIndex].nArrSize;
		if (nPos <= m_nMaxIterations && --nPos >= 0)
		{
			TIMESTAMP_STRUCT tm = {1900,01,01,00,00,00,000};
			if (SQL_TYPE_TIMESTAMP == m_vColumArrInfo[nIndex].sql_type)
			{
				const char* pTm = (const char*)pValue;
				if (pTm)
				{
					sscanf(pTm, "%04d-%02d-%02d %02d:%02d:%02d.%06d", &tm.year, &tm.month, &tm.day, &tm.hour, &tm.minute, &tm.second, &tm.fraction);
					pValue = &tm;
					nValueLen = nValueMax;
				}
			}
			unsigned int nOffSet = nPos*nValueMax;
			memcpy(m_vColumArrInfo[nIndex].pValueArr+nOffSet, pValue, nValueLen);
		}
	}
    catch (exception& e)
    {
        ThrowException(0, e.what());
    }
}

//5.完成绑定
void ODBC_SQLRESULT_NSAMESPACE::CSQLResult::EndParamBind()
{
	//检查数据库是否已打开
    ThrowExceptionIfNotOpen();
	
	try
    {	
		//设置本次插入的行数
		SQLSetStmtAttr(m_hStmt, SQL_ATTR_PARAMSET_SIZE, (SQLPOINTER)m_vColumArrInfo[0].nArrSize, 0);
		for (int i = 0; i < m_nParamNum; ++i)
		{
			SQLRETURN nSqlRet = SQLBindParameter(m_hStmt,
				i + 1,
				SQL_PARAM_INPUT,
				m_vColumArrInfo[i].sql_c_type,
				m_vColumArrInfo[i].sql_type,
				m_vColumArrInfo[i].cbColDef,
				0,
				m_vColumArrInfo[i].pValueArr,
				m_vColumArrInfo[i].cbValueMax,
				(SQLLEN*)m_vColumArrInfo[i].pIndArr);
			
			if (nSqlRet != SQL_SUCCESS && nSqlRet != SQL_SUCCESS_WITH_INFO)
			{
				HandleError(SQL_HANDLE_STMT, m_hStmt);
				break;
			}
		}
	}
    catch (exception& e)
    {
        ThrowException(0, e.what());
    }
}

//6.执行绑定
void ODBC_SQLRESULT_NSAMESPACE::CSQLResult::ExecuteParamBind(bool bClearMem)
{
	//检查数据库是否已打开
    ThrowExceptionIfNotOpen();
	
	try
    {
		SQLRETURN nSqlRet = SQLExecute(m_hStmt);
		if (nSqlRet != SQL_SUCCESS && nSqlRet != SQL_SUCCESS_WITH_INFO)
		{
			HandleError(SQL_HANDLE_STMT, m_hStmt);
		}
		ResetParamBind(bClearMem);
	}
    catch (exception& e)
    {
		ThrowException(0, e.what());
    }
}

//7.获取结果集
int ODBC_SQLRESULT_NSAMESPACE::CSQLResult::GetRecords()
{
	int nRet = 0;

	try
	{
		GetFields();
	}
	catch (...)
	{
		nRet = 97;
	}

	return nRet;
}

//获取下一个结果集
int ODBC_SQLRESULT_NSAMESPACE::CSQLResult::GetNextRecords()
{
	int nRet = 0;

	try
	{
		nRet = GetMoreRecords();
	}
	catch (...)
	{
		nRet = 97;
	}

	return nRet;
}

int ODBC_SQLRESULT_NSAMESPACE::CSQLResult::Query(const void* szSql, ODBC_SQLTYPES_NSAMESPACE::SQLSTRTYPE nType, bool bNeedRetSet, bool bWchar)
{
    int nRet = -1;
    if (NULL == szSql)
    {
        return nRet;
    }

    //检查数据库是否已打开
    ThrowExceptionIfNotOpen();

    try
    {
        m_hStmt = NULL;
        /*Alloc Statement Handle*/
        SQLRETURN nSqlRet = SQLAllocHandle(SQL_HANDLE_STMT, m_hCon, &m_hStmt) ;
        if (nSqlRet != SQL_SUCCESS && nSqlRet != SQL_SUCCESS_WITH_INFO)
        {
            HandleError(SQL_HANDLE_DBC, m_hCon);
            //EndFetch();
        }
        else
        {
            if (ODBC_SQLTYPES_NSAMESPACE::SQLSTRTYPE_NORMAL == nType)
            {
                nSqlRet = SQLSetStmtAttr(m_hStmt, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_DYNAMIC, SQL_IS_INTEGER);
            }
            //nRet = SQLSetConnectAttr(m_hCon, SQL_ATTR_ODBC_CURSORS, (SQLPOINTER)SQL_CUR_USE_ODBC, SQL_IS_INTEGER);
            //exec sql

			if (bWchar)
			{
				nSqlRet = SQLExecDirectW(m_hStmt, (SQLWCHAR*)szSql, SQL_NTS);
			}
			else
			{
				nSqlRet = SQLExecDirect(m_hStmt, (UCHAR*)szSql, SQL_NTS);
			}

            if (nSqlRet == SQL_ERROR || (nSqlRet != SQL_SUCCESS && nSqlRet != SQL_SUCCESS_WITH_INFO && nSqlRet!= SQL_NO_DATA))
            {
                HandleError(SQL_HANDLE_STMT, m_hStmt);
                //EndFetch();
            }
            else
            {
                if (bNeedRetSet)
                {
                    GetFields();
                }
                nRet = 0;
            }
        }
    }
    catch (exception& e)
    {
        ThrowException(0, e.what());
    }
    return nRet;
}

int ODBC_SQLRESULT_NSAMESPACE::CSQLResult::QueryEx(const void* szSql, int &nErrorCode, ODBC_SQLTYPES_NSAMESPACE::SQLSTRTYPE nType, bool bNeedRetSet, bool bWchar)
{
	nErrorCode = -1;
    int nRet = -1;
    if (NULL == szSql)
    {
        return nRet;
    }

    //检查数据库是否已打开
    ThrowExceptionIfNotOpen();

    try
    {
        m_hStmt = NULL;
        /*Alloc Statement Handle*/
        SQLRETURN nSqlRet = SQLAllocHandle(SQL_HANDLE_STMT, m_hCon, &m_hStmt) ;
        if (nSqlRet != SQL_SUCCESS && nSqlRet != SQL_SUCCESS_WITH_INFO)
        {
            HandleError(SQL_HANDLE_DBC, m_hCon);
            //EndFetch();
        }
        else
        {
            if (ODBC_SQLTYPES_NSAMESPACE::SQLSTRTYPE_NORMAL == nType)
            {
                nSqlRet = SQLSetStmtAttr(m_hStmt, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_DYNAMIC, SQL_IS_INTEGER);
            }
            //nRet = SQLSetConnectAttr(m_hCon, SQL_ATTR_ODBC_CURSORS, (SQLPOINTER)SQL_CUR_USE_ODBC, SQL_IS_INTEGER);
            //exec sql

			if (bWchar)
			{
				nSqlRet = SQLExecDirectW(m_hStmt, (SQLWCHAR*)szSql, SQL_NTS);
			}
			else
			{
				nSqlRet = SQLExecDirect(m_hStmt, (UCHAR*)szSql, SQL_NTS);
			}

            if (nSqlRet == SQL_ERROR || (nSqlRet != SQL_SUCCESS && nSqlRet != SQL_SUCCESS_WITH_INFO && nSqlRet!= SQL_NO_DATA))
            {
				nErrorCode = 97;
                HandleError(SQL_HANDLE_STMT, m_hStmt);
                //EndFetch();
            }
            else
            {
				if (bNeedRetSet)
				{
					if (nSqlRet != SQL_NO_DATA)
					{
						GetFields(bWchar);
						nErrorCode = 0;
					}
				}

				nRet = 0;
            }
        }
    }
    catch (exception& e)
	{
		nErrorCode = 99;
        ThrowException(0, e.what());
    }
    return nRet;
}

bool ODBC_SQLRESULT_NSAMESPACE::CSQLResult::Fetch(ODBC_SQLVALUE_NSAMESPACE::SQLValueArray& arrValues, bool bNeedCheckNextRecord, bool bFetchBLOB)
{
    arrValues.clear();
    bool bRet = false;
    ThrowExceptionIfNotOpen();
    ThrowExceptionIfNoResultSet();

    SQLRETURN nRet = SQLFetch(m_hStmt);
    if (nRet == SQL_NO_DATA)
    {
		//只有不需要下一个结果集才能关闭，否则会失败
		if (!bNeedCheckNextRecord)
		{
			EndFetch();
		}
    }
    else if (nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO)
    {
        HandleError(SQL_HANDLE_STMT, m_hStmt);
        //EndFetch();
    }
    else
    {
        std::vector<ODBC_SQLTYPES_NSAMESPACE::MetaData>::iterator it = m_vMetaData.begin();
        for (int nIndex = 0; it != m_vMetaData.end(); ++nIndex, ++it)
        {
            ODBC_SQLVALUE_NSAMESPACE::CSQLValue value(it->pData, it->nLen, it->nType);
            arrValues.insert(std::make_pair(nIndex+1, value));
        }
        bRet = true;
    }
    return bRet;
}

void ODBC_SQLRESULT_NSAMESPACE::CSQLResult::EndFetch()
{
    if (NULL != m_hStmt)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
        m_hStmt = NULL;
    }
    std::vector<ODBC_SQLTYPES_NSAMESPACE::MetaData>::iterator it = m_vMetaData.begin();
    for (it; it != m_vMetaData.end(); ++it)
    {
        if (NULL != it->pData)
        {
            delete []it->pData;
            it->pData = NULL;
        }
    }
    m_vMetaData.clear();
}

int ODBC_SQLRESULT_NSAMESPACE::CSQLResult::GetMoreRecords()
{
	int nRet = 0;
	ThrowExceptionIfNotOpen();
	ThrowExceptionIfNoResultSet();
	
	SQLRETURN nSqlRet = SQLMoreResults(m_hStmt);
	if (nSqlRet == SQL_NO_DATA)
	{
		nRet = -1;
		EndFetch();
	}
	else if (nSqlRet != SQL_SUCCESS && nSqlRet != SQL_SUCCESS_WITH_INFO)
	{
		nRet = 97;
		HandleError(SQL_HANDLE_STMT, m_hStmt);
	}
	

	return nRet;
}

void ODBC_SQLRESULT_NSAMESPACE::CSQLResult::GetFields(bool bWhar)
{
    m_SQLFIELD.clear();
    ThrowExceptionIfNotOpen();
    ThrowExceptionIfNoResultSet();

    SQLSMALLINT nColNum = 0;
    //get column num
    SQLRETURN nRet = SQLNumResultCols(m_hStmt, &nColNum);
    if (nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO)
    {
        HandleError(SQL_HANDLE_STMT, m_hStmt);
    }
    else
    {
        SQLCHAR ColName[SQL_MAX_ID_LENGTH+1] = {0};
        SQLSMALLINT ColNameLen = 0;
        SQLSMALLINT ColType = 0;
        SQLULEN MaxColSize = 0;//YZ...全文替换SQLUINTEGER->SQLULEN
        SQLSMALLINT DecimalDigits = 0;
        SQLLEN  Displaysize = 0;
        SQLLEN  OutLen = 0;
        ODBC_SQLTYPES_NSAMESPACE::MetaData    md;
        //bind columns
        for (SQLSMALLINT i = 1; i <= nColNum; ++i)
        {
            //get column descriptions
            SQLDescribeCol(m_hStmt,
                i,
                ColName,
                sizeof(ColName),
                &ColNameLen,
                &ColType,
                &MaxColSize,
                &DecimalDigits,
                NULL
                );

            //get display length for column
            SQLColAttribute( m_hStmt,
                i,
                SQL_DESC_DISPLAY_SIZE,
                NULL,
                0,
                NULL,
                &Displaysize
                );

            MaxColSize = MAX2(Displaysize, strlen((char*)ColName)) + 1;

#ifdef WIN32
			//WIN下不需要这样特殊处理
			bWhar = false;
#endif

			if (bWhar)
			{
				MaxColSize *= sizeof(SQLWCHAR);
			}

            char *p = new char[MaxColSize];
            memset(p, 0, MaxColSize);
            md.nLen  = MaxColSize;
			if (bWhar)
			{
				md.nType = SQL_C_WCHAR;//ColType;
			}
			else
			{
				md.nType = SQL_C_CHAR;//ColType;
			}

            md.pData = p;
            m_vMetaData.push_back(md);
            //bind columns to program vars, converting all types to CHAR

			if (bWhar)
			{
				SQLBindCol(m_hStmt,
					i,
					SQL_C_WCHAR,
					p,
					MaxColSize,
					&OutLen
					);
			}
			else
			{
				SQLBindCol(m_hStmt,
					i,
					SQL_C_CHAR,
					p,
					MaxColSize,
					&OutLen
					);
			}
			
            to_upper((char*)ColName);
            ODBC_SQLFIELD_NSAMESPACE::CSQLField field((char*)ColName, i);
            m_SQLFIELD.insert(std::make_pair((char*)ColName, field));
        }
    }
}

void ODBC_SQLRESULT_NSAMESPACE::CSQLResult::ThrowExceptionIfNoResultSet() const
{
    if(NULL == m_hStmt)
    {
        std::string strErr = "No result set,Please call Query before Fetch.";
        ThrowException(0, strErr.c_str());
    }
}

int ODBC_SQLRESULT_NSAMESPACE::CSQLResult::GetItemIndex(const char* szField)
{
    char szBuf[64+1] = {0};
    memcpy(szBuf, szField, MIN2((int)(sizeof(szBuf)-1), (int)(strlen(szField))));
    to_upper(szBuf);
    int nIndex = m_SQLFIELD[szBuf].GetIndex();
    if (nIndex < 0)
    {
        std::string strErr;
        strErr = "\"";
        strErr += szBuf;
        strErr += "\"";
        strErr += " is not in this result set.";
        ThrowException(0, strErr.c_str());
    }
    return nIndex;
}

//////////////////////////////////////////////////////////////////////////
#define DESC_LEN 51
#define ARRAY_SIZE 10
#define TM_LEN 26
#define BIGINT_LEN 21

struct PartStruct 
{
	SQLLEN    PartID;
	SQLCHAR       Desc[DESC_LEN];
	TIMESTAMP_STRUCT	      Tm;
	SQLNUMERIC    bigint[BIGINT_LEN];
	SQLLEN    PartIDInd;
	SQLLEN    DescLenOrInd;
	SQLLEN    TmInd;
	SQLLEN    BigIntInd;
};
SQLLEN arrPartID[ARRAY_SIZE];
SQLLEN arrPartIDInd[ARRAY_SIZE];

PartStruct arrPartStruct[ARRAY_SIZE];

const char *      Statement = "INSERT INTO JHBTEST (DESC) VALUES (?)";
//const char *      Statement = "INSERT INTO JHBTEST (PartId,Description,TestId) VALUES (?,?,?)";
SQLUSMALLINT   i, ParamsProcessed, ParamStatusArray[ARRAY_SIZE];
int  ODBC_SQLRESULT_NSAMESPACE::CSQLResult::Test()
{
	SQLCHAR** pArrValue = new SQLCHAR*[DESC_LEN];
	SQLLEN* pArrInd = new SQLLEN[ARRAY_SIZE];
	int i = 0;
	for (i = 0; i < ARRAY_SIZE; ++i)
	{
		pArrValue[i] = new SQLCHAR[DESC_LEN];
		memset(pArrValue[i], 0, DESC_LEN);
		sprintf((char*)pArrValue[i], "%d", i);

		pArrInd[i] = SQL_NTS;
	}
	SQLCHAR*p1 = pArrValue[0];
	SQLCHAR*p2 = p1 + DESC_LEN;
	//检查数据库是否已打开
    ThrowExceptionIfNotOpen();
	
	m_hStmt = NULL;
	/*Alloc Statement Handle*/
	SQLRETURN nSqlRet = SQLAllocHandle(SQL_HANDLE_STMT, m_hCon, &m_hStmt);
	if (nSqlRet != SQL_SUCCESS && nSqlRet != SQL_SUCCESS_WITH_INFO)
	{
		HandleError(SQL_HANDLE_DBC, m_hCon);
	}
	nSqlRet = SQLPrepare(m_hStmt, (unsigned char*)Statement, SQL_NTS);
	// Set the SQL_ATTR_PARAM_BIND_TYPE statement attribute to use
	// column-wise binding.
	//SQLLEN nSize1 = SQL_PARAM_BIND_BY_COLUMN;
	//SQLSetStmtAttr(m_hStmt, SQL_ATTR_PARAMSET_SIZE, &nSize1, 0);
	//nSqlRet = SQLSetStmtAttr(m_hStmt, SQL_ATTR_PARAMSET_SIZE, (SQLPOINTER)ARRAY_SIZE, 0);
	//nSqlRet = SQLSetStmtAttr(m_hStmt, SQL_ATTR_PARAM_BIND_TYPE, SQL_PARAM_BIND_BY_COLUMN, 0);

	nSqlRet = SQLSetStmtAttr(m_hStmt, SQL_ATTR_PARAMSET_SIZE, (SQLPOINTER)ARRAY_SIZE, 0);
	SQLSetStmtAttr(m_hStmt, SQL_ATTR_PARAM_BIND_TYPE, SQL_PARAM_BIND_BY_COLUMN, 0);
	// Specify the number of elements in each parameter array.
	//SQLLEN nSize2 = sizeof(PartStruct);
	//nSqlRet = SQLSetStmtAttr(m_hStmt, SQL_ATTR_PARAM_BIND_TYPE, (SQLPOINTER)sizeof(PartStruct), 0);
	
	// Specify an array in which to return the status of each set of
	// parameters.
	//SQLSetStmtAttr(m_hStmt, SQL_ATTR_PARAM_STATUS_PTR, ParamStatusArray, 0);
	
	// Specify an SQLULEN value in which to return the number of sets of
	// parameters processed.
	//SQLSetStmtAttr(m_hStmt, SQL_ATTR_PARAMS_PROCESSED_PTR, &ParamsProcessed, 0);
	
	// Bind the parameters in row-wise fashion.
	/*
	nSqlRet = SQLBindParameter(m_hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0,
		&arrPartStruct[0].PartID, 0, &arrPartStruct[0].PartIDInd);
	
	SQLBindParameter(m_hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, sizeof(arrPartStruct[0].Desc)-1, 0,
		arrPartStruct[0].Desc, sizeof(arrPartStruct[0].Desc), &arrPartStruct[0].DescLenOrInd);

	SQLBindParameter(m_hStmt, 3, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, sizeof(arrPartStruct[0].Tm), 0,
		&arrPartStruct[0].Tm, sizeof(arrPartStruct[0].Tm), &arrPartStruct[0].TmInd);

	SQLBindParameter(m_hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_NUMERIC, sizeof(arrPartStruct[0].bigint)-1, 0,
		arrPartStruct[0].bigint, sizeof(arrPartStruct[0].bigint), &arrPartStruct[0].BigIntInd);
	*/
	/*	
	SQLBindParameter(m_hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_C_CHAR, sizeof(arrPartStruct[0].Tm)-1, 0,
		arrPartStruct[0].Tm, sizeof(arrPartStruct[0].Tm), &arrPartStruct[0].TmInd);
	SQLBindParameter(m_hStmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0,
		&arrPartStruct[0].bigint, 0, &arrPartStruct[0].BigIntInd);
	*/

	//SQLBindParameter(m_hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0,
	//	arrPartID, 0, arrPartIDInd);
	SQLBindParameter(m_hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, DESC_LEN, 0,
		pArrValue[0], DESC_LEN, pArrInd);
	
	// Set part ID, description, and price.
	/*for (int i = 0; i < ARRAY_SIZE; ++i) 
	{
		arrPartID[i] = 1000;
		arrPartIDInd[i] = 0;

		
		arrPartStruct[i].PartID = i;
		arrPartStruct[i].PartIDInd = 0;
		
		memset(arrPartStruct[i].Desc, 0, sizeof(arrPartStruct[i].Desc));
		sprintf((char*)arrPartStruct[i].Desc, "buf=%d", i);
		arrPartStruct[i].DescLenOrInd = SQL_NTS;
	
		SYSTEMTIME t;
		::GetLocalTime(&t);
		arrPartStruct[i].TmInd = sizeof(arrPartStruct[i].Tm);
		arrPartStruct[i].Tm.year = t.wYear;
		arrPartStruct[i].Tm.month = t.wMonth;
		arrPartStruct[i].Tm.day = t.wDay;
		arrPartStruct[i].Tm.hour = t.wHour;
		arrPartStruct[i].Tm.minute = t.wMinute;
		arrPartStruct[i].Tm.second = t.wSecond;
		arrPartStruct[i].Tm.fraction = t.wMilliseconds;

		sprintf((char*)arrPartStruct[i].bigint, "%I64d", 123456789045646464);
		arrPartStruct[i].BigIntInd = SQL_NTS;
		
		//arrPartStruct[i].bigint = 1234567890;
		//arrPartStruct[i].BigIntInd = 0;
		

		//arrPartID[i] = i;
		//memset(PartArray[i].Desc, 0, sizeof(PartArray[i].Desc));
		//sprintf((char*)PartArray[i].Desc, "buf=%d", i);
		//arrPartIDInd[i] = 0;
		//PartArray[i].DescLenOrInd = SQL_NTS;
	}
	*/
	// Execute the statement.
	//nSqlRet = SQLExecDirect(m_hStmt, (unsigned char*)Statement, SQL_NTS);
	//nSqlRet = SQLParamOptions(m_hStmt, ARRAY_SIZE, NULL);
	nSqlRet = SQLExecute(m_hStmt);
	if (nSqlRet != SQL_SUCCESS)
	{
		SQLCHAR         SqlState[SQL_SQLSTATE_SIZE+1] = {0};
		SQLCHAR         ErMsg[SQL_MAX_MESSAGE_LENGTH+1] = {0};
		SQLINTEGER      NativeError;
		SQLSMALLINT     i = 1;
		SQLSMALLINT     MsgLen = 0;
		SQLRETURN nSqlRet = SQLGetDiagRec(SQL_HANDLE_DBC, m_hCon, i, SqlState, &NativeError, ErMsg, sizeof(ErMsg), &MsgLen);
	}

	// Check to see which sets of parameters were processed successfully.
	for (i = 0; i < ParamsProcessed; i++) 
	{
		printf("Parameter Set  Status\n");
		printf("-------------  -------------\n");
		switch (ParamStatusArray[i]) 
		{
		case SQL_PARAM_SUCCESS:
		case SQL_PARAM_SUCCESS_WITH_INFO:
			printf("%13d  Success\n", i);
			break;
			
		case SQL_PARAM_ERROR:
			printf("%13d  Error\n", i);
			break;
			
		case SQL_PARAM_UNUSED:
			printf("%13d  Not processed\n", i);
			break;
			
		case SQL_PARAM_DIAG_UNAVAILABLE:
			printf("%13d  Unknown\n", i);
			break;
			
		}
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////