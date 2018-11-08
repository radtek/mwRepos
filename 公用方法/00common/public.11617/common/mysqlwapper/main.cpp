#include "stdio.h"
#include "stdlib.h"
#include "SQLConnection.h"
#include "SQLException.h"
#include "SQLResult.h"
using namespace std;
//#include <unistd.h>
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

#ifdef WIN32
        Sleep(1);
#else
        sleep(1);
#endif
    }
    return 0;
}
