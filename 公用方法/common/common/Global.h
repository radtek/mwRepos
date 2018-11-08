#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include <vector>
//zhubo 2017-4-6
//#include "shlwapi.h"
#include <algorithm>
#include <string>
#include <deque>
#include <math.h>
#include <map>
#include <iostream>
#include <string.h>
#include <stdarg.h>
#include <wbs_config.h>



using namespace std;

extern BOOL g_bMemError;



extern int MIN2(int a, int b);
int max(int i1, int i2);
int min(int i1, int i2);



class cstr
{
public:
    static void to_lower(char *str)
    {
        int   i = 0;
        while(str[i] != '\0')
        {
            if((str[i] >= 'A') && (str[i] <= 'Z'))
            {
                str[i] += 32;
            }
            ++i;
        }
    }

	static void to_lower(string& strInOut)
	{
	
		string strOut("");
		int nLen = strInOut.size();
		int   i = 0;
		
        while(i <= nLen -1)
        {
            if((strInOut[i] >= 'A') && (strInOut[i] <= 'Z'))
            {
                strOut += (char)(strInOut[i] + 32);
            }
			else
			{
				strOut += strInOut[i];
			}
            ++i;
        }
    	strInOut.swap(strOut);
	}

	static std::string to_lower_s(const char* str)
    {
		std::string strTmp = str;
        int   i = 0;
        while(strTmp[i] != '\0')
        {
            if((strTmp[i] >= 'A') && (strTmp[i] <= 'Z'))
            {
                strTmp[i] += 32;
            }
            ++i;
        }
		return strTmp;
    }


    static void to_upper(char *str)
    {
        int  i = 0;
        while(str[i] != '\0')
        {
            if((str[i] >= 'a') && (str[i] <= 'z'))
            {
                str[i] -= 32;
            }
            ++i;
        }
    }



	static std::string to_upper_s(const char* str)
    {
		std::string strTmp = str;
        int  i = 0;
        while(strTmp[i] != '\0')
        {
            if((strTmp[i] >= 'a') && (strTmp[i] <= 'z'))
            {
                strTmp[i] -= 32;
            }
            ++i;
        }
		return strTmp;
    }
    
    //字符串格式化函数
    static std::string format(const char *fmt, ...)
    {
			va_list marker;
			va_start(marker, fmt);
			std::string strResult = "";
			int nLength = vsnprintf(NULL, 0, fmt, marker);
			va_end(marker);
			if (nLength < 0) return "";
			va_start(marker, fmt);
			strResult.resize(nLength + 1);
			int nSize = vsnprintf((char*)strResult.data(), nLength + 1, fmt, marker);
			va_end(marker);
			if (nSize > 0) 
			{
				strResult.resize(nSize);
			}
			else 
			{
				strResult = "";
			}
			return strResult;
    }

		static void format(std::string& strRet, const char *fmt, ...)
		{
			va_list marker;
			va_start(marker, fmt);
      std::string strResult = "";
			int nLength = vsnprintf(NULL, 0, fmt, marker);
			va_end(marker);
			if (nLength < 0) return;
			va_start(marker, fmt);
			strRet.resize(nLength + 1);
			int nSize = vsnprintf((char*)strRet.data(), nLength + 1, fmt, marker);
			va_end(marker);
			strResult.resize(nSize);
			if (nSize > 0) 
			{
				strResult.resize(nSize);
			}
			else 
			{
				strResult = "";
			}
		}

    //字符串格式化函数
    //zhubo 2017-4-7
  
    static std::string format_s(size_t maxsize, const char *fmt, ...)
    {
		if (NULL == fmt)
		{
			return "";
		}
        std::string strResult = "";
        va_list marker;
        va_start(marker, fmt);								//初始化变量参数
        size_t nLength = maxsize+1;							//获取格式化字符串长度
        std::vector<char> vBuffer(nLength, '\0');			//创建用于存储格式化字符串的字符数组
        int nWritten = _vsnprintf(&vBuffer[0], vBuffer.size(), fmt, marker);
        //当nwritten不小于0时返回的格式化到vbuffer中的数据长度,当小于0时说明出错,此时返回空字符串
		if (nWritten > 0)
        {
            strResult = &vBuffer[0];
        }
        va_end(marker);                                    //重置变量参数
        return strResult;
    }
   

