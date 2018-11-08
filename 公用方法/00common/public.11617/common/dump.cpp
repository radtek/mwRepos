#include "stdafx.h"
#include "dump.h"
#include <stdio.h>

#ifdef WIN32
//全局信息
struct 
{	
	HMODULE	hKernel32;
	HMODULE hDbgHelp;
	MINIDUMP_WRITE_DUMP	MiniDumpWriteDump_;
	CREATE_TOOL_HELP32_SNAPSHOT	CreateToolhelp32Snapshot_;
	MODULE32_FIRST	Module32First_;
	MODULE32_NEST	Module32Next_;
}g_WinDdmpDepend;

LONG WINAPI UnhandledExceptionFilter_Dump(LPEXCEPTION_POINTERS pException)
{
	SYSTEMTIME t;		
	HANDLE	hDump_File;		
	char szDMPFile[MAX_PATH+1] = {0};
	char szModulePath[MAX_PATH+1] = {0};

	::GetModuleFileName(NULL,szModulePath,MAX_PATH);	
	strcpy(szDMPFile, szModulePath);
	char* pDot = strrchr(szDMPFile, '.');	
	::GetLocalTime(&t);		
	if (NULL != pDot)
	{
		
		int nLeftLen = MAX_PATH - (pDot - szDMPFile);		
		_snprintf(pDot, nLeftLen, "_%04d%02d%02d_%02d%02d%02d_%03d.dmp", t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,t.wMilliseconds);			
		
	}
	else
	{		
		_snprintf(szDMPFile, MAX_PATH, "\\%04d%02d%02d_%02d%02d%02d_%03d.dmp", t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,t.wMilliseconds);			
	}
	
	//创建并输出异常点信息到文件
	MINIDUMP_EXCEPTION_INFORMATION	M;			
	M.ThreadId = GetCurrentThreadId();
	M.ExceptionPointers = pException;
	M.ClientPointers = FALSE;
	
	//创建dump文件
	hDump_File = CreateFile(szDMPFile,
		GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (NULL != hDump_File)
	{				
		//写入常点信息到文件
		g_WinDdmpDepend.MiniDumpWriteDump_(
			GetCurrentProcess(),
			GetCurrentProcessId(),
			hDump_File,
			MiniDumpNormal, (pException) ? &M : NULL, NULL, NULL
			);
		//关闭句柄
		CloseHandle(hDump_File);
	}
	else
	{
		
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

bool CDump::SetDumpOpen()
{	
	g_WinDdmpDepend.hDbgHelp = LoadLibrary("DBGHELP.DLL");
	if (NULL != g_WinDdmpDepend.hDbgHelp)
	{
		g_WinDdmpDepend.MiniDumpWriteDump_ = (MINIDUMP_WRITE_DUMP)GetProcAddress(g_WinDdmpDepend.hDbgHelp, "MiniDumpWriteDump");
		g_WinDdmpDepend.hKernel32 = GetModuleHandle("KERNEL32");
		if (NULL != g_WinDdmpDepend.hKernel32)
		{
			g_WinDdmpDepend.CreateToolhelp32Snapshot_ = (CREATE_TOOL_HELP32_SNAPSHOT)GetProcAddress(g_WinDdmpDepend.hKernel32, "CreateToolhelp32Snapshot");
			g_WinDdmpDepend.Module32First_ = (MODULE32_FIRST)GetProcAddress(g_WinDdmpDepend.hKernel32, "Module32First");
			g_WinDdmpDepend.Module32Next_ = (MODULE32_NEST)GetProcAddress(g_WinDdmpDepend.hKernel32, "Module32Next");
		}		
	}
	if (NULL != g_WinDdmpDepend.MiniDumpWriteDump_ \
		&& NULL != g_WinDdmpDepend.CreateToolhelp32Snapshot_\
		&& NULL != g_WinDdmpDepend.Module32First_\
		&& NULL != g_WinDdmpDepend.Module32Next_
		)
	{
		SetUnhandledExceptionFilter(UnhandledExceptionFilter_Dump);
		return true;
	}

	return false;
}
#elif defined LINUX
bool CDump::SetDumpOpen()
{
	return true;
}
#endif
