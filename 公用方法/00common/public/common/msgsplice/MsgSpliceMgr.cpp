// MsgSpliceMgr.cpp: implementation of the CMsgSpliceMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "MsgSpliceMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int CSameMsgMgr::WideCharToMultiByte(
		unsigned int CodePage,            // code page
		unsigned long dwFlags,            // performance and mapping flags
		const wchar_t* lpWideCharStr,    // wide-character string
		int cchWideChar,          // number of chars in string
		char* lpMultiByteStr,     // buffer for new string
		int cbMultiByte,          // size of buffer
		const char* lpDefaultChar,     // default for unmappable chars
		int* lpUsedDefaultChar  // set when default char used
		)
{
    int iRet = 0;
	//WINDOWS编译环境
#if (defined  MW_WINDOWS) || (defined  WIN32)
    iRet = ::WideCharToMultiByte(CodePage,dwFlags,lpWideCharStr,
		cchWideChar,lpMultiByteStr,
		cbMultiByte,lpDefaultChar,
		lpUsedDefaultChar);
	//linux编译环境
#elif defined LINUX
    if(CP_ACP == CodePage)
    {
        setlocale(LC_ALL,LC_NAME_zh_CN_DEFAULT/*"zh_CN.gb18030"*/);
    }
    else if(CP_UTF8 == CodePage)
    {
        setlocale(LC_ALL,LC_NAME_zh_CN_UTF8);
    }
    else
    {
        setlocale(LC_ALL,LC_NAME_zh_CN_DEFAULT);
    }
    //获取所需缓冲区大小
    if (0 == cbMultiByte)
    {
        //char dest[8192+1] = {0};
        iRet = wcstombs(NULL, lpWideCharStr, 0);
    }
    iRet = wcstombs(lpMultiByteStr, lpWideCharStr, cbMultiByte);
    if(-1 == iRet)
    {
        iRet = 0;
    }
    setlocale(LC_ALL,NULL);
	
#endif
    return iRet;
}

//************************************
// Description:将US2字符串转为ASCII字符串
// Parameter: [IN] const char *InMsg    : 原始字符串
//            [IN] int InLen            : 原始字符串长度
//            [OUT]char *Data           : ASCII串
//            [OUT]int *Len             : ASCII串长度
// Returns:   int                       0:成功  非0:失败
//************************************
/*
int CSameMsgMgr::US2ToAscii(const char *InMsg, int InLen, char *Data, int *Len)
{
    wchar_t uMsg[2048] = {0};
    int  uLen = 0;
    char temp[2048] = {0};
	
    try
    {
        uLen = InLen/2;
        memset(uMsg, '\0', sizeof(uMsg));
		
        for (int i=0; i<uLen; ++i)
        {
            uMsg[i] = InMsg[2*i] & 0xff;
            uMsg[i] = uMsg[i] << 8;
            uMsg[i] = uMsg[i] + (InMsg[2*i+1] & 0xff);
        }
        uLen = CSameMsgMgr::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, uMsg, uLen, temp, sizeof(temp), NULL, NULL);
        *Len = uLen;
        memcpy(Data, temp, uLen);
        return 0;
    }
    catch(...)
    {
    }
    return -99;
}*/


int CSameMsgMgr::US2ToAsciiEx(const char *InMsg, int InLen, string& strAscii)
{	
	int iRet = 0;
    try
    {   
		string strUCS2(InMsg, InLen);

		//转换网络字节
        for (int i=0; i<strUCS2.size(); i+=2)
        {
			swap(strUCS2[i], strUCS2[i+1]);			
        }

		//准换字符集
		strAscii.resize(strUCS2.size());
        int uLen = CSameMsgMgr::WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, (wchar_t*)strUCS2.data(), strUCS2.size()/2, (char*)strAscii.data(), strAscii.capacity(), NULL, NULL);
		strAscii.erase(uLen);
		iRet = 0;
    }
    catch(...)
    {
		iRet = -99;
    }
    return iRet;
}

//构造时至少需要一个元素
CSameMsgMgr::CSameMsgMgr( CMsgInfor& msg  )
{
	MsgList.push_back( msg );		
}	

