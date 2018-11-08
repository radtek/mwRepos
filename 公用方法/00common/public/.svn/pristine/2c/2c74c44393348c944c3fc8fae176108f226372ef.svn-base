#include "utility/common/mwcorepch.h"
#include "utility/common/charset_code.h"

using namespace UTI::COM;
DCharsetCode::DCharsetCode(const std::string& in_encode,
    const std::string& out_encode,
    bool ignore_error,
    size_t buf_size)
    : ignore_error_(ignore_error),
    buf_size_(buf_size)
{
    if (buf_size == 0) {
        throw std::runtime_error("buffer size must be greater than zero");
    }

    iconv_t conv = ::iconv_open(out_encode.c_str(), in_encode.c_str());
    if (conv == (iconv_t)-1) {
        if (errno == EINVAL)
            throw std::runtime_error(
            "not supported from " + in_encode + " to " + out_encode);
        else
            throw std::runtime_error("unknown error");
    }
    iconv_ = conv;
}

DCharsetCode::~DCharsetCode(void)
{
    iconv_close(iconv_);
}

void DCharsetCode::convert(const std::string& input, std::string& output) const 
{
    // copy the string to a buffer as iconv function requires a non-const char
    // pointer.
    std::vector<char> in_buf(input.begin(), input.end());
    char* src_ptr = &in_buf[0];
    size_t src_size = input.size();

    std::vector<char> buf(buf_size_);
    std::string dst;
    while (0 < src_size) {
        char* dst_ptr = &buf[0];
        size_t dst_size = buf.size();
        size_t res = ::iconv(iconv_, &src_ptr, &src_size, &dst_ptr, &dst_size);
        if (res == (size_t)-1) {
            if (errno == E2BIG)  {
                // ignore this error
            }
            else if (ignore_error_) {
                // skip character
                ++src_ptr;
                --src_size;
            }
            else {
                check_convert_error();
            }
        }
        dst.append(&buf[0], buf.size() - dst_size);
    }
    dst.swap(output);
}

void DCharsetCode::check_convert_error() const
{
    switch (errno) {
    case EILSEQ:
    case EINVAL:
        throw std::runtime_error("invalid multibyte chars");
    default:
        throw std::runtime_error("unknown error");
    }
}

/**************************************************************************************
* Function      : mc_wstrlen
* Description   : 获取宽字串长度
* Input         : [IN] const wchar_t* str  - 宽字符内容
* Output        : int
* Return        : 返回宽字符串长度
* Modify        : 无
* Others        : 无
**************************************************************************************/
int DCharsetCode::mc_wstrlen(const wchar_t* str)
{
	if (NULL == str)
		return 0;
	return wcslen(str);
}

/**************************************************************************************
* Function      : mc_strlen
* Description   : 获取字串长度
* Input         : [IN] const char* str  - 字符内容
* Output        : int
* Return        : 返回字符串长度
* Modify        : 无
* Others        : 无
**************************************************************************************/
int DCharsetCode::mc_strlen( const char* str )
{
	if (NULL == str)
		return 0;

	return strlen(str);
}

