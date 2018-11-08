/*****************************************************************************************************
copyright (C),2015-2020,montnets .Co.,Ltd.

FileName     : FileOperEx.h
Author       : Shijh      Version : 1.0    Date: 2014年09月02日
Description  : 文件操作
Version      : 1.0
Function List:

History      :
<author>       <time>             <version>            <desc>
Shijh       2014年09月02日          1.0          build this moudle
******************************************************************************************************/

#ifndef __FILEOPEREX_H__
#define __FILEOPEREX_H__

#pragma warning(disable:4800)
#pragma warning(disable:4267)

#if (defined  MW_WINDOWS) || (defined  WIN32)
#include <WinBase.h>
#endif

namespace UTI{
namespace COM{

#include "utility/common/mwcore_export.h"
#include <vector>
#include <string>



class UTILITY_Export CFileOperEx
{

public:
    /**************************************************************************************
    * Function      : Inner_IsExistsPath
    * Description   : 判断文件路径是否存在
    * Input         :
    * Output        : bool
    * Return        : [true] - 存在 [false] - 不存在
    * Author        : Shijh      Version : 1.0    Date: 2014年09月02日
    * Modify        : 无
    * Others        : 无
    **************************************************************************************/
    static bool Inner_IsExistsPath(const char* path)
    {
        //WINDOWS编译环境
#if (defined  MW_WINDOWS) || (defined  WIN32)

        DWORD fa = 0;
        fa = GetFileAttributes(path);
        return (fa != (DWORD)-1);
        //linux编译环境
#elif defined LINUX
        return 0 == access(path, R_OK);
#endif
    }

    /**************************************************************************************
    * Function      : Inner_CreatePath
    * Description   : 创建目录路径
    * Input         :
    * Output        : bool
    * Return        : [true] - 成功 [false] - 失败
    * Author        : Shijh      Version : 1.0    Date: 2014年09月02日
    * Modify        : 无
    * Others        : 无
    **************************************************************************************/
    static bool Inner_CreatePath(const char* path)
    {
        //WINDOWS编译环境
#if (defined  MW_WINDOWS) || (defined  WIN32)
        return ::CreateDirectory(path, NULL);
        //linux编译环境
#elif defined LINUX
        //创建已经存在的目录或递归创建目录时将失败
        if (0 == mkdir(path, 0777))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
#endif

    }

    /**************************************************************************************
    * Function      : CreateFullPath
    * Description   : 创建文件全路径
    * Input         :
    * Output        : bool
    * Return        : [true] - 成功 [false] - 失败
    * Author        : Shijh      Version : 1.0    Date: 2014年09月02日
    * Modify        : 无
    * Others        : 无
    **************************************************************************************/
    static bool CreateFullPath(const char* fullPath)
    {
        if (fullPath == NULL || fullPath[0] == 0)
        {
            return false;
        }

        int pos = -1;
        for (int i = strlen(fullPath) - 1; i >= 0; i--)
        {
            if (fullPath[i] == '\\' || fullPath[i] == '/')
            {
                pos = i;
                break;
            }
        }

        if (pos == -1)
        {
            return false;
        }

        char strPath[MAX_PATH] = { 0 };
        strncpy(strPath, fullPath, pos);
        strPath[pos + 1] = '\0';

        if (Inner_IsExistsPath(strPath))
        {
            return true;
        }

        std::vector<std::string> vDir;
        char * p = strPath;
        if (p[0] == '/')
        {
            vDir.push_back("/");
            p++;
        }
        bool bFind = false;
        while (strlen(p) > 0)
        {
            bFind = false;
            for (int i = 0; i < strlen(p); i++)
            {
                if (p[i] == '\\' || p[i] == '/')
                {
                    p[i] = '\0';
                    vDir.push_back(p);
                    p += i + 1;
                    bFind = true;
                    break;
                }
            }

            if (!bFind)
            {
                vDir.push_back(p);
                break;
            }
        }

        if (vDir.empty())
        {
            return false;
        }

        std::string currPath = "";
        for (int i = 0; i < vDir.size(); i++)
        {
            if (i == 0)
            {
                currPath = vDir[i];
            }
            else
            {
                currPath += "/" + vDir[i];
            }

            if (Inner_IsExistsPath(currPath.c_str()))
            {
                continue;
            }

            if (!Inner_CreatePath(currPath.c_str()))
            {
                return false;
            }
        }

        return true;
    }

    /**************************************************************************************
    * Function      : DeleteDirectory
    * Description   : 删除目录
    * Input         :
    * Output        : bool
    * Return        : [true] - 成功 [false] - 失败
    * Author        : Shijh      Version : 1.0    Date: 2016年11月08日
    * Modify        : 无
    * Others        : 无
    **************************************************************************************/
    static bool DeleteDirectory(char* dirName)
    {
        //WINDOWS编译环境
#if (defined  MW_WINDOWS) || (defined  WIN32)
        CFileFind tempFind;
        char sTempFileFind[_MAX_PATH] = { 0 };
        sprintf(sTempFileFind, "%s//*.*", dirName);
        BOOL IsFinded = tempFind.FindFile(sTempFileFind);
        while (IsFinded)
        {
            IsFinded = tempFind.FindNextFile();
            // 删除目录
            if (!tempFind.IsDots())
            {
                char sFoundFileName[_MAX_PATH] = { 0 };
                strcpy(sFoundFileName, tempFind.GetFileName().GetBuffer(200));
                if (tempFind.IsDirectory())
                {
                    char sTempDir[_MAX_PATH] = { 0 };
                    sprintf(sTempDir, "%s//%s", dirName, sFoundFileName);
                    DeleteDirectory(sTempDir);
                }
                // 删除文件目录
//                 else
//                 {
//                     char sTempFileName[_MAX_PATH] = { 0 };
//                     sprintf(sTempFileName, "%s//%s", strDirName, sFoundFileName);
//                     DeleteFile(sTempFileName);
//                 }
            }
        }
        tempFind.Close();
        if (!RemoveDirectory(dirName))
        {
            return false;
        }
#elif defined LINUX

#endif
        return true;
    }

};
}}
#endif // !__FILEOPEREX_H__
