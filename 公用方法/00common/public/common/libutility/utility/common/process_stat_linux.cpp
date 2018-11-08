#include "utility/common/mwcorepch.h"

#ifdef LINUX

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "utility/common//process_stat.h"

static uint64_t s_nPidCpu = 0;//前次调用时当前进程cpu量
static uint64_t s_nSumCpu = 0;//前次调用时主机cpu量
int SubStr(char **szSvr,char *szSubOut,char *szCutFlag)
{
    if (NULL == *szSvr || NULL == szSubOut || NULL == szCutFlag)
    {
        return -1;
    }
    int nFlagLen = strlen(szCutFlag);
    int nSub = 0;
    char *pTemp = *szSvr;
    char *pTempFlag = szCutFlag;
    bool bFlag = false;
    while (**szSvr != '\0')
    {
        while (1)
        {
            if(!memcmp(*szSvr,pTempFlag,nFlagLen))
            {
                if (!bFlag)
                {
                    memcpy(szSubOut,pTemp,nSub);
                    szSubOut[nSub] = '\0';
                    bFlag = true;
                }
                *szSvr += nFlagLen;
                nSub += nFlagLen;
            }
            else
            {
                break;
            }
        }
        if (bFlag)
        {
            return nSub;
        }
        else
        {
            ++nSub;
            ++(*szSvr);
        }
    }
    if (**szSvr == '\0')
    {
        strcpy(szSubOut,pTemp);
        *szSvr = NULL;
    }
    return nSub;
}

void TrimLeft(char *pchScr)
{
    if (pchScr != NULL)
    {
        int nLen = strlen(pchScr);
        int nAdd = 0;
        while (1)
        {
            if (pchScr[nAdd] == 32)
            {
                ++nAdd;
                continue;
            }
            else
            {
                char *pTemp = new char[nLen - nAdd + 1];
                if (pTemp != NULL)
                {
                    memset(pTemp,0,nLen - nAdd + 1);
                    memcpy(pTemp,pchScr + nAdd,nLen - nAdd);
                    memset(pchScr,0,nLen);
                    strcpy(pchScr,pTemp);
                    delete [] pTemp;
                    pTemp = NULL;
                }
                break;
            }
        }
    }
}

int read_file(char *szFileName,char *szOutBuf,int nBufLen)
{
    int nRes = 0;
    int fd = open(szFileName,O_RDONLY);
    if(fd != -1)
    {
        nRes = read(fd,szOutBuf,nBufLen);
        close(fd);
    }
    return nRes;
}

// 获取当前进程cpu使用率(%)，返回-1失败，大于等于0成功，首次调用返回0，相当于初始化
int get_cpu_usage()
{
    int nRes = 0;
    uint64_t nSumCpu = 0,nPidCpu = 0;
    uint64_t nCpuUsr = 0;
    char szCpuTemp[20] = {0};
    char szFileName[256+1] = {0};
    char szBuf[1024 * 2] = {0};
    sprintf(szFileName, "/proc/stat");
    int nRead = read_file(szFileName,szBuf,sizeof(szBuf));
    if(nRead != -1)
    {
        char *p = szBuf;
        for(int nAdd = 0;nAdd < 9;++nAdd)
        {
            memset(szCpuTemp,0,sizeof(szCpuTemp));
            int nRet = SubStr(&p,szCpuTemp,(char *)" ");
            if (nRet > 0)
            {
                nSumCpu += atoll(szCpuTemp);
            }
            else
            {
                return -1;
            }
        }
    }
    memset(szFileName, 0, sizeof(szFileName));
    sprintf(szFileName,"/proc/%d/stat",getpid());
    memset(szBuf,0,sizeof(szBuf));
    int nReadPid = read_file(szFileName,szBuf,sizeof(szBuf));
    if(nReadPid != -1)
    {
        char *p = szBuf;
        for(int nAdd = 0;nAdd < 15;++nAdd)
        {
            memset(szCpuTemp,0,sizeof(szCpuTemp));
            int nRet = SubStr(&p,szCpuTemp,(char *)" ");
            if (nRet > 0)
            {
                if(nAdd >= 13)
                {
                    nPidCpu += atoll(szCpuTemp);
                }
            }
            else
            {
                return -1;
            }
        }
    }
    
    nCpuUsr = nSumCpu - s_nSumCpu;
    if(nCpuUsr > 0)
    {
        nRes = ((nPidCpu - s_nPidCpu) * 100 / nCpuUsr);
    }
    s_nSumCpu = nSumCpu;
    s_nPidCpu = nPidCpu;
    return nRes ;
}

// 获取当前进程内存和虚拟内存使用量，返回-1失败，0成功
int get_memory_usage(uint64_t* mem, uint64_t* vmem)
{
    int nRes = -1;
    char szTemp[20] = {0};
    char szFileName[256+1] = {0};
    char szBuf[1024 * 2] = {0};
    sprintf(szFileName,"/proc/%d/status",getpid());
    int nReadPid = read_file(szFileName,szBuf,sizeof(szBuf));
    if(nReadPid != -1)
    {
        char *p = strstr(szBuf,"VmSize:");
        if (p != NULL)
        {
            p = p + strlen("VmSize:");
            TrimLeft(p);
            memcpy(szTemp,p,sizeof(szTemp) - 1);
            *vmem = atoll(szTemp);
        }

        p = strstr(szBuf,"VmRSS:");
        if (p != NULL)
        {
            p = p + strlen("VmRSS:");
            TrimLeft(p);
            memset(szTemp,0,sizeof(szTemp));
            memcpy(szTemp,p,sizeof(szTemp) - 1);
            *mem  = atoll(szTemp);
        }
        nRes = 0;
    }
    *mem = *mem * 1024;
    *vmem =*vmem * 1024;
    return nRes ;
}

// 获取当前进程总共读和写的IO字节数，返回-1失败，0成功
int get_io_bytes(uint64_t* read_bytes, uint64_t* write_bytes)
{
    return 0;
}

//取磁盘空余量,返回-1失败，大于等于0成功,返回的单位为k
int get_disk_free_space(const char* driver)
{
    int nRes = -1;
    if (driver != NULL)
    {
        struct statfs sfs;
        if(0 == statfs(driver, &sfs))
        {
            nRes = (sfs.f_bfree * 4) / 1024;
        }
    }
    return nRes;
}

#endif