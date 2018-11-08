// CompressStr.h: interface for the CCompressStr class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __COMPRESSSTR_H__ 
#define __COMPRESSSTR_H__

#include "utility/common/mwcore_export.h"

namespace UTI{
namespace COM{

class UTILITY_Export CCompressStr
{
public:
    CCompressStr();
    virtual ~CCompressStr();                                    
    // 压缩源缓冲区到目标缓冲区  nDestLen 目标缓冲区大小 nSrcLen:源缓冲区大小 
    int CompressStr(unsigned char *szDestBuf, unsigned long *nDestLen, unsigned char *szSrcBuf, unsigned long nSrcLen);

    // 解压源缓冲区到目标缓冲区  参数2: nDestLen 为目标缓冲区大小（不能初始化为0）
    int UnCompressStr(unsigned char *szDestBuf, unsigned long *nDestLen, unsigned char *szSrcBuf, unsigned long nSrcLen);

    // 压缩文件
    int CompressFile(char *szDestFileName, char *szSrcFileName, int nMaxFileLen);

    // 解压文件
    int UnCompressFile(char *szDestFileName, char *szSrcFileName, int nMaxFileLen);
private:
    int ExecFile(char *szDestFileName, char *szSrcFileName, int nMaxFileLen, int nFlag);
};
}} // end namespace 

#endif 
