#ifndef __ADAPTER__H__
#define __ADAPTER__H__

#pragma warning(disable:4786)

#include <string>
#include <vector>
#include "../../common/DerivedLock.h"
#include "../../common/SafeLock.h"

#define IN                  //函数输入参数标志
#define OUT                 //函数输出参数标志

//控制打印信息
#ifdef DEBUG
#define DEBUG_PRINTF if(1)printf
#else
#define DEBUG_PRINTF if(0)printf
#endif

#ifndef TINYINT
#define TINYINT    unsigned char
#endif

//WINDOWS编译环境引用及类型定义
#if (defined  MW_WINDOWS) || (defined  WIN32)
//#include <windows.h>
#include <process.h>
#include "Shlwapi.h"
#include <winsock2.h>
#include <sys/stat.h>
#include <afxinet.h>
#include "Wininet.h"
#include <vector>
#include <string>
#include <wtypes.h>

#ifndef FORMAT_I64D
#define FORMAT_I64D "%I64d"
#endif

#ifndef __STDCALL
#define __STDCALL __stdcall         //区别WINDOWS环境线程函数的调用方式
#endif

#ifndef FAR_STDCALL                 //区别windows和linux调用dll时调用方式的不同
#define FAR_STDCALL far __stdcall
#endif

#ifndef pthread_t                 
#define pthread_t unsigned long int
#endif

typedef HANDLE  EVENT;
typedef unsigned    THREAD_RETURN;   //统一线程函数的返回值
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "ws2_32.lib")

/* Flags we can use with send/ and recv. */
#ifndef MSG_WINMASK
#define MSG_WINMASK     0x7             /* flags understood by WinSock calls */
#endif
#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL    0x00            /* Don't raise SIGPIPE */
#endif
#ifndef MSG_TRUNC
#define MSG_TRUNC       0x0100          /* Normal data truncated */
#endif
#ifndef MSG_CTRUNC
#define MSG_CTRUNC      0x0200          /* Control data truncated */
#endif

//linux编译环境引用及类型定义
#elif defined LINUX
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <locale.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/vfs.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#if (defined  _X64_) || (defined  _686_M32_)
#include <ext/atomicity.h>
#else
#include <bits/atomicity.h>
#endif

#include <netdb.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include <strings.h>
#include <memory.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#ifdef NOT_NEED_SQLTYPES
#else
#include <sqltypes.h>
#endif
#include <iconv.h>
#ifndef FORMAT_I64D
#define FORMAT_I64D "%lld"
#endif

#ifndef HMODULE
#define HMODULE void*
#endif

#ifndef HINSTANCE
#define HINSTANCE HMODULE
#endif

#ifndef FARPROC
#define FARPROC void*
#endif

#ifndef CALLBACK
#define CALLBACK                   //区别WINDOWS环境回调函数调用方式
#endif

#ifndef __STDCALL
#define __STDCALL                   //区别WINDOWS环境线程函数的调用方式
#endif

#ifndef FAR_STDCALL                 //区别windows和linux调用dll时调用方式的不同
#define FAR_STDCALL
#endif

#ifndef __SQLTYPES_H                //防止与sqltypes.h中的定义冲突

#ifndef LPWSTR
#define LPWSTR  wchar_t*
#endif

#ifndef LPCSTR
#define LPCSTR  const char*
#endif

#ifndef LPTSTR
#define LPTSTR  char*
#endif

#ifndef LPSTR
#define LPSTR   char*
#endif

#endif

#ifndef LPCWSTR
#define LPCWSTR const wchar_t*
#endif

#ifndef LPCTSTR
#define LPCTSTR const char*
#endif

#ifndef LONG
#define LONG    long
#endif

#ifndef PLONG
#define PLONG   LONG*
#endif

#ifndef LPVOID
#define LPVOID  void*
#endif

#ifndef UINT
#define UINT    unsigned int
#endif

#ifndef ULONG
#define ULONG   unsigned long
#endif

#ifndef INT
#define INT     int
#endif

#ifndef CHAR
#define CHAR    char
#endif

#ifndef USHORT
#define USHORT  unsigned short
#endif

#ifndef UCHAR
#define UCHAR   unsigned char
#endif

#ifndef BYTE
#define BYTE    unsigned char
#endif

#ifndef WCHAR
#define WCHAR   wchar_t
#endif

#ifndef DWORD
#ifdef _X64_
#define DWORD   unsigned int
#else
#define DWORD   unsigned long
#endif
#endif

#ifndef WORD
#define WORD    unsigned short
#endif

#ifndef DATE
#define DATE    double
#endif

#ifndef INT64
#define INT64   long long
#endif

#ifndef UINT64
#define UINT64   unsigned long long
#endif

#ifndef __int64
#define __int64 long long
#endif

#ifndef _int64
#define _int64 long long
#endif

#ifndef __uint64
#define __uint64 unsigned long long
#endif

#ifndef _stat
#define _stat   stat
#endif

#ifndef BOOL
#define BOOL    int
#endif

#ifndef LPBOOL
#define LPBOOL  int*
#endif
#ifndef LPBYTE
#define LPBYTE  unsigned char*
#endif

#ifndef CONST
#define CONST  const
#endif

#ifndef SOCKET
#ifdef _X64_
#define INT_PTR INT64
#define SOCKET unsigned int
#else
#define INT_PTR INT
#define SOCKET int
#endif
#endif

#ifndef EVENT
#define EVENT       pthread_cond_t  //统一事件
#endif

