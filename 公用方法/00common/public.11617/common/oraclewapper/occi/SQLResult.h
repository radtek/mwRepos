/******************************************************************************
   File:        SQLResult.h
   Contents:    CSQLResult class definition
   Description: Used to perform SQL queries and fetch the result set rows
******************************************************************************/

#ifndef __ORACLE_SQLRESULT_H__
#define __ORACLE_SQLRESULT_H__

#include "SQLBase.h"
#include "SQLValue.h"
#include "SQLField.h"

//typedef std::map<int, ORACLE_SQLVALUE_NSAMESPACE::CSQLValue> SQLValueArray;
//typedef std::map<std::string, ORACLE_SQLFIELD_NSAMESPACE::CSQLField> SQLFieldArray;

namespace ORACLE_SQLRESULT_NSAMESPACE {
class CSQLResult : public ORACLE_SQLBASE_NSAMESPACE::CSQLBase
{
public:
   CSQLResult(ORACLE_SQLTYPES_NSAMESPACE::ConnectionPtr pCon);
   void Init(ORACLE_SQLTYPES_NSAMESPACE::ConnectionPtr pCon,ORACLE_SQLTYPES_NSAMESPACE::EnvironmentPtr pEnv);
   CSQLResult();
   virtual ~CSQLResult();
protected:
   ORACLE_SQLTYPES_NSAMESPACE::RecordSetPtr m_pResultSet;
   ORACLE_SQLFIELD_NSAMESPACE::SQLFieldArray m_SQLFIELD;
   ORACLE_SQLTYPES_NSAMESPACE::StatementPtr m_pStatment;
   std::vector<oracle::occi::MetaData> m_vMetaData;
   ORACLE_SQLTYPES_NSAMESPACE::EnvironmentPtr m_pEnv;
   bool m_bGetCol;    //是否已取得所有列名和索引
public:
   bool IsEmpty()
   {
       return NULL==m_pResultSet;
   };
   int  GetItemIndex(const char* szField);
   //nType 0:表示szSql是普通SQL语句(select,insert,update,delete) 1:表示szSql是存储过程
   //nNeedRetSet表示szSql是否需要返回记录集
   int  Query(const char* szSql, ORACLE_SQLTYPES_NSAMESPACE::SQLSTRTYPE nType=ORACLE_SQLTYPES_NSAMESPACE::SQLSTRTYPE_NORMAL, bool bNeedRetSet=true);
   bool Fetch(ORACLE_SQLVALUE_NSAMESPACE::SQLValueArray& arrValues, bool bFetchBLOB = true);
   void EndFetch();
   void SetSQL(const char* szSql)
   {
       int nRet = -1;
       int nErCode = 0;
       std::string strErMsg = "";
       try
       {
           m_pStatment = m_pCon->createStatement(szSql);
           nRet = 0;
           //m_pStatment->setSQL(szSql);
       }
       catch (SQLException& e)
       {
           nErCode = e.getErrorCode();
           strErMsg = e.getMessage();
       }
       catch (exception& e)
       {
           nErCode = 0;
           strErMsg = e.what();
       }
       catch (...)
       {
           nErCode = 0;
           strErMsg = "SetSQL error!";
       }
       if (0 != nRet)
       {
           TerminateStatement();
           ThrowException(nErCode, strErMsg.c_str());
       }
   }
   void SetMaxIterations(unsigned int maxIterations)
   {
       int nRet = -1;
       int nErCode = 0;
       std::string strErMsg = "";

       try
       {
           m_pStatment->setMaxIterations(maxIterations);
           nRet = 0;
       }
       catch (SQLException& e)
       {
           nErCode = e.getErrorCode();
           strErMsg = e.getMessage();
       }
       catch (exception& e)
       {
           nErCode = 0;
           strErMsg = e.what();
       }
       catch (...)
       {
           nErCode = 0;
           strErMsg = "SetMaxIterations error!";
       }
       if (0 != nRet)
       {
           TerminateStatement();
           ThrowException(nErCode, strErMsg.c_str());
       }
   }
   void SetMaxParamSize(unsigned int paramIndex, unsigned int maxSize)
   {
       int nRet = -1;
       int nErCode = 0;
       std::string strErMsg = "";

       try
       {
           m_pStatment->setMaxParamSize(paramIndex, maxSize);
           nRet = 0;
       }
       catch (SQLException& e)
       {
           nErCode = e.getErrorCode();
           strErMsg = e.getMessage();
       }
       catch (exception& e)
       {
           nErCode = 0;
           strErMsg = e.what();
       }
       catch (...)
       {
           nErCode = 0;
           strErMsg = "setMaxParamSize error!";
       }
       if (0 != nRet)
       {
           TerminateStatement();
           ThrowException(nErCode, strErMsg.c_str());
       }
   }
   void SetString(unsigned int paramIndex, const char* str)
   {
       int nRet = -1;
       int nErCode = 0;
       std::string strErMsg = "";

       try
       {
           std::string strStr = "";
           if (NULL == str || '\0' == str[0])
           {
               strStr = " ";
           }
           else
           {
               strStr = str;
           }
           m_pStatment->setString(paramIndex, strStr);
           nRet = 0;
       }
       catch (SQLException& e)
       {
           nErCode = e.getErrorCode();
           strErMsg = e.getMessage();
       }
       catch (exception& e)
       {
           nErCode = 0;
           strErMsg = e.what();
       }
       catch (...)
       {
           nErCode = 0;
           strErMsg = "SetString error!";
       }
       if (0 != nRet)
       {
           TerminateStatement();
           ThrowException(nErCode, strErMsg.c_str());
       }
   }
   void SetTimestamp(unsigned int paramIndex, const char* szTimeStamp)
   {
       int nRet = -1;
       int nErCode = 0;
       std::string strErMsg = "";

       try
       {
           Timestamp tm;
           tm.fromText(szTimeStamp, "YYYY-MM-DD HH24:MI:SS", "", m_pEnv);
           m_pStatment->setTimestamp(paramIndex, tm);
           nRet = 0;
       }
       catch (SQLException& e)
       {
           nErCode = e.getErrorCode();
           strErMsg = e.getMessage();
       }
       catch (exception& e)
       {
           nErCode = 0;
           strErMsg = e.what();
       }
       catch (...)
       {
           nErCode = 0;
           strErMsg = "SetTimestamp error!";
       }
       if (0 != nRet)
       {
           TerminateStatement();
           ThrowException(nErCode, strErMsg.c_str());
       }
   }
   void SetInt(unsigned int paramIndex, int x)
   {
       int nRet = -1;
       int nErCode = 0;
       std::string strErMsg = "";

       try
       {
           m_pStatment->setInt(paramIndex, x);
           nRet = 0;
       }
       catch (SQLException& e)
       {
           nErCode = e.getErrorCode();
           strErMsg = e.getMessage();
       }
       catch (exception& e)
       {
           nErCode = 0;
           strErMsg = e.what();
       }
       catch (...)
       {
           nErCode = 0;
           strErMsg = "SetInt error!";
       }
       if (0 != nRet)
       {
           TerminateStatement();
           ThrowException(nErCode, strErMsg.c_str());
       }
   }
   void SetNumber(unsigned int paramIndex, int nNumber)
   {
       int nRet = -1;
       int nErCode = 0;
       std::string strErMsg = "";

       try
       {
           m_pStatment->setNumber(paramIndex, nNumber);
           nRet = 0;
       }
       catch (SQLException& e)
       {
           nErCode = e.getErrorCode();
           strErMsg = e.getMessage();
       }
       catch (exception& e)
       {
           nErCode = 0;
           strErMsg = e.what();
       }
       catch (...)
       {
           nErCode = 0;
           strErMsg = "SetNumber error!";
       }
       if (0 != nRet)
       {
           TerminateStatement();
           ThrowException(nErCode, strErMsg.c_str());
       }
   }
   void SetNumberStr(unsigned int paramIndex, const char* szNumber)
   {
       int nRet = -1;
       int nErCode = 0;
       std::string strErMsg = "";

       try
       {
           if (NULL == szNumber || '\0' == szNumber[0])
           {
               m_pStatment->setNumber(paramIndex, 0);
           }
           else
           {
               m_pStatment->setString(paramIndex, szNumber);
           }
           nRet = 0;
       }
       catch (SQLException& e)
       {
           nErCode = e.getErrorCode();
           strErMsg = e.getMessage();
       }
       catch (exception& e)
       {
           nErCode = 0;
           strErMsg = e.what();
       }
       catch (...)
       {
           nErCode = 0;
           strErMsg = "SetNumberStr error!";
       }
       if (0 != nRet)
       {
           TerminateStatement();
           ThrowException(nErCode, strErMsg.c_str());
       }
   }
   void AddIteration()
   {
       int nRet = -1;
       int nErCode = 0;
       std::string strErMsg = "";

       try
       {
           m_pStatment->addIteration();
           nRet = 0;
       }
       catch (SQLException& e)
       {
           nErCode = e.getErrorCode();
           strErMsg = e.getMessage();
       }
       catch (exception& e)
       {
           nErCode = 0;
           strErMsg = e.what();
       }
       catch (...)
       {
           nErCode = 0;
           strErMsg = "AddIteration error!";
       }
       if (0 != nRet)
       {
           TerminateStatement();
           ThrowException(nErCode, strErMsg.c_str());
       }
   }
   void TerminateStatement()
   {
       try
       {
           if (NULL != m_pStatment)
           {
               m_pCon->terminateStatement(m_pStatment);
               m_pStatment = NULL;
           }
       }
       catch (...)
       {
       }
   }
   int ExecuteUpdate()
   {
       int nRet = -1;
       int nErCode = 0;
       std::string strErMsg = "";
       try
       {
           if (NULL != m_pStatment)
           {
               m_pStatment->executeUpdate();
               m_pCon->commit();
               m_pCon->terminateStatement(m_pStatment);
               m_pStatment = NULL;
           }
           nRet = 0;
       }
       catch (SQLException& e)
       {
           nErCode = e.getErrorCode();
           strErMsg = e.getMessage();
       }
       catch (exception& e)
       {
           nErCode = 0;
           strErMsg = e.what();
       }
       catch (...)
       {
           nErCode = 0;
           strErMsg = "ExecuteUpdate error!";
       }
       if (0 != nRet)
       {
           TerminateStatement();
           ThrowException(nErCode, strErMsg.c_str());
       }
       return nRet;
   }
protected:
   void GetFields();
   void ThrowExceptionIfNoResultSet() const;
   std::string FormatSql(const char* szProc, bool bAppendix);

    int MIN2(int a, int b)
    {
        return a>b ? b : a;
    }
    //去左空格
    std::string trimleft(const char* pStr)
    {
        std::string strRet = "";
        while (pStr && *pStr != '\0')
        {
            if (*pStr != 0x20)
            {
                strRet = pStr;
                break;
            }
            ++pStr;
        }
        return strRet;
    }

    void to_upper(char *str)
    {
        int  i = 0;
        while(str[i] != '\0')
        {
            if((str[i] >= 'a') && (str[i] <= 'z'))
            {
                str[i] -= 32;
            }
            ++i;
        }
    }

    void to_upper(std::string& str)
    {
        int  i = 0;
        while(str[i] != '\0')
        {
            if((str[i] >= 'a') && (str[i] <= 'z'))
            {
                str[i] -= 32;
            }
            ++i;
        }
    }
};
}

#endif
