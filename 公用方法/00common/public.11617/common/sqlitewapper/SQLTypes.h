#ifndef __SQLITE_SQLTYPES_H__
#define __SQLITE_SQLTYPES_H__

#pragma warning (disable:4103)
#pragma warning (disable:4786)

#ifdef WIN32
#include <windows.h>
#ifndef  __SQLITE_LIB__
#define  __SQLITE_LIB__ 
//#pragma comment(lib, "sqlite3.lib")
#endif
#endif

//#include "sql.h"
//#include "sqlext.h"
//#include "sqltypes.h"
//#include "sqlucode.h"
#include "sqlite3.h"

#include <map>
#include <vector>
#include <string>
#include <assert.h>
#include <stdlib.h>
#include "stdio.h"
#include <iostream>

using namespace std;

#ifndef WIN32

#ifndef __int64
#define __int64 long long
#endif

#ifndef INT64
#define INT64 __int64
#endif

#endif

#ifndef SQL_MAX_ID_LENGTH
#define SQL_MAX_ID_LENGTH 128

#define	SQL_UNKNOWN_TYPE	0     //无类型
#define SQL_TEXT            1     //文本
#define SQL_INTEGER         2     //整数
#define SQL_REAL            3     //实数是十进制的数值
#define SQL_BLOB            4     //二进制

#endif

#define SQLITE_SQLTYPES_NSAMESPACE           sqlite_sqltypes_namespace
namespace SQLITE_SQLTYPES_NSAMESPACE {

//typedef SQLHANDLE         ConnectionPtr;
typedef sqlite3*          ConnectionPtr;
typedef int               Type;
//typedef ResultSet*          RecordSetPtr;
//typedef Statement*          StatementPtr;
typedef void*        EnvironmentPtr;

#define SQLITE_SQLVALUE_NSAMESPACE          sqlite_sqlvalue_namespace
#define SQLITE_SQLRESULT_NSAMESPACE         sqlite_sqlresult_namespace
#define SQLITE_SQLFIELD_NSAMESPACE          sqlite_sqlfield_namespace
#define SQLITE_SQLEXCEPTION_NSAMESPACE      sqlite_sqlexception_namespace
#define SQLITE_SQLCONNECTION_NSAMESPACE     sqlite_sqlconnection_namespace
#define SQLITE_SQLBASE_NSAMESPACE           sqlite_sqlbase_namespace
// struct MetaData
// {
//     char *pData;
//     int nLen;
//     int nType;
// };

//SQL语句类型
enum SQLSTRTYPE
{
    SQLSTRTYPE_NORMAL=0,    //SELECT INSERT DELETE UPDATE
    SQLSTRTYPE_PROC,        //CALL PROC();
    SQLSTRTYPE_UNKNOWN
};
}

#endif