#ifndef THREAD_RETURN
#define THREAD_RETURN   void*   //统一线程函数的返回值
#endif

#ifndef LPSECURITY_ATTRIBUTES
#define LPSECURITY_ATTRIBUTES int
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef _atoi64
#define _atoi64 atoll
#endif

//宽字符与多字节转换兼容windows的一些宏
//CP
#ifndef CP_ACP
#define CP_ACP              0
#endif

#ifndef CP_OEMCP
#define CP_OEMCP            1
#endif

#ifndef CP_MACCP
#define CP_MACCP            2
#endif

#ifndef CP_THREAD_ACP
#define CP_THREAD_ACP       3
#endif

#ifndef CP_SYMBOL
#define CP_SYMBOL           42
#endif

#ifndef CP_UTF7
#define CP_UTF7             65000
#endif

#ifndef CP_UTF8
#define CP_UTF8             65001
#endif

//WC
#ifndef WC_NO_BEST_FIT_CHARS
#define WC_NO_BEST_FIT_CHARS    0
#endif

#ifndef WC_COMPOSITECHECK
#define WC_COMPOSITECHECK       512
#endif

#ifndef WC_DISCARDNS
#define WC_DISCARDNS            16
#endif

#ifndef WC_SEPCHARS
#define WC_SEPCHARS             32
#endif

#ifndef WC_DEFAULTCHAR
#define WC_DEFAULTCHAR          64
#endif

//MB
#ifndef MB_PRECOMPOSED
#define MB_PRECOMPOSED          1
#endif

#ifndef MB_COMPOSITE
#define MB_COMPOSITE            2
#endif

#ifndef MB_ERR_INVALID_CHARS
#define MB_ERR_INVALID_CHARS    8
#endif

#ifndef MB_USEGLYPHCHARS
#define MB_USEGLYPHCHARS        4
#endif

//字符集的一些定义
#ifndef CSET_GBK
//#define CSET_GBK                    "GBK"
#define CSET_GBK                    "gb18030"
#endif

#ifndef CSET_UTF8
#define CSET_UTF8                   "UTF-8"
#endif

//小端
#ifndef CSET_UCS4LE
#define CSET_UCS4LE                   "UCS-4LE"
#endif

//大端
#ifndef CSET_UCS4BE
#define CSET_UCS4BE                   "UCS-4BE"
#endif

#ifndef LC_NAME_zh_CN
#define LC_NAME_zh_CN               "zh_CN"
#endif

#ifndef LC_NAME_zh_CN_GBK
#define LC_NAME_zh_CN_GBK           LC_NAME_zh_CN "." CSET_GBK
#endif

#ifndef LC_NAME_zh_CN_UTF8
#define LC_NAME_zh_CN_UTF8          LC_NAME_zh_CN "." CSET_UTF8
#endif

#ifndef LC_NAME_zh_CN_DEFAULT
#define LC_NAME_zh_CN_DEFAULT       LC_NAME_zh_CN_GBK
#endif

//一些系统规定的长度的定义
#ifndef MAXLINELENGTH
#define MAXLINELENGTH   512          //配置文件一行的最大长度512
#endif

#ifndef MAX_PATH
#define MAX_PATH        260
#endif

//一些windows下的函数在linux下的名字
#ifndef _stricmp
#define _stricmp    strcasecmp
#endif

#ifndef _strnicmp
#define _strnicmp   strncasecmp
#endif

#ifndef stricmp
#define stricmp     strcasecmp
#endif

#ifndef strnicmp
#define strnicmp    strncasecmp
#endif

#ifndef _vsnprintf
#define _vsnprintf  vsnprintf
#endif

#ifndef _snprintf
#define _snprintf  snprintf
#endif

#ifndef FAR
#define FAR
#endif

#ifndef MAXIMUM_WAIT_OBJECTS
#define MAXIMUM_WAIT_OBJECTS 64     // Maximum number of wait objects
#endif

#ifndef WSADESCRIPTION_LEN
#define WSADESCRIPTION_LEN  256
#endif

#ifndef WSASYS_STATUS_LEN
#define WSASYS_STATUS_LEN  128
#endif

//一些等待事件返回值的宏定义
#ifndef STATUS_WAIT_0
#define STATUS_WAIT_0 ((DWORD )0x00000000L)
#define WAIT_OBJECT_0 ((STATUS_WAIT_0 ) + 0 )
#endif

#ifndef STATUS_TIMEOUT
#define STATUS_TIMEOUT ((DWORD )0x00000102L)
#define WAIT_TIMEOUT STATUS_TIMEOUT
#endif

#ifndef WAIT_FAILED
#define WAIT_FAILED (DWORD)0xFFFFFFFF
#endif

#ifndef STATUS_ABANDONED_WAIT_0
#define STATUS_ABANDONED_WAIT_0 ((DWORD )0x00000080L)
#define WAIT_ABANDONED_0 ((STATUS_ABANDONED_WAIT_0 ) + 0 )
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (SOCKET)(~0)
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

#ifndef PUINT32
#define PUINT32 unsigned int*
#endif

#ifndef HANDLE
#define HANDLE void*
#endif

#ifndef DLL_PROCESS_DETACH
#define DLL_PROCESS_DETACH 0
#endif

#ifndef DLL_PROCESS_ATTACH
#define DLL_PROCESS_ATTACH 1
#endif

#ifndef APIENTRY
#define APIENTRY
#endif

#ifndef MAKEWORD
#define MAKEWORD(low,high) \
        ((unsigned short)((unsigned char)(low)) | (((unsigned short)(unsigned char)(high))<<8))