//回调超时的或者所有的并清楚，返回剩余元素数量
int CSameMsgMgr::OnSpliceTimeOut( BOOL bTickAll, PFCALLBACK pCallBackFun,void* pInvoke )
{
	long Now;

	list<CMsgInfor>::iterator it;						

	for(  time(&Now),it=MsgList.begin();it!=MsgList.end(); ){

		if( bTickAll ){			
			
			int nMsgFmt = it->iMsgFmt;
			//int nOutLen = it->strMsg.size();
			//char szOutMsg[2048+1] = {0};
			string szOutMsg;
			if (8 == it->iMsgFmt)
			{
				CSameMsgMgr::US2ToAsciiEx(it->strMsg.c_str(), it->strMsg.size(), szOutMsg);
				nMsgFmt = 15;
			}
			else
			{
				//memcpy(szOutMsg, it->strMsg.c_str(), nOutLen);
				szOutMsg = it->strMsg;
			}
			pCallBackFun( pInvoke, 886, it->strUser.c_str(), it->strSpno.c_str(), it->strPhone.c_str(), szOutMsg.c_str()/*it->strMsg.c_str()*/, nMsgFmt/*it->iMsgFmt*/, szOutMsg.size()/*it->strMsg.size()*/, it->iPkTotal, it->iPkNumber, it->iPkSeqId );

			MsgList.erase( it++ );

		}else if( Now > it->iTimeOut ){

			int nMsgFmt = it->iMsgFmt;
			//int nOutLen = it->strMsg.size();
			//char szOutMsg[2048+1] = {0};
			string szOutMsg;
			if (8 == it->iMsgFmt)
			{
				//CSameMsgMgr::US2ToAscii(it->strMsg.c_str(), it->strMsg.size(), szOutMsg, &nOutLen);
				CSameMsgMgr::US2ToAsciiEx(it->strMsg.c_str(), it->strMsg.size(), szOutMsg);
				nMsgFmt = 15;				
			}
			else
			{
				//memcpy(szOutMsg, it->strMsg.c_str(), nOutLen);
				szOutMsg = it->strMsg;
			}
			pCallBackFun( pInvoke, 001, it->strUser.c_str(), it->strSpno.c_str(), it->strPhone.c_str(), szOutMsg.c_str(), nMsgFmt, szOutMsg.size(), it->iPkTotal, it->iPkNumber, it->iPkSeqId );
			

			MsgList.erase( it++ );

		}else{

			it++;

		}		
	}		
	return MsgList.size();
}

//拼接信息并回调
void CSameMsgMgr::OnSpliceOk(PFCALLBACK pCallBackFun,void* pInvoke)
{
	string strMsg;			
	list<CMsgInfor>::iterator it;		
	
	for( it=MsgList.begin();it!=MsgList.end();++it )
		strMsg += it->strMsg;			
		
	it=MsgList.begin();		
	
	if( it!=MsgList.end() ){

		int nMsgFmt = it->iMsgFmt;
		//int nOutLen = strMsg.size();
		//char szOutMsg[2048+1] = {0};
		string szOutMsg;
		if (8 == it->iMsgFmt)
		{
			//CSameMsgMgr::US2ToAscii(strMsg.c_str(), strMsg.size(), szOutMsg, &nOutLen);
			CSameMsgMgr::US2ToAsciiEx(strMsg.c_str(), strMsg.size(), szOutMsg);
			nMsgFmt = 15;
		}
		else
		{
			//memcpy(szOutMsg, strMsg.c_str(), nOutLen);
			szOutMsg = strMsg;
		}
		pCallBackFun( pInvoke, 0, it->strUser.c_str(), it->strSpno.c_str(), it->strPhone.c_str(), szOutMsg.c_str(), nMsgFmt/*it->iMsgFmt*/, szOutMsg.size()/*strMsg.size()*/, it->iPkTotal, it->iPkTotal, it->iPkSeqId );

	}		
	
}

//参数[msg]是否和本组具有相同特认证信息
bool CSameMsgMgr::IsMatch( CMsgInfor& msg )
{
	list<CMsgInfor>::iterator it = MsgList.begin();

	if( it != MsgList.end() )//比较一个就可以了
		return (bool)( msg.iPkTotal==it->iPkTotal && msg.iPkSeqId==it->iPkSeqId &&  !it->strSpno.compare(msg.strSpno ) && !it->strPhone.compare( msg.strPhone) );

	return false;
}

//本组是否可以拼接了
bool CSameMsgMgr::IsSpliceOk()
{
	if( !MsgList.empty() )		
		return (bool)(MsgList.begin()->iPkTotal==(int)MsgList.size() );

	return false;
}

//按照iPkNumber顺序将msg插入到本组
int CSameMsgMgr::OrderInsertSibling(CMsgInfor& msg)
{
	list<CMsgInfor>::iterator it;
	int iPkNumber;

	for( it=MsgList.begin();it!=MsgList.end();++it ){

		iPkNumber = it->iPkNumber;

		if( msg.iPkNumber == iPkNumber )	
			return 0;

		if( msg.iPkNumber < iPkNumber )
			break;

	}	
	
	MsgList.insert( it, msg );

	return MsgList.size();		
}	



