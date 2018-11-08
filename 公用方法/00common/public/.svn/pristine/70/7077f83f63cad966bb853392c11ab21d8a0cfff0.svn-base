#include "utility/common/mwcorepch.h"
#include "utility/common/FindFile.h"

CFindFile::CFindFile()
{
	memset(&m_cFileInfo, 0, sizeof(m_cFileInfo));
	memset(m_szFilePath, 0, sizeof(m_szFilePath));
	m_hFile = 0;
	m_sPtrErr = NULL;
}

CFindFile::~CFindFile()
{
	if (m_hFile != 0)
		CFindFile::FindClose(m_hFile);
}

bool CFindFile::EnumAllFile(const char* sPath, _CallBackFindFile pFunc, void *lpParama, bool bIsListSub /* = true */)
{
	FILEFINDINFO fileInfo;
	FILE_HANDLE hFile = CFindFile::FindFirstFile(sPath, &fileInfo);
	if (hFile == 0) return false;

	// 保存当前路径
	char sDirList[MAX_PATH];
	// 遍历文件
	do
	{
		strcpy(sDirList, sPath);
		// 用户返回值大于0时，说明想要退出遍历
		if (pFunc(sDirList, &fileInfo, lpParama) > 0)
		{
			CFindFile::FindClose(hFile);
			return true;
		}

		// 深入子目录继续遍历
		if (bIsListSub == true && fileInfo.fwFileAttributes == FILE_DIR)
		{
			// 去掉子父目录
			if (strcmp(fileInfo.szFileName, ".") != 0 &&
				strcmp(fileInfo.szFileName, "..") != 0)
			{
				strcat(sDirList, "/");
				strcat(sDirList, fileInfo.szFileName);
				EnumAllFile(sDirList, pFunc, lpParama, bIsListSub);
			}
		}
	} while (CFindFile::FindNextFile(hFile, &fileInfo));

	CFindFile::FindClose(hFile);

	return true;
}

void CFindFile::_CloseHandle()
{
	if (m_hFile)
	{
		CFindFile::FindClose(m_hFile);
		m_hFile = 0;
	}
}

bool CFindFile::FindFile(const char* sPath)
{
	_CloseHandle();
	m_hFile = CFindFile::FindFirstFile(sPath, &m_cFileInfo);
	if (m_hFile == 0)
	{
		CFindFile::Global_GetLastErrr(m_sPtrErr);
		return false;
	}
	strcpy(m_szFilePath, sPath);
	return true;
}

bool CFindFile::FindNext()
{
	return CFindFile::FindNextFile(m_hFile, &m_cFileInfo) ? true : false;
}

const char* CFindFile::GetFileName()
{
	return m_cFileInfo.szFileName;
}

const char* CFindFile::GetFilePath()
{
	// return m_cFileInfo.szFilePath;
	return m_szFilePath;
}
const char* CFindFile::GetLastError()
{
	return m_sPtrErr;
}

int CFindFile::GetFileSize(const char *szFileName /* = NULL */)
{
	if (!IsDir())
	{
		if (!szFileName)
		{
			char szFilePath[MAX_PATH] = { 0 };
			strcpy(szFilePath, GetFilePath());
			int len = strlen(szFilePath);
			if (szFilePath[len - 1] != '/')
			{
				szFilePath[len++] = '/';
			}
			strcpy(szFilePath + len, GetFileName());

			struct stat fileinfo;
			stat(szFilePath, &fileinfo);
			return fileinfo.st_size;
		}
		else
		{
			return m_cFileInfo.fileinfo.st_size;
		}
	}
	return 0;
}

bool CFindFile::IsDir()
{
	return (m_cFileInfo.fwFileAttributes == FILE_DIR);
}

void CFindFile::Global_GetLastErrr(char* &sInfo)
{
#ifdef WIN32
	void *hocal = NULL;
	DWORD iErr = ::GetLastError();
	hocal = LocalAlloc(0, MAX_PATH * 3);
	DWORD hOk = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
							  FORMAT_MESSAGE_IGNORE_INSERTS, NULL, iErr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
							  (LPTSTR)&hocal, MAX_PATH * 3, NULL);
	
	if (hOk)
	{
		if (hocal == NULL) { sInfo = NULL; return; }
		size_t iSize = strlen((const char*)hocal);
		if (sInfo) { delete[]sInfo; sInfo = NULL; }
		sInfo = new char[iSize + 1];
		strcpy(sInfo, (const char*)hocal);
		LocalFree(hocal);
	}
#elif defined(LINUX)
	int nErr = errno;
	const char *sErr = strerror(nErr);
	if (sInfo){ delete[]sInfo; sInfo = NULL; }
	int iSize = strlen(sErr);
	sInfo = new char[iSize + 1];
	strcpy(sInfo, sErr);
#endif
}

#ifdef WIN32
static BOOL In_IsDirent(const char* szDir, PWIN32_FIND_DATA pData)
{
	HANDLE hFile = ::FindFirstFile(szDir, pData);
	if (hFile)
	{
		if (pData->dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			::FindClose(hFile);
			return TRUE;
		}
		::FindClose(hFile);
	}
	return FALSE;
}
#endif

void CFindFile::FindClose(FILE_HANDLE hFile)
{
#ifdef WIN32
	if (hFile)
	{
		FIND_FILE_HANDLE* pHandle = (FIND_FILE_HANDLE*)hFile;
		::FindClose(pHandle->hFile);
		delete pHandle;
	}
#elif defined(LINUX)
	if(hFile)
	{
		FIND_FILE_HANDLE* pHandle = (FIND_FILE_HANDLE*)hFile;
		closedir(pHandle->pDir);
		delete pHandle;
	}
#endif
}

