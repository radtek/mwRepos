#include "SQLException.h"
#include "SQLResult.h"
#include "SQLValue.h"
#include "SQLField.h"

ORACLE_SQLRESULT_NSAMESPACE::CSQLResult::CSQLResult(ORACLE_SQLTYPES_NSAMESPACE::ConnectionPtr pCon)
   : ORACLE_SQLBASE_NSAMESPACE::CSQLBase(pCon),m_pResultSet(NULL),m_pStatment(NULL),m_bGetCol(false)
{

}

ORACLE_SQLRESULT_NSAMESPACE::CSQLResult::CSQLResult():m_pResultSet(NULL),m_pStatment(NULL),m_bGetCol(false)
{
}


void ORACLE_SQLRESULT_NSAMESPACE::CSQLResult::Init(ORACLE_SQLTYPES_NSAMESPACE::ConnectionPtr pCon,ORACLE_SQLTYPES_NSAMESPACE::EnvironmentPtr pEnv)
{
    m_pCon    = pCon;
    m_bGetCol = false;
    m_pEnv    = pEnv;
}

ORACLE_SQLRESULT_NSAMESPACE::CSQLResult::~CSQLResult()
{
   EndFetch();
}

std::string ORACLE_SQLRESULT_NSAMESPACE::CSQLResult::FormatSql(const char* szProc, bool bAppendix)
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
        strProc = trimleft(strProc.c_str());
        strProc.replace(0, strlen("CALL"), "BEGIN ");
        strProc += ";END;";
    }
    return strProc;
}

int ORACLE_SQLRESULT_NSAMESPACE::CSQLResult::Query(const char* szSql, ORACLE_SQLTYPES_NSAMESPACE::SQLSTRTYPE nType, bool bNeedRetSet)
{
    int nRet = -1;
    if (NULL == szSql)
    {
        return nRet;
    }

    //检查数据库是否已打开
    ThrowExceptionIfNotOpen();

    bool bErr = false;
    int nErCode = 0;
    std::string strErMsg = "";
    m_pResultSet = NULL;
    //ORACLE_SQLTYPES_NSAMESPACE::StatementPtr pStatment  = NULL;

    try
    {
        m_pResultSet = NULL;
        m_pStatment  = NULL;

        //需要返回记录集的存储过程,需要在存储过程后面加一个参数:1
        if (ORACLE_SQLTYPES_NSAMESPACE::SQLSTRTYPE_PROC == nType)
        {
            std::string strSql = FormatSql(szSql,bNeedRetSet);
            if (bNeedRetSet)
            {
                m_pStatment = m_pCon->createStatement(strSql);
                if (NULL != m_pStatment)
                {
                    m_pStatment->setAutoCommit(true);
                    m_pStatment->registerOutParam(1, OCCICURSOR);
                    m_pStatment->executeUpdate();
                    m_pResultSet = m_pStatment->getCursor(1);
                }
            }
            else
            {
                m_pStatment = m_pCon->createStatement(strSql);
                if (NULL != m_pStatment)
                {
                    m_pStatment->setAutoCommit(true);
                    m_pStatment->executeUpdate();
                }
            }
        }
        //普通查询
        else if (ORACLE_SQLTYPES_NSAMESPACE::SQLSTRTYPE_NORMAL == nType)
        {
            m_pStatment = m_pCon->createStatement(std::string(szSql));
            if (NULL != m_pStatment)
            {
                if (bNeedRetSet)
                {
                    m_pStatment->setAutoCommit(true);
                    m_pResultSet = m_pStatment->executeQuery();
                }
                else
                {
                    m_pStatment->setAutoCommit(true);
                    m_pStatment->executeUpdate();
                }
            }
        }

        //若返回记录集，就取出返回的列名及列索引
        if (NULL != m_pResultSet)
        {
            m_bGetCol = false;
        }
        nRet = 0;
    }
    catch (SQLException& e)
    {
        bErr = true;
        nErCode = e.getErrorCode();
        strErMsg = e.getMessage();
    }
    catch (exception& e)
    {
        bErr = true;
        nErCode = 0;
        strErMsg = e.what();
    }
    catch (...)
    {
        bErr = true;
        nErCode = 0;
        strErMsg = "Query error!";
    }
    if (bErr)
    {
        ThrowException(nErCode, strErMsg.c_str());
    }
    return nRet;
}

