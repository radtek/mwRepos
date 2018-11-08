#include <stdio.h>
#include "stdlib.h"
#include "SQLConnection.h"
#include "SQLException.h"
#include "SQLResult.h"
#include "time.h"
#include <signal.h>
//#include "../../logmgr/LogMgr.h"

using namespace ORACLE_SQLEXCEPTION_NSAMESPACE;
using namespace ORACLE_SQLCONNECTION_NSAMESPACE;
using namespace ORACLE_SQLRESULT_NSAMESPACE;
using namespace ORACLE_SQLTYPES_NSAMESPACE;

//消息处理例程
/*
void sigroutine(int dunno)
{
    char szBuf[1024+1] = {0};
    sprintf(szBuf, "Recv system signal, signal=%d", dunno);
    //CLogMgr::GetInstance().WriteLog(szBuf, "signalno.txt");
    // 信号处理例程，其中dunno将会得到信号的值 
    switch (dunno)
    {
	case SIGINT://CTRL+C
		printf("Get a signal -- SIGINT...closeserver...\r\n");
		if (m_pBusiness) m_pBusiness->StopService();
		delete m_pBusiness;
		m_pBusiness = NULL;
		exit(0);
		break;
	case SIGBUS:
		system("clear");
		printf("Get a signal -- SIGBUS...show statsinfo...\r\n");
		m_bShowInfo?m_bShowInfo=false:m_bShowInfo=true;
		break;
	case SIGSEGV:
		sprintf(szBuf, "[%s:%d] Program received signal SIGSEGV, Segmentation fault.",
			__FILE__, __LINE__);
		printf("%s \n", szBuf);
		//CLogMgr::GetInstance().WriteLog(szBuf, "log.txt");
		if (m_pBusiness) m_pBusiness->StopService();
		delete m_pBusiness;
		m_pBusiness = NULL;
		exit(1);
		break;
    }
    return;
}
*/
void WriteLog(const char* szLog)
{	
	FILE *fp = fopen("oraLog.txt", "a+"/*"rb"*/);	
	if (NULL==fp)
	{
		return;
	}
	
	time_t tNow = time(NULL);
	struct tm *local = localtime(&tNow);
	char szTime[32] = {0};
	sprintf(szTime, "[%04d-%02d-%02d %02d:%02d:%02d] --- ", 1900+local->tm_year, 1+local->tm_mon, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);
	fputs(szTime, fp);
	fputs(szLog, fp);
	fputs("\r\n", fp);
	fflush(fp);	
	fclose(fp);
}
int main(int argc, char*argv[])
{
	//创建进程前忽略SIGCHLD
    /*
	sigset(SIGCHLD, SIG_IGN);
    pid_t pid = fork();
    if (pid != 0)
    {
		return 0;
    }
    setsid();
	*/
    WriteLog("programe open success!");
	printf("programe open success!\n");
	/*
	char params[256] = {0};
	printf("Please Input ip,dbname,uid,pwd,minute As[127.0.0.1,db1,sa,123456,5]:\n");
	scanf("%s", params);

	char *pbeg = (char*)params;
	char *pcusor = NULL;
	
	char szHost[128]={0};
	char szName[64]={0};
	char szUid[32]={0};
	char szPwd[32]={0};
	int nSecDelay = 0;
	if((pcusor=strchr(pbeg,','))&&pcusor>pbeg)
	{
		strncpy(szHost, pbeg, pcusor-pbeg);
		pbeg = pcusor+1;	
	}
	
	if((pcusor=strchr(pbeg,','))&&pcusor>pbeg)
	{
		strncpy(szName, pbeg, pcusor-pbeg);
		pbeg = pcusor+1;
	}
	
	if((pcusor=strchr(pbeg,','))&&pcusor>pbeg)
	{
		strncpy(szUid, pbeg, pcusor-pbeg);
		pbeg = pcusor+1;
	}
	
	if((pcusor=strchr(pbeg,','))&&pcusor>pbeg)
	{
		strncpy(szPwd, pbeg, pcusor-pbeg);
		pbeg = pcusor+1;
	}
	
	nSecDelay = 60*atoi(pbeg);
	nSecDelay<=0 ? nSecDelay=120:1;
	*/
	
	CSQLConnection sqlconn;
	
	int nSecDelay = 60*atoi(argv[5]);	
	char szLog[1024] = {0};
	sprintf(szLog, "current config:IP=%s,DbName=%s,Uid=%s,Pwd=%s,Seconds=%d!", argv[1], argv[2], argv[3], argv[4], nSecDelay);
	WriteLog(szLog);

	int nOpenFail=0, nOpenSucc=0, nOpenAll=0;
	
	if (sqlconn.Open(argv[1], argv[2], argv[3], argv[4]))
	{
		WriteLog("database first open success!");		        
		int nCntQuery = 0;
		while (1)
		{
			int iRet = 0;
			bool bError = false;
			try
			{
				++nCntQuery;
				sprintf(szLog, "query begin[%08d]...", nCntQuery);
				WriteLog(szLog);

				CSQLResult rs(sqlconn);
				iRet = rs.Query("select current_timestamp from dual", SQLSTRTYPE_NORMAL, false);

				sprintf(szLog, "query ok   [%08d]!!!", nCntQuery);
				WriteLog(szLog);
			}
			catch (CSQLException e)
			{
				sprintf(szLog, "query error: %s,%s!", e.ErrorMessage(), e.Description());
				WriteLog(szLog);
				bError = true;
			}
			
			if (bError)
			{
				//重连接
				++nOpenAll;
				sprintf(szLog, "reopen database:%d,[%010d]!", iRet, nOpenAll);
				WriteLog(szLog);
				while (!sqlconn.Open(argv[1], argv[2], argv[3], argv[4]))
				{
					++nOpenFail;
					++nOpenAll;
					//日志
					sprintf(szLog, "reopen database fail:[fail_%010d]/[all_%010d]!", nOpenFail, nOpenAll);
					WriteLog(szLog);
					sleep(10);
				}
				++nOpenSucc;
				sprintf(szLog, "reopen database succ:[fail_%010d]/[succ_%010d]/[all_%010d]!", nOpenFail, nOpenSucc, nOpenAll);
				WriteLog(szLog);				
			}		
			sleep(nSecDelay);
		}		
	}
	else
	{
		WriteLog("database first open fail, programe exit!");
		sleep(2);
		exit(0);
	}	

    //getchar();
    return 0;
}


