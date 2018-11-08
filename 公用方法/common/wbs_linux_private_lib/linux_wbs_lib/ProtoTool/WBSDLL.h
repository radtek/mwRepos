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
//Ŀǰ��֧��linux��linuxʹ��֪ͨ�ķ�ʽ����window���¼���ʽ
#ifndef	LINUX
#define DLIB_API
#else
#define DLIB_API
#endif

#endif

#pragma pack(1)

//�ӳ�����ֹǰ����˿ո�ռ��λ��
#define MAX_RESULT_LEN		30

enum ERR_CODE
{
	ERR_OK,
	ERR_FMT = -100100,		//��ʽ���󣬰����ֶ����ʹ����ݰ���ʽ��
	ERR_FULL_CACHE,			//������
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

//��׼����·��
static const char* g_szReqStdPath = "sms/v1/std";

//��׼��ǩ
static const char *g_szUesrId = "userid";
static const char *g_szUesrIdEx = "sign";
static const char *g_szPwd = "pwd";
static const char *g_szSign = "signex";   //�����ͻ�sign
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

//��Ӧ��ǩ
static const char *g_szCmd = "cmd";
static const char *g_szSeqId = "seqid";


//xml��ǩ��
static const char *g_szMutiMtXml = "<multimt>";
static const char *g_szMutiMtXml2 = "</multimt>";

static const char *g_szMsgId = "msgid";     //�û���������ˮ��

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

//����mo����
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

//����rpt����
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

//������������mo����rptǰ�벿��
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

//�������͵���mo����rptǰ�벿��
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

#define ALL_SUCCESS_RSP	2           //ȫ�ɹ�
#define ALL_FAIL_RSP 3              //ȫʧ��
#define SUCCESS_FAIL_RSP 4          //���ֳɹ�������ʧ��
#define DETAILS_RSP 5				//����

#define MSGID_TMP_LEN        64

//�̶����봮
#define PWDSTR "00000000"

//rpt״̬�����ַ���
#define RPTSTATUSDESC_OK "success"

#define RPTSTATUSDESC_ERR "fail"

typedef map<string, string> STR_STRMAP;

//���͵�ʱ���ʹ�õĴ�����
#define ERRCODE_PUSH_USE_ONLY    88888

class DLIB_API CParseTool
{
public:
	CParseTool();
	virtual ~CParseTool();


public:
	//------����MO��RPT------
	//��׼����
	virtual int MakeMoRequestV5_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, string &strMo);
	virtual int MakeRptRequestV5_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, const WBS_RPTERRCODEMAP& errcodemap, string &strRpt);

	//������������
	virtual int MakeMoRequestV5_std(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, string &strMo);
	virtual int MakeRptRequestV5_std(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, const WBS_RPTERRCODEMAP& errcodemap, string &strRpt);

	//��Ӧ
	virtual void ParseResponse_std(const char* pStart, RSP_PACK &rsp);

	//�Ǳ�׼����
	virtual int MakeMoRequestV5_cstd(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strMo);
	virtual int MakeRptRequestV5_cstd(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strRpt);

	//ģ�嵥������MO/RPT
	virtual int MakeMoRequestV5_cstd(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &mapTmp, string &strMo);
	virtual int MakeRptRequestV5_cstd(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strRpt);

	//��Ӧ
	virtual void ParseResponse_cstd(const char* pStart, const WBSTEMPLATEINFO &mapTmp, const RSP_CMD_PUSH_TMP &pushRsp,
								RSP_PACK &rsp,std::vector<INT64> &vSuccess, std::vector<INT64> &vFailed);


	//V3.2Э��ӿ�
	virtual int MakeMoRequestV32_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, string &strMo);
	virtual int MakeRptRequestV32_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, string &strRpt);
	virtual int MakeMoRequestV32_cstd(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strMo);
	virtual int MakeRptRequestV32_cstd(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strRpt);

	//��̬����
	virtual void LoadConfig(int nType);
	CGlobal m_global;
};


//������ʽ���ÿ�
class DLIB_API CParseToolFactory
{
public:
	static CParseTool* New();
	static void Destroy(CParseTool* pbase);
};

#pragma pack()
#endif
