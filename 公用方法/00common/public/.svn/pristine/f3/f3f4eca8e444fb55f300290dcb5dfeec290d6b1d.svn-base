// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.

//ansi-c
#include "stdio.h"
//std-cpp
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <queue>
#include <limits>
#include <stack>
#include <stdexcept>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
//boost
// #include <boost/format.hpp>
// #include <boost/shared_ptr.hpp>
// #include <boost/shared_array.hpp>
// #include <boost/scoped_ptr.hpp>
// #include <boost/intrusive_ptr.hpp>
// #include <boost/enable_shared_from_this.hpp>
// #include <boost/scoped_array.hpp>
// #include <boost/lexical_cast.hpp>
// #include <boost/operators.hpp>
// #include <boost/thread.hpp>
// #include <boost/thread/mutex.hpp>
// #include <boost/thread/condition.hpp>
// #include <boost/asio/io_service.hpp>
// #include <boost/function.hpp>
// #include <boost/any.hpp>
//MW
#include "utility/common/mwcore_export.h"
#include "utility/common/dstring.h"

using namespace std;


#ifndef TYPES_H
#define TYPES_H

namespace UTI{
namespace COM{


// 32位移植到64位，以后统一使用以下数据类型

// 	使用boost的类型定义
// 	/// @brief Unsigned character. AKA: a byte
// 	typedef unsigned char uchar;
// 	/// @brief Signed 8 bit integer. AKA: char on some, but not all platforms.
// 	typedef boost::int8_t int8;
// 	typedef boost::uint8_t uint8;
// 	/// @brief 16 bit signed integer
// 	typedef boost::int16_t int16;
// 	/// @brief 16 bit unsigned integer
// 	typedef boost::uint16_t uint16;
// 	/// @brief 32 bit signed integer
// 	typedef boost::int32_t int32;
// 	/// @brief 32 bit unsigned integer
// 	typedef boost::uint32_t uint32;
// 	/// @brief 64 bit signed integer
// 	typedef boost::int64_t int64;
// 	/// @brief 64 bit unsigned integer
// 	typedef boost::uint64_t uint64;

	typedef void*			   daddress;

    typedef signed char        int8;
    typedef short              int16;
    typedef int                int32;
    typedef long long          int64;
    typedef unsigned char      uint8;
    typedef unsigned short     uint16;
    typedef unsigned int       uint32;
    typedef unsigned long long uint64;


#if defined(MW_WIN)
//---
#elif defined(MW_LINUX) || defined(MW_IOS) || defined(MW_ANDROID)
//--------
#define  	TRUE		1
#define  	FALSE		0
#define 	ASSERT(T) 	assert(T)  
#define 	LPCTSTR		char* 
// typedef 	void* HANDLE;
//typedef 	unsigned long DWORD;
// typedef 	long LONG;
typedef 	double LONGLONG;
// typedef 	int BOOL;

//--------
#else
#	error MW dont support this platform
#endif


class UTILITY_Export DObject
{
public:
  DObject() {}
  virtual ~DObject() {}
};

class UTILITY_Export DInterface
{
public:
  DInterface() {}
  virtual ~DInterface() {}
};

// typedef boost::shared_ptr<std::string>		StringPtr;
class DString;
typedef std::vector<DString>				CmdParaVect;
typedef std::vector<DString>::iterator		CmdParaVectIter;
// typedef boost::function<void (std::string)> CmdRspFunc;
class UTILITY_Export DIOsgiCmdReceiver
{
public:
  DIOsgiCmdReceiver() {}
  virtual ~DIOsgiCmdReceiver() {}
//   virtual void OnReceiveCommand(const std::string& rCmdName, CmdParaVect& rParaVect, CmdRspFunc rspFunc) = 0;
};//end class
class UTILITY_Export DOsgiCmdRegister
{
public:
	DOsgiCmdRegister() {}
	virtual ~DOsgiCmdRegister() {}
	//控制台终端接口函数
	virtual void InputCommandText(const std::string& rCmdText) = 0;
// 	virtual bool RegisterOutput(CmdRspFunc rspFunc) = 0;
	//命令接收器接口函数
	virtual bool RegisterCmdRecver(const std::string& rBundleName, DIOsgiCmdReceiver* pCmdReceiver, const std::string& rNote) = 0;
	virtual bool RemoveCmdRecver(const std::string& rBundleName) = 0;
};

typedef std::vector<std::string>			StringVect;
typedef std::vector<std::string>::iterator	StringVectIter;
// typedef boost::shared_ptr<StringVect>		StringVectPtr;

typedef std::vector<DString>				DStringVect;
typedef std::vector<DString>::iterator		DStringVectIter;
// typedef boost::shared_ptr<DStringVect>		DStringVectPtr;

}}//end namespace
using namespace UTI::COM;


#endif // TYPES_H


