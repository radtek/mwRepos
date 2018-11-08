
#include "Global.h"
#include "TypeDef.h"
//zhubo 2017-4-6
//#include "sys/stat.h"
//#include "process.h"
#include "iconv.hpp"




CONST BYTE base64_alphabet[] = {
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P', 
		'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f', 
		'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v', 
		'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/',
		'='
};
bool CGlobal::BeepOrNot = TRUE;

CGlobal::CGlobal()
{	

}

int MIN2(int a, int b)
{
    return a>b ? b : a;
}


int max(int i1, int i2)
{
    return i1 > i2 ? i1 : i2;
}


int min(int i1, int i2)
{
    return i1 < i2 ? i1 : i2;
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

int CGlobal::HexchartoInt_ex(char chHex)
{
	int iDec = 0;
	switch (chHex)
	{
	case '0':
		iDec = 0;
		break;
	case '1':
		iDec = 1;
		break;
	case '2':
		iDec = 2;
		break;
	case '3':
		iDec = 3;
		break;
	case '4':
		iDec = 4;
		break;
	case '5':
		iDec = 5;
		break;
	case '6':
		iDec = 6;
		break;
	case '7':
		iDec = 7;
		break;
	case '8':
		iDec = 8;
		break;
	case '9':
		iDec = 9;
		break;
	case 'A':
		iDec = 10;
		break;
	case 'B':
		iDec = 11;
		break;
	case 'C':
		iDec = 12;
		break;
	case 'D':
		iDec = 13;
		break;
	case 'E':
		iDec = 14;
		break;
	case 'F':
		iDec = 15;
		break;
	case 'a':
		iDec = 10;
		break;
	case 'b':
		iDec = 11;
		break;
	case 'c':
		iDec = 12;
		break;
	case 'd':
		iDec = 13;
		break;
	case 'e':
		iDec = 14;
		break;
	case 'f':
		iDec = 15;
		break;
	default:
		iDec = 0xFF;
		break;
	}
	return iDec;
}
//对短信内容进行HEX解码
int	CGlobal::TransMsg(char *szDest, const char *szSrc, int nMsgFmt, int nSrcLen, int nMaxSize)
{
	if (NULL == szSrc || NULL == szDest || 0 == nSrcLen || 0 == nMaxSize)
	{
		return 0;
	}
	int nReadLen = min(nSrcLen, nMaxSize);
	int nLen = 0;
	if (8 == nMsgFmt)
	{
		for (int i = 0, k = 0; i < nReadLen; i += 2)
		{
			szDest[k++] = HexchartoInt(szSrc[i]) * 16 + HexchartoInt(szSrc[i+1]);
		}
		nLen = nReadLen/2;
	}
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
	int nReadLen = min(nSrcLen, nDestSize);
	for (int i = 0, k = 0; i < nReadLen; i += 2)
	{
		szDest[k++] = HexchartoInt(szSrc[i]) * 16 + HexchartoInt(szSrc[i+1]);
	}
	return nReadLen/2;
}

//替换函数将szSrc中的cSrc替换为cDest
void CGlobal::ReplaceChar(unsigned char *szSrc, char cSrc, char cDest, unsigned int nSrcLen)
{
	if (NULL != szSrc)
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


//ASCII转换为UNICODE
/*
int CGlobal::AsciiToUS2(const char* Msg, unsigned char *Data, unsigned int *Len)
{
	wchar_t temp[1024*16]={0};
	char szUMsg[1024*16]={0};
	try
	{
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,Msg, strlen(Msg), temp, sizeof(temp));
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
	catch(...)
	{
	}
	return -99;
}
*/

//UNICODE转换为ASCII
int CGlobal::US2ToAscii(const char *InMsg, int InLen, char *Data, int *Len)
{
	wchar_t uMsg[1024*16] = {0};
	int  uLen = 0;
	char temp[1024*16] = {0};
	
	try
	{
		uLen = InLen/2;
		memset(uMsg, '\0', sizeof(uMsg));
		
		for (int i=0; i<uLen; ++i)
		{
			uMsg[i] = InMsg[2*i] & 0xff;
			uMsg[i] = uMsg[i] << 8;
			uMsg[i] = uMsg[i] + (InMsg[2*i+1] & 0xff);
		}
		uLen = CAdapter::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, (LPCWSTR)uMsg, uLen, (LPSTR)temp, sizeof(temp), (LPCSTR)NULL, (LPBOOL)NULL);		
		*Len = uLen;
		memcpy(Data, temp, uLen);
		return 0;
	}
	catch(...)
	{
	}
	return -99;	
}


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

int CGlobal::MemBytesToHexStr(const unsigned char *szSrc, int nLen, char *szDest, bool bLowFlag)
{
	int nDestLen = 0;
	if (NULL != szSrc && NULL != szDest)
	{
		int nRet = 0;
		int nPos = 0;
		for (int i = 0; i < nLen; ++i)
		{
			if(bLowFlag)
			{
				nRet = sprintf(szDest+nPos, "%02x", szSrc[i]&0xff);
			}
			else
			{
				nRet = sprintf(szDest+nPos, "%02X", szSrc[i]&0xff);
			}
			
			nPos += nRet;
		}
		//szDest[nPos-1] = '\0';
		nDestLen = nPos;
	}
	return nDestLen;
}

__int64 CGlobal::TranMsgIdCharToI64(const unsigned char *szMsgId)
{
	if (NULL == szMsgId)
	{
		return 0;
	}

	__int64 j = 0;
	const unsigned char* p = szMsgId;
	j += (INT64)(*p) << 56;
	j += (INT64)*(p + 1) << 48;
	j += (INT64)*(p + 2) << 40;
	j += (INT64)*(p + 3) << 32;
	j += (INT64)*(p + 4) << 24;
	j += (INT64)*(p + 5) << 16;
	j += (INT64)*(p + 6) << 8;
	j += (INT64)*(p + 7);
	return j;
}

void CGlobal::TranMsgIdI64ToChar(unsigned char* MsgId, __int64 IMsgId)
{
	__int64 j = IMsgId;
	MsgId[0] = (unsigned char)((j >> 56) & 0xff);
	MsgId[1] = (unsigned char)((j >> 48) & 0xff);
	MsgId[2] = (unsigned char)((j >> 40) & 0xff);
	MsgId[3] = (unsigned char)((j >> 32) & 0xff);
	MsgId[4] = (unsigned char)((j >> 24) & 0xff);
	MsgId[5] = (unsigned char)((j >> 16) & 0xff);
	MsgId[6] = (unsigned char)((j >> 8) & 0xff);
	MsgId[7] = (unsigned char)(j & 0xff);
	return;
}

__int64 CGlobal::GetI64MsgIdAt(__int64 SrcId, int PkNum)
{	
	__int64 j = (__int64)PkNum;	
	return ((j << 34) | SrcId);
	//return ( j * 17179869184 + SrcId);
}

int CGlobal::GeneranteSeqID()
{
	static long s_iSeqId = 0;

	return CAdapter::InterlockedIncrement(&s_iSeqId);
}

int CGlobal::GetMsgId(int Num, bool &bFlow)
{
	static int g_iMsgId = 0;
	int iFirstId = 0;
	try
	{
		SafeLock safelock(&cs_MsgId);
		//cs_MsgId.Lock();
		iFirstId = g_iMsgId + 1;
		g_iMsgId += Num;
		//32位int
		//unsigned int 4294967295
		//int	2147483647
		if (g_iMsgId > 1000000)
		{
			g_iMsgId = 0;
			iFirstId = 0;
			g_iMsgId += Num;
			bFlow = TRUE;
		}
		//cs_MsgId.Unlock();
		return iFirstId;
	}
	catch (...)
	{
		//cs_MsgId.Unlock();
	}
	return 0;
}



int CGlobal::GetMsgId(int Num)
{
	static int g_iMsgId = 0;
	//溢出时间
	//zhubo 2017-4-7
	static SYSTEMTIME_EX_LINUX g_tLastFlowTm;
	//static SYSTEMTIME g_tLastFlowTm;
	//本秒的起始值
	static int g_nFirstValueOfCurrentSecond = 1;
    //zhubo 2017-4-7
    static SYSTEMTIME_EX_LINUX g_tSecondSwitchTm;
	//static SYSTEMTIME g_tSecondSwitchTm;

	//本次需要返回的ID
	int nThisNeedReturnId = 0;

	{
		SafeLock safelock(&cs_MsgId);

        //zhubo 2017-4-7
		//SYSTEMTIME tNow;
		//GetLocalTime(&tNow);
		
		SYSTEMTIME_EX syst;
        CAdapter::GetLocalTime(&syst);
        SYSTEMTIME_EX_LINUX tNow(syst);


		nThisNeedReturnId = g_iMsgId + 1;
		g_iMsgId += Num;
		//32位int
		//unsigned int 4294967295
		//int   2147483647
		if (g_iMsgId > 1000000)
		{
			g_iMsgId = 0;
			nThisNeedReturnId = g_iMsgId + 1;
			g_iMsgId += Num;

			//记录溢出的时间
			g_tLastFlowTm = tNow;
		}
		
		//计算本秒的起始值
		if (tNow.wMonth != g_tSecondSwitchTm.wMonth
			|| tNow.wDay != g_tSecondSwitchTm.wDay
			|| tNow.wHour != g_tSecondSwitchTm.wHour
			|| tNow.wMinute != g_tSecondSwitchTm.wMinute
			|| tNow.wSecond != g_tSecondSwitchTm.wSecond)
		{
			g_nFirstValueOfCurrentSecond = nThisNeedReturnId;
			g_tSecondSwitchTm = tNow;
		}

		//拿当前时间与溢出时的时间比较,看是否处于同一秒,若处于同一秒就延时
		if (tNow.wMonth == g_tLastFlowTm.wMonth
			&& tNow.wDay == g_tLastFlowTm.wDay
			&& tNow.wHour == g_tLastFlowTm.wHour
			&& tNow.wMinute == g_tLastFlowTm.wMinute
			&& tNow.wSecond == g_tLastFlowTm.wSecond
			&& nThisNeedReturnId >= g_nFirstValueOfCurrentSecond)
		{
			int nDelay = 1000 - tNow.wMilliseconds;
			nDelay<0 ? nDelay = 0 : 1;
			nDelay>1000 ? nDelay = 1000 : 1;
			CAdapter::Sleep(nDelay);

			char szBuf[100] = { 0 };
			sprintf(szBuf, "%d-%d-%d", nDelay, nThisNeedReturnId, g_nFirstValueOfCurrentSecond);
			//CLogMgr::GetInstance().WriteLog(szBuf, "MsgIdOverFlow.txt");
		}
	}
	return nThisNeedReturnId;
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
	unsigned int nDay	= 0;
	unsigned int nHour	= 0;
	unsigned int nMin	= 0;
	unsigned int nSec	= 0;
	unsigned int nGate	= 0;
	unsigned int nNo	= 0;
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
	j |= (__int64)(nMonth & 0x0f)	  << 60;
	j |= (__int64)(nDay   & 0x1f)	  << 55;			
	j |= (__int64)(nHour  & 0x1f)	  << 50;		
	j |= (__int64)(nMin   & 0x3f)	  << 44;		
	j |= (__int64)(nSec   & 0x3f)	  << 38;
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


void CGlobal::MakeMsgId(unsigned char *MsgId, unsigned int Num, unsigned int Gate)
{
	if (NULL == MsgId)
	{
		return;
	}

	unsigned int nMonth = 0;
	unsigned int nDay = 0;
	unsigned int nHour = 0;
	unsigned int nMin = 0;
	unsigned int nSec = 0;
	unsigned int nGate = Gate;
	unsigned int nNo = GetMsgId(Num);

	//zhubo 2017-4-7
	//SYSTEMTIME t;
	//GetLocalTime(&t);
    //zhubo 2017-4-7
        
    SYSTEMTIME_EX tt;
    CAdapter::GetLocalTime(&tt);
    SYSTEMTIME_EX_LINUX t(tt);
    
	nMonth = t.wMonth;
	nDay = t.wDay;
	nHour = t.wHour;
	nMin = t.wMinute;
	nSec = t.wSecond;

	INT64 j = 0;
	j |= (INT64)(nMonth & 0x0f) << 60;
	j |= (INT64)(nDay & 0x1f) << 55;
	j |= (INT64)(nHour & 0x1f) << 50;
	j |= (INT64)(nMin & 0x3f) << 44;
	j |= (INT64)(nSec & 0x3f) << 38;
	j |= (INT64)(nGate & 0x03ffff) << 20;
	//j |= (INT64)(nGate  & 0x03fff)   << 20;14~18位用于长短信序号---2016-06-06,暂不调整,只需上层控制网关编号范围在0~16383即可
	j |= (INT64)(nNo & 0x0fffff);

	MsgId[0] = (unsigned char)((j >> 56) & 0xff);
	MsgId[1] = (unsigned char)((j >> 48) & 0xff);
	MsgId[2] = (unsigned char)((j >> 40) & 0xff);
	MsgId[3] = (unsigned char)((j >> 32) & 0xff);
	MsgId[4] = (unsigned char)((j >> 24) & 0xff);
	MsgId[5] = (unsigned char)((j >> 16) & 0xff);
	MsgId[6] = (unsigned char)((j >> 8) & 0xff);
	MsgId[7] = (unsigned char)((j)& 0xff);

	return;
}


void    CGlobal::MakeMsgId_qc(unsigned char *MsgId, unsigned int Num, unsigned int Gate)
{
    static time_t tLastMake;
	unsigned int nMonth = 0;
	unsigned int nDay = 0;
	unsigned int nHour = 0;
	unsigned int nMin = 0;
	unsigned int nSec = 0;
	unsigned int nGate = Gate;
	bool		bFlow = FALSE;
	unsigned int nNo = GetMsgId(Num, bFlow);
	if (NULL == MsgId)
	{
		return;
	}
	time_t tNow;
	time(&tNow);
	if (tNow == tLastMake && bFlow)
	{
		CAdapter::Sleep(1000);
	}

	time(&tLastMake);

    SYSTEMTIME_EX tt;
    CAdapter::GetLocalTime(&tt);
    SYSTEMTIME_EX_LINUX t(tt);
    
	//SYSTEMTIME t;
	//GetLocalTime(&t);
	nMonth = t.wMonth;
	nDay = t.wDay;
	nHour = t.wHour;
	nMin = t.wMinute;
	nSec = t.wSecond;

	INT64 j = 0;
	j |= (__int64)(nMonth & 0x0f) << 60;
	j |= (__int64)(nDay & 0x1f) << 55;
	j |= (__int64)(nHour & 0x1f) << 50;
	j |= (__int64)(nMin & 0x3f) << 44;
	j |= (__int64)(nSec & 0x3f) << 38;
	j |= (__int64)(nGate & 0x03ffff) << 20;
	j |= (__int64)(nNo & 0x0fffff);
	//j |= (__int64)(nGate & 0x3fffff)<< 16;
	//j |= (__int64)(nNo & 0x0ffff);

	MsgId[0] = (unsigned char)((j >> 56) & 0xff);
	MsgId[1] = (unsigned char)((j >> 48) & 0xff);
	MsgId[2] = (unsigned char)((j >> 40) & 0xff);
	MsgId[3] = (unsigned char)((j >> 32) & 0xff);
	MsgId[4] = (unsigned char)((j >> 24) & 0xff);
	MsgId[5] = (unsigned char)((j >> 16) & 0xff);
	MsgId[6] = (unsigned char)((j >> 8) & 0xff);
	MsgId[7] = (unsigned char)((j)& 0xff);
	return;
}


int CGlobal::TranMsgId(unsigned char *pSrc, char *MsgId)
{
	unsigned int nMon	= 0;
	unsigned int nDay	= 0;
	unsigned int nHour	= 0;
	unsigned int nMin	= 0;
	unsigned int nSec	= 0;
	unsigned int nGate	= 0;
	unsigned int nNo	= 0;
	unsigned char *p	= NULL;
	
	//先转为64bit整形再移位
	__int64 j = 0;
	p = pSrc;
	j |= (__int64)(*(p))	<< 56;
	j |= (__int64)(*(++p))	<< 48;
	j |= (__int64)(*(++p))	<< 40;
	j |= (__int64)(*(++p))	<< 32;
	j |= (__int64)(*(++p))	<< 24;
	j |= (__int64)(*(++p))	<< 16;
	j |= (__int64)(*(++p))	<< 8;
	j |= (__int64)(*(++p));
	
	nMon  = (unsigned int)(j >> 60) & 0x0f;            //4  bit
	nDay  = (unsigned int)(j >> 55) & 0x1f;           //5  bit
	nHour = (unsigned int)(j >> 50) & 0x1f;           //5  bit
	nMin  = (unsigned int)(j >> 44) & 0x3f;           //6  bit
	nSec  = (unsigned int)(j >> 38) & 0x3f;           //6  bit
	nGate = (unsigned int)(j >> 20) & 0x03ffff;       //18 bit,最大值9999
	nNo   = (unsigned int)(j)		& 0x0fffff;       //20 bit,最大值999999
	sprintf(MsgId, "%02d%02d%02d%02d%02d%06d%06d", nMon, nDay, nHour, nMin, nSec, nGate, nNo);
	return 0;
}

//zhubo 2017-4-7
/*
time_t systime_to_timet(const SYSTEMTIME& st)
{
    struct tm gm = {st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth-1, st.wYear-1900, st.wDayOfWeek, 0, 0};
	
    return mktime(&gm);
}
*/



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

    
    //zhubo 2017-4-7
    //SYSTEMTIME t;
    //GetLocalTime(&t);
    //zhubo 2017-4-7
    
    SYSTEMTIME_EX tt;
    CAdapter::GetLocalTime(&tt);
    SYSTEMTIME_EX_LINUX t(tt);
    //COleDateTime dt(t.wYear, nMon, nDay, nHour, nMin, nSec);
	struct tm gm = {nSec, nMin, nHour, nDay, nMon-1, t.wYear-1900, t.wDayOfWeek, 0, 0};

    return mktime(&gm);
}

time_t CGlobal::string2time(const char * str,const char * formatStr)
{
	if(strlen(str) <= 0)
		return 0;

	try
	{
		struct tm tm1;  
		sscanf(str, formatStr,      
		&tm1.tm_year,   
		&tm1.tm_mon,   
		&tm1.tm_mday,   
		&tm1.tm_hour,   
		&tm1.tm_min,  
		&tm1.tm_sec);  

		tm1.tm_year -= 1900;  
		tm1.tm_mon --;  

		tm1.tm_isdst=-1;  
		return mktime(&tm1);
	}
	catch(...)
	{
		return 0;
	}
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
		int nNum			= 0;
		int i				= 0;
		char szBuf[64+1]	= {0};
		
		for (i = 0; i < 64; ++i)
		{
			szBuf[i] = '0';
		}

		unsigned char szData[8+1] = {0};
		char szTmp[8+1] = {0};
		int nTmp = 0;

		unsigned int nMonth = 0;
		unsigned int nDay	= 0;
		unsigned int nHour	= 0;
		unsigned int nMin	= 0;
		unsigned int nSec	= 0;
		unsigned int nGate	= nUId;

		//zhubo 2017-4-7
		//SYSTEMTIME t;
		//GetLocalTime(&t);
        //zhubo 2017-4-7
        
        SYSTEMTIME_EX tt;
        CAdapter::GetLocalTime(&tt);
        SYSTEMTIME_EX_LINUX t(tt);

        
		nMonth	= t.wMonth;
		nDay	= t.wDay;
		nHour	= t.wHour;
		nMin	= t.wMinute;
		nSec	= t.wSecond;
		
		//要根据短信条数设置步长
		//nNum = GetMsgId(1);

		
		//将所有数据转化成二进制，并生成二制字符串
		std::string str;
		int nPos = 0;
		int nLen = 0;
		CAdapter::itoa(nMonth, szBuf+nPos, 2);
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
		
		CAdapter::itoa(nDay, szBuf+nPos, 2);
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
		
		CAdapter::itoa(nHour, szBuf+nPos, 2);
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
		
		CAdapter::itoa(nMin, szBuf+nPos, 2);
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
		
		CAdapter::itoa(nSec, szBuf+nPos, 2);
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
		
		
		CAdapter::itoa(nGate, szBuf+nPos, 2);
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
		
		CAdapter::itoa(nNum, szBuf + nPos, 2);
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


int CGlobal::CalcMsgNum(int nMsgLen, int nSplitLen, const char *szMsg, BOOL bAsciiToUS2)
{
	int nTotalSize = nMsgLen;
	int nTotalNum = 1;
	unsigned char szBuf[3096] = {0};
	if (bAsciiToUS2)
	{
    	//zhubo 2017-4-7
		//AsciiToUS2(szMsg, szBuf, (unsigned int*)&nTotalSize);
		CAdapter::AsciiToUS2(szMsg, szBuf, (unsigned int*)&nTotalSize);
	}
	
	if (nTotalSize <= nSplitLen * 2)
	{
		nTotalNum = 1;
	}
	else
	{
		nTotalNum = 1 + (nTotalSize - 57 * 2 + 67 * 2 - 1) / (67 * 2);
	}
	
	return nTotalNum;
}

//计算短信条数
int CGlobal::CalcMsgNum(int nMsgFmt , int nMsgLen, int nSplitLen, int nMultilen1, int nMultilen2, const char *szMsg, BOOL bAsciiToUS2)
{
	int nTotalSize = nMsgLen;
	int nTotalNum = 1;
	unsigned char szBuf[3096] = {0};
	if (bAsciiToUS2)
	{
	    //zhubo 2017-4-7
		//AsciiToUS2(szMsg, szBuf, (unsigned int*)&nTotalSize);
		CAdapter::AsciiToUS2(szMsg, szBuf, (unsigned int*)&nTotalSize);
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


bool CGlobal::IsSpecialAscii(char cAscii)
{
	//12个特殊字符,按2个字节计算
	if (0x7B == cAscii || 0x7C == cAscii || 0x7D == cAscii || 0x7E == cAscii //{|}~
		|| 0x5B == cAscii || 0x5C == cAscii || 0x5D == cAscii || 0x5E == cAscii//[\]^
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//计算纯英文ASCII短信长度(按gsm 7-bit编码中对符串字节数的要求进行计算)
int CGlobal::CalcEnMsgLen_Global(const char* szEnMsg)
{
	int i = 0;
	int nLen = 0;
	while (szEnMsg[i] != '\0')
	{
		//12个特殊字符,按2个字节计算
		if (IsSpecialAscii(szEnMsg[i]))
		{
			nLen += 2;
		}
		else
		{
			++nLen;
		}
		++i;
	}
	return nLen;
}


//计算纯英文ASCII短信条数
int CGlobal::CalcEnMsgNum_Global(const char* szEnMsg/*ASCII串短信(含签名)*/, int nSingleLen/*单条长度(含签名)*/, int nMultilen1/*分条1长度(含签名(若有))*/, int nMultilen2/*分条2长度(含签名(若有)*/)
{
	int nTotalNum = 1;
	int nTotalSize = CalcEnMsgLen_Global(szEnMsg);
	//单条
	if (nTotalSize <= nSingleLen)
	{
		nTotalNum = 1;
	}
	//多条
	else
	{
		nTotalNum = 0;
		int nMsgLen = strlen(szEnMsg);
		int i = 0;
		int nGsmLen = 0;
		while (i < nMsgLen)
		{
			if (IsSpecialAscii(szEnMsg[i]))
			{
				nGsmLen += 2;
				if (nGsmLen > nMultilen1)
				{
					nTotalNum += 1;
					nGsmLen = 2;
				}
				else if (nGsmLen == nMultilen1)
				{
					nTotalNum += 1;
					nGsmLen = 0;
				}
			}
			else
			{
				++nGsmLen;
				if (nGsmLen >= nMultilen1)
				{
					nTotalNum += 1;
					nGsmLen = 0;
				}
			}
			++i;
		}
		//不足一条
		if (nGsmLen > 0)
		{
			//比分条1小,说明while里面最后一条没计算在内,这里要加1
			if (nGsmLen < nMultilen1)
			{
				nTotalNum += 1;
			}
			//如果最后一条长度比分条2大,按分条2长度计算最后一条应该是几条
			if (nGsmLen > nMultilen2)
			{
				//整除
				if (0 == nGsmLen%nMultilen2)
				{
					nTotalNum += (nGsmLen/nMultilen2-1);
				}
				//有余数
				else
				{
					nTotalNum += nGsmLen/nMultilen2;
				}	
			}
		}
		//足一条
		else
		{
			//如果最后一条长度比分条2大,按分条2长度计算最后一条应该是几条
			if (nMultilen1 > nMultilen2)
			{
				//整除
				if (0 == nMultilen1%nMultilen2)
				{
					nTotalNum += (nMultilen1/nMultilen2-1);
				}
				//有余数
				else
				{
					nTotalNum += nMultilen1/nMultilen2;
				}
			}
		}
	}
	
	return nTotalNum;
}




/*
szInMsg		本次要拆的短信内容
szOutMsg	输出拆分后该条的内容(已解码)
nMultilen1	
nMultilen2
nPkNum		要第几条
nPkTotal	总共几条
nPreMsgLen	上次返回的长度
nMsgLen		本次输入内容的长度(若内容中带有签名，需减去签名的长度=nMsgLen-nSignLen)
bUS2ToAscii 是否要解码(仅对编码方式是8的),无需编码时,输入字符需为宽字符,长度需宽字符串长度
*/
int CGlobal::SplitMsgEx(const unsigned char *szInMsg, char *szOutMsg, int nMultilen1, int nMultilen2,
			int nPkNum, int nPkTotal, int nPreMsgLen, int nMsgLen, BOOL bUS2ToAscii, int nSignLen)
{
	int nSplitLen	= nMsgLen;
	int nLen		= nMsgLen;
	int nTmp		= nPreMsgLen;
	char szBuf[4096] = {0};
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
    		//zhubo 2017-4-7
			//US2ToAscii(szBuf, nSplitLen, szOutMsg, &nLen);
			CAdapter::US2ToAscii(szBuf, nSplitLen, szOutMsg, &nLen);
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
		    //zhubo 2017-4-7
			//US2ToAscii(szBuf, nSplitLen, szOutMsg, &nLen);
			CAdapter::US2ToAscii(szBuf, nSplitLen, szOutMsg, &nLen);
		}
		else
		{
			memcpy(szOutMsg, szBuf, nSplitLen);
		}
	}
	return nSplitLen;
}

//拆分短信
/*

int CGlobal::SplitMsg(CString Msg, int nSplitLen, char *Msg1, char *Msg2)
{
	CString csMsg;
	if (Msg.GetLength() <= 0)
	{
		return -1;
	}
	else if (Msg.GetLength() > 720)
	{
		//2014-10-27 增加截取功能
		csMsg=Msg.Left(720);
		//return -2;
	}else
	{
		csMsg = Msg;
	}
	
	CString strTemp("");
	wchar_t temp[721]	= {0}; //70*6
	wchar_t aa[721]		= {0};
	memset(Msg1, '\0', sizeof(Msg1));
	memset(Msg2, '\0', sizeof(Msg2));
	try
	{
	
		CAdapter::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, csMsg, strlen(csMsg), temp, sizeof(temp));
		int len = wcslen(temp);
		if (len > 0 && len <= nSplitLen)
		{//只有一条信息
			wcsncpy(aa, temp, nSplitLen);
			strTemp = (CString)aa;
			strcpy(Msg1, strTemp);
			return 0;
		}
		else if (len > nSplitLen)//超过一条信息
		{
			wcsncpy(aa, temp, nSplitLen);
			strTemp = (CString)aa;
			strcpy(Msg1, strTemp);
			strTemp = "";
			strTemp = (CString)(temp + nSplitLen);
			strcpy(Msg2, strTemp);
			return 1;
		}
	}
	catch(...)
	{
	}
	return -9;
}
*/


//拆分短信
int CGlobal::SplitMsg(const char* Msg, int nSplitLen, char *Msg1, char *Msg2)
{
    /*
    int nInMsgLen = strlen(Msg);
    if (nInMsgLen <= 0)
    {
        return -1;
    }
    else if (nInMsgLen > 720)
    {
        return -2;
    }

    wchar_t temp[721]   = {0}; //70*6
    wchar_t aa[721]     = {0};
    memset(Msg1, '\0', sizeof(Msg1));
    memset(Msg2, '\0', sizeof(Msg2));
    try
    {
        CAdapter::MultiByteToWideChar((UINT)CP_ACP, (DWORD)MB_PRECOMPOSED, (LPCSTR)Msg, strlen(Msg), (LPWSTR)temp, sizeof(temp));
        int len = wcslen(temp);
        if (len > 0 && len <= nSplitLen)
        {//只有一条信息
            wcsncpy(aa, temp, nSplitLen);
			CAdapter::WideCharToMultiByte((UINT)CP_ACP, (DWORD)WC_COMPOSITECHECK, (LPCWSTR)aa, -1, (LPSTR)Msg1, nSplitLen*2, (LPCSTR)NULL, (LPBOOL)NULL);
            return 0;
        }
        else if (len > nSplitLen)//超过一条信息
        {
            wcsncpy(aa, temp, nSplitLen);
			CAdapter::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, aa, -1, Msg1, nSplitLen*2, NULL, NULL);
			CAdapter::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, temp+nSplitLen,-1, Msg2, wcslen(temp+nSplitLen)*2, NULL, NULL);
            return 1;
        }
    }
    catch(...)
    {
    }
    return -9;
    */
    
  	string csMsg(Msg);
	if (csMsg.length() <= 0)
	{
		return -1;
	}
	else if (csMsg.length() > MAX_MSG_LEN*2)
	{
		//2014-10-27 增加截取功能
		//csMsg = Msg.Left(MAX_MSG_LEN * 2);
		csMsg = cstr::left(csMsg,MAX_MSG_LEN * 2);
		//return -2;
	}
    
	//CString strTemp("");
	//wchar_t temp[MAX_MSG_LEN * 2+1] = { 0 }; //70*6
	//wchar_t aa[MAX_MSG_LEN * 2+1] = { 0 };
	
	//char   szOut[MAX_MSG_LEN * 2+1] = {0};
  //  string strOut;
	memset(Msg1, '\0', sizeof(Msg1));
	memset(Msg2, '\0', sizeof(Msg2));
	try
	{
	    //iconvpp::converter conv(CSET_GBK, CSET_UTF8);
        //conv.convert(csMsg,strOut);
        
	    //std::vector <wchar_t> vWCH(MAX_MSG_LEN * 2+1, '\0'); 
    	//CAdapter::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)csMsg.c_str(), -1, (LPWSTR)&vWCH[0], MAX_MSG_LEN * 2); 
		//MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, csMsg, strlen(csMsg), temp, sizeof(temp));
		//int len = wcslen(temp);
		//int len = strlen(szOut);
		int len = csMsg.length();
		if (len > 0 && len <= nSplitLen)
		{//只有一条信息
			//wcsncpy(aa, temp, nSplitLen);
			//strTemp = (CString)aa;
			strcpy(Msg1, csMsg.c_str());
			return 0;
		}
		else if (len > nSplitLen)//超过一条信息
		{
		    
		    memcpy(Msg1,csMsg.c_str(),nSplitLen);
            memcpy(Msg2,csMsg.c_str()+nSplitLen,len-nSplitLen);
			//wcsncpy(aa, temp, nSplitLen);
			//strTemp = (CString)aa;
			//strcpy(Msg1, strTemp);
			//strTemp = "";
			//strTemp = (CString)(temp + nSplitLen);
			//strcpy(Msg2, strTemp);
			return 1;
		}
	}
	catch(...)
	{
	}
	return -9;
}


UINT TrimRight(char *szSrc, UINT nLen)
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

UINT TrimUnicodeRight(char *szSrc, UINT nLen)
{
	if (NULL != szSrc && nLen > 0)
	{
		while (nLen > 1)
		{
			//去空格、回车、换行、制表符 
			//2011-12-06 要求不去除回车和回车换行
			if (szSrc[nLen - 2] == 0x00
				&& (szSrc[nLen - 1] == 0x20 || szSrc[nLen - 1] == 0x09))
			{
				szSrc[nLen - 1] = '\0';
				szSrc[nLen - 2] = '\0';
				nLen -= 2;
			}
			else
			{
				break;
			}
		}
	}
	return nLen;
}

//验证短信内容的合法性
int CGlobal::CheckMsgValidity(int nMsgFmt, int nMaxLen, USHORT& nMsgLen, /*const */char *szMsg, bool bMo)
{
	int nRet = 0;
	wchar_t wcMsg[4 * MAX_MSG_LEN] = { 0 };
	if (8 == nMsgFmt)
	{
		if (nMsgLen > nMaxLen*2)
		{
			nRet = 6;
		}
		else
		{
			nMsgLen = TrimUnicodeRight(szMsg, nMsgLen);
		}
	}
	else if (15 == nMsgFmt)
	{
		nMsgLen = CAdapter::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)szMsg, nMsgLen, (LPWSTR)wcMsg, sizeof(wcMsg));
		
		if (nMsgLen > nMaxLen)
		{
			nRet = 6;
		}
		else
		{
			nMsgLen = CAdapter::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wcMsg, nMsgLen, szMsg, MAX_MSG_LEN*2, NULL, NULL);	
			nMsgLen = TrimRight(szMsg, nMsgLen);
		}
	}
	else if (0 == nMsgFmt)
	{
		if(nMsgLen > nMaxLen)
		{
			nRet = 6;
		}
	}
	else if ((4 == nMsgFmt || 248 == nMsgFmt || 246 == nMsgFmt) && !bMo)
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
		nMsgLen = CAdapter::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)szMsg, nMsgLen, (LPWSTR)wcMsg, sizeof(wcMsg));
		
		if (nMsgLen > nMaxLen)
		{
			nRet = 6;
		}
		else
		{
			nMsgLen = CAdapter::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wcMsg, nMsgLen, szMsg, MAX_MSG_LEN*2, NULL, NULL);	
			nMsgLen = TrimRight(szMsg, nMsgLen);
		}
	}
	
	return nRet;

}


