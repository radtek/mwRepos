// DerivedLock.cpp: implementation of the DerivedLock class.
//
//////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include "DerivedLock.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#if (defined  MW_WINDOWS) || (defined  WIN32)

DerivedLock::DerivedLock()
{
    InitializeCriticalSection(&m_cs);
}

DerivedLock::~DerivedLock()
{
    DeleteCriticalSection(&m_cs);
}

void DerivedLock::Lock()
{
    EnterCriticalSection(&m_cs);
}

void DerivedLock::UnLock()
{
    LeaveCriticalSection(&m_cs);
}

void DerivedLock::Destroy()
{
    DeleteCriticalSection(&m_cs);
}


#elif defined LINUX

DerivedLock::DerivedLock()
{
    pthread_mutex_init(&m_cs, NULL);
}

DerivedLock::~DerivedLock()
{
    pthread_mutex_destroy(&m_cs);
}

void DerivedLock::Lock()
{
    pthread_mutex_lock(&m_cs);
}

void DerivedLock::UnLock()
{
    pthread_mutex_unlock(&m_cs);
}

void DerivedLock::Destroy()
{
    pthread_mutex_destroy(&m_cs);
}


#endif

