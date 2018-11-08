#include "utility/common/mwcorepch.h"
#include "utility/common/Global.h"
#include "utility/common/MemList.h"
#include "utility/common/UTF-8.h"
#include "utility/common/md5.h"
#include "sys/stat.h"


#pragma comment(lib, "winmm.lib")

#ifndef maxx
#define maxx(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef minn
#define minn(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX_MSG_LEN
#define MAX_MSG_LEN                 360         //系统支持的最大短信字数（汉字）(720字节)
#endif

#ifndef MAX_SINGLE_LEN
#define MAX_SINGLE_LEN              70          //单条短信最大长度（汉字）
#endif

//报警
#ifdef WIN32
#include <mmsystem.h>
#endif

const UCHAR base64_alphabet[] = {
    'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
        'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
        'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
        'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/',
        '='
};
bool CGlobal::BeepOrNot = TRUE;
DerivedLock g_csMsgId;

CGlobal::CGlobal()
{
}


int MIN2(int a, int b)
{
    return a>b ? b : a;
}

std::string FORMATI64D(const char* format)
{
	std::string strFmt(format);
#ifdef WIN32
	cstr::replace(strFmt, "%lld", "%I64d");
#elif defined LINUX
	cstr::replace(strFmt, "%I64d", "%lld");
#endif
	return strFmt;
}

// time_t CGlobal::SystemTimeToTime_t(const SYSTEMTIME& st)
// {
//     tm temptm = { st.wSecond, st.wMinute, st.wHour, st.wDay,
//         st.wMonth - 1, st.wYear - 1900, st.wDayOfWeek, 0, 0 };
// 
//     return mktime(&temptm);
// }

long CGlobal::StrTimeToLong(const char* szTime)
{
    int nHour = 0;
    int nMin  = 0;
    int nSec  = 0;
    if (NULL != szTime)
    {
        sscanf(szTime, "%d:%d:%d", &nHour, &nMin, &nSec);
    }
    return nHour*3600+nMin*60+nSec;
}

//十六进制字符转换成整数
int CGlobal::HexchartoInt(char chHex)
{
    int iDec = 0;
    switch (chHex)
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        iDec = chHex - '0';
        break;
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
        iDec = chHex - 'A' + 10;
        break;
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
        iDec = chHex - 'a' + 10;
        break;
    default:
        iDec = 0;
        break;
    }
    return iDec;
}

//对短信内容进行HEX解码
int CGlobal::TransMsg(char *szDest, const char *szSrc, int nMsgFmt, int nSrcLen, int nMaxSize)
{
    if (NULL == szSrc || NULL == szDest || 0 == nSrcLen || 0 == nMaxSize)
    {
        return 0;
    }
    int nReadLen = MIN2(nSrcLen, nMaxSize);
    int nLen = 0;
    if (8 == nMsgFmt)
    {
        for (int i = 0, k = 0; i < nReadLen; i += 2)
        {
            szDest[k++] = HexchartoInt(szSrc[i]) * 16 + HexchartoInt(szSrc[i+1]);
        }
        nLen = nReadLen/2;
    }
	/*
    //如果是密文的base64需将base64解密
    else if (25 == nMsgFmt)
    {
        nLen = GetEncryptMsgFromBase64(szSrc, nSrcLen, szDest);
    }
	*/
    else
    {
        memcpy(szDest, szSrc, nReadLen);
        nLen = nReadLen;
    }
    return nLen;
}

int CGlobal::HexStrToMemBytes(unsigned char *szDest, const char *szSrc, int nSrcLen, int nDestSize)
{
    if (NULL == szSrc || NULL == szDest || 0 == nSrcLen || 0 == nDestSize)
    {
        return 0;
    }
    int nReadLen = MIN2(nSrcLen, nDestSize);
    for (int i = 0, k = 0; i < nReadLen; i += 2)
    {
        szDest[k++] = HexchartoInt(szSrc[i]) * 16 + HexchartoInt(szSrc[i+1]);
    }
    return nReadLen/2;
}

//替换函数将szSrc中的cSrc替换为cDest
void CGlobal::ReplaceChar(unsigned char *szSrc, char cSrc, char cDest, unsigned int nSrcLen)
{
    if (NULL != szSrc && nSrcLen > 0)
    {
        int nLen = nSrcLen;
        while (--nLen >= 0)
        {
            if (szSrc[nLen] == cSrc)
            {
                szSrc[nLen] = cDest;
            }
        }
    }
}

void CGlobal::ReplaceChar(unsigned char *szSrc, char cSrc, char* szDest, unsigned int nSrcLen)
{
	if (NULL != szSrc && nSrcLen > 0 && NULL != szDest)
	{
		int nLen = 0;
		int nTmp = strlen(szDest);
        while (nLen < nSrcLen)
        {
            if (szSrc[nLen] == cSrc)
            {
				//将nLen后面的字节后移strlen(szDest个长度)
				memcpy(szSrc+nLen+nTmp, szSrc+nLen+1, nSrcLen-nLen);
				memcpy(szSrc+nLen, szDest, nTmp);
				nLen += nTmp-1;
				nSrcLen += nTmp-1;
            }
			++nLen;
        }
	}
}

//获取带','字符串中第一节的内容
std::string CGlobal::GetSubStrFirst(const char *szStr)
{
    std::string str = "";
    if (szStr == NULL)
    {
        str = "";
    }
    else
    {
		const char szFlag[] = ",";
		const char *pStart = szStr;
		const char *pFind = strstr(szStr, szFlag);
		int nLen = 0;
		if (pFind)
		{
			nLen = pFind-pStart;
		}
		else
		{
			nLen = strlen(szStr);
		}
		if (nLen > 0)
		{
			str.assign(pStart, nLen);
		}
		else 
		{
			str = "";
		}
	}
    return str;
}

//取以逗个隔开的字符串，两个逗号之间的内容
std::string CGlobal::GetSubStr(const char *szStr, int nPos)
{
    std::string str("");
	const char szFlag[] = ",";
    if (szStr == NULL)
    {
        str = "";
    }
	//0位上的返回第一个逗号前的内容，若没有逗号，直接返回整个字符串
	else if (nPos == 0)
	{
		return GetSubStrFirst(szStr);
	}
    else
    {
        
        const char *pStart  = NULL;
        const char *pEnd    = NULL;
        int nCount          = 0;
        pStart              = strstr(szStr, szFlag);
        //循环查找","的存在
        while (pStart)
        {
            nCount++;
            pStart++;
            if (nCount == nPos)//找到第nPos个","
            {
                pEnd = strstr(pStart, szFlag);//看是否有第nPos+1个","存在
                if (pEnd == NULL)//如果nPos是最后一个","
                {
                    str = pStart;
                    break;
                }
                else
                {
                    //取第nPos个","和第nPos+1个","间的内容
                    char *p = new char[pEnd-pStart+1];
                    if (p)
                    {
                        try
                        {
                            memset(p, 0, pEnd-pStart+1);
                            memcpy(p, pStart, pEnd-pStart);
                            p[pEnd-pStart] = '\0';
                            str = p;
                        }
                        catch (...)
                        {
                        }
                    }
                    if (NULL != p)
                    {
                        delete []p;
                        p = NULL;
                    }
                    break;
                }
            }
            else
            {
                //如果当前字串没有nPos个","，返回空值
                pStart = strstr(pStart, szFlag);
                if (!pStart)
                {
                    str = "";
                    break;
                }
            }
        }
    }
    return str;
}

//ASCII转换为UNICODE
int CGlobal::AsciiToUS2(const char* Msg, unsigned char *Data, unsigned int *Len)
{
    wchar_t temp[8192+1] = {0};
    char szUMsg[8192+1] = {0};
    CAdapter::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,Msg, strlen(Msg), temp, sizeof(temp));
    int len = wcslen(temp);
    for (int i=0;i<len;++i)
    {
        int iFirst = 1;
        for(int j=0;j<2;++j)
        {
            if(1 == iFirst)
            {
                szUMsg[2*i] = (temp[i]>>8) & 0xff;
                iFirst = 0;
            }
            else
            {
                szUMsg[2*i+1] = temp[i]&0xff;
                iFirst = 1;
            }
        }
    }
    *Len = 2*len;
    memcpy(Data, szUMsg, *Len);
    return 0;
}

//UNICODE转换为ASCII
int CGlobal::US2ToAscii(const char *InMsg, int InLen, char *Data, int *Len)
{
    wchar_t uMsg[8192+1] = {0};
    char temp[8192+1] = {0};
    int uLen = InLen/2;
    for (int i=0; i<uLen; ++i)
    {
        uMsg[i] = InMsg[2*i] & 0xff;
        uMsg[i] = uMsg[i] << 8;
        uMsg[i] = uMsg[i] + (InMsg[2*i+1] & 0xff);
    }
    uLen = CAdapter::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, uMsg, uLen, temp, sizeof(temp), NULL, NULL);
    *Len = uLen;
    memcpy(Data, temp, uLen);
    return 0;
}

bool CGlobal::IsAllDigital(const char* str)
{
	bool bRet = false;

	while (NULL != str && *str != '\0')
	{
		bRet = true;
		if (0 == isdigit(*str++))
		{
			bRet = false;
			break;
		}
	}
	
	return bRet;
}

bool CGlobal::IsAllAlpNum(const char* str)
{	
	bool bRet = false;
	
	while (NULL != str && *str != '\0')
	{
		bRet = true;
		if (*str<0 || 0==isalnum(*str++))
		{
			bRet = false;
			break;
		}
	}
	
	return bRet;
}

//判断编码是否是可识别编码
bool CGlobal::IsUnKnownCode(int nMsgFmt) 
{
	return (0 != nMsgFmt && 8 != nMsgFmt && 15 != nMsgFmt);
}

//生成MT扩展参数,返回扩展参数及其长度
int  CGlobal::GenMtExParam(const char* szMsgSrc/*发送帐号*/, const char* szSvrType/*业务类型*/, const char* szVerifyCode/*验证码*/, char* szExParam/*最小需预留32字节*/)
{
	SYSTEMTIME_EX t;
	CAdapter::GetLocalTime(&t);
	char cMd5Src[128+1] = {0};
	int nTimeStamp  = t.m_iMonth*100000000;
	nTimeStamp += t.m_iDay*1000000;
	nTimeStamp += t.m_iHour*10000;
	nTimeStamp += t.m_iMinute*100;
	nTimeStamp += t.m_iSecond;
	int nSrcLen = sprintf(cMd5Src, 
		"%010d%s%s%s%010d", 
		nTimeStamp, 
		szMsgSrc,
		szVerifyCode,
		"*#06#~(</",  
		nTimeStamp);
	//timestamp
	unsigned int* pos = (unsigned int*)szExParam;
	*pos = htonl(nTimeStamp);
	//MD5
	GetMD5((unsigned char*)cMd5Src, nSrcLen, (unsigned char*)(szExParam+4));

	return 20;
}

//校验MT扩展参数,返回帐号接入类型
bool CGlobal::CheckMtExParam(const char* szExParam, const char* szMsgSrc, const char* szSvrType, int nVerifyCode)
{
	char cMd5Src[128+1] = {0};
	char szMd5Des[16+1] = {0};

	//校验扩展参数
	int nTimeStamp = (unsigned int)(szExParam[0]&0xff)*256*256*256 + (unsigned int)(szExParam[1]&0xff)*256*256
		+ (unsigned int)(szExParam[2]&0xff)*256 + (unsigned int)(szExParam[3]&0xff);

	int nSrcLen = sprintf(cMd5Src, 
		"%010d%s%d%s%010d", 
		nTimeStamp, 
		szMsgSrc,
		nVerifyCode,
		"*#06#~(</",  
		nTimeStamp);
	//MD5
	GetMD5((unsigned char*)cMd5Src, nSrcLen, (unsigned char*)szMd5Des);
	
	return !memcmp(szMd5Des, szExParam+4, 16);
}

