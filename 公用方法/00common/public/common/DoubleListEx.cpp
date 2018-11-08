// DoubleListEx.cpp: implementation of the DerivedLock class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "DoubleListEx.h"

//////////////////////////////////////////////////////////////////////////

//����ȫ��key
DerivedLock g_csGlobalKey;
int g_nGlobalKey = 0;

int GetGlobalKey()
{
	SafeLock safelock(&g_csGlobalKey);
	
	//key��ֵ��ȡ0,���������ֵ���1
	++g_nGlobalKey;
	g_nGlobalKey<=0?g_nGlobalKey=1:g_nGlobalKey;
	
	return g_nGlobalKey;
}
//////////////////////////////////////////////////////////////////////////