#ifndef __DB2_SQLCONNECTION_H__
#define __DB2_SQLCONNECTION_H__

#include "SQLBase.h"

namespace DB2_SQLCONNECTION_NSAMESPACE {
//using namespace DB2_SQLTYPES_NSAMESPACE;
class CSQLConnection : public DB2_SQLBASE_NSAMESPACE::CSQLBase
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

    operator DB2_SQLTYPES_NSAMESPACE::ConnectionPtr() const
    {
        return m_hCon;
    }
    DB2_SQLTYPES_NSAMESPACE::EnvironmentPtr GetEnvironment() const
    {
        return NULL;
    }
private:
    DB2_SQLTYPES_NSAMESPACE::ConnectionPtr m_hEnv;
    //SQLHANDLE m_hStmt;
};
}

#endif
