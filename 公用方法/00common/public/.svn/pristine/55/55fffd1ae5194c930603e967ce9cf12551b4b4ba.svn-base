#include "SQLException.h"
#include "SQLBase.h"

void ORACLE_SQLBASE_NSAMESPACE::CSQLBase::ThrowException(int nErCode, const char* szErr) const
{
    ORACLE_SQLEXCEPTION_NSAMESPACE::CSQLException e(nErCode, szErr);
    throw e;
}

void ORACLE_SQLBASE_NSAMESPACE::CSQLBase::ThrowExceptionIfNotOpen() const
{
    if(NULL == m_pCon)
    {
        std::string strErr = "createConnection error.";
        ORACLE_SQLEXCEPTION_NSAMESPACE::CSQLException e(0, strErr.c_str());
        throw e;
    }
}
