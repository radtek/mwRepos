#pragma once

#include <stdio.h>
#include <map>
//#include <iostream.h>
#include <string>

#include "filebuffer.h"
#include "../../../../00common/public/common/adapter/Adapter.h"
#include "../../../../00common/public/common/SafeLock.h"
#include "../../../../00common/public/common/DerivedLock.h"
using   namespace   std;

#define MAPMAXFILE 10000
#define VERSION_V2 "00002"
class CFileBufferManage: public CFileBuffer
{
public:
	CFileBufferManage();
	~CFileBufferManage(void);
	
	virtual bool InitFileBuffer(const char* szDirName="bufferData",
								int nMaxDataCount=50,
								int nTimeOut=1,
								int nDataTypeSize=1,
								const int nWGNO=0,
								const char *szVersion="00001"
								);

	virtual bool WriteData(const void *data, int nDataSize);
	virtual int ReadData(void *readData, int nToBeRead);
	virtual bool SwitchFile();
	virtual int GetDataRecordCount();
	virtual int GetMaxDataCount();

private:
	//�滻ָ�����ַ���
	void ReplaceStr(std::string &strData, const char *szOldData, const char *szNewData); 

	std::string GetFirstOkFilePath();
	
	//��ӵ��ļ�·��ӳ�����
	void AddToFilePathMap(INT64 nFileId);
	
	//��ӵ��ļ�·��ӳ�����
	void AddToFilePathMap(const char *szFilePath);

	//����ļ�·��ӳ�����
	void ClearFilePathMap();
	//�����ļ�·����ӳ�����
	void LoadFilePathToMap();
	//�رյ�ǰ�ļ�
	bool CloseCurFile();
	void SwitchAllFile();//�����������
	//ȡ��ǰĿ¼�¹��ж�������¼
	int GetAllDataSize();

	bool RetryWriteData(const void *data, int nDataSize);

private:
	DerivedLock m_csCurWriteFile;//��ǰ���ڲ����ļ���(��δ�رյ��ļ�)
	DerivedLock m_csCurReadFile;//��ǰ���ڲ����ļ���(��δ�رյ��ļ�)
	DerivedLock m_csFilePathMap;//�ļ���ӳ�����

	time_t m_tLastSwitch;		//���һ���л�ʱ��
	int m_nTimeOutSwitch;	//��ʱ�л���ʱ��
	int m_nMaxDataCnt;		//һ���ļ����洢�ĸ���
	int m_nCurrDataCnt;		//��ǰ�ļ����д��˶��ٸ��ṹ������
	int m_nAllDataCnt;		//��ǰ�ļ����д��˶��ٸ��ṹ������
	int m_nIndexId;			//��ǰ�ļ�������ID ���Ϊ99999
	int m_nDataTypeSize;	//һ����¼�Ĵ�С
	int m_nWGNO;			//���ر��
	
	FILE *m_fpCurrfile;		//��ǰ�ļ���ָ��
	INT64 m_nCurrFileId;	//��ǰ�ļ���ID
	char  m_szCurrFileName[MAX_PATH];	//��ǰ�ļ�������

	char m_szCurrFileDir[MAX_PATH];//��ŵ�ǰĿ¼�ľ���·��
	char saveFileName[MAX_PATH];
	char m_szVersion[6];    //����5���ֽ�
	std::map<std::string, char> m_mFileName;//key-�ļ�����ţ�value-0��Ϊ��ʱ�ļ���1Ϊok�ļ��� size���Ϊ10000
};
