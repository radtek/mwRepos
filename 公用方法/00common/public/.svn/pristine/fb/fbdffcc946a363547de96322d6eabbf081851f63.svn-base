/*****************************************************************************************************
copyright (C),2015-2020,montnets .Co.,Ltd.

FileName     : mwcorepch.h
Author       : Shijh      Version : 1.0    Date: 2016年04月21日
Description  : MW预编译头文件
Version      : 1.0
Function List:

History      :
<author>       <time>             <version>            <desc>
Shijh       2016年04月21日          1.0          build this moudle
******************************************************************************************************/

#ifdef _MSC_VER
#	define  MW_COMPILER_MSC
#	pragma warning(disable:4251) //disable VC warning about dll linkage required (for private members?)
#	pragma warning(disable:4275) //disable warning about non dll-interface base class.
#	pragma warning(disable:4996) //disable VC warning that std library may be unsafe
//#	pragma warning(disable:4290) //C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#	pragma warning(disable:4396) //disable 'boost::operator !=' : the inline specifier cannot be used when a friend declaration refers to a specialization of a function template
#	pragma warning(disable:4018) //warning C4018: “<”: 有符号/无符号不匹配
#	pragma warning(disable:4244) //warning C4244: “=”: 从“uint64”转换到“uint32”，可能丢失数据
#	pragma warning(disable:4996) //warning C4996: 'access': The POSIX name
#	pragma warning(disable:4819) //warning C4819: 该文件包含不能在当前代码页(936)中表示的字符
#	pragma warning(disable:4800) //warning C4800: “BOOL”: 将值强制为布尔值“true”或“false”(性能警告)
#	pragma warning(disable:4200) //warning C4200: 使用了非标准扩展 : 结构/联合中的零大小数组
#else
#	define MW_COMPILER_GCC
//#	pragma GCC diagnostic ignored "-Wno-eof-newline"
//#	pragma GCC diagnostic ignored "-Weof-newline"
#endif

#pragma once

#ifndef MWCOREPCH_H
#define MWCOREPCH_H

#define BOOST_DISABLE_ASSERTS
#define BOOST_ALL_DYN_LINK

#ifdef _WIN32
#	define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
#	define NOMINMAX                 // Do not define min & max a macros: l'histoire anciene
// #	include <windows.h>
#endif // _WIN32

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define   _WIN32_WINNT   0x0603
#pragma warning (disable:4103)
#pragma warning (disable:4503)
#pragma warning (disable:4786)
#pragma warning (disable:4267)
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define NO_WARN_MBCS_MFC_DEPRECATION


//MW Macro
#if defined(WIN32)
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#	define MW_WIN
#	define MW_WIN32		"win32_app"
#	define MW_CORR_OS		MW_WIN32
#	define MW_32BIT
#elif defined(WIN64)
#	define MW_WIN
#	define MW_WIN64		"win64_app"
#	define MW_CORR_OS		MW_WIN64
#	define MW_64BIT
#elif defined(IOS32) || defined(IOS64)
#	define MW_IOS
#	if __LP64__
#		define MW_IOS64	"ios64_app"
#		define MW_CORR_OS	MW_IOS64
#		define MW_64BIT
#	else
#		define MW_IOS32	"ios32_app"
#		define MW_CORR_OS	MW_IOS32
#		define MW_32BIT
#	endif
#elif defined(ANDROID)
#	define MW_ANDROID
#	include "bits/wordsize.h"
#	if __WORDSIZE==64
#		define MW_ANDROID64	"android64_app"
#		define MW_CORR_OS		MW_ANDROID64
#		define MW_64BIT
#	else
#		define MW_ANDROID32	"android32_app"
#		define MW_CORR_OS		MW_ANDROID32
#		define MW_32BIT
#	endif
#elif defined(linux) || defined(__linux) || defined(__linux__)
#	define MW_LINUX
#	include "bits/wordsize.h"
#	if __WORDSIZE==64
#		define MW_LINUX64		"linux64_app"
#		define MW_CORR_OS		MW_LINUX64
#		define MW_64BIT
#	else
#		define MW_LINUX32		"linux32_app"
#		define MW_CORR_OS		MW_LINUX32
#		define MW_32BIT
#	endif
#endif


