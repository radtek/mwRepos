#ifndef  _H_WBSDLL_H
#define  _H_WBSDLL_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <map>
#include "TypeDef.h"
#include "Global.h"

using namespace std;



#ifdef WIN32

#ifdef WBSDLL_EXPORTS
#define DLIB_API  __declspec(dllexport)
#else
#define DLIB_API  __declspec(dllimport)
#endif

#else
//目前不支持linux，linux使用通知的方式不是window的事件方式
#ifndef	LINUX
#define DLIB_API
#else
#define DLIB_API
#endif

#endif

#pragma pack(1)

//加长，防止前面加了空格占用位数
#define MAX_RESULT_LEN		30

enum ERR_CODE
{
	ERR_OK,
	ERR_FMT = -100100,		//格式错误，包括字段类型错，数据包格式错
	ERR_FULL_CACHE,			//缓存满
	EEE_JSON_DOC,
	ERR_XML_DOC,
	ERR_JSON_ARR,
	ERR_JSON_OBJ,
	ERR_XML,
	ERR_JSON,
	ERR_EXCEPTION,
	ERR_XML_EXCEPTION,
	ERR_JSON_EXCEPTION

};

//标准请求路径
static const char* g_szReqStdPath = "sms/v1/std";

//标准标签
static const char *g_szUesrId = "userid";
static const char *g_szUesrIdEx = "sign";
static const char *g_szPwd = "pwd";
static const char *g_szSign = "signex";   //国美客户sign
static const char *g_szApiKey = "apikey";
static const char *g_szMobile = "mobile";
static const char *g_szMsg = "content";
static const char *g_szTimeStamp = "timestamp";
static const char *g_szSvrType = "svrtype";
static const char *g_szExNo = "exno";
static const char *g_szCustID = "custid";
static const char *g_szPackID = "packid";
static const char *g_szExData = "exdata";
static const char *g_szMutiMt = "multimt";
static const char *g_szMt = "mt";
static const char *g_szMtRsp = "mtrsp";
static const char *g_szMtReq = "mtreq";
static const char *g_szMoReq = "moreq";
static const char *g_szMoRsp = "morsp";
static const char *g_szSpNo = "spno";
static const char *g_szRecvTime = "rtime";
static const char *g_szSendTime = "stime";
static const char *g_szPushTime = "ptime";
static const char *g_szPkNum = "pknum";
static const char *g_szPkTotal = "pktotal";
static const char *g_szStatus = "status";
static const char *g_szErrCode = "errcode";
static const char *g_szErrCodeDesc = "errdesc";
static const char *g_szFailMap = "failmap";
static const char *g_szSuccesMap = "succmap";

static const char *g_szResult = "result";
static const char *g_szChargeType = "chargetype";
static const char *g_szBalance = "balance";
static const char *g_szMoney = "money";

static const char *g_szFeeReq = "feereq";
static const char *g_szFeeRsp = "feersp";

//响应标签
static const char *g_szCmd = "cmd";
static const char *g_szSeqId = "seqid";


//xml标签对
static const char *g_szMutiMtXml = "<multimt>";
static const char *g_szMutiMtXml2 = "</multimt>";

static const char *g_szMsgId = "msgid";     //用户自生成流水号

static const char *g_szRetSize = "retsize";
static const char *g_szRpts = "rpts";
static const char *g_szRpt = "rpt";
static const char *g_szRptReq = "rptreq";
static const char *g_szRptRsp = "rptrsp";
static const char *g_szRptRspDetails = "rptdetails";
static const char *g_szMoRspDetails = "modetails";


static const char *g_szMOS = "mos";
static const char *g_szMO = "mo";
static const char *g_szMoCmd = "MO_REQ";
static const char *g_szRptCmd = "RPT_REQ";

#define XML_HEAD  "<?xml version=\"1.0\" encoding=\"utf-8\"?>"

//单个mo对象
#define URL_MO_REQ_BODY  "msgid=%lld&"\
	"mobile=%s&"\
	"spno=%s&"\
	"exno=%s&"\
	"rtime=%s&"\
	"content=%s"

#define JSON_MO_RSP_BODY  "{\"msgid\":%lld,"\
	"\"mobile\":\"%s\","\
	"\"spno\":\"%s\","\
	"\"exno\":\"%s\","\
	"\"rtime\":\"%s\","\
	"\"content\":\"%s\"},"

#define XML_MO_RSP_BODY   "<mo>"\
	"<msgid>%lld</msgid>"\
	"<mobile>%s</mobile>"\
	"<spno>%s</spno>"\
	"<exno>%s</exno>"\
	"<rtime>%s</rtime>"\
	"<content>%s</content></mo>"\

#define JSON_MO_SINGLE_REQ_BODY  "\"msgid\":%lld,"\
	"\"mobile\":\"%s\","\
	"\"spno\":\"%s\","\
	"\"exno\":\"%s\","\
	"\"rtime\":\"%s\","\
	"\"content\":\"%s\"}"

