#ifndef __SQLITE_SQLEXCEPTION_H__
#define __SQLITE_SQLEXCEPTION_H__

#include "SQLTypes.h"

using namespace SQLITE_SQLTYPES_NSAMESPACE;

namespace SQLITE_SQLEXCEPTION_NSAMESPACE {
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