    static std::string left(const std::string& str, int nCount)
    {
        std::string s = str;
        int nSize = s.size();
        nCount<0?nCount=0:nCount;
        nCount>nSize?nCount=nSize:nCount;
        if (0 == nCount && 0 == nSize)
        {
            s = "";
        }
        else
        {
            s.erase(MIN2(nCount, nSize), nSize);
        }
        return s;
    }
    static std::string right(const std::string& str, int nCount)
    {
        std::string s = str;
        nCount<0?nCount=0:nCount;
        int nSize = s.size();
        int nLen  = nSize-nCount;
        if (nLen > 0)
        {
            s.erase(0, nLen);
        }
        return s;
    }
    static void replace(std::string& str, const char* szOld, const char* szNew)
    {
		if (NULL != szNew && NULL != szOld)
		{
			int nOldLen = strlen(szOld);
			int nNewLen = strlen(szNew);
			bool bEqual = ((nNewLen == nOldLen) && 0 == memcmp(szOld, szNew, nOldLen));
			//szOld不为空字符串且szOld和szNew不相同
			if (nOldLen > 0 && !bEqual)
			{
				size_t found = str.find(szOld);
				while (found != std::string::npos)
				{
					str.replace(found, nOldLen, szNew);
					found += nNewLen;
					found = str.find(szOld, found);
				}
			}
		}
    }
    //去右空格
    static void trimright(std::string& str, const char* szTargets=NULL)
    {
        int nLen = str.size();
        int nTargetsLen = 0;
        //默认只去空格
        if (NULL == szTargets || (0 == strlen(szTargets)))
        {
            if (nLen > 0)
            {
                while (nLen > 0)
                {
                    if (str[nLen - 1] != 32)
                    {
                        break;
                    }
                    else
                    {
                        --nLen;
						str.erase(nLen, 1);
                    }
                }
            }
        }
        else
        {
            nTargetsLen = strlen(szTargets);
            if (nLen > 0)
            {
                int i = 0;
                while (nLen > 0)
                {
                    if (str[nLen - 1] != szTargets[nTargetsLen - i - 1])
                    {
                        break;
                    }
                    else
                    {
                        ++i;
                        --nLen;
                    }
                    if (i == nTargetsLen)
                    {
                        str.erase(nLen, nTargetsLen);
                        i = 0;
                        nLen = str.size();
                    }
                }
            }
        }
    }
    //去左空格
    static void trimleft(std::string& str, const char* szTargets=NULL)
    {
        int nLen = str.size();
        int nTargetsLen = 0;
        //默认只去空格
        if (NULL == szTargets || (0 == strlen(szTargets)))
        {
            if (nLen > 0)
            {
                int i = 0;
                while (i <  nLen)
                {
                    if (str[i] != 32)
                    {
                        right(str, nLen-i);
                        break;
                    }
                    else
                    {
                        ++i;
                    }
                }
            }
        }
        else
        {
           
		   nTargetsLen = strlen(szTargets);
 		   if (nLen > 0)
 		   {
 			   int i = 0;
 			   int j = 0;
 			   while (nLen > 0)
 			   {
 				   if (str[j] != szTargets[i])
 				   {
 					   break;
 				   }
 				   else
 				   {
 					   ++i;
 					   ++j;
 				   }
 				   if (i == nTargetsLen)
 				   {
 					   str = right(str, nLen - nTargetsLen);
 					   i = 0;
 					   j = 0;
 					   nLen = str.size();
 				   }
 			   }
 		   }
        }
    }
};

class CGlobal
{
public: 
	CGlobal();
static std::string to_upper_s(const char* str)
    {
		std::string strTmp = str;
        int  i = 0;
        while(strTmp[i] != '\0')
        {
            if((strTmp[i] >= 'a') && (strTmp[i] <= 'z'))
            {
                strTmp[i] -= 32;
            }
            ++i;
        }
		return strTmp;
    }
	
	/************************* qiwei add ***************************/
	static void DeleteSpecialChar(char * szSrc , int nSrclen);
	/************************* end qiwei add ******************************/


	//************************************
	// Description:将内存字符串转为ASCII字符串
	// Parameter: [IN] const char *szSrc	: 原始字符串
	//			  [IN] int nLen				: 原始字符串长度
	//		      [OUT]char *szDest			: ASCII串
	// Returns:   void 
	//************************************
	static int	CharToASCII(const char *szSrc, int nLen, char *szDest);
	static int  CharToASCIIEx(const char *szSrc, int nLen, char *szDest);