#endif

#ifndef LOBYTE
#define LOBYTE(w)           ((BYTE)(w))
#endif

#ifndef HIBYTE
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))
#endif

typedef struct WSAData {
        WORD                    wVersion;
        WORD                    wHighVersion;
        char                    szDescription[WSADESCRIPTION_LEN+1];
        char                    szSystemStatus[WSASYS_STATUS_LEN+1];
        unsigned short          iMaxSockets;
        unsigned short          iMaxUdpDg;
        char *              lpVendorInfo;
} WSADATA, *LPWSADATA;

#endif


//获取系统当前时间的自定义拓展结构体
//
typedef struct SYSTEMTIME_EX
{
    int m_iYear;
    int m_iMonth;
    int m_iDayOfWeek;
    int m_iDay;
    int m_iHour;
    int m_iMinute;
    int m_iSecond;
    int m_iMilliseconds;

    SYSTEMTIME_EX()
    {
        memset(this, 0, sizeof(struct SYSTEMTIME_EX));
    }

    SYSTEMTIME_EX& operator=(const struct SYSTEMTIME_EX& other)
    {
        if(this == &other)
        {
            return *this;
        }
        memset(this, 0, sizeof(struct SYSTEMTIME_EX));
        memcpy(this, &other, sizeof(struct SYSTEMTIME_EX));
        return *this;
    }

}SYSTEMTIME_EX,*LPSYSTEMTIME_EX;


typedef struct SYSTEMTIME_EX_LINUX
{
    int wYear;
    int wMonth;
    int wDayOfWeek;
    int wDay;
    int wHour;
    int wMinute;
    int wSecond;
    int wMilliseconds;

    SYSTEMTIME_EX_LINUX()
    {
        memset(this, 0, sizeof(struct SYSTEMTIME_EX_LINUX));
    }
	
	SYSTEMTIME_EX_LINUX(SYSTEMTIME_EX& systimeEx)
    {
        memset(this, 0, sizeof(struct SYSTEMTIME_EX_LINUX));
		memcpy(this, &systimeEx, sizeof(struct SYSTEMTIME_EX_LINUX));
    }

    SYSTEMTIME_EX_LINUX& operator=(const struct SYSTEMTIME_EX_LINUX& other)
    {
        if(this == &other)
        {
            return *this;
        }
        memset(this, 0, sizeof(struct SYSTEMTIME_EX_LINUX));
        memcpy(this, &other, sizeof(struct SYSTEMTIME_EX_LINUX));
        return *this;
    }

}SYSTEMTIME_EX_LINUX,*LPSYSTEMTIME_EX_LINUX;
#if  !(defined(_MSC_VER) && (_MSC_VER>1200))
typedef struct _MEMORYSTATUSEX { // mst 
    DWORD dwLength;        // sizeof(MEMORYSTATUS) 
    DWORD dwMemoryLoad;    // percent of memory in use 
    DWORD dwTotalPhys;     // bytes of physical memory 
    DWORD dwAvailPhys;     // free physical memory bytes 
    DWORD dwTotalPageFile; // bytes of paging file 
    DWORD dwAvailPageFile; // free bytes of paging file 
    DWORD dwTotalVirtual;  // user bytes of address space 
    DWORD dwAvailVirtual;  // free user bytes 
} MEMORYSTATUSEX, *LPMEMORYSTATUSEX; 
#endif


#ifndef DEBUG_PRINTF
#define DEBUG_PRINTF(s, val)  if(1){printf("%s  %d  %s  %d\n", __FILE__, __LINE__, s, val);}
#endif

class CCompatibleEvent;
class CCompatibleThread;
class CAdapter
{
public:

    //信号处理函数
    //static void sighandler(int signum, siginfo_t *pSigInfo, void *pContext)

    //**********************************
    //Description: 适配器初始化事件对象 默认创建自动重置事件
    // Parameter:  CCompatibleEvent &event
    //             bool bManualReset  是否需要手动重置事件
    // Returns:    bool
    //************************************
    static bool InitCompatibleEvent(CCompatibleEvent &event, bool bManualReset=false);

    //**********************************
    //Description: 适配器销毁事件对象
    // Parameter:  CCompatibleEvent &event
    // Returns:    void
    //************************************
    static void UnInitCompatibleEvent(CCompatibleEvent &event);

    //**********************************
    //Description: 等待事件
    // Parameter:  CCompatibleEvent &event
    //             DWORD dwMilliseconds  等待超时时间/毫秒
    // Returns:    int 返回0表示有事件  1表示超时  -1无信号
    //************************************
    static int WaitForCompatibleEvent(CCompatibleEvent &event, DWORD dwMilliseconds);

    //**********************************
    //Description: 等待多个事件(WaitForMultipleObjects函数兼容)
    // Parameter:  nCount 事件的数量 最大值为MAXIMUM_WAIT_OBJECTS（64）
    //             lpHandles 事件数组的指针
    //             bWaitAll 等待的类型，如果为TRUE 则等待所有信号量有效在往下执行，
    //                      FALSE 当有其中一个信号量有效时就向下执行(暂时只支持FALSE的情况)
    //             dwMilliseconds 超时时间
    // Returns:    int 返回0表示有事件  1表示超时  -1无信号
    //************************************
    static DWORD WaitForMultipleCompatibleEvent(
                 DWORD nCount,                       // number of handles in the handle array
                 CCompatibleEvent** lppHandles,          // pointer to the object-handle array
                 BOOL fWaitAll,                      // wait flag
                 DWORD dwMilliseconds                // time-out interval in milliseconds
                 );

