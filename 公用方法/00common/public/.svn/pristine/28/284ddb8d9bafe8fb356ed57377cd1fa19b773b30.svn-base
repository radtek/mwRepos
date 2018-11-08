// SafeLock.cpp: implementation of the SafeLock class.
//
//////////////////////////////////////////////////////////////////////

#include "utility/common/mwcorepch.h"
#include "utility/common/SafeLock.h"
#include "utility/common/LockBase.h"
#include <stdio.h>
#include <stdlib.h>
using namespace UTI::COM;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SafeLock::SafeLock()
{
    m_pLock = NULL;
}

SafeLock::SafeLock(LockBase* pLock)
{
    m_pLock = pLock;
    if (m_pLock)
    {
        m_pLock->Lock();
    }
}

SafeLock::~SafeLock()
{
    if (m_pLock)
    {
        m_pLock->UnLock();
        m_pLock = NULL;
    }
}
