// CompressStr.cpp: implementation of the CCompressStr class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "CompressStr.h"
#include "../include/zlib.h"
#include "./adapter/Adapter.h"

/*
在工程主CPP中加入下面的正确路径
#ifdef WIN32
#ifdef _DEBUG
#pragma comment(lib, "../public/lib/zlibd.lib")
#else
#pragma comment(lib, "../public/lib/zlib.lib")
#endif
#endif
*/

#define MAX_SRCFILE_LEN		1024*1024*500

int CCompressStr::ExecFile(char *szDestFileName,char *szSrcFileName,int nMaxFileLen,int nFlag)
{
	int nRes = -2;
	FILE* in;
	FILE* out;
	in = fopen(szSrcFileName, "rb");
	if (in == NULL)
	{
		DEBUG_PRINTF("[%s:%d] Can't open source file", __FILE__, __LINE__);
		return nRes;
	}
	out = fopen(szDestFileName, "wb");
	if (out == NULL)
	{
		DEBUG_PRINTF("[%s:%d] Can't open dest file", __FILE__, __LINE__);
		fclose(in);
		return nRes;
	}
	fseek(in, 0, SEEK_END);
	unsigned long soulen = ftell(in);
	if (soulen > MAX_SRCFILE_LEN)
	{
		fclose(in);
	    fclose(out);
		return nRes;
	}
	fseek(in, 0, SEEK_SET);
	unsigned long nRetLen = nMaxFileLen;
	int nRet = -2;
	if (nFlag == 1) // compress
	{
		BYTE *soubuf = new BYTE[soulen];
		BYTE *destbuf= new BYTE[soulen];
		memset((char*)soubuf,0,soulen);
		memset((char*)destbuf,0,soulen);
		fread(soubuf, 1, soulen, in);
		nRet = compress((BYTE*)destbuf,&nRetLen,(BYTE*)soubuf, soulen);
		if (nRet == 0)		// can't compress the block
		{
			fwrite(destbuf, 1, nRetLen, out);
			nRes = 0;
		}
		else
		{		
			nRes = -1;				
		}
		delete []soubuf;
		delete []destbuf;
	}
    else
	{
		if (nMaxFileLen > 0 && nMaxFileLen <= MAX_SRCFILE_LEN)
		{
			BYTE *soubuf = new BYTE[soulen];
			BYTE *destbuf= new BYTE[nMaxFileLen];
			memset(soubuf,0,soulen);
			memset(destbuf,0,nMaxFileLen);
			fread(soubuf, 1, soulen, in);
			nRet = uncompress(destbuf,&nRetLen,soubuf,soulen);
			if (nRet == 0)		// can't compress the block
			{
				fwrite(destbuf, 1, nRetLen, out);
				nRes = 0;
			}
			else
			{		
				nRes = -1;				
			}
			delete []soubuf;
			delete []destbuf;
		}
		else
		{
			nRes = -1;
		}
	}
	fclose(in);
	fclose(out);
	return nRes;
}

CCompressStr::CCompressStr()
{
	//m_pDestBuf = new BYTE[MAX_SRCFILE_LEN+1];
}

CCompressStr::~CCompressStr()
{
	//delete [] m_pDestBuf; 
}

int CCompressStr::CompressStr(unsigned char *szDestBuf, unsigned long *nDestLen, unsigned char *szSrcBuf, unsigned long nSrcLen)
{
    int nRes = compress(szDestBuf, nDestLen, szSrcBuf, nSrcLen);
	if (0 == nRes)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int CCompressStr::UnCompressStr(unsigned char *szDestBuf, unsigned long *nDestLen, unsigned char *szSrcBuf, unsigned long nSrcLen)
{
	int nRes = uncompress(szDestBuf, nDestLen, szSrcBuf, nSrcLen);
	if (0 == nRes)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int CCompressStr::CompressFile(char *szDestFileName,char *szSrcFileName,int nMaxFileLen)
{
    return ExecFile(szDestFileName,szSrcFileName,nMaxFileLen,1);
}

int CCompressStr::UnCompressFile(char *szDestFileName,char *szSrcFileName,int nMaxFileLen)
{
    return ExecFile(szDestFileName,szSrcFileName,nMaxFileLen,2);
}