    //**********************************
    //Description: 设置事件状态
    // Parameter:  CCompatibleEvent &event
    // Returns:    void
    //************************************
    static void SetCompatibleEvent(CCompatibleEvent &event);

    //**********************************
    //Description: 设置事件状态
    // Parameter:  CCompatibleEvent &event
    // Returns:    void
    //************************************
	static void SetEvent(CCompatibleEvent &event);

    //**********************************
    //Description: 重置事件对象为无信号
    // Parameter:  CCompatibleEvent &event
    // Returns:    void
    //************************************
    static void ResetCompatibleEvent(CCompatibleEvent &event);

    //**********************************
    //Description: 创建WINDOWS和linux环境兼容的线程
    // Parameter:  start_address  线程函数入口地址
    //             arglist        传给线程的参数
    // Returns:    bool  创建成功返回true 否则返回false
    //************************************
    static bool CreateCompatibleThread(THREAD_RETURN(__STDCALL *start_address )( void * ), void *arglist);
	static bool CreateCompatibleThread(THREAD_RETURN(__STDCALL *start_address )( void * ), void *arglist, unsigned int stack_size);
	static bool CreateCompatibleThread(THREAD_RETURN(__STDCALL *start_address )( void * ), void *arglist, pthread_t& ptid);

	//在线程结束时调用,否则容易造成资源不能释放
	static void EndCompatibleThread();

    //**********************************
    //Description: 获取当前线程ID
    // Parameter:  void
    // Returns:    unsigned long 线程ID
    //************************************
    static unsigned long GetCurrentThreadId(void);

    //**********************************
    //Description: 加载动态库
    // Parameter:  LPCTSTR lpFileName 动态库名称或路径
    //             int nFlag 加载方式
    // Returns:    失败返回NULL
    //************************************
    static HMODULE LoadLibrary(const char* pfilename, int nFlag=0);

    //**********************************
    //Description: 取函数地址
    // Parameter:  HMODULE handle loadlibrary返回的值
    //             const char* symbol  函数名
    // Returns:    失败返回NULL
    //************************************
    static FARPROC GetProcAddress(HMODULE handle, const char* symbol);

    //**********************************
    //Description: 取函数地址
    // Parameter:  HMODULE handle loadlibrary返回的值
    //             const char* symbol  函数名
    // Returns:    失败返回NULL
    //************************************
    static int FreeLibrary(HMODULE handle);

    //**********************************
    //Description: 转换为宽字符模式
    // Parameter:  参照MSDN
    // Returns:    int 返回被转换的字节数
    //              失败返回0
    //              暂时支持gbk和utf8两种
    //注:cchWideChar为0时表示要取需要缓冲区大小，
    //此时linux版本下最大可支持转换后宽字符长度不超过8K的操作
    //************************************
    static int MultiByteToWideChar(
            UINT CodePage,         // code page
            DWORD dwFlags,         // character-type options
            LPCSTR lpMultiByteStr, // string to map
            int cbMultiByte,       // number of bytes in string
            LPWSTR lpWideCharStr,  // wide-character buffer
            int cchWideChar        // size of buffer
            );

    //**********************************
    //Description: 转换为多字节模式
    // Parameter:  参照MSDN
    // Returns:    int 返回被转换的字节数
    //              失败返回0
    //              暂时支持gbk和utf8两种
    //注:cbMultiByte为0时表示要取需要缓冲区大小，
    //此时linux版本下最大可支持转换后宽字符长度不超过8K的操作
    //************************************
    static int WideCharToMultiByte(
            UINT CodePage,            // code page
            DWORD dwFlags,            // performance and mapping flags
            LPCWSTR lpWideCharStr,    // wide-character string
            int cchWideChar,          // number of chars in string
            LPSTR lpMultiByteStr,     // buffer for new string
            int cbMultiByte,          // size of buffer
            LPCSTR lpDefaultChar,     // default for unmappable chars
            LPBOOL lpUsedDefaultChar  // set when default char used
            );

    //**********************************
    //Description: 将整型按radix指定的进制转成字符串(linux下只支持radix=8,10,16)
    // Parameter:  参照MSDN
    // Returns:    string指针
    //             没有错误码返回
    //************************************
    static char* itoa(int value, char *string, int radix);

    //**********************************
    //Description: 将整型按radix指定的进制转成字符串(linux下只支持radix=8,10,16)
    // Parameter:  参照MSDN
    // Returns:    string指针
    //             没有错误码返回
    //************************************
    static char* ltoa(int value, char *string, int radix);

    //**********************************
    //Description: 将大整型按radix指定的进制转成字符串(linux下只支持radix=10)
    // Parameter:  参照MSDN
    // Returns:    string指针
    //             没有错误码返回
    //************************************
    static char* _i64toa(__int64 value, char *string, int radix);
    static char* _ui64toa(unsigned _int64 value, char *string, int radix);

    //************************************
    // Description:将US2字符串转为ASCII字符串
    // Parameter: [IN] const char *InMsg    : 原始字符串
    //            [IN] int InLen            : 原始字符串长度
    //            [OUT]char *Data           : ASCII串
    //            [OUT]int *Len             : ASCII串长度
    // Returns:   int                       0:成功  非0:失败
    //************************************
    static int  US2ToAscii(const char *InMsg, int InLen, char *Data, int *Len);

