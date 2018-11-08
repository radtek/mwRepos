/******************************************************************************
   File:        SQLBase.h
   Contents:    CSQLBase class definition
   Description: Base class for CSQLConnection and CSQLResult
******************************************************************************/

#ifndef __ODBC_SQLBASE_H__
#define __ODBC_SQLBASE_H__

#include "SQLTypes.h"
#include "SQLValue.h"
#include "SQLField.h"

namespace ODBC_SQLBASE_NSAMESPACE {
class CSQLBase
{
public:
   CSQLBase(SQLHANDLE hCon) :m_hCon(hCon)
   {
   };
   CSQLBase()
   {
        m_hCon = NULL;
   };
   virtual ~CSQLBase()
   {
   };
protected:
   ODBC_SQLTYPES_NSAMESPACE::ConnectionPtr m_hCon;
protected:
   void HandleError(SQLSMALLINT nType, SQLHANDLE hHandle);
   void ThrowException(int nErCode, const char* szErr) const;
   void ThrowExceptionIfNotOpen() const;
};
}

#endif
