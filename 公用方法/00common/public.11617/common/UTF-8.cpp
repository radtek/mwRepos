#include "StdAfx.h"
#include "UTF-8.h"
#include <stdlib.h>
#include <string.h>
#include "adapter/Adapter.h"

#ifdef LINUX
#include <iconv.h>
#endif

// 把UTF-8转换成Unicode
void UTF_8ToUnicode(WCHAR* pOut,char *pText)
{
    char* uchar = (char *)pOut;

    uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
    uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);

    return;
}

// Unicode 转换成UTF-8
void UnicodeToUTF_8(char* pOut, WCHAR* pText)
{
    // 注意 WCHAR高低字的顺序,低字节在前，高字节在后
    char* pchar = (char *)pText;

    pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
    pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
    pOut[2] = (0x80 | (pchar[0] & 0x3F));

    return;
}

// 把Unicode 转换成 GB2312
void UnicodeToGB2312(char* pOut,unsigned short uData)
{
    CAdapter::WideCharToMultiByte(CP_ACP,0,(LPCWSTR)&uData,1,pOut,sizeof(WCHAR),NULL,NULL);
    //setlocale(LC_ALL,LC_NAME_zh_CN_DEFAULT);
    //wcstombs(pOut, (WCHAR*)&uData, sizeof(WCHAR));
    //setlocale(LC_ALL,NULL);
    return;
}

// GB2312 转换成　Unicode
void Gb2312ToUnicode(WCHAR* pOut, char *gbBuffer)
{
    CAdapter::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED, gbBuffer, 2, pOut,1);
    //setlocale(LC_ALL,LC_NAME_zh_CN_DEFAULT);
    //mbstowcs(pOut, gbBuffer, 2);
    //setlocale(LC_ALL,NULL);
    return;
}

//GB2312 转为 UTF-8
char* GB2312ToUTF_8(char *pOut, char *pText, int pLen)
{
    char buf[4] = {0};
    char* rst = new char[pLen + (pLen >> 2) + 2];
    if (rst)
    {
        try
        {
            memset(rst, 0, pLen + (pLen >> 2) + 2);
            int i = 0;
            int j = 0;
            while(i < pLen)
            {
                //如果是英文直接复制就可以
                if( *(pText + i) >= 0)
                {
                    rst[j++] = pText[i++];
                }
                else
                {
                    WCHAR pbuffer;
                    Gb2312ToUnicode(&pbuffer, pText + i);

                    UnicodeToUTF_8(buf, &pbuffer);

                    unsigned short int tmp = 0;
                    tmp = rst[j] = buf[0];
                    tmp = rst[j+1] = buf[1];
                    tmp = rst[j+2] = buf[2];


                    j += 3;
                    i += 2;
                }
            }
            rst[j] = '\0';

            //返回结果
            //*pOut = 0;
            //strcpy(pOut, rst);
            memcpy(pOut, rst, j);
            *(pOut + j) = 0;
        }
        catch (...)
        {
        }
    }
    if (rst)
    {
        delete []rst;
        rst = NULL;
    }
    return pOut;  
}

//UTF-8 转为 GB2312
char* UTF_8ToGB2312(char *pOut, char *pText, int pLen)
{
#ifdef WIN32
    //utf82ascii((UCHAR*)pText, pLen, pOut);
    //return pOut;
    if (NULL == pOut || NULL == pText)
    {
        return NULL;
    }

    DWORD UniCodeLen = pLen;//MultiByteToWideChar(CP_UTF8, 0, pText, -1, NULL, 0);
    if (UniCodeLen > 0)
    {
        std::vector <wchar_t> vWCH(UniCodeLen+1, '\0');
		//unsigned short szTmp[1024] = {0};
        UniCodeLen = CAdapter::MultiByteToWideChar(CP_UTF8, 0, pText, pLen, &vWCH[0], UniCodeLen);
        DWORD dwASCIILen = CAdapter::WideCharToMultiByte(CP_ACP, 0, &vWCH[0], UniCodeLen, NULL, 0, NULL, NULL);
        CAdapter::WideCharToMultiByte(CP_ACP, 0, &vWCH[0], UniCodeLen, pOut, dwASCIILen, NULL, NULL);
    }
#elif defined LINUX
	iconv_t cd;
	int rc;
	char **pin = &pText;
	char **pout = &pOut;

	try
	{
		cd = iconv_open(CSET_GBK, CSET_UTF8);
		if (cd == (void*)0xffffffff)
		{
			//printf("%s %d\n", strerror(errno), errno);
			return pOut;
		}

		if (iconv(cd,pin, (size_t*)&pLen, pout, (size_t*)&pLen)==-1)
		{
			iconv_close(cd);
			return pOut;
		}
		iconv_close(cd);
	}
	catch (...)
	{
	}
#endif
	
    return pOut;

/*    int len = pLen;//MultiByteToWideChar(CP_UTF8, 0, pText, -1, NULL, 0);
    unsigned short * wszGBK = new unsigned short[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)pText, -1, wszGBK, len);
  
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP,0, wszGBK, -1, szGBK, len, NULL, NULL);
    //strUTF8 = szGBK;
    //std::string strTemp(szGBK);
    memcpy(pOut, szGBK, strlen(szGBK));
    delete[]szGBK;
    szGBK = NULL;
    delete[]wszGBK;
    wszGBK = NULL;
    return pOut;
/*    char *newBuf = new char[pLen+1];
    char Ctemp[4] = {0};

    int i = 0;
    int j = 0;

    while(i < pLen)
    {
        if(pText[i] > 0)
        {
            newBuf[j++] = pText[i++];
        }
        else
        {
            WCHAR Wtemp;
            UTF_8ToUnicode(&Wtemp, pText + i);

            UnicodeToGB2312(Ctemp, Wtemp);

            newBuf[j] = Ctemp[0];
            newBuf[j + 1] = Ctemp[1];

            i += 3;
            j += 2;
        }
    }
    newBuf[j] = '\0';

    //*pOut = 0;
    //strcpy(pOut, newBuf);
    memcpy(pOut, newBuf, j);
    *(pOut + j) = 0;

    delete newBuf;
    newBuf = NULL;

    return pOut;
*/
}