#if defined(_DEBUG)
#	define MW_D_VER	"debug_ver"
#elif defined(MW_IOS) && DEBUG > 0
#	define MW_D_VER	"debug_ver"
#else
#	define MW_D_VER	"release_ver"
#endif

#if defined(MW_WIN)
#	define snprintf		_snprintf_s
#endif

//using static fast lib
#define QUICKFAST_HAS_DLL 0

////////////////////////
// Doxygen documentation

/// @mainpage
/// This project is an implementation of the CORE Protcol.
///  http://www.fixprotocol.org/CORE
///
/// <h3>CORE</h3>
/// CORE is a standard for serializing an <b><i>application data type</i></b>
/// into a <b><i>message</i></b>.
/// The goal is to minimize the size of the encoded message thereby minimizing the
/// bandwidth required to transmit the information.  Ideally this should be done
/// without requiring excessive CPU useage.
///
/// This is achieved by creating a custom encoding/decoding compression strategy
/// for each type of messsage using a set of basic codec instructions.  This strategy
/// is called a <b><i>template</i></b>.   More than one template can be (and usually
/// is) defined.
/// There is a many-to-many correspondence between application data types and templates.
/// The encoder is free to choose the most effect template to apply to each message that
/// it encodes.
///
/// Templates are most commonly represented as XML documents.  See the specification
/// available from the URL above for details about the XML schema.
///
/// The template file is shared between both counterparties via out-of-band
/// communication. It is vital that both counterparties be using the same set of templates.
///
/// <h3>Using MW::CORE in an application that receives CORE data -- low level approach.</h3>
/// To receive CORE encoded data, a typical application would:<ul>
/// <li>Use a MW::CORE::Codecs::XMLTemplateParser to read a template file and produce
/// a MW::CORE::Codecs::TemplateRegistry.</li>
/// <li>Create a MW::CORE::Codecs::Decoder and supply it with the template registry to use.</li>
/// <li>Create an object that implements the MW::CORE::Codecs::DataSource interface to supply the
/// incoming stream of data to the decoder.</li>
/// <li>Create an object that implements the MW::CORE::Messages::MessageBuilder interface to
/// accept the decoded fields and store them in a message object.</li>
/// <li>Each time the data source has a complete message available<ul>
///  <li>Pass the data source and the message to the decoder's decodeMessage() method.</li>
///  <li>When the decoder calls the MessageBuilder::endMessage method, process the decoded message</li></ul>
/// </li></ul>
///
/// The application should catch exceptions thrown by the decoder.  The data source should have
/// a method for resynchronizing the input after an exception has disrupted the decoding of an
/// incoming message.
///
/// <h3>"Prepackaged" decoders.</h3>
/// The MW::CORE::Codecs::SynchronousDecoder and MW::CORE::Codecs::MulticastDecoder
/// objects support simpler approach to writing a decoder application.  To use either
/// of these objects, you need an application object that can handle the incoming messages.
/// This object should implement the MW::CORE::Codecs::MessageConsumer interface.
///
/// The SynchronousDecoder or MulticastDecoder handles the details of receiving and decoding
/// incoming messages.  Each time a complete messages has been decoded, the consumeMessage()
/// method of your MessageConsumer object will be called.
///
/// The SynchronousDecoder can be used to receive messages from a DataSource that either has
/// all incoming data available (reading from a file, for example.) or that blocks waiting
/// for incoming data to arrive when necessary.
///
/// The MulticastDecoder using nonblocking boost::asio to dispatch incoming data to the
/// decoder out of an event loop run by a boost::io_service object.  Multiple input channels
/// can be supported--each with it's own independent decoder.  Multiple threads can be used.
/// be used, to allow parallel processing, although each decoder will only be executing in one
/// thread at a time.  The documentation for MulticastDecoder provides more details on how it
/// should be used.
/// You might also want to read the boost::asio documentation for details.
///
/// <h3>Using MW::CORE in an application that sends CORE data.</h3>
/// To send CORE encoded data, a typical application would:<ul>
/// <li>Use a MW::CORE::Codecs::XMLTemplateParser to read a template file and produce
/// a MW::CORE::Codecs::TemplateRegistry.</li>
/// <li>Create a MW::CORE::Codecs::Encoder and supply it with the template registry to use.</li>
/// <li>Create an object that implements the MW::CORE::Codecs::DataDestination interface to handle
/// the CORE encoded data that will be generated.</li>
/// <li>Whenever data is available to be sent:<ul>
///  <li>Create an empty MW::CORE::Messages::Message</li>
///  <li>Add fields to the message by <ul>
///   <li>creating a MW::CORE::Messages::FieldIdentity to identify the field, and</li>
///   <li>creating the appropriate implementation of MW::CORE::Messages::Field to represent the field value.</li>
///   <li>adding the field to the message using the addField() method.</li></ul></li>
///  <li>Pass the populated message and the data destination to the encoders encodeMessage() method.</li>
/// </ul></ul>
///
/// The encoder will send the encoded data to the DataDestination, then call the DataDestination::endMessage()
/// method.  This call is a signal for the destination to actually transmit or otherwise process the encoded
/// message.
///
/// The application should catch exceptions thrown by the encoder and tell the the data destination
/// to discard any partially encoded information.
///
/// <h4>
/// For examples of how MW::CORE can be used in an application </h4>
/// @see MW::CORE::InterpretCORE
/// @see MW::CORE::MulticastInterpreter
/// @see MW::CORE::PerformanceTest
///
/// These examples use some support programs
/// @see MW::CORE::FileToTCP
/// @see MW::CORE::FileToMulticast
///
/// @todo: Discuss threading issues.
/// @todo: Discuss message buffering issues.
///
/// This page was generated from comments in src/common/MWCOREPch.h

