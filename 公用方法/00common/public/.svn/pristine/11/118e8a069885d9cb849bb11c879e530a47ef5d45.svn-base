// LongMsgSeq.cpp: implementation of the CLongMsgSeq class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "LongMsgSeq.h"
#include "SafeLock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLongMsgSeq::CLongMsgSeq()
{
	InitLongMsgSeqList();
}

CLongMsgSeq::~CLongMsgSeq()
{

}

//初始化
void CLongMsgSeq::InitLongMsgSeqList()
{
	SafeLock safelock(&m_cs);
	memset(m_szSeqList, 0, sizeof(m_szSeqList));
}

//获取流水号
UCHAR CLongMsgSeq::GetLongMsgSeq(int lPhone, UCHAR ucMaxLongMsgSeq)
{
	SafeLock safelock(&m_cs);
	
	int lTel = lPhone%LONGMSGSEQ_QUESIZE;
	if (ucMaxLongMsgSeq<1 || ucMaxLongMsgSeq>255)
	{
		ucMaxLongMsgSeq = 255;
	}
	
	//取最新seq及历史记录
	UCHAR cseq = m_szSeqList[lTel][0];	
	UCHAR* pHis = (m_szSeqList[lTel])+1;
	int nHisNum = 0;

	cseq = (cseq%ucMaxLongMsgSeq)+1; //cseq[1,ucMaxLongMsgSeq]	

	while (1)
	{
		BOOL bOnlySelf = TRUE;
		nHisNum = 0;
		for (int i=0; i<LONGMSGSEQ_HISSIZE; i++)
		{
			if (0 == pHis[i])
			{
				break;
			}
			++nHisNum; //历史数量
			if (cseq == pHis[i])
			{
				bOnlySelf = FALSE;
				break;
			}
		}
		
		if (bOnlySelf)
		{
			break;
		} 
		else
		{
			//继续计算调整
			cseq = (cseq%ucMaxLongMsgSeq)+1; //cseq[1,ucMaxLongMsgSeq]
		}
	}

	//更新历史队列
	if (nHisNum < LONGMSGSEQ_HISSIZE)
	{		
		m_szSeqList[lTel][0] = cseq;
		pHis[nHisNum] = cseq;
		return cseq;
	} 
	else
	{		
		//队列满
		m_szSeqList[lTel][0] = cseq;
		memcpy(pHis, pHis+1, (LONGMSGSEQ_HISSIZE-1));
		pHis[LONGMSGSEQ_HISSIZE-1] = cseq;
		return cseq;
	}
}

//更新流水号
void CLongMsgSeq::UpdateLongMsgSeq(int lPhone, UCHAR cseq)
{
	SafeLock safelock(&m_cs);
	
	int lTel = lPhone%LONGMSGSEQ_QUESIZE;
	
	UCHAR* pHis = (m_szSeqList[lTel])+1;
	int nHisNum = 0;
	
	BOOL bFind = FALSE;
	for (int i=0; i<LONGMSGSEQ_HISSIZE; i++)
	{
		if (0 == pHis[i])
		{
			break;
		}
		++nHisNum;
		if (cseq == pHis[i])
		{
			bFind = TRUE;
			break;
		}
	}
	
	if (bFind)
	{
		return;
	}

	//更新历史队列
	if (nHisNum < LONGMSGSEQ_HISSIZE)
	{		
		pHis[nHisNum] = cseq;
	} 
	else
	{
		//队列满
		memcpy(pHis, pHis+1, (LONGMSGSEQ_HISSIZE-1));
		pHis[LONGMSGSEQ_HISSIZE-1] = cseq;
	}
	return;
}

int CLongMsgSeq::WriteLongMsgQueue(const char* lpWorkPath)
{
	SafeLock safelock(&m_cs);
	
	char szFilePath[MAX_PATH] = {0};
	if (NULL != lpWorkPath && CAdapter::PathFileExists(lpWorkPath))
	{
		strncpy(szFilePath, lpWorkPath, sizeof(szFilePath)-1);
		TrimRight(szFilePath, '\\');
		TrimRight(szFilePath, '/');
		sprintf(szFilePath+strlen(szFilePath), "//%s", "Temp_LongMsgSeq.dat");
	} 
	else
	{
		sprintf(szFilePath, "%s", "Temp_LongMsgSeq.dat");
	}
	
	unsigned char cCheck[16] = {0}; //16位校验信息	
	cCheck[0] = (unsigned char)0xF0;
	cCheck[1] = (unsigned char)0xA8;
	cCheck[2] = (unsigned char)0xB1;
	cCheck[3] = (unsigned char)0xD5;
	
	FILE* fp = NULL;
	if(fp = fopen(szFilePath, "wb+"))
	{	
		//写校验位
		fwrite(cCheck, 1, sizeof(cCheck), fp);
		//写数据
		size_t nSize = fwrite(m_szSeqList, 1, sizeof(m_szSeqList), fp);		
		if (fp)
		{
			fclose(fp);
		}
		return 0;
	}
	return (-1);
}

int CLongMsgSeq::ReadLongMsgTempFile(const char* lpWorkPath)
{	
	SafeLock safelock(&m_cs);
	
	char szFilePath[MAX_PATH] = {0};
	if (NULL != lpWorkPath && CAdapter::PathFileExists(lpWorkPath))
	{
		strncpy(szFilePath, lpWorkPath, sizeof(szFilePath)-1);
		TrimRight(szFilePath, '\\');
		TrimRight(szFilePath, '/');
		sprintf(szFilePath+strlen(szFilePath), "//%s", "Temp_LongMsgSeq.dat");
	} 
	else
	{
		sprintf(szFilePath, "%s", "Temp_LongMsgSeq.dat");
	}
	
	FILE* fp = NULL;
	if(fp = fopen(szFilePath, "rb"))
	{	
		//读校验信息
		unsigned char cCheck[16] = {0};	
		fread(cCheck, 1, sizeof(cCheck), fp);
		
		if ((unsigned char)0xF0 == cCheck[0] && (unsigned char)0xA8 == cCheck[1] 
			&& (unsigned char)0xB1 == cCheck[2] && (unsigned char)0xD5 == cCheck[3])
		{
			//读数据块
			size_t nSize = fread(m_szSeqList, 1, sizeof(m_szSeqList), fp);
			if (fp)
			{
				fclose(fp);
			}
			//删除文件
			CAdapter::ShellDeleteFiles(szFilePath);
			return 0;
		}
		else
		{
			return 1;
		}
	}	
	return (-1);	
}

void CLongMsgSeq::TrimRight(const char* lpString, char chTrim)
{
	if (NULL == lpString || 0 == chTrim)
	{
		return;
	}
	
	char *p = (char*)lpString+strlen(lpString);
	while(--p >= lpString)
	{
		if (*p == '\0')
		{
			break;
		}
		if (*p != chTrim)
		{
			break;
		}
		*p = '\0';
	}
}
