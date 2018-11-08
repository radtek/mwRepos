// LockBase.h: interface for the LockBase class.
// Author: JiangHongbo
//////////////////////////////////////////////////////////////////////

#ifndef _LOCKBASEH_
#define _LOCKBASEH_

#include "utility/common/mwcore_export.h"

namespace UTI{
namespace COM{

class UTILITY_Export LockBase
{
public:
    LockBase();
    virtual ~LockBase();
    virtual void Lock() = 0;
    virtual void UnLock() = 0;
};
}} // end  namespace

#endif // !_LOCKBASEH_
