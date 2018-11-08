#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "OCIDB.h"

bool g_out = false;

void sigroutine(int dunno)
{
    /* 信号处理例程，其中dunno将会得到信号的值 */
    switch (dunno)
    {
        case 1:
            printf("Get a signal -- SIGHUP ");
            break;
        case 2:
            //printf("Get a signal -- SIGINT ");
            g_out = true;
            break;
        case 3:
            printf("Get a signal -- SIGQUIT ");
            break;
    }
    return;
}

int main()
{
    int ret;
    OCIDB odb;
    odb.SetDataParam("smsacc", "123456", "smsacc");
    ret = odb.Multiple_Conn();
    if (0 == ret)
    {
        //ret = odb.ExcuteSQL("insert into mw_test(id,name) values(3,'ccc')");
        ret = odb.UserSelect("select * from userdata");
        while(odb.UserFetch() == 0)
        {
            char stime[10] = {0};
            odb.UserGetInt("uid");
            int year,month,day,hour,minute,second;
            //odb.UserGetDate("TDATE",year,month,day,hour,minute,second);
            //sprintf(stime,"%d-%d-%d %d:%d:%d",year,month,day,hour,minute,second);

            printf("uid[%d] , userid[%s]",
                odb.UserGetInt("uid"),
                odb.UserGetString("userid")//,
                //stime
                );
        }
        odb.UserSelectFree();
        odb.Multiple_Disc();
    }

    //signal(SIGINT,sigroutine);
    while(1)
    {
        if(g_out)
        {
            return 0;
        }
        usleep(500);
    }
    return 0;
}


