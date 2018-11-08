/******************************************************************************
   File:        SQLBase.h
   Contents:    CSQLBase class definition
   Description: Base class for CSQLConnection and CSQLResult
******************************************************************************/

#ifndef __SQLITE_SQLBASE_H__
#define __SQLITE_SQLBASE_H__

#include "SQLTypes.h"
#include "SQLValue.h"
#include "SQLField.h"
#include "sqlite3.h"

namespace SQLITE_SQLBASE_NSAMESPACE {
//using namespace SQLITE_SQLTYPES_NSAMESPACE;
class CSQLBase
{
public:
   CSQLBase(SQLITE_SQLTYPES_NSAMESPACE::ConnectionPtr hCon):m_hCon(hCon)
   {
   }
   CSQLBase()
   {
        m_hCon = NULL;
   };
   virtual ~CSQLBase()
   {
   };
protected:
   SQLITE_SQLTYPES_NSAMESPACE::ConnectionPtr m_hCon;
protected:
   void HandleError(int nType/*, SQLHANDLE hHandle*/);
   void ThrowException(int nErCode, const char* szErr) const;
   void ThrowExceptionIfNotOpen() const;
};
}

#endif