int CGlobal::TrimUnicodeStrRight(string& strUnicodeStr)
{       
    int nLen = strUnicodeStr.size();
    while (nLen > 1)
    {
        //去空格、回车、换行、制表符 
        //2011-12-06 要求不去除回车和回车换行
        if (strUnicodeStr[nLen - 2] == 0x00
            && (strUnicodeStr[nLen - 1] == 0x20 || strUnicodeStr[nLen - 1] == 0x09))
        {
            strUnicodeStr.erase(nLen - 2,2);
            //szSrc[nLen - 1] = '\0';
            //szSrc[nLen - 2] = '\0';
            nLen -= 2;
        }
        else
        {
            break;
        }
    }
    return nLen;
}


int  CGlobal::CheckMsgValidity(int nMsgFmt, int nMaxLen, string& strContent)
{       

    int nRet = 0;
    int nMsgLen = strContent.size();
    
	if (8 == nMsgFmt)
	{
		if (nMsgLen > nMaxLen*2)
		{
			nRet = 6;
		}
		else
		{
			//nMsgLen = TrimUnicodeRight(szMsg, nMsgLen);
		}
	}
    
	else if (0 == nMsgFmt)
	{
		if(nMsgLen > nMaxLen)
		{
			nRet = 6;
		}
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
        //zhubo 2017-4-7
		//US2ToAscii(szMsg, nLen, szBuf, &OutLen);
		CAdapter::US2ToAscii(szMsg, nLen, szBuf, &OutLen);
		OutLen  = CAdapter::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)szBuf, OutLen, (LPWSTR)wcMsg, sizeof(wcMsg));
		nMsgLen = OutLen;
	}
	else
	{
		nMsgLen = CAdapter::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)szMsg, nMsgLen, (LPWSTR)wcMsg, sizeof(wcMsg));
	}
	
	return nMsgLen;
}

