#include "stdafx.h"
#include "SQLField.h"

ODBC_SQLFIELD_NSAMESPACE::CSQLField::CSQLField(const std::string& strFieldName, int nIndex)
{
    m_strName = strFieldName;
    m_nIndex  = nIndex;
}

ODBC_SQLFIELD_NSAMESPACE::CSQLField::CSQLField()
{

}

ODBC_SQLFIELD_NSAMESPACE::CSQLField::CSQLField(const ODBC_SQLFIELD_NSAMESPACE::CSQLField& rField)
{
    Copy(rField);
}

void ODBC_SQLFIELD_NSAMESPACE::CSQLField::Copy(const CSQLField& rField)
{
    m_strName = rField.m_strName;
    m_nIndex  = rField.m_nIndex;
}

ODBC_SQLFIELD_NSAMESPACE::CSQLField& ODBC_SQLFIELD_NSAMESPACE::CSQLField::operator=(const ODBC_SQLFIELD_NSAMESPACE::CSQLField& rField)
{
    if(this != &rField)
    {
        Copy(rField);
    }
    return *this;
}
