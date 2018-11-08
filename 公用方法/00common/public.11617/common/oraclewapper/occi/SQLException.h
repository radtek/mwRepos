#ifndef __ORACLE_SQLEXCEPTION_H__
#define __ORACLE_SQLEXCEPTION_H__

#include "SQLTypes.h"

namespace ORACLE_SQLEXCEPTION_NSAMESPACE {
class CSQLException
{
public:
   CSQLException(int nErCode, const char* szErr);
   virtual ~CSQLException();
protected:
    int m_nErCode;
    std::string m_ErCode;
    std::string m_ErMsg;
public:
    const char* ErrorMessage();
    const char* Description();
};
}

#endif