    //************************************
    // Description:将ASCII字符串转为US2字符串
    // Parameter: [IN] const char* Msg      : 原始字符串
    //            [OUT]unsigned char *Data  : US2字符串
    //            [OUT]unsigned int *Len    : US2串长度
    // Returns:   int                       0:成功  非0:失败
    //************************************
    static int  AsciiToUS2(const char* Msg, unsigned char *Data, unsigned int *Len);

    //**********************************
    //Description: 适配windows下的Sleep和linux下的sleep（参数前者是毫秒，后者是秒）
    // Parameter:  DWORD dwMilliseconds 毫秒
    // Returns:    void
    //
    //************************************
    static void Sleep(DWORD dwMilliseconds);

    //**********************************
    //Description: 获取系统启动后的滴答时间，毫秒
    // Parameter:  void
    // Returns:    DWORD
    // note：      该返回计数都有溢出，请确保使用无符号数
    //************************************
    static DWORD GetTickCount (void);

    //**********************************
    //Description: 获取当前时间(HH:MM:SS),并转化为DATE类型
    // Parameter:  void
    // Returns:    DATE
    // note：      win下返回0.000000000000-0.99999999999999之间的DATE
    //             linux下返回秒数
    //************************************
    static DATE get_current_time();

    //**********************************
    //Description: 蜂鸣函数
    // Parameter:  DWORD dwFreq 频率
    //             DWORD dwDuration 持续时长
    // Returns:    void
    //
    //************************************
    static void Beep(DWORD dwFreq, DWORD dwDuration);

    //**********************************
    //Description: 获取当前的系统时间
    // Parameter:  LPSYSTEMTIME_EX lpSystemTime_ex 自定义的拓展系统时间
    //              和windows下SYSTEMTIME成员一致，数据类型改为32位int
    // Returns:    void
    //
    //************************************
    static void GetLocalTime(LPSYSTEMTIME_EX lpSystemTime_ex);

    //**********************************
    //Description: 获取当前的系统时间(加上一个时间偏移)
    // Parameter:  LPSYSTEMTIME_EX lpSystemTime_ex 自定义的拓展系统时间
    //              和windows下SYSTEMTIME成员一致，数据类型改为32位int
    // Returns:    void
    //
    //************************************
    static void GetLocalTimeEx(LPSYSTEMTIME_EX lpSystemTime_ex, unsigned int delay);

	//**********************************
    //Description: 获取当前的系统时间(加上一个时间偏移)
    // Parameter:  LPSYSTEMTIME_EX lpSystemTime_ex 自定义的拓展系统时间
    //              和windows下SYSTEMTIME成员一致，数据类型改为32位int
    // Returns:    void
    //
    //************************************
	static void GetLocalTimeExEx(LPSYSTEMTIME_EX lpSystemTime_ex, unsigned int delay);

    //**********************************
    //Description: 检测文件或目录是否存在
    // Parameter:  LPCTSTR pszPath 指定目录
    // Returns:    BOOL
    //************************************
    static BOOL PathFileExists(LPCTSTR pszPath);

    //**********************************
    //Description: 取当前应用所在目录
    // Parameter:  char* szPath 全路径
    //             int nMaxSize 路径最长值
    // Returns:    void
    //************************************
    static void GetCurrentPath(char* szPath, int nMaxSize);
    
    //**********************************

    // Parameter:  
    //			   char* szPath 全路径
    //             int nMaxSize 路径最长值
    // Returns:    void
    //************************************
	static void GetModuleFileName(char* szPath, int nMaxSize);
	
