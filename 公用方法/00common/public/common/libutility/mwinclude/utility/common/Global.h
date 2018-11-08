#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#pragma warning (disable:4267)
#pragma warning (disable:4996)

#include <vector>
#include <algorithm>
#include <string>
#include <deque>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include "utility/common/SafeLock.h"
#include "utility/common/DerivedLock.h"
#include "utility/common/adapter/Adapter.h"
#include "utility/common/mwcore_export.h"
#include "utility/common/eatool.h"

extern int UTILITY_Export MIN2(int a, int b);
//格式化%I64d %lld
extern std::string UTILITY_Export FORMATI64D(const char* format);
//格式化%I64d %lld
extern std::string UTILITY_Export FORMATI64D(const char* format);

class UTILITY_Export cstr
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
/*
    static std::string to_upper(char *str)
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
        return str;
    }
// */
//     //字符串格式化函数
//     static std::string format(const char *fmt, ...)
//     {
// 		if (NULL == fmt)
// 		{
// 			return "";
// 		}
//         std::string strResult = "";
//         va_list marker = { 0 };
//         va_start(marker, fmt);								//初始化变量参数
//         size_t nLength = 1024*8+1;							//获取格式化字符串长度
//         std::vector<char> vBuffer(nLength, '\0');			//创建用于存储格式化字符串的字符数组
// 		MARK:int nWritten = _vsnprintf(&vBuffer[0], vBuffer.size(), fmt, marker);
//         //当nwritten不小于0时返回的格式化到vbuffer中的数据长度,当小于0时说明出错,等于-1时说明长度不够,此时将重新分配空间,然后再做格式化
// 		if (nWritten >= 0)
//         {
//             strResult = &vBuffer[0];
//         }
// 		//vbuffer长度不够,扩展nlength/2的长度
// 		else if (-1 == nWritten)
// 		{
// 			vBuffer.erase(vBuffer.begin(), vBuffer.end());
// 			nLength += nLength;
// 			vBuffer.resize(nLength, '\0');
// 			//最多分配1G的空间,超过1G将不再处理
// 			if (nLength < 1024*1024*1024)
// 			{
// 				goto MARK;
// 			}
// 		}
//         va_end(marker);                                    //重置变量参数
//         return strResult;
//     }
// 
//     //字符串格式化函数
//     static std::string format_s(size_t maxsize, const char *fmt, ...)
//     {
// 		if (NULL == fmt)
// 		{
// 			return "";
// 		}
//         std::string strResult = "";
//         va_list marker = NULL;
//         va_start(marker, fmt);								//初始化变量参数
//         size_t nLength = maxsize+1;							//获取格式化字符串长度
//         std::vector<char> vBuffer(nLength, '\0');			//创建用于存储格式化字符串的字符数组
//         int nWritten = _vsnprintf(&vBuffer[0], vBuffer.size(), fmt, marker);
//         //当nwritten不小于0时返回的格式化到vbuffer中的数据长度,当小于0时说明出错,此时返回空字符串
// 		if (nWritten > 0)
//         {
//             strResult = &vBuffer[0];
//         }
//         va_end(marker);                                    //重置变量参数
//         return strResult;
//     }

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
                        right(str, nLen-nTargetsLen);
                        i = 0;
                        j = 0;
                        nLen = str.size();
                    }
                }
            }
        }
    }
};

class UTILITY_Export CSpeedLimit
{
public:
	CSpeedLimit();
	virtual ~CSpeedLimit();
	//限速，并返回需要延时的值(毫秒)
	int  LimitSpeed(int nCount);
	//设置需要限制到的速度值
	void SetMaxSpeed(int nMaxSpd);
	//获取设置的最大速度
	int	 GetMaxSpeed();
private:
	//计数
	long m_nCount;
	int m_nPrecision; 
	int m_nMaxSpeed;
	int m_nLastTm;
};

class UTILITY_Export CGlobal
{
public:
    CGlobal();

    //************************************
    // Description:获取带','字符串中每一节的内容
    // Parameter: [IN] const char *szStr    : 原始字符串
    //            [IN] int nPos             : 逗号的位置
    // Returns:   std::string               : 返回以逗号分隔的字符串
    //************************************
    static std::string GetSubStr(const char *szStr, int nPos);

	//获取带','字符串中第一节的内容
	static std::string GetSubStrFirst(const char *szStr);