FILE_HANDLE CFindFile::FindFirstFile(const char *szDir, PFILEFINDINFO pInfo)
{
	FILE_HANDLE hFind = (FILE_HANDLE)NULL;
#ifdef WIN32
	WIN32_FIND_DATA info;
	// 检查路径是否为目录
	if (!In_IsDirent(szDir, &info)) {
		return FILE_HANDLE((HANDLE*)NULL);
	}
	FIND_FILE_HANDLE *pHandle = new FIND_FILE_HANDLE;
	// windows 需要加*.*遍历
	char szFindPath[MAX_PATH] = { 0 };
	strcpy(szFindPath, szDir);
	PathAppend(szFindPath, "/*.*");
	pHandle->hFile = ::FindFirstFile(szFindPath, &info);
	if (pHandle->hFile == INVALID_HANDLE_VALUE) {
		delete pHandle;
		return FILE_HANDLE((HANDLE*)NULL);
	}
	strcpy(pInfo->szFileName, info.cFileName);
	if (info.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
		pInfo->fwFileAttributes = FILE_DIR;
	}
	else {
		pInfo->fwFileAttributes = FILE_NORMAL;
	}
	strcpy(pHandle->szFilePath, szDir);
	
	strcpy(szFindPath, szDir);
	if(szFindPath[strlen(szFindPath)-1] != '/')
	{
		strcat(szFindPath, "/");
	}
	strcat(szFindPath, pInfo->szFileName);
	stat(szFindPath, &pInfo->fileinfo);

	hFind = (FILE_HANDLE)pHandle;
#elif defined(LINUX)
	FIND_FILE_HANDLE *pHandle = new FIND_FILE_HANDLE;
	if(pHandle)
	{
		pHandle->pDir = opendir(szDir);
		if (pHandle->pDir != NULL)
		{
			struct dirent *dirInfo = readdir(pHandle->pDir);
			if (dirInfo)
			{
				char szFileName[MAX_PATH];

				strcpy(szFileName, szDir);
				if(szFileName[strlen(szFileName)-1] != '/') strcat(szFileName, "/");
				strcat(szFileName, dirInfo->d_name);

				stat(szFileName, &pInfo->fileinfo);
				strcpy(pInfo->szFileName, dirInfo->d_name);
				if (S_ISDIR(pInfo->fileinfo.st_mode)) {
					pInfo->fwFileAttributes = FILE_DIR;
				}
				else {
					pInfo->fwFileAttributes = FILE_NORMAL;
				}

				strcpy(pHandle->szFilePath, szDir);
				hFind = (FILE_HANDLE)pHandle;
			}
			else {
				closedir(pHandle->pDir);
				delete pHandle;
				return (FILE_HANDLE)NULL;
			}
		}
	}
#endif

	return hFind;
}

BOOL CFindFile::FindNextFile(FILE_HANDLE hFile, PFILEFINDINFO pInfo)
{
	BOOL bRet = TRUE;
#ifdef WIN32
	WIN32_FIND_DATA info;
	FIND_FILE_HANDLE* pHandle = (FIND_FILE_HANDLE*)hFile;
	bRet = ::FindNextFile((HANDLE)pHandle->hFile, &info);
	if (bRet)
	{
		strcpy(pInfo->szFileName, info.cFileName);
		if (info.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY){
			pInfo->fwFileAttributes = FILE_DIR;
		}
		else {
			pInfo->fwFileAttributes = FILE_NORMAL;
		}

		char szFindPath[MAX_PATH] = { 0 };
		strcpy(szFindPath, pHandle->szFilePath);
		if(szFindPath[strlen(szFindPath)-1] != '/')
		{
			strcat(szFindPath, "/");
		}
		strcat(szFindPath, pInfo->szFileName);
		stat(szFindPath, &pInfo->fileinfo);
	}
#elif defined(LINUX)
	FIND_FILE_HANDLE* pHandle = (FIND_FILE_HANDLE*)hFile;
	struct dirent *dirInfo = readdir(pHandle->pDir);
	if (dirInfo)
	{
		char szFileName[MAX_PATH] = { 0 };
		strcpy(szFileName, pHandle->szFilePath);
		if(szFileName[strlen(szFileName)-1] != '/') strcat(szFileName, "/");
		strcat(szFileName, dirInfo->d_name);
		
		stat(szFileName, &pInfo->fileinfo);
		strcpy(pInfo->szFileName, dirInfo->d_name);
		if (S_ISDIR(pInfo->fileinfo.st_mode)){
			pInfo->fwFileAttributes = FILE_DIR;
		}
		else{
			pInfo->fwFileAttributes = FILE_NORMAL;
		}
	}
	else {
		bRet = FALSE;
	}
#endif

	return bRet;
}

void CFindFile::PathAppend(char *szSrcDir, const char *szSubDir)
{
	size_t srclen = strlen(szSrcDir);
	size_t sublen = strlen(szSubDir);
	if (srclen + sublen < MAX_PATH)
	{
		if (szSrcDir[srclen - 1] == '/')
		{
			szSrcDir[srclen] = '/';
			strcpy(szSrcDir + srclen + 1, szSubDir);
		}
		else if (szSrcDir[srclen - 1] == '\\')
		{
			szSrcDir[srclen] = '\\';
			strcpy(szSrcDir + srclen + 1, szSubDir);
		}
		else
		{
			strcpy(szSrcDir + srclen, szSubDir);
		}
	}
}