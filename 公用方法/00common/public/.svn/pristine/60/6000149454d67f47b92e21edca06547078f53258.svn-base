// DoubleListEx.cpp: implementation of the DerivedLock class.
//
//////////////////////////////////////////////////////////////////////

#include "utility/common/mwcorepch.h"
#include "utility/common/DoubleListEx.h"
//////////////////////////////////////////////////////////////////////////

//生成全局key
DerivedLock g_csGlobalKey;
int g_nGlobalKey = 0;

int GetGlobalKey()
{
	SafeLock safelock(&g_csGlobalKey);
	
	//key的值不取0,自增至最大值后归1
	++g_nGlobalKey;
	g_nGlobalKey<=0?g_nGlobalKey=1:g_nGlobalKey;
	
	return g_nGlobalKey;
}
//////////////////////////////////////////////////////////////////////////