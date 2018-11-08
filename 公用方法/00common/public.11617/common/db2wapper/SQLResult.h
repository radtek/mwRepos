/******************************************************************************
   File:        SQLResult.h
   Contents:    CSQLResult class definition
   Description: Used to perform SQL queries and fetch the result set rows
******************************************************************************/

#ifndef __DB2_SQLRESULT_H__
#define __DB2_SQLRESULT_H__

#include "SQLBase.h"
#include "SQLValue.h"
#include "SQLField.h"

namespace DB2_SQLRESULT_NSAMESPACE {
//using namespace DB2_SQLTYPES_NSAMESPACE;
class CSQLResult : public DB2_SQLBASE_NSAMESPACE::CSQLBase
{
public:
   CSQLResult(DB2_SQLTYPES_NSAMESPACE::ConnectionPtr pCon);
   void Init(DB2_SQLTYPES_NSAMESPACE::ConnectionPtr pCon,DB2_SQLTYPES_NSAMESPACE::EnvironmentPtr pEnv);
   CSQLResult();
   virtual ~CSQLResult();
protected:
   DB2_SQLFIELD_NSAMESPACE::SQLFieldArray m_SQLFIELD;
   DB2_SQLTYPES_NSAMESPACE::ConnectionPtr m_hStmt;
   std::vector<DB2_SQLTYPES_NSAMESPACE::MetaData> m_vMetaData;
public:
   bool IsEmpty()
   {
       return NULL==m_hStmt;
   };
   int  GetItemIndex(const char* szField);
   //nType 0:表示szSql是普通SQL语句(select,insert,update,delete) 1:表示szSql是存储过程
   //nNeedRetSet表示szSql是否需要返回记录集
   int  Query(const char* szSql, DB2_SQLTYPES_NSAMESPACE::SQLSTRTYPE nType=DB2_SQLTYPES_NSAMESPACE::SQLSTRTYPE_NORMAL, bool bNeedRetSet=true);
   bool Fetch(DB2_SQLVALUE_NSAMESPACE::SQLValueArray& arrValues, bool bFetchBLOB = true);
   void EndFetch();
protected:
   void GetFields();
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