	static int JavaEnCode(const unsigned char* szSrcMsg, int nSrcMsgLen, char* szOutMsg)
	{
		int nLen = 0;
		for (int i = 0; i < nSrcMsgLen; i+=2)
		{
			nLen += sprintf(szOutMsg+nLen, "&#x%02X%02X;", szSrcMsg[i], szSrcMsg[i+1]);
		}
		return nLen;
	}
    //************************************
    // Description:将内存字符串转为ASCII字符串
    // Parameter: [IN] const char *szSrc    : 原始字符串
    //            [IN] int nLen             : 原始字符串长度
    //            [OUT]char *szDest         : ASCII串
    // Returns:   void
    //************************************
    static int  CharToASCII(const char *szSrc, int nLen, char *szDest, int nMaxDestLen);
    static int  CharToASCIIEx(const char *szSrc, int nLen, char *szDest);

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

    //************************************
    // Description:将XML文档中&gt &lt转换成"<>"
    // Parameter: [IN/OUT]char *buf         : 输入原始字符串,返回转换后的字符串
    // Returns:   int                       0:成功  非0:失败
    //************************************
    static int  TranHttpSpace(char *buf);

    //************************************
    // Description:根据指定的拆分规则，计算该条短信要被拆成几条
    // Parameter: [IN] int nMsgLen          : 短信长度
    //            [IN] int nSplitLen        : 拆分长度,超过nsplitlen将被拆分
    //            [IN] int nMultilen1       : 长短信第一条的长度
    //            [IN] int nMultilen2       : 长短信最后一条的长度
    //            [IN] const char *szMsg    : 短信内容，若该内容是us2编码，则无需输入只填nMsgLen即可
    //            [IN] BOOL bAsciiToUS2     : 若短信是ASCII编码，需传入szMsg并将该项置为TRUE
    // Returns:   int                       返回拆分条数
    //************************************
    static int  CalcMsgNum(
                    int nMsgLen,
                    int nSplitLen,
                    int nMultilen1,
                    int nMultilen2,
                    const char *szMsg = NULL,
                    bool bAsciiToUS2 = false
                    );

    //************************************
    // Description:根据指定的拆分规则，取总条数中的某一条
    // Parameter: [IN] const unsigned char *szInMsg :本次要拆的短信内容
    //            [OUT]char *szOutMsg       : 输出拆分后该条的内容(已解码)
    //            [IN] int nMultilen1       : 长短信第一条的长度
    //            [IN] int nMultilen2       : 长短信最后一条的长度
    //            [IN] int nPkNum           : 要第几条
    //            [IN] int nPkTotal         : 总共几条
    //            [IN] int nPreMsgLen       : 上次返回的长度
    //            [IN] int nMsgLen          : 本次输入内容的长度(若内容中带有签名，需减去签名的长度=nMsgLen-nSignLen)
    //            [IN] BOOL bUS2ToAscii     : 是否要解码(仅对编码方式是8的),无需编码时,输入字符需为宽字符,长度需宽字符串长度
    //            [IN] INT nSignLen         : 签名的长度(字数*2)，若bUS2ToAscii=false则该参数需填0，若bUS2ToAscii=true则可以为0也可以为实际值
    // Returns:   int                       返回拆分条数
    //************************************
    static int  SplitMsgEx(
                    const unsigned char *szInMsg,
                    char *szOutMsg,
                    int nMultilen1,
                    int nMultilen2,
                    int nPkNum,
                    int nPkTotal,
                    int nPreMsgLen,
                    int nMsgLen,
                    bool bUS2ToAscii = true,
                    int nSignLen = 0
                    );

    //************************************
    // Description:以固定长度均分一条短信
    // Parameter: [IN]  const char* Msg     : 本次要拆的短信内容
    //            [OUT] char *szOutMsg      : 输出拆分后该条的内容(已解码)
    //            [OUT] char *Msg1          : 本次拆的内容
    //            [OUT] char *Msg2          : 还余下的内容
    // Returns:   int                       >0:成功 <0:失败
    //************************************
    static int  SplitMsg(const char* Msg, int nSplitLen, char *Msg1, char *Msg2);

    //************************************
    // Description:生成序列ID
    // Parameter: void
    // Returns:   int 返回生成的序列ID
    //************************************
    static int   GeneranteSeqID();

    //************************************
    // Description:将INT64的MsgId转换成8字节的char
    // Parameter: [OUT]unsigned char* MsgId : 转换后的char型MsgId
    //            [IN] __int64 IMsgId       : INT64型MsgId
    // Returns:   void
    //************************************
    static void TranMsgIdI64ToChar(unsigned char* MsgId, INT64 IMsgId);