bool ORACLE_SQLRESULT_NSAMESPACE::CSQLResult::Fetch(ORACLE_SQLVALUE_NSAMESPACE::SQLValueArray& arrValues, bool bFetchBLOB)
{
    arrValues.clear();
    bool bRet = false;
    ThrowExceptionIfNotOpen();
    //ThrowExceptionIfNoResultSet();
    if (NULL != m_pResultSet)
    {
        bool bErr = false;
        int nErCode = 0;
        std::string strErMsg = "";
        try
        {
            int nRet = m_pResultSet->next();
            //END_OF_FETCH
            if (m_pResultSet->END_OF_FETCH != nRet)
            {
                GetFields();
                //std::vector<oracle::occi::MetaData>::iterator it = m_vMetaData.begin();
                //for(int nIndex = 1; it != m_vMetaData.end(); ++nIndex, ++it)
                //{
                //    oracle::occi::Type eType   = static_cast<oracle::occi::Type>(it->getInt(oracle::occi::MetaData::ATTR_DATA_TYPE));
                //    std::string str = m_pResultSet->getString(nIndex);
                //    ORACLE_SQLVALUE_NSAMESPACE::CSQLValue value(str.c_str(), str.size(), eType);
                //   arrValues.insert(std::make_pair(nIndex, value));
                //}
                int i, nIndex;
                for(i=0, nIndex=1; i < m_vMetaData.size(); ++i, ++nIndex)
                {
                    oracle::occi::MetaData metaData = m_vMetaData[i];
                    oracle::occi::Type eType   = static_cast<oracle::occi::Type>(metaData.getInt(oracle::occi::MetaData::ATTR_DATA_TYPE));
                    std::string str = m_pResultSet->getString(nIndex);
                    ORACLE_SQLVALUE_NSAMESPACE::CSQLValue value(str.c_str(), str.size(), eType);
                   arrValues.insert(std::make_pair(nIndex, value));
                }
                bRet = true;
            }
            else
            {
                EndFetch();
            }
        }
        catch (SQLException& e)
        {
            bErr = true;
            nErCode = e.getErrorCode();
            strErMsg = e.getMessage();
            //ThrowException(e.getErrorCode(), e.getMessage().c_str());
        }
        catch (exception& e)
        {
            bErr = true;
            nErCode = 0;
            strErMsg = e.what();
            //ThrowException(0, e.what());
        }
        catch (...)
        {
            bErr = true;
            nErCode = 0;
            strErMsg = "Fetch error!";
        }
        if (bErr)
        {
            ThrowException(nErCode, strErMsg.c_str());
        }
    }
    return bRet;
}

void ORACLE_SQLRESULT_NSAMESPACE::CSQLResult::EndFetch()
{
    bool bErr = false;
    int nErCode = 0;
    std::string strErMsg = "";
    try
    {
        if (NULL != m_pStatment)
        {
            if (NULL != m_pResultSet)
            {
                m_pStatment->closeResultSet(m_pResultSet);
                m_pResultSet = NULL;
            }
            if (NULL != m_pCon)
            {
                if (NULL != m_pStatment)
                {
                    m_pCon->terminateStatement(m_pStatment);
                    m_pStatment = NULL;
                }
            }
        }
    }
    catch (SQLException& e)
    {
        bErr = true;
        nErCode = e.getErrorCode();
        strErMsg = e.getMessage();
    }
    catch (exception& e)
    {
        bErr = true;
        nErCode = 0;
        strErMsg = e.what();
    }
    catch (...)
    {
        bErr = true;
        nErCode = 0;
        strErMsg = "EndFetch error!";
    }
    if (bErr)
    {
        ThrowException(nErCode, strErMsg.c_str());
    }
}

void ORACLE_SQLRESULT_NSAMESPACE::CSQLResult::GetFields()
{
    if (m_bGetCol)
    {
        return;
    }
    m_SQLFIELD.clear();
    m_vMetaData.clear();

    ThrowExceptionIfNotOpen();
    ThrowExceptionIfNoResultSet();
    bool bErr = false;
    int nErCode = 0;
    std::string strErMsg = "";
    try
    {
        m_vMetaData  = m_pResultSet->getColumnListMetaData();
        //std::vector<oracle::occi::MetaData>::iterator it = m_vMetaData.begin();
        std::string strFieldName = "";
        //for(int nIndex = 1; it != m_vMetaData.end(); ++nIndex, ++it)
        //{
        //    strFieldName = it->getString(oracle::occi::MetaData::ATTR_NAME);
        //    ORACLE_SQLFIELD_NSAMESPACE::CSQLField field(strFieldName, nIndex);
        //    m_SQLFIELD.insert(std::make_pair(strFieldName, field));
        //}
        int i, nIndex;
        for(i=0, nIndex=1; i < m_vMetaData.size(); ++i, ++nIndex)
        {
            oracle::occi::MetaData metaData = m_vMetaData[i];
            strFieldName = metaData.getString(oracle::occi::MetaData::ATTR_NAME);
            ORACLE_SQLFIELD_NSAMESPACE::CSQLField field(strFieldName, nIndex);
            m_SQLFIELD.insert(std::make_pair(strFieldName, field));
        }

    }
    catch (SQLException& e)
    {
        bErr = true;
        nErCode = e.getErrorCode();
        strErMsg = e.getMessage();
    }
    catch (exception& e)
    {
        bErr = true;
        nErCode = 0;
        strErMsg = e.what();
    }
    catch (...)
    {
        bErr = true;
        nErCode = 0;
        strErMsg = "GetFields error!";
    }
    if (bErr)
    {
        ThrowException(nErCode, strErMsg.c_str());
    }
    m_bGetCol = true;
}

void ORACLE_SQLRESULT_NSAMESPACE::CSQLResult::ThrowExceptionIfNoResultSet() const
{
    if(NULL == m_pResultSet)
    {
        std::string strErr = "No result set,Please call Query before Fetch.";
        ThrowException(0, strErr.c_str());
    }
}

int ORACLE_SQLRESULT_NSAMESPACE::CSQLResult::GetItemIndex(const char* szField)
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

