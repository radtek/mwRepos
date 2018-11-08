// MobiAreaQry.cpp : Defines the entry point for the DLL application.
//

//头文件
#include "StdAfx.h"
//头文件
#include "MobiAreaQry.h"
//头文件
#include <stdio.h>
//头文件
#include <stdlib.h>
//头文件
#include <time.h>
//头文件
#include <string.h>
//头文件
#include <ctype.h>
//头文件

//平台兼容性控制
#if (defined  MW_WINDOWS) || (defined  WIN32)

//DLL与LIB版本控制
#ifndef _LIB
//DLL入口
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}
#endif
#elif defined LINUX

#endif

//全局对象
CMobiAreaQry  g_MobiAreaQry;
//最后加载时间
time_t g_nLastLoadTime = 0;
//号段文件
const char * g_pMobileAreaFile = "mobilearea.txt";

// 
//ReloadMobiAreaToObj：重新加载号段到CMobiAreaQry对象obj
//obj: 存储的目标对象
//返回值：是否成功
// 
bool ReloadMobiAreaToObj(CMobiAreaQry &obj  );

//导出查询号码所属区域API，函数近对号码前7位进行校验
//参数：pMobile【入参】：需要查询的号码，
//参数：pArea【出参】：返回接口0时，该缓冲区填充所属区域信息，至少32字节
//返回值：0 查询到所属地
//返回值：1 参数有误
//返回值：2 查询不到所属地
API_PREFIX int WINAPI GetMobiArea( const char* Mobile, char* Area )
{	
	time_t nNow;
	volatile static int iCnt;

	//每天加载一次
	if( ( time(&nNow)-g_nLastLoadTime ) >= 86400/*24*3600*/ )
	{
		if( iCnt ==0 )
		{
			++iCnt;
			//重新加载
			if( ReloadMobiAreaToObj(  g_MobiAreaQry ) == true )
			{
				g_nLastLoadTime = nNow;
			}
			g_nLastLoadTime = nNow;
			--iCnt;
			//对上一次加载时间进行调整,调整成当天0点0分0秒,以便下一次0点0分0秒加载
			struct tm *tLastLoadTime;
			tLastLoadTime = localtime(&nNow);
			tLastLoadTime->tm_hour = 0;
			tLastLoadTime->tm_min = 0;
			tLastLoadTime->tm_sec = 0;
			g_nLastLoadTime = mktime(tLastLoadTime);
		}		
	}
	return g_MobiAreaQry.QryMobiArea( Mobile, Area );	
}


// 
//ReloadMobiAreaToObj：重新加载号段到CMobiAreaQry对象obj
//obj: 存储的目标对象
//返回值：是否成功
// 
bool ReloadMobiAreaToObj(CMobiAreaQry &obj  )
{
	CMobiAreaQry *pNewObj;	
	if( pNewObj = new CMobiAreaQry( false ) ){
		int iRet = pNewObj->LoadMobiSegAreaFromFile( g_pMobileAreaFile );
		if( 0==iRet ){
			obj.CopyFrom( pNewObj );
			delete pNewObj;
			return true;
		}else{
			delete pNewObj;
			return false;
		}
	}else{
		return false;
	}
}

// 
//构造函数
// 
CMobiAreaQry::CMobiAreaQry(bool bLoad)
{
	//置空
	memset( this, 0, sizeof(CMobiAreaQry) );		
	if( bLoad ){				
		//Loging
		WriteLogMsg( "First loading....\r\n" );
		//加载
		int iRet = LoadMobiSegAreaFromFile( g_pMobileAreaFile  );		
		//计时
		time( &g_nLastLoadTime );

		//调整成当天0点0分0秒,以便下一次0点0分0秒加载
		struct tm *tLastLoadTime;
		tLastLoadTime = localtime(&g_nLastLoadTime);
		tLastLoadTime->tm_hour = 0;
		tLastLoadTime->tm_min = 0;
		tLastLoadTime->tm_sec = 0;
		g_nLastLoadTime = mktime(tLastLoadTime);
	}
}

// 
//析构函数
// 
CMobiAreaQry::~CMobiAreaQry()
{	
	//Loging
	CMobiAreaQry::WriteLogMsg("886\n");
}

//
//AddAreaSegLine：把格式形如 "1nnnnnn,北京等" 一行号段区域信息加载到内存
//pLine：源号段所属地行
//成功返回 0，~0为添加失败
//
int CMobiAreaQry::AddAreaSegLine(char* pLine)
{
	char *pc, chTmp;
	char *pSeg = pLine;
	char *pArea;

	//循环解析
	for(pc=pSeg;*pc&&isdigit(*pc);++pc);

	//数字之后必须为,
	if( *pc != ','  )
		return -1;

	//首个数字必须为1
	if( pSeg[0] != '1' )
		return -2;

	//数字1xxxxxx长度为7, 或者为首个号码格式
	if( 7 != (pc-pSeg)  && 11 != (pc-pSeg) )		
		return -3;

	chTmp = pSeg[7];
	pSeg[7] = 0;
	//舍首位1取值
	int iSeg = atoi( pSeg+1 );//
	pSeg[7] = chTmp;
	
	pArea = pc+1;
	int iAreaLen = strlen( pArea );
	
	//trim right space
	pc=pArea+iAreaLen-1;
	while( iAreaLen>=1 ){
		if(  ' ' != *pc ) 
			break;
		*pc-- = '\0';
		--iAreaLen;		
	}
	//溢出判断
	if( iAreaLen<=0 || iAreaLen>=MAX_AREA_NAME_LEN )
		return -4;

	//获取参数pAreaName区域在已加载区域数组中的下表加一,不存在并且不溢出时增加区域
	int iAreaId = GetAreaIndex( pArea );

	if( iAreaId <0 )
		return iAreaId;

	m_arrSeg2Area[iSeg] = iAreaId;

	//加载次数
	++m_iLoadSegCnt;

	//返回
	return 0;
	
}

