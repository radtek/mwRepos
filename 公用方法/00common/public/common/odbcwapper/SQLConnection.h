#ifndef __ODBC_SQLCONNECTION_H__
#define __ODBC_SQLCONNECTION_H__

#include "SQLBase.h"

namespace ODBC_SQLCONNECTION_NSAMESPACE {
//using namespace ODBC_SQLTYPES_NSAMESPACE;
class CSQLConnection : public ODBC_SQLBASE_NSAMESPACE::CSQLBase
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

    operator ODBC_SQLTYPES_NSAMESPACE::ConnectionPtr() const
    {
        return m_hCon;
    }
    ODBC_SQLTYPES_NSAMESPACE::EnvironmentPtr GetEnvironment() const
    {
        return NULL;
    }
private:
    ODBC_SQLTYPES_NSAMESPACE::ConnectionPtr m_hEnv;
    //SQLHANDLE m_hStmt;
};
}

#endif
