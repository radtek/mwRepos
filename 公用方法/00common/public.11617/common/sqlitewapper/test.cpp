#include <stdio.h>
#include "stdlib.h"
#include "SQLConnection.h"
#include "SQLException.h"
#include "SQLResult.h"
#include "SQLValue.h"

using namespace SQLITE_SQLEXCEPTION_NSAMESPACE;
using namespace SQLITE_SQLCONNECTION_NSAMESPACE;
using namespace SQLITE_SQLRESULT_NSAMESPACE;
using namespace SQLITE_SQLVALUE_NSAMESPACE;

int main()
{
    CSQLConnection sqlconn;
    try
    {
        if (sqlconn.Open("./mysqlite", "db2empsvr", "db2jhb", "123456"))
        {
            CSQLResult rs(sqlconn);
            //rs.Query("select * from userfee where upper(UserId)=upper('pre001')", SQLSTRTYPE_NORMAL);
			const char *pSQL = "select * from uinfo";
//			const char *pSQL = "insert into uinfo values(106,'ss','s',28)";
//			const char *pSQL = "update uinfo set age=25,name='aa' where id=100";
//			const char *pSQL = "delete from uinfo where id=101";

			if (0)
			{
				rs.Query(pSQL, SQLSTRTYPE_NORMAL, false);
			}
			else
			{
				rs.Query(pSQL, SQLSTRTYPE_NORMAL, true);
				SQLValueArray arryValue;
				int nIndex;
				while (rs.Fetch(arryValue))
				{
					nIndex = rs.GetItemIndex("ID");
					cout << "ID" << " " << arryValue[nIndex].GetData() << endl;
					nIndex = rs.GetItemIndex("name");
					cout << "name" << " " << arryValue[nIndex].GetData() << endl; 
					nIndex = rs.GetItemIndex("sex");
					cout << "sex" << " " << arryValue[nIndex].GetData() << endl; 
					nIndex = rs.GetItemIndex("age");
					cout << "age" << " " << arryValue[nIndex].GetData() << endl; 
				}
			}
           
        }
    }
    catch (CSQLException e)
    {
        printf("%s,%s", e.ErrorMessage(), e.Description());
        //CLogMgr::GetInstance().WriteLog(e.Description());
    }

    return 0;
}


