#ifndef _DUMP_H
#define _DUMP_H

#ifdef _DUMP
#define TRY_TYPE try
#define TRY_UNKNOWN if(1)
#define  CATCH_TYPE(e) catch(e)
#define  CATCH_UNKNOWN() if(0)
#else
#define TRY_TYPE try
#define TRY_UNKNOWN try
#define  CATCH_TYPE(e) catch(e)
#define  CATCH_UNKNOWN() catch(...)
#endif

class CDump
{
public:
#ifdef WIN32
	static bool SetDumpOpen();
#elif defined LINUX
	static bool SetDumpOpen()
	{
		return true;
	}
#else
#endif
};


#ifdef WIN32
#include <windows.h>
#include <tlhelp32.h>

#pragma optimize("y", off)		//generate stack frame pointers for all functions - same as /Oy- in the project
#pragma warning(disable: 4200)	//nonstandard extension used : zero-sized array in struct/union
#pragma warning(disable: 4100)	//unreferenced formal parameter


// In case you don't have dbghelp.h.
#ifndef _DBGHELP_

typedef struct _MINIDUMP_CALLBACK_OUTPUT {
	union {
		ULONG ModuleWriteFlags;
		ULONG ThreadWriteFlags;
	};
} MINIDUMP_CALLBACK_OUTPUT, *PMINIDUMP_CALLBACK_OUTPUT;

typedef struct _MINIDUMP_THREAD_CALLBACK {
	ULONG ThreadId;
	HANDLE ThreadHandle;
	CONTEXT Context;
	ULONG SizeOfContext;
	ULONG64 StackBase;
	ULONG64 StackEnd;
} MINIDUMP_THREAD_CALLBACK, *PMINIDUMP_THREAD_CALLBACK;

typedef struct _MINIDUMP_THREAD_EX_CALLBACK {
    ULONG ThreadId;
    HANDLE ThreadHandle;
    CONTEXT Context;
    ULONG SizeOfContext;
    ULONG64 StackBase;
    ULONG64 StackEnd;
    ULONG64 BackingStoreBase;
    ULONG64 BackingStoreEnd;
} MINIDUMP_THREAD_EX_CALLBACK, *PMINIDUMP_THREAD_EX_CALLBACK;

typedef struct _MINIDUMP_MODULE_CALLBACK {
	PWCHAR FullPath;
	ULONG64 BaseOfImage;
	ULONG SizeOfImage;
	ULONG CheckSum;
	ULONG TimeDateStamp;
	VS_FIXEDFILEINFO VersionInfo;
	PVOID CvRecord; 
	ULONG SizeOfCvRecord;
	PVOID MiscRecord;
	ULONG SizeOfMiscRecord;
} MINIDUMP_MODULE_CALLBACK, *PMINIDUMP_MODULE_CALLBACK;

typedef struct _MINIDUMP_INCLUDE_THREAD_CALLBACK {
    ULONG ThreadId;
} MINIDUMP_INCLUDE_THREAD_CALLBACK, *PMINIDUMP_INCLUDE_THREAD_CALLBACK;

typedef struct _MINIDUMP_INCLUDE_MODULE_CALLBACK {
    ULONG64 BaseOfImage;
} MINIDUMP_INCLUDE_MODULE_CALLBACK, *PMINIDUMP_INCLUDE_MODULE_CALLBACK;

typedef struct _MINIDUMP_CALLBACK_INPUT {
	ULONG ProcessId;
	HANDLE ProcessHandle;
	ULONG CallbackType;
	union {
		MINIDUMP_THREAD_CALLBACK Thread;
		MINIDUMP_THREAD_EX_CALLBACK ThreadEx;
		MINIDUMP_MODULE_CALLBACK Module;
		MINIDUMP_INCLUDE_THREAD_CALLBACK IncludeThread;
		MINIDUMP_INCLUDE_MODULE_CALLBACK IncludeModule;
	};
} MINIDUMP_CALLBACK_INPUT, *PMINIDUMP_CALLBACK_INPUT;

typedef enum _MINIDUMP_CALLBACK_TYPE {
	ModuleCallback,
		ThreadCallback,
		ThreadExCallback,
		IncludeThreadCallback,
		IncludeModuleCallback
} MINIDUMP_CALLBACK_TYPE;