    //**********************************
    //Description: 创建目录
    // Parameter:  LPCTSTR lpPathName 要创建的目录名
    // Returns:    BOOL
    // Note：      创建多级目录和已经存在的目录会失败
    //************************************
    static BOOL CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);

    //**********************************
    //Description: 枚举指定目录下的文件夹
    // Parameter:  const char* szFolderName 主目录
    //             std::vector<std::string>& vFileName 满足条件的文件夹路径数组
    // Returns:    void
    // Note：
    //************************************
    static void EnumDir(const char* szFolderName, std::vector<std::string>& vFileName);

    //**********************************
    //Description: 枚举指定目录下的文件
    // Parameter:  const char* szFolderName 主目录
    //             std::vector<std::string>& vFileName 满足条件的文件路径数组
    // Returns:    void
    // Note：
    //************************************
    static void EnumFile(const char* szFolderName, const char *szExName, std::vector<std::string>& vFileName);
    static void EnumFile(const char* szFolderName, const char *szExName, std::vector<std::string>& vFileName, int nFileCount);

	//**********************************
    //Description: 枚举指定目录下的文件
    // Parameter:  const char* szFolderName 主目录
    //             std::vector<std::string>& vFileName 满足条件的文件路径数组
    // Returns:    void
    // Note：
    //************************************
    static void EnumFile(const char* szFolderName, const char *szExName, int nDay, std::vector<std::string>& vFileName);

    //**********************************
    //Description: 枚举创建时间距当前一定天数的文件夹
    // Parameter:  const char* szFolderName 主目录
    //             int nDay 天数
    //             std::vector<std::string>& vFileName 满足条件的文件夹路径数组
    // Returns:    void
    // Note：
    //************************************
    static void EnumDirEx(const char* szFolderName, int nDay, std::vector<std::string>& vFileName);

	//**********************************
    //Description: 判断某文件是否在N天之前修改的
    // Parameter:  szFilePath   文件全路径
    //             int nDay 天数
    // Returns:    void
    // Note：
    //************************************
	static BOOL IsFileModifyBeforeDay(const char* szFilePath, int nDay);

    //**********************************
    //Description: shell删除文件或文件夹
    // Parameter:  const char *szFilePath 文件或文件夹全路径
    // Returns:    BOOL
    // Note：      若想删除文件夹路径需这样表示  C:\\aa\bb\*.*
    //************************************
    static BOOL ShellDeleteFiles(const char *szFilePath);

    //**********************************
    //Description: Finds the first occurrence of a substring within a string. The comparison is not case sensitive
    // Parameter:  LPCTSTR lpFirst: Address of the null-terminated string being searched
    //             LPCTSTR lpSrch:Substring to search for
    // Returns:    LPTSTR
    // Note：      不区分大小写查找字串
    //************************************
    static LPTSTR StrStrI(LPCTSTR lpFirst, LPCTSTR lpSrch);

		//不区分大小写内存比较
    static int Memicmp(const void* s1, const void* s2, size_t size);

    ////////////////////////////////////////////////////////////////////////////
    ///        原子操作系列函数兼容                                          ///
    ///                                                                      ///
    ///                                                                      ///
    ////////////////////////////////////////////////////////////////////////////

    //**********************************
    //Description: 原子操作-递增
    // Parameter:  [IN] PLONG  Addend 要递增的数
    // Returns:    LONG  32位有符号整数
    //
    //************************************
    static LONG InterlockedIncrement(IN PLONG  Addend);

    //**********************************
    //Description: 原子操作-递减
    // Parameter:  [IN] PLONG  Addend （long*)
    // Returns:    LONG  32位有符号整数
    //************************************
    static LONG InterlockedDecrement(IN PLONG  Addend);

    //**********************************
    //Description: 原子操作-进行Addend 与Value相加
    // Parameter:  IN OUT PLONG  Addend,
    //             IN LONG  Value
    // Returns:    LONG  Addend 相加前的值
    //************************************
    static LONG InterlockedExchangeAdd(IN OUT PLONG  Addend,IN LONG  Value);


    //**********************************
    //Description: 原子操作-比较交换
    // Parameter:  [IN] [OUT] PLONG  Destination （long*)
    //             [IN] LONG  Exchange
    //             [IN] LONG  Comparand
    // Returns:    LONG  32位有符号整数
    //************************************
    static LONG InterlockedCompareExchangeEx(
                    IN OUT PLONG  Destination,
                    IN LONG  Exchange,
                    IN LONG  Comparand
                    );

    //**********************************
    //Description: 原子操作-交换
    // Parameter:  [IN] [OUT] PLONG  Target （long*)
    //             [IN] LONG  Value
    // Returns:    LONG  32位有符号整数
    //************************************
    static LONG InterlockedExchange(
                    IN OUT PLONG  Target,
                    IN LONG  Value
                    );



	//**********************************
	//Description: 写INI配置文件 写入字符串
	// Parameter:  与windows接口一致，参照MSDN
	// Returns:    bool
	// Note：      linux下大小写敏感
	//************************************
	static BOOL WritePrivateProfileString(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpString, LPCSTR lpFileName);

    //**********************************
    //Description: 读INI配置文件 读取int整数
    // Parameter:  与windows接口一致，参照MSDN
    // Returns:    UINT
    // Note：      linux下大小写敏感
    //************************************
    static UINT GetPrivateProfileInt(
              LPCTSTR lpAppName,  // section name
              LPCTSTR lpKeyName,  // key name
              INT nDefault,       // return value if key name not found
              LPCTSTR lpFileName  // initialization file name
               );

    //**********************************
    //Description: 读INI配置文件 读取字符串
    // Parameter:  与windows接口一致，参照MSDN
    // Returns:    DWORD
    // Note：      linux下大小写敏感
    //************************************
    static DWORD GetPrivateProfileString(
              LPCTSTR lpAppName,        // section name
              LPCTSTR lpKeyName,        // key name
              LPCTSTR lpDefault,        // default string
              LPTSTR lpReturnedString,  // destination buffer
              DWORD nSize,              // size of destination buffer
              LPCTSTR lpFileName        // initialization file name
               );

    //**********************************
    //Description: 初始化socket环境
    // Parameter:  void
    // Returns:    BOOL
    //************************************
    static BOOL InitSocket(void);

    //**********************************
    //Description: 终止socket环境
    // Parameter:  void
    // Returns:    BOOL
    //************************************
    static BOOL UninitSocket(void);

    //**********************************
    //Description: 关闭socket套接字
    // Parameter:  SOCKET& sockfd
    // Returns:    int 成功返回0
    //************************************
    static int CloseSocket(SOCKET& sockfd);

    //**********************************
    //Description: 关闭socket套接字
    // Parameter:  SOCKET s
    // Returns:    BOOL
    //************************************
    static int closesocket(SOCKET s);

    //**********************************
    //Description: 初始化网络环境环境
    // Parameter:  WORD wVersionRequested,LPWSADATA lpWSAData
    // Returns:    int
    //************************************
    static int WSAStartup(WORD wVersionRequested,LPWSADATA lpWSAData);


    //**********************************
    //Description: 终止socket环境
    // Parameter:  void
    // Returns:    int 成功返回0
    //************************************
    static int WSACleanup(void);

	//**********************************
    //Description: 字符串大小写转换
    // Parameter:  char* szStr
    // Returns:    char*
    //************************************
    static char* strupr(char *szStr);
	static char* strlow(char *szStr);

	//去右空格
    static void TrimRight(char *pchScr);
	//去左空格
	static void TrimLeft(char *pchScr);
	
	//文件行读,参数说明见fgets函数,功能实现CStdioFile中ReadString的功能
	static char *ReadString(char *string, int n, FILE *stream);

	//获得计算机当前使用的内存情况包括物理几存和虚拟内存
	static void GlobalMemoryStatus(LPMEMORYSTATUSEX lpBuffer);

	//设置线程的名称(暂只在LINUX版本有效)
	static void SetThreadName(const char* szThreadName);

	//zhubo 2018-01-29  兼容centos下readdir d_type不能获取的问题
	static bool IsDir(std::string& strPath);
	static bool IsRegularFile(std::string& strPath);
	//zhubo 2018-01-29

