#include "SQLException.h"

ORACLE_SQLEXCEPTION_NSAMESPACE::CSQLException::CSQLException(int nErCode, const char* szErr)
{
    m_nErCode = nErCode;
    m_ErMsg   = szErr;
}

ORACLE_SQLEXCEPTION_NSAMESPACE::CSQLException::~CSQLException()
{

}

const char* ORACLE_SQLEXCEPTION_NSAMESPACE::CSQLException::ErrorMessage()
{
    char szBuf[16+1] = {0};
    sprintf(szBuf, "%d", m_nErCode);
    m_ErCode = szBuf;
    return m_ErCode.c_str();
}

const char* ORACLE_SQLEXCEPTION_NSAMESPACE::CSQLException::Description()
{
    return m_ErMsg.c_str();
}
