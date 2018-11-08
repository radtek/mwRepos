#ifndef __DB2_SQLTYPES_H__
#define __DB2_SQLTYPES_H__

#pragma warning (disable:4103)
#pragma warning (disable:4786)
/*
#include <sqlcli1.h>
#include <sqlcli.h>
#include <sqlsystm.h>
*/

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
#endif

#define DB2_SQLTYPES_NSAMESPACE           db2_sqltypes_namespace
namespace DB2_SQLTYPES_NSAMESPACE {

typedef SQLHANDLE         ConnectionPtr;
typedef int               Type;
//typedef ResultSet*          RecordSetPtr;
//typedef Statement*          StatementPtr;
typedef void*        EnvironmentPtr;

#define DB2_SQLVALUE_NSAMESPACE          db2_sqlvalue_namespace
#define DB2_SQLRESULT_NSAMESPACE         db2_sqlresult_namespace
#define DB2_SQLFIELD_NSAMESPACE          db2_sqlfield_namespace
#define DB2_SQLEXCEPTION_NSAMESPACE      db2_sqlexception_namespace
#define DB2_SQLCONNECTION_NSAMESPACE     db2_sqlconnection_namespace
#define DB2_SQLBASE_NSAMESPACE           db2_sqlbase_namespace
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
