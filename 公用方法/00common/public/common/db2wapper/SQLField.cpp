#include "SQLField.h"

DB2_SQLFIELD_NSAMESPACE::CSQLField::CSQLField(const std::string& strFieldName, int nIndex)
{
    m_strName = strFieldName;
    m_nIndex  = nIndex;
}

DB2_SQLFIELD_NSAMESPACE::CSQLField::CSQLField()
{

}

DB2_SQLFIELD_NSAMESPACE::CSQLField::CSQLField(const DB2_SQLFIELD_NSAMESPACE::CSQLField& rField)
{
    Copy(rField);
}

void DB2_SQLFIELD_NSAMESPACE::CSQLField::Copy(const CSQLField& rField)
{
    m_strName = rField.m_strName;
    m_nIndex  = rField.m_nIndex;
}

DB2_SQLFIELD_NSAMESPACE::CSQLField& DB2_SQLFIELD_NSAMESPACE::CSQLField::operator=(const DB2_SQLFIELD_NSAMESPACE::CSQLField& rField)
{
    if(this != &rField)
    {
        Copy(rField);
    }
    return *this;
}