typedef enum _MINIDUMP_TYPE {
	MiniDumpNormal                           = 0x00000000,
		MiniDumpWithDataSegs                     = 0x00000001,
		MiniDumpWithFullMemory                   = 0x00000002,
		MiniDumpWithHandleData                   = 0x00000004,
		MiniDumpFilterMemory                     = 0x00000008,
		MiniDumpScanMemory                       = 0x00000010,
		MiniDumpWithUnloadedModules              = 0x00000020,
		MiniDumpWithIndirectlyReferencedMemory   = 0x00000040,
		MiniDumpFilterModulePaths                = 0x00000080,
		MiniDumpWithProcessThreadData            = 0x00000100,
		MiniDumpWithPrivateReadWriteMemory       = 0x00000200,
		MiniDumpWithoutOptionalData              = 0x00000400,
		MiniDumpWithFullMemoryInfo               = 0x00000800,
		MiniDumpWithThreadInfo                   = 0x00001000,
		MiniDumpWithCodeSegs                     = 0x00002000,
		MiniDumpWithoutAuxiliaryState            = 0x00004000,
		MiniDumpWithFullAuxiliaryState           = 0x00008000,
		MiniDumpWithPrivateWriteCopyMemory       = 0x00010000,
		MiniDumpIgnoreInaccessibleMemory         = 0x00020000,
		MiniDumpWithTokenInformation             = 0x00040000 
} MINIDUMP_TYPE;

typedef enum _MODULE_WRITE_FLAGS {
	ModuleWriteModule        = 0x0001,
		ModuleWriteDataSeg       = 0x0002,
		ModuleWriteMiscRecord    = 0x0004,
		ModuleWriteCvRecord      = 0x0008,
		ModuleReferencedByMemory = 0x0010
} MODULE_WRITE_FLAGS;

typedef struct _MINIDUMP_EXCEPTION_INFORMATION {
	DWORD ThreadId;
	PEXCEPTION_POINTERS ExceptionPointers;
	BOOL ClientPointers;
}MINIDUMP_EXCEPTION_INFORMATION, *PMINIDUMP_EXCEPTION_INFORMATION;

typedef  BOOL (CALLBACK* MINIDUMP_CALLBACK_ROUTINE)(
								PVOID CallbackParam,
								CONST PMINIDUMP_CALLBACK_INPUT CallbackInput,
								PMINIDUMP_CALLBACK_OUTPUT CallbackOutput
								);

typedef struct _MINIDUMP_CALLBACK_INFORMATION {
	MINIDUMP_CALLBACK_ROUTINE CallbackRoutine;
	PVOID CallbackParam;
} MINIDUMP_CALLBACK_INFORMATION, *PMINIDUMP_CALLBACK_INFORMATION;

typedef struct _MINIDUMP_USER_STREAM {
	ULONG32 Type;
	ULONG BufferSize;
	PVOID Buffer;
} MINIDUMP_USER_STREAM, *PMINIDUMP_USER_STREAM;

typedef struct _MINIDUMP_USER_STREAM_INFORMATION {
	ULONG UserStreamCount;
	PMINIDUMP_USER_STREAM UserStreamArray;
} MINIDUMP_USER_STREAM_INFORMATION, *PMINIDUMP_USER_STREAM_INFORMATION;


typedef	BOOL (WINAPI * MINIDUMP_WRITE_DUMP)(
											IN HANDLE			hProcess,
											IN DWORD			ProcessId,
											IN HANDLE			hFile,
											IN MINIDUMP_TYPE	DumpType,
											IN CONST PMINIDUMP_EXCEPTION_INFORMATION	ExceptionParam, OPTIONAL
											IN PVOID									UserStreamParam, OPTIONAL
											IN PVOID									CallbackParam OPTIONAL
											);

#else

typedef	BOOL (WINAPI * MINIDUMP_WRITE_DUMP)(
											IN HANDLE			hProcess,
											IN DWORD			ProcessId,
											IN HANDLE			hFile,
											IN MINIDUMP_TYPE	DumpType,
											IN CONST PMINIDUMP_EXCEPTION_INFORMATION	ExceptionParam, OPTIONAL
											IN PMINIDUMP_USER_STREAM_INFORMATION		UserStreamParam, OPTIONAL
											IN PMINIDUMP_CALLBACK_INFORMATION			CallbackParam OPTIONAL
											);
#endif //#ifndef _DBGHELP_

// Tool Help functions.
typedef	HANDLE (WINAPI * CREATE_TOOL_HELP32_SNAPSHOT)(DWORD dwFlags, DWORD th32ProcessID);
typedef	BOOL (WINAPI * MODULE32_FIRST)(HANDLE hSnapshot, LPMODULEENTRY32 lpme);
typedef	BOOL (WINAPI * MODULE32_NEST)(HANDLE hSnapshot, LPMODULEENTRY32 lpme);
#endif

#endif