#ifndef __ODBC_SQLFIELD_H__
#define __ODBC_SQLFIELD_H__

#include "SQLTypes.h"

using namespace ODBC_SQLTYPES_NSAMESPACE;

namespace ODBC_SQLFIELD_NSAMESPACE {
class CSQLField
{
public:
    CSQLField();
    CSQLField(const std::string& strFieldName, int nIndex);
    CSQLField(const CSQLField& rField);
protected:
    int  m_nIndex;
    std::string m_strName;
public:
    CSQLField& operator=(const CSQLField& rField);
    inline int  GetIndex() const
    {
        return m_nIndex;
    }
protected:
    void Copy(const CSQLField& rField);
};

typedef std::map<std::string, CSQLField> SQLFieldArray;
}

#endif
