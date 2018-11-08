// MobiAreaQry.cpp : Defines the entry point for the DLL application.
//


#if !((defined  MW_WINDOWS) || (defined  WIN32))
#include <stdio.h>
#include <dlfcn.h>


typedef void* HMODULE;
#define LoadLibrary(libname) dlopen(libname,RTLD_NOW)
#define GetProcAddress(p1,p2) dlsym(p1,p2)
#define FreeLibrary(m) dlclose(m)
#define  __stdcall

typedef int (__stdcall *_GetMobiArea)( const char*,char*);
int main(int argc,char* argv[])
{
	//HMODULE hDll = LoadLibrary( "F:\\work\\MobiAreaQry\\MobiAreaQry.dll" );
	//HMODULE hDll = LoadLibrary( "MobiAreaQry.dll" );
	const char* LibName = "./MobiAreaQry.so";
	HMODULE hDll = LoadLibrary( LibName );
	char szArea[64]="";
	int iRet;
	if( !hDll ){
		printf("Load lib %s fail:%s\n", LibName, dlerror() );
		return 0;	
	}
	printf("Load IF OK\n");
	
	_GetMobiArea GetMobiArea =  (_GetMobiArea)GetProcAddress( hDll,"GetMobiArea" );
	if( GetMobiArea ){
		char  Mobile[512];// = "15919264936"; 

		while (1){
			gets( Mobile );
			if( Mobile[0] == 'q' )
				break;
			iRet = GetMobiArea( Mobile, szArea );
			printf( "%s in area[%s] %d\n ", Mobile, szArea, iRet );
			iRet+= 0;
		}		
	}
	FreeLibrary( hDll );
	return 0;
}
#else
#include "StdAfx.h"
#endif

