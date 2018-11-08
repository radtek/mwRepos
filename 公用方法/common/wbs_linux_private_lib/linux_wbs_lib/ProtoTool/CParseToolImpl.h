#ifndef _H_CPARSETOOLIMPL_H
#define _H_CPARSETOOLIMPL_H
#pragma warning (disable:4103)
#pragma warning (disable:4503)
#pragma warning (disable:4786)

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


#define PT_PUSH_IS_IN_FIELDMAP(pItemKey)  ( (it = allFiled.filedmap.find(pItemKey)) != allFiled.filedmap.end() && strlen(it->second.szCArgName) > 0)    

	                                                                           





using namespace std;

//支持多线程 
char* MDStringMT(const char *Src, char *szOut/*至少33*/);

class CParseToolImpl :public CParseTool
{
public:
	CParseToolImpl();
	virtual ~CParseToolImpl();



	//------推送MO、RPT------
	//标准请求
	//批量推送MO/RPT
	int MakeMoRequestV5_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, string &strMo);
	int MakeRptRequestV5_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, const WBS_RPTERRCODEMAP& errcodemap, string &strRpt);
	
	//单条推送MO/RPT
	int MakeMoRequestV5_std(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, string &strMo);
	int MakeRptRequestV5_std(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, const WBS_RPTERRCODEMAP& errcodemap, string &strRpt);

	//回应
	void ParseResponse_std(const char* pStart, RSP_PACK &rsp);

	//非标准请求(3.0)
	int MakeMoRequestV5_cstd(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &mapTmp, string &strMo);
	int MakeRptRequestV5_cstd(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &mapTmp, string &strRpt);

	//模板单条推送MO/RPT(3.1)
	int MakeMoRequestV5_cstd(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &mapTmp, string &strMo);
	int MakeRptRequestV5_cstd(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strRpt);
	
	//回应
//	void ParseResponse_cstd(const char* pStart, int nRetMode, const ALLFILED &mapTmp, RSP_PACK &rsp);
    
	void ParseResponse_cstd(const char* pStart, const WBSTEMPLATEINFO &mapTmp, const RSP_CMD_PUSH_TMP &pushRsp,
									RSP_PACK &rsp, std::vector<INT64> &vSuccess, std::vector<INT64> &vFailed);
	
	int MakeMoRequestV32_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, string &strMo);
	int MakeRptRequestV32_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, string &strRpt);
	int MakeMoRequestV32_cstd(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &mapTmp, string &strMo);
	int MakeRptRequestV32_cstd(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &mapTmp, string &strRpt);
	