BOOL InitSocket()
{
	WORD wVersionRequired;
	WSADATA wsaData;
	wVersionRequired = MAKEWORD(2, 2);
	
	int err = CAdapter::WSAStartup(wVersionRequired, &wsaData);
	if (0 != err)
	{
		return FALSE;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wHighVersion) != 2)
	{
		//not support
		CAdapter::WSACleanup();
		return FALSE;
	}
	return TRUE;
}

BOOL UnInitSocket()
{
	CAdapter::WSACleanup();
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
	const char *pStart = /*StrStrI*/strstri(szUrl, "http://");
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



//由URL地址取得IP地址和端口
int  CGlobal::GetIpFromHttps(const char *szUrl, char *szWebPath, char *szIp, char* szDomain, int& nPort)
{


    int nRet = -1;
    if (NULL == szUrl || '\0' == szUrl[0] || NULL == szIp || NULL == szWebPath || NULL == szDomain)
    {
        return nRet;
    }
    //https://192.169.3.141:447/sms/mt 	//"https://192.169.3.141:447"
    const char *pStart = /*StrStrI*/strstri(szUrl, "https://");
    if (NULL != pStart)
    {
        pStart += strlen("https://");
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
            memcpy(szIp, pStart, p2 - pStart);
            nPort = atoi(p2 + 1);
        }
        else
        {
            if (p1 != NULL)
            {
                memcpy(szIp, pStart, p1 - pStart);
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

int CGlobal::GetHostFromUrl(char* szUrl,char* pHost,int nMaxHostLen)
{
    int nRet = -1;
    if (NULL == szUrl || '\0' == szUrl[0])
    {
        return nRet;
    }
    char * pStart = szUrl;

    if (0 == strncasecmp(szUrl, "https", sizeof("https")-1))
    {
        //https
        pStart += strlen("https://");
        char *p1 = strstr(pStart, "/");
        if (NULL != p1)
        {
            memcpy(pHost,pStart,min((int)(p1 - pStart),nMaxHostLen));
            nRet = 0;
        }
    }
    else if(0 == strncasecmp(szUrl, "http", sizeof("http")-1))
    {
        //http
        pStart += strlen("http://");
        char *p1 = strstr(pStart, "/");
        if (NULL != p1)
        {
            memcpy(pHost,pStart,min((int)(p1 - pStart),nMaxHostLen));
            nRet = 0;
        }
    }
    return nRet;
}

//qiwei 20150409 add
void CGlobal::DeleteSpecialChar(char * szSrc , int nSrclen)
{
	if (szSrc == NULL)
	{
		return;
	}
	int npos = 0 ;
	bool bIsForeign = false; 
	char * szTemp = NULL;
	szTemp = szSrc;
	while (*szTemp != '\0')
	{
		if (*szTemp != '+')
		{
			if ( *szTemp < '0'|| *szTemp > '9')
			{
				szTemp++;
				npos++;
			}
			else
			{
				break;
			}
		}
		else
		{
			//算是国外号码
			bIsForeign = true;
			break;
		}
	}
	
	if(bIsForeign)
	{
		char szCode[SIGN_MAX_PHONE_LEN+1]={0};
		if ( npos > 0 )
		{
			strncpy(szCode,szTemp,sizeof(szCode)-1);
			int nCopyLen = strlen(szCode);
			memcpy(szSrc,szCode,nCopyLen);
			szSrc[nCopyLen]='\0';
		}
	}
	else
	{
		
		//判断是否是国外号码
		char szCode[SIGN_MAX_PHONE_LEN+1]={0};
		if ( npos > 0 )
		{
			strncpy(szCode,szTemp,sizeof(szCode)-1);
		}
		else
		{
			strncpy(szCode,szTemp,sizeof(szCode)-1);
		}
		int len = 0 ;
		//去0
		if (szCode[len] == '0' && szCode[len+1] == '1' && szCode[len+2] > '0' && szCode[len+3] <= '9')
		{
			++len;
		}
		//去86
		if (szCode[len] == '8' && szCode[len+1] == '6')
		{
			len += 2;
		}
		int nCpyLen = strlen(szCode+len) ;
		memcpy(szSrc,szCode+len,nCpyLen);
		szSrc[nCpyLen] = '\0';
		
	}

	//去掉手机号码右边的空格2018/10/17
	int nLen = strlen(szSrc);
	if(nLen > 0 && ' ' == szSrc[nLen -1])
	{
		szSrc[nLen -1] = '\0';
	}
}

	
//拆分以逗号分隔的字符串
int CGlobal::SplitStr(const char *szInStr, std::vector<std::string>& vOutStr, int nMaxLen)
{
	int nCount = 0;
	if (NULL != szInStr && szInStr[0] != '\0')
	{
		int nLen		= strlen(szInStr);
		char *pSegStr	= new char[nMaxLen+1];
		if (pSegStr)
		{
			int nCpyLen = 0 ;
			try
			{
				const char *pStart	= (char *)szInStr;
				const char *pEnd		= strstr(szInStr, ",");
				if (NULL == pEnd)
				{
					memset(pSegStr, 0, nMaxLen+1);
					nCpyLen = min(nLen, nMaxLen);
					memcpy(pSegStr, szInStr, nCpyLen);
					//去掉手机号前面的非数字及其数字0
					DeleteSpecialChar(pSegStr ,nCpyLen );
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
						nCpyLen = min(pEnd-pStart, nMaxLen);
						memcpy(pSegStr, szInStr+nPos,nCpyLen );
						DeleteSpecialChar(pSegStr ,nCpyLen);
						vOutStr.push_back(pSegStr);
						pStart = pEnd+1;
						pEnd = strstr(pStart, ",");
						if (pEnd == NULL)
						{
							nPos = pStart-szInStr;
							memset(pSegStr, 0, strlen(pSegStr));
							nCpyLen = min(nLen-nPos, nMaxLen);
							memcpy(pSegStr, szInStr+nPos,nCpyLen );
							DeleteSpecialChar(pSegStr ,nCpyLen);
							vOutStr.push_back(pSegStr);
						}
						++nCount;
					}
				}
			}
			catch (...)
			{
				if (pSegStr)
				{
					delete []pSegStr;
					pSegStr = NULL;
				}
			}
		}
		if (NULL != pSegStr)
		{
			delete []pSegStr;
			pSegStr = NULL;
		}
	}
	return nCount;
}

int CGlobal::SplitStrEx(const char *szInStr, std::vector<std::string> &vOutStr, const char* c, int nMaxLen)
{
	int nCount = 0;
	if (NULL != szInStr && szInStr[0] != '\0')
	{
		int nLen		= strlen(szInStr);
		char *pSegStr	= new char[nMaxLen+1];
		if (pSegStr)
		{
			int nCpyLen = 0 ;
			try
			{
				const char *pStart	= (char *)szInStr;
				const char *pEnd		= strstr(szInStr, c);
				if (NULL == pEnd)
				{
					memset(pSegStr, 0, nMaxLen+1);
					nCpyLen = min(nLen, nMaxLen);
					memcpy(pSegStr, szInStr, nCpyLen);
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
						nCpyLen = min(pEnd-pStart, nMaxLen);
						memcpy(pSegStr, szInStr+nPos,nCpyLen );
						vOutStr.push_back(pSegStr);
						pStart = pEnd+1;
						pEnd = strstr(pStart, c);
						if (pEnd == NULL)
						{
							nPos = pStart-szInStr;
							memset(pSegStr, 0, strlen(pSegStr));
							nCpyLen = min(nLen-nPos, nMaxLen);
							memcpy(pSegStr, szInStr+nPos,nCpyLen );
							vOutStr.push_back(pSegStr);
						}
						++nCount;
					}
				}
			}
			catch (...)
			{
				if (pSegStr)
				{
					delete []pSegStr;
					pSegStr = NULL;
				}
			}
		}
		if (NULL != pSegStr)
		{
			delete []pSegStr;
			pSegStr = NULL;
		}
	}
	return nCount;

}

int CGlobal::SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));

	return 0;
}

