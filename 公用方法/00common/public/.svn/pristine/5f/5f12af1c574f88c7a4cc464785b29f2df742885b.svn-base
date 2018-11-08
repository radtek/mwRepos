#ifndef __ORACLE_SQLCONNECTION_H__
#define __ORACLE_SQLCONNECTION_H__

#include "SQLBase.h"

namespace ORACLE_SQLCONNECTION_NSAMESPACE {
class CSQLConnection : public ORACLE_SQLBASE_NSAMESPACE::CSQLBase
{
public:
    CSQLConnection();
    virtual ~CSQLConnection();
public:
    bool Open(const char* szHost,
            const char* szDatabase,
            const char* szUser,
            const char* szPassword,
            unsigned int nPort = 1521
            );

    void Close();

    bool IsOpen();

    operator ORACLE_SQLTYPES_NSAMESPACE::ConnectionPtr() const
    {
        return m_pCon;
    }
    ORACLE_SQLTYPES_NSAMESPACE::EnvironmentPtr GetEnvironment() const
    {
        return m_pEnv;
    }
private:
    ORACLE_SQLTYPES_NSAMESPACE::EnvironmentPtr m_pEnv;
};
}

#endif