private:
	//组主动推送rpt包
	void MakePushMoJsonData(const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, const HSBC_URLINFO& urlinfo, string &strOut);
    void MakePushMoXMLData(const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, const HSBC_URLINFO& urlinfo, string &strOut);

	void MakePushMoURLData(const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, const HSBC_URLINFO& urlinfo, string &strOut);
	void MakePushRptJsonData(const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, const HSBC_URLINFO& urlinfo, string &strOut);
	void MakePushRptXMLData(const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, const HSBC_URLINFO& urlinfo, string &strOut);
	void MakePushRptURLData(const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, const HSBC_URLINFO& urlinfo, string &strOut);
	void MakePushMoJsonData(const ALLFILED &allFiled, const PACK_DELIVER &FileMoPack, const HSBC_URLINFO& urlinfo, string &strOut);
	void MakePushMoXMLData(const ALLFILED &allFiled, const PACK_DELIVER &FileMoPack, const HSBC_URLINFO& urlinfo, string &strOut);
	void MakePushMoURLData(const ALLFILED &allFiled, const PACK_DELIVER &FileMoPack, const HSBC_URLINFO& urlinfo, string &strOut);
	void MakePushRptJsonData(const ALLFILED &allFiled, const PACK_DELIVER &FileRptPack, const HSBC_URLINFO& urlinfo, string &strOut);
	void MakePushRptXMLData(const ALLFILED &allFiled, const PACK_DELIVER &FileRptPack, const HSBC_URLINFO& urlinfo, string &strOut);
	void MakePushRptURLData(const ALLFILED &allFiled, const PACK_DELIVER &FileRptPack, const HSBC_URLINFO& urlinfo, string &strOut);
	virtual void LoadConfig(int nType);
	//void MakePushRptJsonStdData(const vector<PACK_DELIVER> &FileRptPackArr, const HSBC_URLINFO& urlinfo, string &strOut);

	//void MakePushRptXMLStdData(const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, const HSBC_URLINFO& urlinfo, string &strOut);

	//void MakePushRptURLStdData(const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, const HSBC_URLINFO& urlinfo, string &strOut);

    //int ParsePartErrRsp(const char* pStart, const ALLFILED &allFiled, RSP_PACK &rsp, std::vector<__int64> &failedMsgid, std::vector<__int64> &succesMsgid);

	bool CheckArgLegal(const char* szCustid, const char* szExdata);

	//组mo包,不带根节点(模板)
	void MakeMoJsonNoRootData(int nRetCode, const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, string &strOut, const HSBC_URLINFO& urlinfo, bool bSign);

	//组rpt包, 不带根节点(模板)
	void MakeRptJsonNoRootData(int nRetCode, const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, string &strOut, const HSBC_URLINFO& urlinfo, bool bSign);

	bool IsAllNumber(const char*szIn);
	
	//解析值兼容urlencode方式的
	int  GetCmdValueEx(char *Data, char *CmdItem, int MaxSize, char *CmdVal, BOOL bConvert=TRUE);
	int  GetMultiCmdValue(char *Data, char *CmdItem, int MaxSize, char *CmdVal, BOOL bConvert = TRUE);

	bool IsEnglishMsg(const char * szMsg, int nLen);

	int MakeMoRequestV5_std_pri(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, string &strMo);
	int MakeRptRequestV5_std_pri(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, const WBS_RPTERRCODEMAP& errcodemap, string &strRpt);
	
	//国美专用
	//获取国美在线的账号和密码。
	void GetNameForConfigFile();
	BOOL ToUTF8(char* szSrc, char* pBuf, int cbBufLen);
	int MakeMoRequestV5_GOME(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, string &strMo);
	int MakeRptRequestV5_GOME(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, bool bBus, string &strRpt);
	
	void MakePushMoJsonData_V32(const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, const HSBC_URLINFO& urlinfo, string &strOut);
	void MakePushRptJsonData_V32(const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, const HSBC_URLINFO& urlinfo, string &strOut);
	
	/*
	//将strFixedFiels 设置为key=value&key=value&  形式
	bIgnoreLastTag=false  key=value&key=value&
	bIgnoreLastTag=true  key=value&key=value
	*/
	
	void GetUrlRequestFixedValue(const ALLFILED &allFiled,OUT string& strFixedFiels,bool bIgnoreLastTag=false);

	//void AddRootFixFiledToDoc(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* root, ALLFILED& allFiled);

private:
	//zhubo 2017-12-15
	void MakeFixedJsonValueToDoc(const ALLFILED &allFiled, OUT rapidjson::Document& doc);
	void MakeFixedJsonValueToObj(const ALLFILED &allFiled, string& tagArrName, rapidjson::Document& doc, OUT rapidjson::Value& obj);
	void MakeFixedJsonValueToObj(const ALLFILED &allFiled,rapidjson::Document& doc, OUT rapidjson::Value& obj);
	void MakeFixedXmlValueToParentNode(const ALLFILED &allFiled, rapidxml::xml_document<>& doc,rapidxml::xml_node<>* pNode);
	//zhubo 2017-12-15

	void MakeUrlItemValue(const ALLFILED &allFiled, const char* pItemKey, const char* pValue, OUT string& strItem, bool bAddFirstTag = false);

	//艺龙个性化接口
	int MakeMoRequestV5_YL(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, string &strMo);
	int MakeRptRequestV5_YL(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, string &strRpt);
	
	int DealECErrcode(const WBS_RPTERRCODEMAP& errcodemap, std::string& strErrCode, std::string& strErrDesc);

private:
	std::string m_strMOName;
	std::string m_strMOPwd;

	std::string m_strRPTNameBus;
	std::string m_strRPTPwdBus;

	std::string m_strRPTName;
	std::string m_strRPTPwd;

	std::string m_strRPTURL_Gome;
	std::string m_strRPTURL_Gome_BUS;
	std::string m_strMOURL_Gome;
	std::string m_strMOURL_Gome_BUS;

	std::vector<std::string> m_vcYLName;           //艺龙账号
	
};


#endif