/*
//ASCII转换为HEX编码
int CGlobal::CharToASCII(const char *szSrc, int nLen, char *szDest)
{
    int nDestLen = 0;
    if (NULL != szSrc && NULL != szDest)
    {
        int nRet = 0;
        int nPos = 0;
        for (int i = 0; i < nLen; ++i)
        {
            nRet = sprintf(szDest+nPos, "%02X-", szSrc[i]&0xff);
            nPos += nRet;
        }
        szDest[nPos-1] = '\0';
        nDestLen = nPos-1;
    }
    return nDestLen;
}
*/
/*
//ASCII转换为HEX编码
int CGlobal::CharToASCII(const char *szSrc, int nLen, char *szDest)
{
    int nDestLen = 0;
    if (NULL != szSrc && NULL != szDest)
    {
        int nRet = 0;
        int nPos = 0;
		int iLoop = nLen/20;
		int iLeave= nLen%20;
		for (int i = 0; i < iLoop; i+=20)
		{
			nRet = sprintf(szDest+nPos, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", 
				szSrc[i]&0xff, szSrc[i+1]&0xff, szSrc[i+2]&0xff, szSrc[i+3]&0xff, szSrc[i+4]&0xff, 
				szSrc[i+5]&0xff, szSrc[i+6]&0xff, szSrc[i+7]&0xff, szSrc[i+8]&0xff, szSrc[i+9]&0xff,
				szSrc[i+10]&0xff, szSrc[i+11]&0xff, szSrc[i+12]&0xff, szSrc[i+13]&0xff, szSrc[i+14]&0xff, 
				szSrc[i+15]&0xff, szSrc[i+16]&0xff, szSrc[i+17]&0xff, szSrc[i+18]&0xff, szSrc[i+19]&0xff);
			nPos += nRet;
		}
		int nCurPos = i;
		for (i = 0; i < iLeave; ++i,++nCurPos)
		{
			nRet = sprintf(szDest+nPos, "%02X", szSrc[nCurPos]&0xff);
			nPos += nRet;
		}
        szDest[nPos-1] = '\0';
        nDestLen = nPos-1;
    }
    return nDestLen;
}
*/

//ASCII转换为HEX编码
int CGlobal::CharToASCII(const char *szSrc, int nLen, char *szDest, int nMaxDestLen)
{
    int nDestLen = 0;
    if (NULL != szSrc && NULL != szDest)
    {
		for (int i = 0; (i < nLen && nDestLen < nMaxDestLen); ++i)
		{
			unsigned char c0 = ((unsigned char)szSrc[i]) >> 4;
			unsigned char c1 = ((unsigned char)szSrc[i]) & 0x0F;	
			if (c0 >= 0 && c0 <= 9)
			{
				c0 = c0 - 0 + '0';
			}
			else
			{
				c0 = c0 - 10 + 'A';
			}
			if (c1 >= 0 && c1 <= 9)
			{
				c1 = c1 - 0 + '0';
			}
			else
			{
				c1 = c1 - 10 + 'A';
			}
			*szDest++ = c0;
			*szDest++ = c1;
			//*szDest++ = ' ';
			nDestLen += 2;
		}
    }
    return nDestLen;
}

int CGlobal::CharToASCIIEx(const char *szSrc, int nLen, char *szDest)
{
    int nDestLen = 0;
    if (NULL != szSrc && NULL != szDest)
    {
        int nRet = 0;
        int nPos = 0;
        for (int i = 0; i < nLen; ++i)
        {
            nRet = sprintf(szDest+nPos, "%02x", szSrc[i]&0xff);
            nPos += nRet;
        }
        //szDest[nPos-1] = '\0';
        nDestLen = nPos;
    }
    return nDestLen;
}

int CGlobal::MemBytesToHexStr(const unsigned char *szSrc, int nLen, char *szDest)
{
    int nDestLen = 0;
    if (NULL != szSrc && NULL != szDest)
    {
        int nRet = 0;
        int nPos = 0;
        for (int i = 0; i < nLen; ++i)
        {
            nRet = sprintf(szDest+nPos, "%02X", szSrc[i]&0xff);
            nPos += nRet;
        }
        //szDest[nPos-1] = '\0';
        nDestLen = nPos;
    }
    return nDestLen;
}


INT64 CGlobal::TranMsgIdCharToI64(const unsigned char *szMsgId)
{
    if (NULL == szMsgId)
    {
        return 0;
    }

    INT64 j = 0;
    const unsigned char* p = szMsgId;
    j += (INT64)(*p)     << 56;
    j += (INT64)*(p + 1) << 48;
    j += (INT64)*(p + 2) << 40;
    j += (INT64)*(p + 3) << 32;
    j += (INT64)*(p + 4) << 24;
    j += (INT64)*(p + 5) << 16;
    j += (INT64)*(p + 6) << 8;
    j += (INT64)*(p + 7);
    return j;
}

void CGlobal::TranMsgIdI64ToChar(unsigned char* MsgId, INT64 IMsgId)
{
    INT64 j = IMsgId;
    MsgId[0] = (unsigned char)((j >> 56) & 0xff);
    MsgId[1] = (unsigned char)((j >> 48) & 0xff);
    MsgId[2] = (unsigned char)((j >> 40) & 0xff);
    MsgId[3] = (unsigned char)((j >> 32) & 0xff);
    MsgId[4] = (unsigned char)((j >> 24) & 0xff);
    MsgId[5] = (unsigned char)((j >> 16) & 0xff);
    MsgId[6] = (unsigned char)((j >>  8) & 0xff);
    MsgId[7] = (unsigned char)(j & 0xff);
    return;
}

INT64 CGlobal::GetI64MsgIdAt(INT64 SrcId, int PkNum)
{
    INT64 j = (INT64)PkNum;
    return ((j << 34) | SrcId);
    //return ( j * 17179869184 + SrcId);
}

int CGlobal::GeneranteSeqID()
{
    static long s_iSeqId = 1;
	
    return CAdapter::InterlockedIncrement(&s_iSeqId);
}

int CGlobal::GetMsgId(int Num, bool &bFlow)
{
    static int g_iMsgId = 0;
    int iFirstId = 0;

	{
		SafeLock safelock(&g_csMsgId);
		iFirstId = g_iMsgId + 1;
		g_iMsgId += Num;
		//32位int
		//unsigned int 4294967295
		//int   2147483647
		if (g_iMsgId > 1000000)
		{
			g_iMsgId = 0;
			iFirstId = 0;
			g_iMsgId += Num;
			bFlow = TRUE;
		}
	}
    return iFirstId;
}

int CGlobal::GetMsgId(int Num)
{
    static int g_iMsgId = 0;
    int iFirstId = 0;

	{
		SafeLock safelock(&g_csMsgId);
		iFirstId = g_iMsgId + 1;
		g_iMsgId += Num;
		//32位int
		//unsigned int 4294967295
		//int   2147483647
		if (g_iMsgId > 1000000)
		{
			g_iMsgId = 0;
			iFirstId = 0;
			g_iMsgId += Num;
		}
	}
    return iFirstId;
}

// 一个2进制表示的字符串转化为数
int CGlobal::BinStr2Num(char *szBuf)
{
    char *p = szBuf;
    int n = 0;
    while (*p == '0' || *p == '1')
    {
        n = n*2 + (*p -'0');
        ++p;
    }
    return n;
}

//生成流水号
void CGlobal::MakeMsgId(unsigned char *MsgId, char *pSrc)
{
    unsigned int nMonth = 0;
    unsigned int nDay   = 0;
    unsigned int nHour  = 0;
    unsigned int nMin   = 0;
    unsigned int nSec   = 0;
    unsigned int nGate  = 0;
    unsigned int nNo    = 0;
    char ch[16] = {0};
    char *p = pSrc;

    if (NULL == p || strlen(pSrc) < 5)
    {
        return;
    }

    memset(ch, 0, sizeof(ch));
    memcpy(ch, p, 2);
    nMonth = atoi(ch);
    memset(ch, 0, sizeof(ch));
    memcpy(ch, p + 2, 2);
    nDay = atoi(ch);
    memset(ch, 0, sizeof(ch));
    memcpy(ch, p + 4, 2);
    nHour = atoi(ch);
    memset(ch, 0, sizeof(ch));
    memcpy(ch, p + 6, 2);
    nMin = atoi(ch);
    memset(ch, 0, sizeof(ch));
    memcpy(ch, p + 8, 2);
    nSec = atoi(ch);
    memset(ch, 0, sizeof(ch));
    memcpy(ch, p + 10, 6);
    nGate = atoi(ch);
    memset(ch, 0, sizeof(ch));
    memcpy(ch, p +16, 6);
    nNo = atoi(ch);

    INT64 j = 0;
    j |= (__int64)(nMonth & 0x0f)     << 60;
    j |= (__int64)(nDay   & 0x1f)     << 55;
    j |= (__int64)(nHour  & 0x1f)     << 50;
    j |= (__int64)(nMin   & 0x3f)     << 44;
    j |= (__int64)(nSec   & 0x3f)     << 38;
    j |= (__int64)(nGate  & 0x03ffff) << 20;   //18 bit
    j |= (__int64)(nNo    & 0x0fffff);           //20 bit
    //j |= (__int64)(nGate & 0x3fffff) << 16;
    //j |= (__int64)(nNo & 0x0ffff);

    MsgId[0] = (unsigned char)((j >> 56) & 0xff);
    MsgId[1] = (unsigned char)((j >> 48) & 0xff);
    MsgId[2] = (unsigned char)((j >> 40) & 0xff);
    MsgId[3] = (unsigned char)((j >> 32) & 0xff);
    MsgId[4] = (unsigned char)((j >> 24) & 0xff);
    MsgId[5] = (unsigned char)((j >> 16) & 0xff);
    MsgId[6] = (unsigned char)((j >>  8) & 0xff);
    MsgId[7] = (unsigned char)((j) & 0xff);
    return;
}

void CGlobal::MakeSgipMsgId(/*out*/unsigned char *MsgId, /*in*/unsigned int SrcNode, /*in*/unsigned int TimeStamp, /*in*/unsigned int SeqId)
{
    if (NULL == MsgId)
    {
        return;
    }
	//取时间
    unsigned int nMonth = 0;
    unsigned int nDay   = 0;
    unsigned int nHour  = 0;
    unsigned int nMin   = 0;
    unsigned int nSec   = 0;
	char szTime[21+1] = {0};
	sprintf(szTime, "%010d", TimeStamp);
	sscanf(szTime, "%02d%02d%02d%02d%02d", &nMonth, &nDay, &nHour, &nMin, &nSec);

    INT64 j = 0;
    j |= (INT64)(nMonth & 0x0f)       << 60;
    j |= (INT64)(nDay   & 0x1f)       << 55;
    j |= (INT64)(nHour  & 0x1f)       << 50;
    j |= (INT64)(nMin   & 0x3f)       << 44;
    j |= (INT64)(nSec   & 0x3f)       << 38;
    j |= (INT64)(SrcNode  & 0x3f)	  << 32;
    j |= (INT64)(SeqId);
	//SrcNode最大值只能取64
		
    MsgId[0] = (unsigned char)((j >> 56) & 0xff);
    MsgId[1] = (unsigned char)((j >> 48) & 0xff);
    MsgId[2] = (unsigned char)((j >> 40) & 0xff);
    MsgId[3] = (unsigned char)((j >> 32) & 0xff);
    MsgId[4] = (unsigned char)((j >> 24) & 0xff);
    MsgId[5] = (unsigned char)((j >> 16) & 0xff);
    MsgId[6] = (unsigned char)((j >>  8) & 0xff);
    MsgId[7] = (unsigned char)((j)       & 0xff);
    return;
}

