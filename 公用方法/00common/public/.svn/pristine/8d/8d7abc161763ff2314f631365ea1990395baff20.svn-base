#include "stdafx.h"
#include "SQLConnection.h"
#include "SQLException.h"

SQLITE_SQLCONNECTION_NSAMESPACE::CSQLConnection::CSQLConnection()
   : SQLITE_SQLBASE_NSAMESPACE::CSQLBase(NULL),m_hEnv(NULL)//,m_hStmt(NULL)
{

}

SQLITE_SQLCONNECTION_NSAMESPACE::CSQLConnection::~CSQLConnection()
{
   Close();
}

bool SQLITE_SQLCONNECTION_NSAMESPACE::CSQLConnection::Open(const char* szHost,
                        const char* szDatabase,
                        const char* szUser,
                        const char* szPassword,
                        unsigned int nPort)
{
    bool bRet = true;
    try
    {
        Close();
		sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
        //alloc environment handle
        int nRet   = sqlite3_open(szHost, &m_hCon);
        if (nRet)
        {
            bRet = false;
			m_hCon = NULL;
        }
        else
        {
			bRet    = true;
        }
		//Shared-Cache Mode
		sqlite3_enable_shared_cache(1);
    }
    catch (exception& e)
    {
        bRet = false;
        ThrowException(0, e.what());
    }
    return bRet;
}

void SQLITE_SQLCONNECTION_NSAMESPACE::CSQLConnection::Close()
{
/*  if (NULL != m_hStmt)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
        m_hStmt = NULL;
    }
*/
    if (NULL != m_hCon)
    {
        sqlite3_close(m_hCon);
        m_hCon = NULL;
    }
}

bool SQLITE_SQLCONNECTION_NSAMESPACE::CSQLConnection::IsOpen()
{
   return (NULL != m_hCon);
}

