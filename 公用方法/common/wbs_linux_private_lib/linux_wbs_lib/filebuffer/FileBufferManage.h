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
	//替换指定的字符串
	void ReplaceStr(std::string &strData, const char *szOldData, const char *szNewData); 

	std::string GetFirstOkFilePath();
	
	//添加到文件路径映射表中
	void AddToFilePathMap(INT64 nFileId);
	
	//添加到文件路径映射表中
	void AddToFilePathMap(const char *szFilePath);

	//清空文件路径映射表中
	void ClearFilePathMap();
	//重载文件路径到映射表中
	void LoadFilePathToMap();
	//关闭当前文件
	bool CloseCurFile();
	void SwitchAllFile();//这里会重命名
	//取当前目录下共有多少条记录
	int GetAllDataSize();

	bool RetryWriteData(const void *data, int nDataSize);

private:
	DerivedLock m_csCurWriteFile;//当前正在操作文件锁(尚未关闭的文件)
	DerivedLock m_csCurReadFile;//当前正在操作文件锁(尚未关闭的文件)
	DerivedLock m_csFilePathMap;//文件名映射表锁

	time_t m_tLastSwitch;		//最后一次切换时间
	int m_nTimeOutSwitch;	//超时切换的时间
	int m_nMaxDataCnt;		//一个文件最大存储的个数
	int m_nCurrDataCnt;		//当前文件件中存了多少个结构体数据
	int m_nAllDataCnt;		//当前文件夹中存了多少个结构体数据
	int m_nIndexId;			//当前文件名自增ID 最大为99999
	int m_nDataTypeSize;	//一条记录的大小
	int m_nWGNO;			//网关编号
	
	FILE *m_fpCurrfile;		//当前文件的指针
	INT64 m_nCurrFileId;	//当前文件的ID
	char  m_szCurrFileName[MAX_PATH];	//当前文件的名称

	char m_szCurrFileDir[MAX_PATH];//存放当前目录的绝对路径
	char saveFileName[MAX_PATH];
	char m_szVersion[6];    //定长5个字节
	std::map<std::string, char> m_mFileName;//key-文件名存放，value-0，为临时文件，1为ok文件， size最大为10000
};