//
//pAreaName：获取参数pAreaName区域在已加载区域数组中的下表加一,不存在并且不溢出时增加区域
//pAreaName：区域名称
//返回值>0对应区域下标加一， 其他值：区域太多，溢出
	//
int CMobiAreaQry::GetAreaIndex( const char* pAreaName )
{
	//循环查找
	for( int i=m_iAreaCnt-1;i>=0;--i){
		if( 0 == strcmp(m_szAreaName[i],pAreaName) )
			return i+1;
	}

	//区域太多，溢出
	if( m_iAreaCnt >=MAX_AREA_NUM ){
		return -5;
	}
	
	strcpy( m_szAreaName[m_iAreaCnt++], pAreaName );

	//返回
	return (m_iAreaCnt);
}

//
//LoadMobiSegAreaFromFile：从参数[pFile]指向文件加载号段区域信息到内存
//pFile：文件
//返回值：
	//
int CMobiAreaQry::LoadMobiSegAreaFromFile(const char* pFile)
{
	FILE* file;
	int iRet = 0;
	char szLogMsg[512];

	//Loging
	WriteLogMsg( szLogMsg, sprintf(szLogMsg,"load from %s ....\r\n", g_pMobileAreaFile) );

	//Open File
	if( file = fopen(pFile,"rb" ) ){
		long length = 0;
		//文件定位
		fseek( file, 0, SEEK_END );
		length = ftell( file );
		//文件定位
		fseek( file, 0, SEEK_SET );
		if( length >0 ){
			char* buf = new char[ 1+length+1 ];
			buf[0] = 0;
			buf[1+length] = 0;
			//缓冲区分配
			if ( fread( buf+1, length, 1, file ) == 1 ) {					
				///////////////////////////////
				char *pCRLF,*pLine = buf+1;				
				//循环读取
				while( *pLine )
				{						
					//解析行
					if( pCRLF = strchr( pLine, '\n' ) ){							
						*pCRLF = 0;
						if( '\r' == *(pCRLF-1) ){
							*(pCRLF-1) = 0;
						}
						
						//添加
						if( iRet = AddAreaSegLine( pLine ) ){
							++m_iFailLines;
							//Loging
							WriteLogMsg( pLine, 0 );								
						}					
						
						pLine = pCRLF + 1;
					}else{
						//添加
						if( iRet = AddAreaSegLine( pLine ) ){
							++m_iFailLines;
							//Loging
							WriteLogMsg( pLine, 0 );
						}
						break;
					}
				}					
				///////////////////////////////
			}
			delete [] buf;
		}			
		fclose( file );		
	}else{		
		//Loging
		WriteLogMsg( szLogMsg, sprintf(szLogMsg,"load fail, can't open %s\r\n", g_pMobileAreaFile) );
		return ER_FILE_NOT_EXIST;
	}	 
	//Loging
	WriteLogMsg( szLogMsg, sprintf(szLogMsg,"load over: area=%d  seg=%d fail_line=%d\r\n", m_iAreaCnt, m_iLoadSegCnt,m_iFailLines ) );
	return 0;
}

//从参数指向对象复制号段区域信息到自己
void CMobiAreaQry::CopyFrom( const CMobiAreaQry *pObj )
{
	memcpy( this, pObj, sizeof(CMobiAreaQry) );
}

// 
//查询号码所属地，仅对前7位进行有效性判定
// 
int CMobiAreaQry::QryMobiArea( const char* Mobile, char* Area )
{

	char szMobiSeg[8];

	//参数判断
	if ( !Mobile || !Area )
		return ER_PARAM_ERROR;

	//判断前7位
	memcpy( szMobiSeg, Mobile, 8 );
	szMobiSeg[7] = 0;

	//进一步判断
	if ( szMobiSeg[0] != '1' )
		return ER_PARAM_ERROR;

	//进一步判断
	for ( int i=1;i<7;i++){
		if ( !isdigit(szMobiSeg[i]) )
			return ER_PARAM_ERROR;	
	}
	
	//区号
	int iAreaId = m_arrSeg2Area[atoi(szMobiSeg+1) ];
	if ( iAreaId<=0 ){
		return ER_SEG_NOT_EXIST;
	}
	
	//复制
	strcpy( Area, m_szAreaName[iAreaId-1] );

	//返回
  	return 0;

}

//
//WriteLogMsg：写日志函数
//pLogMsg：日志缓冲区
//iSize：日志长度
//返回值:无
//
void CMobiAreaQry::WriteLogMsg(const char *pLogMsg, int iSize )
{
	//定义变量
	//并初始化
	//开始了
	char szTimeStamp[64];
	FILE *fpLog;
	time_t tNow = time(NULL);			
	struct tm lt = *localtime( &tNow );		
	if( !iSize ) iSize = strlen( pLogMsg );	
	//打开文件
	if( fpLog = fopen("mobiareaqry.log", "a") ){				
		//写文件
		fwrite( szTimeStamp,1,sprintf( szTimeStamp, "%d-%02d-%02d %02d:%02d:%02d ", lt.tm_year+1900, lt.tm_mon+1, lt.tm_mday, lt.tm_hour,lt.tm_min,lt.tm_sec ), fpLog );
		fwrite( pLogMsg,1, iSize, fpLog );
		if( pLogMsg[iSize-1]!='\n' )	
			fwrite( "\r\n",1, 2, fpLog );
		fflush( fpLog );
		//关闭文件
		fclose( fpLog );
	}
}

