// DerivedLock.h: interface for the DerivedLock class.
// Author: JiangHongbo
//////////////////////////////////////////////////////////////////////
#ifndef _DERIVEDLOCK_H_
#define _DERIVEDLOCK_H_

#ifdef LINUX
#include <pthread.h>
typedef pthread_mutex_t CRITICAL_SECTION;
#endif

#if (defined  MW_WINDOWS) || (defined  WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#undef _WINDOWS_
#endif

#include "LockBase.h"
#include "utility/common/mwcore_export.h"

namespace UTI{
namespace COM{

class UTILITY_Export DerivedLock : public LockBase
{
public:
    DerivedLock();
    virtual ~DerivedLock();
    virtual void Lock();
    virtual void UnLock();
    virtual void Destroy();
private:
    CRITICAL_SECTION m_cs;
};
}} // end namespace 

#endif