    //************************************
    // Description:将以MMDDHHMMSS+网关代码+序列号格式的字符串转成为8字节字符串
    // Parameter: [OUT]unsigned char* MsgId : 转换后的8字节字符串
    //            [IN] char *pSrc           : MMDDHHMMSS+网关代码+序列号格式的字符串
    // Returns:   void
    //************************************
    static void MakeMsgId(unsigned char *MsgId, char *pSrc);

    //************************************
    // Description:由时间+网关代码+序列号+步长生成8字节MsgId
    // Parameter: [OUT]unsigned char* MsgId : 生成的8字节字符串
    //            [IN] unsigned int Num     : 要生成的MsgId数目
    //            [IN] unsigned int Gate    : 网关编号
    // Returns:   void
    //************************************
    static void MakeMsgId(unsigned char *MsgId, unsigned int Num = 1, unsigned int Gate = 1);

	/*
	* 作者姓名：jianghb
	* 函数介绍：由用户传递的流水号生成 由时间+节点编号(目前只支持1-64)+序列号 组成的8字节MsgId
	* 定义的参数含义说明
	* 输入参数：
				1./in/unsigned int SrcNode					//节点编号
				2./in/unsigned int TimeStamp				//时间	
				3./in/unsigned int SeqId					//序号
	* 输出参数：
				1./out/unsigned char *MsgId					//新组成的流水号
	* 返 回 值：void
	*/
	static void	MakeSgipMsgId(/*out*/unsigned char *MsgId, /*in*/unsigned int SrcNode, /*in*/unsigned int TimeStamp, /*in*/unsigned int SeqId);

	/*
	* 作者姓名：jianghb
	* 函数介绍：反解sgip流水号
	* 定义的参数含义说明
	* 输入参数：
				1./in/const unsigned char *MsgId				//流水号
	* 输出参数：
				1./in/out/unsigned int *pSrcNode				//节点编号
				2./in/out/unsigned int *pTimeStamp				//时间	
				3./in/out/unsigned int *pSeqId					//序号
	* 返 回 值：void
	*/
	static void	UnMakeSgipMsgId(/*in*/const unsigned char *MsgId, /*out/in*/unsigned int *pSrcNode, /*out/in*/unsigned int *pTimeStamp, /*out/in*/unsigned int *pSeqId);

    //************************************
    // Description:将由时间+网关代码+序列号+步长生成8字节MsgId，转换成原始的格式MMDDHHMMSS+网关代码+序列号格式的字符串
    // Parameter: [IN] unsigned char *pSrc : 8字节的MsgId字符串
    //            [OUT]char *MsgId         : MMDDHHMMSS+网关代码+序列号格式的字符串
    // Returns:   int                      0:成功 非0:失败
    //************************************
    static int  TranMsgId(unsigned char *pSrc, char *MsgId);

    //************************************
    // Description:将8字节MsgId中的时间解析出来
    // Parameter: [OUT]char *szTime         : MsgId生成的时间
    //            [IN] const unsigned char *pSrc : 8字节MsgId
    // Returns:   void
    //************************************
    static void		GetTimeInMsgId(char *szTime, const unsigned char *pSrc);
    static time_t   GetTimeInMsgId(const unsigned char *pSrc);
	static time_t	GetTimeInMsgId(INT64 nMsgId);
	static time_t	GetTimeInMsgId(const unsigned char *pSrc, INT64 nSdTm);
	//从流水号获取该流水号相对于今天是哪一天的 0:今天 1:昨天 2:前天 >2:大前天
	static int		GetRptDayFromMsgId(const unsigned char *pSrc);
	static bool		IsMsgIdInToday(const unsigned char *pSrc);
	//判断是否是今天发送的MT的RPT
	static bool		IsTodayMtRpt(const unsigned char *pSrc, INT64 nSdTm);
	//判断是否是成功的RPT
	static bool		IsSuccessRpt(const char* pErCode);
	//判断是否是内部拒绝的RPT
	static bool		IsInnerErrRpt(const char* pErCode);
	//判断是否是过期的RPT
	static bool		IsTimeOutRpt(const unsigned char* pMsgId, TINYINT nTimeOut);

    //该函数已不用
    void    UnMakeMsgId(char *MsgId, const unsigned char *pSrc);

    //************************************
    // Description:将8字节MsgId转换成INT64的整型数
    // Parameter: [IN] const unsigned char *szMsgId : 8字节MsgId
    // Returns:   __int64   整型MsgId
    //************************************
    static INT64 TranMsgIdCharToI64(const unsigned char *szMsgId);