/// @brief General utility/overhead classes used throughout the rest of the
/// system are in the ::MW::CORE namespace.
///
/// Source files for elements in this namespace are in the src/Common directory.
///
/// <i>This page was generated from comments in src/common/MWCOREPch.h</i>
namespace MW{
	namespace CORE{

		/// @brief A CORE encoder and decoder.
		///
		/// This namespace focuses on templates, codecs, and the encoding/decoding process, not on
		/// the use of the application data.
		///
		/// MW::CORE::Codecs::Encoder is the encoder. <br>
		/// MW::CORE::Codecs::Decoder is the decoder. <br>
		/// MW::CORE::Codecs::XMLTemplateParser parses the templates.
		///
		///  @see XMLTemplateParser for more detailed information about parsing XML into templates.
		///
		/// Source files for elements in this namespace are in the src/Codecs directory.
		///
		/// <i>This page was generated from comments in src/common/MWCOREPch.h</i>
		namespace Codecs{}
		/// @brief Classes for managing CORE application data: messages and fields.
		///
		/// This namespace focuses on using the application data.  It has no knowledge
		/// of the encoding/decoding process.  In particular nothing in this namespace
		/// should know anything about the ::MW::CORE::Codecs namespace.
		///
		/// Application data is sent to and from MW::CORE via MW::CORE::Messages::Message
		/// objects.  A Message is a collection of MW::CORE::Messages::Field objects.
		/// Each field within the message is identified by a MW::CORE::Messages::FieldIdentity.
		///
		/// Source files for elements in this namespace are in src/Messages directory.
		///
		/// <i>This page was generated from comments in src/common/MWCOREPch.h</i>
		namespace Messages{}
	}
}

namespace UTI{
	namespace COM{

	}
}

using namespace UTI::COM;
#include <string.h>
#include <new>
#include <vector>
using namespace std;

#include <iostream>
#include <fstream>
using namespace std;

// #import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF", "EndOfFile")

#endif //MWCOREPCH_H

