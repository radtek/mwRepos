#ifndef _ICONV_HPP_H
#define _ICONV_HPP_H

/*
Copyright (c) 2011, Yuya Unno
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Yuya Unno nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <errno.h>
#include <iconv.h>

#include <stdexcept>
#include <string>
#include <vector>

/*定义一些常用的*/
#define ICONV_ASCII "ASCII"

#define ICONV_UTF_8 "UTF-8"
#define ICONV_UTF_16 "UTF-16"
#define ICONV_UTF_32 "UTF-32"
//ucs-2be 和ucs-2se   一个是大端法(big-endian)，一个是小端法(little-endian)，
#define ICONV_UCS_2 "UCS-2BE"
#define ICONV_UCS_4 "UCS-4"

#define ICONV_GBK "GBK"
//#define ICONV_GBK "GB18030"





enum ICONV_TOCODE_TYPE
{
	ICONV_TOCODE_NORM = 0,
	ICONV_TOCODE_TRANSLIT = 1,
	ICONV_TOCODE_IGNORE = 2
};

namespace iconvpp {
	class converter {
	 public:
	  converter(const std::string& in_encode,
	            const std::string& out_encode,
	            bool ignore_error = true,
	            size_t buf_size = 1024)
	      : ignore_error_(ignore_error),
	        buf_size_(buf_size) {
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

	  ~converter() {
	    iconv_close(iconv_);
	  }

	  void convert(const std::string& input, std::string& output) const {
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
	        } else if (ignore_error_) {
	          // skip character
	          ++src_ptr;
	          --src_size;
	        } else {
	          check_convert_error();
	        }
	      }
	      dst.append(&buf[0], buf.size() - dst_size);
	    }
	    dst.swap(output);
	  }

	 private:
	  void check_convert_error() const {
	    switch (errno) {
	      case EILSEQ:
	      case EINVAL:
	        throw std::runtime_error("invalid multibyte chars");
	      default:
	        throw std::runtime_error("unknown error");
	    }
	  }

	  iconv_t iconv_;
	  bool ignore_error_;
	  const size_t buf_size_;
	};

}  // namespace iconvpp

/**
* 转码函数
* @param  const char* {pFromCode}  : 原始编码
* @param  const char* {pToCode}  : 目标编码
* 目的编码可在后面追加, TRANSLIT：遇到无法转换的字符就找相近字符替换
*					    IGNORE  ：遇到无法转换字符跳过
*						eg: UNICODE//TRANSLIT
*
* @param  const int {AddModer}  : 目标追加编码，0：无 1：TRANSLIT，2：IGNORE
* @param  const std::string& {strIn}  : 原始缓冲
* @param  size_t {iInLen}  : 原始缓冲长度
* @param  std::string& {strOut}  : 目标缓冲
* @param  size_t {iDestLen}  : 目标缓冲长度
* @return {int} 返回转换后的长度
*/
static int mw_iconv_change_code(const char* pFromCode,
	const char* pToCode,
//	const int  addModer,
	const std::string& strIn,
//	size_t iInLen,
	std::string& strOut,
	size_t iMaxDestLen)
{
    int    nLen = 0;
    iconvpp::converter convert(pFromCode,pToCode,iMaxDestLen);
    convert.convert(strIn,strOut);
	
    return strOut.length();

    /*
	//长度判断，如果大于最大长度，则进行截断
	if (nLen > iDestLen)
	{
		nLen = iDestLen;
		strOut = strOut.substr(0,nLen);
	}
    return nLen;
    */
}
	
/**
* 转码函数
* @param  const char* {pFromCode}  : 原始编码
* @param  const char* {pToCode}  : 目标编码
* @param  const int {AddModer}  : 目标追加编码，0：无 1：TRANSLIT，2：IGNORE
* @param  const char* {pInBuf}  : 原始缓冲
* @param  size_t {iInLen}  : 原始缓冲长度
* @param  char* {pOutBuf}  : 目标缓冲
* @param  size_t {iDestLen}  : 目标缓冲长度
* @return {int} 返回转换后的长度
*/

static int Iconv_ChangeCode(const char* pFromCode,
	const char* pToCode,
	const int	AddModer,
	const char* pInBuf,
	size_t iInLen,
	char* pOutBuf,
	size_t inDestLen)
{
	int nOutLen = 0; 

	try
	{
		char szMaxToCode[128] = { 0 };
		strcpy (szMaxToCode, pToCode);

		if (AddModer == ICONV_TOCODE_TRANSLIT)
		{
			strcat(szMaxToCode, "//TRANSLIT");
		}
		else if (AddModer == ICONV_TOCODE_IGNORE)
		{
			strcat(szMaxToCode, "//IGNORE");
		}
		szMaxToCode[strlen(szMaxToCode)] = 0;

		//打开字符集转换 
		iconv_t hIconv = iconv_open(szMaxToCode, pFromCode);
		if ((iconv_t)-1 == hIconv)
		{
			//打开失败，可能不支持的字符集 
			return nOutLen;
		}

		//剩余量
		size_t nSurplusLen = inDestLen;
		//开始转换 
		int nRet = iconv(hIconv, (char**)&pInBuf, &iInLen, &pOutBuf, &nSurplusLen);
		if (nRet != 0)
		{
			//转换异常
		}

		//转出长度为进入长度减去剩余长度
		nOutLen = inDestLen - nSurplusLen;
		nOutLen > inDestLen ? nOutLen = inDestLen : inDestLen;

		//关闭字符集转换 
		iconv_close(hIconv);
	}
	catch (...)
	{
	}

	return nOutLen;
} 
#endif

