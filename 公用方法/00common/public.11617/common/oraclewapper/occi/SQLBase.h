/******************************************************************************
   File:        SQLBase.h
   Contents:    CSQLBase class definition
   Description: Base class for CSQLConnection and CSQLResult
******************************************************************************/

#ifndef __ORACLE_SQLBASE_H__
#define __ORACLE_SQLBASE_H__

#include "SQLTypes.h"
#include "SQLValue.h"
#include "SQLField.h"

namespace ORACLE_SQLBASE_NSAMESPACE {
class CSQLBase
{
public:
   CSQLBase(ORACLE_SQLTYPES_NSAMESPACE::ConnectionPtr pCon) :m_pCon(pCon)
   {
   };
   CSQLBase()
   {
        m_pCon  = NULL;
   };
   virtual ~CSQLBase()
   {
   };
protected:
   ORACLE_SQLTYPES_NSAMESPACE::ConnectionPtr m_pCon;
protected:
   void ThrowException(int nErCode, const char* szErr) const;
   void ThrowExceptionIfNotOpen() const;
};
}

#endif
