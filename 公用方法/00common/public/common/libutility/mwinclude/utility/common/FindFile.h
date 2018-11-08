#ifndef _FINDFILE_H
#define _FINDFILE_H

#include "utility/common/mwcore_export.h"
#include "utility/common/adapter/Adapter.h"
#include <stdint.h>

// 文件句柄
#define FILE_HANDLE intptr_t

#ifdef LINUX
typedef struct _FIND_FILE_HANDLE_
{
	DIR* pDir;
	char szFilePath[MAX_PATH];
}FIND_FILE_HANDLE;
#elif defined(WIN32)
typedef struct _FIND_FILE_HANDLE_
{
	HANDLE hFile;
	char szFilePath[MAX_PATH];
}FIND_FILE_HANDLE;
#endif

typedef struct _FILEFINDINFOA_
{
	char szFileName[MAX_PATH]; // 文件名称
	DWORD fwFileAttributes;	   // 文件属性
	struct stat fileinfo;	   // 文件的其它属性信息
	//USHORT fileNameSize;	   // 文件名称长度
}FILEFINDINFO, *PFILEFINDINFO;

// 返回值等于0则继续遍历，否则退出遍历
typedef int(__STDCALL *_CallBackFindFile)
(OUT char *szCurPath, OUT PFILEFINDINFO pFileInfo, IN void *lpParama);

class UTILITY_Export CFindFile
{
public:
	enum FILEATTRIBUTE
	{
		FILE_DIR,	 // 目录
		FILE_NORMAL	 // 其它文件
	};
public:
	CFindFile();
	~CFindFile();
	bool FindFile(const char* sPath);
	bool IsDir();
	bool FindNext();
	const char* GetFileName();				 // 获取当前文件名称
	const char* GetFilePath();				 // 获取当前文件路径
	const char* GetLastError();	
	int GetFileSize(const char* szFileName = NULL); // 获取当前文件大小,对目录无效
public:
	/* 枚举目录下的所有文件
	* sPath:目录
	* pFunc:回掉函数，若函数返回值大于0则退出枚举
	* pFileInfo[out]:文件信息
	* szCurPath[out]:当前的文件路径
	* lpParama:用户传入参数
	* 返回值：成功返回true,失败返回false
	*/
	static bool EnumAllFile(const char* sPath,
							_CallBackFindFile pFunc, void *lpParama = NULL,
							bool bIsListSub = true);
public:
	/* 文件遍历，win32中不不用主动加上*.*,文件分割统一用'/'
	* char:szDir
	* pInfo:文件信息
	* 返回值:函数失败返回NULL，成功返回一个有效的句柄
	* 备注：，此函数不支持通配符查找，szDir必须是一个目录
	*/
	static FILE_HANDLE FindFirstFile(const char *szDir, PFILEFINDINFO pInfo);

	/* 继续遍历目录
	* hFile:FindFirstFile返回的句柄
	* pInfo:接受的数据信息
	* 返回值:如果存在文件或目录则返回TRUE，不存在或函数失败返回FALSE
	*/
	static BOOL FindNextFile(FILE_HANDLE hFile, PFILEFINDINFO pInfo);

	/* 关闭目录遍历
	* 参数传入一个FindFirstFile返回的句柄值
	*/
	static void FindClose(FILE_HANDLE);

	static void Global_GetLastErrr(char* &sInfo);

	static void PathAppend(char *szPath, const char* szAppend);
protected:
	void _CloseHandle();
	CFindFile(const CFindFile&);
	CFindFile& operator = (const CFindFile&);
private:
	char *m_sPtrErr;
	char m_szFilePath[MAX_PATH];
	FILE_HANDLE m_hFile;
	FILEFINDINFO m_cFileInfo;
};

#endif