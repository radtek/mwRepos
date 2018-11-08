#ifndef __SQLITE_SQLCONNECTION_H__
#define __SQLITE_SQLCONNECTION_H__

#include "SqliteSQLBase.h"
#include "sqlite3.h"

namespace SQLITE_SQLCONNECTION_NSAMESPACE {
//using namespace SQLITE_SQLTYPES_NSAMESPACE;
class CSQLConnection : public SQLITE_SQLBASE_NSAMESPACE::CSQLBase
{
public:
    CSQLConnection();
    virtual ~CSQLConnection();
public:
    bool Open(const char* szHost,
		const char* szDatabase,
		const char* szUser,
		const char* szPassword,
		unsigned int nPort = 50000
		);
	
    void Close();
	
    bool IsOpen();
	
    operator SQLITE_SQLTYPES_NSAMESPACE::ConnectionPtr() const
    {
        return m_hCon;
    }
    SQLITE_SQLTYPES_NSAMESPACE::EnvironmentPtr GetEnvironment() const
    {
        return NULL;
    }
private:
    SQLITE_SQLTYPES_NSAMESPACE::ConnectionPtr m_hEnv;
	//char     m_szDbFile[256];
    //SQLHANDLE m_hStmt;
};
}

#endif