//转换UTF-8特殊字符
int CGlobal::TranHttpSpace(char *buf )
{
	int i = 0;
	int j = 0;
	
		//int nLen = strlen(buf);
	while (buf[i] != '\0')
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

	return j;
}

//base64加密
int CGlobal::Base64Encode(LPBYTE pbSrc, DWORD SrcLen, LPBYTE pbDes) 
{		
    /*
	LPBYTE pbOut = pbDes;	
	DWORD dwSrc;
	DWORD iSrc = 0;
	if( SrcLen<=0 ) return 0;
	
	for( iSrc;iSrc<SrcLen;iSrc+=3 ){
		
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
	*/
    
    static BYTE   basis[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	
    DWORD  len = SrcLen;
    LPBYTE s =   pbSrc;
    LPBYTE d =  pbDes;
    bool   padding = 1;

    while (len > 2) {
        *d++ = basis[(s[0] >> 2) & 0x3f];
        *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
        *d++ = basis[((s[1] & 0x0f) << 2) | (s[2] >> 6)];
        *d++ = basis[s[2] & 0x3f];

        s += 3;
        len -= 3;
    }

    if (len) {
        *d++ = basis[(s[0] >> 2) & 0x3f];

        if (len == 1) {
            *d++ = basis[(s[0] & 3) << 4];
            if (padding) {
                *d++ = '=';
            }

        } else {
            *d++ = basis[((s[0] & 3) << 4) | (s[1] >> 4)];
            *d++ = basis[(s[1] & 0x0f) << 2];
        }

        if (padding) {
            *d++ = '=';
        }
    }

    //dst->len = d - dst->data;
    return (int)(d - pbDes);
}

//base64解密
int CGlobal::Base64Decode(LPBYTE pbSrc, DWORD SrcLen, LPBYTE pbDes) 
{
    /*
	unsigned char base64_decode_map[256] = {
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
	LPBYTE pbOut = pbDes;		
	DWORD b1,b2,b3,b4;	
	
	for( DWORD iSrc=0;iSrc<SrcLen;iSrc+=4 ){
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
	*/

    
    static u_char   basis[] = {
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 62, 77, 77, 77, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 77, 77, 77, 77, 77, 77,
        77,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 77, 77, 77, 77, 77,
        77, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 77, 77, 77, 77, 77,

        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77
    };

    //size_t          len;
    u_char         *d, *s;
    DWORD           len = 0;

    for (len = 0; len < SrcLen; len++) {
        if (pbSrc[len] == '=') {
            break;
        }

        if (basis[pbSrc[len]] == 77) {
            return -1;
        }
    }

    if (len % 4 == 1) {
        return -1;
    }

    s = (u_char*)pbSrc;
    d = (u_char*)pbDes;

    while (len > 3) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
        *d++ = (u_char) (basis[s[2]] << 6 | basis[s[3]]);

        s += 4;
        len -= 4;
    }

    if (len > 1) {
        *d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
    }

    if (len > 2) {
        *d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
    }

    //dst->len = d - dst->data;

    return d - pbDes;
}

