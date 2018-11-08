// MsgSplice.cpp : Defines the entry point for the DLL application.
//

#include "StdAfx.h"
#include "MsgSplice.h"
#include "MsgSpliceMgr.h"



#ifdef _WIN32
#ifndef _LIB
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
#endif
#endif



CMsgSpliceMgr* g_pInstance;
DerivedLock g_VisitLock;
API_PREFIX int WINAPI InitSpliceMgr( void* pCallBack, void* pInvoke,  int Reserve1, int Reserve2, int Reserve3 )
{	
	int iRet = 0;
	
	g_VisitLock.Lock();

	if( !g_pInstance ){
	
		if( NULL==pCallBack )
			iRet = ER_PARAM;
		else
			g_pInstance = new CMsgSpliceMgr( (PFCALLBACK)pCallBack, pInvoke );
	}

	g_VisitLock.UnLock();
	
	return iRet;
}

API_PREFIX int WINAPI AddToSpliceMgr( const char* User, const char* Spno,const char* Phone,const char* Msg, int MsgFmt, int MsgLen, int PkTotal, int PkNumber,	int PkSeqId, int  TimeOut )
{
	if( !g_pInstance )
		return ER_INSTANCE_NULL;
	return g_pInstance->AddMsg( User, Spno, Phone, Msg, MsgFmt, MsgLen, PkTotal, PkNumber, PkSeqId, TimeOut );
}

API_PREFIX void WINAPI UinitSpliceMgr()
{
	g_VisitLock.Lock();
	
	if( g_pInstance )
		g_pInstance->StopSplice();	
	
	g_VisitLock.UnLock();
}