	//************************************
	// Description:将US2字符串转为ASCII字符串
	// Parameter: [IN] const char *InMsg	: 原始字符串
	//			  [IN] int InLen			: 原始字符串长度
	//		      [OUT]char *Data			: ASCII串
	//			  [OUT]int *Len				: ASCII串长度
	// Returns:   int						0:成功  非0:失败
	//************************************
	static int  US2ToAscii(const char *InMsg, int InLen, char *Data, int *Len);

	//************************************
	// Description:将ASCII字符串转为US2字符串
	// Parameter: [IN] const char* Msg		: 原始字符串
	//			  [OUT]unsigned char *Data	: US2字符串
	//			  [OUT]unsigned int *Len	: US2串长度
	// Returns:   int						0:成功  非0:失败
	//************************************
	static int	AsciiToUS2(const char* Msg, unsigned char *Data, unsigned int *Len);

	//************************************
	// Description:将XML文档中&gt &lt转换成"<>"
	// Parameter: [IN/OUT]char *buf			: 输入原始字符串,返回转换后的字符串
	// Returns:   int						0:成功  非0:失败
	//************************************
	static int	TranHttpSpace(char *buf);

	//************************************
	// Description:根据指定的拆分规则，计算该条短信要被拆成几条
	// Parameter: [IN] int nMsgLen			: 短信长度
	//			  [IN] int nSplitLen		: 拆分长度,超过nsplitlen将被拆分
	//			  [IN] int nMultilen1		: 长短信第一条的长度
	//			  [IN] int nMultilen2		: 长短信最后一条的长度
	//			  [IN] const char *szMsg	: 短信内容，若该内容是us2编码，则无需输入只填nMsgLen即可
	//			  [IN] BOOL bAsciiToUS2		: 若短信是ASCII编码，需传入szMsg并将该项置为TRUE
	// Returns:   int						返回拆分条数
	//************************************
	static int	CalcMsgNum(
					int nMsgFmt ,
					int nMsgLen, 
					int nSplitLen,
					int nMultilen1,
					int nMultilen2,
					const char *szMsg = NULL, 
					BOOL bAsciiToUS2 = FALSE
					);
	
	//计算纯英文ASCII短信条数
static	int CalcEnMsgNum_Global(const char* szEnMsg/*ASCII串短信(含签名)*/, int nSingleLen/*单条长度(含签名)*/, int nMultilen1/*分条1长度(含签名(若有))*/, int nMultilen2/*分条2长度(含签名(若有)*/);
		
		//计算纯英文ASCII短信长度(按gsm 7-bit编码中对符串字节数的要求进行计算)
static   int CalcEnMsgLen_Global(const char* szEnMsg);

static bool IsSpecialAscii(char cAscii);
	//************************************
	// Description:根据指定的拆分规则，计算该条短信要被拆成几条
	// Parameter: [IN] int nMsgLen			: 短信长度
	//			  [IN] int nSplitLen		: 拆分长度,超过nsplitlen将被拆分
	//			  [IN] const char *szMsg	: 短信内容，若该内容是us2编码，则无需输入只填nMsgLen即可
	//			  [IN] BOOL bAsciiToUS2		: 若短信是ASCII编码，需传入szMsg并将该项置为TRUE
	// Returns:   int						返回拆分条数
	//************************************
	static int	CalcMsgNum(
					int nMsgLen, 
					int nSplitLen,
					const char *szMsg = NULL,
					BOOL bAsciiToUS2 = FALSE
					);

	//************************************
	// Description:根据指定的拆分规则，取总条数中的某一条
	// Parameter: [IN] const unsigned char *szInMsg	:本次要拆的短信内容
	//			  [OUT]char *szOutMsg		: 输出拆分后该条的内容(已解码)
	//			  [IN] int nMultilen1		: 长短信第一条的长度
	//			  [IN] int nMultilen2		: 长短信最后一条的长度
	//			  [IN] int nPkNum			: 要第几条
	//			  [IN] int nPkTotal			: 总共几条
	//			  [IN] int nPreMsgLen		: 上次返回的长度
	//			  [IN] int nMsgLen			: 本次输入内容的长度(若内容中带有签名，需减去签名的长度=nMsgLen-nSignLen)
	//			  [IN] BOOL bUS2ToAscii		: 是否要解码(仅对编码方式是8的),无需编码时,输入字符需为宽字符,长度需宽字符串长度
	//			  [IN] INT nSignLen			: 签名的长度(字数*2)，若bUS2ToAscii=false则该参数需填0，若bUS2ToAscii=true则可以为0也可以为实际值
	// Returns:   int						返回拆分条数
	//************************************
	static int	SplitMsgEx(
					const unsigned char *szInMsg, 
					char *szOutMsg, 
					int nMultilen1,
					int nMultilen2,
					int nPkNum, 
					int nPkTotal, 
					int nPreMsgLen, 
					int nMsgLen, 
					BOOL bUS2ToAscii = TRUE,
					int nSignLen = 0
					);

