#ifndef __ODBC_SQLTYPES_H__
#define __ODBC_SQLTYPES_H__

#pragma warning (disable:4103)
#pragma warning (disable:4786)
/*
#include <sqlcli1.h>
#include <sqlcli.h>
#include <sqlsystm.h>
*/
#if (defined  MW_WINDOWS) || (defined  WIN32)
#include <windows.h>
#endif

#include "sql.h"
#include "sqlext.h"
#include "sqltypes.h"
#include "sqlucode.h"

#include <map>
#include <vector>
#include <string>
#include <assert.h>
#include <stdlib.h>
#include "stdio.h"
#include <iostream>

using namespace std;

#if !((defined  MW_WINDOWS) || (defined  WIN32))

#ifndef __int64
#define __int64 long long
#endif

#ifndef INT64
#define INT64 __int64
#endif

#include <string.h>
#endif

#ifndef SQL_MAX_ID_LENGTH
#define SQL_MAX_ID_LENGTH 128
#endif

#define ODBC_SQLTYPES_NSAMESPACE           odbc_sqltypes_namespace
namespace ODBC_SQLTYPES_NSAMESPACE {

typedef SQLHANDLE         ConnectionPtr;
typedef int               Type;
//typedef ResultSet*          RecordSetPtr;
//typedef Statement*          StatementPtr;
typedef void*        EnvironmentPtr;

#define ODBC_SQLVALUE_NSAMESPACE          odbc_sqlvalue_namespace
#define ODBC_SQLRESULT_NSAMESPACE         odbc_sqlresult_namespace
#define ODBC_SQLFIELD_NSAMESPACE          odbc_sqlfield_namespace
#define ODBC_SQLEXCEPTION_NSAMESPACE      odbc_sqlexception_namespace
#define ODBC_SQLCONNECTION_NSAMESPACE     odbc_sqlconnection_namespace
#define ODBC_SQLBASE_NSAMESPACE           odbc_sqlbase_namespace
struct MetaData
{
    char *pData;
    int nLen;
    int nType;
};

//SQL”Ôæ‰¿‡–Õ
enum SQLSTRTYPE
{
    SQLSTRTYPE_NORMAL=0,    //SELECT INSERT DELETE UPDATE
    SQLSTRTYPE_PROC,        //CALL PROC();
    SQLSTRTYPE_UNKNOWN
};
}

#endif