CMsgSpliceMgr::CMsgSpliceMgr(PFCALLBACK pCallBackFun, void* pInvoke)
{	
	m_pCallBackFun = pCallBackFun;

	m_pInvoke = pInvoke;		

	m_lSpliceOkNum = 0;

	m_bExit = FALSE;

	CAdapter::InitCompatibleEvent( m_EventNewMsg, TRUE );
		
	CAdapter::CreateCompatibleThread( _ThreadAsynDeal, this );
}

CMsgSpliceMgr::~CMsgSpliceMgr()
{
	CAdapter::UnInitCompatibleEvent( m_EventNewMsg );
}	

//添加信息
int CMsgSpliceMgr::AddMsg(const char* User, const char* Spno,const char* Phone,const char* Msg, int MsgFmt, int MsgLen, int PkTotal, int PkNumber,	int PkSeqId, int  TimeOut )
{		
	if( !User || !Spno || !Phone || !Msg  || MsgLen<=0  || PkTotal<=0 || PkNumber<=0 || PkNumber>PkTotal || TimeOut<=0 ){

		return ER_PARAM;

	}else if( m_bExit ){

		return ER_INSTANCE_EXIT;

	}else{
		
		int iRet;			
		list<CSameMsgMgr>::iterator it;			
		CMsgInfor MsgInfo(User, Spno,Phone,Msg, MsgFmt, MsgLen, PkTotal, PkNumber,	PkSeqId, TimeOut );			
		SafeLock safelock( &m_csVisit );
				
		for(it=m_MsgList.begin();it!=m_MsgList.end();++it){

			if( it->IsMatch( MsgInfo ) ){	

				iRet = it->OrderInsertSibling( MsgInfo );

				if( iRet == 0  ) 
					return ER_PKNUMBER_REPEAT;

				if( iRet == PkTotal ){

					CAdapter::InterlockedIncrement( &m_lSpliceOkNum );						

					CAdapter::SetCompatibleEvent( m_EventNewMsg );

				}

				return 0;
			}
		}		
		
		m_MsgList.push_back( CSameMsgMgr( MsgInfo ) );					
		
		return 0;
		
	}		
}

//停止拼接服务
void CMsgSpliceMgr::StopSplice()
{
	m_bExit = TRUE;		

	CAdapter::SetCompatibleEvent( m_EventNewMsg );

	while( !m_MsgList.empty() )
		CAdapter::Sleep( 10 );

	CAdapter::Sleep( 10 );
}


//超时或者所有节点进行回调
void CMsgSpliceMgr::CallBackTimeOut( BOOL bTickAll)
{
	int iRet;
	list<CSameMsgMgr>::iterator it;			

	for(it=m_MsgList.begin();it!=m_MsgList.end();){
		iRet = it->OnSpliceTimeOut( bTickAll, m_pCallBackFun, m_pInvoke );
		if( !iRet ){
			m_MsgList.erase( it++ );
		}else{
			it++;
		}
	}
	
}

//对于可以拼接的组CSameMsgMgr进行拼接并回调，之后清除
void CMsgSpliceMgr::CallBackIfSpliceOk()
{		
	list<CSameMsgMgr>::iterator it;			

	for(it=m_MsgList.begin();it!=m_MsgList.end();){

		if( it->IsSpliceOk( ) ){

			it->OnSpliceOk( m_pCallBackFun, m_pInvoke );

			m_MsgList.erase( it++ );				

			CAdapter::InterlockedDecrement( &m_lSpliceOkNum );

		}else{

			++it;

		}
	}
}

THREAD_RETURN __STDCALL  CMsgSpliceMgr::_ThreadAsynDeal(void* lp)
{
	THREAD_RETURN ret = ((CMsgSpliceMgr*)lp)->ThreadAsynDeal();
	CAdapter::EndCompatibleThread();
	return ret;
}

THREAD_RETURN CMsgSpliceMgr::ThreadAsynDeal()
{
	int iRet;
	while( !m_bExit )
	{
		if( !m_lSpliceOkNum )
			iRet = CAdapter::WaitForCompatibleEvent( m_EventNewMsg, 2000 );	
		
		if( m_bExit ) 
			break;
		
		// 			 if( WAIT_OBJECT_0 == iRet )
		// 				 printf( "WaitEvent=%d WAIT_OBJECT_0\n", iRet  );
		// 			 else	 if( WAIT_TIMEOUT == iRet )
		// 				 printf( "WaitEvent=%d WAIT_TIMEOUT\n", iRet  );
		// 			 else
		// 				 printf( "WaitEvent=%d WAIT_FAILED\n", iRet  );
		// 			
		if( m_lSpliceOkNum >= 1 ){

			CallBackIfSpliceOk();

		}else{

			CAdapter::ResetCompatibleEvent( m_EventNewMsg );

		}	 
		
		CallBackTimeOut( FALSE );
	}	
	
	CallBackTimeOut( TRUE );
	
	return (THREAD_RETURN)(0);
}