	//************************************
	// Description:以固定长度均分一条短信
	// Parameter: [IN]  CString Msg			: 本次要拆的短信内容
	//			  [OUT] char *szOutMsg		: 输出拆分后该条的内容(已解码)
	//			  [OUT] char *Msg1			: 本次拆的内容
	//			  [OUT] char *Msg2			: 还余下的内容
	// Returns:   int						>0:成功 <0:失败
	//************************************
	//static int	SplitMsg(CString Msg, int nSplitLen, char *Msg1, char *Msg2);
	//zhubo 2017-4-11 增加static
	static int SplitMsg(const char* Msg, int nSplitLen, char *Msg1, char *Msg2);
	
	//************************************
	// Description:生成序列ID
	// Parameter: void
	// Returns:   int 返回生成的序列ID
	//************************************
	int		GeneranteSeqID();

	//************************************
	// Description:将INT64的MsgId转换成8字节的char
	// Parameter: [OUT]unsigned char* MsgId : 转换后的char型MsgId
	//			  [IN] __int64 IMsgId		: INT64型MsgId
	// Returns:   void
	//************************************
	static void	TranMsgIdI64ToChar(unsigned char* MsgId, __int64 IMsgId);

	//************************************
	// Description:将以MMDDHHMMSS+网关代码+序列号格式的字符串转成为8字节字符串
	// Parameter: [OUT]unsigned char* MsgId : 转换后的8字节字符串
	//			  [IN] char *pSrc			: MMDDHHMMSS+网关代码+序列号格式的字符串
	// Returns:   void
	//************************************
	static void	MakeMsgId(unsigned char *MsgId, char *pSrc);

	//************************************
	// Description:由时间+网关代码+序列号+步长生成8字节MsgId
	// Parameter: [OUT]unsigned char* MsgId : 生成的8字节字符串
	//			  [IN] unsigned int Num		: 要生成的MsgId数目
	//			  [IN] unsigned int Gate	: 网关编号
	// Returns:   void
	//************************************
	void	MakeMsgId(unsigned char *MsgId, unsigned int Num = 1, unsigned int Gate = 1);

	
	void	MakeMsgId_qc(unsigned char *MsgId, unsigned int Num = 1, unsigned int Gate = 1);

	//************************************
	// Description:将由时间+网关代码+序列号+步长生成8字节MsgId，转换成原始的格式MMDDHHMMSS+网关代码+序列号格式的字符串
	// Parameter: [IN] unsigned char *pSrc : 8字节的MsgId字符串
	//			  [OUT]char *MsgId		   : MMDDHHMMSS+网关代码+序列号格式的字符串
	// Returns:   int					   0:成功 非0:失败
	//************************************
	static int	TranMsgId(unsigned char *pSrc, char *MsgId);

	//************************************
	// Description:将8字节MsgId中的时间解析出来
	// Parameter: [OUT]char *szTime			: MsgId生成的时间
	//			  [IN] const unsigned char *pSrc : 8字节MsgId
	// Returns:   void
	//************************************
	static void	GetTimeInMsgId(char *szTime, const unsigned char *pSrc);
	static time_t  GetTimeInMsgId(const unsigned char *pSrc);
	static time_t string2time(const char * str,const char * formatStr);

	//该函数已不用
	void	UnMakeMsgId(char *MsgId, const unsigned char *pSrc);

	//************************************
	// Description:将8字节MsgId转换成INT64的整型数
	// Parameter: [IN] const unsigned char *szMsgId : 8字节MsgId
	// Returns:   __int64	整型MsgId
	//************************************
	static __int64 TranMsgIdCharToI64(const unsigned char *szMsgId);

	//************************************
	// Description:取该SrcId下,长短信第PkNum条的MsgId(该函数针对该MsgId为长短信的首MsgId而设计)
	// Parameter: [IN] __int64 SrcId		: 首MsgId
	//			  [IN] int PkNum			: 要取第几条的MsgId
	// Returns:   __int64					整型MsgId
	//************************************
	static __int64 GetI64MsgIdAt(__int64 SrcId, int PkNum);
		
