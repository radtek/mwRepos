/******************************************************************************
   File:        SQLResult.h
   Contents:    CSQLResult class definition
   Description: Used to perform SQL queries and fetch the result set rows
******************************************************************************/

#ifndef __ODBC_SQLRESULT_H__
#define __ODBC_SQLRESULT_H__

#include "SQLBase.h"
#include "SQLValue.h"
#include "SQLField.h"

#ifndef SQLLEN
	#ifdef  LINUX
		#ifdef _X64_
//			typedef long            SQLLEN;
//			typedef unsigned long   SQLULEN;
//			typedef unsigned long   SQLSETPOSIROW;
		#else
			#define SQLLEN          SQLINTEGER
			#define SQLULEN         SQLUINTEGER
			#define SQLSETPOSIROW   SQLUSMALLINT
		#endif
	#else
		#ifdef _WIN64
			typedef INT64           SQLLEN;
			typedef UINT64          SQLULEN;
			typedef UINT64          SQLSETPOSIROW;
		#else
			#define SQLLEN          SQLINTEGER
			#define SQLULEN         SQLUINTEGER
			#define SQLSETPOSIROW   SQLUSMALLINT
		#endif
	#endif
#endif

//列信息
struct COLUM_ARR_INFO
{
	SQLCHAR*    pValueArr;				//值数组指针
//	SQLLEN* pIndArr;				//IND数组指针
	void* pIndArr;				//IND数组指针

	SQLSMALLINT sql_c_type;			//c类型	
	SQLSMALLINT sql_type;			//DB中字段类型
	SQLLEN  cbColDef;			//列定义的大小
	SQLLEN	cbValueMax;			//列值的最大值
	unsigned int nMemSize;			//pValueAddr的长度，nMemSize/cbVauleMax可得出块数，原则上等于nArrSize
	unsigned int nArrSize;			//该列数组中当前值的个数
	COLUM_ARR_INFO()
	{
		memset(this, 0, sizeof(COLUM_ARR_INFO));
	}
};

namespace ODBC_SQLRESULT_NSAMESPACE {
//using namespace ODBC_SQLTYPES_NSAMESPACE;
class CSQLResult : public ODBC_SQLBASE_NSAMESPACE::CSQLBase
{
public:
   CSQLResult(ODBC_SQLTYPES_NSAMESPACE::ConnectionPtr pCon);
   void Init(ODBC_SQLTYPES_NSAMESPACE::ConnectionPtr pCon, ODBC_SQLTYPES_NSAMESPACE::EnvironmentPtr pEnv, int nDbType = 0);
   CSQLResult();
   virtual ~CSQLResult();
protected:
   UINT m_nDbType;
   ODBC_SQLFIELD_NSAMESPACE::SQLFieldArray m_SQLFIELD;
   ODBC_SQLTYPES_NSAMESPACE::ConnectionPtr m_hStmt;
   std::vector<ODBC_SQLTYPES_NSAMESPACE::MetaData> m_vMetaData;
public:
   bool IsEmpty()
   {
       return NULL==m_hStmt;
   };
   int  GetItemIndex(const char* szField);
   //nType 0:表示szSql是普通SQL语句(select,insert,update,delete) 1:表示szSql是存储过程
   //nNeedRetSet表示szSql是否需要返回记录集
   //nErrorCode:0 正常
   int  Query(const void* szSql, ODBC_SQLTYPES_NSAMESPACE::SQLSTRTYPE nType = ODBC_SQLTYPES_NSAMESPACE::SQLSTRTYPE_NORMAL, bool bNeedRetSet = true, bool bWchar = false);

   //nType 0:表示szSql是普通SQL语句(select,insert,update,delete) 1:表示szSql是存储过程
   //nNeedRetSet表示szSql是否需要返回记录集
   //nErrorCode:0 正常， -1表示无结果集
   int  QueryEx(const void* szSql, int &nErrorCode, ODBC_SQLTYPES_NSAMESPACE::SQLSTRTYPE nType = ODBC_SQLTYPES_NSAMESPACE::SQLSTRTYPE_NORMAL, bool bNeedRetSet = true, bool bWchar = false);
   
   bool Fetch(ODBC_SQLVALUE_NSAMESPACE::SQLValueArray& arrValues, bool bNeedCheckNextRecord = false, bool bFetchBLOB = true);
   void EndFetch();
//绑定变量
public:
   //1.设置SQL语句(同时申请stmt)
	void SetSQL(const void* szSql, bool bWchar=false);
   //2.设置最大迭代次数(请如实填写，尽量不要比实际的大，因为这个操作会预先分配内存)
   void SetMaxIterations(unsigned int nMaxIterations=1);
   //3.设置参数个数(如实填写，不能多也不能少)(暂时停用)
   //void SetParamNum(unsigned int nParamNum=1);
   //3.设置每个参数的属性及对应的值
   //paramIndex从1开始,对应SQL语句中字段的顺序
   //sql_type:该字段在数据库中的类型(SQL_VARCHAR,SQL_CHAR,SQL_INTEGER,SQL_SMALLINT,SQL_BIGINT,SQL_NUMERIC,SQL_TYPE_TIMESTAMP)
   //nMaxSize的取值请使用sizeof,如果是字符串,需要将'\0'的长度算在内 
   //当nMaxSize取0时表示取sql_type类型的长度(当该类型的长度为定长时可填0)
   //bTmstrToTm:是否将时间字符串转换为时间结构体(当数据库类型为mssql和db2时不需要,oracle时需要)
   //注:BIGINT也就是INT64类型被为NUMERIC来处理，所以可以使用SQL_BIGINT和SQL_NUMERIC
   //注:mssql中的datetime,db2中的timestamp,oracle中的timestamp,请格式化为标准时间"yyyy-mm-dd hh-mm-ss.fff"或"yyyy-mm-dd hh-mm-ss"
   //并使用类型SQL_TYPE_TIMESTAMP,且传入正确的nMaxSize值,mssql/db2时bTmstrToTm=false,oracle时bTmstrToTm=true
   void SetParamAttr(unsigned int nParamIndex, int sql_type, unsigned int nMaxSize=0, bool bTmstrToTm=false);
   //4.设置参数的值
   //paramIndex从1开始
   void SetParamValue(unsigned int nParamIndex, unsigned int nValueLen, const void* pValue);
   //5.完成绑定
   void EndParamBind();
   //6.执行绑定
   void ExecuteParamBind(bool bClearMem=false);
   //7.获取结果集 //0成功，其他失败
   int GetRecords();

   //7.获取下一个结果集 //0成功，其他失败
   int GetNextRecords();

   int	Test();
protected:
   //清空参数绑定相关的内存
   void FreeParamBind();
   //重置参数定相关的内存中的内容,bClearMem=true时清空列属性值,bClearMem=false时保持现有列属性,仅清空内存中的内容
   void ResetParamBind(bool bClearMem=false);
   //调整列信息数组大
   void ResizeColumArr(int nParamIndex);
protected:
	//绑定变量最大迭代次数
	unsigned int m_nMaxIterations;
	//参数个数
	unsigned int m_nParamNum;
	//列信息
	std::vector<COLUM_ARR_INFO> m_vColumArrInfo;
protected:
   void GetFields(bool bWhar=false);
   int GetMoreRecords();

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
