// SafeLock.h: interface for the SafeLock class.
// Author: JiangHongbo
//////////////////////////////////////////////////////////////////////

#ifndef _SAFELOCK_H_
#define _SAFELOCK_H_

#include "utility/common/mwcore_export.h"

namespace UTI{
namespace COM{

class LockBase;
class UTILITY_Export SafeLock
{
public:
    SafeLock();
    SafeLock(LockBase* pLock);
    virtual ~SafeLock();
private:
    LockBase *m_pLock;
};

}} // end namespace
#endif
