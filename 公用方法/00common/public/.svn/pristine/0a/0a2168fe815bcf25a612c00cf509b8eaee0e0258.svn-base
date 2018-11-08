#ifndef PROCESS_STAT_EX_H
#define PROCESS_STAT_EX_H

#ifdef WIN32

typedef __int64             int64_t;
typedef unsigned __int64        uint64_t;

typedef struct _PROCESS_MEMORY_COUNTERS {
    DWORD cb;
    DWORD PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage;
    SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
} PROCESS_MEMORY_COUNTERS;
typedef PROCESS_MEMORY_COUNTERS *PPROCESS_MEMORY_COUNTERS;

BOOL
WINAPI
GetProcessMemoryInfo(
                     HANDLE Process,
                     PPROCESS_MEMORY_COUNTERS ppsmemCounters,
                     DWORD cb
                     );

typedef struct _IO_COUNTERS {
    ULONGLONG  ReadOperationCount;
    ULONGLONG  WriteOperationCount;
    ULONGLONG  OtherOperationCount;
    ULONGLONG ReadTransferCount;
    ULONGLONG WriteTransferCount;
    ULONGLONG OtherTransferCount;
} IO_COUNTERS;
typedef IO_COUNTERS *PIO_COUNTERS;

typedef BOOL (WINAPI *GETPROCESSMEMORYINFO)(HANDLE, PPROCESS_MEMORY_COUNTERS, DWORD);


// 获取当前进程的cpu使用率，返回-1失败
int get_cpu_usage();


// 获取当前进程内存和虚拟内存使用量，返回-1失败，0成功
int get_memory_usage(uint64_t* mem, uint64_t* vmem);


// 获取当前进程总共读和写的IO字节数，返回-1失败，0成功
int get_io_bytes(uint64_t* read_bytes, uint64_t* write_bytes);

//取磁盘空余量
int get_disk_free_space(const char* driver);

#endif /*WIN32*/
#ifdef LINUX

#include <stdint.h>

// 获取当前进程cpu使用率(%)，返回-1失败，大于等于0成功，首次调用返回0，相当于初始化
int get_cpu_usage();

// 获取当前进程内存和虚拟内存使用量，返回-1失败，0成功 返回单位byte
int get_memory_usage(uint64_t* mem, uint64_t* vmem);

// 获取当前进程总共读和写的IO字节数，返回-1失败，0成功
int get_io_bytes(uint64_t* read_bytes, uint64_t* write_bytes);

//取磁盘空余量,返回-1失败，大于等于0成功,返回的单位为k
int get_disk_free_space(const char* driver);

//////////////////////////////////////////////////////////////////////////

//获取cpu信息
int get_cpu_info(int* pCpuNum,char* pCpuName);

//获取内存信息
int get_mem_info(char*pMem, char* pPhyMemTol, char*pPhyMemCanUse, char* pVirMemTol);

//获取系统时间
int get_syscurrent_time(char*pCurrTime);

//获取进程数量
int get_process_cnt();

//获取指定进程ID所占CPU
double get_cpuinfo_for_pid(int nPid);

//获取指定进程的进程名，所占内存，线程数
int get_proceinfo_for_pid(int nPid,char* pProcName, char* pMemory, char* pThreadCnt);

//获取进程信息  出参为进程个数和各个进程信息
int get_process_Info(int*nCnt, char* pProcInfo);

//获取网络信息
int get_network_info(int* nNetCnt, char* pNetworkInfo);


//获取外网IP
int get_outip_info(char* pOutIp);
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif /*LINUX*/
#endif/*PROCESS_STAT_H*/