#define XML_MO_SINGLE_REQ_BODY  "<msgid>%lld</msgid>"\
	"<mobile>%s</mobile>"\
	"<spno>%s</spno>"\
	"<exno>%s</exno>"\
	"<rtime>%s</rtime>"\
	"<content>%s</content>"\

//单个rpt对象
#define URL_RPT_RSP_BODY  "msgid=%lld&"\
	"custid=%s&"\
	"pknum=%d&"\
	"pktotal=%d&"\
	"mobile=%s&"\
	"spno=%s&"\
	"exno=%s&"\
	"stime=%s&"\
	"rtime=%s&"\
	"status=%d&"\
	"errcode=%s&"\
	"errdesc=%s&"\
	"exdata=%s"

#define JSON_RPT_RSP_BODY  "{\"msgid\":%lld,"\
	"\"custid\":\"%s\","\
	"\"pknum\":%d,"\
	"\"pktotal\":%d,"\
	"\"mobile\":\"%s\","\
	"\"spno\":\"%s\","\
	"\"exno\":\"%s\","\
	"\"stime\":\"%s\","\
	"\"rtime\":\"%s\","\
	"\"status\":%d,"\
	"\"errcode\":\"%s\","\
	"\"errdesc\":\"%s\","\
	"\"exdata\":\"%s\"},"

#define XML_RPT_RSP_BODY   "<rpt>"\
	"<msgid>%lld</msgid>"\
	"<custid>%s</custid>"\
	"<pknum>%d</pknum>"\
	"<pktotal>%d</pktotal>"\
	"<mobile>%s</mobile>"\
	"<spno>%s</spno>"\
	"<exno>%s</exno>"\
	"<stime>%s</stime>"\
	"<rtime>%s</rtime>"\
	"<status>%d</status>"\
	"<errcode>%s</errcode>"\
	"<errdesc>%s</errdesc>"\
	"<exdata>%s</exdata>"\
	"</rpt>"\

#define JSON_RPT_SINGLE_REQ_BODY  "\"msgid\":%lld,"\
	"\"custid\":\"%s\","\
	"\"pknum\":%d,"\
	"\"pktotal\":%d,"\
	"\"mobile\":\"%s\","\
	"\"spno\":\"%s\","\
	"\"exno\":\"%s\","\
	"\"stime\":\"%s\","\
	"\"rtime\":\"%s\","\
	"\"status\":%d,"\
	"\"errcode\":\"%s\","\
	"\"errdesc\":\"%s\","\
	"\"exdata\":\"%s\"}"

#define XML_RPT_SINGLE_REQ_BODY  "<msgid>%lld</msgid>"\
	"<custid>%s</custid>"\
	"<pknum>%d</pknum>"\
	"<pktotal>%d</pktotal>"\
	"<mobile>%s</mobile>"\
	"<spno>%s</spno>"\
	"<exno>%s</exno>"\
	"<stime>%s</stime>"\
	"<rtime>%s</rtime>"\
	"<status>%d</status>"\
	"<errcode>%s</errcode>"\
	"<errdesc>%s</errdesc>"\
	"<exdata>%s</exdata>"\

#define JSON_RPT_RSP_BODY_1 "{\"result\":%d,"\
	"\"rpts\":[%s]}"

//主动推送批量mo或者rpt前半部分
#define JSON_PUSHRPT_PRE   "{"\
	"\"userid\":\"%s\","\
	"\"pwd\":\"%s\","\
	"\"timestamp\":\"%s\","\
	"\"cmd\":\"%s\","\
	"\"seqid\":%d,"\
	"\"rpts\":["\

#define JSON_PUSHMO_PRE   "{"\
	"\"userid\":\"%s\","\
	"\"pwd\":\"%s\","\
	"\"timestamp\":\"%s\","\
	"\"cmd\":\"%s\","\
	"\"seqid\":%d,"\
	"\"mos\":["

#define XML_PUSHRPT_PRE  "<?xml version=\"1.0\" encoding=\"utf-8\"?>"\
	"<rptreq>"\
	"<userid>%s</userid>"\
	"<pwd>%s</pwd>"\
	"<timestamp>%s</timestamp>"\
	"<cmd>RPT_REQ</cmd>"\
	"<seqid>%d</seqid>"\
	"<rpts>"

#define XML_PUSHMO_PRE  "<?xml version=\"1.0\" encoding=\"utf-8\"?>"\
	"<moreq>"\
	"<userid>%s</userid>"\
	"<pwd>%s</pwd>"\
	"<timestamp>%s</timestamp>"\
	"<cmd>MO_REQ</cmd>"\
	"<seqid>%d</seqid>"\
	"<mos>"