int CGlobal::Base64Decode(string& strIn,string& strOut)
{
    /*
   	unsigned char base64_decode_map[256] = {
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
//	LPBYTE pbOut = pbDes;		
	DWORD b1,b2,b3,b4;	
    LPBYTE pbSrc = (LPBYTE)strIn.c_str();
    int SrcLen = strIn.size();

	
	for( DWORD iSrc=0;iSrc<SrcLen;iSrc+=4 ){
		b1 = *pbSrc++;
		b2 = *pbSrc++;
		b3 = *pbSrc++;
		b4 = *pbSrc++;		
        strOut += base64_decode_map[b1]<<2 | base64_decode_map[b2]>>4;
   		strOut += base64_decode_map[b2]<<4 | base64_decode_map[b3]>>2;		
		strOut += base64_decode_map[b3]<<6 | base64_decode_map[b4];
		
		//*pbDes++ = base64_decode_map[b1]<<2 | base64_decode_map[b2]>>4;		
		//*pbDes++ = base64_decode_map[b2]<<4 | base64_decode_map[b3]>>2;		
		//*pbDes++ = base64_decode_map[b3]<<6 | base64_decode_map[b4];
	}
	
	if( SrcLen ){		
		if( *--pbSrc=='=' ){
			//strOut.pop_back();
			if(strOut.size() > 0)
            {
                strOut = strOut.erase(strOut.size() - 1,1);
            }
			if( *--pbSrc=='=' )
            {   
                if(strOut.size() > 0)
                {
                    strOut = strOut.erase(strOut.size() - 1,1);
                }            
            }         
		}
	}

    return strOut.size();
    */

    static u_char   basis[] = {
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 62, 77, 77, 77, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 77, 77, 77, 77, 77, 77,
        77,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 77, 77, 77, 77, 77,
        77, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 77, 77, 77, 77, 77,

        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77
    };

    u_char         *s;
    DWORD           len = 0;

    
    LPBYTE pbSrc = (LPBYTE)strIn.c_str();

    for (len = 0; len < strIn.size(); len++) {
        if (pbSrc[len] == '=') {
            break;
        }

        if (basis[pbSrc[len]] == 77) {
            return -1;
        }
    }

    if (len % 4 == 1) {
        return -1;
    }

    s = (u_char*)pbSrc;
    //d = (u_char*)pbDes;

    while (len > 3) {
        //*d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        //*d++ = (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
        //*d++ = (u_char) (basis[s[2]] << 6 | basis[s[3]]);

        strOut += (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        strOut += (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
        strOut += (u_char) (basis[s[2]] << 6 | basis[s[3]]);

        s += 4;
        len -= 4;
    }

    if (len > 1) {
        //*d++ = (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        strOut += (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
    }

    if (len > 2) {
        strOut += (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
    }

    //dst->len = d - dst->data;

    return strOut.size();
	//*pbDes = 0;		
	//return (int)(pbDes-pbOut);		
}


/*
//zhubo 2017-4-7
//时间变成字符串
CString	CGlobal::DateToTimeStr(DATE dt)
{
	COleDateTime oletime(dt);
	CString s("");
	s.Format("%02d:%02d:%02d", 
			oletime.GetHour(),
			oletime.GetMinute(),
			oletime.GetSecond()
			);
	return s;
}
*/

//取xml指定命令下的键值
int CGlobal::GetCmdValue(char *Data, char *CmdItem, int MaxSize, char *CmdVal, BOOL bUTF8)
{
	char *p1 = NULL;
	char *p2 = NULL;
	char *p3 = NULL;
	char find[256] = {0};
	
	memset(find, 0, sizeof(find));
	_snprintf(find, sizeof(find)-1, "<%s", CmdItem);
	//strcpy(find, "<");
	//strcat(find, CmdItem);
	p1 = /*StrStrI*/strstri(Data, find);
	if (NULL == p1)
	{
		return -1;
	}
	
	memset(find, 0, sizeof(find));
	find[0] = '>';
	//strcat(find, ">");
	p2 = /*StrStrI*/strstri(p1, find);
	if (NULL == p2)
	{
		return -1;
	}
	p1 = p2 + 1;
	
	memset(find, 0, sizeof(find));
	_snprintf(find, sizeof(find)-1, "</%s>", CmdItem);
	//strcpy(find, "</");
	//strcat(find, CmdItem);
	//strcat(find, ">");
	
	p2 = /*StrStrI*/strstri(p1, find);
	if (NULL == p2)
	{
		//这里防止用户对整个xml文档进行utf-8致使</>中的<被转了码转成了0x3c
		//若没有发现</cmd>就来找/cmd>找到后指针后移一位
		memset(find, 0, sizeof(find));
		_snprintf(find, sizeof(find)-1, "/%s>", CmdItem);
		//strcpy(find, "/");
		//strcat(find, CmdItem);
		//strcat(find, ">");

		p2 = /*StrStrI*/strstri(p1, find);
		if (NULL == p2)
		{
			return -1;
		}
		--p2; 
	}
	
	//截取到的长度，超出最大值，或为负数则放弃之
	if (p2 - p1 > MaxSize || p2 - p1 < 0)
	{
		return -2;
	}

	int nCmdValueLen = 0;
	if (bUTF8)
	{
		char *szTmp = new char[p2 - p1 + 1];
        char *szIn   = new char[p2 - p1 + 1];
		if (szTmp && szIn)
		{
			try
			{
				memset(szTmp, 0, p2-p1+1);
                memset(szIn, 0,p2-p1+1);
                //zhubo 2017-06-20
                memcpy(szIn,p1,p2-p1);
                
                //zhubo 2017-4-7
                
             //   UTF_8ToGB2312_EX(szTmp, szIn, p2 - p1, p2 - p1);	
                //UTF_8ToGB2312_EX(szTmp, p1, p2 - p1, p2 - p1);	
				//UTF_8ToGB2312(szTmp, p1, p2 - p1, p2 - p1);	
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
        if(szIn)
        {
            delete[] szIn;
            szIn = NULL;
        }
		nCmdValueLen = strlen(CmdVal);
	}
	else
	{
// 		if (p2 - p1 > MaxSize)
// 		{
// 			return -2;
// 		}
		memcpy(CmdVal, p1, p2 - p1);
		CmdVal[p2-p1] = 0x0;
		nCmdValueLen = p2 - p1;
	}
	return nCmdValueLen;//strlen(CmdVal);
}


//解析Http-post请求的内容参数以&符号分开的这种形式
int CGlobal::GetCmdValueEx(char *Data, char *CmdItem, int MaxSize, char *CmdVal, BOOL bConvert)
{
	char *p1 = NULL;
	char *p2 = NULL;
	char *p3 = NULL;
	char find[256] = {0};
	
	_snprintf(find, sizeof(find)-1, "%s=", CmdItem);
	//strcat(find, CmdItem);
	//strcat(find, "=");
	p1 = /*StrStrI*/strstri(Data, find);
	if (NULL == p1)
	{
		return -1;
	}
	p2 = p1 + strlen(find);
// 	memset(find, 0, sizeof(find));
// 	strcat(find, "=");
// 	p2 = StrStrI(p1, find);
// 	if (NULL == p2)
// 	{
// 		return -1;
// 	}
	p1 = p2;
	
	memset(find, 0, sizeof(find));
	find[0] = '&';
	//strcpy(find, "&");
	
	p2 = /*StrStrI*/strstri(p1, find);
	if (NULL == p2)
	{
		p2 = &Data[strlen(Data)];
	}
	
	//截取到的长度，超出最大值，或为负数则放弃之
	if (p2 - p1 > MaxSize || p2 - p1 < 0)
	{
		return -2;
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
                //zhubo 2017-4-7
				//UTF_8ToGB2312(CmdVal, szTmp, nOutLen, MaxSize);
				//UTF_8ToGB2312_EX(CmdVal, szTmp, nOutLen, MaxSize);
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


//解析Http-post请求的内容参数以&符号分开的这种形式
int CGlobal::GetCmdValueExEx(char *Data, char *CmdItem, int MaxSize, char *CmdVal)
{
	char *p1 = NULL;
	char *p2 = NULL;
	char *p3 = NULL;
	char find[256] = {0};
	
	_snprintf(find, sizeof(find)-1, "%s=", CmdItem);
	//strcat(find, CmdItem);
	//strcat(find, "=");
	p1 = /*StrStrI*/strstri(Data, find);
	if (NULL == p1)
	{
		return -1;
	}
	p2 = p1 + strlen(find);
	// 	memset(find, 0, sizeof(find));
	// 	strcat(find, "=");
	// 	p2 = StrStrI(p1, find);
	// 	if (NULL == p2)
	// 	{
	// 		return -1;
	// 	}
	p1 = p2;
	
	memset(find, 0, sizeof(find));
	find[0] = '&';
	//strcpy(find, "&");
	
	p2 = /*StrStrI*/strstri(p1, find);
	if (NULL == p2)
	{
		p2 = &Data[strlen(Data)];
	}
	
	//截取到的长度，超出最大值，或为负数则放弃之
	if (p2 - p1 > MaxSize || p2 - p1 < 0)
	{
		return -2;
	}

	int nCmdValueLen = 0;
	int nOutLen = p2 - p1;
	char *szTmp = new char[nOutLen + 1];
	if (NULL != szTmp)
	{
		try
		{
			memset(szTmp, 0, nOutLen+1);
			DecodeHtmlStrEx(szTmp, p1, nOutLen);	
			if (nOutLen > (unsigned int)MaxSize)
			{
				if (NULL != szTmp)
				{
					delete[] szTmp;
					szTmp = NULL;
				}
				return -2;
			}
		//	UTF_8ToGB2312(CmdVal, szTmp, nOutLen);
			memcpy(CmdVal, szTmp, nOutLen);
			CmdVal[nOutLen] = '\0';
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
// 	else
// 	{
// 		if (p2 - p1 > MaxSize)
// 		{
// 			return -2;
// 		}
// 	memcpy(CmdVal, p1, p2 - p1);
// 	CmdVal[p2-p1] = 0x0;
// 	nCmdValueLen = p2-p1;
// 	}
	return nCmdValueLen;/*strlen(CmdVal);*/
}








//解析JAVA网页编码
void CGlobal::ParaseJavaEncode(char *szDest, char *szSrc, int& nInOutLen, int nDestMaxLen)
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
	if (NULL == /*StrStrI*/strstri(szSrc, "&#x"))
	{
		nInOutLen = 0;
		return;
	}
	/*&#x606D;;123&#xFF01;"*/
	int i			= 0;
	int nPos		= 0;
	char szTmp[2+1] = {0};
	char *pPos = szSrc;
	int nOutLen = 0;
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
			if (nDestMaxLen - nPos <= 0 )
			{
				break;
			}
			szDest[nPos++] = nRet;
			pPos += 2;
			i    += 2;
			
			memset(szTmp, 0, 2);
			memcpy(szTmp, pPos, 2);
			sscanf(szTmp, "%x", &nRet);
			if (nDestMaxLen - nPos <= 0 )
			{
				break;
			}
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
			/*int */nOutLen = 0;
            //zhubo 2017-4-7
            //AsciiToUS2(szBuf, szBufOut, (UINT*)&nOutLen);
			CAdapter::AsciiToUS2(szBuf, szBufOut, (UINT*)&nOutLen);
			if (nDestMaxLen - nPos <= 1 )
			{
				break;
			}
			memcpy(szDest+nPos, szBufOut, nOutLen);
			nPos += nOutLen;
			++i;
		}
	}

	nInOutLen = nPos;
}

typedef char Ch;
static void insert_coded_character(Ch *&text, unsigned long code)
{
	// Insert UTF8 sequence
	if (code < 0x80)    // 1 byte sequence
	{
		text[0] = static_cast<unsigned char>(code);
		text += 1;
	}
	else if (code < 0x800)  // 2 byte sequence
	{
		text[1] = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
		text[0] = static_cast<unsigned char>(code | 0xC0);
		text += 2;
	}
	else if (code < 0x10000)    // 3 byte sequence
	{
		text[2] = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
		text[1] = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
		text[0] = static_cast<unsigned char>(code | 0xE0);
		text += 3;
	}
	else if (code < 0x110000)   // 4 byte sequence
	{
		text[3] = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
		text[2] = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
		text[1] = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
		text[0] = static_cast<unsigned char>(code | 0xF0);
		text += 4;
	}
	else    // Invalid, only codes up to 0x10FFFF are allowed in Unicode
	{
		//RAPIDXML_PARSE_ERROR("invalid numeric character entity", text);
	}
}



static void insert_coded_character(std::string& strText, unsigned long code)
{
	// Insert UTF8 sequence
	if (code < 0x80)    // 1 byte sequence
	{
		strText += static_cast<unsigned char>(code);
	}
	else if (code < 0x800)  // 2 byte sequence
	{
		Ch c1 = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
		Ch c0 = static_cast<unsigned char>(code | 0xC0);
		strText += c0;
		strText += c1;
	}
	else if (code < 0x10000)    // 3 byte sequence
	{
		Ch c2 = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
		Ch c1 = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
		Ch c0 = static_cast<unsigned char>(code | 0xE0);
		strText += c0;
		strText += c1;
		strText += c2;
	}
	else if (code < 0x110000)   // 4 byte sequence
	{
		Ch c3 = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
		Ch c2 = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
		Ch c1 = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
		Ch c0 = static_cast<unsigned char>(code | 0xF0);
		strText += c0;
		strText += c1;
		strText += c2;
		strText += c3;
	}
	else    // Invalid, only codes up to 0x10FFFF are allowed in Unicode
	{
		//RAPIDXML_PARSE_ERROR("invalid numeric character entity", text);
	}
}



void CGlobal::ParaseXmlUTF8(string& strIn,string& strOut)
{
    const char *src = strIn.c_str();
	//std::string strDest = "";
	while (*src != '\0')
	{
		// If entity translation is enabled    
		// Test if replacement is needed
		if (src[0] == Ch('&'))
		{
			switch (src[1])
			{
				// &amp; &apos;
			case Ch('a'):
				if (src[2] == Ch('m') && src[3] == Ch('p') && src[4] == Ch(';'))
				{
					strOut += Ch('&');
					src += 5;
					continue;
				}
				if (src[2] == Ch('p') && src[3] == Ch('o') && src[4] == Ch('s') && src[5] == Ch(';'))
				{
					strOut += Ch('\'');
					src += 6;
					continue;
				}
				break;
				// &quot;
			case Ch('q'):
				if (src[2] == Ch('u') && src[3] == Ch('o') && src[4] == Ch('t') && src[5] == Ch(';'))
				{
					strOut += Ch('"');
					src += 6;
					continue;
				}
				break;
				// &gt;
			case Ch('g'):
				if (src[2] == Ch('t') && src[3] == Ch(';'))
				{
					strOut += Ch('>');
					src += 4;
					continue;
				}
				break;
				// &lt;
			case Ch('l'):
				if (src[2] == Ch('t') && src[3] == Ch(';'))
				{
					strOut += Ch('<');
					src += 4;
					continue;
				}
				break;
				// &#...; - assumes ASCII
			case Ch('#'):
			{
				unsigned long code = 0;
				const char *pOrgSrc = src;

				if (src[2] == Ch('x'))
				{
					//unsigned long code = 0;
					src += 3;   // Skip &#x
					while (1)
					{
						unsigned char digit = HexchartoInt_ex(*src);
						if (digit == 0xFF)
							break;
						code = code * 16 + digit;
						++src;
					}
					//insert_coded_character(dest, code);    // Put character in output
				}
				else
				{
					//unsigned long code = 0;
					src += 2;   // Skip &#
					while (1)
					{
						unsigned char digit = HexchartoInt_ex(*src);
						if (digit == 0xFF)
							break;

						code = code * 10 + digit;
						++src;
					}
					//insert_coded_character(dest, code);    // Put character in output
				}
				//判断是否有数据结束符。
				if (*src == Ch(';'))
				{
					insert_coded_character(strOut, code);    // Put character in output
					++src;
					continue;
				}
				else
				{
					strOut.append(pOrgSrc, src - pOrgSrc);
					continue;
				}
			}
			default:
				// Ignore, just copy '&' verbatim
				break;
			}
		}
		// No replacement, only copy character
		strOut += (*src++);
	}
}


//解析JAVA网页编码
void CGlobal::ParaseXmlUtf8ToGBk(char *szDest, const char *szSrc, int nInOutLen, int nMaxOutLen)
{
	const char *src = szSrc;
	char *dest = szDest;
	while (*src != '\0')
	{
		// If entity translation is enabled    
			// Test if replacement is needed
			if (src[0] == Ch('&'))
			{
				switch (src[1])
				{
					// &amp; &apos;
				case Ch('a'): 
					if (src[2] == Ch('m') && src[3] == Ch('p') && src[4] == Ch(';'))
					{
						*dest = Ch('&');
						++dest;
						src += 5;
						continue;
					}
					if (src[2] == Ch('p') && src[3] == Ch('o') && src[4] == Ch('s') && src[5] == Ch(';'))
					{
						*dest = Ch('\'');
						++dest;
						src += 6;
						continue;
					}
					break;
					
					// &quot;
				case Ch('q'): 
					if (src[2] == Ch('u') && src[3] == Ch('o') && src[4] == Ch('t') && src[5] == Ch(';'))
					{
						*dest = Ch('"');
						++dest;
						src += 6;
						continue;
					}
					break;
					
					// &gt;
				case Ch('g'): 
					if (src[2] == Ch('t') && src[3] == Ch(';'))
					{
						*dest = Ch('>');
						++dest;
						src += 4;
						continue;
					}
					break;
					
					// &lt;
				case Ch('l'): 
					if (src[2] == Ch('t') && src[3] == Ch(';'))
					{
						*dest = Ch('<');
						++dest;
						src += 4;
						continue;
					}
					break;
					
					// &#...; - assumes ASCII
				case Ch('#'): 
					
					{
						unsigned long code = 0;
						const char *pOrgSrc = src;
						
						if (src[2] == Ch('x'))
						{
							//unsigned long code = 0;
							src += 3;   // Skip &#x
							while (1)
							{
								unsigned char digit = HexchartoInt_ex(*src);
								if (digit == 0xFF)
									break;
								code = code * 16 + digit;
								++src;
							}
							//insert_coded_character(dest, code);    // Put character in output
						}
						else
						{
							//unsigned long code = 0;
							src += 2;   // Skip &#
							while (1)
							{
								unsigned char digit = HexchartoInt_ex(*src);
								if (digit == 0xFF)
									break;
								
								code = code * 10 + digit;
								++src;
							}
							//insert_coded_character(dest, code);    // Put character in output
						}
						
						
						//判断是否有数据结束符。
						if (*src == Ch(';'))
						{
							insert_coded_character(dest, code);    // Put character in output
							++src;
							continue;
						}
						else
						{
							
							memcpy(dest,pOrgSrc,src - pOrgSrc);
							dest += src - pOrgSrc;
							continue;
						}
						
					}
					
				default:
					// Ignore, just copy '&' verbatim
					break;
					
				}
			}
			
			// No replacement, only copy character
			*dest++ = *src++;
    }
	*dest = 0;
	//zhubo 2017-4-7

    /*
    char  *pIN = NULL;
    UINT  il = strlen(szDest);
    if(il > 0)
    {
        pIN = new char[il + 1];
        memcpy(pIN,szDest,il);
        pIN[il]= 0;
    }
    */
    
	//UTF_8ToGB2312(szDest, szDest, dest-szDest, nMaxOutLen);
	//UTF_8ToGB2312_EX(szDest, szDest, dest-szDest, nMaxOutLen);
	//UTF_8ToGB2312_EX(szDest, szDest, dest-szDest, nMaxOutLen);

    /*
    if(pIN != NULL)
    {
        delete[] pIN;
        pIN = NULL;
    }
    */
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
	int i			= 0;
	int nPos		= 0;
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


void CGlobal::DecodeHtmlStr(string& strIn,string& strOut)
{
	if (strIn.size() <= 0)
	{
		return;
	}
	int i			= 0;
	int nPos		= 0;
	char szTmp[2+1] = {0};
	char *pPos = (char*)strIn.c_str();
    int nLen = strIn.size();
	while (i < nLen)
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
			//szDest[nPos++] = nRet;
			strOut += nRet;
			pPos += 2;
			i    += 2;
		}
		else
		{
			if('+' == *pPos)
            {
                strOut += ' ';
            }
            else
            {
                strOut += *pPos;
            }   
			++pPos;
			++nPos;
			++i;
		}
	}
    
	//nInOutLen = nPos;
}

//解析URL-ENCODE
void CGlobal::DecodeHtmlStrEx(char *szDest, char *szSrc, int& nInOutLen)
{
	if (NULL == szDest)
	{
		nInOutLen = 0;
		return;
	}
	if (NULL == szSrc)
	{
		szDest[0] = '\0';
		nInOutLen = 0;
		return;
	}
	int i = 0;
	int nPos = 0;
	char szTmp[2 + 1] = { 0 };
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
			i += 2;
		}
		else
		{
			szDest[nPos] = *pPos;
			/*if ('+' == szDest[nPos])
			{
			szDest[nPos] = ' ';
			}*/
			++pPos;
			++nPos;
			++i;
		}
	}
	nInOutLen = nPos;
}

void CGlobal::DecodeHtmlStrEx(string& strIn,string& strOut)
{

	if (strIn.size() <= 0)
	{
		return;
	}
    
	int i			= 0;
	int nPos		= 0;
	char szTmp[2+1] = {0};
	char *pPos = (char*)strIn.c_str();
    int nLen = strIn.size();
	while (i < nLen)
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
			//szDest[nPos++] = nRet;
			strOut += nRet;
			pPos += 2;
			i    += 2;
		}
		else
		{
			strOut += *pPos;
			++pPos;
			++nPos;
			++i;
		}
	}
    
	//nInOutLen = nPos;
}



//取用户特权属性值
int	CGlobal::GetUserPrivilege(int nPrivilege, int nBit)
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

//控制蜂鸣报警
void CGlobal::BeepEx(DWORD dwFreq, DWORD dwDuration)
{
    CAdapter::Beep(dwFreq,dwDuration);
    //zhubo 2017-4-7
    /*
	if (BeepOrNot)
	{
		Beep(dwFreq, dwDuration);
	}
	else
	{
		Sleep(dwDuration);
	}
	*/
} 


//由流水号和外部时间来获取一个合理的时间
int  CGlobal::GetValidTime(const char* szMsgId, const char* szDoneTime, char* szTime/*in/out "YYYY-MM-DD HH-MM-SS"*/)
{   
    
	struct tm tNow;
	time_t t = time(NULL);
	localtime_r(&t,&tNow);
	int nSec = tNow.tm_sec;
    
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

char* CGlobal::strstri(const char* pString, const char* pFind)
{
    char* char1 = NULL;
    char* char2 = NULL;
    if((pString == NULL) || (pFind == NULL) || (strlen(pString) < strlen(pFind)))
    {
        return NULL;
    }
	
    for(char1 = (char*)pString; (*char1) != '\0'; ++char1)
    {
        char* char3 = char1;
        for(char2 = (char*)pFind; (*char2) != '\0' && (*char1) != '\0'; ++char2, ++char1)
        {
            char c1 = (*char1) & 0xDF;
            char c2 = (*char2) & 0xDF;
            if((c1 != c2) || (((c1 > 0x5A) || (c1 < 0x41)) && (*char1 != *char2)))
                break;
        }
		
        if((*char2) == '\0')
            return char3;
		
        char1 = char3;
    }
    return NULL;
}

void CGlobal::replace(std::string& str, std::string olds, std::string news)
{
	size_t pos = 0;	
	while ((pos = str.find(olds, pos)) != std::string::npos)
	{
		str.replace(pos, olds.size(), news);
		pos += news.length();
	}
}

std::string CGlobal::date(std::string fmt /*= "Y-m-d H:i:s" */, int nday /*= 0*/)
{	
	//struct tm* newtime; 
	struct tm newtime;
	char tmpbuf[100] = {0}; 
	time_t lt; 
	lt = time(NULL) + nday * 24 * 60 * 60; 	 
	localtime_r(&lt,&newtime); 
	replace(fmt, "Y", "%Y");
	replace(fmt, "m", "%m");
	replace(fmt, "d", "%d");
	replace(fmt, "H", "%H");
	replace(fmt, "i", "%M");
	replace(fmt, "s", "%S");		
	strftime(tmpbuf, 100, fmt.c_str(), &newtime);
	std::string str(tmpbuf);
	return str;
}

void CGlobal::trimLR(std::string &str)
{
	int nLen = str.length();
	int nTmp = nLen;
	int nLeft = 0, nRight = 0;
	int flag1 = 1, flag2 = 1;
	
	while(nLeft + nRight < nTmp)
	{
		if(str.at(nLen -1) == ' ' && flag1)
		{
			--nLen;
			++nRight;   //统计右边的空格数
		}
		else
		{
			flag1 = 0;
		}
		
		if(str.at(nLeft) == ' ' && flag2)
		{
			++nLeft;   //统计左边的空格数
		}
		else
		{
			flag2 = 0;
		}
		if(!flag1 && !flag2)  // 左右空格已计算完毕
		{
			break;
		}
	}
	
	str.assign(str, nLeft,  nTmp - nLeft - nRight);
	
	return;
}


void CGlobal::trimLR(char *str)
{
	int nLen = strlen(str);
	int nTmp = nLen;
	int nLeft = 0, nRight = 0;
	int flag1 = 1, flag2 = 1;
	
	while(nLeft + nRight < nTmp)
	{
		if(str[nLen -1] == ' ' && flag1)
		{
			--nLen;
			++nRight;   //统计右边的空格数
		}
		else
		{
			flag1 = 0;
		}
		
		if(str[nLeft] == ' ' && flag2)
		{
			++nLeft;   //统计左边的空格数
		}
		else
		{
			flag2 = 0;
		}
		if(!flag1 && !flag2)  // 左右空格已计算完毕
		{
			break;
		}
	}
	memcpy(str, str + nLeft, nTmp - nLeft - nRight);
	str[nTmp - nLeft - nRight] = '\0';
	
	return;
}



BOOL CGlobal::UrlEncode(char* szSrc, char* pBuf, int cbBufLen, BOOL bUpperCase)
{
	if (szSrc == NULL || pBuf == NULL || cbBufLen <= 0)
		return FALSE;

	size_t len_ascii = strlen(szSrc);
	if (len_ascii == 0)
	{
		pBuf[0] = 0;
		return TRUE;
	}

	char baseChar = bUpperCase ? 'A' : 'a';
	iconv_t hConv = iconv_open("utf-8", "gbk");
	if (0 == hConv)
	{
		return FALSE;
	}
	
	char* inBuf = szSrc;
	size_t inBufsize = len_ascii;
	size_t outBufsize = (inBufsize + 1) * 2;
	char* outBuf = new char[outBufsize];
	memset(outBuf, 0, outBufsize);

	if (NULL == outBuf)
	{
		iconv_close(hConv);
		return FALSE;
	}
	
	char* currOutBuf = outBuf;
	size_t nRet = iconv(hConv, &inBuf, &inBufsize, &currOutBuf, &outBufsize);
	if ((size_t)(-1) == nRet)
	{
		if (outBuf)
		{
			delete [] outBuf;
		}
		iconv_close(hConv);
		return FALSE;
	}
	iconv_close(hConv);
	
	unsigned char c;
	int cbDest = 0; //累加
	unsigned char *pSrc = (unsigned char*)outBuf;
	unsigned char *pDest = (unsigned char*)pBuf;
	while (*pSrc && cbDest < cbBufLen - 1)
	{
		c = *pSrc;
		if (isalpha(c) || isdigit(c) || c == '-' || c == '.' || c == '~')
		{
			*pDest = c;
			++pDest;
			++cbDest;
		}
		else if (c == ' ')
		{
			*pDest = '+';
			++pDest;
			++cbDest;
		}
		else
		{
			//检查缓冲区大小是否够用？
			if (cbDest + 3 > cbBufLen - 1)
				break;
			pDest[0] = '%';
			pDest[1] = (c >= 0xA0) ? ((c >> 4) - 10 + baseChar) : ((c >> 4) + '0');
			pDest[2] = ((c & 0xF) >= 0xA) ? ((c & 0xF) - 10 + baseChar) : ((c & 0xF) + '0');
			pDest += 3;
			cbDest += 3;
		}
		++pSrc;
	}

	*pDest = '\0';
	if (outBuf)
	{
		delete [] outBuf;
	}
	return TRUE;
	/*
    if(szSrc == NULL || pBuf == NULL || cbBufLen <= 0)
        return FALSE;

    size_t len_ascii = strlen(szSrc);
    if(len_ascii == 0)
    {
        pBuf[0] = 0;
        return TRUE;
    }
    
    //先转换到UTF-8
    char baseChar = bUpperCase ? 'A' : 'a';
    int cchWideChar = CAdapter::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)szSrc, len_ascii, (LPWSTR)NULL, 0);
    LPWSTR pUnicode = (LPWSTR)malloc((cchWideChar + 1) * sizeof(WCHAR));
    if(pUnicode == NULL)
        return FALSE;
    CAdapter::MultiByteToWideChar((UINT)CP_ACP, (DWORD)0, (LPCSTR)szSrc, len_ascii, (LPWSTR)pUnicode, cchWideChar + 1);

    int cbUTF8 = CAdapter::WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)pUnicode, cchWideChar, (LPSTR)NULL, 0, (LPCSTR)NULL, (LPBOOL)NULL);
    LPSTR pUTF8 = (LPSTR)malloc((cbUTF8 + 1) * sizeof(CHAR));
    if(pUTF8 == NULL)
    {
        free(pUnicode);
        return FALSE;
    }
    CAdapter::WideCharToMultiByte((UINT)CP_UTF8, 0, (LPCWSTR)pUnicode, cchWideChar, (LPSTR)pUTF8, cbUTF8 + 1, (LPCSTR)NULL, (LPBOOL)NULL);
    pUTF8[cbUTF8] = '\0';

    unsigned char c;
    int cbDest = 0; //累加
    unsigned char *pSrc = (unsigned char*)pUTF8;
    unsigned char *pDest = (unsigned char*)pBuf;
    while(*pSrc && cbDest < cbBufLen - 1)
    {
        c = *pSrc;
        if(isalpha(c) || isdigit(c) || c == '-' || c == '.' || c == '~')
        {
            *pDest = c;
            ++pDest;
            ++cbDest;
        }
        else if(c == ' ')
        {
            *pDest = '+';
            ++pDest;
            ++cbDest;
        }
        else
        {
            //检查缓冲区大小是否够用？
            if(cbDest + 3 > cbBufLen - 1)
                break;
            pDest[0] = '%';
            pDest[1] = (c >= 0xA0) ? ((c >> 4) - 10 + baseChar) : ((c >> 4) + '0');
            pDest[2] = ((c & 0xF) >= 0xA)? ((c & 0xF) - 10 + baseChar) : ((c & 0xF) + '0');
            pDest += 3;
            cbDest += 3;
        }
        ++pSrc;
    }
    //null-terminator
    *pDest = '\0';
    free(pUnicode);
    free(pUTF8);
    return TRUE;
    */
}

std::string CGlobal::UrlEncode(const std::string& src)
{
	static    char hex[] = "0123456789ABCDEF";
	std::string dst;

	for (size_t i = 0; i < src.size(); i++)
	{
		unsigned char ch = src[i];
		if (isalnum(ch))
		{
			dst += ch;
		}
		else
		if (src[i] == ' ')
		{
			dst += '+';
		}
		else
		{
			unsigned char c = static_cast<unsigned char>(src[i]);
			dst += '%';
			dst += hex[c / 16];
			dst += hex[c % 16];
		}
	}
	return dst;
}

//解码后是utf-8编码
BOOL CGlobal::UrlDecode(char* szSrc, char* pBuf, int cbBufLen)
{
    if(szSrc == NULL || pBuf == NULL || cbBufLen <= 0)
        return FALSE;

    size_t len_ascii = strlen(szSrc);
    if(len_ascii == 0)
    {
        pBuf[0] = 0;
        return TRUE;
    }
    
    char *pUTF8 = (char*)malloc(len_ascii + 1);
    if(pUTF8 == NULL)
        return FALSE;

    int cbDest = 0; //累加
    unsigned char *pSrc = (unsigned char*)szSrc;
    unsigned char *pDest = (unsigned char*)pUTF8;
    while(*pSrc)
    {
        if(*pSrc == '%')
        {
            *pDest = 0;
            //高位
            if(pSrc[1] >= 'A' && pSrc[1] <= 'F')
                *pDest += (pSrc[1] - 'A' + 10) * 0x10;
            else if(pSrc[1] >= 'a' && pSrc[1] <= 'f')
                *pDest += (pSrc[1] - 'a' + 10) * 0x10;
            else
                *pDest += (pSrc[1] - '0') * 0x10;

            //低位
            if(pSrc[2] >= 'A' && pSrc[2] <= 'F')
                *pDest += (pSrc[2] - 'A' + 10);
            else if(pSrc[2] >= 'a' && pSrc[2] <= 'f')
                *pDest += (pSrc[2] - 'a' + 10);
            else
                *pDest += (pSrc[2] - '0');

            pSrc += 3;
        }
        else if(*pSrc == '+')
        {
            *pDest = ' ';
            ++pSrc;
        }
        else
        {
            *pDest = *pSrc;
            ++pSrc;
        }
        ++pDest;
        ++cbDest;
    }
    //null-terminator
    *pDest = '\0';
    ++cbDest;

    int cchWideChar = CAdapter::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pUTF8, cbDest, (LPWSTR)NULL, 0);
    LPWSTR pUnicode = (LPWSTR)malloc(cchWideChar * sizeof(WCHAR));
    if(pUnicode == NULL)
    {
        free(pUTF8);
        return FALSE;
    }
    CAdapter::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pUTF8, cbDest, (LPWSTR)pUnicode, cchWideChar);
    CAdapter::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)pUnicode, cchWideChar, (LPSTR)pBuf, cbBufLen, (LPCSTR)NULL, (LPBOOL)NULL);
    free(pUTF8);
    free(pUnicode);
    return TRUE;
}