void CGlobal::UnMakeSgipMsgId(/*in*/const unsigned char *MsgId, /*out/in*/unsigned int *pSrcNode, /*out/in*/unsigned int *pTimeStamp, /*out/in*/unsigned int *pSeqId)
{
    if (NULL == MsgId)
    {
		*pSeqId = 0;
		*pTimeStamp = 0;
		*pSrcNode = 0;
        return;
    }
	if (NULL == pSrcNode || NULL == pTimeStamp || NULL == pSeqId)
	{
		return;
	}

	unsigned char ch;
    unsigned int nMon = 0;
    unsigned int nDay = 0;
    unsigned int nHour = 0;
    unsigned int nMin = 0;
    unsigned int nSec = 0;
	
    ch = *(MsgId++);
    nMon = ch >> 4;
    nDay = (unsigned char)(ch << 4) >> 3;
	
    ch = *(MsgId++);
    nDay += (ch >> 7);
	
    nHour = (unsigned char)(ch << 1) >> 3;
	
    nMin = (unsigned char)(ch << 6) >> 2;
    ch = *(MsgId++);
    nMin += (ch >> 4);
	
    nSec = (unsigned char)(ch << 4) >> 2;
	
    ch = *(MsgId++);
    nSec += (ch >> 6);
	
	*pTimeStamp = nMon*100000000+nDay*1000000+nHour*10000+nMin*100+nSec;
    *pSrcNode = (unsigned char)(ch << 2) >> 2;

	//*pSeqId = *((unsigned int*)(MsgId)); //网络字节
	unsigned int &nSeqId = *pSeqId;
	nSeqId = (nSeqId<<8) | *MsgId++;
	nSeqId = (nSeqId<<8) | *MsgId++;
	nSeqId = (nSeqId<<8) | *MsgId++;
	nSeqId = (nSeqId<<8) | *MsgId++;			
	
    return;
}

void CGlobal::MakeMsgId(unsigned char *MsgId, unsigned int Num, unsigned int Gate)
{
    static time_t tLastMake;
    unsigned int nMonth = 0;
    unsigned int nDay   = 0;
    unsigned int nHour  = 0;
    unsigned int nMin   = 0;
    unsigned int nSec   = 0;
    unsigned int nGate  = Gate;
    bool        bFlow   = FALSE;
    unsigned int nNo    = GetMsgId(Num, bFlow);
    if (NULL == MsgId)
    {
        return;
    }

    if (time(NULL) == tLastMake && bFlow)
    {
        CAdapter::Sleep(1000);
    }

    time(&tLastMake);
    SYSTEMTIME_EX t;
    CAdapter::GetLocalTime(&t);
    nMonth  = t.m_iMonth;
    nDay    = t.m_iDay;
    nHour   = t.m_iHour;
    nMin    = t.m_iMinute;
    nSec    = t.m_iSecond;

    INT64 j = 0;
    j |= (INT64)(nMonth & 0x0f)       << 60;
    j |= (INT64)(nDay   & 0x1f)       << 55;
    j |= (INT64)(nHour  & 0x1f)       << 50;
    j |= (INT64)(nMin   & 0x3f)       << 44;
    j |= (INT64)(nSec   & 0x3f)       << 38;
    j |= (INT64)(nGate  & 0x03ffff)   << 20;
    j |= (INT64)(nNo    & 0x0fffff);
	//nGate最大值只能取262144
	//nNo最大值只能取1048575

    //j |= (__int64)(nGate & 0x3fffff)<< 16;
    //j |= (__int64)(nNo & 0x0ffff);

    MsgId[0] = (unsigned char)((j >> 56) & 0xff);
    MsgId[1] = (unsigned char)((j >> 48) & 0xff);
    MsgId[2] = (unsigned char)((j >> 40) & 0xff);
    MsgId[3] = (unsigned char)((j >> 32) & 0xff);
    MsgId[4] = (unsigned char)((j >> 24) & 0xff);
    MsgId[5] = (unsigned char)((j >> 16) & 0xff);
    MsgId[6] = (unsigned char)((j >>  8) & 0xff);
    MsgId[7] = (unsigned char)((j)       & 0xff);
    return;
}

int CGlobal::TranMsgId(unsigned char *pSrc, char *MsgId)
{
    unsigned int nMon   = 0;
    unsigned int nDay   = 0;
    unsigned int nHour  = 0;
    unsigned int nMin   = 0;
    unsigned int nSec   = 0;
    unsigned int nGate  = 0;
    unsigned int nNo    = 0;
    unsigned char *p    = NULL;

    //先转为64bit整形再移位
    INT64 j = 0;
    p = pSrc;
    j |= (INT64)(*(p))    << 56;
    j |= (INT64)(*(++p))  << 48;
    j |= (INT64)(*(++p))  << 40;
    j |= (INT64)(*(++p))  << 32;
    j |= (INT64)(*(++p))  << 24;
    j |= (INT64)(*(++p))  << 16;
    j |= (INT64)(*(++p))  << 8;
    j |= (INT64)(*(++p));

    nMon  = (unsigned int)(j >> 60) & 0x0f;            //4  bit
    nDay  = (unsigned int)(j >> 55) & 0x1f;           //5  bit
    nHour = (unsigned int)(j >> 50) & 0x1f;           //5  bit
    nMin  = (unsigned int)(j >> 44) & 0x3f;           //6  bit
    nSec  = (unsigned int)(j >> 38) & 0x3f;           //6  bit
    nGate = (unsigned int)(j >> 20) & 0x03ffff;       //18 bit,最大值9999
    nNo   = (unsigned int)(j)       & 0x0fffff;       //20 bit,最大值999999
    sprintf(MsgId, "%02d%02d%02d%02d%02d%06d%06d", nMon, nDay, nHour, nMin, nSec, nGate, nNo);
    return 0;
}

//判断是否是过期的RPT
bool CGlobal::IsTimeOutRpt(const unsigned char* pMsgId, TINYINT nTimeOut)
{
	time_t tMsgId = GetTimeInMsgId(pMsgId);
	time_t tDiff = time(NULL)-tMsgId;
	return (nTimeOut > 0 && tDiff > 3600*nTimeOut);
}

bool CGlobal::IsSuccessRpt(const char* pErCode)
{
	return (0 == strnicmp("DELIVRD", pErCode, 7) || pErCode[0] == '0');
}

bool CGlobal::IsInnerErrRpt(const char* pErCode)
{
	return (0 == strnicmp("M1:", pErCode, 3) 
		|| 0 == strnicmp("M2:", pErCode, 3) 
		|| 0 == strnicmp("R1:", pErCode, 3)
		|| 0 == strnicmp("R2:", pErCode, 3));
}

bool CGlobal::IsTodayMtRpt(const unsigned char *pSrc, INT64 nSdTm)
{
	if (nSdTm <= 0)
	{
		return IsMsgIdInToday(pSrc);
	}
	else
	{
		SYSTEMTIME_EX tSdTm;
		Systime_Bigint sendtime(nSdTm);
		sendtime.FormatBigintToSystime(&tSdTm);

		SYSTEMTIME_EX tNow;
		CAdapter::GetLocalTime(&tNow);

		return (tSdTm.m_iYear == tNow.m_iYear && tSdTm.m_iMonth == tNow.m_iMonth && tSdTm.m_iDay == tNow.m_iDay);
	}
}

bool CGlobal::IsMsgIdInToday(const unsigned char *pSrc)
{
	unsigned char ch;
    unsigned int nMon = 0;
    unsigned int nDay = 0;
	
    ch = *(pSrc++);
    nMon = ch >> 4;
    nDay = (unsigned char)(ch << 4) >> 3;
	
    ch = *(pSrc++);
    nDay += (ch >> 7);
	
    SYSTEMTIME_EX t;
    CAdapter::GetLocalTime(&t);
	return (nMon == t.m_iMonth && nDay == t.m_iDay);
}

int CGlobal::GetRptDayFromMsgId(const unsigned char *pSrc)
{
    unsigned char ch;
    unsigned int nMon = 0;
    unsigned int nDay = 0;
    unsigned int nHour = 0;
    unsigned int nMin = 0;
    unsigned int nSec = 0;
    unsigned int nGate = 0;
    unsigned int nNo = 0;
	
    ch = *(pSrc++);
    nMon = ch >> 4;
    nDay = (unsigned char)(ch << 4) >> 3;
	
    ch = *(pSrc++);
    nDay += (ch >> 7);
	
    nHour = (unsigned char)(ch << 1) >> 3;
	
    nMin = (unsigned char)(ch << 6) >> 2;
    ch = *(pSrc++);
    nMin += (ch >> 4);
	
    nSec = (unsigned char)(ch << 4) >> 2;
	
    ch = *(pSrc++);
    nSec += (ch >> 6);
	
    SYSTEMTIME_EX t;
    CAdapter::GetLocalTime(&t);
	int nYear = t.m_iYear;
	//处理跨年问题,如果流水号中的月是12月,而当前月是1月,则年减去1
	if (nMon == 12 && t.m_iMonth == 1) nYear -= 1;
    struct tm gm1 = {0, 0, 0, nDay, nMon-1, nYear-1900, 0, 0, 0};
	struct tm gm2 = {0, 0, 0, t.m_iDay, t.m_iMonth-1, t.m_iYear-1900, 0, 0, 0};
	time_t tMsgId = mktime(&gm1);
	time_t tNow   = mktime(&gm2);
	int nRptDay = (tNow-tMsgId)/(24*3600);
    return (nRptDay>=0?nRptDay:0);
}

time_t CGlobal::GetTimeInMsgId(const unsigned char *pSrc, INT64 nSdTm)
{
	if (nSdTm <= 0)
	{
		return GetTimeInMsgId(pSrc);
	}
	else
	{
		SYSTEMTIME_EX t;
		Systime_Bigint sendtime(nSdTm);
		sendtime.FormatBigintToSystime(&t);
		struct tm gm = {t.m_iSecond, t.m_iMinute, t.m_iHour, t.m_iDay, t.m_iMonth-1, t.m_iYear-1900, 0, 0, 0};
		return mktime(&gm);
	}
}

time_t CGlobal::GetTimeInMsgId(INT64 nMsgId)
{
	unsigned char szMsgId[16+1] = {0};
	TranMsgIdI64ToChar(szMsgId, nMsgId);
	return GetTimeInMsgId(szMsgId);
}

