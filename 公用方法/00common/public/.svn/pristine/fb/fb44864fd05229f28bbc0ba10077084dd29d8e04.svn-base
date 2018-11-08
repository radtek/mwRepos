#include "utility/common/mwcorepch.h"

#if (defined  MW_WINDOWS) || (defined  WIN32)

//#include <windows.h>
#include <Winbase.h>
#include <assert.h>
#include "utility/common/process_stat.h"

/// 时间转换
static uint64_t file_time_2_utc(const FILETIME* ftime)
{
	LARGE_INTEGER li;
	if (NULL == ftime)
	{
		return 0;
	}
//	assert(ftime);
	li.LowPart = ftime->dwLowDateTime;
	li.HighPart = ftime->dwHighDateTime;
	return li.QuadPart;
}

/// 获得CPU的核数
static int get_processor_number()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return (int)info.dwNumberOfProcessors;
}

int get_cpu_usage()
{
	//cpu数量
	static int processor_count_ = -1;
	//上一次的时间
	static int64_t last_time_ = 0;
	static int64_t last_system_time_ = 0;

	FILETIME now;
	FILETIME creation_time;
	FILETIME exit_time;
	FILETIME kernel_time;
	FILETIME user_time;
	int64_t system_time;
	int64_t time;
	int64_t system_time_delta;
	int64_t time_delta;

	int cpu = -1;

	if(processor_count_ == -1)
	{
		processor_count_ = get_processor_number();
	}

	GetSystemTimeAsFileTime(&now);

	if (!GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time,
		&kernel_time, &user_time))
	{
		// We don't assert here because in some cases (such as in the Task Manager)
		// we may call this function on a process that has just exited but we have
		// not yet received the notification.
		return -1;
	}
	system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time)) /processor_count_;
	time = file_time_2_utc(&now);

	if ((last_system_time_ == 0) || (last_time_ == 0))
	{
		// First call, just set the last values.
		last_system_time_ = system_time;
		last_time_ = time;
		return -1;
	}

	system_time_delta = system_time - last_system_time_;
	time_delta = time - last_time_;

//	assert(time_delta != 0);

	if (time_delta == 0)
		return -1;

	// We add time_delta / 2 so the result is rounded.
	cpu = (int)((system_time_delta * 100 + time_delta / 2) / time_delta);
	last_system_time_ = system_time;
	last_time_ = time;
	return cpu;
}
HINSTANCE hDll = NULL;
GETPROCESSMEMORYINFO fpGetProcessMemoryInfo = NULL;
int get_memory_usage(uint64_t* mem, uint64_t* vmem)
{
	int nRet = 0;
	if (!hDll)
	{
		hDll = LoadLibrary("psapi.dll");
		if (!fpGetProcessMemoryInfo)
		{
			fpGetProcessMemoryInfo = (GETPROCESSMEMORYINFO)GetProcAddress(hDll, "GetProcessMemoryInfo");
		}
	}
	if (NULL != hDll)
	{
		if (fpGetProcessMemoryInfo)
		{
			PROCESS_MEMORY_COUNTERS pmc;
			if(fpGetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
			{
				if(mem) *mem = pmc.WorkingSetSize;
				if(vmem) *vmem = pmc.PagefileUsage;
				nRet = 0;
			}
			else
			{
				nRet = -1;
			}
		}
		else
		{
			nRet = -1;
		}
	}
	else
	{
		nRet = -1;
	}
// 	if (NULL != hDll)
// 	{
// 		FreeLibrary(hDll);
// 	}
	return nRet;
}

int get_io_bytes(uint64_t* read_bytes, uint64_t* write_bytes)
{
	int nRet = 0;

/*	IO_COUNTERS io_counter;
	if(GetProcessIoCounters(GetCurrentProcess(), &io_counter))
	{
		if(read_bytes) *read_bytes = io_counter.ReadTransferCount;
		if(write_bytes) *write_bytes = io_counter.WriteTransferCount;
		nRet = 0;
	}
	else
	{
		nRet = -1;
	}
*/
	return nRet;
}

int get_disk_free_space(const char* driver)
{
	ULARGE_INTEGER nFreeBytesAvailable;
	ULARGE_INTEGER nTotalNumberOfBytes;
	ULARGE_INTEGER nTotalNumberOfFreeBytes;
	if (GetDiskFreeSpaceEx(driver,
		&nFreeBytesAvailable,
		&nTotalNumberOfBytes,
		&nTotalNumberOfFreeBytes))
	{
		return nTotalNumberOfFreeBytes.QuadPart/(ULONGLONG)(1024*1024);
	}
	return 0;
}
#endif /*WIN32*/