void ConvertGBKToUtf8(std::string& strGBK)
{
    int len = CAdapter::MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL,0);
    unsigned int * wszUtf8 = new unsigned int[len+1];
    char *szUtf8 = NULL;
    if (wszUtf8)
    {
        try
        {
            try
            {
                memset(wszUtf8, 0, len * 4 + 4);
                CAdapter::MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, (LPWSTR)wszUtf8, len);
                len = CAdapter::WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, NULL, 0, NULL, NULL);
            }
            catch (...)
            {
                if (wszUtf8)
                {
                    delete[] wszUtf8;
                    wszUtf8 = NULL;
                }
            }
            if (wszUtf8)
            {
                szUtf8 = new char[len + 1];
                if (szUtf8)
                {
                    try
                    {
                        memset(szUtf8, 0, len + 1);
                        CAdapter::WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL);
                        strGBK = szUtf8;
                    }
                    catch (...)
                    {
                    }
                }
                if (szUtf8)
                {
                    delete[] szUtf8;
                    szUtf8 = NULL;
                }
            }
            if (wszUtf8)
            {
                delete[] wszUtf8;
                wszUtf8 = NULL;
            }
        }
        catch (...)
        {
            if (wszUtf8)
            {
                delete[] wszUtf8;
                wszUtf8 = NULL;
            }
            if (szUtf8)
            {
                delete[] szUtf8;
                szUtf8 = NULL;
            }
        }
    }
}
//
// char *wchar_char(wchar_t w_cn , char c_cn[])
// {
//       c_cn[0] = w_cn >> 8;
//       c_cn[1] = w_cn;
//       c_cn[2] = '\0';
//       return c_cn ;
// }
//
// char* wchar_char(wchar_t* szWchar, char* szChar)
// {
//     int nSrcLen = szWchar != NULL ? wcslen(szWchar) : 0;
//     if (nSrcLen != 0)
//     {
//         _wcstombsz(szChar, szWchar, (nSrcLen*2)+1);
//     }
//     return szChar;
// }

char *u2s( unsigned char *buf, unsigned short length, char *tmpbuf )
{
    int i;
    char *p;
    char s[100] = {0};
    wchar_t wstr[1] = {0};
    char mstr[10] = {0};
    *tmpbuf = 0;
    for( i = 0; i < ( int )length; i += 2 )
    {
        p = ( char * )wstr;
        *p = *( buf + 1 );            //high
        *( p + 1 ) = *buf;            //low
        memset(mstr, 0, sizeof(mstr));
        CAdapter::WideCharToMultiByte(CP_ACP, 0, wstr, -1, mstr, sizeof(mstr), NULL, NULL);
        memset( s, 0, sizeof( s ) );
        strcat( s, mstr);
        if( *buf )        //if low is not 0
        {
            s[ 2 ] = 0;
        }
        else
        {
            s[ 1 ] = 0;
        }

        strcat( tmpbuf, ( const char * )s );

        buf += 2;
    }

    return tmpbuf;
}

char *utf82ascii( unsigned char *utf8, unsigned short length, char *buf )
{
    int i = 0;
    unsigned short a;
    unsigned char b;
    char *p = buf;
    unsigned char t[ 2 ];

    do
    {
        if( 0xe0 == ( *utf8 & 0xf0 ) &&
            0x80 == ( *( utf8 + 1 ) & 0xc0 ) &&
            0x80 == ( *( utf8 + 2 ) & 0xc0 ) )
        {
            //3个字节
            b = *utf8;
            b &= 0x0f;
            a = b;

            a <<= 6;
            b = *( utf8 + 1 );
            b &= 0x3f;
            a |= b;

            a <<= 6;
            b = *( utf8 + 2 );
            b &= 0x3f;
            a |= b;

            t[ 0 ] = ( a >> 8 ) & 0xff;
            t[ 1 ] = ( a & 0xff );
            u2s( t, 2, p );

            p += 2;

            i += 3;
            utf8 += 3;
        }
        else if ( 0xc0 == ( *utf8 & 0xe0 ) &&
            0x80 == ( *( utf8 + 1 ) & 0xc0 ) )
        {
            //2个字节
            b = *utf8;
            b &= 0x1f;
            a = b;

            a <<= 6;
            b = *( utf8 + 1 );
            b &= 0x3f;
            a |= b;

            t[ 0 ] = ( a >> 8 ) & 0xff;
            t[ 1 ] = ( a & 0xff );
            u2s( t, 2, p );

            p += 2;

            i += 2;
            utf8 += 2;
        }
        else
        {
            //1个字节
            *( p++ ) = *( utf8++ );

            i++;
        }
    } while ( i < length );
    *p = 0;

    return buf;
}