//IP转换成int
unsigned int CGlobal::IpToInt(const char* ipStr)
{
    unsigned int nRet = 0;
	char szIp[64] = {0};
	memcpy(szIp, ipStr, min(strlen(ipStr),sizeof(szIp)-1));
	
    char* szBufTemp = NULL;
    char* szBuf = strtok_r(szIp,".",&szBufTemp);

    int i = 0;//计数
    while(NULL != szBuf)//取一个
    {
        nRet += atoi(szBuf)<<((3-i)*8);
        szBuf = strtok_r(NULL,".",&szBufTemp);
        i++;
    }

    return nRet;
}

/*
unsigned int CGlobal::IpToInt(const char* ipStr)
{
	std::string s = ipStr;

    unsigned int ipInt = 0;
    int tokenInt = 0;
    char * token;
    token = strtok((char*)s.c_str(), ".");
    int i = 3;
    while(token != NULL)
	{
		//      tokenInt = strtol(token, NULL, 10); //strtol comes from stdlib.h
        tokenInt = atoi(token);
        ipInt += tokenInt * pow(double(256), i);
        token = strtok(NULL, ".");
        i--;
    }
    return ipInt;
}
*/
bool CGlobal::IsIpInner(unsigned int clientIp, unsigned int begin, unsigned int end)
{
	return (clientIp >= begin) && (clientIp <= end);
}

