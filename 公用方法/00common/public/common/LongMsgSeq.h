// LongMsgSeq.h: interface for the CLongMsgSeq class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__LONGMSGSEQ_H__)
#define __LONGMSGSEQ_H__

#include "./adapter/Adapter.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define LONGMSGSEQ_QUESIZE	1000000
#define LONGMSGSEQ_HISSIZE	10

class CLongMsgSeq  
{
public:
	CLongMsgSeq();
	virtual ~CLongMsgSeq();	
public:
	//初始化
	void InitLongMsgSeqList();
	//获取流水号
	UCHAR GetLongMsgSeq(int lPhone, UCHAR ucMaxLongMsgSeq);
	//更新流水号
	void UpdateLongMsgSeq(int lPhone, UCHAR cseq);
	
	int WriteLongMsgQueue(const char* lpWorkPath);
	int ReadLongMsgTempFile(const char* lpWorkPath);
	void TrimRight(const char* lpString, char chTrim);
private:
	UCHAR m_szSeqList[LONGMSGSEQ_QUESIZE][1+LONGMSGSEQ_HISSIZE+5];
	DerivedLock m_cs;
};

#endif // !defined(__LONGMSGSEQ_H__)
