#include "SQLConnection.h"
#include "SQLException.h"

ORACLE_SQLCONNECTION_NSAMESPACE::CSQLConnection::CSQLConnection()
   : ORACLE_SQLBASE_NSAMESPACE::CSQLBase(NULL),m_pEnv(NULL)
{

}

ORACLE_SQLCONNECTION_NSAMESPACE::CSQLConnection::~CSQLConnection()
{
   Close();
}

bool ORACLE_SQLCONNECTION_NSAMESPACE::CSQLConnection::Open(const char* szHost,
                        const char* szDatabase,
                        const char* szUser,
                        const char* szPassword,
                        unsigned int nPort)
{
    bool bRet = true;
    int nErCode = 0;
    std::string strErMsg = "";
    try
    {
        Close();
        //Begin Modify by liuhuan 2011-06-23
        //ORA-21500: internal error code, arguments: [17099], [], [], [], [], [], [], []
        //oracle驱动中对多线程的处理有点问题。
        //参考：http://hi.baidu.com/%D0%FD%D7%AA%BF%D5%D6%F1/blog/item/758ae3f9d2a24051d6887d9a.html
        //m_pEnv = Environment::createEnvironment(Environment::DEFAULT);
        m_pEnv = Environment::createEnvironment("ZHS16GBK","UTF8",oracle::occi::Environment::Mode(Environment::OBJECT|Environment::THREADED_MUTEXED));
        //End Modify by liuhuan

        if (NULL == m_pEnv)
        {
            bRet = false;
        }
        else
        {
            char szConStr[1024+1] = {0};
			sprintf(szConStr,
                    "(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=%s)(PORT=%d)))(CONNECT_DATA=(%s=%s)(SERVER=DEDICATED)))",
                    szHost, nPort, (szDatabase[0]=='/')?"SERVICE_NAME":"SID", (szDatabase[0]=='/')?(szDatabase+1):szDatabase);
            //sprintf(szConStr, "%s:%d/%s", szHost, nPort, szDatabase);
			//m_pEnv->setCacheOptSize(4*1024*1024);
			//m_pEnv->setCacheMaxSize(10);
			m_pCon = m_pEnv->createConnection(szUser, szPassword, szConStr);
            if(NULL == m_pCon)
            {
                bRet = false;
            }
        }
    }
    catch (SQLException& e)
    {
        bRet = false;
        nErCode = e.getErrorCode();
        strErMsg = e.getMessage();
        //ThrowException(e.getErrorCode(), e.getMessage().c_str());
    }
    catch (exception& e)
    {
        bRet = false;
        nErCode = 0;
        strErMsg = e.what();
        //ThrowException(0, e.what());
    }
    catch (...)
    {
        bRet = false;
        nErCode = 0;
        strErMsg = "Open error!";
    }
    if (!bRet)
    {
        ThrowException(nErCode, strErMsg.c_str());
    }
    return bRet;
}

void ORACLE_SQLCONNECTION_NSAMESPACE::CSQLConnection::Close()
{
    try
    {
        if (NULL != m_pEnv)
        {
            if (NULL != m_pCon)
            {
                m_pEnv->terminateConnection(m_pCon);
                m_pCon = NULL;
            }
            Environment::terminateEnvironment(m_pEnv);
            m_pEnv = NULL;
        }
    }
    catch (...)
    {
    }
}

bool ORACLE_SQLCONNECTION_NSAMESPACE::CSQLConnection::IsOpen()
{
   return (NULL != m_pCon);
}