//主动推送单个mo或者rpt前半部分
#define JSON_PUSHSINGLERPT_PRE   "{"\
	"\"userid\":\"%s\","\
	"\"pwd\":\"%s\","\
	"\"timestamp\":\"%s\","\
	"\"cmd\":\"%s\","\
	"\"seqid\":%d,"\

#define JSON_PUSHSINGLEMO_PRE   "{"\
	"\"userid\":\"%s\","\
	"\"pwd\":\"%s\","\
	"\"timestamp\":\"%s\","\
	"\"cmd\":\"%s\","\
	"\"seqid\":%d,"\

#define XML_PUSHSINGLERPT_PRE  "<?xml version=\"1.0\" encoding=\"utf-8\"?>"\
	"<rptreq>"\
	"<userid>%s</userid>"\
	"<pwd>%s</pwd>"\
	"<timestamp>%s</timestamp>"\
	"<cmd>RPT_REQ</cmd>"\
	"<seqid>%d</seqid>"

#define XML_PUSHSINGLEMO_PRE  "<?xml version=\"1.0\" encoding=\"utf-8\"?>"\
	"<moreq>"\
	"<userid>%s</userid>"\
	"<pwd>%s</pwd>"\
	"<timestamp>%s</timestamp>"\
	"<cmd>MO_REQ</cmd>"\
	"<seqid>%d</seqid>"

#define JSON_MO_RSP_BODY_GOME  "{\"Userid\":\"%s\",\"msgid\":%lld,"\
	"\"mobile\":\"%s\","\
	"\"spno\":\"%s\","\
	"\"exno\":\"%s\","\
	"\"rtime\":\"%s\","\
	"\"content\":\"%s\"},"

#define JSON_RPT_RSP_BODY_V32  "{\"msgid\":%lld,"\
	"\"custid\":\"%s\","\
	"\"pknum\":%d,"\
	"\"pktotal\":%d,"\
	"\"mobile\":\"%s\","\
	"\"spno\":\"%s\","\
	"\"exno\":\"%s\","\
	"\"stime\":\"%s\","\
	"\"rtime\":\"%s\","\
	"\"ptime\":\"%s\","\
	"\"status\":%d,"\
	"\"errcode\":\"%s\","\
	"\"errdesc\":\"%s\","\
	"\"exdata\":\"%s\"},"


enum DEAL_STATUS
{
	DEAL_OK,
	DEAL_FAIL
};

#define ALL_SUCCESS_RSP	2           //全成功
#define ALL_FAIL_RSP 3              //全失败
#define SUCCESS_FAIL_RSP 4          //部分成功，部分失败
#define DETAILS_RSP 5				//详情

#define MSGID_TMP_LEN        64

//固定密码串
#define PWDSTR "00000000"

//rpt状态描述字符串
#define RPTSTATUSDESC_OK "success"

#define RPTSTATUSDESC_ERR "fail"

typedef map<string, string> STR_STRMAP;

//推送的时候才使用的错误码
#define ERRCODE_PUSH_USE_ONLY    88888

class DLIB_API CParseTool
{
public:
	CParseTool();
	virtual ~CParseTool();


public:
	//------推送MO、RPT------
	//标准请求
	virtual int MakeMoRequestV5_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, string &strMo);
	virtual int MakeRptRequestV5_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, const WBS_RPTERRCODEMAP& errcodemap, string &strRpt);

	//单条推送请求
	virtual int MakeMoRequestV5_std(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, string &strMo);
	virtual int MakeRptRequestV5_std(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, const WBS_RPTERRCODEMAP& errcodemap, string &strRpt);

	//回应
	virtual void ParseResponse_std(const char* pStart, RSP_PACK &rsp);

	//非标准请求
	virtual int MakeMoRequestV5_cstd(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strMo);
	virtual int MakeRptRequestV5_cstd(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strRpt);

	//模板单条推送MO/RPT
	virtual int MakeMoRequestV5_cstd(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &mapTmp, string &strMo);
	virtual int MakeRptRequestV5_cstd(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strRpt);

	//回应
	virtual void ParseResponse_cstd(const char* pStart, const WBSTEMPLATEINFO &mapTmp, const RSP_CMD_PUSH_TMP &pushRsp,
								RSP_PACK &rsp,std::vector<INT64> &vSuccess, std::vector<INT64> &vFailed);


	//V3.2协议接口
	virtual int MakeMoRequestV32_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, string &strMo);
	virtual int MakeRptRequestV32_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, string &strRpt);
	virtual int MakeMoRequestV32_cstd(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strMo);
	virtual int MakeRptRequestV32_cstd(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strRpt);

	//动态加载
	virtual void LoadConfig(int nType);
	CGlobal m_global;
};


//工厂方式调用库
class DLIB_API CParseToolFactory
{
public:
	static CParseTool* New();
	static void Destroy(CParseTool* pbase);
};

#pragma pack()
#endif