time_t CGlobal::GetTimeInMsgId(const unsigned char *pSrc)
{
    unsigned char ch;
    unsigned int nMon = 0;
    unsigned int nDay = 0;
    unsigned int nHour = 0;
    unsigned int nMin = 0;
    unsigned int nSec = 0;
    unsigned int nGate = 0;
    unsigned int nNo = 0;

    ch = *(pSrc++);
    nMon = ch >> 4;
    nDay = (unsigned char)(ch << 4) >> 3;

    ch = *(pSrc++);
    nDay += (ch >> 7);

    nHour = (unsigned char)(ch << 1) >> 3;

    nMin = (unsigned char)(ch << 6) >> 2;
    ch = *(pSrc++);
    nMin += (ch >> 4);

    nSec = (unsigned char)(ch << 4) >> 2;

    ch = *(pSrc++);
    nSec += (ch >> 6);

    SYSTEMTIME_EX t;
    CAdapter::GetLocalTime(&t);
	//处理跨年问题,如果流水号中的月是12月,而当前月是1月,则年减去1
	if (nMon == 12 && t.m_iMonth == 1) t.m_iYear -= 1;
    struct tm gm = {nSec, nMin, nHour, nDay, nMon-1, t.m_iYear-1900, 0, 0, 0};
    return mktime(&gm);
}

void CGlobal::GetTimeInMsgId(char *szTime, const unsigned char *pSrc)
{
    unsigned char ch;
    unsigned int nMon = 0;
    unsigned int nDay = 0;
    unsigned int nHour = 0;
    unsigned int nMin = 0;
    unsigned int nSec = 0;
    unsigned int nGate = 0;
    unsigned int nNo = 0;

    ch = *(pSrc++);
    nMon = ch >> 4;
    nDay = (unsigned char)(ch << 4) >> 3;

    ch = *(pSrc++);
    nDay += (ch >> 7);

    nHour = (unsigned char)(ch << 1) >> 3;

    nMin = (unsigned char)(ch << 6) >> 2;
    ch = *(pSrc++);
    nMin += (ch >> 4);

    nSec = (unsigned char)(ch << 4) >> 2;

    ch = *(pSrc++);
    nSec += (ch >> 6);

    sprintf(szTime, "%02d%02d%02d%02d%02d", nMon, nDay, nHour, nMin, nSec);
}

//将一定的毫秒分成几段来延时
void CGlobal::Delay(int nDelayTm/*毫秒*/)
{
	int nLoop = nDelayTm/100;
	for (int i = 0; i < nLoop; ++i)
	{
		CAdapter::Sleep(100);
	}
}

//判断是否在一个时间段内
bool CGlobal::IsInSpan(DATE dt1, DATE dt2, DATE dt)
{
	//类如：08:00:00-20:00:00
	if (dt1 < dt2)
	{
		return (dt >= dt1 && dt <= dt2);
	}
	else if (dt1 == dt2)
	{
		return false;
	}
	else
	{
		//类如：21:00:00-09:00:00 情况1：当前时间在21:00:00-23:59:59之间 情况2：当前时间在00:00:00-09:00:00之间
		if ((dt1 < dt && dt2 < dt) || (dt1 > dt && dt2 > dt))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

//由流水号和外部时间来获取一个合理的时间
int  CGlobal::GetValidTime(const char* szMsgId, const char* szDoneTime, char* szTime/*in/out "YYYY-MM-DD HH-MM-SS"*/)
{
	struct tm *tNow;
	time_t t = time(NULL);
	tNow = localtime(&t);
	int nSec = tNow->tm_sec;
	if (szMsgId != NULL && szDoneTime != NULL && szTime != NULL)
	{
		//由流水号取时间(短信接收时间)
		int nMon_MsgId=0,nDay_MsgId=0,nHour_MsgId=0,nMin_MsgId=0,nSec_MsgId=0;
		char szMsgIdTm[19+1] = {0};
		GetTimeInMsgId(szMsgIdTm, (unsigned char*)szMsgId);
		sscanf(szMsgIdTm, "%02d%02d%02d%02d%02d", &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
		
		//取外部donetime(网关收到rpt的时间)
		int nYear_DoneTm=0,nMon_DoneTm=0,nDay_DoneTm=0,nHour_DoneTm=0,nMin_DoneTm=0,nSec_DoneTm=0;
		sscanf(szDoneTime, "%02d%02d%02d%02d%02d", &nYear_DoneTm, &nMon_DoneTm, &nDay_DoneTm, &nHour_DoneTm, &nMin_DoneTm);
		
		//取外部sztime(本地接收到Rpt时间)
		int nYearRecvTm = 0;
		int nYear_RecvTm=0,nMon_RecvTm=0,nDay_RecvTm=0,nHour_RecvTm=0,nMin_RecvTm=0,nSec_RecvTm=0;
		sscanf(szTime, "%04d-%02d-%02d %02d:%02d:%02d", &nYear_RecvTm, &nMon_RecvTm, &nDay_RecvTm, &nHour_RecvTm, &nMin_RecvTm, &nSec_RecvTm);
		nYearRecvTm = nYear_RecvTm;
		nYear_RecvTm = nYear_RecvTm%100;
		
		//如果sztime比donetime的时间晚，那么取donetime的时间。然后再与流水号中的时间做有效性检查(rpt返回时间不能小于流水号时间)
		int nYear=nYear_RecvTm,nMonth=nMon_RecvTm,nDay=nDay_RecvTm,nHour=nHour_RecvTm,nMin=nMin_RecvTm,nSec=nSec_RecvTm;
		INT64 nTm_Recv = nYear_RecvTm*100000000+nMon_RecvTm*1000000+nDay_RecvTm*10000+nHour_RecvTm*100+nMin_RecvTm;
		INT64 nTm_DoneTm = nYear_DoneTm*100000000+nMon_DoneTm*1000000+nDay_DoneTm*10000+nHour_DoneTm*100+nMin_DoneTm;
		INT64 nTm_MsgId = nYear_DoneTm*100000000+nMon_MsgId*1000000+nDay_MsgId*10000+nHour_MsgId*100+nMin_MsgId;
		//同一秒之内时取接收时间
		//超出一秒时取donetime
		//donetime再与msgid时间做比较
		//donetime若小于等于msgid时间,则取msgid时间+3秒
		//donetime若大于msgid时间且刚好分相差一分，则当msgid时间的秒大于30时取59-xxx，当msgid时间的秒小于30时就取msgid时间的秒
		if (nTm_Recv != nTm_DoneTm)
		{
			nYear = nYear_DoneTm;
			nMonth = nMon_DoneTm;
			nDay = nDay_DoneTm;
			nHour = nHour_DoneTm;
			nMin = nMin_DoneTm;
			if (nTm_DoneTm <= nTm_MsgId)
			{
				nYear = nYear_DoneTm;
				nMonth = nMon_MsgId;
				nDay = nDay_MsgId;
				nHour = nHour_MsgId;
				nMin = nMin_MsgId;
				nSec = nSec_MsgId+3;
				nSec>59?nSec=nSec_MsgId:1;
			}
			else
			{
				nSec_MsgId>30?nSec=59-nSec_MsgId:nSec=nSec_MsgId;
			}
			memset(szTime, 0, strlen(szTime));
			sprintf(szTime, "%04d-%02d-%02d %02d:%02d:%02d", nYearRecvTm, nMonth, nDay, nHour, nMin, nSec);
		}
	}
	return nSec;
}

void CGlobal::UnMakeMsgId(char *MsgId, const unsigned char *pSrc)
{
    unsigned char ch;
    unsigned int nMon = 0;
    unsigned int nDay = 0;
    unsigned int nHour = 0;
    unsigned int nMin = 0;
    unsigned int nSec = 0;
    unsigned int nGate = 0;
    unsigned int nNo = 0;

    ch = *(pSrc++);
    nMon = ch >> 4;
    nDay = (unsigned char)(ch << 4) >> 3;

    ch = *(pSrc++);
    nDay += (ch >> 7);

    nHour = (unsigned char)(ch << 1) >> 3;

    nMin = (unsigned char)(ch << 6) >> 2;
    ch = *(pSrc++);
    nMin += (ch >> 4);

    nSec = (unsigned char)(ch << 4) >> 2;

    ch = *(pSrc++);
    nSec += (ch >> 6);

    nGate = (unsigned char)(ch << 2) >> 2;

    ch = *(pSrc++);
    nGate = (unsigned int)(nGate << 8) + ch;
    nGate = (unsigned int)(nGate << 8) + ch;
    ch = *(pSrc++);
    nGate = (unsigned int)(nGate << 8) + ch;

    ch = *(pSrc++);
    nNo = ch;
    ch = *(pSrc++);
    nNo = (unsigned int)(nNo << 8) + ch;

    sprintf(MsgId, "%02d%02d%02d%02d%02d%07d%05d", nMon, nDay, nHour, nMin, nSec, nGate, nNo);
}

void CGlobal::GenernateMsgIDEx(unsigned char *szMsgID, int nUId)
{
    try
    {
        int nNum            = 0;
        int i               = 0;
        char szBuf[64+1]    = {0};

        for (i = 0; i < 64; ++i)
        {
            szBuf[i] = '0';
        }

        unsigned char szData[8+1] = {0};
        char szTmp[8+1] = {0};
        int nTmp = 0;

        unsigned int nMonth = 0;
        unsigned int nDay   = 0;
        unsigned int nHour  = 0;
        unsigned int nMin   = 0;
        unsigned int nSec   = 0;
        unsigned int nGate  = nUId;

        SYSTEMTIME_EX t;
        CAdapter::GetLocalTime(&t);
        nMonth  = t.m_iMonth;
        nDay    = t.m_iDay;
        nHour   = t.m_iHour;
        nMin    = t.m_iMinute;
        nSec    = t.m_iSecond;

        //要根据短信条数设置步长
        //nNum = GetMsgId(1);


        //将所有数据转化成二进制，并生成二制字符串
        std::string str;
        int nPos = 0;
        int nLen = 0;
        //CAdapter::itoa(nMonth, szBuf+nPos, 2);
        sprintf(szBuf+nPos, "%d", nMonth);
        str.assign(szBuf+nPos);
        nLen = str.size();
        if (nLen < 4)
        {
            for (i = 0; i < 4-nLen; ++i)
            {
                str.insert(str.begin(), '0');
            }
        }
        strncpy(szBuf+nPos, str.c_str(), 4);
        nPos += 4;//4

        //CAdapter::itoa(nDay, szBuf+nPos, 2);
        sprintf(szBuf+nPos, "%d", nDay);
        str.assign(szBuf+nPos);
        nLen = str.size();
        if (nLen < 5)
        {
            for (i = 0; i < 5-nLen; ++i)
            {
                str.insert(str.begin(), '0');
            }
        }
        strncpy(szBuf+nPos, str.c_str(), 5);
        nPos += 5;//9

        //CAdapter::itoa(nHour, szBuf+nPos, 2);
        sprintf(szBuf+nPos, "%d", nHour);
        str.assign(szBuf+nPos);
        nLen = str.size();
        if (nLen < 5)
        {
            for (i = 0; i < 5-nLen; ++i)
            {
                str.insert(str.begin(), '0');
            }
        }
        strncpy(szBuf+nPos, str.c_str(), 5);
        nPos += 5;//14

        //CAdapter::itoa(nMin, szBuf+nPos, 2);
        sprintf(szBuf+nPos, "%d", nMin);
        str.assign(szBuf+nPos);
        nLen = str.size();
        if (nLen < 6)
        {
            for (i = 0; i < 6-nLen; ++i)
            {
                str.insert(str.begin(), '0');
            }
        }
        strncpy(szBuf+nPos, str.c_str(), 6);
        nPos += 6;//20

        //CAdapter::itoa(nSec, szBuf+nPos, 2);
        sprintf(szBuf+nPos, "%d", nSec);
        str.assign(szBuf+nPos);
        nLen = str.size();
        if (nLen < 6)
        {
            for (i = 0; i < 6-nLen; ++i)
            {
                str.insert(str.begin(), '0');
            }
        }
        strncpy(szBuf+nPos, str.c_str(), 6);
        nPos += 6;//26


        //CAdapter::itoa(nGate, szBuf+nPos, 2);
        sprintf(szBuf+nPos, "%d", nGate);
        str.assign(szBuf+nPos);
        nLen = str.size();
        if (nLen < 22)
        {
            for (i = 0; i < 22-nLen; ++i)
            {
                str.insert(str.begin(), '0');
            }
        }
        strncpy(szBuf+nPos, str.c_str(), 22);
        nPos += 22;//48

        //CAdapter::itoa(nNum, szBuf + nPos, 2);
        sprintf(szBuf+nPos, "%d", nNum);
        str.assign(szBuf + nPos);
        nLen = str.size();
        if (nLen < 16)
        {
            for (i = 0; i < 16-nLen; ++i)
            {
                str.insert(str.begin(), '0');
            }
        }
        strncpy(szBuf+nPos, str.c_str(), 16);
        nPos += 16;//64
        szBuf[64] = '\0';

        //以8为单位（1字节）将0/1的字符串转换为10进制再转为16进制存入szData
        int j = 0;
        for (i = 0; i < 64; i += 8)
        {
            memcpy(szTmp, szBuf+i, 8);
            nTmp = BinStr2Num(szTmp);
            szData[j++] = (unsigned char)nTmp;
        }
        memcpy(szMsgID, szData, 8);
    }
    catch (...)
    {

    }
}

//计算短信条数
int CGlobal::CalcMsgNum(int nMsgLen, int nSplitLen, int nMultilen1, int nMultilen2, const char *szMsg, bool bAsciiToUS2)
{
    int nTotalSize = nMsgLen;
    int nTotalNum = 1;
    unsigned char szBuf[8192+1] = {0};
    if (bAsciiToUS2)
    {
        AsciiToUS2(szMsg, szBuf, (unsigned int*)&nTotalSize);
    }

    if (nTotalSize <= nSplitLen * 2)
    {
        nTotalNum = 1;
    }
    else
    {
        nTotalNum = 1 + (nTotalSize - nMultilen2 * 2 + nMultilen1 * 2 - 1) / (nMultilen1 * 2);
    }

    return nTotalNum;
}

/*
szInMsg     本次要拆的短信内容
szOutMsg    输出拆分后该条的内容(已解码)
nMultilen1
nMultilen2
nPkNum      要第几条
nPkTotal    总共几条
nPreMsgLen  上次返回的长度
nMsgLen     本次输入内容的长度(若内容中带有签名，需减去签名的长度=nMsgLen-nSignLen)
bUS2ToAscii 是否要解码(仅对编码方式是8的),无需编码时,输入字符需为宽字符,长度需宽字符串长度
*/
int CGlobal::SplitMsgEx(const unsigned char *szInMsg, char *szOutMsg, int nMultilen1, int nMultilen2,
            int nPkNum, int nPkTotal, int nPreMsgLen, int nMsgLen, bool bUS2ToAscii, int nSignLen)
{
    int nSplitLen   = nMsgLen;
    int nLen        = nMsgLen;
    int nTmp        = nPreMsgLen;
    char szBuf[8192+1] = {0};
    if (nPkTotal > 1)
    {
        if (nPkNum == (nPkTotal - 1))
        {
            if ((nMsgLen - nTmp) <= nMultilen1 * 2 /*&& (nMsgLen - nTmp) > nMultilen2 * 2*/)
            {
                nSplitLen = nMsgLen - nTmp - 2;
            }
            else
            {
                nSplitLen = nMultilen1 * 2;
            }
        }
        else if (nPkNum == nPkTotal)
        {
            nSplitLen = (nMsgLen - nTmp) + nSignLen/*% (nMultilen2 * 2 + 1) / 2 * 2*/;
        }
        else
        {
            nSplitLen = nMultilen1 * 2;
        }
        memcpy(szBuf, szInMsg+nTmp, nSplitLen);
        if (bUS2ToAscii)
        {
            US2ToAscii(szBuf, nSplitLen, szOutMsg, &nLen);
        }
        else
        {
            memcpy(szOutMsg, szBuf, nSplitLen);
        }
    }
    else
    {
        nSplitLen = nMsgLen;
        memcpy(szBuf, szInMsg, nSplitLen);
        if (bUS2ToAscii)
        {
            US2ToAscii(szBuf, nSplitLen, szOutMsg, &nLen);
        }
        else
        {
            memcpy(szOutMsg, szBuf, nSplitLen);
        }
    }
    return nSplitLen;
}

//拆分短信
int CGlobal::SplitMsg(const char* Msg, int nSplitLen, char *Msg1, char *Msg2)
{
    int nInMsgLen = strlen(Msg);
    if (nInMsgLen <= 0)
    {
        return -1;
    }
    else if (nInMsgLen > 2048)
    {
        return -2;
    }

    wchar_t tempsrc[2048+1]   = {0}; 
    wchar_t tempdest[2048+1]  = {0};
    memset(Msg1, '\0', sizeof(Msg1));
    memset(Msg2, '\0', sizeof(Msg2));
    CAdapter::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Msg, strlen(Msg), tempsrc, sizeof(tempsrc));
    int len = wcslen(tempsrc);
    if (len > 0 && len <= nSplitLen)
    {
		//只有一条信息
        wcsncpy(tempdest, tempsrc, nSplitLen);
		CAdapter::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, tempdest, -1, Msg1, nSplitLen*2, NULL, NULL);
        return 0;
    }
    else if (len > nSplitLen)
    {
		//超过一条信息
        wcsncpy(tempdest, tempsrc, nSplitLen);
		CAdapter::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, tempdest, -1, Msg1, nSplitLen*2, NULL, NULL);
		CAdapter::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, tempsrc+nSplitLen,-1, Msg2, wcslen(tempsrc+nSplitLen)*2, NULL, NULL);
        return 1;
    }
	else
	{
		return -3;
	}
}

