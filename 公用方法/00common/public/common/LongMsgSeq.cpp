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

//��ʼ��
void CLongMsgSeq::InitLongMsgSeqList()
{
	SafeLock safelock(&m_cs);
	memset(m_szSeqList, 0, sizeof(m_szSeqList));
}

//��ȡ��ˮ��
UCHAR CLongMsgSeq::GetLongMsgSeq(int lPhone, UCHAR ucMaxLongMsgSeq)
{
	SafeLock safelock(&m_cs);
	
	int lTel = lPhone%LONGMSGSEQ_QUESIZE;
	if (ucMaxLongMsgSeq<1 || ucMaxLongMsgSeq>255)
	{
		ucMaxLongMsgSeq = 255;
	}
	
	//ȡ����seq����ʷ��¼
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
			++nHisNum; //��ʷ����
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
			//�����������
			cseq = (cseq%ucMaxLongMsgSeq)+1; //cseq[1,ucMaxLongMsgSeq]
		}
	}

	//������ʷ����
	if (nHisNum < LONGMSGSEQ_HISSIZE)
	{		
		m_szSeqList[lTel][0] = cseq;
		pHis[nHisNum] = cseq;
		return cseq;
	} 
	else
	{		
		//������
		m_szSeqList[lTel][0] = cseq;
		memcpy(pHis, pHis+1, (LONGMSGSEQ_HISSIZE-1));
		pHis[LONGMSGSEQ_HISSIZE-1] = cseq;
		return cseq;
	}
}

//������ˮ��
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

	//������ʷ����
	if (nHisNum < LONGMSGSEQ_HISSIZE)
	{		
		pHis[nHisNum] = cseq;
	} 
	else
	{
		//������
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
	
	unsigned char cCheck[16] = {0}; //16λУ����Ϣ	
	cCheck[0] = (unsigned char)0xF0;
	cCheck[1] = (unsigned char)0xA8;
	cCheck[2] = (unsigned char)0xB1;
	cCheck[3] = (unsigned char)0xD5;
	
	FILE* fp = NULL;
	if(fp = fopen(szFilePath, "wb+"))
	{	
		//дУ��λ
		fwrite(cCheck, 1, sizeof(cCheck), fp);
		//д����
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
		//��У����Ϣ
		unsigned char cCheck[16] = {0};	
		fread(cCheck, 1, sizeof(cCheck), fp);
		
		if ((unsigned char)0xF0 == cCheck[0] && (unsigned char)0xA8 == cCheck[1] 
			&& (unsigned char)0xB1 == cCheck[2] && (unsigned char)0xD5 == cCheck[3])
		{
			//�����ݿ�
			size_t nSize = fread(m_szSeqList, 1, sizeof(m_szSeqList), fp);
			if (fp)
			{
				fclose(fp);
			}
			//ɾ���ļ�
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
