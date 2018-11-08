#ifndef __PTHREAD_COND_T_EX_H__
#define __PTHREAD_COND_T_EX_H__


namespace UTI{
namespace COM{

#if defined(MW_LINUX) || defined(LINUX)
#include "SafeLock.h"
#include "DerivedLock.h"
class pthread_cond_t_ex
{
public:
    pthread_cond_t_ex();
    bool pthread_cond_init_ex();
    void pthread_cond_destroy_ex();
    void pthread_cond_signal_ex();
    bool issignaled();
    pthread_cond_t* getinstance();
    virtual ~pthread_cond_t_ex();
private:
    UTI::COM::DerivedLock m_cs;
    pthread_cond_t t;
    int nSignalCnt;
};
#endif
}} // end namespace

#endif