UINT CGlobal::TrimRight(char *szSrc, UINT nLen, char ch)
{
    if (NULL != szSrc && nLen > 0)
    {
        while (nLen > 0)
        {
            if (szSrc[nLen-1] != ch)
            {
                break;
            }
            else
            {
                szSrc[nLen-1] = '\0';
                --nLen;
            }
        }
    }
    return nLen;
}

UINT CGlobal::TrimRight(char *szSrc, UINT nLen)
{
    if (NULL != szSrc && nLen > 0)
    {
        while (nLen > 0)
        {
            //去空格、回车、换行、制表符 
			//2011-12-06 要求不去除回车和回车换行
            if (szSrc[nLen-1] != 0x20 /*&& szSrc[nLen-1] != 0x0d && szSrc[nLen-1] != 0x0a*/ && szSrc[nLen-1] != 0x09)
            {
                break;
            }
            else
            {
                szSrc[nLen-1] = '\0';
                --nLen;
            }
        }
    }
    return nLen;
}

int CGlobal::TrimGarbledCharacter(char *szSrc, int nLen, int nMaxLen)
{
	int nRet = 0;
	if (nLen > 0)
	{
		int nWcMaxBuf = nLen*2+1;
		wchar_t pWcMsg[1024+1] = {0};
		int nWcLen = CAdapter::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szSrc, nLen, pWcMsg, nWcMaxBuf);	
		nRet = CAdapter::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pWcMsg, nWcLen, szSrc, nMaxLen, NULL, NULL);
	}
	return nRet;
}
/*
int CGlobal::TrimGarbledCharacter(char *szSrc, int nLen, int nMaxLen)
{
	int nRet = 0;
	if (nLen > 0)
	{
		int nWcMaxBuf = nLen*2+1;
		wchar_t* pWcMsg = new wchar_t[nWcMaxBuf];
		if (pWcMsg)
		{
			memset(pWcMsg, 0, nWcMaxBuf);
			int nWcLen = CAdapter::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szSrc, nLen, pWcMsg, nWcMaxBuf);	
			nRet = CAdapter::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pWcMsg, nWcLen, szSrc, nMaxLen, NULL, NULL);
			delete pWcMsg;
		}
	}
	return nRet;
}
*/
//验证短信内容的合法性
int CGlobal::CheckMsgValidity(int nMsgFmt, int nMaxLen, USHORT& nMsgLen, /*const */char *szMsg, bool bTransHex, bool bMo)
{
    int nRet = 0;
	int nSrcMsgLen = nMsgLen;
    wchar_t wcMsg[4096] = {0};
    if (8 == nMsgFmt)
    {
        if (nMsgLen > nMaxLen*2)
        {
            nRet = 6;
        }
        else
        {
            char szBuf[2048+1] = {0};
            int  nLen   = nMsgLen;
            int  OutLen = 0;
            US2ToAscii(szMsg, nLen, szBuf, &OutLen);
			OutLen = strlen(szBuf);
            OutLen = CAdapter::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szBuf, OutLen, wcMsg, sizeof(wcMsg));
            memset(szBuf, 0, sizeof(szBuf));
            OutLen = CAdapter::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wcMsg, OutLen, szBuf, MAX_MSG_LEN*2, NULL, NULL);
            //上行不trim
			if(!bMo) OutLen = TrimRight(szBuf, OutLen);
            AsciiToUS2(szBuf, (UCHAR*)szMsg, (UINT*)&nMsgLen);
        }
    }
    else if (15 == nMsgFmt)
    {
        nMsgLen = CAdapter::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szMsg, nMsgLen, wcMsg, sizeof(wcMsg));

        if (nMsgLen > nMaxLen)
        {
            nRet = 6;
        }
        else
        {
            nMsgLen = CAdapter::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wcMsg, nMsgLen, szMsg, MAX_MSG_LEN*2, NULL, NULL);
            //上行不trim
			if (!bMo) nMsgLen = TrimRight(szMsg, nMsgLen);
			nMsgLen = strlen(szMsg);
        }
    }
    else if (0 == nMsgFmt)
    {
        nMsgLen = CAdapter::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szMsg, nMsgLen, wcMsg, sizeof(wcMsg));

        if (nMsgLen > nMaxLen)
        {
            nRet = 6;
        }
        else
        {
            nMsgLen = CAdapter::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wcMsg, nMsgLen, szMsg, MAX_MSG_LEN*2, NULL, NULL);
            //上行不trim
			if (!bMo) nMsgLen = TrimRight(szMsg, nMsgLen);
			nMsgLen = strlen(szMsg);
        }
    }
    /*else if ((4 == nMsgFmt || 248 == nMsgFmt || 246 == nMsgFmt) && !bMo)
    {
        //检查长度是否超过MAX_SINGLE_LEN*2，若超过MAX_SINGLE_LEN*2返回超长
        //若长度合法则转成HEX编码
        if (nMsgLen > MAX_SINGLE_LEN*2)
        {
            nRet = 6;
        }
        else
        {
            char szBuf[MAX_MSG_LEN*2+1] = {0};
            int nLen = nMsgLen;
            nMsgLen = MemBytesToHexStr((UCHAR*)szMsg, nLen, szBuf);
            memset(szMsg, 0, nLen);
            memcpy(szMsg, szBuf, nMsgLen);
        }
    }*/
	//不是0/8/15的编码且不是上行
	else if (!bMo && bTransHex)
	{
        //检查长度是否超过MAX_SINGLE_LEN*2，若超过MAX_SINGLE_LEN*2返回超长
        //若长度合法则转成HEX编码
        if (nMsgLen > MAX_SINGLE_LEN*2)
        {
            nRet = 6;
        }
        else
        {
            char szBuf[MAX_MSG_LEN*2+1] = {0};
            int nLen = nMsgLen;
            nMsgLen = MemBytesToHexStr((UCHAR*)szMsg, nLen, szBuf);
            memset(szMsg, 0, nLen);
            memcpy(szMsg, szBuf, nMsgLen);
        }
	}
    else
    {
        nMsgLen = CAdapter::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szMsg, nMsgLen, wcMsg, sizeof(wcMsg));

        if (nMsgLen > nMaxLen)
        {
            nRet = 6;
        }
        else
        {
            nMsgLen = CAdapter::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wcMsg, nMsgLen, szMsg, MAX_MSG_LEN*2, NULL, NULL);
            //上行不trim
			if (!bMo) nMsgLen = TrimRight(szMsg, nMsgLen);
			nMsgLen = strlen(szMsg);
        }
    }
    if (nRet != 0)
    {
		nMsgLen = nSrcMsgLen;
    }
    return nRet;
}