    //************************************
    // Description:取该SrcId下,长短信第PkNum条的MsgId(该函数针对该MsgId为长短信的首MsgId而设计)
    // Parameter: [IN] __int64 SrcId        : 首MsgId
    //            [IN] int PkNum            : 要取第几条的MsgId
    // Returns:   __int64                   整型MsgId
    //************************************
    static INT64 GetI64MsgIdAt(INT64 SrcId, int PkNum);

    //************************************
    // Description:Base64加密
    // Parameter: [IN] LPBYTE pbSrc         : 源字符串
    //            [IN] DWORD SrcLen         : 源字符串长度
    //            [OUT]LPBYTE pbDes         : 加密后字符串
    // Returns:   int                       返回加密后字串长度
    //************************************
    static int      Base64Encode(UCHAR* pbSrc, DWORD SrcLen, UCHAR* pbDes);

    //************************************
    // Description:Base64解密
    // Parameter: [IN] LPBYTE pbSrc         : 源字符串
    //            [IN] DWORD SrcLen         : 源字符串长度
    //            [OUT]LPBYTE pbDes         : 解密后字符串
    // Returns:   int                       返回解密后字串长度
    //************************************
    static int      Base64Decode(UCHAR* pbSrc, DWORD SrcLen, UCHAR* pbDes);

    //************************************
    // Description:将DATE转化为时间格式  linux下暂不支持
    // Parameter: [IN] DATE dt
    // Returns:   std::string 时间字符串
    //************************************
    static std::string DateToTimeStr(DATE dt);

    //对soap封装消息按命令字解析各标签对应的内容
    static int GetCmdValue(char *Data, char *CmdItem, int MaxSize, char *CmdVal, bool bUTF8=true);

    //对HTTP-post/get模式的消息进行解析，解析出各命令字所对应的内容
    static int GetCmdValueEx(char *Data, char *CmdItem, int MaxSize, char *CmdVal, bool bConvert=true);

    //解析JAVA网页编码
    static void ParaseJavaEncode(char *szDest, char *szSrc, int& nInOutLen);

    //解析网页编码
    static void DecodeHtmlStr(char *szDest, char *szSrc, int& nInOutLen);

    //将以','分隔的字符串拆分后放到容器中返回拆分的段数 nMaxLen为每段的最大长度 
	//2013-08-19 将nMaxLen在函数内部定死1024字节(为了提高效率),使用时需要格外注意长度问题
    static int  SplitStr(const char *szInStr, std::vector<std::string>& vOutStr, const char* pObj, int nMaxLen = 1024);
    static int  SplitStr(const char *szInStr, std::vector<std::string>& vOutStr, int nMaxLen=1024);
    static int  SplitStr(const char *szInStr, const char* szSign, std::vector<std::string>& vOutStr, int nMaxLen = 1024);
    static int  SplitRptMoStr(const char *szInStr, int nLen, std::vector<std::string>& vOutStr);

    //************************************
    // Description:校验短信的有效性(长度/不可见字符等)
    // Parameter: [IN] int nMsgFmt:短信编码方式
    //            [IN] int nMaxLen:允许的最大长度
    //            [IN] int nMsgLen:短信的长度
    //            [IN] /*const */char *szMsg:短信的内容
    // Returns:   int  0:成功(如有不可见字符等，会修改短信的内容，去掉不可见字符，并返回新的短信长度) 6:超长
    //************************************
    static int  CheckMsgValidity(int nMsgFmt, int nMaxLen, USHORT& nMsgLen, /*const */char *szMsg, bool bTransHex=true, bool bMo=false);

	//去掉字符串中的乱码,以防止写库错误
	//2013-08-19 将nMaxLen在函数内部定死1024字节(为了提高效率),使用时需要格外注意长度问题
	static int TrimGarbledCharacter(char *szSrc, int nLen, int nMaxLen);

	//trim指定的字符
	static UINT TrimRight(char *szSrc, UINT nLen, char ch);

	//trim网关系统内不愿接受的字符
	static UINT TrimRight(char *szSrc, UINT nLen);

    //计算短信长度
    static int  CalcMsgLen(int nMsgFmt, int nMsgLen, const char *szInMsg);

    //由Url取得IP和端口  http://192.169.1.130:8081/sms/mt  ip=192.169.1.130 port=8081 webpath=/sms/mt
	static int  GetIpFromUrl(const char *szUrl, char *szWebPath, char *szIp, char* szDomain, int& nPort);
	
	static BOOL InitSocket();
	static BOOL UnInitSocket();