	//************************************
	// Description:Base64加密
	// Parameter: [IN] LPBYTE pbSrc			: 源字符串
	//			  [IN] DWORD SrcLen			: 源字符串长度
	//			  [OUT]LPBYTE pbDes			: 加密后字符串
	// Returns:   int						返回加密后字串长度
	//************************************
	static int		Base64Encode(LPBYTE pbSrc, DWORD SrcLen, LPBYTE pbDes); 

	//************************************
	// Description:Base64解密
	// Parameter: [IN] LPBYTE pbSrc			: 源字符串
	//			  [IN] DWORD SrcLen			: 源字符串长度
	//			  [OUT]LPBYTE pbDes			: 解密后字符串
	// Returns:   int						返回解密后字串长度
	//************************************
	static int		Base64Decode(LPBYTE pbSrc, DWORD SrcLen, LPBYTE pbDes); 

	static int		Base64Decode(string& strIn,string& strOut); 

	//************************************
	// Description:将DATE转化为时间格式
	// Parameter: [IN] DATE dt
	// Returns:   CString 时间字符串						
	//************************************
	//static CString	DateToTimeStr(DATE dt);

	//对soap封装消息按命令字解析各标签对应的内容
	static int GetCmdValue(char *Data, char *CmdItem, int MaxSize, char *CmdVal, BOOL bUTF8=true);

	//对HTTP-post/get模式的消息进行解析，解析出各命令字所对应的内容
	static int GetCmdValueEx(char *Data, char *CmdItem, int MaxSize, char *CmdVal, BOOL bConvert=true);
	static int GetCmdValueExEx(char *Data, char *CmdItem, int MaxSize, char *CmdVal);


	//解析JAVA网页编码
	static void ParaseXmlUTF8(string& strIn,string& strOut);
	static void ParaseXmlUtf8ToGBk(char *szDest, const char *szSrc, int nInOutLen, int nMaxOutLen);
	static void ParaseJavaEncode(char *szDest, char *szSrc, int& nInOutLen, int nDestMaxLen);
	static	int HexchartoInt_ex(char chHex);

	//解析网页编码
	static void DecodeHtmlStr(char *szDest, char *szSrc, int& nInOutLen);
	//multisend 专用
	static void DecodeHtmlStrEx(char *szDest, char *szSrc, int& nInOutLen);
	static void DecodeHtmlStr(string& strIn,string& strOut);
	//multisend 专用
	static void DecodeHtmlStrEx(string& strIn,string& strOut);
	

	//将以','分隔的字符串拆分后放到容器中返回拆分的段数 nMaxLen为每段的最大长度
	static int  SplitStr(const char *szInStr, std::vector<std::string>& vOutStr, int nMaxLen=32);

	static int SplitStrEx(const char *szInStr, std::vector<std::string> &vOutStr, const char* c, int nMaxLen);

	static int SplitString(const string& s, vector<string>& v, const string& c);
	
	//************************************
	// Description:校验短信的有效性(长度/不可见字符等)
	// Parameter: [IN] int nMsgFmt:短信编码方式
	//			  [IN] int nMaxLen:允许的最大长度
	//			  [IN] int nMsgLen:短信的长度
	//			  [IN] /*const */char *szMsg:短信的内容
	// Returns:   int  0:成功(如有不可见字符等，会修改短信的内容，去掉不可见字符，并返回新的短信长度) 6:超长 
	//************************************
	static int	CheckMsgValidity(int nMsgFmt, int nMaxLen, USHORT& nMsgLen, /*const */char *szMsg, bool bMo=false);
	static int  CheckMsgValidity(int nMsgFmt, int nMaxLen, string& strContent);

	static int TrimUnicodeStrRight(string& strUnicodeStr);

	//计算短信长度
	static int  CalcMsgLen(int nMsgFmt, int nMsgLen, const char *szInMsg);

	//由Url取得IP和端口  http://192.169.1.130:8081/sms/mt  ip=192.169.1.130 port=8081 webpath=/sms/mt
	static int  GetIpFromUrl(const char *szUrl, char *szWebPath, char *szIp, char* szDomain, int& nPort);
    static int  GetIpFromHttps(const char *szUrl, char *szWebPath, char *szIp, char* szDomain, int& nPort);
	static int  GetHostFromUrl(char* szUrl,char* pHost,int nMaxHostLen);

	//十六进制字符转换成整数
	static int  HexchartoInt(char chHex);
	static int	TransMsg(char *szDest, const char *szSrc, int nMsgFmt, int nSrcLen, int nMaxSize);