//计算短信长度
int  CGlobal::CalcMsgLen(int nMsgFmt, int nMsgLen, const char *szInMsg)
{
    wchar_t wcMsg[4096] = {0};
    char szMsg[2048+1] = {0};
    strcpy(szMsg, szInMsg);
    if (8 == nMsgFmt)
    {
        char szBuf[2048+1] = {0};
        int  nLen   = nMsgLen;
        int  OutLen = 0;
        US2ToAscii(szMsg, nLen, szBuf, &OutLen);
        OutLen  = CAdapter::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szBuf, OutLen, wcMsg, sizeof(wcMsg));
        nMsgLen = OutLen;
    }
    else
    {
        nMsgLen = CAdapter::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szMsg, nMsgLen, wcMsg, sizeof(wcMsg));
    }

    return nMsgLen;
}

BOOL CGlobal::InitSocket()
{
//linux下无需初始化socket库
#ifndef LINUX
    WORD wVersionRequired;
    WSADATA wsaData;
    wVersionRequired = MAKEWORD(2, 2);

    int err = WSAStartup(wVersionRequired, &wsaData);
    if (0 != err)
    {
        return FALSE;
    }
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wHighVersion) != 2)
    {
        //not support
        WSACleanup();
        return FALSE;
    }
#endif
    return TRUE;
}

BOOL CGlobal::UnInitSocket()
{
#ifndef LINUX
    WSACleanup();
#endif
    return TRUE;
}

//由URL地址取得IP地址和端口
int  CGlobal::GetIpFromUrl(const char *szUrl, char *szWebPath, char *szIp, char* szDomain, int& nPort)
{
    int nRet = -1;
	if (NULL == szUrl || '\0' == szUrl[0] || NULL == szIp || NULL == szWebPath || NULL == szDomain)
    {
        return nRet;
    }
    //"http://192.169.1.130:8081/sms/mt"
    const char *pStart = CAdapter::StrStrI(szUrl, "http://");
    if (NULL != pStart)
    {
        pStart += strlen("http://");
        ///sms/mt
        const char *p1 = strstr(pStart, "/");
        if (NULL != p1)
        {
            strcpy(szWebPath, p1);
        }
        //192.169.1.130:8081
        const char *p2 = strstr(pStart, ":");
        if (NULL != p2)
        {
            memcpy(szIp, pStart, p2-pStart);
            nPort = atoi(p2+1);
        }
        else
        {
			if (p1 != NULL)
			{
				memcpy(szIp, pStart, p1-pStart);
			}
			else
			{
				strcpy(szIp, pStart);
			}
			nPort = 80;
		}
		strcpy(szDomain, szIp);
        InitSocket();
        struct hostent     *pHostInfo = NULL;
        struct sockaddr_in svrAddr;
        //包含主机名字和地址信息的hostent结构指针
        pHostInfo = gethostbyname(szIp);
        if (pHostInfo == NULL)
        {
            //IP解析失败或无效的IP,或网络不通
            pHostInfo = gethostbyaddr(szIp, strlen(szIp), AF_INET);
            if (NULL != pHostInfo)
            {
                nRet = 0;
            }
        }
        else
        {
            svrAddr.sin_family = AF_INET;
            memcpy(&svrAddr.sin_addr, pHostInfo->h_addr_list[0], pHostInfo->h_length);
            memset(szIp, 0, sizeof(szIp));
            strcpy(szIp, inet_ntoa(svrAddr.sin_addr));
            nRet = 0;
        }

        UnInitSocket();
    }

    return nRet;
}


int CGlobal::SplitStr(const char *szInStr, std::vector<std::string>& vOutStr, const char* pObj, int nMaxLen)
{
    int nCount = 0;
    if (NULL != szInStr && szInStr[0] != '\0')
    {
        int nLen = strlen(szInStr);
        char szSegStr[1024 + 1] = { 0 };
        char *pStart = (char *)szInStr;
        char *pEnd = new char[strlen(strstr(szInStr, pObj)) + 1];
        strcpy(pEnd, strstr(szInStr, pObj));
        if (NULL == pEnd)
        {
            memset(szSegStr, 0, nMaxLen + 1);
            memcpy(szSegStr, szInStr, MIN2(nLen, sizeof(szSegStr)));
            vOutStr.push_back(szSegStr);
            ++nCount;
        }
        else
        {
            ++nCount;
            while (pEnd)
            {
                int nPos = pStart - szInStr;
                memset(szSegStr, 0, nMaxLen + 1);
                memcpy(szSegStr, szInStr + nPos, MIN2(pEnd - pStart, sizeof(szSegStr)));
                vOutStr.push_back(szSegStr);
                pStart = pEnd + 1;
                pEnd = strstr(pStart, pObj);
                if (pEnd == NULL)
                {
                    nPos = pStart - szInStr;
                    memset(szSegStr, 0, strlen(szSegStr));
                    memcpy(szSegStr, szInStr + nPos, MIN2(nLen - nPos, sizeof(szSegStr)));
                    vOutStr.push_back(szSegStr);
                }
                ++nCount;
            }
        }
    }
    //若是空字符串，返回手机号为空，个数为1
    else if (NULL != szInStr && szInStr[0] == '\0')
    {
        ++nCount;
        vOutStr.push_back(std::string(""));
    }
    return nCount;
}

int CGlobal::SplitStr(const char *szInStr, const char* szSign, std::vector<std::string>& vOutStr, int nMaxLen)
{
    int nCount = 0;
    if (NULL != szInStr && szInStr[0] != '\0')
    {
        int nLen = strlen(szInStr);
        char szSegStr[1024 + 1] = { 0 };
        char *pStart = (char *)szInStr;
        char *pEnd = (char *)strstr(szInStr, szSign);
        if (NULL == pEnd)
        {
            memset(szSegStr, 0, nMaxLen + 1);
            memcpy(szSegStr, szInStr, MIN2(nLen, sizeof(szSegStr)));
            vOutStr.push_back(szSegStr);
            ++nCount;
        }
        else
        {
            ++nCount;
            while (pEnd)
            {
                int nPos = pStart - szInStr;
                memset(szSegStr, 0, nMaxLen + 1);
                memcpy(szSegStr, szInStr + nPos, MIN2(pEnd - pStart, sizeof(szSegStr)));
                vOutStr.push_back(szSegStr);
                pStart = pEnd + 1;
                pEnd = strstr(pStart, szSign);
                if (pEnd == NULL)
                {
                    nPos = pStart - szInStr;
                    memset(szSegStr, 0, strlen(szSegStr));
                    memcpy(szSegStr, szInStr + nPos, MIN2(nLen - nPos, sizeof(szSegStr)));
                    vOutStr.push_back(szSegStr);
                }
                ++nCount;
            }
        }
    }
    //若是空字符串，返回手机号为空，个数为1
    else if (NULL != szInStr && szInStr[0] == '\0')
    {
        ++nCount;
        vOutStr.push_back(std::string(""));
    }
    return nCount;
}

//拆分以逗号分隔的字符串
int CGlobal::SplitStr(const char *szInStr, std::vector<std::string>& vOutStr, int nMaxLen)
{
    int nCount = 0;
    if (NULL != szInStr && szInStr[0] != '\0')
    {
        int nLen = strlen(szInStr);
        char szSegStr[1024+1] = {0};
        const char *pStart = szInStr;
        const char *pEnd   = strstr(szInStr, ",");
        if (NULL == pEnd)
        {
            memset(szSegStr, 0, nMaxLen+1);
            memcpy(szSegStr, szInStr, MIN2(nLen, sizeof(szSegStr)));
            vOutStr.push_back(szSegStr);
            ++nCount;
        }
        else
        {
            ++nCount;
            while (pEnd)
            {
                int nPos = pStart-szInStr;
                memset(szSegStr, 0, nMaxLen+1);
                memcpy(szSegStr, szInStr+nPos, MIN2(pEnd-pStart, sizeof(szSegStr)));
                vOutStr.push_back(szSegStr);
                pStart = pEnd+1;
                pEnd = strstr(pStart, ",");
                if (pEnd == NULL)
                {
                    nPos = pStart-szInStr;
                    memset(szSegStr, 0, strlen(szSegStr));
                    memcpy(szSegStr, szInStr+nPos, MIN2(nLen-nPos, sizeof(szSegStr)));
                    vOutStr.push_back(szSegStr);
                }
                ++nCount;
            }
        }
    }
	//若是空字符串，返回手机号为空，个数为1
	else if (NULL != szInStr && szInStr[0] == '\0')
	{
		++nCount;
		vOutStr.push_back(std::string(""));
	}
    return nCount;
}
/*
//拆分以逗号分隔的字符串
int CGlobal::SplitStr(const char *szInStr, std::vector<std::string>& vOutStr, int nMaxLen)
{
    int nCount = 0;
    if (NULL != szInStr && szInStr[0] != '\0')
    {
        int nLen        = strlen(szInStr);
        char *pSegStr   = new char[nMaxLen+1];
        if (pSegStr)
        {
            try
            {
                char *pStart    = (char *)szInStr;
                char *pEnd      = strstr(szInStr, ",");
                if (NULL == pEnd)
                {
                    memset(pSegStr, 0, nMaxLen+1);
                    memcpy(pSegStr, szInStr, MIN2(nLen, nMaxLen));
                    vOutStr.push_back(pSegStr);
                    ++nCount;
                }
                else
                {
                    ++nCount;
                    while (pEnd)
                    {
                        int nPos = pStart-szInStr;
                        memset(pSegStr, 0, nMaxLen+1);
                        memcpy(pSegStr, szInStr+nPos, MIN2(pEnd-pStart, nMaxLen));
                        vOutStr.push_back(pSegStr);
                        pStart = pEnd+1;
                        pEnd = strstr(pStart, ",");
                        if (pEnd == NULL)
                        {
                            nPos = pStart-szInStr;
                            memset(pSegStr, 0, strlen(pSegStr));
                            memcpy(pSegStr, szInStr+nPos, MIN2(nLen-nPos, nMaxLen));
                            vOutStr.push_back(pSegStr);
                        }
                        ++nCount;
                    }
                }
            }
            catch (...)
            {
            }
        }
        if (NULL != pSegStr)
        {
            delete []pSegStr;
            pSegStr = NULL;
        }
    }
	//若是空字符串，返回手机号为空，个数为1
	else if (NULL != szInStr && szInStr[0] == '\0')
	{
		++nCount;
		vOutStr.push_back(std::string(""));
	}
    return nCount;
}
*/

