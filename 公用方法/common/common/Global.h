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
    
    //�ַ�����ʽ������
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

    //�ַ�����ʽ������
    //zhubo 2017-4-7
  
    static std::string format_s(size_t maxsize, const char *fmt, ...)
    {
		if (NULL == fmt)
		{
			return "";
		}
        std::string strResult = "";
        va_list marker;
        va_start(marker, fmt);								//��ʼ����������
        size_t nLength = maxsize+1;							//��ȡ��ʽ���ַ�������
        std::vector<char> vBuffer(nLength, '\0');			//�������ڴ洢��ʽ���ַ������ַ�����
        int nWritten = _vsnprintf(&vBuffer[0], vBuffer.size(), fmt, marker);
        //��nwritten��С��0ʱ���صĸ�ʽ����vbuffer�е����ݳ���,��С��0ʱ˵������,��ʱ���ؿ��ַ���
		if (nWritten > 0)
        {
            strResult = &vBuffer[0];
        }
        va_end(marker);                                    //���ñ�������
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
			//szOld��Ϊ���ַ�����szOld��szNew����ͬ
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
    //ȥ�ҿո�
    static void trimright(std::string& str, const char* szTargets=NULL)
    {
        int nLen = str.size();
        int nTargetsLen = 0;
        //Ĭ��ֻȥ�ո�
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
    //ȥ��ո�
    static void trimleft(std::string& str, const char* szTargets=NULL)
    {
        int nLen = str.size();
        int nTargetsLen = 0;
        //Ĭ��ֻȥ�ո�
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
	// Description:���ڴ��ַ���תΪASCII�ַ���
	// Parameter: [IN] const char *szSrc	: ԭʼ�ַ���
	//			  [IN] int nLen				: ԭʼ�ַ�������
	//		      [OUT]char *szDest			: ASCII��
	// Returns:   void 
	//************************************
	static int	CharToASCII(const char *szSrc, int nLen, char *szDest);
	static int  CharToASCIIEx(const char *szSrc, int nLen, char *szDest);

	//************************************
	// Description:��US2�ַ���תΪASCII�ַ���
	// Parameter: [IN] const char *InMsg	: ԭʼ�ַ���
	//			  [IN] int InLen			: ԭʼ�ַ�������
	//		      [OUT]char *Data			: ASCII��
	//			  [OUT]int *Len				: ASCII������
	// Returns:   int						0:�ɹ�  ��0:ʧ��
	//************************************
	static int  US2ToAscii(const char *InMsg, int InLen, char *Data, int *Len);

	//************************************
	// Description:��ASCII�ַ���תΪUS2�ַ���
	// Parameter: [IN] const char* Msg		: ԭʼ�ַ���
	//			  [OUT]unsigned char *Data	: US2�ַ���
	//			  [OUT]unsigned int *Len	: US2������
	// Returns:   int						0:�ɹ�  ��0:ʧ��
	//************************************
	static int	AsciiToUS2(const char* Msg, unsigned char *Data, unsigned int *Len);

	//************************************
	// Description:��XML�ĵ���&gt &ltת����"<>"
	// Parameter: [IN/OUT]char *buf			: ����ԭʼ�ַ���,����ת������ַ���
	// Returns:   int						0:�ɹ�  ��0:ʧ��
	//************************************
	static int	TranHttpSpace(char *buf);

	//************************************
	// Description:����ָ���Ĳ�ֹ��򣬼����������Ҫ����ɼ���
	// Parameter: [IN] int nMsgLen			: ���ų���
	//			  [IN] int nSplitLen		: ��ֳ���,����nsplitlen�������
	//			  [IN] int nMultilen1		: �����ŵ�һ���ĳ���
	//			  [IN] int nMultilen2		: ���������һ���ĳ���
	//			  [IN] const char *szMsg	: �������ݣ�����������us2���룬����������ֻ��nMsgLen����
	//			  [IN] BOOL bAsciiToUS2		: ��������ASCII���룬�贫��szMsg����������ΪTRUE
	// Returns:   int						���ز������
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
	
	//���㴿Ӣ��ASCII��������
static	int CalcEnMsgNum_Global(const char* szEnMsg/*ASCII������(��ǩ��)*/, int nSingleLen/*��������(��ǩ��)*/, int nMultilen1/*����1����(��ǩ��(����))*/, int nMultilen2/*����2����(��ǩ��(����)*/);
		
		//���㴿Ӣ��ASCII���ų���(��gsm 7-bit�����жԷ����ֽ�����Ҫ����м���)
static   int CalcEnMsgLen_Global(const char* szEnMsg);

static bool IsSpecialAscii(char cAscii);
	//************************************
	// Description:����ָ���Ĳ�ֹ��򣬼����������Ҫ����ɼ���
	// Parameter: [IN] int nMsgLen			: ���ų���
	//			  [IN] int nSplitLen		: ��ֳ���,����nsplitlen�������
	//			  [IN] const char *szMsg	: �������ݣ�����������us2���룬����������ֻ��nMsgLen����
	//			  [IN] BOOL bAsciiToUS2		: ��������ASCII���룬�贫��szMsg����������ΪTRUE
	// Returns:   int						���ز������
	//************************************
	static int	CalcMsgNum(
					int nMsgLen, 
					int nSplitLen,
					const char *szMsg = NULL,
					BOOL bAsciiToUS2 = FALSE
					);

	//************************************
	// Description:����ָ���Ĳ�ֹ���ȡ�������е�ĳһ��
	// Parameter: [IN] const unsigned char *szInMsg	:����Ҫ��Ķ�������
	//			  [OUT]char *szOutMsg		: �����ֺ����������(�ѽ���)
	//			  [IN] int nMultilen1		: �����ŵ�һ���ĳ���
	//			  [IN] int nMultilen2		: ���������һ���ĳ���
	//			  [IN] int nPkNum			: Ҫ�ڼ���
	//			  [IN] int nPkTotal			: �ܹ�����
	//			  [IN] int nPreMsgLen		: �ϴη��صĳ���
	//			  [IN] int nMsgLen			: �����������ݵĳ���(�������д���ǩ�������ȥǩ���ĳ���=nMsgLen-nSignLen)
	//			  [IN] BOOL bUS2ToAscii		: �Ƿ�Ҫ����(���Ա��뷽ʽ��8��),�������ʱ,�����ַ���Ϊ���ַ�,��������ַ�������
	//			  [IN] INT nSignLen			: ǩ���ĳ���(����*2)����bUS2ToAscii=false��ò�������0����bUS2ToAscii=true�����Ϊ0Ҳ����Ϊʵ��ֵ
	// Returns:   int						���ز������
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
	// Description:�Թ̶����Ⱦ���һ������
	// Parameter: [IN]  CString Msg			: ����Ҫ��Ķ�������
	//			  [OUT] char *szOutMsg		: �����ֺ����������(�ѽ���)
	//			  [OUT] char *Msg1			: ���β������
	//			  [OUT] char *Msg2			: �����µ�����
	// Returns:   int						>0:�ɹ� <0:ʧ��
	//************************************
	//static int	SplitMsg(CString Msg, int nSplitLen, char *Msg1, char *Msg2);
	//zhubo 2017-4-11 ����static
	static int SplitMsg(const char* Msg, int nSplitLen, char *Msg1, char *Msg2);
	
	//************************************
	// Description:��������ID
	// Parameter: void
	// Returns:   int �������ɵ�����ID
	//************************************
	int		GeneranteSeqID();

	//************************************
	// Description:��INT64��MsgIdת����8�ֽڵ�char
	// Parameter: [OUT]unsigned char* MsgId : ת�����char��MsgId
	//			  [IN] __int64 IMsgId		: INT64��MsgId
	// Returns:   void
	//************************************
	static void	TranMsgIdI64ToChar(unsigned char* MsgId, __int64 IMsgId);

	//************************************
	// Description:����MMDDHHMMSS+���ش���+���кŸ�ʽ���ַ���ת��Ϊ8�ֽ��ַ���
	// Parameter: [OUT]unsigned char* MsgId : ת�����8�ֽ��ַ���
	//			  [IN] char *pSrc			: MMDDHHMMSS+���ش���+���кŸ�ʽ���ַ���
	// Returns:   void
	//************************************
	static void	MakeMsgId(unsigned char *MsgId, char *pSrc);

	//************************************
	// Description:��ʱ��+���ش���+���к�+��������8�ֽ�MsgId
	// Parameter: [OUT]unsigned char* MsgId : ���ɵ�8�ֽ��ַ���
	//			  [IN] unsigned int Num		: Ҫ���ɵ�MsgId��Ŀ
	//			  [IN] unsigned int Gate	: ���ر��
	// Returns:   void
	//************************************
	void	MakeMsgId(unsigned char *MsgId, unsigned int Num = 1, unsigned int Gate = 1);

	
	void	MakeMsgId_qc(unsigned char *MsgId, unsigned int Num = 1, unsigned int Gate = 1);

	//************************************
	// Description:����ʱ��+���ش���+���к�+��������8�ֽ�MsgId��ת����ԭʼ�ĸ�ʽMMDDHHMMSS+���ش���+���кŸ�ʽ���ַ���
	// Parameter: [IN] unsigned char *pSrc : 8�ֽڵ�MsgId�ַ���
	//			  [OUT]char *MsgId		   : MMDDHHMMSS+���ش���+���кŸ�ʽ���ַ���
	// Returns:   int					   0:�ɹ� ��0:ʧ��
	//************************************
	static int	TranMsgId(unsigned char *pSrc, char *MsgId);

	//************************************
	// Description:��8�ֽ�MsgId�е�ʱ���������
	// Parameter: [OUT]char *szTime			: MsgId���ɵ�ʱ��
	//			  [IN] const unsigned char *pSrc : 8�ֽ�MsgId
	// Returns:   void
	//************************************
	static void	GetTimeInMsgId(char *szTime, const unsigned char *pSrc);
	static time_t  GetTimeInMsgId(const unsigned char *pSrc);
	static time_t string2time(const char * str,const char * formatStr);

	//�ú����Ѳ���
	void	UnMakeMsgId(char *MsgId, const unsigned char *pSrc);

	//************************************
	// Description:��8�ֽ�MsgIdת����INT64��������
	// Parameter: [IN] const unsigned char *szMsgId : 8�ֽ�MsgId
	// Returns:   __int64	����MsgId
	//************************************
	static __int64 TranMsgIdCharToI64(const unsigned char *szMsgId);

	//************************************
	// Description:ȡ��SrcId��,�����ŵ�PkNum����MsgId(�ú�����Ը�MsgIdΪ�����ŵ���MsgId�����)
	// Parameter: [IN] __int64 SrcId		: ��MsgId
	//			  [IN] int PkNum			: Ҫȡ�ڼ�����MsgId
	// Returns:   __int64					����MsgId
	//************************************
	static __int64 GetI64MsgIdAt(__int64 SrcId, int PkNum);
		
	//************************************
	// Description:Base64����
	// Parameter: [IN] LPBYTE pbSrc			: Դ�ַ���
	//			  [IN] DWORD SrcLen			: Դ�ַ�������
	//			  [OUT]LPBYTE pbDes			: ���ܺ��ַ���
	// Returns:   int						���ؼ��ܺ��ִ�����
	//************************************
	static int		Base64Encode(LPBYTE pbSrc, DWORD SrcLen, LPBYTE pbDes); 

	//************************************
	// Description:Base64����
	// Parameter: [IN] LPBYTE pbSrc			: Դ�ַ���
	//			  [IN] DWORD SrcLen			: Դ�ַ�������
	//			  [OUT]LPBYTE pbDes			: ���ܺ��ַ���
	// Returns:   int						���ؽ��ܺ��ִ�����
	//************************************
	static int		Base64Decode(LPBYTE pbSrc, DWORD SrcLen, LPBYTE pbDes); 

	static int		Base64Decode(string& strIn,string& strOut); 

	//************************************
	// Description:��DATEת��Ϊʱ���ʽ
	// Parameter: [IN] DATE dt
	// Returns:   CString ʱ���ַ���						
	//************************************
	//static CString	DateToTimeStr(DATE dt);

	//��soap��װ��Ϣ�������ֽ�������ǩ��Ӧ������
	static int GetCmdValue(char *Data, char *CmdItem, int MaxSize, char *CmdVal, BOOL bUTF8=true);

	//��HTTP-post/getģʽ����Ϣ���н�����������������������Ӧ������
	static int GetCmdValueEx(char *Data, char *CmdItem, int MaxSize, char *CmdVal, BOOL bConvert=true);
	static int GetCmdValueExEx(char *Data, char *CmdItem, int MaxSize, char *CmdVal);


	//����JAVA��ҳ����
	static void ParaseXmlUTF8(string& strIn,string& strOut);
	static void ParaseXmlUtf8ToGBk(char *szDest, const char *szSrc, int nInOutLen, int nMaxOutLen);
	static void ParaseJavaEncode(char *szDest, char *szSrc, int& nInOutLen, int nDestMaxLen);
	static	int HexchartoInt_ex(char chHex);

	//������ҳ����
	static void DecodeHtmlStr(char *szDest, char *szSrc, int& nInOutLen);
	//multisend ר��
	static void DecodeHtmlStrEx(char *szDest, char *szSrc, int& nInOutLen);
	static void DecodeHtmlStr(string& strIn,string& strOut);
	//multisend ר��
	static void DecodeHtmlStrEx(string& strIn,string& strOut);
	

	//����','�ָ����ַ�����ֺ�ŵ������з��ز�ֵĶ��� nMaxLenΪÿ�ε���󳤶�
	static int  SplitStr(const char *szInStr, std::vector<std::string>& vOutStr, int nMaxLen=32);

	static int SplitStrEx(const char *szInStr, std::vector<std::string> &vOutStr, const char* c, int nMaxLen);

	static int SplitString(const string& s, vector<string>& v, const string& c);
	
	//************************************
	// Description:У����ŵ���Ч��(����/���ɼ��ַ���)
	// Parameter: [IN] int nMsgFmt:���ű��뷽ʽ
	//			  [IN] int nMaxLen:�������󳤶�
	//			  [IN] int nMsgLen:���ŵĳ���
	//			  [IN] /*const */char *szMsg:���ŵ�����
	// Returns:   int  0:�ɹ�(���в��ɼ��ַ��ȣ����޸Ķ��ŵ����ݣ�ȥ�����ɼ��ַ����������µĶ��ų���) 6:���� 
	//************************************
	static int	CheckMsgValidity(int nMsgFmt, int nMaxLen, USHORT& nMsgLen, /*const */char *szMsg, bool bMo=false);
	static int  CheckMsgValidity(int nMsgFmt, int nMaxLen, string& strContent);

	static int TrimUnicodeStrRight(string& strUnicodeStr);

	//������ų���
	static int  CalcMsgLen(int nMsgFmt, int nMsgLen, const char *szInMsg);

	//��Urlȡ��IP�Ͷ˿�  http://192.169.1.130:8081/sms/mt  ip=192.169.1.130 port=8081 webpath=/sms/mt
	static int  GetIpFromUrl(const char *szUrl, char *szWebPath, char *szIp, char* szDomain, int& nPort);
    static int  GetIpFromHttps(const char *szUrl, char *szWebPath, char *szIp, char* szDomain, int& nPort);
	static int  GetHostFromUrl(char* szUrl,char* pHost,int nMaxHostLen);

	//ʮ�������ַ�ת��������
	static int  HexchartoInt(char chHex);
	static int	TransMsg(char *szDest, const char *szSrc, int nMsgFmt, int nSrcLen, int nMaxSize);

	//�ڴ��ֽ�ת����HEX�ִ�
	static int  MemBytesToHexStr(const unsigned char *szSrc, int nLen, char *szDest, bool bLowFlag = false);
	//HEX�ִ�ת�����ڴ��ֽ�
	static int	HexStrToMemBytes(unsigned char *szDest, const char *szSrc, int nSrcLen, int nDestSize);
	//�滻������szSrc�е�cSrc�滻ΪcDest
	static void	ReplaceChar(unsigned char *szSrc, char cSrc, char cDest, unsigned int nSrcLen);

	//************************************
	// Description:ȡ�û���Ȩ����ֵ(00000000000000000000000000000000)32λ����һλ��ʾ�Ա�MsgIdȨ�ޣ��ڶ�λ��ʾ�Ƿ��Ǵ����ʺ�
	// Parameter: [IN] int nPrivilege		:��Ȩ������ֵ�ӵ�λ����λ��01ֵ����ʾ��λ����Ӧ�������Ƿ����
	//			  [IN] int nBit				:�ڼ�λ������
	// Returns:   int  ��nBitλ������ֵ  
	//************************************
	static int	GetUserPrivilege(int nPrivilege, int nBit);

	//���Ʒ�������
	static void BeepEx(DWORD dwFreq, DWORD dwDuration);

	static int GetValidTime(const char* szMsgId, const char* szDoneTime, char* szTime/*in/out "YYYY-MM-DD HH-MM-SS"*/);
	
	static bool BeepOrNot;

	//�滻����
	static void replace(std::string& str, std::string olds, std::string news);

	//ʱ�亯��
	static std::string date(std::string fmt = "Y-m-d H:i:s", int nday = 0);
	
	// added by youbiyun 2013-07-18
	// ȥ�����ҿո�
	static void trimLR(std::string &strSrc);
	
	static void trimLR(char *szSrc);
	
	static	char* strstri(const char* pString, const char* pFind);
	
	/*
	UrlEncode ���ٷֺű��룩�������ǰ�������ַ������� UTF-8 ���룬
	Ȼ��ѻ����ַ�������ֽ��ðٷֺż�16���Ƶ���ʽ���롣UrlEncode 
	�����һ�����������ǣ��������е�16�����ַ��Ƿ���ô�д��ĸ��ʾ��
	*/
	//�ٷֺű���
	//http://zh.wikipedia.org/zh-cn/%E7%99%BE%E5%88%86%E5%8F%B7%E7%BC%96%E7%A0%81
	static BOOL UrlEncode(char* szSrc, char* pBuf, int cbBufLen, BOOL bUpperCase);
	static std::string UrlEncode(const std::string& src);
	static BOOL UrlDecode(char* szSrc, char* pBuf, int cbBufLen);

	static unsigned int IpToInt(const char* ipStr);
	static bool  IsIpInner(unsigned int clientIp, unsigned int begin, unsigned int end);

	/**
	* ȡһ��һ���ڵ����ʱ�䣬����ʱ����
	* @param  {int} nBeginHour : �������ʼʱ
	* @param  {int} nBeginMin : �������ʼ��
	* @param  {int} nBeginSec : �������ʼ��
	* @param  {int} nMaxHour : �������ʱ,0Ĭ�ϲ���
	* @param  {int} nMaxMinu : ��������,0Ĭ�ϲ���
	* @param  {int} nMaxSec : ���������,0Ĭ�ϲ���
	* @return {int} ������������
	*/
	static int GetRandomTime(int nBeginHour, int nBeginMin, int nBeginSec,
		int nMaxHour = 0, int nMaxMinu = 0, int nMaxSec = 0);

	/**
	* �жϵ�ǰʱ���Ƿ���ڵ�������ĵ���ʱ��
	* @param  {int} nDaySec : ����ĵ���ʱ��������
	* @return {bool} �Ƿ񳬹������
	*/
	static bool bInOrOutTime(int nDaySec);
    
    // add by dengyu 2017-4-28
    static bool IsAllDigital(const char* str);
    
    // add by dengyu 2017-5-25
    static DATE StrToDATE(const char* strTime);

	static void string_replace(string&s1, const string&s2, const string&s3);

	static bool IsEnglishMsg(const char * szMsg, int nLen);

	//����������SQEID
	static int MakeSeqId();

private:

	
	//�ú����Ѳ���
	void	GenernateMsgIDEx(unsigned char *szMsgID, int nUId);

	//************************************
	// Description:��һ��2���Ʊ�ʾ���ַ���ת��Ϊ����
	// Parameter: [IN] char *szBuf			: �������ַ���
	// Returns:   int						�������ַ�����Ӧ������
	//************************************
	int		BinStr2Num(char *szBuf);

	//************************************
	// Description:�����ڲ�ָ�����������
	// Parameter: [IN] int Num				: ����
	// Returns:   int						���
	//************************************
	int		GetMsgId(int Num);
	int 	GetMsgId(int Num, bool &bFlow);

private:
	DerivedLock cs_MsgId;
};
#endif
