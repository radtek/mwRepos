// CompressStr.h: interface for the CCompressStr class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __COMPRESSSTR_H__ 
#define __COMPRESSSTR_H__

class CCompressStr  
{
public:
	CCompressStr();
	virtual ~CCompressStr();
    int CompressStr(unsigned char *szDestBuf, unsigned long *nDestLen, unsigned char *szSrcBuf, unsigned long nSrcLen);

    //参数2: nDestLen 为目标缓冲区大小（不能初始化为0）
	int UnCompressStr(unsigned char *szDestBuf, unsigned long *nDestLen, unsigned char *szSrcBuf, unsigned long nSrcLen);

	int CompressFile(char *szDestFileName,char *szSrcFileName,int nMaxFileLen);
    
	int UnCompressFile(char *szDestFileName,char *szSrcFileName,int nMaxFileLen);
private:
	int ExecFile(char *szDestFileName,char *szSrcFileName,int nMaxFileLen,int nFlag);
};

#endif 
