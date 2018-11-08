#include "PushHelper.h"




PushHelper::PushHelper()
{
}

PushHelper::~PushHelper()
{

}

void PushHelper::GetGomePushTime(string& strPushTime)
{
	SYSTEMTIME_EX tt;
	CAdapter::GetLocalTime(&tt);
	SYSTEMTIME_EX_LINUX t(tt);
	char szPushTime[20] = { 0 };
	sprintf(szPushTime, "%04d%02d%02d%02d%02d%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);

	strPushTime = szPushTime;

}

void PushHelper::MakeGomeSign(const char* pUserId, const char* strPushTime, const char* pPwd , OUT string& strSign)
{
	char szMd5[32 + 1] = { 0 };
	char szMd5Src[MAX_PWD_LEN + 128 + 1] = { 0 };

	memset(szMd5Src, 0, sizeof(szMd5Src));
	memset(szMd5, 0, sizeof(szMd5));
	_snprintf(szMd5Src, sizeof(szMd5Src), "%s%s%s", pUserId, strPushTime, pPwd);
	MDStringMT(szMd5Src, szMd5);

	strSign = szMd5;
}



void PushHelper::GetMoReqUserIdAndPwd(const HSBC_URLINFO& urlinfo, string& strTime, OUT char* pUserID, OUT char* pPwd)
{
	if (strlen(urlinfo.szMoUsrId) > 0 && strlen(urlinfo.szMoPwd) > 0)
	{
		memcpy(pUserID, urlinfo.szMoUsrId, min(strlen(urlinfo.szMoUsrId), sizeof(urlinfo.szMoUsrId) - 1));
		memcpy(pPwd, urlinfo.szMoPwd, min(strlen(urlinfo.szMoPwd), MAX_PWD_LEN));
	}
	else
	{
		memcpy(pUserID, urlinfo.szUsrId, MAX_SPID_LEN);

		char szMd5Src[MAX_PWD_LEN + 128 + 1] = { 0 };
		//pwd md5º”√‹
		int nMd5SrcLen = _snprintf(szMd5Src, sizeof(szMd5Src), "%s%s%s%s", CAdapter::strupr((char*)urlinfo.szUsrId), PWDSTR, urlinfo.szPwd, strTime.c_str());
		MDStringMT(szMd5Src, pPwd);
	}
}

void PushHelper::GetRptReqUserIdAndPwd(const HSBC_URLINFO& urlinfo, string& strTime, OUT char* pUserID, OUT char* pPwd)
{
	if (strlen(urlinfo.szRptUsrId) > 0 && strlen(urlinfo.szRptPwd) > 0)
	{
		memcpy(pUserID, urlinfo.szRptUsrId, min(strlen(urlinfo.szRptUsrId),sizeof(urlinfo.szRptUsrId) - 1));
		memcpy(pPwd, urlinfo.szRptPwd, min(strlen(urlinfo.szRptPwd), MAX_PWD_LEN));
	}
	else
	{
		memcpy(pUserID, urlinfo.szUsrId, MAX_SPID_LEN);

		char szMd5Src[MAX_PWD_LEN + 128 + 1] = { 0 };
		//pwd md5º”√‹
		int nMd5SrcLen = _snprintf(szMd5Src, sizeof(szMd5Src), "%s%s%s%s", CAdapter::strupr((char*)urlinfo.szUsrId), PWDSTR, urlinfo.szPwd, strTime.c_str());
		MDStringMT(szMd5Src, pPwd);
	}
}