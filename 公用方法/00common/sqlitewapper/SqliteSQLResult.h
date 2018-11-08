/******************************************************************************
   File:        SQLResult.h
   Contents:    CSQLResult class definition
   Description: Used to perform SQL queries and fetch the result set rows
******************************************************************************/

#ifndef __SQLITE_SQLRESULT_H__
#define __SQLITE_SQLRESULT_H__

#include "SqliteSQLBase.h"
#include "SqliteSQLValue.h"
#include "SqliteSQLField.h"

namespace SQLITE_SQLRESULT_NSAMESPACE {
//using namespace SQLITE_SQLTYPES_NSAMESPACE;
class CSQLResult : public SQLITE_SQLBASE_NSAMESPACE::CSQLBase
{
public:
   CSQLResult(SQLITE_SQLTYPES_NSAMESPACE::ConnectionPtr pCon);
   void Init(SQLITE_SQLTYPES_NSAMESPACE::ConnectionPtr pCon,SQLITE_SQLTYPES_NSAMESPACE::EnvironmentPtr pEnv);
   CSQLResult();
   virtual ~CSQLResult();
protected:
   SQLITE_SQLFIELD_NSAMESPACE::SQLFieldArray m_SQLFIELD;
//   SQLITE_SQLTYPES_NSAMESPACE::ConnectionPtr m_hStmt;
   int m_nRows;
   int m_nCols;
   int m_nCurrentRow;
   char **m_pRst;
public:
   bool IsEmpty()
   {
       return NULL==m_pRst;
   };
   int  GetItemIndex(const char* szField);
   //nType 0:表示szSql是普通SQL语句(select,insert,update,delete) 1:表示szSql是存储过程
   //nNeedRetSet表示szSql是否需要返回记录集
   int  Query(const char* szSql, SQLITE_SQLTYPES_NSAMESPACE::SQLSTRTYPE nType=SQLITE_SQLTYPES_NSAMESPACE::SQLSTRTYPE_NORMAL, bool bNeedRetSet=true);
   bool Fetch(SQLITE_SQLVALUE_NSAMESPACE::SQLValueArray& arrValues, bool bFetchBLOB = false);
   void EndFetch();
   int  Excute(char *szSql,sqlite3_callback rowCallBack=0, void* pInvoke=0);   
protected:
   void GetFields();
   //int  Excute(char *szSql);
   int  Select(char *szSql);
   void ThrowExceptionIfNoResultSet() const;

    int MIN2(int a, int b)
    {
        return a>b ? b : a;
    }
    int MAX2(int a, int b)
    {
        return a>b ? a : b;
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
};
}
#endif
