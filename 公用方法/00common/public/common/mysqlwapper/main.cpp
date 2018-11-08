#include "stdio.h"
#include "stdlib.h"
#include "SQLConnection.h"
#include "SQLException.h"
#include "SQLResult.h"
#include "SQLValue.h"
#include "MySqlHelper.h"

using namespace MySqlHelper;
using namespace std;
//#include <unistd.h>
/*
int main()
{

    CSQLConnection sqlconn;
    sqlconn.Open("192.169.1.130", "smsaccount", "root", "521521", 3306);
    while (1)
    {
        try
        {
            CSQLResult rs(sqlconn);
            //rs.Query("insert into testtb (testdata) values ('testdata')", false);
            rs.Query("CALL GetLoginUserInfo_B('WBS00A')", true);
            //SQLValueArray values;
            //while (rs.Fetch(values))
            {
                //printf("get values...\r\n");
            }
        }
        catch (CSQLException e)
        {
            printf("ErrorMsg:%s\r\nDescription:%s\r\n", e.ErrorMessage(), e.Description());
        }
        catch (...)
        {
            printf("other error...\r\n");
        }
        sleep(1);
    }
    while(1)
    {

#if (defined  MW_WINDOWS) || (defined  WIN32)
        Sleep(1);
#else
        sleep(1);
#endif
    }
    return 0;
}
*/


int main()
{
	CMySqlHelper mysqlhelper;
	std::string strErrMsg;
	std::string strErrDesc;
	if (mysqlhelper.CreateConnPool(strErrMsg, strErrDesc, "testpool","192.169.1.130","rms_main","mwnet","mwnet2018",3306,"SELECT 1",60,true,1,1))
	{
		for(int i = 0; i<10000000; ++i)
			{
		CMySqlRecordSet records;
		if (0 == mysqlhelper.ExecuteWithRecordSetRet(strErrMsg,strErrDesc,records,"testpool","SELECT TMPLID,USERID,ECID FROM RMS_TEMPLATE"))
		{
			size_t nNum = records.GetRecordsNum();
			records.MoveFirst();
			while (!records.bEOF())
			{
				SQLVALUE value;
				//取该字段的值,有才取,没有时不要取
				if (records.GetFieldValueFromCurrRow("TMPLID", value))
				{
					printf("tmplid=%lld\n", (__int64)value);
				}
				if (records.GetFieldValueFromCurrRow("USERID", value))
				{
					printf("USERID=%s\n", (const char*)value);
				}
				if (records.GetFieldValueFromCurrRow("ECID", value))
				{
					printf("ECID=%d\n", (int)value);
				}
				records.MoveNext();
			}
		}
		//失败原因打印
		else
		{
			printf("%s--%s\n", strErrMsg.c_str(), strErrDesc.c_str());
		}
			}
	}
	//失败原因打印
	else
	{
		printf("%s--%s\n", strErrMsg.c_str(), strErrDesc.c_str());
	}
    return 0;
}

