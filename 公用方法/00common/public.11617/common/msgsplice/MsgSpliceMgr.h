// MsgSpliceMgr.h: interface for the CMsgSpliceMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSPLICEMGR_H__B51D0560_E378_488D_BB5D_095A44BE9111__INCLUDED_)
#define AFX_MSGSPLICEMGR_H__B51D0560_E378_488D_BB5D_095A44BE9111__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ER_PARAM 1
#define ER_INSTANCE_NULL 2
#define ER_INSTANCE_EXIT 3
#define ER_PKNUMBER_REPEAT 4
 
#include "../adapter/Adapter.h"
#include "../DerivedLock.h"
#include "../adapter/CompatibleEvent.h"
#include "../adapter/CompatibleThread.h"
typedef void (__STDCALL *PFCALLBACK)(void* Invoke, int ErrCode, const char* UserId, const char* Spno,const char* Phone,const char* Msg, int MsgFmt, int MsgLen, int PkTotal, int PkNumber, int PkSeqId );

#include <list>
using namespace std;

class CMsgInfor
{
public:
	string strUser;	//帐号，
	string strSpno;	//通道号，
	string strPhone; //手机号,
	string strMsg;		//短信内容,
	int iMsgFmt;	//短信编码, 
	int iMsgLen;	//短信长度	
	int iPkTotal;	//总条数pktotal,
	int iPkNumber;	//分条数pknumber,
	int iPkSeqId;	//头序号,	
	long iTimeOut;	//过期时间
	
	CMsgInfor( const char* User, const char* Spno,const char* Phone,const char* Msg, int MsgFmt, int MsgLen, int PkTotal, int PkNumber,	int PkSeqId, int  TimeOut )
		:strUser(User),strSpno(Spno),strPhone(Phone),strMsg( Msg,MsgLen )
	{		
		iMsgFmt = MsgFmt;
		iMsgLen = MsgLen; 
		iPkTotal = PkTotal;
		iPkNumber = PkNumber;
		iPkSeqId = PkSeqId;		
		iTimeOut = time(NULL) + TimeOut ;		
	}	
};

//相同特征信息的放在一组[链表]
class CSameMsgMgr
{
	list<CMsgInfor> MsgList;

public:
	
	//构造时至少需要一个元素
	CSameMsgMgr( CMsgInfor& msg  );	

	//回调超时的或者所有的并清楚，返回剩余元素数量
	int OnSpliceTimeOut( BOOL bTickAll, PFCALLBACK pCallBackFun,void* pInvoke );

	//拼接信息并回调
	void OnSpliceOk(PFCALLBACK pCallBackFun,void* pInvoke);

	//参数[msg]是否和本组具有相同特认证信息
	bool IsMatch( CMsgInfor& msg );

	//本组是否可以拼接了
	bool IsSpliceOk();

	//按照iPkNumber顺序将msg插入到本组
	int OrderInsertSibling(CMsgInfor& msg);

	//**********************************
    //Description: 转换为多字节模式
    // Parameter:  参照MSDN
    // Returns:    int 返回被转换的字节数
    //              失败返回0
    //              暂时支持gbk和utf8两种
    //注:cbMultiByte为0时表示要取需要缓冲区大小，
    //此时linux版本下最大可支持转换后宽字符长度不超过8K的操作
    //************************************
    static int WideCharToMultiByte(
		unsigned int CodePage,            // code page
		unsigned long dwFlags,            // performance and mapping flags
		const wchar_t* lpWideCharStr,    // wide-character string
		int cchWideChar,          // number of chars in string
		char* lpMultiByteStr,     // buffer for new string
		int cbMultiByte,          // size of buffer
		const char* lpDefaultChar,     // default for unmappable chars
		int* lpUsedDefaultChar  // set when default char used
		);
	//static int US2ToAscii(const char *InMsg, int InLen, char *Data, int *Len);
	//出参直接支持string
	static int US2ToAsciiEx(const char *InMsg, int InLen, string& Data);
};


//拼接管理类，用于管理不同特征组[CSameMsgMgr]的管理
class CMsgSpliceMgr
{
public:

	//构造
	CMsgSpliceMgr(PFCALLBACK pCallBackFun, void* pInvoke);

	//析构
	virtual ~CMsgSpliceMgr();

	//添加信息
	int AddMsg(const char* User, const char* Spno,const char* Phone,const char* Msg, int MsgFmt, int MsgLen, int PkTotal, int PkNumber,	int PkSeqId, int  TimeOut );

	//停止拼接服务
	void StopSplice();

private:

	//超时或者所有节点进行回调
	void CallBackTimeOut( BOOL bTickAll);

	//对于可以拼接的组CSameMsgMgr进行拼接并回调，之后清除
	void CallBackIfSpliceOk();
	
protected:
	void* m_pInvoke;	//上层指针，
	PFCALLBACK m_pCallBackFun;
	list<CSameMsgMgr> m_MsgList;	
	DerivedLock m_csVisit ;                       //记录日志对文件操作用的锁
	CCompatibleEvent m_EventNewMsg;
	BOOL m_bExit;
	long m_lSpliceOkNum;

	static THREAD_RETURN __STDCALL  _ThreadAsynDeal(void* lp);
	THREAD_RETURN ThreadAsynDeal();  

};

#endif // !defined(AFX_MSGSPLICEMGR_H__B51D0560_E378_488D_BB5D_095A44BE9111__INCLUDED_)