protected:
    //去除字符串中指定的字符
    static void RemoveChar(char *from, char Del);
    //用于去除指定字符串的空格，但'='之后的空格不去除
    static void RemoveSpace(char *from);
    //linux版本ltoa
    static char* ltoa_linux(long value, char *string, int radix);
    //linux版本itoa
    static char* itoa_linux(int value, char *string, int radix);
    //linux版本i64toa
    static char* i64toa_linux(__int64 value, char *string, int radix);
    static char* ui64toa_linux(unsigned _int64 value, char *string, int radix);

};

struct  Systime_Bigint
{
	INT64 m_ISystime;
	Systime_Bigint()
	{
		SYSTEMTIME_EX t;
		CAdapter::GetLocalTime(&t);
		m_ISystime = FormatSystimeToBigint(t);
	}
	Systime_Bigint(INT64 ISystime)
	{
		//Systime_Bigint();
		if (0 == ISystime)
		{
			SYSTEMTIME_EX t;
			CAdapter::GetLocalTime(&t);
			ISystime = FormatSystimeToBigint(t);
		}
		m_ISystime = ISystime;
	}
	UINT GetYear()
	{
		return GetYear(m_ISystime);
	}
	UINT GetYear(INT64 ISystime)
	{
		UINT nYear  = (UINT)(ISystime >> 36) & 0x3fff;		//14  bit
		return nYear;
	}
	UINT GetMonth()
	{
		return GetMonth(m_ISystime);
	}
	UINT GetMonth(INT64 ISystime)
	{
		UINT nMonth  = (UINT)(ISystime >> 32) & 0x0f;		//4  bit
		return nMonth;
	}
	UINT GetDay()
	{
		return GetDay(m_ISystime);
	}
	UINT GetDay(INT64 ISystime)
	{
		UINT nDay  = (UINT)(ISystime >> 27) & 0x1f;        //5  bit
		return nDay;
	}
	UINT GetHour()
	{
		return GetHour(m_ISystime);
	}
	UINT GetHour(INT64 ISystime)
	{
		UINT nHour = (UINT)(ISystime >> 22) & 0x1f;        //5  bit
		return nHour;
	}
	UINT GetMin()
	{
		return GetMin(m_ISystime);
	}
	UINT GetMin(INT64 ISystime)
	{
		UINT nMin  = (UINT)(ISystime >> 16) & 0x3f;        //6  bit
		return nMin;
	}
	UINT GetSec()
	{
		return GetSec(m_ISystime);
	}
	UINT GetSec(INT64 ISystime)
	{
		UINT nSec  = (UINT)(ISystime >> 10) & 0x3f;        //6  bit
		return nSec;
	}
	UINT GetMillSec()
	{
		return GetMillSec(m_ISystime);
	}
	UINT GetMillSec(INT64 ISystime)
	{
		UINT nMillSec  = (UINT)(ISystime ) & 0x3ff;		//10  bit
		return nMillSec;
	}
	INT64 FormatSystimeToBigint(SYSTEMTIME_EX t)
	{		
		MakeTimeValid(&t);
		UINT nYear = t.m_iYear;				//14 bit
		UINT nMonth = t.m_iMonth;			//4 bit
		UINT nDay = t.m_iDay;				//5 bit
		UINT nHour = t.m_iHour;				//5 bit
		UINT nMin = t.m_iMinute;				//6 bit
		UINT nSec = t.m_iSecond;				//6 bit
		UINT nMillSec = t.m_iMilliseconds;	//10 bit
		
		INT64 j = 0;
		j |= (INT64)(nYear & 0x3fff) << 36;
		j |= (INT64)(nMonth & 0x0f) << 32;
		j |= (INT64)(nDay & 0x1f) << 27;			
		j |= (INT64)(nHour & 0x1f) << 22;		
		j |= (INT64)(nMin & 0x3f) << 16;		
		j |= (INT64)(nSec & 0x3f) << 10;
		j |= (INT64)(nMillSec & 0x3ff);
		return j;
	}
	INT64 GetNowTimeAsInt64D()
	{	
		SYSTEMTIME_EX t;
		CAdapter::GetLocalTime(&t);
		return FormatSystimeToBigint(t);
	}
	void FormatBigintToSystime(SYSTEMTIME_EX *lpSystime)
	{	
		FormatBigintToSystime(m_ISystime, lpSystime);
		return ;
	}
	void FormatBigintToSystime(INT64 ISystime, SYSTEMTIME_EX *lpSystime)
	{	
		if (NULL == lpSystime)
		{
			return;
		}
		if (0 == ISystime)
		{
			SYSTEMTIME_EX t;
			CAdapter::GetLocalTime(&t);
			ISystime = FormatSystimeToBigint(t);
		}
		INT64 j = ISystime;
		UINT nYear  = (UINT)(j >> 36) & 0x3fff;		//14  bit
		UINT nMonth  = (UINT)(j >> 32) & 0x0f;		//4  bit
		UINT nDay  = (UINT)(j >> 27) & 0x1f;        //5  bit
		UINT nHour = (UINT)(j >> 22) & 0x1f;        //5  bit
		UINT nMin  = (UINT)(j >> 16) & 0x3f;        //6  bit
		UINT nSec  = (UINT)(j >> 10) & 0x3f;        //6  bit
		UINT nMillSec  = (UINT)(j ) & 0x3ff;		//10  bit
		lpSystime->m_iYear = nYear;
		lpSystime->m_iMonth = nMonth;
		lpSystime->m_iDay = nDay;
		lpSystime->m_iHour = nHour;
		lpSystime->m_iMinute = nMin;
		lpSystime->m_iSecond = nSec;
		lpSystime->m_iMilliseconds = nMillSec;
		MakeTimeValid(lpSystime);
		return ;
	}
	INT64 FormatStimeToItime(const char * lpStrTime)
	{	
		INT64 j = 0;
		if (lpStrTime)
		{			
			SYSTEMTIME_EX sysTimeEx;
			sscanf(lpStrTime, "%04d-%02d-%02d %02d:%02d:%02d.%03d"
				, &sysTimeEx.m_iYear
				, &sysTimeEx.m_iMonth
				, &sysTimeEx.m_iDay
				, &sysTimeEx.m_iHour
				, &sysTimeEx.m_iMinute
				, &sysTimeEx.m_iSecond
				, &sysTimeEx.m_iMilliseconds);				
			
			MakeTimeValid(&sysTimeEx);

			j |= (INT64)((UINT)sysTimeEx.m_iYear & 0x3fff) << 36;
			j |= (INT64)((UINT)sysTimeEx.m_iMonth & 0x0f) << 32;
			j |= (INT64)((UINT)sysTimeEx.m_iDay & 0x1f) << 27;			
			j |= (INT64)((UINT)sysTimeEx.m_iHour & 0x1f) << 22;		
			j |= (INT64)((UINT)sysTimeEx.m_iMinute & 0x3f) << 16;		
			j |= (INT64)((UINT)sysTimeEx.m_iSecond & 0x3f) << 10;
			j |= (INT64)((UINT)sysTimeEx.m_iMilliseconds & 0x3ff);
		}
		return j;
	}
	void FormatItimeToStime(char * lpStrTime,INT64 Itime)
	{	
		if (lpStrTime)
		{	
			INT64 j = Itime;
			SYSTEMTIME_EX t;
			t.m_iYear		= (UINT)(j >> 36) & 0x3fff;		//14  bit
			t.m_iMonth		= (UINT)(j >> 32) & 0x0f;		//4  bit
			t.m_iDay		= (UINT)(j >> 27) & 0x1f;        //5  bit
			t.m_iHour		= (UINT)(j >> 22) & 0x1f;        //5  bit
			t.m_iMinute		= (UINT)(j >> 16) & 0x3f;        //6  bit
			t.m_iSecond		= (UINT)(j >> 10) & 0x3f;        //6  bit
			t.m_iMilliseconds  = (UINT)(j ) & 0x3ff;		//10  bit
			MakeTimeValid(&t);
			sprintf(lpStrTime, "%04d-%02d-%02d %02d:%02d:%02d.%03d"
				, t.m_iYear, t.m_iMonth, t.m_iDay, t.m_iHour, t.m_iMinute, t.m_iSecond, t.m_iMilliseconds);
		}
	}
	void GetStdStringSystime(char * lpStrTime, LPSYSTEMTIME_EX lpSysTime)
	{
		SYSTEMTIME_EX t;
		FormatBigintToSystime(&t);
		MakeTimeValid(&t);
		if (lpStrTime)
		{
			sprintf(lpStrTime, "%04d-%02d-%02d %02d:%02d:%02d.%03d", t.m_iYear, t.m_iMonth, t.m_iDay, t.m_iHour, t.m_iMinute, t.m_iSecond, t.m_iMilliseconds);
		}
		if (lpSysTime)
		{
			*lpSysTime = t;
		}
	}
	//确保时间的合理性
	void MakeTimeValid(LPSYSTEMTIME_EX lpSysTime)
	{
		bool bInvalid = false;
		//年 1900-2099
		if (lpSysTime->m_iYear > 2099 || lpSysTime->m_iYear < 1900)
		{
			bInvalid = true;
		}
		//月 1-12
		else if (lpSysTime->m_iMonth > 12 || lpSysTime->m_iMonth <1)
		{
			bInvalid = true;
		}
		//日 1-31
		else if (lpSysTime->m_iDay > 31 || lpSysTime->m_iDay < 1)
		{
			bInvalid = true;
		}
		//时 0-23
		else if (lpSysTime->m_iHour > 23 || lpSysTime->m_iHour < 0)
		{
			bInvalid = true;
		}
		//分 0-59
		else if (lpSysTime->m_iMinute > 59 || lpSysTime->m_iMinute < 0)
		{
			bInvalid = true;
		}
		//秒 0-59
		else if (lpSysTime->m_iSecond > 59 || lpSysTime->m_iSecond < 0)
		{
			bInvalid = true;
		}
		//毫秒 0-999
		else if (lpSysTime->m_iMilliseconds > 999 || lpSysTime->m_iMilliseconds < 0)
		{
			bInvalid = true;
		}
		if (bInvalid)
		{
			CAdapter::GetLocalTime(lpSysTime);
		}
	}
};
char* strstri(const char* pString, const char* pFind);
#endif

