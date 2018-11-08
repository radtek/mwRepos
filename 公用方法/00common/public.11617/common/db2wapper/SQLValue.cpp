#include "SQLValue.h"

DB2_SQLVALUE_NSAMESPACE::CSQLValue::CSQLValue()
{
    m_pData     = NULL;
    m_nLength   = 0;
    m_eType     = SQL_CHAR;
}

DB2_SQLVALUE_NSAMESPACE::CSQLValue::~CSQLValue()
{
   if(NULL != m_pData)
   {
      delete []m_pData;
      m_pData = NULL;
   }
}

DB2_SQLVALUE_NSAMESPACE::CSQLValue::CSQLValue(const char* pData, unsigned long nLength, DB2_SQLTYPES_NSAMESPACE::Type eType)
{
    m_nLength = nLength;
    m_eType   = eType;
    if((nLength > 0) && (NULL != pData))
    {
        m_pData = new char[nLength+1];
        memset(m_pData, '\0', nLength+1);
        memcpy(m_pData, pData, nLength);
    }
    else
    {
        m_pData = new char[1];
        m_pData[0] = '\0';
    }
}

DB2_SQLVALUE_NSAMESPACE::CSQLValue::CSQLValue(const DB2_SQLVALUE_NSAMESPACE::CSQLValue& rValue)
{
    m_nLength = rValue.m_nLength;
    m_eType   = rValue.m_eType;
    unsigned long nLength =  rValue.m_nLength;
    if((nLength > 0) && (NULL != rValue.m_pData))
    {
        m_pData = new char[nLength+1];
        memset(m_pData, '\0', nLength+1);
        memcpy(m_pData, rValue.m_pData, nLength);
    }
    else
    {
        m_pData = new char[1];
        m_pData[0] = '\0';
    }
}

DB2_SQLVALUE_NSAMESPACE::CSQLValue& DB2_SQLVALUE_NSAMESPACE::CSQLValue::operator=(const DB2_SQLVALUE_NSAMESPACE::CSQLValue& rValue)
{
    if(this != &rValue)
    {
        //清空原来的数据
        if(NULL != m_pData)
        {
           delete []m_pData;
           m_pData = NULL;
        }
        m_nLength   = 0;
        m_eType     = SQL_CHAR;

        m_nLength = rValue.m_nLength;
        m_eType   = rValue.m_eType;
        unsigned long nLength =  rValue.m_nLength;
        if((nLength > 0) && (NULL != rValue.m_pData))
        {
            m_pData = new char[nLength+1];
            memset(m_pData, '\0', nLength+1);
            memcpy(m_pData, rValue.m_pData, nLength);
        }
        else
        {
            m_pData = new char[1];
            m_pData[0] = '\0';
        }
    }
    return *this;
}



