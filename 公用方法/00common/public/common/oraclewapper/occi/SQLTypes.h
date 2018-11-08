#ifndef __ORACLE_SQLTYPES_H__
#define __ORACLE_SQLTYPES_H__

#pragma warning (disable:4103)
#pragma warning (disable:4786)
#include "occi.h"
#include <map>
#include <string.h>
#include <string>
#include <assert.h>
#include <stdlib.h>
#include "stdio.h"
#include <iostream>

#if !((defined  MW_WINDOWS) || (defined  WIN32))

#ifndef __int64
#define __int64 long long
#endif

#ifndef INT64
#define INT64 __int64
#endif

#endif

using namespace std;
using namespace oracle::occi;

#define ORACLE_SQLTYPES_NSAMESPACE           oracle_sqltypes_namespace
namespace ORACLE_SQLTYPES_NSAMESPACE {

typedef Connection*         ConnectionPtr;
typedef ResultSet*          RecordSetPtr;
typedef Statement*          StatementPtr;
typedef Environment*        EnvironmentPtr;
#define ORACLE_SQLVALUE_NSAMESPACE          oracle_sqlvalue_namespace
#define ORACLE_SQLRESULT_NSAMESPACE         oracle_sqlresult_namespace
#define ORACLE_SQLFIELD_NSAMESPACE          oracle_sqlfield_namespace
#define ORACLE_SQLEXCEPTION_NSAMESPACE      oracle_sqlexception_namespace
#define ORACLE_SQLCONNECTION_NSAMESPACE     oracle_sqlconnection_namespace
#define ORACLE_SQLBASE_NSAMESPACE           oracle_sqlbase_namespace

//SQL”Ôæ‰¿‡–Õ
enum SQLSTRTYPE
{
    SQLSTRTYPE_NORMAL=0,    //SELECT INSERT DELETE UPDATE
    SQLSTRTYPE_PROC,        //CALL PROC();
    SQLSTRTYPE_UNKNOWN
};
}

#endif
