#include "utility/common/mwcore_export.h"
#include <iostream>
#include <string>
#include "iconv.h"
#include <errno.h>
#include <stdexcept>
using namespace std;

namespace UTI{
namespace COM{

/*常用的编码类型*/
#define ICONV_ASCII     "ASCII"
#define ICONV_UTF_8     "UTF-8"
#define ICONV_UTF_16    "UTF-16"
#define ICONV_UTF_32    "UTF-32"
//ucs-2be 和 ucs-2se   一个是大端法(big-endian)，一个是小端法(little-endian)
// 默认大端
#define ICONV_UCS_2BE   "UCS-2BE"
#define ICONV_UCS_2SE   "UCS-2SE"
#define ICONV_UCS_4     "UCS-4"
#define ICONV_GBK       "GBK"
#define ICONV_GB2312    "GB2312"

typedef unsigned char   UTF8;    /* typically 8 bits */

enum utf_check_result 
{ 
	ok = 0, //全部字符为utf-8
	fault=1, //全部非utf-8编码
	cutted=2,//部分字符为utf-8编码
};

// 字符级编码类
class UTILITY_Export DCharsetCode{
    iconv_t iconv_;
    bool ignore_error_;
    const size_t buf_size_;
public:

    DCharsetCode(const std::string& in_encode,
        const std::string& out_encode,
        bool ignore_error = true,
        size_t buf_size = 1024);
    ~DCharsetCode();

    // 字符编码转换
    void convert(const std::string& input, std::string& output) const;

private:
    // 转换错误校验
    void check_convert_error() const; 

public:
    /**************************************************************************************
    * Function      : IsUtf8
    * Description   : 字串是否为UTF8
    * Input         : [IN] const void* pBuffer  - 内容
    *                 [long] long size          - 判断内容长度
    * Output        : bool
    * Return        : bRet - true[是] false[不是]
    * Modify        : 无
    * Others        : 无
    **************************************************************************************/
    static bool IsUtf8(const void* pBuffer, long size);
    /**************************************************************************************
    * Function      : isLegalUTF8
    * Description   : 字串是否为UTF8（LINUX）
    * Input         : [IN] const UTF8* source   - 内容
    *                 [INT] int length          - 截取内容长度
    * Output        : bool
    * Return        : bRet - true[是] false[不是]
    * Modify        : 无
    * Others        : 无
    **************************************************************************************/
    static bool isLegalUTF8(const UTF8* source, int length);

    /**************************************************************************************
    * Function      : utf8_check
    * Description   : UTF8校验
    * Input         : [IN] const char* pzSrcStr - 源字符串
    *                 [OUT] int&cutted_len      - 切割长度
    * Output        : utf_check_result
    * Return        : bRet - [ok]全部字符为utf-8 [fault]全部非utf-8编码 [cutted]部分字符为utf-8编码
    * Modify        : 无
    * Others        : 无
    **************************************************************************************/
    utf_check_result utf8_check(const char* pzSrcStr, int&cutted_len);
    /**************************************************************************************
    * Function      : mc_strlen
    * Description   : 获取字串长度
    * Input         : [IN] const char* str  - 字符内容
    * Output        : int
    * Return        : 返回字符串长度
    * Modify        : 无
    * Others        : 无
    **************************************************************************************/
    static int mc_strlen(const char* str);
    /**************************************************************************************
    * Function      : mc_wstrlen
    * Description   : 获取宽字串长度
    * Input         : [IN] const wchar_t* str  - 宽字符内容
    * Output        : int
    * Return        : 返回宽字符串长度
    * Modify        : 无
    * Others        : 无
    **************************************************************************************/
    static int mc_wstrlen(const wchar_t* str);
};

}}//end namespace


/**
* 转码函数  通用接口
* @param  const char* {pFromCode}  : 原始编码
* @param  const char* {pToCode}  : 目标编码
* 目的编码可在后面追加, TRANSLIT：遇到无法转换的字符就找相近字符替换
*					    IGNORE  ：遇到无法转换字符跳过
*						eg: UNICODE//TRANSLIT
*
* @param  const std::string& {strIn}  : 原始缓冲
* @param  std::string& {strOut}  : 目标缓冲
* @param  size_t {iDestLen}  : 目标缓冲长度
* @return {int} 返回转换后的长度
*/
int UTILITY_Export mw_iconv_change_code(const char* pFromCode,
    const char* pToCode,
    const std::string& strIn,
    std::string& strOut,
    size_t iMaxDestLen);
