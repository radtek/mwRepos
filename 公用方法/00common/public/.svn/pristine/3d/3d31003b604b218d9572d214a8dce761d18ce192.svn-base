#ifndef __ODBC_SQLEXCEPTION_H__
#define __ODBC_SQLEXCEPTION_H__

#include "SQLTypes.h"

using namespace ODBC_SQLTYPES_NSAMESPACE;

namespace ODBC_SQLEXCEPTION_NSAMESPACE {
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
