#ifndef _MW_TIME_H
#define _MW_TIME_H


#ifdef WIN32
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

#elif LINUX
#include <time.h>
#endif



class CMWTime
{
public:
 	CMWTime();
 	void initTime();
	//void getTimeNow();

	//获取用时，单位ms
	double getUseTime();

	void sleepTime(int time);

private:
#ifdef WIN32
	LARGE_INTEGER		m_start;
	LARGE_INTEGER		m_now;
	LARGE_INTEGER		m_freq;
#elif LINUX
	timespec ts1;
	timespec ts2;
	double diff(timespec start, timespec end);
#endif
	double				m_useTime;

	unsigned long		_time_start;				  
	bool				use_large_time;           // flag that indicate whether use large time
};

#endif
