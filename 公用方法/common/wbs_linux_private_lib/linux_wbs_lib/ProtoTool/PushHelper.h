#ifndef _MW_PUSH_HELPER
#define _MW_PUSH_HELPER

#include "WBSDLL.h"
#include <map>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <deque>
#include "TypeDef.h"
#include "tools/XMLParse.h"
#include "tools/JsonParse.h"
#include "CParseToolImpl.h"

class PushHelper
{
public:
	PushHelper();
	~PushHelper();
public:
	//============================GOME
	static void GetGomePushTime(OUT string& strPushTime);
	static void MakeGomeSign(const char* pUserId, const char* strPushTime, const char* pPwd,OUT string& strSign);
	//============================GOME

	//��ȡMO �û���������
	static void GetMoReqUserIdAndPwd(const HSBC_URLINFO& urlinfo, string& strTime, OUT char* pUserID,  OUT char* pPwd);
	//��ȡRpt �û���������
	static void GetRptReqUserIdAndPwd(const HSBC_URLINFO& urlinfo, string& strTime, OUT char* pUserID,  OUT char* pPwd);
};
#endif