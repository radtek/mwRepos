#include "stdafx.h"
#include "SqliteSQLField.h"

SQLITE_SQLFIELD_NSAMESPACE::CSQLField::CSQLField(const std::string& strFieldName, int nIndex)
{
    m_strName = strFieldName;
    m_nIndex  = nIndex;
}

SQLITE_SQLFIELD_NSAMESPACE::CSQLField::CSQLField()
{

}

SQLITE_SQLFIELD_NSAMESPACE::CSQLField::CSQLField(const SQLITE_SQLFIELD_NSAMESPACE::CSQLField& rField)
{
    Copy(rField);
}

void SQLITE_SQLFIELD_NSAMESPACE::CSQLField::Copy(const CSQLField& rField)
{
    m_strName = rField.m_strName;
    m_nIndex  = rField.m_nIndex;
}

SQLITE_SQLFIELD_NSAMESPACE::CSQLField& SQLITE_SQLFIELD_NSAMESPACE::CSQLField::operator=(const SQLITE_SQLFIELD_NSAMESPACE::CSQLField& rField)
{
    if(this != &rField)
    {
        Copy(rField);
    }
    return *this;
}
