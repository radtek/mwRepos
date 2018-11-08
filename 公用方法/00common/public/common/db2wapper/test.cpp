#include <stdio.h>
#include "stdlib.h"
#include "SQLConnection.h"
#include "SQLException.h"
#include "SQLResult.h"

using namespace DB2_SQLEXCEPTION_NSAMESPACE;
using namespace DB2_SQLCONNECTION_NSAMESPACE;
using namespace DB2_SQLRESULT_NSAMESPACE;
using namespace DB2_SQLVALUE_NSAMESPACE;

int main()
{
    CSQLConnection sqlconn;
    try
    {
        if (sqlconn.Open("192.169.1.130", "db2empsvr", "db2jhb", "123456"))
        {
            CSQLResult rs(sqlconn);
            //rs.Query("select * from userfee where upper(UserId)=upper('pre001')", SQLSTRTYPE_NORMAL);
            rs.Query("select * MT_TASK where 0=1", SQLSTRTYPE_NORMAL, false);
        }
    }
    catch (CSQLException e)
    {
        printf("%s,%s", e.ErrorMessage(), e.Description());
        //CLogMgr::GetInstance().WriteLog(e.Description());
    }

    return 0;
}


