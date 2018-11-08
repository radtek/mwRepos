#ifndef __DB2_SQLEXCEPTION_H__
#define __DB2_SQLEXCEPTION_H__

#include "SQLTypes.h"

using namespace DB2_SQLTYPES_NSAMESPACE;

namespace DB2_SQLEXCEPTION_NSAMESPACE {
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
