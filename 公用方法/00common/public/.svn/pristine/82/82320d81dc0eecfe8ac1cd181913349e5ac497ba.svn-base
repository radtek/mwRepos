
#ifndef _MSG_SPLICE_H
#define _MSG_SPLICE_H

#if (defined  MW_WINDOWS) || (defined  WIN32)

#define API_PREFIX 

#ifdef _LIB
#undef  WINAPI
#define WINAPI 
#endif


#else

#define API_PREFIX extern "C"
#define WINAPI 
#endif



API_PREFIX int WINAPI InitSpliceMgr(void* pCallBack, void* pInvoke,  int Reserve1, int Reserve2, int Reserve3 );
API_PREFIX int WINAPI AddToSpliceMgr( const char* User, const char* Spno,const char* Phone,const char* Msg, int MsgFmt, int MsgLen, int PkTotal, int PkNumber,	int PkSeqId, int  TimeOut );
API_PREFIX void WINAPI UinitSpliceMgr( );

#endif