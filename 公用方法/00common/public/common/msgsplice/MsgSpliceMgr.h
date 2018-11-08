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
	string strUser;	//�ʺţ�
	string strSpno;	//ͨ���ţ�
	string strPhone; //�ֻ���,
	string strMsg;		//��������,
	int iMsgFmt;	//���ű���, 
	int iMsgLen;	//���ų���	
	int iPkTotal;	//������pktotal,
	int iPkNumber;	//������pknumber,
	int iPkSeqId;	//ͷ���,	
	long iTimeOut;	//����ʱ��
	
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

//��ͬ������Ϣ�ķ���һ��[����]
class CSameMsgMgr
{
	list<CMsgInfor> MsgList;

public:
	
	//����ʱ������Ҫһ��Ԫ��
	CSameMsgMgr( CMsgInfor& msg  );	

	//�ص���ʱ�Ļ������еĲ����������ʣ��Ԫ������
	int OnSpliceTimeOut( BOOL bTickAll, PFCALLBACK pCallBackFun,void* pInvoke );

	//ƴ����Ϣ���ص�
	void OnSpliceOk(PFCALLBACK pCallBackFun,void* pInvoke);

	//����[msg]�Ƿ�ͱ��������ͬ����֤��Ϣ
	bool IsMatch( CMsgInfor& msg );

	//�����Ƿ����ƴ����
	bool IsSpliceOk();

	//����iPkNumber˳��msg���뵽����
	int OrderInsertSibling(CMsgInfor& msg);

	//**********************************
    //Description: ת��Ϊ���ֽ�ģʽ
    // Parameter:  ����MSDN
    // Returns:    int ���ر�ת�����ֽ���
    //              ʧ�ܷ���0
    //              ��ʱ֧��gbk��utf8����
    //ע:cbMultiByteΪ0ʱ��ʾҪȡ��Ҫ��������С��
    //��ʱlinux�汾������֧��ת������ַ����Ȳ�����8K�Ĳ���
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
	//����ֱ��֧��string
	static int US2ToAsciiEx(const char *InMsg, int InLen, string& Data);
};


//ƴ�ӹ����࣬���ڹ���ͬ������[CSameMsgMgr]�Ĺ���
class CMsgSpliceMgr
{
public:

	//����
	CMsgSpliceMgr(PFCALLBACK pCallBackFun, void* pInvoke);

	//����
	virtual ~CMsgSpliceMgr();

	//�����Ϣ
	int AddMsg(const char* User, const char* Spno,const char* Phone,const char* Msg, int MsgFmt, int MsgLen, int PkTotal, int PkNumber,	int PkSeqId, int  TimeOut );

	//ֹͣƴ�ӷ���
	void StopSplice();

private:

	//��ʱ�������нڵ���лص�
	void CallBackTimeOut( BOOL bTickAll);

	//���ڿ���ƴ�ӵ���CSameMsgMgr����ƴ�Ӳ��ص���֮�����
	void CallBackIfSpliceOk();
	
protected:
	void* m_pInvoke;	//�ϲ�ָ�룬
	PFCALLBACK m_pCallBackFun;
	list<CSameMsgMgr> m_MsgList;	
	DerivedLock m_csVisit ;                       //��¼��־���ļ������õ���
	CCompatibleEvent m_EventNewMsg;
	BOOL m_bExit;
	long m_lSpliceOkNum;

	static THREAD_RETURN __STDCALL  _ThreadAsynDeal(void* lp);
	THREAD_RETURN ThreadAsynDeal();  

};

#endif // !defined(AFX_MSGSPLICEMGR_H__B51D0560_E378_488D_BB5D_095A44BE9111__INCLUDED_)
