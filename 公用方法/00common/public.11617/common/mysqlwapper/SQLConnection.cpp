#include "SQLConnection.h"
#include "SQLException.h"

MYSQL_SQLCONNECTION_NSAMESPACE::CSQLConnection::CSQLConnection()
   : MYSQL_SQLBASE_NSAMESPACE::CSQLBase(NULL)
{
}

MYSQL_SQLCONNECTION_NSAMESPACE::CSQLConnection::~CSQLConnection()
{
   Close();
}

bool MYSQL_SQLCONNECTION_NSAMESPACE::CSQLConnection::Open(const char* szHost, const char* szDatabase,
                          const char* szUser, const char* szPassword,
                          unsigned int nPort)
{
    bool bRet = true;
    Close();
    m_hConnect = mysql_init(NULL);
    if (NULL == m_hConnect)
    {
        ThrowExceptionIfNotOpen();
        bRet = false;
    }
    else
    {
        //mysql_options(m_hConnect, 3, NULL);

        MYSQL_SQLTYPES_NSAMESPACE::ConnectionPtr hConnect = mysql_real_connect(m_hConnect, szHost,
            szUser, szPassword,
            szDatabase,
            nPort, NULL, CLIENT_MULTI_RESULTS|CLIENT_MULTI_STATEMENTS);

        // if mysql_real_connect failed then returns NULL
        // but first argument, m_hConnect, which was initialized by
        // mysql_init can be further used to get the error
        if(NULL == hConnect)
        {
            // m_hConnect is used to get the error
            ThrowException(mysql_errno(m_hConnect), mysql_error(m_hConnect));
            bRet = false;
        }
        else
        {
            mysql_query(m_hConnect, "set names 'gbk'");
        }
    }
    return bRet;
}

void MYSQL_SQLCONNECTION_NSAMESPACE::CSQLConnection::Close()
{
   if(NULL != m_hConnect)
   {
      mysql_close(m_hConnect);
      m_hConnect = NULL;
   }
}

bool MYSQL_SQLCONNECTION_NSAMESPACE::CSQLConnection::IsOpen()
{
   return (NULL != m_hConnect);
}