//////////////////////////////////////////////////////////////////////////
//随机时间,24小时制
int CGlobal::GetRandomTime(int nBeginHour, int nBeginMin, int nBeginSec,
	int nMaxHour, int nMaxMinu, int nMaxSec)
{
	int nRet = 0;
	MCHECK_INVALID_VALUE_EX(nBeginHour, 0, 23);
	MCHECK_INVALID_VALUE_EX(nBeginMin, 0, 59);
	MCHECK_INVALID_VALUE_EX(nBeginSec, 0, 59);

	MCHECK_INVALID_VALUE_EX(nMaxHour, 0, 23);
	MCHECK_INVALID_VALUE_EX(nMaxMinu, 0, 59);
	MCHECK_INVALID_VALUE_EX(nMaxSec, 0, 59);

	srand(time(NULL));

	//随机小时0~23
	int nHour = nBeginHour + rand() % (24 - nBeginHour);
	if (nMaxHour > 0 && nHour > nMaxHour) nHour = nMaxHour;

	//随机分0~59
	int nMin = nBeginMin + rand() % (60 - nBeginMin);
	if (nMaxMinu > 0 && nMin > nMaxMinu) nMin = nMaxMinu;

	//随机秒0~59
	int nSec = nBeginSec + rand() % (60 - nBeginSec);
	if (nMaxSec > 0 && nSec > nMaxSec) nSec = nMaxSec;

	nRet = nHour * 3600 + nMin * 60 + nSec;

	return nRet;
}


