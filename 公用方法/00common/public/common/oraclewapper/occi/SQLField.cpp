#include "SQLField.h"

ORACLE_SQLFIELD_NSAMESPACE::CSQLField::CSQLField(const std::string& strFieldName, int nIndex)
{
    m_strName = strFieldName;
    m_nIndex  = nIndex;
}

ORACLE_SQLFIELD_NSAMESPACE::CSQLField::CSQLField()
{

}

ORACLE_SQLFIELD_NSAMESPACE::CSQLField::CSQLField(const ORACLE_SQLFIELD_NSAMESPACE::CSQLField& rField)
{
    Copy(rField);
}

void ORACLE_SQLFIELD_NSAMESPACE::CSQLField::Copy(const ORACLE_SQLFIELD_NSAMESPACE::CSQLField& rField)
{
    m_strName = rField.m_strName;
    m_nIndex  = rField.m_nIndex;
}

ORACLE_SQLFIELD_NSAMESPACE::CSQLField& ORACLE_SQLFIELD_NSAMESPACE::CSQLField::operator=(const ORACLE_SQLFIELD_NSAMESPACE::CSQLField& rField)
{
    if(this != &rField)
    {
        Copy(rField);
    }
    return *this;
}
