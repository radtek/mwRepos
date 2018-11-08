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

/*����һЩ���õ�*/
#define ICONV_ASCII "ASCII"

#define ICONV_UTF_8 "UTF-8"
#define ICONV_UTF_16 "UTF-16"
#define ICONV_UTF_32 "UTF-32"
//ucs-2be ��ucs-2se   һ���Ǵ�˷�(big-endian)��һ����С�˷�(little-endian)��
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
* ת�뺯��
* @param  const char* {pFromCode}  : ԭʼ����
* @param  const char* {pToCode}  : Ŀ�����
* Ŀ�ı�����ں���׷��, TRANSLIT�������޷�ת�����ַ���������ַ��滻
*					    IGNORE  �������޷�ת���ַ�����
*						eg: UNICODE//TRANSLIT
*
* @param  const int {AddModer}  : Ŀ��׷�ӱ��룬0���� 1��TRANSLIT��2��IGNORE
* @param  const std::string& {strIn}  : ԭʼ����
* @param  size_t {iInLen}  : ԭʼ���峤��
* @param  std::string& {strOut}  : Ŀ�껺��
* @param  size_t {iDestLen}  : Ŀ�껺�峤��
* @return {int} ����ת����ĳ���
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
	//�����жϣ����������󳤶ȣ�����нض�
	if (nLen > iDestLen)
	{
		nLen = iDestLen;
		strOut = strOut.substr(0,nLen);
	}
    return nLen;
    */
}
	
/**
* ת�뺯��
* @param  const char* {pFromCode}  : ԭʼ����
* @param  const char* {pToCode}  : Ŀ�����
* @param  const int {AddModer}  : Ŀ��׷�ӱ��룬0���� 1��TRANSLIT��2��IGNORE
* @param  const char* {pInBuf}  : ԭʼ����
* @param  size_t {iInLen}  : ԭʼ���峤��
* @param  char* {pOutBuf}  : Ŀ�껺��
* @param  size_t {iDestLen}  : Ŀ�껺�峤��
* @return {int} ����ת����ĳ���
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

		//���ַ���ת�� 
		iconv_t hIconv = iconv_open(szMaxToCode, pFromCode);
		if ((iconv_t)-1 == hIconv)
		{
			//��ʧ�ܣ����ܲ�֧�ֵ��ַ��� 
			return nOutLen;
		}

		//ʣ����
		size_t nSurplusLen = inDestLen;
		//��ʼת�� 
		int nRet = iconv(hIconv, (char**)&pInBuf, &iInLen, &pOutBuf, &nSurplusLen);
		if (nRet != 0)
		{
			//ת���쳣
		}

		//ת������Ϊ���볤�ȼ�ȥʣ�೤��
		nOutLen = inDestLen - nSurplusLen;
		nOutLen > inDestLen ? nOutLen = inDestLen : inDestLen;

		//�ر��ַ���ת�� 
		iconv_close(hIconv);
	}
	catch (...)
	{
	}

	return nOutLen;
} 
#endif

