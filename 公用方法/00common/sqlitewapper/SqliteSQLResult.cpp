#include "stdafx.h"
#include "SqliteSQLException.h"
#include "SqliteSQLResult.h"
#include "SqliteSQLValue.h"
#include "SqliteSQLField.h"

SQLITE_SQLRESULT_NSAMESPACE::CSQLResult::CSQLResult(SQLITE_SQLTYPES_NSAMESPACE::ConnectionPtr pCon)
   : SQLITE_SQLBASE_NSAMESPACE::CSQLBase(pCon),m_pRst(NULL)
{

}

SQLITE_SQLRESULT_NSAMESPACE::CSQLResult::CSQLResult():m_pRst(NULL)
{
}


void SQLITE_SQLRESULT_NSAMESPACE::CSQLResult::Init(SQLITE_SQLTYPES_NSAMESPACE::ConnectionPtr pCon,SQLITE_SQLTYPES_NSAMESPACE::EnvironmentPtr pEnv)
{
    m_hCon    = pCon;
}

SQLITE_SQLRESULT_NSAMESPACE::CSQLResult::~CSQLResult()
{
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
int SQLITE_SQLRESULT_NSAMESPACE::CSQLResult::Query(const char* szSql, SQLITE_SQLTYPES_NSAMESPACE::SQLSTRTYPE nType, bool bNeedRetSet)
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
		m_pRst = NULL;
		
		if (bNeedRetSet)
		{
			nRet = Select((char *)szSql);
			if (!nRet)
			{
				GetFields();
			}
		}
		else
		{
			nRet = Excute((char *)szSql);
		}

    }
    catch (exception& e)
    {
        ThrowException(0, e.what());
    }
    return nRet;
}

int SQLITE_SQLRESULT_NSAMESPACE::CSQLResult::Excute(char *szSql,sqlite3_callback rowCallBack, void* pInvoke)
{
	int nRes = 0;
	char *szErrMsg = NULL;
    nRes = sqlite3_exec(m_hCon,szSql,rowCallBack,pInvoke, &szErrMsg);
	if (nRes)
	{
		ThrowException(nRes, szErrMsg);		
	}
	return nRes;
}

int SQLITE_SQLRESULT_NSAMESPACE::CSQLResult::Select(char *szSql)
{
	int nRes = 0;
	char *szErrMsg = {0};
	m_nCurrentRow = -1;    
	m_nRows = 0;
	m_nCols = 0;
	nRes = sqlite3_get_table(m_hCon, szSql, &m_pRst, &m_nRows, &m_nCols, &szErrMsg);
	if (nRes)
	{
		if (szErrMsg != NULL)
		{
			ThrowException(nRes, szErrMsg);	
		}
		else
		{
			ThrowException(nRes, "获取数据错误!");
		}
		nRes = -1;     
	}
	else
	{
		if (m_nRows > 0)
		{
			m_nCurrentRow = 0;			
		}
	}
	return nRes;
}

bool SQLITE_SQLRESULT_NSAMESPACE::CSQLResult::Fetch(SQLITE_SQLVALUE_NSAMESPACE::SQLValueArray& arrValues, bool bFetchBLOB)
{
    arrValues.clear();
    bool bRet = false;
    ThrowExceptionIfNotOpen();
    ThrowExceptionIfNoResultSet(); 

	++m_nCurrentRow;
	if (m_nCurrentRow > m_nRows)
	{
		EndFetch();
    }
	else
    {
		int  nRowPos = m_nCurrentRow*m_nCols;
		for (int i = 0; i < m_nCols; ++i)
		{
			if (m_pRst[nRowPos+i] != '\0')
			{
				SQLITE_SQLVALUE_NSAMESPACE::CSQLValue value(m_pRst[nRowPos+i], strlen(m_pRst[nRowPos+i]), SQL_TEXT);
				arrValues.insert(std::make_pair(i+1, value));
			}
		}
        bRet = arrValues.size()>0;
    }
    return bRet;
}

void SQLITE_SQLRESULT_NSAMESPACE::CSQLResult::EndFetch()
{
    if (NULL != m_pRst)
    {
        sqlite3_free_table(m_pRst);
        m_pRst = NULL;
    }
}

void SQLITE_SQLRESULT_NSAMESPACE::CSQLResult::GetFields()
{
    m_SQLFIELD.clear();
    ThrowExceptionIfNotOpen();
    ThrowExceptionIfNoResultSet();
	
	m_nCurrentRow = 0;
	char ColName[SQL_MAX_ID_LENGTH+1] = {0};
	for (int i = 1; i <= m_nCols; ++i)
	{
		strcpy(ColName, m_pRst[i-1]);
		to_upper(ColName);
		SQLITE_SQLFIELD_NSAMESPACE::CSQLField field((char*)ColName, i);
		m_SQLFIELD.insert(std::make_pair((char*)ColName, field));
	}

}

void SQLITE_SQLRESULT_NSAMESPACE::CSQLResult::ThrowExceptionIfNoResultSet() const
{
    if(NULL == m_pRst)
    {
        std::string strErr = "No result set,Please call Query before Fetch.";
        ThrowException(0, strErr.c_str());
    }
}

int SQLITE_SQLRESULT_NSAMESPACE::CSQLResult::GetItemIndex(const char* szField)
{
    char szBuf[SQL_MAX_ID_LENGTH+1] = {0};
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