//拆分以逗号分隔的字符串
int CGlobal::SplitRptMoStr(const char *szInStr, int nLen, std::vector<std::string>& vOutStr)
{
    int nCount = 0;
    if (NULL != szInStr && szInStr[0] != '\0')
    {
        //int nLen = strlen(szInStr);
        char pSegStr[8 * MAX_MSG_LEN + 1] = { 0 };
        const char *pStart = (char *)szInStr;
        const char *pEnd = strstr(szInStr, ",");
        if (NULL == pEnd)
        {
            memset(pSegStr, 0, sizeof(pSegStr));
            memcpy(pSegStr, szInStr, nLen);
            vOutStr.push_back(pSegStr);
            ++nCount;
        }
        else
        {
            ++nCount;
            while (pEnd)
            {
                int nPos = pStart - szInStr;
                memset(pSegStr, 0, sizeof(pSegStr));
                memcpy(pSegStr, szInStr + nPos, pEnd - pStart);
                vOutStr.push_back(pSegStr);
                pStart = pEnd + 1;
                pEnd = strstr(pStart, ",");
                if (pEnd == NULL)
                {
                    nPos = pStart - szInStr;
                    memset(pSegStr, 0, sizeof(pSegStr));
                    memcpy(pSegStr, szInStr + nPos, nLen - nPos);
                    vOutStr.push_back(pSegStr);
                }
                ++nCount;
            }
        }
    }
    //若是空字符串，返回为空，个数为1
    else if (NULL != szInStr && szInStr[0] == '\0')
    {
        ++nCount;
        vOutStr.push_back(std::string(""));
    }
    return nCount;
}

//转换UTF-8特殊字符
int CGlobal::TranHttpSpace(char *buf)
{
    int i = 0;
    int j = 0;
	int nLen = strlen(buf);
    while (i < nLen)
    {
        //&lt;
        if ('&' == buf[i] && 'l' == buf[i+1] && 't' == buf[i+2] && ';' == buf[i+3])
        {
            buf[j++] = '<';
            i += 4;
        }
        //&gt;
        else if ('&' == buf[i] && 'g' == buf[i+1] && 't' == buf[i+2] && ';' == buf[i+3])
        {
            buf[j++] = '>';
            i += 4;
        }
        //&amp;
        else if ('&' == buf[i] && 'a' == buf[i+1] && 'm' == buf[i+2] && 'p' == buf[i+3] && ';' == buf[i+4])
        {
            buf[j++] = '&';
            i += 5;
        }
        //&quot;
        else if ('&' == buf[i] && 'q' == buf[i+1] && 'u' == buf[i+2] && 'o' == buf[i+3] && 't' == buf[i+4] && ';' == buf[i+5])
        {
            buf[j++] = '\"';
            i += 6;
        }
        //&apos;
        else if ('&' == buf[i] && 'a' == buf[i+1] && 'p' == buf[i+2] && 'o' == buf[i+3] && 's' == buf[i+4] && ';' == buf[i+5])
        {
            buf[j++] = '\'';
            i += 6;
        }
        else
        {
            buf[j++] = buf[i++];
        }
    }
    buf[j] = 0x0;
    return 0;
}

//base64加密
int CGlobal::Base64Encode(UCHAR* pbSrc, DWORD SrcLen, UCHAR* pbDes)
{
    UCHAR* pbOut = pbDes;
    DWORD dwSrc;
    if( SrcLen<=0 ) return 0;


    DWORD iSrc=0;
    for(iSrc=0;iSrc<SrcLen;iSrc+=3 ){

        dwSrc =  *pbSrc++;
        dwSrc <<= 8;
        dwSrc += *pbSrc++;
        dwSrc <<= 8;
        dwSrc += *pbSrc++;

        *pbOut++ = base64_alphabet[ (dwSrc>>18) & 0x3F ];
        *pbOut++ = base64_alphabet[ (dwSrc>>12) & 0x3F ];
        *pbOut++ = base64_alphabet[ (dwSrc>>6)  & 0x3F ];
        *pbOut++ = base64_alphabet[ (dwSrc)     & 0x3F ];

    }

    iSrc -= SrcLen;

    if( iSrc==2 ){//需要补2位
        *(pbOut-1) = *(pbOut-2) = '=';
    }else if( iSrc==1 ){//需要补1位
        *(pbOut-1) = '=';
    }
    *pbOut = 0;
    iSrc = (int)(pbOut-pbDes);
    return  iSrc;
}