	//内存字节转换成HEX字串
	static int  MemBytesToHexStr(const unsigned char *szSrc, int nLen, char *szDest, bool bLowFlag = false);
	//HEX字串转换成内存字节
	static int	HexStrToMemBytes(unsigned char *szDest, const char *szSrc, int nSrcLen, int nDestSize);
	//替换函数将szSrc中的cSrc替换为cDest
	static void	ReplaceChar(unsigned char *szSrc, char cSrc, char cDest, unsigned int nSrcLen);

	//************************************
	// Description:取用户特权属性值(00000000000000000000000000000000)32位，第一位表示自编MsgId权限，第二位表示是否是代理帐号
	// Parameter: [IN] int nPrivilege		:特权码整型值从低位到高位以01值来表示该位所对应的属性是否存在
	//			  [IN] int nBit				:第几位的属性
	// Returns:   int  第nBit位的属性值  
	//************************************
	static int	GetUserPrivilege(int nPrivilege, int nBit);

	//控制蜂鸣报警
	static void BeepEx(DWORD dwFreq, DWORD dwDuration);

	static int GetValidTime(const char* szMsgId, const char* szDoneTime, char* szTime/*in/out "YYYY-MM-DD HH-MM-SS"*/);
	
	static bool BeepOrNot;

	//替换函数
	static void replace(std::string& str, std::string olds, std::string news);

	//时间函数
	static std::string date(std::string fmt = "Y-m-d H:i:s", int nday = 0);
	
	// added by youbiyun 2013-07-18
	// 去掉左右空格
	static void trimLR(std::string &strSrc);
	
	static void trimLR(char *szSrc);
	
	static	char* strstri(const char* pString, const char* pFind);
	
	/*
	UrlEncode （百分号编码）。方法是把输入的字符串先用 UTF-8 编码，
	然后把基本字符以外的字节用百分号加16进制的形式编码。UrlEncode 
	的最后一个参数含义是，编码结果中的16进制字符是否采用大写字母表示。
	*/
	//百分号编码
	//http://zh.wikipedia.org/zh-cn/%E7%99%BE%E5%88%86%E5%8F%B7%E7%BC%96%E7%A0%81
	static BOOL UrlEncode(char* szSrc, char* pBuf, int cbBufLen, BOOL bUpperCase);
	static std::string UrlEncode(const std::string& src);
	static BOOL UrlDecode(char* szSrc, char* pBuf, int cbBufLen);

	static unsigned int IpToInt(const char* ipStr);
	static bool  IsIpInner(unsigned int clientIp, unsigned int begin, unsigned int end);

	/**
	* 取一个一天内的随机时间，包括时分秒
	* @param  {int} nBeginHour : 随机的起始时
	* @param  {int} nBeginMin : 随机的起始分
	* @param  {int} nBeginSec : 随机的起始秒
	* @param  {int} nMaxHour : 限制最大时,0默认不限
	* @param  {int} nMaxMinu : 限制最大分,0默认不限
	* @param  {int} nMaxSec : 限制最大秒,0默认不限
	* @return {int} 返回整的秒数
	*/
	static int GetRandomTime(int nBeginHour, int nBeginMin, int nBeginSec,
		int nMaxHour = 0, int nMaxMinu = 0, int nMaxSec = 0);

	/**
	* 判断当前时间是否大于等于输入的当天时间
	* @param  {int} nDaySec : 输入的当天时间整秒数
	* @return {bool} 是否超过或等于
	*/
	static bool bInOrOutTime(int nDaySec);
    
    // add by dengyu 2017-4-28
    static bool IsAllDigital(const char* str);
    
    // add by dengyu 2017-5-25
    static DATE StrToDATE(const char* strTime);

	static void string_replace(string&s1, const string&s2, const string&s3);

	static bool IsEnglishMsg(const char * szMsg, int nLen);

	//生成自增的SQEID
	static int MakeSeqId();

private:

	
	//该函数已不用
	void	GenernateMsgIDEx(unsigned char *szMsgID, int nUId);

	//************************************
	// Description:把一个2进制表示的字符串转化为整数
	// Parameter: [IN] char *szBuf			: 二进制字符串
	// Returns:   int						二进制字符串对应的整数
	//************************************
	int		BinStr2Num(char *szBuf);

	//************************************
	// Description:生成内部指定步长的序号
	// Parameter: [IN] int Num				: 步长
	// Returns:   int						序号
	//************************************
	int		GetMsgId(int Num);
	int 	GetMsgId(int Num, bool &bFlow);

private:
	DerivedLock cs_MsgId;
};
#endif
