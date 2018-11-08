// MobiAreaQry.cpp : Defines the entry point for the DLL application.
//

//ͷ�ļ�
#include "StdAfx.h"
//ͷ�ļ�
#include "MobiAreaQry.h"
//ͷ�ļ�
#include <stdio.h>
//ͷ�ļ�
#include <stdlib.h>
//ͷ�ļ�
#include <time.h>
//ͷ�ļ�
#include <string.h>
//ͷ�ļ�
#include <ctype.h>
//ͷ�ļ�

//ƽ̨�����Կ���
#if (defined  MW_WINDOWS) || (defined  WIN32)

//DLL��LIB�汾����
#ifndef _LIB
//DLL���
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

//ȫ�ֶ���
CMobiAreaQry  g_MobiAreaQry;
//������ʱ��
time_t g_nLastLoadTime = 0;
//�Ŷ��ļ�
const char * g_pMobileAreaFile = "mobilearea.txt";

// 
//ReloadMobiAreaToObj�����¼��غŶε�CMobiAreaQry����obj
//obj: �洢��Ŀ�����
//����ֵ���Ƿ�ɹ�
// 
bool ReloadMobiAreaToObj(CMobiAreaQry &obj  );

//������ѯ������������API���������Ժ���ǰ7λ����У��
//������pMobile����Ρ�����Ҫ��ѯ�ĺ��룬
//������pArea�����Ρ������ؽӿ�0ʱ���û������������������Ϣ������32�ֽ�
//����ֵ��0 ��ѯ��������
//����ֵ��1 ��������
//����ֵ��2 ��ѯ����������
API_PREFIX int WINAPI GetMobiArea( const char* Mobile, char* Area )
{	
	time_t nNow;
	volatile static int iCnt;

	//ÿ�����һ��
	if( ( time(&nNow)-g_nLastLoadTime ) >= 86400/*24*3600*/ )
	{
		if( iCnt ==0 )
		{
			++iCnt;
			//���¼���
			if( ReloadMobiAreaToObj(  g_MobiAreaQry ) == true )
			{
				g_nLastLoadTime = nNow;
			}
			g_nLastLoadTime = nNow;
			--iCnt;
			//����һ�μ���ʱ����е���,�����ɵ���0��0��0��,�Ա���һ��0��0��0�����
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
//ReloadMobiAreaToObj�����¼��غŶε�CMobiAreaQry����obj
//obj: �洢��Ŀ�����
//����ֵ���Ƿ�ɹ�
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
//���캯��
// 
CMobiAreaQry::CMobiAreaQry(bool bLoad)
{
	//�ÿ�
	memset( this, 0, sizeof(CMobiAreaQry) );		
	if( bLoad ){				
		//Loging
		WriteLogMsg( "First loading....\r\n" );
		//����
		int iRet = LoadMobiSegAreaFromFile( g_pMobileAreaFile  );		
		//��ʱ
		time( &g_nLastLoadTime );

		//�����ɵ���0��0��0��,�Ա���һ��0��0��0�����
		struct tm *tLastLoadTime;
		tLastLoadTime = localtime(&g_nLastLoadTime);
		tLastLoadTime->tm_hour = 0;
		tLastLoadTime->tm_min = 0;
		tLastLoadTime->tm_sec = 0;
		g_nLastLoadTime = mktime(tLastLoadTime);
	}
}

// 
//��������
// 
CMobiAreaQry::~CMobiAreaQry()
{	
	//Loging
	CMobiAreaQry::WriteLogMsg("886\n");
}

//
//AddAreaSegLine���Ѹ�ʽ���� "1nnnnnn,������" һ�кŶ�������Ϣ���ص��ڴ�
//pLine��Դ�Ŷ���������
//�ɹ����� 0��~0Ϊ���ʧ��
//
int CMobiAreaQry::AddAreaSegLine(char* pLine)
{
	char *pc, chTmp;
	char *pSeg = pLine;
	char *pArea;

	//ѭ������
	for(pc=pSeg;*pc&&isdigit(*pc);++pc);

	//����֮�����Ϊ,
	if( *pc != ','  )
		return -1;

	//�׸����ֱ���Ϊ1
	if( pSeg[0] != '1' )
		return -2;

	//����1xxxxxx����Ϊ7, ����Ϊ�׸������ʽ
	if( 7 != (pc-pSeg)  && 11 != (pc-pSeg) )		
		return -3;

	chTmp = pSeg[7];
	pSeg[7] = 0;
	//����λ1ȡֵ
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
	//����ж�
	if( iAreaLen<=0 || iAreaLen>=MAX_AREA_NAME_LEN )
		return -4;

	//��ȡ����pAreaName�������Ѽ������������е��±��һ,�����ڲ��Ҳ����ʱ��������
	int iAreaId = GetAreaIndex( pArea );

	if( iAreaId <0 )
		return iAreaId;

	m_arrSeg2Area[iSeg] = iAreaId;

	//���ش���
	++m_iLoadSegCnt;

	//����
	return 0;
	
}

//
//pAreaName����ȡ����pAreaName�������Ѽ������������е��±��һ,�����ڲ��Ҳ����ʱ��������
//pAreaName����������
//����ֵ>0��Ӧ�����±��һ�� ����ֵ������̫�࣬���
	//
int CMobiAreaQry::GetAreaIndex( const char* pAreaName )
{
	//ѭ������
	for( int i=m_iAreaCnt-1;i>=0;--i){
		if( 0 == strcmp(m_szAreaName[i],pAreaName) )
			return i+1;
	}

	//����̫�࣬���
	if( m_iAreaCnt >=MAX_AREA_NUM ){
		return -5;
	}
	
	strcpy( m_szAreaName[m_iAreaCnt++], pAreaName );

	//����
	return (m_iAreaCnt);
}

//
//LoadMobiSegAreaFromFile���Ӳ���[pFile]ָ���ļ����غŶ�������Ϣ���ڴ�
//pFile���ļ�
//����ֵ��
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
		//�ļ���λ
		fseek( file, 0, SEEK_END );
		length = ftell( file );
		//�ļ���λ
		fseek( file, 0, SEEK_SET );
		if( length >0 ){
			char* buf = new char[ 1+length+1 ];
			buf[0] = 0;
			buf[1+length] = 0;
			//����������
			if ( fread( buf+1, length, 1, file ) == 1 ) {					
				///////////////////////////////
				char *pCRLF,*pLine = buf+1;				
				//ѭ����ȡ
				while( *pLine )
				{						
					//������
					if( pCRLF = strchr( pLine, '\n' ) ){							
						*pCRLF = 0;
						if( '\r' == *(pCRLF-1) ){
							*(pCRLF-1) = 0;
						}
						
						//���
						if( iRet = AddAreaSegLine( pLine ) ){
							++m_iFailLines;
							//Loging
							WriteLogMsg( pLine, 0 );								
						}					
						
						pLine = pCRLF + 1;
					}else{
						//���
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

//�Ӳ���ָ������ƺŶ�������Ϣ���Լ�
void CMobiAreaQry::CopyFrom( const CMobiAreaQry *pObj )
{
	memcpy( this, pObj, sizeof(CMobiAreaQry) );
}

// 
//��ѯ���������أ�����ǰ7λ������Ч���ж�
// 
int CMobiAreaQry::QryMobiArea( const char* Mobile, char* Area )
{

	char szMobiSeg[8];

	//�����ж�
	if ( !Mobile || !Area )
		return ER_PARAM_ERROR;

	//�ж�ǰ7λ
	memcpy( szMobiSeg, Mobile, 8 );
	szMobiSeg[7] = 0;

	//��һ���ж�
	if ( szMobiSeg[0] != '1' )
		return ER_PARAM_ERROR;

	//��һ���ж�
	for ( int i=1;i<7;i++){
		if ( !isdigit(szMobiSeg[i]) )
			return ER_PARAM_ERROR;	
	}
	
	//����
	int iAreaId = m_arrSeg2Area[atoi(szMobiSeg+1) ];
	if ( iAreaId<=0 ){
		return ER_SEG_NOT_EXIST;
	}
	
	//����
	strcpy( Area, m_szAreaName[iAreaId-1] );

	//����
  	return 0;

}

//
//WriteLogMsg��д��־����
//pLogMsg����־������
//iSize����־����
//����ֵ:��
//
void CMobiAreaQry::WriteLogMsg(const char *pLogMsg, int iSize )
{
	//�������
	//����ʼ��
	//��ʼ��
	char szTimeStamp[64];
	FILE *fpLog;
	time_t tNow = time(NULL);			
	struct tm lt = *localtime( &tNow );		
	if( !iSize ) iSize = strlen( pLogMsg );	
	//���ļ�
	if( fpLog = fopen("mobiareaqry.log", "a") ){				
		//д�ļ�
		fwrite( szTimeStamp,1,sprintf( szTimeStamp, "%d-%02d-%02d %02d:%02d:%02d ", lt.tm_year+1900, lt.tm_mon+1, lt.tm_mday, lt.tm_hour,lt.tm_min,lt.tm_sec ), fpLog );
		fwrite( pLogMsg,1, iSize, fpLog );
		if( pLogMsg[iSize-1]!='\n' )	
			fwrite( "\r\n",1, 2, fpLog );
		fflush( fpLog );
		//�ر��ļ�
		fclose( fpLog );
	}
}

