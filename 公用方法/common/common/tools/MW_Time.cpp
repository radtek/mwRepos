//#include "StdAfx.h"
#include "MW_Time.h"
//#include <iostream>

using namespace std;

CMWTime::CMWTime()
{
#ifdef WIN32
	QueryPerformanceFrequency(&m_freq);
#endif
}


void CMWTime::initTime()
{
#ifdef WIN32
	if(QueryPerformanceCounter(&m_start))
	{
		use_large_time = true;	
	}
	else
	{
		use_large_time = false;
		_time_start = timeGetTime();		//timeGetTime¾«¶ÈÊÇ1ms
	}
#elif LINUX
		int nRet = clock_gettime(CLOCK_REALTIME, &ts1);
#endif
}

double CMWTime::getUseTime()
{
#ifdef WIN32
	if(use_large_time == true)
	{
		QueryPerformanceCounter(&m_now);
		return ((m_now.QuadPart - m_start.QuadPart) * 1000.0) / m_freq.QuadPart;
	}

	return (timeGetTime() - _time_start);
#elif LINUX
	int nRet = -1;
	nRet = clock_gettime(CLOCK_REALTIME, &ts2);

	return diff(ts1, ts2);//(ts2.tv_sec - ts1.tv_sec)*1000 + (ts2.tv_nsec - ts1.tv_nsec)*1000*1000;
#endif
}

#ifdef LINUX
double  CMWTime::diff(timespec start, timespec end)
{
    timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp.tv_sec * 1000 + temp.tv_nsec / 1000000.0;
}
#endif
