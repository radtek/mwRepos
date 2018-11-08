#include "pthread_cond_t_ex.h"

pthread_cond_t_ex::pthread_cond_t_ex()
{
}

pthread_cond_t_ex::~pthread_cond_t_ex()
{
}

bool pthread_cond_t_ex::pthread_cond_init_ex()
{
    SafeLock safelock(&m_cs);
    nSignalCnt = 0;
    if (0 != pthread_cond_init(&t, NULL))
    {
        return false;
    }
    else
    {
        return true;
    }
}

void pthread_cond_t_ex::pthread_cond_destroy_ex()
{
    SafeLock safelock(&m_cs);
    pthread_cond_destroy(&t);
    nSignalCnt = 0;
}

void pthread_cond_t_ex::pthread_cond_signal_ex()
{
    SafeLock safelock(&m_cs);
    //pthread_cond_signal(&t);
    pthread_cond_broadcast(&t);
    ++nSignalCnt;
    //++nSignalCnt;
}

bool pthread_cond_t_ex::issignaled()
{
    SafeLock safelock(&m_cs);
    bool bTrue = nSignalCnt>0;
    nSignalCnt = 0;
    return bTrue;
}

pthread_cond_t* pthread_cond_t_ex::getinstance()
{
    return &t;
}