bool CGlobal::bInOrOutTime(int nDaySec)
{
	if (nDaySec < 0)
	{
		return true;
	}

	bool bRet = false;
	time_t tCurrent = time(NULL);
	//struct tm* ptCurrentTime = localtime(&tCurrent);
	struct tm currentTime,*ptCurrentTime;
    
    localtime_r(&tCurrent,&currentTime);

	int nHour = 0, nMin = 0, nSec = 0;
	nHour = nDaySec / 3600;
	nMin = (nDaySec - nHour * 3600) / 60;
	nSec = (nDaySec - nHour * 3600) - 60 * nMin;

    ptCurrentTime = &currentTime;

	if (ptCurrentTime->tm_hour > nHour)
	{
		bRet = true;
	}
	else if (ptCurrentTime->tm_hour == nHour)
	{
		if (ptCurrentTime->tm_min > nMin)
		{
			bRet = true;
		}
		else if (ptCurrentTime->tm_min == nMin && ptCurrentTime->tm_sec >= nSec)
		{
			bRet = true;
		}
	}

	return bRet;
}

// add by dengyu 2017-4-28
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

// add by dengyu 2017-5-24
DATE CGlobal::StrToDATE(const char* strTime)
{
	DATE result = 0;
	int hour = 0, min = 0, sec = 0;
	sscanf(strTime, "%d:%d:%d", &hour, &min, &sec);
	result = hour * 3600 + min * 60 + sec;
	return result;
}

void CGlobal::string_replace(string&s1, const string&s2, const string&s3)
{
	string::size_type pos = 0;
	string::size_type a = s2.size();
	string::size_type b = s3.size();
	while ((pos = s1.find(s2, pos)) != string::npos)
	{
		s1.replace(pos, a, s3);
		pos += b;
	}
	return;
}

bool CGlobal::IsEnglishMsg(const char * szMsg, int nLen)
{
	bool bRet = true;
	//判断是否是ASCII
	int i = 0;
	while (i < nLen)
	{
		bRet = (0 == szMsg[i] && isascii(szMsg[i + 1]) != 0);
		if (!bRet)
		{
			break;
		}
		i += 2;
	}

	return bRet;
}

DerivedLock g_ccccsSeqId;
int CGlobal::MakeSeqId()
{
    SafeLock safelock(&g_ccccsSeqId);
	static int s_nSeqId = 1;
	++s_nSeqId<0?s_nSeqId=1:1;
	return s_nSeqId;
}
//////////////////////////////////////////////////////////////////