    //十六进制字符转换成整数
    static int  HexchartoInt(char chHex);
    static int  TransMsg(char *szDest, const char *szSrc, int nMsgFmt, int nSrcLen, int nMaxSize);

    //将时间转化为long
    static long StrTimeToLong(const char* szTime);

    //内存字节转换成HEX字串
    static int  MemBytesToHexStr(const unsigned char *szSrc, int nLen, char *szDest);
    //HEX字串转换成内存字节
    static int  HexStrToMemBytes(unsigned char *szDest, const char *szSrc, int nSrcLen, int nDestSize);
    //替换函数将szSrc中的cSrc替换为cDest
    static void ReplaceChar(unsigned char *szSrc, char cSrc, char cDest, unsigned int nSrcLen);
	//使用该函数时请自行确保szSrc的内存大小足够用
	static void ReplaceChar(unsigned char *szSrc, char cSrc, char* szDest, unsigned int nSrcLen);
    
	//************************************
    // Description:取用户特权属性值(00000000000000000000000000000000)32位，第一位表示自编MsgId权限，第二位表示是否是代理帐号
    // Parameter: [IN] int nPrivilege       :特权码整型值从低位到高位以01值来表示该位所对应的属性是否存在
    //            [IN] int nBit             :第几位的属性
    // Returns:   int  第nBit位的属性值
    //************************************
    static int  GetUserPrivilege(int nPrivilege, int nBit);

	//置一个整型数某一位为1或0
	static void SetBitValue(int& nValue, int nBit, bool bBit);

	//置一个整型数某一位为1
	static void SetBit(int& nValue, int nBit);

	//置一个大整型数某一位为1
	static void SetInt64Bit(INT64& nValue, int nBit);

	//取一个大整型某一位是否为1
	static bool GetInt64Bit(INT64 nValue, int nBit);

	//置一个整型数某一位为0
	static void ClrBit(int& nValue, int nBit);

	//取一个整数某一位是否为1
	static bool GetBit(int nValue, int nBit);

	//判断是否为汉字
	static bool IsZHCN(const char* str);

    //控制蜂鸣报警
    static void BeepEx(DWORD dwFreq, DWORD dwDuration);

	//使系统时间有效，若系统时间没问题格式化成想要的时间，若有问题则取当前时间
	static void MakeSysTmValid(const char* szInTm, char* szOutTm);

	//使写库短信有效，将编码8转换为15，并做结尾去'处理，将短信内容中的单引号替换为双引号
	static void MakeMsgValid(const char* szInMsg, char* szOutMsg, int nMsgFmt, int nInLen, int nMaxOutSize, bool bConvertFmt=true, bool bTrimQuote=true);

	//将手机号列表转化为协议需的destid
	static void	PhoneListToDestId(char *szDest, const char *szSrc, UINT nSrcLen, UINT nFixDestIdLen=21);

	//判断字符串是否全是数字
	static bool IsAllDigital(const char* str);

	//判断字符串是否全是数字字母
	static bool IsAllAlpNum(const char* str);

	//判断编码是否是可识别编码
	static bool IsUnKnownCode(int nMsgFmt);

	//生成MT扩展参数,返回扩展参数及其长度
	static int  GenMtExParam(const char* szMsgSrc/*发送帐号*/, const char* szSvrType/*业务类型*/, const char* szVerifyCode/*验证码*/, char* szExParam/*最小需预留32字节*/);

	//校验MT扩展参数
	static bool CheckMtExParam(const char* szExParam, const char* szMsgSrc, const char* szSvrType, int nVerifyCode);

	//由流水号和外部时间来获取一个合理的时间
	static int  GetValidTime(const char* szMsgId, const char* szDoneTime, char* szTime);

	//判断是否在一个时间段内
	static bool IsInSpan(DATE dt1, DATE dt2, DATE dt);

	//将一定的毫秒分成几段来延时
	static void Delay(int nDelayTm/*毫秒*/);

    static bool BeepOrNot;
private:
    //该函数已不用
    void    GenernateMsgIDEx(unsigned char *szMsgID, int nUId);

    //************************************
    // Description:把一个2进制表示的字符串转化为整数
    // Parameter: [IN] char *szBuf          : 二进制字符串
    // Returns:   int                       二进制字符串对应的整数
    //************************************
    int     BinStr2Num(char *szBuf);

    //************************************
    // Description:生成内部指定步长的序号
    // Parameter: [IN] int Num              : 步长
    // Returns:   int                       序号
    //************************************
    static int GetMsgId(int Num);
    static int GetMsgId(int Num, bool &bFlow);
};
#endif