const unsigned char base64_decode_map[256] = {
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 62, 255, 255, 255, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 255, 255,
		255, 0, 255, 255, 255, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 255, 255, 255, 255, 255, 255, 26, 27, 28,
		29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
		49, 50, 51, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

//base64解密
int CGlobal::Base64Decode(UCHAR* pbSrc, DWORD SrcLen, UCHAR* pbDes)
{
    UCHAR* pbOut = pbDes;
    DWORD b1,b2,b3,b4;

    DWORD iSrc=0;
    for( iSrc=0;iSrc<SrcLen;iSrc+=4 ){
        b1 = *pbSrc++;
        b2 = *pbSrc++;
        b3 = *pbSrc++;
        b4 = *pbSrc++;
        *pbDes++ = base64_decode_map[b1]<<2 | base64_decode_map[b2]>>4;
        *pbDes++ = base64_decode_map[b2]<<4 | base64_decode_map[b3]>>2;
        *pbDes++ = base64_decode_map[b3]<<6 | base64_decode_map[b4];
    }

    if( SrcLen ){
        if( *--pbSrc=='=' ){
            --pbDes;
            if( *--pbSrc=='=' ) --pbDes;
        }
    }
    *pbDes = 0;
    return (int)(pbDes-pbOut);
}

//时间变成字符串
std::string CGlobal::DateToTimeStr(DATE dt)
{
    char szBuf[25+1] = {0};
#ifdef WIN32
    COleDateTime oletime(dt);
    sprintf(szBuf, "%02d:%02d:%02d",
            oletime.GetHour(),
            oletime.GetMinute(),
            oletime.GetSecond()
            );
#elif defined LINUX
	int nTotalSec = (int)dt;
	int nHour = nTotalSec/3600;
	int nMin  = (nTotalSec - nHour*3600)/60;
	int nSec  = nTotalSec - nMin*60;
	sprintf(szBuf, "%02d:%02d:%02d",
		nHour,
		nMin,
		nSec);
#endif
    return std::string(szBuf);
}

//取xml指定命令下的键值
int CGlobal::GetCmdValue(char *Data, char *CmdItem, int MaxSize, char *CmdVal, bool bUTF8)
{
    char *p1 = NULL;
    char *p2 = NULL;
    char find[256] = "0";

    memset(find, 0, sizeof(find));
    strcpy(find, "<");
    strcat(find, CmdItem);
    p1 = CAdapter::StrStrI(Data, find);
    if (NULL == p1)
    {
        return -1;
    }

    memset(find, 0, sizeof(find));
    strcat(find, ">");
    p2 = CAdapter::StrStrI(p1, find);
    if (NULL == p2)
    {
        return -1;
    }
    p1 = p2 + 1;

    memset(find, 0, sizeof(find));
    strcpy(find, "</");
    strcat(find, CmdItem);
    strcat(find, ">");

    p2 = CAdapter::StrStrI(p1, find);
    if (NULL == p2)
    {
        //这里防止用户对整个xml文档进行utf-8致使</>中的<被转了码转成了0x3c
        //若没有发现</cmd>就来找/cmd>找到后指针后移一位
        memset(find, 0, sizeof(find));
        strcpy(find, "/");
        strcat(find, CmdItem);
        strcat(find, ">");

        p2 = CAdapter::StrStrI(p1, find);
        if (NULL == p2)
        {
            return -1;
        }
        --p2;
    }

    int nCmdValueLen = 0;
    if (bUTF8)
    {
        char *szTmp = new char[p2 - p1 + 1];
        if (szTmp)
        {
            try
            {
                memset(szTmp, 0, p2-p1+1);
                UTF_8ToGB2312(szTmp, p1, p2 - p1);
                if (strlen(szTmp) > (unsigned int)MaxSize)
                {
                    if (szTmp)
                    {
                        delete [] szTmp;
                        szTmp = NULL;
                    }
                    return -2;
                }
                strcpy(CmdVal, szTmp);
            }
            catch (...)
            {
            }
        }
        if (szTmp)
        {
            delete [] szTmp;
            szTmp = NULL;
        }
        nCmdValueLen = strlen(CmdVal);
    }
    else
    {
        if (p2 - p1 > MaxSize)
        {
            return -2;
        }
        memcpy(CmdVal, p1, p2 - p1);
        CmdVal[p2-p1] = 0x0;
        nCmdValueLen = p2 - p1;
    }
    return nCmdValueLen;//strlen(CmdVal);
}

//解析Http-post请求的内容参数以&符号分开的这种形式
int CGlobal::GetCmdValueEx(char *Data, char *CmdItem, int MaxSize, char *CmdVal, bool bConvert)
{
    char *p1 = NULL;
    char *p2 = NULL;
    char find[256] = {0};

    strcat(find, CmdItem);
    strcat(find, "=");
    p1 = CAdapter::StrStrI(Data, find);
    if (NULL == p1)
    {
        return -1;
    }
    p2 = p1 + strlen(find);
//  memset(find, 0, sizeof(find));
//  strcat(find, "=");
//  p2 = CAdapter::StrStrI(p1, find);
//  if (NULL == p2)
//  {
//      return -1;
//  }
    p1 = p2;

    memset(find, 0, sizeof(find));
    strcpy(find, "&");

    p2 = CAdapter::StrStrI(p1, find);
    if (NULL == p2)
    {
        p2 = &Data[strlen(Data)];
    }

    int nCmdValueLen = 0;
    if (bConvert)
    {
        int nOutLen = p2 - p1;
        char *szTmp = new char[nOutLen + 1];
        if (NULL != szTmp)
        {
            try
            {
                memset(szTmp, 0, nOutLen+1);
                DecodeHtmlStr(szTmp, p1, nOutLen);
                if (nOutLen > (unsigned int)MaxSize)
                {
                    if (NULL != szTmp)
                    {
                        delete[] szTmp;
                        szTmp = NULL;
                    }
                    return -2;
                }
                UTF_8ToGB2312(CmdVal, szTmp, nOutLen);
            }
            catch (...)
            {
            }
        }
        if (NULL != szTmp)
        {
            delete[] szTmp;
            szTmp = NULL;
        }
        nCmdValueLen = strlen(CmdVal);
    }
    else
    {
        if (p2 - p1 > MaxSize)
        {
            return -2;
        }
        memcpy(CmdVal, p1, p2 - p1);
        CmdVal[p2-p1] = 0x0;
        nCmdValueLen = p2-p1;
    }
    return nCmdValueLen;/*strlen(CmdVal);*/
}

//解析JAVA网页编码
void CGlobal::ParaseJavaEncode(char *szDest, char *szSrc, int& nInOutLen)
{
    if (NULL == szDest)
    {
        nInOutLen = 0;
        return ;
    }
    if (NULL == szSrc)
    {
        szDest[0] = '\0';
        nInOutLen = 0;
        return;
    }
    if (NULL == CAdapter::StrStrI(szSrc, "&#x"))
    {
        nInOutLen = 0;
        return;
    }
    /*&#x606D;;123&#xFF01;"*/
    int i           = 0;
    int nPos        = 0;
    char szTmp[2+1] = {0};
    char *pPos = szSrc;
    while (i < nInOutLen)
    {
        bool bSplit = false;
        if (*pPos == '&' && *(pPos+1) == '#' && (*(pPos+2) == 'x'||*(pPos+2) == 'X'))
        {
            bSplit = true;
            pPos += 3;
            i += 3;
        }
        if (bSplit)
        {
            memset(szTmp, 0, 2);
            memcpy(szTmp, pPos, 2);
            int nRet = 0;
            sscanf(szTmp, "%x", &nRet);
            szDest[nPos++] = nRet;
            pPos += 2;
            i    += 2;

            memset(szTmp, 0, 2);
            memcpy(szTmp, pPos, 2);
            sscanf(szTmp, "%x", &nRet);
            szDest[nPos++] = nRet;
            pPos += 2;
            i    += 2;
            ++pPos;//;
            ++i;
        }
        else
        {
            char szBuf[1+1] = {0};
            UCHAR szBufOut[10+1] = {0};
            szBuf[0] = *pPos++;
            int nOutLen = 0;
            AsciiToUS2(szBuf, szBufOut, (UINT*)&nOutLen);
            memcpy(szDest+nPos, szBufOut, nOutLen);
            nPos += nOutLen;
            ++i;
        }
    }
    nInOutLen = nPos;
}

//解析URL-ENCODE
void CGlobal::DecodeHtmlStr(char *szDest, char *szSrc, int& nInOutLen)
{
    if (NULL == szDest)
    {
        nInOutLen = 0;
        return ;
    }
    if (NULL == szSrc)
    {
        szDest[0] = '\0';
        nInOutLen = 0;
        return;
    }
    int i           = 0;
    int nPos        = 0;
    char szTmp[2+1] = {0};
    char *pPos = szSrc;
    while (i < nInOutLen)
    {
        bool bSplit = false;
        if (*pPos == '%')
        {
            bSplit = true;
            pPos += 1;
            i += 1;
        }
        if (bSplit)
        {
            memset(szTmp, 0, 2);
            memcpy(szTmp, pPos, 2);
            int nRet = 0;
            sscanf(szTmp, "%x", &nRet);
            szDest[nPos++] = nRet;
            pPos += 2;
            i    += 2;
        }
        else
        {
            szDest[nPos] = *pPos;
            if ('+' == szDest[nPos])
            {
                szDest[nPos] = ' ';
            }
            ++pPos;
            ++nPos;
            ++i;
        }
    }
    nInOutLen = nPos;
}

//取用户特权属性值
int CGlobal::GetUserPrivilege(int nPrivilege, int nBit)
{
    if (nBit <= 0)
    {
        return 0;
    }
    if (nBit > 1)
    {
        nBit = 1 << (nBit-1);
    }
    return nPrivilege&nBit;
}

void CGlobal::SetBitValue(int& nValue, int nBit, bool bBit)
{
	if (bBit)
	{
		SetBit(nValue, nBit);
	}
	else 
	{
		ClrBit(nValue, nBit);
	}
}

void CGlobal::SetBit(int& nValue, int nBit)
{
	nValue |= (1 << nBit);
}

//置一个大整型数某一位为1
void CGlobal::SetInt64Bit(INT64& nValue, int nBit)
{
	nValue |= (1 << nBit);
}

void CGlobal::ClrBit(int& nValue, int nBit)
{
	nValue &= ~(1 << nBit);
}

bool CGlobal::GetBit(int nValue, int nBit)
{
	return (nValue >> (nBit)&1);
}

//取一个大整型某一位是否为1
bool CGlobal::GetInt64Bit(INT64 nValue, int nBit)
{
	return (nValue >> (nBit)&1);
}

bool CGlobal::IsZHCN(const char* str)
{
	return ((unsigned int)*str > 128 && (unsigned int)*(str+1) > 128);
}

//控制蜂鸣报警
void CGlobal::BeepEx(DWORD dwFreq, DWORD dwDuration)
{
//     if (BeepOrNot)
//     {
// 		//蜂鸣报警
//         CAdapter::Beep(dwFreq, dwDuration);
// 		try
// 		{
// 			//音乐报警
// 			char szTmp[MAX_PATH+1] = {0};
// 			char szWavFile[MAX_PATH+1] = {0};
// 			CAdapter::GetCurrentPath(szWavFile, MAX_PATH);
// 			strcat(szWavFile, "/warning.wav"); 
// 			PlaySound(szWavFile, NULL, SND_ASYNC|SND_FILENAME);
// 			PlaySound(szWavFile, GetModuleHandle(NULL), SND_RESOURCE);
// 		}
// 		catch (...)
// 		{
// 		}
//     }
//     else
//     {
//         CAdapter::Sleep(dwDuration);
//     }
}

//使系统时间有效，若系统时间没问题格式化成想要的时间，若有问题则取当前时间
void CGlobal::MakeSysTmValid(const char* szInTm, char* szOutTm)
{
	if (NULL != szInTm && NULL != szOutTm)
	{
		int	nYear = 0,nMonth = 0,nDay = 0,nHour = 0,nMin = 0,nSec = 0,nMillSec = 0;
		//取时间
		sscanf(szInTm, "%d-%d-%d %d:%d:%d:%d",
			&nYear, &nMonth, &nDay, &nHour, &nMin, &nSec, &nMillSec);
		if (nYear <= 0 
			|| (nMonth <=0 || nMonth > 12) 
			|| (nDay <= 0 || nDay > 31)
			|| (nHour < 0 || nHour > 23)
			|| (nMin < 0 || nMin > 59)
			|| (nSec < 0 || nSec > 59)
			)
		{
			SYSTEMTIME_EX t;
			CAdapter::GetLocalTime(&t);
			nYear	= t.m_iYear;
			nMonth	= t.m_iMonth;
			nDay	= t.m_iDay;
			nHour	= t.m_iHour;
			nMin	= t.m_iMinute;
			nSec	= t.m_iSecond;
			nMillSec= t.m_iMilliseconds;
		}
		memset(szOutTm, 0, strlen(szOutTm));
		sprintf(szOutTm, "%04d-%02d-%02d %02d:%02d:%02d.%03d", nYear, nMonth, nDay, nHour, nMin, nSec, nMillSec);
	}
}

//将手机号列表转化为协议需的destid
void CGlobal::PhoneListToDestId(char *szDest, const char *szSrc, UINT nSrcLen, UINT nFixDestIdLen)
{
    if (NULL == szDest || NULL == szSrc)
    {
        return;
    }
    int i = 0;
    int k = 0;
    int m = 0;
    while (i < nSrcLen)
    {
        while (szSrc[i] == ',')
        {
            ++k;
            ++i;
            m = 0;
        }
		szDest[k*nFixDestIdLen+m] = szSrc[i];
		//memcpy(szDest+k*nFixDestIdLen+m, szSrc+i, 1);
        ++m;
        ++i;
    }
}

//使写库短信有效，将编码8转换为15，将短信内容中的'替换为''
void CGlobal::MakeMsgValid(const char* szInMsg, char* szOutMsg, int nMsgFmt, int nInLen, int nMaxOutSize, bool bConvertFmt, bool bTrimQuote)
{
	if (NULL != szInMsg && NULL != szOutMsg && nInLen > 0)
	{
		int nOutLen = nInLen;
		memset(szOutMsg, 0, nMaxOutSize);
		if (8 == nMsgFmt && bConvertFmt)
		{
			US2ToAscii(szInMsg, nInLen, szOutMsg, &nOutLen);
		}
		/*
        //Begin add by liuhuan 2011-08-10 加密短信写库时做base64转换处理
        else if (25 == nMsgFmt && bConvertFmt)
        {
            GetBase64FromEncryptMsg(szInMsg, nInLen, szOutMsg);
			nOutLen = strlen(szOutMsg);
        }
		*/
        //End Add by liuhuan 2011-08-10
		else
		{
			nOutLen = MIN2(nInLen, nMaxOutSize);
			memcpy(szOutMsg, szInMsg, nOutLen);
		}
		//nOutLen = TrimRight(szOutMsg, nOutLen, '\\');
		//ReplaceChar((unsigned char*)szOutMsg, '\'', '"', nOutLen);
		if (bTrimQuote)
		{
			std::string strOutMsg = szOutMsg;
			cstr::replace(strOutMsg, "'", "''");
			int nLen = strOutMsg.size();
			nLen>nMaxOutSize?nOutLen=0:nOutLen=nLen;
			memcpy(szOutMsg, strOutMsg.c_str(), nOutLen);
		}

		//这里再做一次短信内容有效性校验
		wchar_t wcMsg[4096] = {0};
		int nMsgLenSrc = CAdapter::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szOutMsg, nOutLen, wcMsg, sizeof(wcMsg));
		memset(szOutMsg, 0, nOutLen);
		CAdapter::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wcMsg, nMsgLenSrc, szOutMsg, nMaxOutSize, NULL, NULL);
	}
}


//////////////////////////////////////////////////////////////////

CSpeedLimit::CSpeedLimit()
{
	m_nPrecision	= 100;                        
	m_nMaxSpeed		= 10;                         
	m_nCount		= 0;
	m_nLastTm		= CAdapter::GetTickCount();
}

CSpeedLimit::~CSpeedLimit()
{

}

int CSpeedLimit::LimitSpeed(int nCount)
{
	int nLastTm = CAdapter::GetTickCount();
	int nPreTm  = m_nLastTm;
	int nDeltTm = nLastTm - nPreTm;
	if (nDeltTm < 0)
	{
		nPreTm  = m_nLastTm = nLastTm;
		nDeltTm = 0;
		m_nCount= 0;
	}	
	int nSndNum = CAdapter::InterlockedExchangeAdd((long*)&m_nCount, nCount);
	nSndNum += nCount;

	if (nDeltTm >= 2000)
	{
		CAdapter::InterlockedExchangeAdd((long*)&m_nLastTm, nLastTm - nPreTm);
		CAdapter::InterlockedExchangeAdd(&m_nCount, 1 - nSndNum);
		return 0;
	}
	
	int nAvgSndNum = int(m_nMaxSpeed * m_nPrecision / 1000.0);	
	if (nAvgSndNum <= 0) nAvgSndNum = 1;
	//调速
	//if (nSndNum%nAvgSndNum == 0 || nSndNum >= m_nMaxSpeed)
	{	
		//超速
		if ((double)nSndNum > (double)((m_nMaxSpeed * nDeltTm)/1000.0))
		{
			//延时ms
			int nWaitTime = (int)(nSndNum * 1000.0 / m_nMaxSpeed) - nDeltTm;
			nWaitTime<0?nWaitTime=0:1;
			nWaitTime>60?nWaitTime=60:1;
			if (nWaitTime > 0) CAdapter::InterlockedExchangeAdd((long*)&m_nCount, -nCount);
			return nWaitTime;
			//CAdapter::Sleep(minn(nWaitTime, 2000));
		}
	}
	return 0;
}

void CSpeedLimit::SetMaxSpeed(int nMaxSpd)
{
	if (nMaxSpd < 1)
	{
        m_nMaxSpeed = 1;
	}
	else
	{
		m_nMaxSpeed = nMaxSpd+5;
	} 
}

int	CSpeedLimit::GetMaxSpeed()
{
	return m_nMaxSpeed-5;
}
////////////////////////////////////////////////