/**************************************************************************************
* Function      : IsUtf8
* Description   : 字串是否为UTF8
* Input         : [IN] const void* pBuffer  - 内容
*                 [INT] long size           - 截取内容长度
* Output        : bool
* Return        : bRet - true[是] false[不是]
* Modify        : 无
* Others        : 无
**************************************************************************************/
bool DCharsetCode::IsUtf8(const void* pBuffer, long size)
{
	bool IsUTF8 = true;
	unsigned char* start = (unsigned char*)pBuffer;
	unsigned char* end = (unsigned char*)pBuffer + size;
	while (start < end)
	{
		if (*start < 0x80) // (10000000): 值小于0x80的为ASCII字符
		{
			start++;
		}
		else if (*start < (0xC0)) // (11000000): 值介于0x80与0xC0之间的为无效UTF-8字符
		{
			IsUTF8 = false;
			break;
		}
		else if (*start < (0xE0)) // (11100000): 此范围内为2字节UTF-8字符
		{
			if (start >= end - 1) 
				break;
			if ((start[1] & (0xC0)) != 0x80)
			{
				IsUTF8 = false;
				break;
			}
			start += 2;
		} 
		else if (*start < (0xF0)) // (11110000): 此范围内为3字节UTF-8字符
		{
			if (start >= end - 2) 
				break;
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)
			{
				IsUTF8 = false;
				break;
			}
			start += 3;
		} 
		else
		{
			IsUTF8 = false;
			break;
		}
	}
	return IsUTF8;

}

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
bool DCharsetCode::isLegalUTF8(const UTF8* source, int length)
{
    UTF8        a;
    const UTF8* srcptr = source + length;
    switch (length)
    {
    default: return false;
        /* Everything else falls through when "true"... */
    case 4: if ((a = (*--srcptr)) < 0x80 || a > 0xBF) return false;
    case 3: if ((a = (*--srcptr)) < 0x80 || a > 0xBF) return false;
    case 2: if ((a = (*--srcptr)) > 0xBF) return false;

        switch (*source)
        {
            /* no fall-through in this inner switch */
        case 0xE0: if (a < 0xA0) return false; break;
        case 0xED: if (a > 0x9F) return false; break;
        case 0xF0: if (a < 0x90) return false; break;
        case 0xF4: if (a > 0x8F) return false; break;
        default:   if (a < 0x80) return false;
        }

    case 1: if (*source >= 0x80 && *source < 0xC2) return false;
    }
    if (*source > 0xF4) return false;
    return true;
}

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
utf_check_result DCharsetCode::utf8_check(const char* pzSrcStr, int&cutted_len)
{
	for ( int i = 0; i <cutted_len; i++ )
	{
		int n = 0;
		int c = ( unsigned char ) pzSrcStr[i];
		if ( 0x00 <= c && c <= 0x7f )
		{
			n = 0;
		} // 0bbbbbbb
		else if ( ( c & 0xE0 ) == 0xC0 )
		{
			n = 1;
		} // 110bbbbb
		else if ( c == 0xed
			&& i < ( cutted_len - 1 )
			&& ( ( unsigned char ) pzSrcStr[i + 1] & 0xa0 ) == 0xa0 )
		{
			return fault;
		} // U+D800 to U+DFFF
		else if ( ( c & 0xF0 ) == 0xE0 )
		{
			n = 2;
		} // 1110bbbb
		else if ( ( c & 0xF8 ) == 0xF0 )
		{
			n = 3;
		} // 11110bbb
		else if ( ( c & 0xFC ) == 0xF8 )
		{
			n = 4;
		} // 111110bb // byte 5, unnecessary in 4 byte UTF-8 ??
		else if ( ( c & 0xFE ) == 0xFC )
		{
			n = 5;
		} // 1111110b // byte 6, unnecessary in 4 byte UTF-8 ??
		else
		{
			return fault;
		}

		int K( i );

		// n bytes matching 10bbbbbb follow ?
		for ( int j = 0; j < n && i < cutted_len; j++ )
		{
			//assert( 0 < n ) ;

			if ( ++i == cutted_len )
			{
				cutted_len = K;
				return cutted;
			}

			if ( ( ( unsigned char ) pzSrcStr[i] & 0xC0 ) != 0x80 )
			{
				return fault;
			}
		}
	}

	return ok;
}

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
int mw_iconv_change_code(const char* pFromCode,
    const char* pToCode,
    const std::string& strIn,
    std::string& strOut,
    size_t iMaxDestLen)
{
    int    nLen = 0;
    UTI::COM::DCharsetCode convert(pFromCode, pToCode, iMaxDestLen);
    convert.convert(strIn, strOut);

    return strOut.length();
}