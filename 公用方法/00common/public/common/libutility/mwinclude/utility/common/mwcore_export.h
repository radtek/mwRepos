#ifndef MWCOREEXPORT_H
#define MWCOREEXPORT_H

#include "utility/common/mwcorepch.h"

#if defined (MW_WIN)
#if		defined MW_BUILD_DLL
		#define MWCORE_Export	__declspec(dllexport)
	#elif defined MW_USE_DLL
		#define MWCORE_Export	__declspec(dllimport)
	#else
		#define MWCORE_Export	__declspec(dllimport)
	#endif

	#if defined UTILITY_BUILD_DLL
		#define UTILITY_Export	__declspec(dllexport)
	#elif defined UTILITY_USE_DLL
		#define UTILITY_Export	__declspec(dllimport)
	#else
		#define UTILITY_Export	__declspec(dllimport)
	#endif

	#define C_FUNC_EXPORT		extern "C" __declspec(dllexport)
	#define STDCALL				__stdcall

#elif defined(LINUX) || defined (MW_LINUX) || defined(MW_IOS) || defined(MW_ANDROID) /* !_WIN32 */
	#define MWCORE_Export
	#define UTILITY_Export
	#define C_FUNC_EXPORT extern "C"
	#define STDCALL 
#else 
	#error not suport 
#endif /* _WIN32 */

#endif //MWCOREEXPORT_H
