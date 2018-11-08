
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CParseToolImpl.h"
#include "adapter/Adapter.h"
#include "TypeDef.h"
#include "tools/XMLParse.h"
#include "tools/JsonParse.h"
#include "rapidxml/rapidxml_print.hpp"
//#include "common/md5.h"
#include "md5_new.h"
#include "LogMgr.h"
#include "tools/cJSON.h"
#include "adapter/Adapter.h"
//#include "AdapterExWbsLinux.h"
#include "iconv.hpp"
#include "PushHelper.h"
using namespace std;

char* MDStringMT(const char *szMd5Src, char *szOut/*至少33*/)
{
    char szMd5[16 + 1] = { 0 };
    unsigned int nMd5SrcLen = strlen(szMd5Src);
    GetMD5((UCHAR*)szMd5Src, nMd5SrcLen, (UCHAR*)szMd5);
    CGlobal::CharToASCIIEx(szMd5, 16, szOut);
    return szOut;
}


CParseToolImpl::CParseToolImpl()
{
	GetNameForConfigFile();
}

CParseToolImpl::~CParseToolImpl()
{

}

int CParseToolImpl::MakeMoRequestV5_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, string &strMo)
{
	//组推送mo请求包，http post
	if (vMoRptPack.empty())
	{
		return 0;
	}
	int nOutMsgLen = 0;

	try
	{
		//比较URL地址
		if (m_strMOURL_Gome.compare(std::string(urlinfo.szMoUrl)) == 0 || m_strMOURL_Gome_BUS.compare(std::string(urlinfo.szMoUrl)) == 0)
		{
			nOutMsgLen = MakeMoRequestV5_GOME(vMoRptPack, urlinfo, strMo);
		}
		else
		{
			nOutMsgLen = MakeMoRequestV5_std_pri(vMoRptPack, urlinfo, strMo);
		}
	}
	catch (...)
	{
		nOutMsgLen = -1;
	}

	return nOutMsgLen;
}


int CParseToolImpl::MakeMoRequestV5_std(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, string &strMo)
{
    char szMd5[32 + 1] = { 0 };
    char szMd5Src[MAX_PWD_LEN + 128 + 1] = { 0 };
    char szBody[MAX_MO_MSG_LEN * 3 * 3 + 1000] = { 0 };
    char szSendBuf[MAX_MO_MSG_LEN * 3 * 3 + 2048] = { 0 };
    int nBodyLen = 0;
    int nOutMsgLen = 0;

    try
    {
        if (URLENCODED == urlinfo.nPushMoFmt)
        {
            string strTime = CGlobal::date("mdHis");
            int nMd5SrcLen = _snprintf(szMd5Src, sizeof(szMd5Src), "%s%s%s%s", CAdapter::strupr((char*)urlinfo.szUsrId), PWDSTR, urlinfo.szPwd, strTime.c_str());
            MDStringMT(szMd5Src, szMd5);

            nBodyLen = _snprintf(szBody, sizeof(szBody), "userid=%s&pwd=%s&timestamp=%s&cmd=MO_REQ&seqid=%d&", urlinfo.szUsrId, szMd5, strTime.c_str(), MoRptPack.sockInfo.nSockId);

            //采用urlencode加密方式
            //CGlobal::UrlEncode((char*)MoRptPack.szMsgContent, (char*)szUrlMsg, sizeof(szUrlMsg), TRUE);
            std::string strMsgTmp = CGlobal::UrlEncode((char*)MoRptPack.szMsgContent);

            char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
            if ('*' != MoRptPack.szSpNumEx[0])
            {
                memcpy(szTmp, MoRptPack.szSpNumEx, strlen((char*)MoRptPack.szSpNumEx));
            }

            int nRetLen = _snprintf(szBody + nBodyLen, sizeof(szBody)-nBodyLen, URL_MO_REQ_BODY,
                                    CGlobal::TranMsgIdCharToI64(MoRptPack.szMsgId),
                                    MoRptPack.szSrcTerminalId,
                                    MoRptPack.szDestId,
                                    szTmp,
                                    MoRptPack.szTime,
                                    strMsgTmp.c_str()
                                    );
            nBodyLen += nRetLen;

            nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                                   "POST %s HTTP/1.1\r\n"
                                   "Host: %s:%d\r\n"
                                   "Connection: Close\r\n"
                                   "Content-Type: application/x-www-form-urlencoded\r\n"
                                   "Content-Length: %d\r\n\r\n"
                                   "%s",
                                   urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, nBodyLen, szBody);

            strMo = szSendBuf;
        }
        else if (XML == urlinfo.nPushMoFmt)
        {
            string strTime = CGlobal::date("mdHis");
            int nMd5SrcLen = _snprintf(szMd5Src, sizeof(szMd5Src), "%s%s%s%s", CAdapter::strupr((char*)urlinfo.szUsrId), PWDSTR, urlinfo.szPwd, strTime.c_str());
            MDStringMT(szMd5Src, szMd5);

            nBodyLen = _snprintf(szBody, sizeof(szBody), XML_PUSHSINGLEMO_PRE, urlinfo.szUsrId, szMd5, strTime.c_str(), MoRptPack.sockInfo.nSockId);

            //采用urlencode加密方式
            //CGlobal::UrlEncode((char*)MoRptPack.szMsgContent, (char*)szUrlMsg, sizeof(szUrlMsg), TRUE);
            std::string strMsgTmp = CGlobal::UrlEncode((char*)MoRptPack.szMsgContent);

            char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
            if ('*' != MoRptPack.szSpNumEx[0])
            {
                memcpy(szTmp, MoRptPack.szSpNumEx, strlen((char*)MoRptPack.szSpNumEx));
            }

            int nRetLen = _snprintf(szBody + nBodyLen, sizeof(szBody)-nBodyLen, XML_MO_SINGLE_REQ_BODY,
                                    CGlobal::TranMsgIdCharToI64(MoRptPack.szMsgId),
                                    MoRptPack.szSrcTerminalId,
                                    MoRptPack.szDestId,
                                    szTmp,
                                    MoRptPack.szTime,
                                    strMsgTmp.c_str()
                                    );
            nBodyLen += nRetLen;

            nBodyLen += strlen(g_szMoReq) + 3;
            nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                                   "POST %s HTTP/1.1\r\n"
                                   "Host: %s:%d\r\n"
                                   "Connection: Close\r\n"
                                   "Content-Type: text/xml\r\n"
                                   "Content-Length: %d\r\n\r\n"
                                   "%s</%s>",
                                   urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, nBodyLen, szBody, g_szMoReq);

            strMo = szSendBuf;
        }
        else
        {
            string strTime = CGlobal::date("mdHis");
            int nMd5SrcLen = _snprintf(szMd5Src, sizeof(szMd5Src), "%s%s%s%s", CAdapter::strupr((char*)urlinfo.szUsrId), PWDSTR, urlinfo.szPwd, strTime.c_str());
            MDStringMT(szMd5Src, szMd5);

            nBodyLen = _snprintf(szBody, sizeof(szBody), JSON_PUSHSINGLERPT_PRE, urlinfo.szUsrId, szMd5, strTime.c_str(), g_szMoCmd, MoRptPack.sockInfo.nSockId);

            //采用urlencode加密方式
            //CGlobal::UrlEncode((char*)MoRptPack.szMsgContent, (char*)szUrlMsg, sizeof(szUrlMsg), TRUE);
            std::string strMsgTmp = CGlobal::UrlEncode((char*)MoRptPack.szMsgContent);

            char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
            if ('*' != MoRptPack.szSpNumEx[0])
            {
                memcpy(szTmp, MoRptPack.szSpNumEx, strlen((char*)MoRptPack.szSpNumEx));
            }

            int nRetLen = _snprintf(szBody + nBodyLen, sizeof(szBody)-nBodyLen, JSON_MO_SINGLE_REQ_BODY,
                                    CGlobal::TranMsgIdCharToI64(MoRptPack.szMsgId),
                                    MoRptPack.szSrcTerminalId,
                                    MoRptPack.szDestId,
                                    szTmp,
                                    MoRptPack.szTime,
                                    strMsgTmp.c_str()
                                    );
            nBodyLen += nRetLen;

            nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                                   "POST %s HTTP/1.1\r\n"
                                   "Host: %s:%d\r\n"
                                   "Connection: Close\r\n"
                                   "Content-Type: text/json\r\n"
                                   "Content-Length: %d\r\n\r\n"
                                   "%s",
                                   urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, nBodyLen, szBody);

            strMo = szSendBuf;
        }
    }
    catch (...)
    {
        nOutMsgLen = -1;
    }

    return nOutMsgLen;
}


int CParseToolImpl::MakeMoRequestV5_cstd(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strMo)
{
	//组mo推送报文，模板方式
	if (vMoRptPack.empty())
	{
		return 0;
	}

	int nOutMsgLen = 0;
	char szSendBuf[MAX_PUSHMO_LEN] = { 0 };
	char szMd5[32 + 1] = { 0 };
	char szMd5Src[MAX_PWD_LEN + 128 + 1] = { 0 };
	char szTmp[MAX_PUSH_PRE_LEN] = { 0 };
	int nTmp = 0;

	try
	{

		if (XML == nRetMode)
		{
			MakePushMoXMLData(allFiled, vMoRptPack, urlinfo, strMo);
			//nTmp = _snprintf(szTmp, sizeof(szTmp), XML_PUSH_PRE, urlinfo.szUsrId, szMd5, strTime.c_str(), g_szMoCmd, vMoRptPack[0].sockInfo.nSockId);
			nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
				"POST %s HTTP/1.1\r\n"
				"Host: %s:%d\r\n"
				"Connection: Close\r\n"
				"Content-Type: text/xml\r\n"
				"Content-Length: %d\r\n\r\n%s",
				urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, strMo.length(), strMo.c_str());
		}
		else if (URLENCODED == nRetMode)
		{
			MakePushMoURLData(allFiled, vMoRptPack, urlinfo, strMo);
			nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
				"POST %s HTTP/1.1\r\n"
				"Host: %s:%d\r\n"
				"Connection: Close\r\n"
				"Content-Type: application/x-www-form-urlencoded\r\n"
				"Content-Length: %d\r\n\r\n%s",
				urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, strMo.length(), strMo.c_str());
		}
		else
		{
			MakePushMoJsonData(allFiled, vMoRptPack, urlinfo, strMo);
			nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
				"POST %s HTTP/1.1\r\n"
				"Host: %s:%d\r\n"
				"Connection: Close\r\n"
				"Content-Type: text/json\r\n"
				"Content-Length: %d\r\n\r\n%s",
				urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, strMo.length(), strMo.c_str());
		}

		strMo = szSendBuf;

	}
	catch (...)
	{
		nOutMsgLen = -1;
	}



	return nOutMsgLen;
}


int CParseToolImpl::MakeMoRequestV5_cstd(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strMo)
{
    //组mo推送报文，模板方式
    int nOutMsgLen = 0;
    char szSendBuf[MAX_PUSHMO_LEN] = { 0 };
    char szMd5[32 + 1] = { 0 };
    char szMd5Src[MAX_PWD_LEN + 128 + 1] = { 0 };
    char szTmp[MAX_PUSH_PRE_LEN] = { 0 };
    int nTmp = 0;

    try
    {
    	//艺龙生产类个性化
		if (std::find(m_vcYLName.begin(), m_vcYLName.end(), std::string(urlinfo.szUsrId)) != m_vcYLName.end())
		{
			return nOutMsgLen = MakeMoRequestV5_YL(MoRptPack, urlinfo, strMo);
		}

        if (XML == nRetMode)
        {
            MakePushMoXMLData(allFiled, MoRptPack, urlinfo, strMo);
            nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                                   "POST %s HTTP/1.1\r\n"
                                   "Host: %s:%d\r\n"
                                   "Connection: Close\r\n"
                                   "Content-Type: text/xml\r\n"
                                   "Content-Length: %d\r\n\r\n%s",
                                   urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, strMo.length(), strMo.c_str());
        }
        else if (URLENCODED == nRetMode)
        {
            MakePushMoURLData(allFiled, MoRptPack, urlinfo, strMo);
            nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                                   "POST %s HTTP/1.1\r\n"
                                   "Host: %s:%d\r\n"
                                   "Connection: Close\r\n"
                                   "Content-Type: application/x-www-form-urlencoded\r\n"
                                   "Content-Length: %d\r\n\r\n%s",
                                   urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, strMo.length(), strMo.c_str());
        }
        else
        {
            MakePushMoJsonData(allFiled, MoRptPack, urlinfo, strMo);
            nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                                   "POST %s HTTP/1.1\r\n"
                                   "Host: %s:%d\r\n"
                                   "Connection: Close\r\n"
                                   "Content-Type: text/json\r\n"
                                   "Content-Length: %d\r\n\r\n%s",
                                   urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, strMo.length(), strMo.c_str());
        }

        strMo = szSendBuf;

    }
    catch (...)
    {
        nOutMsgLen = -1;
    }



    return nOutMsgLen;
}



int CParseToolImpl::MakeRptRequestV5_std(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, const WBS_RPTERRCODEMAP& errcodemap, string &strRpt)
	{
	char szMd5[32 + 1] = { 0 };
	char szMd5Src[MAX_PWD_LEN + 128 + 1] = { 0 };
	char szBody[MAX_PUSHRPT_LEN] = { 0 };
	char szSendBuf[MAX_PUSHRPT_LEN] = { 0 };
	int nBodyLen = 0;
	int nOutMsgLen = 0;
	string strErrDesc("");
	int nStatus = -1;

	try
	{
		if (XML == urlinfo.nPushRptFmt)
		{
			string strTime = CGlobal::date("mdHis");
			int nMd5SrcLen = _snprintf(szMd5Src, sizeof(szMd5Src), "%s%s%s%s", CAdapter::strupr((char*)urlinfo.szUsrId), PWDSTR, urlinfo.szPwd, strTime.c_str());
			MDStringMT(szMd5Src, szMd5);

			nBodyLen = _snprintf(szBody, sizeof(szBody), XML_PUSHSINGLERPT_PRE, urlinfo.szUsrId, szMd5, strTime.c_str(), MoRptPack.sockInfo.nSockId);

			char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
			if ('*' != MoRptPack.szSpNumEx[0])
			{
				memcpy(szTmp, MoRptPack.szSpNumEx, strlen((char*)MoRptPack.szSpNumEx));
			}
			
			//确定成功和失败描述
			std::string strErrCode = (char*)MoRptPack.msgcontent.szState;
			nStatus = _strnicmp((LPCSTR)MoRptPack.msgcontent.szState, "DELIVRD", 7) == 0 ? 0 : 2;
			if (errcodemap.size() <= 0)
			{
				if (0 == nStatus)
				{
					strErrDesc = RPTSTATUSDESC_OK;
				}
				else
				{
					strErrDesc = RPTSTATUSDESC_ERR;
				}
			}
			else
			{
				DealECErrcode(errcodemap, strErrCode, strErrDesc);
			}

			int nRetLen = _snprintf(szBody + nBodyLen, sizeof(szBody)-nBodyLen, XML_RPT_SINGLE_REQ_BODY,
				CGlobal::TranMsgIdCharToI64(MoRptPack.szMsgId),
				MoRptPack.szCustId,
				MoRptPack.nPknum,
				MoRptPack.nPkTotal,
				MoRptPack.msgcontent.szDestTerminalId,
				MoRptPack.szDestId,
				szTmp,
				MoRptPack.szSendTime,
				MoRptPack.szTime,
				nStatus,
				strErrCode.c_str(),
				strErrDesc.c_str(),
				MoRptPack.szExData
				);
			nBodyLen += nRetLen;

			//加上结束标记的长度
			nBodyLen += strlen(g_szRptReq) + 3;

			char szSendBuf[MAX_PUSHRPT_LEN] = { 0 };
			//默认走原来代码
			nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
				"POST %s HTTP/1.1\r\n"
				"Host: %s:%d\r\n"
				"Connection: Close\r\n"
				"Content-Type: text/xml\r\n"
				"Content-Length: %d\r\n\r\n"
				"%s</%s>",
				urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, nBodyLen, szBody, g_szRptReq);

			

			strRpt = szSendBuf;
		}
		else if (URLENCODED == urlinfo.nPushRptFmt)
		{
			string strTime = CGlobal::date("mdHis");
			int nMd5SrcLen = _snprintf(szMd5Src, sizeof(szMd5Src), "%s%s%s%s", CAdapter::strupr((char*)urlinfo.szUsrId), PWDSTR, urlinfo.szPwd, strTime.c_str());
			MDStringMT(szMd5Src, szMd5);

			nBodyLen = _snprintf(szBody, sizeof(szBody), "userid=%s&pwd=%s&timestamp=%s&cmd=RPT_REQ&seqid=%d&", urlinfo.szUsrId, szMd5, strTime.c_str(), MoRptPack.sockInfo.nSockId);

			char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
			if ('*' != MoRptPack.szSpNumEx[0])
			{
				memcpy(szTmp, MoRptPack.szSpNumEx, strlen((char*)MoRptPack.szSpNumEx));
			}

			//确定成功和失败描述
			std::string strErrCode = (char*)MoRptPack.msgcontent.szState;
			nStatus = _strnicmp((LPCSTR)MoRptPack.msgcontent.szState, "DELIVRD", 7) == 0 ? 0 : 2;
			if (errcodemap.size() <= 0)
			{
				if (0 == nStatus)
				{
					strErrDesc = RPTSTATUSDESC_OK;
				}
				else
				{
					strErrDesc = RPTSTATUSDESC_ERR;
				}
			}
			else
			{
				DealECErrcode(errcodemap, strErrCode, strErrDesc);
			}

			int nRetLen = _snprintf(szBody + nBodyLen, sizeof(szBody)-nBodyLen, URL_RPT_RSP_BODY,
				CGlobal::TranMsgIdCharToI64(MoRptPack.szMsgId),
				MoRptPack.szCustId,
				MoRptPack.nPknum,
				MoRptPack.nPkTotal,
				MoRptPack.msgcontent.szDestTerminalId,
				MoRptPack.szDestId,
				szTmp,
				MoRptPack.szSendTime,
				MoRptPack.szTime,
				nStatus,
				strErrCode.c_str(),
				strErrDesc.c_str(),
				MoRptPack.szExData
				);
			nBodyLen += nRetLen;

			nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
				"POST %s HTTP/1.1\r\n"
				"Host: %s:%d\r\n"
				"Connection: Close\r\n"
				"Content-Type: application/x-www-form-urlencoded\r\n"
				"Content-Length: %d\r\n\r\n"
				"%s",
				urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, nBodyLen, szBody);

			strRpt = szSendBuf;
		}
		else
		{
			string strTime = CGlobal::date("mdHis");
			int nMd5SrcLen = _snprintf(szMd5Src, sizeof(szMd5Src), "%s%s%s%s", CAdapter::strupr((char*)urlinfo.szUsrId), PWDSTR, urlinfo.szPwd, strTime.c_str());
			MDStringMT(szMd5Src, szMd5);

			nBodyLen = _snprintf(szBody, sizeof(szBody), JSON_PUSHSINGLERPT_PRE, urlinfo.szUsrId, szMd5, strTime.c_str(), g_szRptCmd, MoRptPack.sockInfo.nSockId);

			char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
			if ('*' != MoRptPack.szSpNumEx[0])
			{
				memcpy(szTmp, MoRptPack.szSpNumEx, strlen((char*)MoRptPack.szSpNumEx));
			}

			//确定成功和失败描述
			std::string strErrCode = (char*)MoRptPack.msgcontent.szState;
			nStatus = _strnicmp((LPCSTR)MoRptPack.msgcontent.szState, "DELIVRD", 7) == 0 ? 0 : 2;
			if (errcodemap.size() <= 0)
			{
				if (0 == nStatus)
				{
					strErrDesc = RPTSTATUSDESC_OK;
				}
				else
				{
					strErrDesc = RPTSTATUSDESC_ERR;
				}
			}
			else
			{
				DealECErrcode(errcodemap, strErrCode, strErrDesc);
			}
			
			int nRetLen = _snprintf(szBody + nBodyLen, sizeof(szBody)-nBodyLen, JSON_RPT_SINGLE_REQ_BODY,
				CGlobal::TranMsgIdCharToI64(MoRptPack.szMsgId),
				MoRptPack.szCustId,
				MoRptPack.nPknum,
				MoRptPack.nPkTotal,
				MoRptPack.msgcontent.szDestTerminalId,
				MoRptPack.szDestId,
				szTmp,
				MoRptPack.szSendTime,
				MoRptPack.szTime,
				nStatus,
				strErrCode.c_str(),
				strErrDesc.c_str(),
				MoRptPack.szExData
				);
			nBodyLen += nRetLen;

			nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
				"POST %s HTTP/1.1\r\n"
				"Host: %s:%d\r\n"
				"Connection: Close\r\n"
				"Content-Type: text/json\r\n"
				"Content-Length: %d\r\n\r\n"
				"%s",
				urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, nBodyLen, szBody);
			

			strRpt = szSendBuf;
		}
	}
	catch (...)
	{
		nOutMsgLen = -1;
	}

	return nOutMsgLen;
}

int CParseToolImpl::MakeRptRequestV5_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, const WBS_RPTERRCODEMAP& errcodemap, string &strRpt)
{
    if (vMoRptPack.empty())
    {
        return 0;
    }
    int nOutMsgLen = 0;

    try
    {
        if (m_strRPTURL_Gome_BUS.compare(std::string(urlinfo.szRptUrl)) == 0)
        {
            nOutMsgLen = MakeRptRequestV5_GOME(vMoRptPack, urlinfo, true, strRpt);
        }
        else if (m_strRPTURL_Gome.compare(std::string(urlinfo.szRptUrl)) == 0)
        {
            nOutMsgLen = MakeRptRequestV5_GOME(vMoRptPack, urlinfo, false, strRpt);
        }
        else
        {
            nOutMsgLen = MakeRptRequestV5_std_pri(vMoRptPack, urlinfo, errcodemap, strRpt);
        }
    }
    catch (...)
    {
        nOutMsgLen = -1;
    }

    return nOutMsgLen;
}



int CParseToolImpl::MakeRptRequestV5_cstd(const std::vector<PACK_DELIVER> &FileRptPackArr, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strRpt)
{
    int nOutMsgLen = 0;
    char szSendBuf[MAX_PUSHRPT_LEN] = { 0 };
    int nTmp = 0;

    try
    {
        int nSize = FileRptPackArr.size();
        std::vector<PACK_DELIVER>::const_iterator itFileRpt = FileRptPackArr.begin();

        if (URLENCODED == nRetMode)
        {
            MakePushRptURLData(allFiled, FileRptPackArr, urlinfo, strRpt);
            nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                                   "POST %s HTTP/1.1\r\n"
                                   "Host: %s:%d\r\n"
                                   "Connection: Close\r\n"
                                   "Content-Type: application/x-www-form-urlencoded\r\n"
                                   "Content-Length: %d\r\n\r\n%s",
                                   urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, strRpt.length(), strRpt.c_str());
        }
        else if (XML == nRetMode)
        {
            MakePushRptXMLData(allFiled, FileRptPackArr, urlinfo, strRpt);
            nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                                   "POST %s HTTP/1.1\r\n"
                                   "Host: %s:%d\r\n"
                                   "Connection: Close\r\n"
                                   "Content-Type: text/xml\r\n"
                                   "Content-Length: %d\r\n\r\n%s",
                                   urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, strRpt.length(), strRpt.c_str());
        }
        else
        {
            //默认走json
            MakePushRptJsonData(allFiled, FileRptPackArr, urlinfo, strRpt);
            nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                                   "POST %s HTTP/1.1\r\n"
                                   "Host: %s:%d\r\n"
                                   "Connection: Close\r\n"
                                   "Content-Type: text/json\r\n"
                                   "Content-Length: %d\r\n\r\n%s",
                                   urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, strRpt.length(), strRpt.c_str());
        }


        strRpt = szSendBuf;
    }
    catch (...)
    {
        nOutMsgLen = -1;
    }

    return nOutMsgLen;
}

int CParseToolImpl::MakeRptRequestV5_cstd(const PACK_DELIVER &FileRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strRpt)
{
    int nOutMsgLen = 0;
    char szSendBuf[MAX_PUSHRPT_LEN] = { 0 };
    int nTmp = 0;

    try
    {
		//艺龙生产类个性化
		if (std::find(m_vcYLName.begin(), m_vcYLName.end(), std::string(urlinfo.szUsrId)) != m_vcYLName.end())
		{
			return nOutMsgLen = MakeRptRequestV5_YL(FileRptPack, urlinfo, strRpt);
		}

        if (URLENCODED == nRetMode)
        {
            MakePushRptURLData(allFiled, FileRptPack, urlinfo, strRpt);
            nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                                   "POST %s HTTP/1.1\r\n"
                                   "Host: %s:%d\r\n"
                                   "Connection: Close\r\n"
                                   "Content-Type: application/x-www-form-urlencoded\r\n"
                                   "Content-Length: %d\r\n\r\n%s",
                                   urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, strRpt.length(), strRpt.c_str());
        }
        else if (XML == nRetMode)
        {
            MakePushRptXMLData(allFiled, FileRptPack, urlinfo, strRpt);
            nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                                   "POST %s HTTP/1.1\r\n"
                                   "Host: %s:%d\r\n"
                                   "Connection: Close\r\n"
                                   "Content-Type: text/xml\r\n"
                                   "Content-Length: %d\r\n\r\n%s",
                                   urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, strRpt.length(), strRpt.c_str());
        }
        else
        {
            //默认走json
            MakePushRptJsonData(allFiled, FileRptPack, urlinfo, strRpt);
            nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                                   "POST %s HTTP/1.1\r\n"
                                   "Host: %s:%d\r\n"
                                   "Connection: Close\r\n"
                                   "Content-Type: text/json\r\n"
                                   "Content-Length: %d\r\n\r\n%s",
                                   urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, strRpt.length(), strRpt.c_str());
        }


        strRpt = szSendBuf;
    }
    catch (...)
    {
        nOutMsgLen = -1;
    }

    return nOutMsgLen;
}






int CParseToolImpl::MakeMoRequestV5_std_pri(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, string &strMo)
{//Primary
    if (vMoRptPack.empty())
    {
        return 0;
    }

    int nOutMsgLen = 0;

    try
    {
        char szMd5[32 + 1] = { 0 };
        char szMd5Src[MAX_PWD_LEN + 128 + 1] = { 0 };
        char szBody[MAX_PUSHMO_LEN - MAX_HTTP_HEAD - 10] = { 0 };
        char szSendBuf[MAX_PUSHMO_LEN] = { 0 };
        int nBodyLen = 0;
        if (URLENCODED == urlinfo.nPushMoFmt)
        {
            string strTime = CGlobal::date("mdHis");
            int nMd5SrcLen = _snprintf(szMd5Src, sizeof(szMd5Src), "%s%s%s%s", CAdapter::strupr((char*)urlinfo.szUsrId), PWDSTR, urlinfo.szPwd, strTime.c_str());
            MDStringMT(szMd5Src, szMd5);

            nBodyLen = _snprintf(szBody, sizeof(szBody), "userid=%s&pwd=%s&timestamp=%s&cmd=MO_REQ&seqid=%d&mos=", urlinfo.szUsrId, szMd5, strTime.c_str(), vMoRptPack[0].sockInfo.nSockId);

            std::vector<PACK_DELIVER>::const_iterator it = vMoRptPack.begin();

			std::string strMosTmp = "[";
				
            //组包体中变化的内容
            for (it; it != vMoRptPack.end(); ++it)
            {
                //采用urlencode加密方式
                //CGlobal::UrlEncode((char*)it->szMsgContent, (char*)szUrlMsg, sizeof(szUrlMsg), TRUE);
                std::string strMsgTmp = CGlobal::UrlEncode((char*)it->szMsgContent);

                char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
                if ('*' != it->szSpNumEx[0])
                {
                    memcpy(szTmp, it->szSpNumEx, strlen((char*)it->szSpNumEx));
                }

				strMosTmp += cstr::format(JSON_MO_RSP_BODY,
					             		CGlobal::TranMsgIdCharToI64(it->szMsgId),
                                        it->szSrcTerminalId,
                                        it->szDestId,
                                        szTmp,
                                        it->szTime,
                                        strMsgTmp.c_str());
				
            }
			
			int nRetLen = strMosTmp.size();
			
            //去掉最后一个逗号
			if (nRetLen > 0 && ',' == strMosTmp[nRetLen - 1])
            {
				strMosTmp[nRetLen - 1] = ']';
            }

			if(WBS_URLENCODE_X2 == urlinfo.nPushCodeType)
			{
				//对mos整体内容进行URLENCODE编码
				strMosTmp = CGlobal::UrlEncode(strMosTmp.c_str());
			}

			//拼接
			nBodyLen += _snprintf(szBody + nBodyLen, sizeof(szBody)-nBodyLen, "%s", strMosTmp.c_str());

			nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                                   "POST %s HTTP/1.1\r\n"
                                   "Host: %s:%d\r\n"
                                   "Connection: Close\r\n"
                                   "Content-Type: application/x-www-form-urlencoded\r\n"
                                   "Content-Length: %d\r\n\r\n"
                                   "%s",
                                   urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, nBodyLen, szBody);

            strMo = szSendBuf;
        }
        else if (XML == urlinfo.nPushMoFmt)
        {
            string strTime = CGlobal::date("mdHis");
            int nMd5SrcLen = _snprintf(szMd5Src, sizeof(szMd5Src), "%s%s%s%s", CAdapter::strupr((char*)urlinfo.szUsrId), PWDSTR, urlinfo.szPwd, strTime.c_str());
            MDStringMT(szMd5Src, szMd5);

            nBodyLen = _snprintf(szBody, sizeof(szBody), XML_PUSHMO_PRE, urlinfo.szUsrId, szMd5, strTime.c_str(), vMoRptPack[0].sockInfo.nSockId);

            std::vector<PACK_DELIVER>::const_iterator it = vMoRptPack.begin();
            //组包体中变化的内容
            for (it; it != vMoRptPack.end(); ++it)
            {
                //采用urlencode加密方式
                //CGlobal::UrlEncode((char*)it->szMsgContent, (char*)szUrlMsg, sizeof(szUrlMsg), TRUE);
                std::string strMsgTmp = CGlobal::UrlEncode((char*)it->szMsgContent);

                char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
                if ('*' != it->szSpNumEx[0])
                {
                    memcpy(szTmp, it->szSpNumEx, strlen((char*)it->szSpNumEx));
                }

                int nRetLen = _snprintf(szBody + nBodyLen, sizeof(szBody)-nBodyLen, XML_MO_RSP_BODY,
                                        CGlobal::TranMsgIdCharToI64(it->szMsgId),
                                        it->szSrcTerminalId,
                                        it->szDestId,
                                        szTmp,
                                        it->szTime,
                                        strMsgTmp.c_str()
                                        );
                nBodyLen += nRetLen;
            }

            nBodyLen += strlen(g_szMOS) + strlen(g_szMoReq) + 3 * 2;
            nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                                   "POST %s HTTP/1.1\r\n"
                                   "Host: %s:%d\r\n"
                                   "Connection: Close\r\n"
                                   "Content-Type: text/xml\r\n"
                                   "Content-Length: %d\r\n\r\n"
                                   "%s</%s></%s>",
                                   urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, nBodyLen, szBody, g_szMOS, g_szMoReq);

            strMo = szSendBuf;
        }
        else
        {
            string strTime = CGlobal::date("mdHis");
            int nMd5SrcLen = _snprintf(szMd5Src, sizeof(szMd5Src), "%s%s%s%s", CAdapter::strupr((char*)urlinfo.szUsrId), PWDSTR, urlinfo.szPwd, strTime.c_str());
            MDStringMT(szMd5Src, szMd5);

            nBodyLen = _snprintf(szBody, sizeof(szBody), JSON_PUSHMO_PRE, urlinfo.szUsrId, szMd5, strTime.c_str(), g_szMoCmd, vMoRptPack[0].sockInfo.nSockId);

            std::vector<PACK_DELIVER>::const_iterator it = vMoRptPack.begin();
            //组包体中变化的内容
            for (it; it != vMoRptPack.end(); ++it)
            {
                //采用urlencode加密方式
                //CGlobal::UrlEncode((char*)it->szMsgContent, (char*)szUrlMsg, sizeof(szUrlMsg), TRUE);
                std::string strMsgTmp = CGlobal::UrlEncode((char*)it->szMsgContent);

                char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
                if ('*' != it->szSpNumEx[0])
                {
                    memcpy(szTmp, it->szSpNumEx, strlen((char*)it->szSpNumEx));
                }

                int nRetLen = _snprintf(szBody + nBodyLen, sizeof(szBody)-nBodyLen, JSON_MO_RSP_BODY,
                                        CGlobal::TranMsgIdCharToI64(it->szMsgId),
                                        it->szSrcTerminalId,
                                        it->szDestId,
                                        szTmp,
                                        it->szTime,
                                        strMsgTmp.c_str()
                                        );
                nBodyLen += nRetLen;
            }

            //去掉最后一个逗号
            if (nBodyLen > 0 && ',' == szBody[nBodyLen - 1])
            {
                szBody[nBodyLen - 1] = ']';
                szBody[nBodyLen] = '}';
                nBodyLen++;
            }

            nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                                   "POST %s HTTP/1.1\r\n"
                                   "Host: %s:%d\r\n"
                                   "Connection: Close\r\n"
                                   "Content-Type: text/json\r\n"
                                   "Content-Length: %d\r\n\r\n"
                                   "%s",
                                   urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, nBodyLen, szBody);

            strMo = szSendBuf;
        }
    }
    catch (...)
    {
        nOutMsgLen = -1;
    }

    return nOutMsgLen;
}




int CParseToolImpl::MakeRptRequestV5_std_pri(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, const WBS_RPTERRCODEMAP& errcodemap, string &strRpt)
{
    if (vMoRptPack.empty())
    {
        return 0;
    }

    char szMd5[32 + 1] = { 0 };
    char szMd5Src[MAX_PWD_LEN + 128 + 1] = { 0 };
    char szBody[MAX_PUSHRPT_LEN] = { 0 };
    char szSendBuf[MAX_PUSHRPT_LEN] = { 0 };
    int nBodyLen = 0;
    int nOutMsgLen = 0;
    string strErrDesc("");
    int nStatus = -1;

    try
    {
        if (JSON == urlinfo.nPushRptFmt)
        {
            string strTime = CGlobal::date("mdHis");
            int nMd5SrcLen = _snprintf(szMd5Src, sizeof(szMd5Src), "%s%s%s%s", CAdapter::strupr((char*)urlinfo.szUsrId), PWDSTR, urlinfo.szPwd, strTime.c_str());
            MDStringMT(szMd5Src, szMd5);

            nBodyLen = _snprintf(szBody, sizeof(szBody), JSON_PUSHRPT_PRE, urlinfo.szUsrId, szMd5, strTime.c_str(), g_szRptCmd, vMoRptPack[0].sockInfo.nSockId);

            std::vector<PACK_DELIVER>::const_iterator it = vMoRptPack.begin();
            //组包体中变化的内容
            for (it; it != vMoRptPack.end(); ++it)
            {
                char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
                if ('*' != it->szSpNumEx[0])
                {
                    memcpy(szTmp, it->szSpNumEx, strlen((char*)it->szSpNumEx));
                }

				//确定成功和失败描述
				std::string strErrCode = (char*)it->msgcontent.szState;
				nStatus = _strnicmp((LPCSTR)it->msgcontent.szState, "DELIVRD", 7) == 0 ? 0 : 2;
				if (errcodemap.size() <= 0)
				{
					if (0 == nStatus)
					{
						strErrDesc = RPTSTATUSDESC_OK;
					}
					else
					{
						strErrDesc = RPTSTATUSDESC_ERR;
					}
				}
				else
				{
					DealECErrcode(errcodemap, strErrCode, strErrDesc);
				}

                int nRetLen = _snprintf(szBody + nBodyLen, sizeof(szBody)-nBodyLen, JSON_RPT_RSP_BODY,
                                        CGlobal::TranMsgIdCharToI64(it->szMsgId),
                                        it->szCustId,
                                        it->nPknum,
                                        it->nPkTotal,
                                        it->msgcontent.szDestTerminalId,
                                        it->szDestId,
                                        szTmp,
                                        it->szSendTime,
                                        it->szTime,
                                        nStatus,
                                        strErrCode.c_str(),
                                        strErrDesc.c_str(),
                                        it->szExData
                                        );
                nBodyLen += nRetLen;
            }

            //去掉最后一个逗号
            if (nBodyLen > 0 && ',' == szBody[nBodyLen - 1])
            {
                szBody[nBodyLen - 1] = ']';
                szBody[nBodyLen] = '}';
                nBodyLen++;
            }


            nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                                   "POST %s HTTP/1.1\r\n"
                                   "Host: %s:%d\r\n"
                                   "Connection: Close\r\n"
                                   "Content-Type: text/json\r\n"
                                   "Content-Length: %d\r\n\r\n"
                                   "%s",
                                   urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, nBodyLen, szBody);

            strRpt = szSendBuf;
        }
        else if (XML == urlinfo.nPushRptFmt)
        {
            string strTime = CGlobal::date("mdHis");
            int nMd5SrcLen = _snprintf(szMd5Src, sizeof(szMd5Src), "%s%s%s%s", CAdapter::strupr((char*)urlinfo.szUsrId), PWDSTR, urlinfo.szPwd, strTime.c_str());
            MDStringMT(szMd5Src, szMd5);

            nBodyLen = _snprintf(szBody, sizeof(szBody), XML_PUSHRPT_PRE, urlinfo.szUsrId, szMd5, strTime.c_str(), vMoRptPack[0].sockInfo.nSockId);

            std::vector<PACK_DELIVER>::const_iterator it = vMoRptPack.begin();
            //组包体中变化的内容
            for (it; it != vMoRptPack.end(); ++it)
            {
                char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
                if ('*' != it->szSpNumEx[0])
                {
                    memcpy(szTmp, it->szSpNumEx, strlen((char*)it->szSpNumEx));
                }

				//确定成功和失败描述
				std::string strErrCode = (char*)it->msgcontent.szState;
				nStatus = _strnicmp((LPCSTR)it->msgcontent.szState, "DELIVRD", 7) == 0 ? 0 : 2;
				if (errcodemap.size() <= 0)
				{
					if (0 == nStatus)
					{
						strErrDesc = RPTSTATUSDESC_OK;
					}
					else
					{
						strErrDesc = RPTSTATUSDESC_ERR;
					}
				}
				else
				{
					DealECErrcode(errcodemap, strErrCode, strErrDesc);
				}

                int nRetLen = _snprintf(szBody + nBodyLen, sizeof(szBody)-nBodyLen, XML_RPT_RSP_BODY,
                                        CGlobal::TranMsgIdCharToI64(it->szMsgId),
                                        it->szCustId,
                                        it->nPknum,
                                        it->nPkTotal,
                                        it->msgcontent.szDestTerminalId,
                                        it->szDestId,
                                        szTmp,
                                        it->szSendTime,
                                        it->szTime,
                                        nStatus,
                                        strErrCode.c_str(),
                                        strErrDesc.c_str(),
                                        it->szExData
                                        );
                nBodyLen += nRetLen;
            }

            //加上结束标记的长度
            nBodyLen += strlen(g_szRpts) + strlen(g_szRptReq) + 3 * 2;

            char szSendBuf[MAX_PUSHRPT_LEN] = { 0 };
            nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                                   "POST %s HTTP/1.1\r\n"
                                   "Host: %s:%d\r\n"
                                   "Connection: Close\r\n"
                                   "Content-Type: text/xml\r\n"
                                   "Content-Length: %d\r\n\r\n"
                                   "%s</%s></%s>",
                                   urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, nBodyLen, szBody, g_szRpts, g_szRptReq);

            strRpt = szSendBuf;
        }
        else if (URLENCODED == urlinfo.nPushRptFmt)
        {
            string strTime = CGlobal::date("mdHis");
            int nMd5SrcLen = _snprintf(szMd5Src, sizeof(szMd5Src), "%s%s%s%s", CAdapter::strupr((char*)urlinfo.szUsrId), PWDSTR, urlinfo.szPwd, strTime.c_str());
            MDStringMT(szMd5Src, szMd5);

            nBodyLen = _snprintf(szBody, sizeof(szBody), "userid=%s&pwd=%s&timestamp=%s&cmd=RPT_REQ&seqid=%d&rpts=", urlinfo.szUsrId, szMd5, strTime.c_str(), vMoRptPack[0].sockInfo.nSockId);

			std::string strRptsTmp = "[";
			
            std::vector<PACK_DELIVER>::const_iterator it = vMoRptPack.begin();
            //组包体中变化的内容
            for (it; it != vMoRptPack.end(); ++it)
            {
                char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
                if ('*' != it->szSpNumEx[0])
                {
                    memcpy(szTmp, it->szSpNumEx, strlen((char*)it->szSpNumEx));
                }

				//确定成功和失败描述
				std::string strErrCode = (char*)it->msgcontent.szState;
				nStatus = _strnicmp((LPCSTR)it->msgcontent.szState, "DELIVRD", 7) == 0 ? 0 : 2;
				if (errcodemap.size() <= 0)
				{
					if (0 == nStatus)
					{
						strErrDesc = RPTSTATUSDESC_OK;
					}
					else
					{
						strErrDesc = RPTSTATUSDESC_ERR;
					}
				}
				else
				{
					DealECErrcode(errcodemap, strErrCode, strErrDesc);
				}


				strRptsTmp += cstr::format(JSON_RPT_RSP_BODY,
                                        CGlobal::TranMsgIdCharToI64(it->szMsgId),
                                        it->szCustId,
                                        it->nPknum,
                                        it->nPkTotal,
                                        it->msgcontent.szDestTerminalId,
                                        it->szDestId,
                                        szTmp,
                                        it->szSendTime,
                                        it->szTime,
                                        nStatus,
                                        strErrCode.c_str(),
                                        strErrDesc.c_str(),
                                        it->szExData
                                        );
            }

			int nRetLen = strRptsTmp.size();
			
            //去掉最后一个逗号
			if (nRetLen > 0 && ',' == strRptsTmp[nRetLen - 1])
            {
				strRptsTmp[nRetLen - 1] = ']';
            }

			if(WBS_URLENCODE_X2 == urlinfo.nPushCodeType)
			{
				//对mos整体内容进行URLENCODE编码
				strRptsTmp = CGlobal::UrlEncode(strRptsTmp.c_str());
			}
			
			//拼接
			nBodyLen += _snprintf(szBody + nBodyLen, sizeof(szBody)-nBodyLen, "%s", strRptsTmp.c_str());
			
            nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                                   "POST %s HTTP/1.1\r\n"
                                   "Host: %s:%d\r\n"
                                   "Connection: Close\r\n"
                                   "Content-Type: application/x-www-form-urlencoded\r\n"
                                   "Content-Length: %d\r\n\r\n"
                                   "%s",
                                   urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, nBodyLen, szBody);

            strRpt = szSendBuf;
        }
        else
        {
            //走json格式
            string strTime = CGlobal::date("mdHis");
            int nMd5SrcLen = _snprintf(szMd5Src, sizeof(szMd5Src), "%s%s%s%s", CAdapter::strupr((char*)urlinfo.szUsrId), PWDSTR, urlinfo.szPwd, strTime.c_str());
            MDStringMT(szMd5Src, szMd5);

            nBodyLen = _snprintf(szBody, sizeof(szBody), JSON_PUSHRPT_PRE, urlinfo.szUsrId, szMd5, strTime.c_str(), g_szRptCmd, vMoRptPack[0].sockInfo.nSockId);

            std::vector<PACK_DELIVER>::const_iterator it = vMoRptPack.begin();
            //组包体中变化的内容
            for (it; it != vMoRptPack.end(); ++it)
            {
                char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
                if ('*' != it->szSpNumEx[0])
                {
                    memcpy(szTmp, it->szSpNumEx, strlen((char*)it->szSpNumEx));
                }

				//确定成功和失败描述
				std::string strErrCode = (char*)it->msgcontent.szState;
				nStatus = _strnicmp((LPCSTR)it->msgcontent.szState, "DELIVRD", 7) == 0 ? 0 : 2;
				if (errcodemap.size() <= 0)
				{
					if (0 == nStatus)
					{
						strErrDesc = RPTSTATUSDESC_OK;
					}
					else
					{
						strErrDesc = RPTSTATUSDESC_ERR;
					}
				}
				else
				{
					DealECErrcode(errcodemap, strErrCode, strErrDesc);
				}

                int nRetLen = _snprintf(szBody + nBodyLen, sizeof(szBody)-nBodyLen, JSON_RPT_RSP_BODY,
                                        CGlobal::TranMsgIdCharToI64(it->szMsgId),
                                        it->szCustId,
                                        it->nPknum,
                                        it->nPkTotal,
                                        it->msgcontent.szDestTerminalId,
                                        it->szDestId,
                                        szTmp,
                                        it->szSendTime,
                                        it->szTime,
                                        nStatus,
                                        strErrCode.c_str(),
                                        strErrDesc.c_str(),
                                        it->szExData
                                        );
                nBodyLen += nRetLen;
            }

            //去掉最后一个逗号
            if (nBodyLen > 0 && ',' == szBody[nBodyLen - 1])
            {
                szBody[nBodyLen - 1] = ']';
                szBody[nBodyLen] = '}';
                nBodyLen++;
            }


            nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                                   "POST %s HTTP/1.1\r\n"
                                   "Host: %s:%d\r\n"
                                   "Connection: Close\r\n"
                                   "Content-Type: text/json\r\n"
                                   "Content-Length: %d\r\n\r\n"
                                   "%s",
                                   urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, nBodyLen, szBody);

            strRpt = szSendBuf;
        }
    }
    catch (...)
    {
        nOutMsgLen = -1;
    }

    return nOutMsgLen;
}

int CParseToolImpl::MakeMoRequestV5_GOME(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, string &strMo)
{
    int nRet = 0;
    try
    {
        std::string strUser;
        std::string strPwd;

        strUser = m_strMOName;
        strPwd = m_strMOPwd;

        //组推送mo请求包，http post
        char szMd5[32 + 1] = { 0 };
        char szMd5Src[MAX_PWD_LEN + 128 + 1] = { 0 };
        char szMd5Sign[MAX_PWD_LEN + 128 + 1] = { 0 };
        char szMd5S[32 + 1] = { 0 };
        char szBody[MAX_PUSHMO_LEN - MAX_HTTP_HEAD - 10] = { 0 };
        char szSendBuf[MAX_PUSHMO_LEN] = { 0 };
        int nBodyLen = 0;
        int nOutMsgLen = 0;
        unsigned char szUrlMsg[4096 + 1] = { 0 };

        //pushtime
        std::string strTime = CGlobal::date("YmdHis");

        //pwd
        int nMd5SrcLen = _snprintf(szMd5Src, sizeof(szMd5Src), "%s%s", strUser.c_str(), strPwd.c_str());
        MDStringMT(szMd5Src, szMd5);
        //cstr::to_lower(szMd5);

        //sign
        int nMd5SignLen = _snprintf(szMd5Sign, sizeof(szMd5Sign), "%s%s%s", strUser.c_str(), strTime.c_str(), szMd5);
        MDStringMT(szMd5Sign, szMd5S);

        nBodyLen = _snprintf(szBody, sizeof(szBody), "name=%s&pwd=%s&pushTime=%s&sign=%s&args=[", strUser.c_str(), szMd5, strTime.c_str(), szMd5S);

        std::vector<PACK_DELIVER>::const_iterator it = vMoRptPack.begin();
        //组包体中变化的内容
        for (it; it != vMoRptPack.end(); ++it)
        {
            //采用urlencode加密方式
            //memset(szUrlMsg, 0, sizeof(szUrlMsg));
            //ToUTF8((char*)it->szMsgContent, (char*)szUrlMsg, sizeof(szUrlMsg));
            char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
            if ('*' != it->szSpNumEx[0])
            {
                memcpy(szTmp, it->szSpNumEx, strlen((char*)it->szSpNumEx));
            }

            int nRetLen = _snprintf(szBody + nBodyLen, sizeof(szBody)-nBodyLen, JSON_MO_RSP_BODY_GOME,
                                    CAdapter::strupr((char*)urlinfo.szUsrId),
                                    CGlobal::TranMsgIdCharToI64(it->szMsgId),
                                    it->szSrcTerminalId,
                                    it->szDestId,
                                    szTmp,
                                    it->szTime,
									/*szUrlMsg*/it->szMsgContent
                                    );
            nBodyLen += nRetLen;
        }

        //去掉最后一个逗号
        if (nBodyLen > 0 && ',' == szBody[nBodyLen - 1])
        {
            szBody[nBodyLen - 1] = ']';
        }

        nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                               "POST %s HTTP/1.1\r\n"
                               "Host: %s:%d\r\n"
                               "Connection: Close\r\n"
                               "Content-Type: application/x-www-form-urlencoded\r\n"
                               "Content-Length: %d\r\n\r\n"
                               "%s",
                               urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, nBodyLen, szBody);

        strMo = szSendBuf;

        nRet = nOutMsgLen;
    }
    catch (...)
    {
        nRet = -1;
    }

    return nRet;
}

int CParseToolImpl::MakeRptRequestV5_GOME(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, bool bBus, string &strRpt)
{
    if (vMoRptPack.empty())
    {
        return 0;
    }

    char szMd5[32 + 1] = { 0 };
    char szMd5Src[MAX_PWD_LEN + 128 + 1] = { 0 };

    char szMd5S[32 + 1] = { 0 };
    char szMd5Sign[MAX_PWD_LEN + 128 + 1] = { 0 };

    char szBody[MAX_PUSHRPT_LEN] = { 0 };
    char szSendBuf[MAX_PUSHRPT_LEN] = { 0 };
    int nBodyLen = 0;
    int nOutMsgLen = 0;
    string strErrDesc("");
    int nStatus = -1;

    try
    {
        std::string strUser;
        std::string strPwd;
        if (bBus)
        {
            strUser = m_strRPTNameBus;
            strPwd = m_strRPTPwdBus;
        }
        else
        {
            strUser = m_strRPTName;
            strPwd = m_strRPTPwd;
        }

        //PUSHTIME
        string strTime = CGlobal::date("YmdHis");

        //PWD
        int nMd5SrcLen = _snprintf(szMd5Src, sizeof(szMd5Src), "%s%s", strUser.c_str(), strPwd.c_str());
        MDStringMT(szMd5Src, szMd5);

        //sign
        int nMd5SignLen = _snprintf(szMd5Sign, sizeof(szMd5Sign), "%s%s%s", strUser.c_str(), strTime.c_str(), szMd5);
        MDStringMT(szMd5Sign, szMd5S);

        nBodyLen = _snprintf(szBody, sizeof(szBody), "name=%s&pwd=%s&pushTime=%s&sign=%s&args=[", strUser.c_str(), szMd5, strTime.c_str(), szMd5S);

        std::vector<PACK_DELIVER>::const_iterator it = vMoRptPack.begin();
        //组包体中变化的内容
        for (it; it != vMoRptPack.end(); ++it)
        {
            char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
            if ('*' != it->szSpNumEx[0])
            {
                memcpy(szTmp, it->szSpNumEx, strlen((char*)it->szSpNumEx));
            }

            //确定成功和失败描述
            nStatus = memcmp(it->msgcontent.szState, "DELIVRD", 7) == 0 ? 0 : 2;
            if (0 == nStatus)
            {
                strErrDesc = RPTSTATUSDESC_OK;
            }
            else
            {
                strErrDesc = RPTSTATUSDESC_ERR;
            }

            int nRetLen = _snprintf(szBody + nBodyLen, sizeof(szBody)-nBodyLen, JSON_RPT_RSP_BODY,
                                    CGlobal::TranMsgIdCharToI64(it->szMsgId),
                                    it->szCustId,
                                    it->nPknum,
                                    it->nPkTotal,
                                    it->msgcontent.szDestTerminalId,
                                    it->szDestId,
                                    szTmp,
                                    it->szSendTime,
                                    it->szTime,
                                    nStatus,
                                    it->msgcontent.szState,
                                    strErrDesc.c_str(),
                                    it->szExData
                                    );
            nBodyLen += nRetLen;
        }

        //去掉最后一个逗号
        if (nBodyLen > 0 && ',' == szBody[nBodyLen - 1])
        {
            szBody[nBodyLen - 1] = ']';
        }


        nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
                               "POST %s HTTP/1.1\r\n"
                               "Host: %s:%d\r\n"
                               "Connection: Close\r\n"
                               "Content-Type: application/x-www-form-urlencoded\r\n"
                               "Content-Length: %d\r\n\r\n"
                               "%s",
                               urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, nBodyLen, szBody);

        strRpt = szSendBuf;
    }
    catch (...)
    {
        nOutMsgLen = -1;
    }

    return nOutMsgLen;
}




void CParseToolImpl::MakePushMoXMLData(const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, const HSBC_URLINFO& urlinfo, string &strOut)
{
    if (FileRptPackArr.empty())
    {
        return;
    }
    //推送客户配置的，没有配置的按照默认的标签推
    int nSize = FileRptPackArr.size();
    std::vector<PACK_DELIVER>::const_iterator itFileMo = FileRptPackArr.begin();
    rapidxml::xml_document<> doc;

    FILEDMAP::const_iterator it;
    FILEDMAP::const_iterator itRpts;
    FILEDMAP::const_iterator itRpt;

    //存标签
    string strTagMoReq("");
    string strTagMos("");
    string strTagMo("");


    itRpt = allFiled.filedmap.find(g_szMO);
    if (itRpt == allFiled.filedmap.end())
    {
        strOut = "";
        return;
    }

    itRpts = allFiled.filedmap.find(g_szMOS);
    if (itRpts == allFiled.filedmap.end())
    {
        strOut = "";
        return;
    }

    //确定标签
    if ((it = allFiled.filedmap.find(g_szMoReq)) != allFiled.filedmap.end())
    {
        strTagMoReq = it->second.szCArgName;
    }
    else
    {
        strTagMoReq = g_szMoReq;
    }

    //Mos
    if ((it = allFiled.filedmap.find(g_szMOS)) != allFiled.filedmap.end())
    {
        strTagMos = it->second.szCArgName;
    }
    else
    {
        strTagMos = g_szMOS;
    }

    //mo
    if ((it = allFiled.filedmap.find(g_szMO)) != allFiled.filedmap.end())
    {
        strTagMo = it->second.szCArgName;
    }
    else
    {
        strTagMo = g_szMO;
    }

    //时间戳
    string strTime = CGlobal::date("mdHis");



	char szMoUserId[MAX_MO_USERID_LEN + 1] = { 0 };
	char szMoMd5[MAX_PWD_LEN + 1] = { 0 };

	PushHelper::GetMoReqUserIdAndPwd(urlinfo, strTime, szMoUserId, szMoMd5);
    // 
    //xmlHead
    rapidxml::xml_node<>* xmlHead = doc.allocate_node(rapidxml::node_pi, doc.allocate_string("xml version=\"1.0\" encoding=\"utf-8\""));
    doc.append_node(xmlHead);

    //moreq
    rapidxml::xml_node<>* root = doc.allocate_node(rapidxml::node_element, strTagMoReq.c_str(), NULL);

    //userid
    if ((it = allFiled.filedmap.find(g_szUesrId)) != allFiled.filedmap.end())
    {
		rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, szMoUserId);
        root->append_node(node1);
    }

    //pwd
    if ((it = allFiled.filedmap.find(g_szPwd)) != allFiled.filedmap.end())
    {
		rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, szMoMd5);
        root->append_node(node1);
    }

    //timestamp
    if ((it = allFiled.filedmap.find(g_szTimeStamp)) != allFiled.filedmap.end())
    {
        rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, strTime.c_str());
        root->append_node(node1);
    }

    //cmd
    if ((it = allFiled.filedmap.find(g_szCmd)) != allFiled.filedmap.end())
    {
		rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, g_szMoCmd);
        root->append_node(node1);
    }

    //seq
    if ((it = allFiled.filedmap.find(g_szSeqId)) != allFiled.filedmap.end())
    {
        char szTmp[20] = { 0 };
        //CAdapter::itoa(FileRptPackArr[0].sockInfo.nSockId, szTmp, 10);
        _snprintf(szTmp, sizeof(szTmp), "%d", FileRptPackArr[0].sockInfo.nSockId);
        rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, szTmp);
        root->append_node(node1);
    }

	//vector
	//zhubo 2017-12-12
	MakeFixedXmlValueToParentNode(allFiled,doc,root);
	//zhubo 2017-12-12

    rapidxml::xml_node<>* mos = doc.allocate_node(rapidxml::node_element, strTagMos.c_str(), NULL);

    for (; (itFileMo != FileRptPackArr.end()); ++itFileMo)
    {
        const PACK_DELIVER &FileRptPack = *itFileMo;

        rapidxml::xml_node<>* mo = doc.allocate_node(rapidxml::node_element, strTagMo.c_str(), NULL);
		rapidxml::xml_node<>* node3;
		if (PT_PUSH_IS_IN_FIELDMAP(g_szMsgId))
		{
			char szTmp[MSGID_TMP_LEN] = { 0 };
			__int64 nTmp = m_global.TranMsgIdCharToI64(FileRptPack.szMsgId);
			_snprintf(szTmp, sizeof(szTmp), "%lld", nTmp);
			node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, doc.allocate_string(szTmp));
			mo->append_node(node3);
		}
        

		if (PT_PUSH_IS_IN_FIELDMAP(g_szMobile))
		{
			node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileRptPack.szSrcTerminalId);
			mo->append_node(node3);
		}
        

		if (PT_PUSH_IS_IN_FIELDMAP(g_szSpNo))
		{
			node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileRptPack.szDestId);
			mo->append_node(node3);
		}
        

        //char szTmpEx[MAX_CHANNEL_LEN + 1] = { 0 };
		if (PT_PUSH_IS_IN_FIELDMAP(g_szExNo))
		{
			if ('*' == FileRptPack.szSpNumEx[0])
			{
				node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, "");
				mo->append_node(node3);
			}
			else
			{
				node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileRptPack.szSpNumEx);
				mo->append_node(node3);
			}
		}
        

		if (PT_PUSH_IS_IN_FIELDMAP(g_szRecvTime))
		{
			node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileRptPack.szTime);
			mo->append_node(node3);
		}
        

		//
		//CGlobal::UrlEncode((char*)itFileMo->szMsgContent, szMsgTmp, sizeof(szMsgTmp), false);
		if (PT_PUSH_IS_IN_FIELDMAP(g_szMsg))
		{
			std::string strMsgTmp = CGlobal::UrlEncode((char*)itFileMo->szMsgContent);

			node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, doc.allocate_string(strMsgTmp.c_str()));
			mo->append_node(node3);
		}

        //zhubo 2017-12-15
		MakeFixedXmlValueToParentNode(allFiled, doc, mo);
		//zhubo 2017-12-15

        mos->append_node(mo);
    }
    root->append_node(mos);
    doc.append_node(root);

    //最后一个包
    rapidxml::print(std::back_inserter(strOut), doc, 0);
}


void CParseToolImpl::MakePushMoXMLData(const ALLFILED &allFiled, const PACK_DELIVER &FileMoPack, const HSBC_URLINFO& urlinfo, string &strOut)
{
    //推送客户配置的，没有配置的按照默认的标签推
    rapidxml::xml_document<> doc;

    FILEDMAP::const_iterator it;

    //存标签
    string strTagMoReq("");


    //确定标签
    if ((it = allFiled.filedmap.find(g_szMoReq)) != allFiled.filedmap.end())
    {
        strTagMoReq = it->second.szCArgName;
    }
    else
    {
        strTagMoReq = g_szMoReq;
    }

    char szMd5[32 + 1] = { 0 };
    char szMd5Src[MAX_PWD_LEN + 128 + 1] = { 0 };

    //时间戳
    string strTime = CGlobal::date("mdHis");

	char szMoUserId[MAX_MO_USERID_LEN+ 1] = { 0 };
	char szMoPwd[MAX_PWD_LEN + 1] = { 0 };

	//获取推送账号和推送密码
	PushHelper::GetMoReqUserIdAndPwd(urlinfo, strTime, szMoUserId, szMoPwd);

    //xmlHead
    rapidxml::xml_node<>* xmlHead = doc.allocate_node(rapidxml::node_pi, doc.allocate_string("xml version=\"1.0\" encoding=\"utf-8\""));
    doc.append_node(xmlHead);

    //moreq
    rapidxml::xml_node<>* root = doc.allocate_node(rapidxml::node_element, strTagMoReq.c_str(), NULL);

    //userid
    if ((it = allFiled.filedmap.find(g_szUesrId)) != allFiled.filedmap.end())
    {
        rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, szMoUserId);
        root->append_node(node1);
    }

    //pwd
    if ((it = allFiled.filedmap.find(g_szPwd)) != allFiled.filedmap.end())
    {
        rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, szMoPwd);
        root->append_node(node1);
    }

    //timestamp
    if ((it = allFiled.filedmap.find(g_szTimeStamp)) != allFiled.filedmap.end())
    {
        rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, strTime.c_str());
        root->append_node(node1);
    }

    //cmd
    if ((it = allFiled.filedmap.find(g_szCmd)) != allFiled.filedmap.end())
    {
        rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, g_szMoCmd);
        root->append_node(node1);
    }

    //seq
    if ((it = allFiled.filedmap.find(g_szSeqId)) != allFiled.filedmap.end())
    {
        char szTmp[20] = { 0 };
        _snprintf(szTmp, sizeof(szTmp), "%d", FileMoPack.sockInfo.nSockId);
        rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, szTmp);
        root->append_node(node1);
    }

	rapidxml::xml_node<>* node1;
	if (PT_PUSH_IS_IN_FIELDMAP(g_szMsgId))
	{
		char szTmp[MSGID_TMP_LEN] = { 0 };
		__int64 nTmp = m_global.TranMsgIdCharToI64(FileMoPack.szMsgId);
		_snprintf(szTmp, sizeof(szTmp), "%lld", nTmp);
		node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, doc.allocate_string(szTmp));
		root->append_node(node1);
	}
    
	if (PT_PUSH_IS_IN_FIELDMAP(g_szMobile))
	{
		node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileMoPack.szSrcTerminalId);
		root->append_node(node1);
	}
    
	if (PT_PUSH_IS_IN_FIELDMAP(g_szSpNo))
	{
		node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileMoPack.szDestId);
		root->append_node(node1);
	}
    
	if (PT_PUSH_IS_IN_FIELDMAP(g_szExNo))
	{
		if ('*' == FileMoPack.szSpNumEx[0])
		{
			node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, "");
			root->append_node(node1);
		}
		else
		{
			node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileMoPack.szSpNumEx);
			root->append_node(node1);
		}
	}
    
	if (PT_PUSH_IS_IN_FIELDMAP(g_szRecvTime))
	{
		node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileMoPack.szTime);
		root->append_node(node1);
	}
    
	if (PT_PUSH_IS_IN_FIELDMAP(g_szMsg))
	{
		//
		//CGlobal::UrlEncode((char*)FileMoPack.szMsgContent, szMsgTmp, sizeof(szMsgTmp), false);
		std::string strMsgTmp = CGlobal::UrlEncode((char*)FileMoPack.szMsgContent);

		node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, doc.allocate_string(strMsgTmp.c_str()));
		root->append_node(node1);
	}
  
    //vector
    int nVecSize = allFiled.vecFiled.size();
    for (int i = 0; i < nVecSize; ++i)
    {
        rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, allFiled.vecFiled[i].szCArgName, allFiled.vecFiled[i].szCArgValue);
        if (0 == strlen(allFiled.vecFiled[i].szBelong))
        {
            //无子节点，无父节点
            doc.append_node(node);
        }
        else
        {
            root->append_node(node);
        }
    }


    doc.append_node(root);

    //最后一个包
    rapidxml::print(std::back_inserter(strOut), doc, 0);
}



//前半部分url，后面mos部分是json
void CParseToolImpl::MakePushMoURLData(const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, const HSBC_URLINFO& urlinfo, string &strOut)
{
    //组推送mo请求包，http post
    if (FileRptPackArr.empty())
    {
        return;
    }
	char szMoUserId[MAX_MO_USERID_LEN + 1] = { 0 };
	char szMoMd5[MAX_PWD_LEN + 1] = { 0 };
    //char szMd5Src[MAX_PWD_LEN + 128 + 1] = { 0 };
    //char szBody[MAX_PUSHMO_LEN - MAX_HTTP_HEAD - 10] = { 0 };
    int nPublicLen = 0;
    int nBodyLen = 0;
    FILEDMAP::const_iterator it;
	string strFixFiels("");
	string strBody("");
	string strTmp("");
	string strMos("");

	int nRetCode = ERRCODE_PUSH_USE_ONLY;

    try
    {
		
    	//时间戳

		string strTime = CGlobal::date("mdHis");
		PushHelper::GetMoReqUserIdAndPwd(urlinfo, strTime, szMoUserId, szMoMd5);
		//================用户名和密码
        //确定标签
		MakeUrlItemValue(allFiled, g_szUesrId, szMoUserId, strBody, true);

		MakeUrlItemValue(allFiled, g_szPwd, szMoMd5, strBody);
		//================用户名和密码


		//是否有签名
		bool bSign = false;
		if (PT_PUSH_IS_IN_FIELDMAP(g_szSign))
		{
        	//查看签名的格式  (兼容国美)
        	//1:name+pushTime+pwd
        	if(1 == urlinfo.nSignType)
        	{
				string strPushTime("");
				string strSign("");
				PushHelper::GetGomePushTime(strPushTime);
				MakeUrlItemValue(allFiled, g_szPushTime, strPushTime.c_str(), strBody);
       			
				PushHelper::MakeGomeSign(szMoUserId, strPushTime.c_str(), szMoMd5, strSign);

				MakeUrlItemValue(allFiled, g_szSign, strSign.c_str(), strBody);

				bSign = true;
			}
		}
		
		MakeUrlItemValue(allFiled, g_szTimeStamp, strTime.c_str(), strBody);

		MakeUrlItemValue(allFiled, g_szCmd, "MO_REQ", strBody);

		strTmp = cstr::format("%d", FileRptPackArr[0].sockInfo.nSockId);
		MakeUrlItemValue(allFiled, g_szSeqId, strTmp.c_str(), strBody);

		//zhubo 2017-12-12
		GetUrlRequestFixedValue(allFiled,strFixFiels,false);
		strBody += strFixFiels;
        
        //mos的json报文
        MakeMoJsonNoRootData(nRetCode, allFiled, FileRptPackArr, strMos, urlinfo, bSign);

        //合并
		MakeUrlItemValue(allFiled, g_szMOS, strMos.c_str(), strBody);

		strOut = strBody;
    }
    catch (...)
    {
        nBodyLen = -1;
    }

    return;
}


void CParseToolImpl::MakePushMoURLData(const ALLFILED &allFiled, const PACK_DELIVER &FileMoPack, const HSBC_URLINFO& urlinfo, string &strOut)
{
    //组推送mo请求包，http post

	std::string strBody = "";

    try
    {
    	//时间戳
		string strTime = CGlobal::date("mdHis");

		char szMoUserId[MAX_MO_USERID_LEN+ 1] = { 0 };
		char szMoPwd[MAX_PWD_LEN + 1] = { 0 };

		//获取推送账号和推送密码
		PushHelper::GetMoReqUserIdAndPwd(urlinfo, strTime, szMoUserId, szMoPwd);

        //确定标签
        //userid
        MakeUrlItemValue(allFiled, g_szUesrId, szMoUserId, strBody, true);

		//pwd
		MakeUrlItemValue(allFiled, g_szPwd, szMoPwd, strBody);

		//timestamp
		MakeUrlItemValue(allFiled, g_szTimeStamp, strTime.c_str(), strBody);

		//cmd
		MakeUrlItemValue(allFiled, g_szCmd, "MO_REQ", strBody);
		
		//seqid
		std::string strTmp = cstr::format("%d", FileMoPack.sockInfo.nSockId);
		MakeUrlItemValue(allFiled, g_szSeqId, strTmp.c_str(), strBody);

		//固定字段值
		std::string strFixFiels = "";
		GetUrlRequestFixedValue(allFiled, strFixFiels);
		strBody += strFixFiels;

		//msgid
		strTmp = cstr::format("%lld",m_global.TranMsgIdCharToI64(FileMoPack.szMsgId));
		MakeUrlItemValue(allFiled, g_szMsgId, strTmp.c_str(), strBody);
		
		//mobile
		MakeUrlItemValue(allFiled, g_szMobile, (char*)FileMoPack.szSrcTerminalId, strBody);

		//spno
		MakeUrlItemValue(allFiled, g_szSpNo, (char*)FileMoPack.szDestId, strBody);

		//exno
		if ('*' == FileMoPack.szSpNumEx[0])
        {
            memset((char*)FileMoPack.szSpNumEx, 0, sizeof(FileMoPack.szSpNumEx));
        }
		MakeUrlItemValue(allFiled, g_szExNo, (char*)FileMoPack.szSpNumEx, strBody);

		char szReceiveTime[25] = { 0 };
		if (2 == urlinfo.nPushtimefmt)
		{
			int nYear_MsgId = 0, nMon_MsgId = 0, nDay_MsgId = 0, nHour_MsgId = 0, nMin_MsgId = 0, nSec_MsgId = 0;
			sscanf(FileMoPack.szTime, "%04d-%02d-%02d %02d:%02d:%02d",
				&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
			sprintf(szReceiveTime, "%04d%02d%02d%02d%02d%02d",
				nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);
		}
		else
		{
			memcpy(szReceiveTime, FileMoPack.szTime, min(strlen(FileMoPack.szTime), sizeof(szReceiveTime)-1));
		}
		//rtime
		MakeUrlItemValue(allFiled, g_szRecvTime, szReceiveTime, strBody);

		//content
        std::string strMsgTmp = CGlobal::UrlEncode((char*)FileMoPack.szMsgContent);
		//最后一个，最后一个参数必须用false
		MakeUrlItemValue(allFiled, g_szMsg, strMsgTmp.c_str(), strBody);

        strOut = strBody;
    }
    catch (...)
    {
    }

    return;
}


void CParseToolImpl::MakePushRptXMLData(const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, const HSBC_URLINFO& urlinfo, string &strOut)
{
    std::vector<PACK_DELIVER>::const_iterator itFileRpt = FileRptPackArr.begin();
    int nExcpLine = __LINE__;        //记录当前行

    if (0 == FileRptPackArr.size())
    {
        return;
    }

    string strStatus = "";
    string strErrDesc("");
    rapidxml::xml_document<> doc;
    FILEDMAP::const_iterator it;
    FILEDMAP::const_iterator itRpts;
    FILEDMAP::const_iterator itRpt;
    //存标签
    string strTagRptReq("");
    string strTagRpts("");
    string strTagRpt("");

    //确定标签
    //Mos
    if ((it = allFiled.filedmap.find(g_szRptReq)) != allFiled.filedmap.end())
    {
        strTagRptReq = it->second.szCArgName;
    }
    else
    {
        strTagRptReq = g_szRptReq;
    }

    if ((it = allFiled.filedmap.find(g_szRpts)) != allFiled.filedmap.end())
    {
        strTagRpts = it->second.szCArgName;
    }
    else
    {
        strTagRpts = g_szRpts;
    }

    if ((it = allFiled.filedmap.find(g_szRpt)) != allFiled.filedmap.end())
    {
        strTagRpt = it->second.szCArgName;
    }
    else
    {
        strTagRpt = g_szRpt;
    }

    //时间戳
    string strTime = CGlobal::date("mdHis");

	char szRptUserId[MAX_RPT_USERID_LEN + 1] = { 0 };
	char szRptPwd[MAX_PWD_LEN + 1] = { 0 };

	PushHelper::GetRptReqUserIdAndPwd(urlinfo, strTime, szRptUserId, szRptPwd);

    // 
    //xmlHead
    rapidxml::xml_node<>* xmlHead = doc.allocate_node(rapidxml::node_pi, doc.allocate_string("xml version=\"1.0\" encoding=\"utf-8\""));
    doc.append_node(xmlHead);

    //rptreq
    rapidxml::xml_node<>* root = doc.allocate_node(rapidxml::node_element, strTagRptReq.c_str(), NULL);

    //seq
    if ((it = allFiled.filedmap.find(g_szSeqId)) != allFiled.filedmap.end())
    {
        char szTmpEx[30] = { 0 };
        //itoa(FileRptPackArr[0].sockInfo.nSockId, szTmp, 10);
        _snprintf(szTmpEx, sizeof(szTmpEx), "%d", FileRptPackArr[0].sockInfo.nSockId);
        rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, szTmpEx);
        root->append_node(node1);
    }

    //userid
    if ((it = allFiled.filedmap.find(g_szUesrId)) != allFiled.filedmap.end())
    {
		rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, szRptUserId);
        root->append_node(node1);
    }

    //pwd
    if ((it = allFiled.filedmap.find(g_szPwd)) != allFiled.filedmap.end())
    {
		rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, szRptPwd);
        root->append_node(node1);
    }

    //timestamp
    if ((it = allFiled.filedmap.find(g_szTimeStamp)) != allFiled.filedmap.end())
    {
        rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, strTime.c_str());
        root->append_node(node1);
    }

    //cmd
    if ((it = allFiled.filedmap.find(g_szCmd)) != allFiled.filedmap.end())
    {
        rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, g_szRptCmd);
        root->append_node(node1);
    }

	//vector
	//zhubo 2017-12-12
	MakeFixedXmlValueToParentNode(allFiled,doc,root);
	//zhubo 2017-12-12

    rapidxml::xml_node<>* rpts = doc.allocate_node(rapidxml::node_element, strTagRpts.c_str(), NULL);

    for (; (itFileRpt != FileRptPackArr.end()); ++itFileRpt)
    {
        const PACK_DELIVER &FileRptPack = *itFileRpt;

        rapidxml::xml_node<>* rpt = doc.allocate_node(rapidxml::node_element, strTagRpt.c_str(), NULL);
        rapidxml::xml_node<>* node3;

        //确定成功和失败描述
        strStatus = memcmp(FileRptPack.msgcontent.szState, "DELIVRD", 7) == 0 ? "0" : "2";
        if ("0" == strStatus)
        {
            strErrDesc = RPTSTATUSDESC_OK;
        }
        else
        {
            strErrDesc = RPTSTATUSDESC_ERR;
        }

		if (PT_PUSH_IS_IN_FIELDMAP(g_szMsgId))
        {
            char szTmpMsgId[MSGID_TMP_LEN] = { 0 };
            __int64 nTmp = m_global.TranMsgIdCharToI64(FileRptPack.szMsgId);
            _snprintf(szTmpMsgId, sizeof(szTmpMsgId), "%lld", nTmp);
			rapidxml::xml_node<>* node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, doc.allocate_string(szTmpMsgId));
            rpt->append_node(node3);
        }

        //custid
        //if (0 != strlen(FileRptPack.szCustId))
		if (PT_PUSH_IS_IN_FIELDMAP(g_szCustID))
        {
			node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileRptPack.szCustId);
            rpt->append_node(node3);
        }

		if (PT_PUSH_IS_IN_FIELDMAP(g_szPkNum))
        {
            //pkno
            char szTmp[30] = { 0 };
            _snprintf(szTmp, sizeof(szTmp), "%d", FileRptPack.nPknum);
			node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, doc.allocate_string(szTmp));
            rpt->append_node(node3);
        }

		if (PT_PUSH_IS_IN_FIELDMAP(g_szPkTotal))
        {
            char szTmp[30] = { 0 };
            _snprintf(szTmp, sizeof(szTmp), "%d", FileRptPack.nPkTotal);
			node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, doc.allocate_string(szTmp));
            rpt->append_node(node3);
        }

		
        //mob
		if (PT_PUSH_IS_IN_FIELDMAP(g_szMobile))
		{
			node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileRptPack.msgcontent.szDestTerminalId);
			rpt->append_node(node3);
		}
        

        //
        //string strDesId((char*)FileRptPack.szDestId);
		
        //if (0 != strDesId.length())
		if (PT_PUSH_IS_IN_FIELDMAP(g_szSpNo))
        {
			node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileRptPack.szDestId);
            rpt->append_node(node3);
        }
		if (PT_PUSH_IS_IN_FIELDMAP(g_szExNo))
        {
            if ('*' == FileRptPack.szSpNumEx[0])
            {
				node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, "");
                rpt->append_node(node3);
            }
            else
            {
				node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileRptPack.szSpNumEx);
                rpt->append_node(node3);
            }
        }

        //stime g_szSendTime
		if (PT_PUSH_IS_IN_FIELDMAP(g_szSendTime))
		{
			node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileRptPack.szSendTime);
			rpt->append_node(node3);
		}
        

        //rtime g_szRecvTime
		if (PT_PUSH_IS_IN_FIELDMAP(g_szRecvTime))
		{
			node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileRptPack.szTime);
			rpt->append_node(node3);
		}
        

        //status g_szStatus
		if (PT_PUSH_IS_IN_FIELDMAP(g_szStatus))
		{
			node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, strStatus.c_str());
			rpt->append_node(node3);
		}
        

        //errcode g_szErrCode
		if (PT_PUSH_IS_IN_FIELDMAP(g_szErrCode))
		{
			node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileRptPack.msgcontent.szState);
			rpt->append_node(node3);
		}
        

        //errcodedesc 
		if (PT_PUSH_IS_IN_FIELDMAP(g_szErrCodeDesc))
		{
			if(WBS_CARGTYPE_UPPER_STRING == it->second.nCargType)
			{
				//转换为大写
				strErrDesc = cstr::to_upper_s(strErrDesc.c_str());
				node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, doc.allocate_string(strErrDesc.c_str()));
				rpt->append_node(node3);
			}
			else
			{	
				node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, strErrDesc.c_str());
				rpt->append_node(node3);
			}

		}

        //exdata
		if (PT_PUSH_IS_IN_FIELDMAP(g_szExData))
        //if (0 != strlen(FileRptPack.szExData))
        {
			node3 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, FileRptPack.szExData);
            rpt->append_node(node3);
        }

        //vector
		MakeFixedXmlValueToParentNode(allFiled, doc, rpt);

        rpts->append_node(rpt);
    }
    root->append_node(rpts);
    doc.append_node(root);
    //最后一个包
    rapidxml::print(std::back_inserter(strOut), doc, 0);
}
void CParseToolImpl::MakePushRptXMLData(const ALLFILED &allFiled, const PACK_DELIVER &FileRptPack, const HSBC_URLINFO& urlinfo, string &strOut)
{

    int nExcpLine = __LINE__;        //记录当前行

    string strStatus = "";
    string strErrDesc("");
    rapidxml::xml_document<> doc;
    FILEDMAP::const_iterator it;

    //存标签
    string strTagRptReq("");


    //确定标签
    //rptreq
    if ((it = allFiled.filedmap.find(g_szRptReq)) != allFiled.filedmap.end())
    {
        strTagRptReq = it->second.szCArgName;
    }
    else
    {
        strTagRptReq = g_szRptReq;
    }


    char szMd5[32 + 1] = { 0 };
    char szMd5Src[MAX_PWD_LEN + 128 + 1] = { 0 };
	
	char szRptUserId[MAX_RPT_USERID_LEN+ 1] = { 0 };
	char szRptPwd[MAX_PWD_LEN + 1] = { 0 };
	
	//时间戳
	string strTime = CGlobal::date("mdHis");
	
	//获取推送账号和推送密码
	PushHelper::GetMoReqUserIdAndPwd(urlinfo, strTime, szRptUserId, szRptPwd);

    // 
    //xmlHead
    rapidxml::xml_node<>* xmlHead = doc.allocate_node(rapidxml::node_pi, doc.allocate_string("xml version=\"1.0\" encoding=\"utf-8\""));
    doc.append_node(xmlHead);

    //rptreq
    rapidxml::xml_node<>* root = doc.allocate_node(rapidxml::node_element, strTagRptReq.c_str(), NULL);

    //seq
    if ((it = allFiled.filedmap.find(g_szSeqId)) != allFiled.filedmap.end())
    {
        char szTmpEx[30] = { 0 };
        _snprintf(szTmpEx, sizeof(szTmpEx), "%d", FileRptPack.sockInfo.nSockId);
        rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, szTmpEx);
        root->append_node(node1);
    }

    //userid
    if ((it = allFiled.filedmap.find(g_szUesrId)) != allFiled.filedmap.end())
    {
        rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, szRptUserId);
        root->append_node(node1);
    }

    //pwd
    if ((it = allFiled.filedmap.find(g_szPwd)) != allFiled.filedmap.end())
    {
        rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, szRptPwd);
        root->append_node(node1);
    }

    //timestamp
    if ((it = allFiled.filedmap.find(g_szTimeStamp)) != allFiled.filedmap.end())
    {
        rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, strTime.c_str());
        root->append_node(node1);
    }

    //cmd
    if ((it = allFiled.filedmap.find(g_szCmd)) != allFiled.filedmap.end())
    {
        rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, g_szRptCmd);
        root->append_node(node1);
    }

    //确定成功和失败描述
    strStatus = memcmp(FileRptPack.msgcontent.szState, "DELIVRD", 7) == 0 ? "0" : "2";
    if ("0" == strStatus)
    {
        strErrDesc = RPTSTATUSDESC_OK;
    }
    else
    {
        strErrDesc = RPTSTATUSDESC_ERR;
    }
	if (PT_PUSH_IS_IN_FIELDMAP(g_szMsgId))
    {
        char szTmpMsgId[MSGID_TMP_LEN] = { 0 };
        __int64 nTmp = m_global.TranMsgIdCharToI64(FileRptPack.szMsgId);
        _snprintf(szTmpMsgId, sizeof(szTmpMsgId), "%lld", nTmp);
		rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, doc.allocate_string(szTmpMsgId));
        root->append_node(node1);
    }

    //custid
	if (PT_PUSH_IS_IN_FIELDMAP(g_szCustID))
    {
		rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileRptPack.szCustId);
        root->append_node(node1);
    }
	if (PT_PUSH_IS_IN_FIELDMAP(g_szPkNum))
    {
        //pkno
        char szTmp[30] = { 0 };
        _snprintf(szTmp, sizeof(szTmp), "%d", FileRptPack.nPknum);
		rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, doc.allocate_string(szTmp));
        root->append_node(node1);
    }
	if (PT_PUSH_IS_IN_FIELDMAP(g_szPkTotal))
    {
        //pktotal
        char szTmp[30] = { 0 };
        _snprintf(szTmp, sizeof(szTmp), "%d", FileRptPack.nPkTotal);
		rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, doc.allocate_string(szTmp));
        root->append_node(node1);
    }
	if (PT_PUSH_IS_IN_FIELDMAP(g_szMobile))
    {
        //mob
		rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileRptPack.msgcontent.szDestTerminalId);
        root->append_node(node1);
    }
	if (PT_PUSH_IS_IN_FIELDMAP(g_szSpNo))
    {
		rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileRptPack.szDestId);
        root->append_node(node1);
    }
	if (PT_PUSH_IS_IN_FIELDMAP(g_szExNo))
    {
        if ('*' == FileRptPack.szSpNumEx[0])
        {
			rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, "");
            root->append_node(node1);
        }
        else
        {
			rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileRptPack.szSpNumEx);
            root->append_node(node1);
        }
    }
	if (PT_PUSH_IS_IN_FIELDMAP(g_szSendTime))
	{
		//stime
		rapidxml::xml_node<>* node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileRptPack.szSendTime);
		root->append_node(node1);
	}
	rapidxml::xml_node<>* node1;
	if (PT_PUSH_IS_IN_FIELDMAP(g_szRecvTime))
	{
		//rtime
		node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileRptPack.szTime);
		root->append_node(node1);
	}
    
	if (PT_PUSH_IS_IN_FIELDMAP(g_szStatus))
	{
		//status
		node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, strStatus.c_str());
		root->append_node(node1);
	}
    
	if (PT_PUSH_IS_IN_FIELDMAP(g_szErrCode))
	{
		//errcode
		node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, (char*)FileRptPack.msgcontent.szState);
		root->append_node(node1);
	}
    
	if (PT_PUSH_IS_IN_FIELDMAP(g_szErrCodeDesc))
	{
		//errcodedesc
		if(WBS_CARGTYPE_UPPER_STRING == it->second.nCargType)
		{
			//转换为大写
			strErrDesc = cstr::to_upper_s(strErrDesc.c_str());			
			node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, doc.allocate_string(strErrDesc.c_str()));
			root->append_node(node1);
		}
		else
		{
			node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, strErrDesc.c_str());
			root->append_node(node1);
		}

	}
	if (PT_PUSH_IS_IN_FIELDMAP(g_szExData))
	{
		//exdata
		string strExData("");
		{
			node1 = doc.allocate_node(rapidxml::node_element, it->second.szCArgName, FileRptPack.szExData);
			root->append_node(node1);
		}
	}
    


    //vector
    int nVecSize = allFiled.vecFiled.size();
    for (int i = 0; i < nVecSize; ++i)
    {
        rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, allFiled.vecFiled[i].szCArgName, allFiled.vecFiled[i].szCArgValue);
        if (0 == strlen(allFiled.vecFiled[i].szBelong))
        {
            //无子节点，无父节点
            doc.append_node(node);
        }
        else
        {
            root->append_node(node);
        }
    }

    doc.append_node(root);
    //最后一个包
    rapidxml::print(std::back_inserter(strOut), doc, 0);
}



void CParseToolImpl::MakePushRptURLData(const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, const HSBC_URLINFO& urlinfo, string &strOut)
{
	int nRetCode = ERRCODE_PUSH_USE_ONLY;
	char szRptMd5[MAX_PWD_LEN + 1] = { 0 };
	char szRptUserId[MAX_RPT_USERID_LEN + 1] = { 0 };

	int nBodyLen = 0;
	int nOutMsgLen = 0;
	FILEDMAP::const_iterator it;

	string strRpts("");
	string strBody("");
	string strFixFiels("");
	string strTmp("");
	if (0 == FileRptPackArr.size())
	{
		return;
	}
	try
	{
		//zhubo 2017-12-12
		GetUrlRequestFixedValue(allFiled, strFixFiels, false);
		strBody += strFixFiels;

		string strTime = CGlobal::date("mdHis");
		//================用户名和密码
		PushHelper::GetRptReqUserIdAndPwd(urlinfo, strTime, szRptUserId, szRptMd5);

		//用户名
		MakeUrlItemValue(allFiled, g_szUesrId, szRptUserId, strBody, true);

		//密码
		MakeUrlItemValue(allFiled, g_szPwd, szRptMd5, strBody);
		//================用户名和密码

		//是否有签名
		bool bSign = false;
		if (PT_PUSH_IS_IN_FIELDMAP(g_szSign))
		{
			//查看签名的格式  (兼容国美)
			//1:name+pushTime+pwd
			if (1 == urlinfo.nSignType)
			{
				string strPushTime("");
				string strSign("");
				PushHelper::GetGomePushTime(strPushTime);
				MakeUrlItemValue(allFiled, g_szPushTime, strPushTime.c_str(), strBody);

				PushHelper::MakeGomeSign(szRptUserId, strPushTime.c_str(), szRptMd5, strSign);

				MakeUrlItemValue(allFiled, g_szSign, strSign.c_str(), strBody);

				bSign = true;
			}
		}

		//timestamp
		MakeUrlItemValue(allFiled, g_szTimeStamp, strTime.c_str(), strBody);
		//Cmd
		MakeUrlItemValue(allFiled, g_szCmd, "RPT_REQ", strBody);
		//Seq
		strTmp = cstr::format("%d", FileRptPackArr[0].sockInfo.nSockId);
		MakeUrlItemValue(allFiled, g_szSeqId, strTmp.c_str(), strBody);

		//==============rpts
		//rpts的json报文
		MakeRptJsonNoRootData(nRetCode, allFiled, FileRptPackArr, strRpts, urlinfo, bSign);
		//Rpts
		MakeUrlItemValue(allFiled, g_szRpts, strRpts.c_str(), strBody);
		//==============rpts


		strOut = strBody;
	}
	catch (...)
	{
		nOutMsgLen = -1;
	}

	return;
}

void CParseToolImpl::MakePushRptURLData(const ALLFILED &allFiled, const PACK_DELIVER &FileRptPack, const HSBC_URLINFO& urlinfo, string &strOut)
{

    FILEDMAP::const_iterator it;

	int nStatus = -1;
	string strErrDesc("");
	std::string strBody = "";
	
    try
    {
		char szRptUserId[MAX_RPT_USERID_LEN+ 1] = { 0 };
		char szRptPwd[MAX_PWD_LEN + 1] = { 0 };

		//时间戳
		string strTime = CGlobal::date("mdHis");
	
		//获取推送账号和推送密码
		PushHelper::GetMoReqUserIdAndPwd(urlinfo, strTime, szRptUserId, szRptPwd);

        //确定标签
        //userid
        MakeUrlItemValue(allFiled, g_szUesrId, szRptUserId, strBody, true);

		//pwd
		MakeUrlItemValue(allFiled, g_szPwd, szRptPwd, strBody);

		//timestamp
		MakeUrlItemValue(allFiled, g_szTimeStamp, strTime.c_str(), strBody);

        //cmd
        MakeUrlItemValue(allFiled, g_szCmd, "RPT_REQ", strBody);

        //seqid
        std::string strTmp = cstr::format("%d", FileRptPack.sockInfo.nSockId);
        MakeUrlItemValue(allFiled, g_szSeqId, strTmp.c_str(), strBody);

		//固定字段值
		std::string strFixedValue = "";
		GetUrlRequestFixedValue(allFiled, strFixedValue);
		strBody += strFixedValue;
		
        //msgid
        strTmp = cstr::format("%lld", m_global.TranMsgIdCharToI64(FileRptPack.szMsgId));
        MakeUrlItemValue(allFiled, g_szMsgId, strTmp.c_str(), strBody);

        //custid
        MakeUrlItemValue(allFiled, g_szCustID, (char*)FileRptPack.szCustId, strBody);

        //pknum
        strTmp = cstr::format("%d", FileRptPack.nPknum);
        MakeUrlItemValue(allFiled, g_szPkNum,strTmp.c_str(), strBody);
		
        //pktotal
        strTmp = cstr::format("%d", FileRptPack.nPkTotal);
        MakeUrlItemValue(allFiled, g_szPkTotal,strTmp.c_str(), strBody);

        //mobile
        MakeUrlItemValue(allFiled, g_szMobile,(char*)FileRptPack.msgcontent.szDestTerminalId, strBody);

        //spno
        MakeUrlItemValue(allFiled, g_szSpNo,(char*)FileRptPack.szDestId, strBody);

        //exno
        char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
        if ('*' != FileRptPack.szSpNumEx[0])
        {
            memcpy(szTmp, FileRptPack.szSpNumEx, strlen((char*)FileRptPack.szSpNumEx));
        }
        MakeUrlItemValue(allFiled, g_szExNo,szTmp, strBody);

		char szSendTime[25] = { 0 };
		char szReceiveTime[25] = { 0 };
		char szPushTime[25] = { 0 };
		SYSTEMTIME_EX tt;
		CAdapter::GetLocalTime(&tt);
		SYSTEMTIME_EX_LINUX t(tt);

		if (2 == urlinfo.nPushtimefmt)
		{
			int nYear_MsgId = 0, nMon_MsgId = 0, nDay_MsgId = 0, nHour_MsgId = 0, nMin_MsgId = 0, nSec_MsgId = 0;
			sscanf(FileRptPack.szSendTime, "%04d-%02d-%02d %02d:%02d:%02d",
			&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
			sprintf(szSendTime, "%04d%02d%02d%02d%02d%02d",
			nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);
			sscanf(FileRptPack.szTime, "%04d-%02d-%02d %02d:%02d:%02d",
			&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
			sprintf(szReceiveTime, "%04d%02d%02d%02d%02d%02d",
			nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);

			sprintf(szPushTime, "%04d%02d%02d%02d%02d%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
		}
		else
		{
			memcpy(szSendTime, FileRptPack.szSendTime, min(strlen(FileRptPack.szSendTime), sizeof(szSendTime)));
			memcpy(szReceiveTime, FileRptPack.szTime, min(strlen(FileRptPack.szTime), sizeof(szReceiveTime)));
			sprintf(szPushTime, "%04d-%02d-%02d %02d:%02d:%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
		}
        //stime
        MakeUrlItemValue(allFiled, g_szSendTime,szSendTime, strBody);

        //rtime
        MakeUrlItemValue(allFiled, g_szRecvTime,szReceiveTime, strBody);

		//ptime
        MakeUrlItemValue(allFiled, g_szPushTime,szPushTime, strBody);

		//status
        nStatus = memcmp(FileRptPack.msgcontent.szState, "DELIVRD", 7) == 0 ? 0 : 2;
        if (0 == nStatus)
        {
            strErrDesc = RPTSTATUSDESC_OK;
        }
        else
        {
            strErrDesc = RPTSTATUSDESC_ERR;
        }
		strTmp = cstr::format("%d", nStatus);
        MakeUrlItemValue(allFiled, g_szStatus,strTmp.c_str(), strBody);
		
        //errcode
        MakeUrlItemValue(allFiled, g_szErrCode,(char*)FileRptPack.msgcontent.szState, strBody);

        //errdesc
        FILEDMAP::const_iterator it;
		if (PT_PUSH_IS_IN_FIELDMAP(g_szErrCodeDesc))
		{
			if(WBS_CARGTYPE_UPPER_STRING == it->second.nCargType)
			{
				//转换为大写
				strErrDesc = cstr::to_upper_s(strErrDesc.c_str());
			}
		}
        MakeUrlItemValue(allFiled, g_szErrCodeDesc,strErrDesc.c_str(), strBody);

        //exdata
        MakeUrlItemValue(allFiled, g_szExData,(char*)FileRptPack.szExData, strBody);

        strOut = strBody;
    }
    catch (...)
    {
		
    }

    return;
}

void CParseToolImpl::MakePushMoJsonData(const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileMoPackArr, const HSBC_URLINFO& urlinfo, string &strOut)
{
    //组mo推送报文，模板方式
    if (FileMoPackArr.empty())
    {
        return;
    }

    //推送客户配置的，没有配置的按照默认的标签推
    int nSize = FileMoPackArr.size();
    std::vector<PACK_DELIVER>::const_iterator itFileMo = FileMoPackArr.begin();
    rapidxml::xml_document<> doc;

    FILEDMAP::const_iterator it;
    FILEDMAP::const_iterator itRpts;
    FILEDMAP::const_iterator itRpt;

    //存标签
    string strTagMoReq("");
    string strTagMos("");
    //string strTagMo("");


    itRpts = allFiled.filedmap.find(g_szMOS);
    if (itRpts == allFiled.filedmap.end())
    {
        strOut = "";
        return;
    }

    //确定标签
    //Mos
    if ((it = allFiled.filedmap.find(g_szMOS)) != allFiled.filedmap.end())
    {
        strTagMos = it->second.szCArgName;
    }
    else
    {
        strTagMos = g_szMOS;
    }

    int nOutMsgLen = 0;
    
    //char szMsgTmp[MAX_SPLIT_MSG_LEN * 3 * 3 + 1] = { 0 };
	char szMoUserId[MAX_MO_USERID_LEN + 1] = { 0 };
	char szMoPwd[MAX_PWD_LEN + 1] = { 0 };

    try
    {
        //xmlHead
        rapidxml::xml_node<>* xmlHead = doc.allocate_node(rapidxml::node_pi, doc.allocate_string("xml version=\"1.0\" encoding=\"utf-8\""));
        doc.append_node(xmlHead);

        std::vector<PACK_DELIVER>::const_iterator itFileMo = FileMoPackArr.begin();
        int nExcpLine = __LINE__;        //记录当前行
        rapidjson::Value array(rapidjson::kArrayType);
        rapidjson::Document document;
        //	rapidjson::Value v2, v3;
        char szBelong[BELONG_LEN + 1] = { 0 };
        document.SetObject();
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
        rapidjson::Value objectPre(rapidjson::kObjectType);
        FILEDMAP::const_iterator it;

        //时间戳
        string strTime = CGlobal::date("mdHis");

		PushHelper::GetMoReqUserIdAndPwd(urlinfo, strTime, szMoUserId, szMoPwd);


        //userid
        if ((it = allFiled.filedmap.find(g_szUesrId)) != allFiled.filedmap.end())
        {
            if ((0 == it->second.nBelongtype) && (0 != strlen(it->second.szCArgName)))
            {
                if (0 != strlen(it->second.szBelong)) //mos
                {
                    szBelong[0] = '\0';
                    strcpy(szBelong, it->second.szBelong);
                    //无子节点，有父节点,而且跟梦网标准字段有对应
                    rapidjson::Value v2(it->second.szCArgName, allocator);
                    rapidjson::Value v3(szMoUserId, allocator);
                    objectPre.AddMember(v2, v3, allocator);
                }
                else
                {
                    //无子节点,无父节点，而且跟梦网标准字段有对应
                    rapidjson::Value v2(it->second.szCArgName, allocator);
                    rapidjson::Value v3(szMoUserId, allocator);
                    document.AddMember(v2, v3, allocator);
                }
            }
        }

        //pwd
        if ((it = allFiled.filedmap.find(g_szPwd)) != allFiled.filedmap.end())
        {
            if ((0 == it->second.nBelongtype) && (0 != strlen(it->second.szCArgName)))
            {
                if (0 != strlen(it->second.szBelong)) //mos
                {
                    szBelong[0] = '\0';
                    strcpy(szBelong, it->second.szBelong);
                    //无子节点，有父节点,而且跟梦网标准字段有对应
                    rapidjson::Value v2(it->second.szCArgName, allocator);
                    rapidjson::Value v3(szMoPwd, allocator);
                    objectPre.AddMember(v2, v3, allocator);
                }
                else
                {
                    //无子节点,无父节点，而且跟梦网标准字段有对应
                    rapidjson::Value v2(it->second.szCArgName, allocator);
                    rapidjson::Value v3(szMoPwd, allocator);
                    document.AddMember(v2, v3, allocator);
                }
            }
        }

        //timestamp
        if ((it = allFiled.filedmap.find(g_szTimeStamp)) != allFiled.filedmap.end())
        {
            if ((0 == it->second.nBelongtype) && (0 != strlen(it->second.szCArgName)))
            {
                if (0 != strlen(it->second.szBelong)) //mos
                {
                    szBelong[0] = '\0';
                    strcpy(szBelong, it->second.szBelong);
                    //无子节点，有父节点,而且跟梦网标准字段有对应
                    rapidjson::Value v2(it->second.szCArgName, allocator);
                    rapidjson::Value v3(strTime.c_str(), allocator);
                    objectPre.AddMember(v2, v3, allocator);
                }
                else
                {
                    //无子节点,无父节点，而且跟梦网标准字段有对应
                    rapidjson::Value v2(it->second.szCArgName, allocator);
                    rapidjson::Value v3(strTime.c_str(), allocator);
                    document.AddMember(v2, v3, allocator);
                }
            }
        }


        //cmd
        if ((it = allFiled.filedmap.find(g_szCmd)) != allFiled.filedmap.end())
        {
            if ((0 == it->second.nBelongtype) && (0 != strlen(it->second.szCArgName)))
            {
                if (0 != strlen(it->second.szBelong)) //mos
                {
                    szBelong[0] = '\0';
                    strcpy(szBelong, it->second.szBelong);
                    //无子节点，有父节点,而且跟梦网标准字段有对应
                    rapidjson::Value v2(it->second.szCArgName, allocator);
                    rapidjson::Value v3(g_szMoCmd, allocator);
                    objectPre.AddMember(v2, v3, allocator);
                }
                else
                {
                    //无子节点,无父节点，而且跟梦网标准字段有对应
                    rapidjson::Value v2(it->second.szCArgName, allocator);
                    rapidjson::Value v3(g_szMoCmd, allocator);
                    document.AddMember(v2, v3, allocator);
                }
            }
        }

        //seq
        if ((it = allFiled.filedmap.find(g_szSeqId)) != allFiled.filedmap.end())
        {
            if ((0 == it->second.nBelongtype) && (0 != strlen(it->second.szCArgName)))
            {
                if (0 != strlen(it->second.szBelong)) //mos
                {
                    szBelong[0] = '\0';
                    strcpy(szBelong, it->second.szBelong);
                    //无子节点，有父节点,而且跟梦网标准字段有对应
                    rapidjson::Value v2(it->second.szCArgName, allocator);
                    objectPre.AddMember(v2, FileMoPackArr[0].sockInfo.nSockId, allocator);
                }
                else
                {
                    //无子节点,无父节点，而且跟梦网标准字段有对应
                    rapidjson::Value v2(it->second.szCArgName, allocator);
                    document.AddMember(v2, FileMoPackArr[0].sockInfo.nSockId, allocator);
                }
            }
        }

		//zhubo 2017-12-12
		MakeFixedJsonValueToDoc(allFiled, document);
		//zhubo 2017-12-12



        for (; (itFileMo != FileMoPackArr.end()); ++itFileMo)
        {
            const PACK_DELIVER &FileRptPack = *itFileMo;
            rapidjson::Value object(rapidjson::kObjectType);

            //mo对象
			if (PT_PUSH_IS_IN_FIELDMAP(g_szMsgId))
            {
                //misgid
                if(WBS_CARGTYPE_OTHER == it->second.nCargType)
				{
					int64_t nMsgId = m_global.TranMsgIdCharToI64(FileRptPack.szMsgId);
					char szMsgIdTmp[64+1] = {0};
					CAdapter::_i64toa(nMsgId, szMsgIdTmp, 10);
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3(szMsgIdTmp, allocator);
					object.AddMember(v2, v3, allocator);				
				}
				else
				{
					rapidjson::Value v2(it->second.szCArgName,  allocator);
					object.AddMember(v2, (int64_t)m_global.TranMsgIdCharToI64(FileRptPack.szMsgId), allocator);
				}
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szMobile))
            {
                //mobile
				rapidjson::Value v2(it->second.szCArgName, allocator);
                rapidjson::Value v3((char*)FileRptPack.szSrcTerminalId, allocator);
                object.AddMember(v2, v3, allocator);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szSpNo))
            {
                //spno
				rapidjson::Value v2(it->second.szCArgName, allocator);
                rapidjson::Value v3((char*)FileRptPack.szDestId, allocator);
                object.AddMember(v2, v3, allocator);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szExNo))
            {
                //exno
                //char szTmpEx[MAX_CHANNEL_LEN + 1] = { 0 };
                if ('*' == FileRptPack.szSpNumEx[0])
                {
                    //memcpy(szTmpEx, FileRptPack.szSpNumEx, strlen((char*)FileRptPack.szSpNumEx));
					rapidjson::Value v2(it->second.szCArgName, allocator);
                    object.AddMember(v2, "", allocator);
                }
                else
                {
					rapidjson::Value v2(it->second.szCArgName, allocator);
                    rapidjson::Value v3((char*)FileRptPack.szSpNumEx, allocator);
                    object.AddMember(v2, v3, allocator);
                }
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szRecvTime))
            {
                //rtime
				char szReceiveTime[25] = { 0 };
				if (2 == urlinfo.nPushtimefmt)
				{
					int nYear_MsgId = 0, nMon_MsgId = 0, nDay_MsgId = 0, nHour_MsgId = 0, nMin_MsgId = 0, nSec_MsgId = 0;
					sscanf(FileRptPack.szTime, "%04d-%02d-%02d %02d:%02d:%02d",
						&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
					sprintf(szReceiveTime, "%04d%02d%02d%02d%02d%02d",
						nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);
				}
				else
				{
					memcpy(szReceiveTime, FileRptPack.szTime, min(strlen(FileRptPack.szTime), sizeof(szReceiveTime)-1));
				}
				//rtime
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(szReceiveTime, allocator);
				object.AddMember(v2, v3, allocator);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szMsg))
            {
				//content
				//CGlobal::UrlEncode((char*)itFileMo->szMsgContent, szMsgTmp, sizeof(szMsgTmp), false);
				std::string strMsgTmp = "";

			    if(WBS_UTF8 == urlinfo.nPushCodeType)
            	{
					strMsgTmp = (char*)itFileMo->szMsgContent;
				}
				else
				{
					strMsgTmp = CGlobal::UrlEncode((char*)itFileMo->szMsgContent);
				}
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(strMsgTmp.c_str(), allocator);
				object.AddMember(v2, v3, allocator);
            }

            //vector
			MakeFixedJsonValueToObj(allFiled, strTagMos, document, object);

            array.PushBack(object, allocator);

        }

        //最后一个包
        rapidjson::Value v(strTagMos.c_str(), allocator);
        document.AddMember(v, array, allocator);

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);
        //可能会崩溃
        strOut = buffer.GetString();
    }
    catch (...)
    {
        nOutMsgLen = -1;
    }

}

void CParseToolImpl::MakePushMoJsonData(const ALLFILED &allFiled, const PACK_DELIVER &FileMoPack, const HSBC_URLINFO& urlinfo, string &strOut)
{
    //组mo推送报文，模板方式
	//推送客户配置的，没有配置的按照默认的标签推
	rapidxml::xml_document<> doc;
	FILEDMAP::const_iterator it;

	int nOutMsgLen = 0;


	try
	{
		int nExcpLine = __LINE__;        //记录当前行
		rapidjson::Value array(rapidjson::kArrayType);
		rapidjson::Document document;
		rapidjson::Value v2, v3;
		char szBelong[BELONG_LEN + 1] = { 0 };
		document.SetObject();
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
		FILEDMAP::const_iterator it;

		//时间戳
		string strTime = CGlobal::date("mdHis");

		char szMoUserId[MAX_MO_USERID_LEN + 1] = { 0 };
		char szMoPwd[MAX_PWD_LEN + 1] = { 0 };
		
		//获取推送账号和推送密码
		PushHelper::GetMoReqUserIdAndPwd(urlinfo, strTime, szMoUserId, szMoPwd);

		//userid
		if ((it = allFiled.filedmap.find(g_szUesrId)) != allFiled.filedmap.end())
		{
			if (0 != strlen(it->second.szCArgName))
			{
				//无子节点,无父节点，而且跟梦网标准字段有对应
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(szMoUserId, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}

		//signex
		if ((it = allFiled.filedmap.find(g_szSign)) != allFiled.filedmap.end())
		{
			if (0 != strlen(it->second.szCArgName))
			{
				//无子节点,无父节点，而且跟梦网标准字段有对应
				rapidjson::Value v2(it->second.szCArgName, allocator);		
				char szUtf8[128] = {0};
				string strSrc = (char*)FileMoPack.szSignature;
				//去左右[]【】
				
				
				if(strSrc.compare(0,1,"[") == 0)
				{
					strSrc.erase(0,1);
				}
				else if(strSrc.compare(0,2,"【") == 0)
				{
					strSrc.erase(0,2);
				}

				int nSignLen = strSrc.length();
				if(nSignLen >1 && strSrc.compare(nSignLen-1,1,"]") == 0)
				{
					strSrc.erase(nSignLen-1,1);
				}
				else if(nSignLen > 2 && strSrc.compare(nSignLen-2,2,"】") == 0)
				{
					strSrc.erase(nSignLen-2,2);
				}
				ToUTF8((char*)strSrc.c_str(), szUtf8, 128);
				rapidjson::Value v3(szUtf8, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}
		
		//pwd
		if ((it = allFiled.filedmap.find(g_szPwd)) != allFiled.filedmap.end())
		{
			if (0 != strlen(it->second.szCArgName))
			{
				//无子节点,无父节点，而且跟梦网标准字段有对应
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(szMoPwd, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}

		//timestamp
		if ((it = allFiled.filedmap.find(g_szTimeStamp)) != allFiled.filedmap.end())
		{
			if (0 != strlen(it->second.szCArgName))
			{
				//无子节点,无父节点，而且跟梦网标准字段有对应
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(strTime.c_str(), allocator);
				document.AddMember(v2, v3, allocator);
			}
		}

		if ((it = allFiled.filedmap.find(g_szUesrIdEx)) != allFiled.filedmap.end())
		{
			if (0 != strlen(it->second.szCArgName))
			{
				//无子节点,无父节点，而且跟梦网标准字段有对应
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(urlinfo.szMoPwd, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}

		//cmd
		if ((it = allFiled.filedmap.find(g_szCmd)) != allFiled.filedmap.end())
		{
			if (0 != strlen(it->second.szCArgName))
			{
				//无子节点,无父节点，而且跟梦网标准字段有对应
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(g_szMoCmd, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}

		//seq
		if ((it = allFiled.filedmap.find(g_szSeqId)) != allFiled.filedmap.end())
		{
			if (0 != strlen(it->second.szCArgName))
			{
				//无子节点,无父节点，而且跟梦网标准字段有对应
				rapidjson::Value v2(it->second.szCArgName, allocator);
				document.AddMember(v2, FileMoPack.sockInfo.nSockId, allocator);
			}
		}

		//mo对象
		if ((it = allFiled.filedmap.find(g_szMsgId)) != allFiled.filedmap.end())
		{
			if (0 != strlen(it->second.szCArgName))
			{
				//misgid
				if(WBS_CARGTYPE_OTHER == it->second.nCargType)
				{
					int64_t nMsgId = m_global.TranMsgIdCharToI64(FileMoPack.szMsgId);
					char szMsgIdTmp[64+1] = {0};
					CAdapter::_i64toa(nMsgId, szMsgIdTmp, 10);
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3(szMsgIdTmp, allocator);
					document.AddMember(v2, v3, allocator);				
				}
				else
				{
					rapidjson::Value v2(it->second.szCArgName, allocator);
					document.AddMember(v2, (int64_t)m_global.TranMsgIdCharToI64(FileMoPack.szMsgId), allocator);
				}
			}
		}

		if ((it = allFiled.filedmap.find(g_szMobile)) != allFiled.filedmap.end())
		{
			if (0 != strlen(it->second.szCArgName))
			{
				//mobile
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3((char*)FileMoPack.szSrcTerminalId, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}

		if ((it = allFiled.filedmap.find(g_szSpNo)) != allFiled.filedmap.end())
		{
			if (0 != strlen(it->second.szCArgName))
			{
				//spno
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3((char*)FileMoPack.szDestId, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}

		if ((it = allFiled.filedmap.find(g_szExNo)) != allFiled.filedmap.end())
		{
			if (0 != strlen(it->second.szCArgName))
			{
				//exno
				if ('*' == FileMoPack.szSpNumEx[0])
				{
					//memcpy(szTmpEx, FileRptPack.szSpNumEx, strlen((char*)FileRptPack.szSpNumEx));
					rapidjson::Value v2(it->second.szCArgName, allocator);
					document.AddMember(v2, "", allocator);
				}
				else
				{
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3((char*)FileMoPack.szSpNumEx, allocator);
					document.AddMember(v2, v3, allocator);
				}
			}
		}

		//rtime
		if ((it = allFiled.filedmap.find(g_szRecvTime)) != allFiled.filedmap.end())
		{
			if (0 != strlen(it->second.szCArgName))
			{
				//rtime
				char szReceiveTime[25] = { 0 };
				if (2 == urlinfo.nPushtimefmt)
				{
					int nYear_MsgId = 0, nMon_MsgId = 0, nDay_MsgId = 0, nHour_MsgId = 0, nMin_MsgId = 0, nSec_MsgId = 0;
					sscanf(FileMoPack.szTime, "%04d-%02d-%02d %02d:%02d:%02d",
						&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
					sprintf(szReceiveTime, "%04d%02d%02d%02d%02d%02d",
						nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);
				}
				else
				{
					memcpy(szReceiveTime, FileMoPack.szTime, min(strlen(FileMoPack.szTime), sizeof(szReceiveTime)-1));
				}
				//rtime
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(szReceiveTime, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}

		if ((it = allFiled.filedmap.find(g_szMsg)) != allFiled.filedmap.end())
		{
			if (0 != strlen(it->second.szCArgName))
			{
				//content
				//CGlobal::UrlEncode((char*)FileMoPack.szMsgContent, szMsgTmp, sizeof(szMsgTmp), false);
				std::string strMsgTmp = "";

			    if(WBS_UTF8 == urlinfo.nPushCodeType)
            	{
					strMsgTmp = (char*)FileMoPack.szMsgContent;
				}
				else
				{
					strMsgTmp = CGlobal::UrlEncode((char*)FileMoPack.szMsgContent);
				}
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(strMsgTmp.c_str(), allocator);
				document.AddMember(v2, v3, allocator);
			}
		}

		//vector
		/*
		int nVecSize = allFiled.vecFiled.size();
		for (int i = 0; i < nVecSize; ++i)
		{
			rapidjson::Value v2(allFiled.vecFiled[i].szCArgName, allocator);
			rapidjson::Value v3(allFiled.vecFiled[i].szCArgValue, allocator);
			document.AddMember(v2, v3, allocator);
		}
		*/
		MakeFixedJsonValueToDoc(allFiled, document);
		
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);

		//可能会崩溃
		strOut = buffer.GetString();
	}
	catch (...)
	{
		nOutMsgLen = -1;
	}

}

void CParseToolImpl::MakeMoJsonNoRootData(int nRetCode, const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, string &strOut, const HSBC_URLINFO& urlinfo, bool bSign)
{//private
    std::vector<PACK_DELIVER>::const_iterator itFileMo = FileRptPackArr.begin();
    cJSON *root, *obj;
    int nExcpLine = __LINE__;        //记录当前行
    //char szBelong[BELONG_LEN+1] = { 0 };
    FILEDMAP::const_iterator it;

    try
    {
        root = cJSON_CreateArray();
        if (NULL == root)
        {
            return;
        }

        for (; (itFileMo != FileRptPackArr.end()); ++itFileMo)
        {
            const PACK_DELIVER &FileRptPack = *itFileMo;
            obj = cJSON_CreateObject();

            //mo对象
            if ((it = allFiled.filedmap.find(g_szUesrIdEx)) != allFiled.filedmap.end())
		    {
                //userid
                if(strlen(it->second.szCArgName) > 0)
                {
               		cJSON_AddStringToObject(obj, it->second.szCArgName,urlinfo.szUsrId);
                }
			}
			
            if ((it = allFiled.filedmap.find(g_szMsgId)) != allFiled.filedmap.end())
		    {
                //misgid
                if(strlen(it->second.szCArgName) > 0)
                {
               		cJSON_AddNumber64ToObject(obj, it->second.szCArgName, m_global.TranMsgIdCharToI64(FileRptPack.szMsgId));
                }
			}

            if ((it = allFiled.filedmap.find(g_szMobile)) != allFiled.filedmap.end())
		    {
                //mobile
                if(strlen(it->second.szCArgName) > 0)
                {
                	cJSON_AddStringToObject(obj, it->second.szCArgName, (char*)FileRptPack.szSrcTerminalId);
                }
			}
			
            if ((it = allFiled.filedmap.find(g_szSpNo)) != allFiled.filedmap.end())
            {
                //spno
                if(strlen(it->second.szCArgName) > 0)
                {
               		cJSON_AddStringToObject(obj, it->second.szCArgName, (char*)FileRptPack.szDestId);
                }
			}
			
            if ((it = allFiled.filedmap.find(g_szExNo)) != allFiled.filedmap.end())
            {
                //exno
                if(strlen(it->second.szCArgName) > 0)
                {
	                char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
	                if ('*' == FileRptPack.szSpNumEx[0])
	                {
	                    cJSON_AddStringToObject(obj, it->second.szCArgName, "");
	                }
	                else
	                {
	                    cJSON_AddStringToObject(obj, it->second.szCArgName, (char*)FileRptPack.szSpNumEx);
	                }
                }
            }

            if ((it = allFiled.filedmap.find(g_szRecvTime)) != allFiled.filedmap.end())
            {
                //rtime
                if(strlen(it->second.szCArgName) > 0)
                {
                	char szReceiveTime[25] = { 0 };
					if (2 == urlinfo.nPushtimefmt)
					{
						int nYear_MsgId = 0, nMon_MsgId = 0, nDay_MsgId = 0, nHour_MsgId = 0, nMin_MsgId = 0, nSec_MsgId = 0;
						sscanf(FileRptPack.szTime, "%04d-%02d-%02d %02d:%02d:%02d",
							&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
						sprintf(szReceiveTime, "%04d%02d%02d%02d%02d%02d",
							nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);
					}
					else
					{
						memcpy(szReceiveTime, FileRptPack.szTime, min(strlen(FileRptPack.szTime), sizeof(szReceiveTime)-1));
					}
            	    cJSON_AddStringToObject(obj, it->second.szCArgName, szReceiveTime);
                }
			}
			
            if ((it = allFiled.filedmap.find(g_szMsg)) != allFiled.filedmap.end())
            {
				//content
				//CGlobal::UrlEncode((char*)itFileMo->szMsgContent, szMsgTmp, sizeof(szMsgTmp), false);
				if(strlen(it->second.szCArgName) > 0)
                {
                	std::string strMsgTmp = "";
                	if(bSign && WBS_UTF8 == urlinfo.nPushCodeType)
                	{
						strMsgTmp = (char*)itFileMo->szMsgContent;
					}
					else
					{
						strMsgTmp = CGlobal::UrlEncode((char*)itFileMo->szMsgContent);
					}
					cJSON_AddStringToObject(obj, it->second.szCArgName, strMsgTmp.c_str());
				}
			}

			//zhubo 2017-12-12

			//zhubo 2017-12-12

            cJSON_AddItemToArray(root, obj);
        }


        strOut = cJSON_Print(root);
        //删除
        cJSON_Delete(root);
    }
    catch (...)
    {

    }
}



void CParseToolImpl::MakePushRptJsonData(const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, const HSBC_URLINFO& urlinfo, string &strOut)
{
    std::vector<PACK_DELIVER>::const_iterator itFileRpt = FileRptPackArr.begin();
    int nExcpLine = __LINE__;        //记录当前行
    rapidjson::Value array(rapidjson::kArrayType);
    rapidjson::Document document;
    //	rapidjson::Value v2, v3;
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    char szBelong[BELONG_LEN + 1] = { 0 };
    FILEDMAP::const_iterator it;

    //存标签
    string strTagRptReq("");
    string strTagRpts("");
    string strTagRpt("");


    //确定标签
    //Mos
    if ((it = allFiled.filedmap.find(g_szRpts)) != allFiled.filedmap.end())
    {
        strTagRpts = it->second.szCArgName;
    }
    else
    {
        strTagRpts = g_szRpts;
    }

    document.SetObject();
    //	rapidjson::Value objectPre(rapidjson::kObjectType);

    try
    {
		int nStatus = -1;
		string strErrDesc("");
		char szRptUserId[MAX_RPT_USERID_LEN + 1] = { 0 };
		char szRptPwd[MAX_PWD_LEN + 1] = { 0 };

		//时间戳
		string strTime = CGlobal::date("mdHis");

		PushHelper::GetRptReqUserIdAndPwd(urlinfo, strTime, szRptUserId, szRptPwd);

        //userid
		if (PT_PUSH_IS_IN_FIELDMAP(g_szUesrId))
        {
            rapidjson::Value v2(it->second.szCArgName, allocator);
            rapidjson::Value v3(szRptUserId, allocator);
            document.AddMember(v2, v3, allocator);
        }

        //pwd
		if (PT_PUSH_IS_IN_FIELDMAP(g_szPwd))
        {
            rapidjson::Value v2(it->second.szCArgName, allocator);
            rapidjson::Value v3(szRptPwd, allocator);
            document.AddMember(v2, v3, allocator);
        }

        //timestamp
		if (PT_PUSH_IS_IN_FIELDMAP(g_szTimeStamp))
        {
            rapidjson::Value v2(it->second.szCArgName, allocator);
            rapidjson::Value v3(strTime.c_str(), allocator);
            document.AddMember(v2, v3, allocator);
        }

        //cmd
		if (PT_PUSH_IS_IN_FIELDMAP(g_szCmd))
        {
            rapidjson::Value v2(it->second.szCArgName, allocator);
            rapidjson::Value v3(g_szRptCmd, allocator);
            document.AddMember(v2, v3, allocator);
        }

        //seq
		if (PT_PUSH_IS_IN_FIELDMAP(g_szSeqId))
        {
            rapidjson::Value v2(it->second.szCArgName, allocator);
            document.AddMember(v2, FileRptPackArr[0].sockInfo.nSockId, allocator);
        }
		//zhubo 2017-12-12
		MakeFixedJsonValueToDoc(allFiled, document);
		//zhubo 2017-12-12

        for (; (itFileRpt != FileRptPackArr.end()); ++itFileRpt)
        {
            const PACK_DELIVER &FileRptPack = *itFileRpt;
            rapidjson::Value object(rapidjson::kObjectType);

            //确定成功和失败描述
            nStatus = _strnicmp((LPCSTR)FileRptPack.msgcontent.szState, "DELIVRD", 7) == 0 ? 0 : 2;
            if (0 == nStatus)
            {
                strErrDesc = RPTSTATUSDESC_OK;
            }
            else
            {
                strErrDesc = RPTSTATUSDESC_ERR;
            }

            //rpt对象
			if (PT_PUSH_IS_IN_FIELDMAP(g_szMsgId))
            {
                //misgid
                if(WBS_CARGTYPE_OTHER == it->second.nCargType)
				{
					int64_t nMsgId = m_global.TranMsgIdCharToI64(FileRptPack.szMsgId);
					char szMsgIdTmp[64+1] = {0};
					CAdapter::_i64toa(nMsgId, szMsgIdTmp, 10);
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3(szMsgIdTmp, allocator);
					object.AddMember(v2, v3, allocator);				
				}
				else
				{
					rapidjson::Value v2(it->second.szCArgName, allocator);
					object.AddMember(v2, (int64_t)m_global.TranMsgIdCharToI64(FileRptPack.szMsgId), allocator);
				}
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szCustID))
            {
				rapidjson::Value v2(it->second.szCArgName, allocator);
                rapidjson::Value v3((char*)FileRptPack.szCustId, allocator);
                object.AddMember(v2, v3, allocator);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szPkNum))
            {
                //pkno
				rapidjson::Value v2(it->second.szCArgName, allocator);
                object.AddMember(v2, FileRptPack.nPknum, allocator);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szPkTotal))
            {
                //
				rapidjson::Value v2(it->second.szCArgName, allocator);
                object.AddMember(v2, FileRptPack.nPkTotal, allocator);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szMobile))
            {
                //mobile
				rapidjson::Value v2(it->second.szCArgName, allocator);
                rapidjson::Value v3((char*)FileRptPack.msgcontent.szDestTerminalId, allocator);
                object.AddMember(v2, v3, allocator);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szSpNo))
            {
                //spno
				rapidjson::Value v2(it->second.szCArgName, allocator);
                rapidjson::Value v3((char*)FileRptPack.szDestId, allocator);
                object.AddMember(v2, v3, allocator);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szExNo))
            {
                //exno
                if ('*' == FileRptPack.szSpNumEx[0])
                {
					rapidjson::Value v2(it->second.szCArgName, allocator);
                    object.AddMember(v2, "", allocator);
                }
                else
                {
					rapidjson::Value v2(it->second.szCArgName, allocator);
                    rapidjson::Value v3((char*)FileRptPack.szSpNumEx, allocator);
                    object.AddMember(v2, v3, allocator);
                }
            }

			char szSendTime[25] = { 0 };
			char szReceiveTime[25] = { 0 };
			char szPushTime[25] = { 0 };
			SYSTEMTIME_EX tt;
			CAdapter::GetLocalTime(&tt);
			SYSTEMTIME_EX_LINUX t(tt);

			if (2 == urlinfo.nPushtimefmt)
			{
				int nYear_MsgId = 0, nMon_MsgId = 0, nDay_MsgId = 0, nHour_MsgId = 0, nMin_MsgId = 0, nSec_MsgId = 0;
				sscanf(FileRptPack.szSendTime, "%04d-%02d-%02d %02d:%02d:%02d",
				&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
				sprintf(szSendTime, "%04d%02d%02d%02d%02d%02d",
				nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);
				sscanf(FileRptPack.szTime, "%04d-%02d-%02d %02d:%02d:%02d",
				&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
				sprintf(szReceiveTime, "%04d%02d%02d%02d%02d%02d",
				nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);

				sprintf(szPushTime, "%04d%02d%02d%02d%02d%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
			}
			else
			{
				memcpy(szSendTime, FileRptPack.szSendTime, min(strlen(FileRptPack.szSendTime), sizeof(szSendTime)));
				memcpy(szReceiveTime, FileRptPack.szTime, min(strlen(FileRptPack.szTime), sizeof(szReceiveTime)));
				sprintf(szPushTime, "%04d-%02d-%02d %02d:%02d:%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
			}
			if (PT_PUSH_IS_IN_FIELDMAP(g_szSendTime))
            {
                //stime
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(szSendTime, allocator);
				object.AddMember(v2, v3, allocator);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szRecvTime))
            {
                //rtime
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(szReceiveTime, allocator);
				object.AddMember(v2, v3, allocator);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szPushTime))
			{
				//ptime
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(szPushTime, allocator);
				object.AddMember(v2, v3, allocator);
			}
			if (PT_PUSH_IS_IN_FIELDMAP(g_szStatus))
            {
                //status
				rapidjson::Value v2(it->second.szCArgName, allocator);
                object.AddMember(v2, nStatus, allocator);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szErrCode))
            {
                //errcode
				rapidjson::Value v2(it->second.szCArgName, allocator);
                rapidjson::Value v3((char*)FileRptPack.msgcontent.szState, allocator);
                object.AddMember(v2, v3, allocator);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szErrCodeDesc))
            {

                //errcodedesc
				if(WBS_CARGTYPE_UPPER_STRING == it->second.nCargType)
				{
					//转换为大写
					strErrDesc = cstr::to_upper_s(strErrDesc.c_str());
				}
				rapidjson::Value v2(it->second.szCArgName, allocator);
                rapidjson::Value v3(strErrDesc.c_str(), allocator);
                object.AddMember(v2, v3, allocator);
            }

			if (PT_PUSH_IS_IN_FIELDMAP(g_szExData))
            {
				rapidjson::Value v2(it->second.szCArgName, allocator);
                rapidjson::Value v3(FileRptPack.szExData, allocator);
                object.AddMember(v2, v3, allocator);
            }

            //vector
			//zhubo 2017-12-12
			MakeFixedJsonValueToObj(allFiled, strTagRpts, document, object);
			//zhubo 2017-12-12

            array.PushBack(object, allocator);

        }
        //最后一个包
        //array.PushBack(object, allocator);
        rapidjson::Value v(strTagRpts.c_str(), allocator);
        document.AddMember(v, array, allocator);
        //document.RemoveAllMembers();
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);
        //可能会崩溃
        strOut = buffer.GetString();
    }
    catch (...)
    {

    }
}

void CParseToolImpl::MakePushRptJsonData(const ALLFILED &allFiled, const PACK_DELIVER &FileRptPack, const HSBC_URLINFO& urlinfo, string &strOut)
{
	int nExcpLine = __LINE__;        //记录当前行
	rapidjson::Value array(rapidjson::kArrayType);
	rapidjson::Document document;
	rapidjson::Value v2, v3;
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	char szBelong[BELONG_LEN + 1] = { 0 };
	FILEDMAP::const_iterator it;

	document.SetObject();

	try
	{
		int nStatus = -1;
		string strErrDesc("");
		
		char szRptUserId[MAX_RPT_USERID_LEN+ 1] = { 0 };
		char szRptPwd[MAX_PWD_LEN + 1] = { 0 };

		//时间戳
		string strTime = CGlobal::date("mdHis");
	
		//获取推送账号和推送密码
		PushHelper::GetRptReqUserIdAndPwd(urlinfo, strTime, szRptUserId, szRptPwd);

		//userid
		if ((it = allFiled.filedmap.find(g_szUesrId)) != allFiled.filedmap.end())
		{
			if (strlen(it->second.szCArgName) > 0)
			{
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(szRptUserId, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}

		//useridex
		if ((it = allFiled.filedmap.find(g_szUesrIdEx)) != allFiled.filedmap.end())
		{
			if (0 != strlen(it->second.szCArgName))
			{
				//无子节点,无父节点，而且跟梦网标准字段有对应
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(urlinfo.szRptPwd, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}


		//pwd
		if ((it = allFiled.filedmap.find(g_szPwd)) != allFiled.filedmap.end())
		{
			if (strlen(it->second.szCArgName) > 0)
			{
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(szRptPwd, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}

		//timestamp
		if ((it = allFiled.filedmap.find(g_szTimeStamp)) != allFiled.filedmap.end())
		{
			if (strlen(it->second.szCArgName) > 0)
			{
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(strTime.c_str(), allocator);
				document.AddMember(v2, v3, allocator);
			}
		}

		//cmd
		if ((it = allFiled.filedmap.find(g_szCmd)) != allFiled.filedmap.end())
		{
			if (strlen(it->second.szCArgName) > 0)
			{
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(g_szRptCmd, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}

		//seq
		if ((it = allFiled.filedmap.find(g_szSeqId)) != allFiled.filedmap.end())
		{
			if (strlen(it->second.szCArgName) > 0)
			{
				rapidjson::Value v2(it->second.szCArgName, allocator);
				document.AddMember(v2, FileRptPack.sockInfo.nSockId, allocator);
			}
		}

		//确定成功和失败描述
		nStatus = _strnicmp((LPCSTR)FileRptPack.msgcontent.szState, "DELIVRD", 7) == 0 ? 0 : 2;
		if (0 == nStatus)
		{
			strErrDesc = RPTSTATUSDESC_OK;
		}
		else
		{
			strErrDesc = RPTSTATUSDESC_ERR;
		}

		//rpt对象	
		if ((it = allFiled.filedmap.find(g_szMsgId)) != allFiled.filedmap.end())
		{
			//misgid
			if (strlen(it->second.szCArgName) > 0)
			{
				if(WBS_CARGTYPE_OTHER == it->second.nCargType)
				{
					int64_t nMsgId = m_global.TranMsgIdCharToI64(FileRptPack.szMsgId);
					char szMsgIdTmp[64+1] = {0};
					CAdapter::_i64toa(nMsgId, szMsgIdTmp, 10);
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3(szMsgIdTmp, allocator);
					document.AddMember(v2, v3, allocator);				
				}
				else
				{
					rapidjson::Value v2(it->second.szCArgName, allocator);
					document.AddMember(v2, (int64_t)m_global.TranMsgIdCharToI64(FileRptPack.szMsgId), allocator);
				}
			}
		}
		
		if ((it = allFiled.filedmap.find(g_szCustID)) != allFiled.filedmap.end())
		{
			if (strlen(it->second.szCArgName) > 0)
			{
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3((char*)FileRptPack.szCustId, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}
		
		if ((it = allFiled.filedmap.find(g_szPkNum)) != allFiled.filedmap.end())
		{
			//pkno
			if (strlen(it->second.szCArgName) > 0)
			{
				rapidjson::Value v2(it->second.szCArgName, allocator);
				document.AddMember(v2, FileRptPack.nPknum, allocator);
			}
		}
		
		if ((it = allFiled.filedmap.find(g_szPkTotal)) != allFiled.filedmap.end())
		{
			if (strlen(it->second.szCArgName) > 0)
			{
				rapidjson::Value v2(it->second.szCArgName, allocator);
				document.AddMember(v2, FileRptPack.nPkTotal, allocator);
			}
		}
		
		if ((it = allFiled.filedmap.find(g_szMobile)) != allFiled.filedmap.end())
		{
			//mobile
			if (strlen(it->second.szCArgName) > 0)
			{
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3((char*)FileRptPack.msgcontent.szDestTerminalId, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}
		
		if ((it = allFiled.filedmap.find(g_szSpNo)) != allFiled.filedmap.end())
		{
			//spno
			if (strlen(it->second.szCArgName) > 0)
			{
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3((char*)FileRptPack.szDestId, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}

		if ((it = allFiled.filedmap.find(g_szExNo)) != allFiled.filedmap.end())
		{
			//exno
			if (strlen(it->second.szCArgName) > 0)
			{
				if ('*' == FileRptPack.szSpNumEx[0])
				{
					rapidjson::Value v2(it->second.szCArgName, allocator);
					document.AddMember(v2, "", allocator);
				}
				else
				{
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3((char*)FileRptPack.szSpNumEx, allocator);
					document.AddMember(v2, v3, allocator);
				}
			}
		}

		char szSendTime[25] = { 0 };
		char szReceiveTime[25] = { 0 };
		char szPushTime[25] = { 0 };
		SYSTEMTIME_EX tt;
		CAdapter::GetLocalTime(&tt);
		SYSTEMTIME_EX_LINUX t(tt);

		if (2 == urlinfo.nPushtimefmt)
		{
			int nYear_MsgId = 0, nMon_MsgId = 0, nDay_MsgId = 0, nHour_MsgId = 0, nMin_MsgId = 0, nSec_MsgId = 0;
			sscanf(FileRptPack.szSendTime, "%04d-%02d-%02d %02d:%02d:%02d",
			&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
			sprintf(szSendTime, "%04d%02d%02d%02d%02d%02d",
			nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);
			sscanf(FileRptPack.szTime, "%04d-%02d-%02d %02d:%02d:%02d",
			&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
			sprintf(szReceiveTime, "%04d%02d%02d%02d%02d%02d",
			nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);

			sprintf(szPushTime, "%04d%02d%02d%02d%02d%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
		}
		else
		{
			memcpy(szSendTime, FileRptPack.szSendTime, min(strlen(FileRptPack.szSendTime), sizeof(szSendTime)));
			memcpy(szReceiveTime, FileRptPack.szTime, min(strlen(FileRptPack.szTime), sizeof(szReceiveTime)));
			sprintf(szPushTime, "%04d-%02d-%02d %02d:%02d:%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
		}
		if ((it = allFiled.filedmap.find(g_szSendTime)) != allFiled.filedmap.end())
		{
			//stime
			if (strlen(it->second.szCArgName) > 0)
			{
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(szSendTime, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}
		
		if ((it = allFiled.filedmap.find(g_szRecvTime)) != allFiled.filedmap.end())
		{
			//rtime
			if (strlen(it->second.szCArgName) > 0)
			{
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(szReceiveTime, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}

		if ((it = allFiled.filedmap.find(g_szPushTime)) != allFiled.filedmap.end())
		{
			if (strlen(it->second.szCArgName) > 0)
			{
				//ptime
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(szPushTime, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}
	
		if ((it = allFiled.filedmap.find(g_szStatus)) != allFiled.filedmap.end())
		{
			//status
			if (strlen(it->second.szCArgName) > 0)
			{
				rapidjson::Value v2(it->second.szCArgName, allocator);
				document.AddMember(v2, nStatus, allocator);
			}
		}
		
		if ((it = allFiled.filedmap.find(g_szErrCode)) != allFiled.filedmap.end())
		{
			//errcode
			if (strlen(it->second.szCArgName) > 0)
			{
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3((char*)FileRptPack.msgcontent.szState, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}
		
		if ((it = allFiled.filedmap.find(g_szErrCodeDesc)) != allFiled.filedmap.end())
		{
			//errcodedesc
			if (strlen(it->second.szCArgName) > 0)
			{
				if(WBS_CARGTYPE_UPPER_STRING == it->second.nCargType)
				{
					//转换为大写
					strErrDesc = cstr::to_upper_s(strErrDesc.c_str());
				}
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(strErrDesc.c_str(), allocator);
				document.AddMember(v2, v3, allocator);
			}
		}
		
		if ((it = allFiled.filedmap.find(g_szExData)) != allFiled.filedmap.end())
		{
			if (strlen(it->second.szCArgName) > 0)
			{
				rapidjson::Value v2(it->second.szCArgName, allocator);
				rapidjson::Value v3(FileRptPack.szExData, allocator);
				document.AddMember(v2, v3, allocator);
			}
		}

		//vector
		/*
		int nVecSize = allFiled.vecFiled.size();
		for (int i = 0; i < nVecSize; ++i)
		{
			rapidjson::Value v2(allFiled.vecFiled[i].szCArgName, allocator);
			rapidjson::Value v3(allFiled.vecFiled[i].szCArgValue, allocator);
			document.AddMember(v2, v3, allocator);
		}
		*/
		MakeFixedJsonValueToDoc(allFiled,document);

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);

		//可能会崩溃
		strOut = buffer.GetString();
	}
	catch (...)
	{

	}
}

void CParseToolImpl::MakeRptJsonNoRootData(int nRetCode, const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, string &strOut, const HSBC_URLINFO& urlinfo, bool bSign)
{//private
    std::vector<PACK_DELIVER>::const_iterator itFileRpt = FileRptPackArr.begin();
    cJSON *root, *obj;
    int nExcpLine = __LINE__;        //记录当前行
    char szBelong[BELONG_LEN + 1] = { 0 };
    FILEDMAP::const_iterator it;

    //存标签


    int nStatus = -1;
    string strErrDesc("");

    try
    {
        root = cJSON_CreateArray();
        if (NULL == root)
        {
            return;
        }

        for (; (itFileRpt != FileRptPackArr.end()); ++itFileRpt)
        {
            const PACK_DELIVER &FileRptPack = *itFileRpt;

            obj = cJSON_CreateObject();
            cJSON_AddItemToArray(root, obj);

            //确定成功和失败描述
            nStatus = _strnicmp((LPCSTR)FileRptPack.msgcontent.szState, "DELIVRD", 7) == 0 ? 0 : 2;
            if (0 == nStatus)
            {
                strErrDesc = RPTSTATUSDESC_OK;
            }
            else
            {
                strErrDesc = RPTSTATUSDESC_ERR;
            }

            //rpt对象
			if (PT_PUSH_IS_IN_FIELDMAP(g_szMsgId))
            {
                //msgid
				cJSON_AddNumber64ToObject(obj, it->second.szCArgName, m_global.TranMsgIdCharToI64(FileRptPack.szMsgId));
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szCustID))
            {
				cJSON_AddStringToObject(obj, it->second.szCArgName, FileRptPack.szCustId);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szPkNum))
            {
                //pkno
				cJSON_AddNumberToObject(obj, it->second.szCArgName, FileRptPack.nPknum);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szPkTotal))
            {
                //
				cJSON_AddNumberToObject(obj, it->second.szCArgName, FileRptPack.nPkTotal);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szMobile))
            {
                //mobile
				cJSON_AddStringToObject(obj, it->second.szCArgName, (char*)FileRptPack.msgcontent.szDestTerminalId);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szSpNo))
            {
                //spno
				cJSON_AddStringToObject(obj, it->second.szCArgName, (char*)FileRptPack.szDestId);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szExNo))
            {
                //exno
                char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
                if ('*' != FileRptPack.szSpNumEx[0])
                {
                    memcpy(szTmp, FileRptPack.szSpNumEx, strlen((char*)FileRptPack.szSpNumEx));
                }
				cJSON_AddStringToObject(obj, it->second.szCArgName, szTmp);
            }

			char szSendTime[25] = { 0 };
			char szReceiveTime[25] = { 0 };
			char szPushTime[25] = { 0 };
			SYSTEMTIME_EX tt;
			CAdapter::GetLocalTime(&tt);
			SYSTEMTIME_EX_LINUX t(tt);
			if (2 == urlinfo.nPushtimefmt)
			{
				int nYear_MsgId = 0, nMon_MsgId = 0, nDay_MsgId = 0, nHour_MsgId = 0, nMin_MsgId = 0, nSec_MsgId = 0;
				sscanf(FileRptPack.szSendTime, "%04d-%02d-%02d %02d:%02d:%02d",
				&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
				sprintf(szSendTime, "%04d%02d%02d%02d%02d%02d",
				nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);
				sscanf(FileRptPack.szTime, "%04d-%02d-%02d %02d:%02d:%02d",
				&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
				sprintf(szReceiveTime, "%04d%02d%02d%02d%02d%02d",
				nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);

				sprintf(szPushTime, "%04d%02d%02d%02d%02d%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
			}
			else
			{
				memcpy(szSendTime, FileRptPack.szSendTime, min(strlen(FileRptPack.szSendTime), sizeof(szSendTime)));
				memcpy(szReceiveTime, FileRptPack.szTime, min(strlen(FileRptPack.szTime), sizeof(szReceiveTime)));
				sprintf(szPushTime, "%04d-%02d-%02d %02d:%02d:%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
			}
			if (PT_PUSH_IS_IN_FIELDMAP(g_szSendTime))
            {
                //stime
				cJSON_AddStringToObject(obj, it->second.szCArgName, szSendTime);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szRecvTime))
            {
                //rtime
				cJSON_AddStringToObject(obj, it->second.szCArgName, szReceiveTime);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szPushTime))
            {
                //ptime
                if(!bSign)
                {
					cJSON_AddStringToObject(obj, it->second.szCArgName, szPushTime);
                }
			}
			if (PT_PUSH_IS_IN_FIELDMAP(g_szStatus))
            {
                //status
				cJSON_AddNumberToObject(obj, it->second.szCArgName, nStatus);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szErrCode))
            {
                //errcode
				cJSON_AddStringToObject(obj, it->second.szCArgName, (char*)FileRptPack.msgcontent.szState);
            }
			if (PT_PUSH_IS_IN_FIELDMAP(g_szErrCodeDesc))
            {
                //errcodedesc
                if(WBS_CARGTYPE_UPPER_STRING == it->second.nCargType)
				{
					//转换为大写
					strErrDesc = cstr::to_upper_s(strErrDesc.c_str());
				}
				cJSON_AddStringToObject(obj, it->second.szCArgName, strErrDesc.c_str());
			}
			if (PT_PUSH_IS_IN_FIELDMAP(g_szExData))
			{
				//exdata
				// 				string strExData("");
				// 				if (0 == strlen(FileRptPack.szExData))
				// 				{
				// 					strExData = "*";
				// 				}
				// 				else
				// 				{
				// 					strExData = FileRptPack.szExData;
				// 				}
				cJSON_AddStringToObject(obj, it->second.szCArgName, FileRptPack.szExData);
			}

			{
				int nVecSize = allFiled.vecFiled.size();
				for (int i = 0; i < nVecSize; ++i)
				{
					//zhubo 2017-12-12
					if (strlen(allFiled.vecFiled[i].szBelong) > 0
						&& 0 == allFiled.vecFiled[i].nBelongtype)
					{
						cJSON_AddStringToObject(obj, allFiled.vecFiled[i].szCArgName, allFiled.vecFiled[i].szCArgValue);

					}

				}
			}
        }

        strOut = cJSON_Print(root);
        //删除
        cJSON_Delete(root);
    }
    catch (...)
    {

    }
}


void CParseToolImpl::ParseResponse_std(const char* szInMsg, RSP_PACK &rsp)
{
    //解析推送MO、RPT的回应报文,json/xml
	char pchCmd[20] = { 0 };
	
	//默认全失败
	_snprintf(rsp.stat, sizeof(rsp.stat), "%d", 3);

	//对回应报文体进行初步校验
	if (strlen(szInMsg) <= 3)
	{
		return;	
	}
	
	try
	{
		if (XML == rsp.nRspFmt)
		{
			xml_document<>  doc;
			char *p = const_cast<char *>(szInMsg);
			try
			{
				doc.parse(p, rapidxml::parse_non_destructive);	// 此处必须传1，不能传0
			}
			catch (...)
			{
				return ;
			}

			xml_node<> *node = doc.first_node(g_szRptRsp);
			if (NULL == node)
			{
				node = doc.first_node(g_szMoRsp);
				if (NULL == node)
				{
					return;
				}
			}

			//不解析命令
			//CXML::parseXMLValueEx((char *)g_szCmd, pchCmd, sizeof(pchCmd)-1, node);

			//if (strcmp(pchCmd, "RPT_RESP") == 0)
			//{
			//	rsp.command = MULTIXRPT_RESPONSE;
			//}
			//else if (strcmp(pchCmd, "MO_RESP") == 0)
			//{
			//	rsp.command = MULTIXMO_RESPONSE;
			//}
			//else
			//{
			//	return;
			//}

			CXML::parseXMLValueEx((char *)g_szResult, rsp.stat, sizeof(rsp.stat) - 1, node);
			if (0 == strnicmp("0", rsp.stat, 1))
			{
				rsp.stat[0] = '2';
			}
			else
			{
				rsp.stat[0] = '3';
			}

		}
		else
		{
			//非xml的，全当作json
			Document doc;
			doc.Parse<0>(szInMsg);

			if (doc.HasParseError() || !doc.IsObject())
			{
				return;
			}

			//不解析命令
			//CJSON::parseJsonStrValue((char *)g_szCmd, pchCmd, sizeof(pchCmd)-1, doc);

			//if (strcmp(pchCmd, "RPT_RESP") == 0)
			//{
			//	rsp.command = MULTIXRPT_RESPONSE;
			//}
			//else if (strcmp(pchCmd, "MO_RESP") == 0)
			//{
			//	rsp.command = MULTIXMO_RESPONSE;
			//}
			//else
			//{
			//	return;
			//}

			int nErrResult = -1, nErrStatus = -1, nErrResCode = -1;

			int nResult = -1;
			int nStatus = -1;
			char szResCode[5] = {0};

			nErrResult = CJSON::parseJsonIntValue((char*)g_szResult, doc, nResult);
			nErrStatus = CJSON::parseJsonIntValue((char*)g_szStatus, doc, nStatus);
			nErrResCode = CJSON::parseJsonStrValue("resCode", szResCode, 3, doc);

			if (0 == nErrResult || 0 == nErrStatus || nErrResCode >= 0)
			{
				if (0 == nResult || 200 == nStatus || !_stricmp(szResCode, "00"))
				{
					nResult = 2;
				}
				else
				{
					nResult = 3;
				}
				_snprintf(rsp.stat, sizeof(rsp.stat), "%d", nResult);
			}
		}

	}
	catch (...)
	{
	}

	return;
}

void CParseToolImpl::ParseResponse_cstd(const char* pStart, const WBSTEMPLATEINFO &mapTmp, const RSP_CMD_PUSH_TMP &pushRsp,
												RSP_PACK &rsp, std::vector<INT64> &vSuccess, std::vector<INT64> &vFailed)
{
	//解析推送MO、RPT的回应报文 (模板)
	char pchCmd[20] = { 0 };
	__int64 iMsgId = 0;
	FILEDMAP::const_iterator it;

	string strTagMsgId("");
//	string strTagRptDetails("");
	string strTagMoRptDetails("");

	//默认全失败
	_snprintf(rsp.stat, sizeof(rsp.stat), "%d", 3);

	//对回应报文体进行初步校验
	if (strlen(pStart) <= 3)
	{
		return;	
	}

	try
	{
		if (URLENCODED == pushRsp.retMode)
		{
			//目前只解析V3.1单条推送回应协议，如果V3.0用户接入过来也按照这种方式解析
			//只解析回应结果值
			RSP_CMD_PUSH_PACK::const_iterator it = pushRsp.cmdRspPack.begin();
			int nRspType = ALL_FAIL_RSP; //只存在全成功，或者全失败
			bool bMatch = false;

			//得到字段值
			char szValue[50] = { 0 };
			m_global.GetCmdValueEx((char*)pStart, (char *)it->second.szCArgName, 50, szValue);
			string strResult(szValue);
			std::vector<string> vecStr;
			for (; it != pushRsp.cmdRspPack.end(); ++it)
			{
				//取出全成功的字段值
				if (ALL_SUCCESS_RSP == it->first)
				{
					//取出全成功字段的值
					CGlobal::SplitStrEx(it->second.szCArgValue, vecStr, ",", 1024);

					for (int i = 0; i < vecStr.size(); ++i)
					{
						if (vecStr[i] == strResult)
						{
							nRspType = ALL_SUCCESS_RSP;
							bMatch = true;
							break;
						}
					}
				}
				if (bMatch)
				{
					break;
				}
			}
			_snprintf(rsp.stat, sizeof(rsp.stat), "%d", nRspType);
		}
		else if (JSON == pushRsp.retMode)
        {
            Document doc;
            char *p = const_cast<char *>(pStart);
            doc.Parse<0>(p);

            if (doc.HasParseError() || !doc.IsObject())
            {
                return;
            }

            RSP_CMD_PUSH_PACK::const_iterator it = pushRsp.cmdRspPack.begin();
            bool bFind = false;
            bool bMatch = false;
            vector<string> vecStr;
            int nRspType = ALL_FAIL_RSP;

            for (; it != pushRsp.cmdRspPack.end(); ++it)
            {
                //遍历2、3、4、5
                //strCName = it->second.szCArgName;
                char szValue[MAX_RESULT_LEN + 1] = { 0 };
                int nErr = -1;
                int nResult = -1;
                string strResult("");
                if (doc.HasMember((char*)it->second.szCArgName))
                {
                    //存在key,则取值，然后匹配2、3、4、5
                    Value & v = doc[(char*)it->second.szCArgName];
                    if (v.IsString())
                    {
                        strResult = v.GetString();
						CGlobal::SplitStrEx(it->second.szCArgValue, vecStr, ",", 1024);
						for (int i = 0; i < vecStr.size(); ++i)
                        {
                            if (vecStr[i] == strResult)
                            {
                                nRspType = it->first;
                                bMatch = true;
                                break;
                            }
                        }
                    }
                    else if (v.IsInt())
                    {
                        //如果是整形
                        char szTmp[10] = { 0 };
                        nResult = v.GetInt();
						CGlobal::SplitStrEx(it->second.szCArgValue, vecStr, ",", 1024);
						CAdapter::itoa(nResult, szTmp, 10);
                        for (int i = 0; i < vecStr.size(); ++i)
                        {
                            if (0 == strcmp(szTmp, vecStr[i].c_str()))
                            {
                                nRspType = it->first;
                                bMatch = true;
                                break;
                            }
                        }
                    }

                    vecStr.clear();
                    if (bMatch)
                    {
                        break;
                    }
                }
            }

            _snprintf(rsp.stat, sizeof(rsp.stat), "%d", nRspType);
            //比较result值
            switch (nRspType)
            {
            case ALL_SUCCESS_RSP:
            case ALL_FAIL_RSP:
            {
                WBSTEMPLATEINFO::const_iterator itAll = mapTmp.find(nRspType);
                FILEDMAP::const_iterator it;
                if (itAll != mapTmp.end())
                {
	                //不解析命令
	                //const ALLFILED &allFiled = itAll->second;

	                //if ((it = allFiled.filedmap.find(g_szCmd)) != allFiled.filedmap.end())
	                //{
	                //	CJSON::parseJsonStrValue((char *)it->second.szCArgName, pchCmd, sizeof(pchCmd)-1, doc);
	                //}

	                //if (strcmp(pchCmd, "RPT_RESP") == 0)
	                //{
	                //	rsp.command = MULTIXRPT_RESPONSE;
	                //}
	                //else if (strcmp(pchCmd, "MO_RESP") == 0)
	                //{
	                //	rsp.command = MULTIXMO_RESPONSE;
	                //}
	                //else
	                //{
	                //	return;
	                //}
                }
                else
                {
                    //配置有问题
                }
            }
            break;
            case SUCCESS_FAIL_RSP:
            {
                //根据failedmap或者succesmap解析失败对象数组，遍历数组
                WBSTEMPLATEINFO::const_iterator itAll = mapTmp.find(nRspType);
                FILEDMAP::const_iterator it;
                FILEDMAP::const_iterator it2;

                if (itAll != mapTmp.end())
                {
                    const ALLFILED &allFiled = itAll->second;

                    //确定标签
                    //msgid
                    if ((it = allFiled.filedmap.find(g_szMsgId)) != allFiled.filedmap.end())
                    {
                        strTagMsgId = it->second.szCArgName;
                    }
                    else
                    {
                        strTagMsgId = g_szMsgId;
                    }

						//不解析命令
						//if ((it = allFiled.filedmap.find(g_szCmd)) != allFiled.filedmap.end())
						//{
						//	CJSON::parseJsonStrValue((char *)it->second.szCArgName, pchCmd, sizeof(pchCmd)-1, doc);
						//}

						//if (strcmp(pchCmd, "RPT_RESP") == 0)
						//{
						//	rsp.command = MULTIXRPT_RESPONSE;
						//	strTagMoRptDetails = g_szRptRspDetails;
						//}
						//else if (strcmp(pchCmd, "MO_RESP") == 0)
						//{
						//	rsp.command = MULTIXMO_RESPONSE;
						//	strTagMoRptDetails = g_szMoRspDetails;
						//}
						//else
						//{
						//	return;
						//}

						if (MULTIXRPT_RESPONSE == rsp.command)
						{
							strTagMoRptDetails = g_szRptRspDetails;
						}
						else if (MULTIXMO_RESPONSE == rsp.command)
						{
							strTagMoRptDetails = g_szMoRspDetails;
						}
                    else
                    {
                        return;
                    }

                    //failedmap
                    if ((it = allFiled.filedmap.find(g_szFailMap)) != allFiled.filedmap.end())
                    {
                        //存在失败数组
                        if (doc.HasMember(it->second.szCArgName))
                        {
                            Value &v = doc[it->second.szCArgName];
                            if (!v.IsArray())
                            {
                                return;
                            }

                            int nSize = v.Size();
                            for (int i = 0; i < nSize; ++i)
                            {
                                Value &v2 = v[i];
                                if (!v2.IsObject())
                                {
                                    return;
                                }

                                if ((it2 = allFiled.filedmap.find(strTagMoRptDetails)) != allFiled.filedmap.end())
                                {
                                    //if (doc.HasMember(it2->second.szCArgName))
                                    if (v2.HasMember(it2->second.szCArgName))
                                    {
                                        Value &v3 = v2[it2->second.szCArgName];
                                        if (!v3.IsObject())
                                        {
                                            return;
                                        }

                                        if (CJSON::parseJsonInt64Value((char*)strTagMsgId.c_str(), v3, iMsgId) != -1)
                                        {
                                            vFailed.push_back(iMsgId);
                                        }
                                    }
                                }
                                //else
                                {
                                    if (CJSON::parseJsonInt64Value((char*)strTagMsgId.c_str(), v2, iMsgId) != -1)
                                    {
                                        vFailed.push_back(iMsgId);
                                    }
                                }
                            }
                        }

                    }

                    //succmap
                    if ((it = allFiled.filedmap.find(g_szSuccesMap)) != allFiled.filedmap.end())
                    {
                        //存在失败数组
                        if (doc.HasMember(it->second.szCArgName))
                        {
                            Value &v = doc[it->second.szCArgName];
                            if (!v.IsArray())
                            {
                                return;
                            }

                            int nSize = v.Size();
                            for (int i = 0; i < nSize; ++i)
                            {
                                Value &v2 = v[i];
                                if (!v2.IsObject())
                                {
                                    return;
                                }

                                if ((it2 = allFiled.filedmap.find(strTagMoRptDetails)) != allFiled.filedmap.end())
                                {
                                    if (v2.HasMember(it2->second.szCArgName))
                                    {
                                        Value &v3 = v2[it2->second.szCArgName];
                                        if (!v3.IsObject())
                                        {
                                            return;
                                        }

                                        if (CJSON::parseJsonInt64Value((char*)strTagMsgId.c_str(), v3, iMsgId) != -1)
                                        {
                                            vSuccess.push_back(iMsgId);
                                        }
                                    }
                                }
                                //else
                                {
                                    if (CJSON::parseJsonInt64Value((char*)strTagMsgId.c_str(), v2, iMsgId) != -1)
                                    {
                                        vSuccess.push_back(iMsgId);
                                    }
                                }
                            }
                        }

                    }
                }
                else
                {
                    //配置有问题
                }
            }
            break;
            case DETAILS_RSP:
            {

            }
            break;
            default:
            break;
            }
        }
        else if (XML == pushRsp.retMode)
        {
            xml_document<> doc;
            xml_node<> *root;
            xml_node<> *node1;
            char szMsgIdTmp[MAX_RESULT_LEN + 1] = { 0 };

            char *p = const_cast<char *>(pStart);
            doc.parse(p, 1);	// 此处必须传1，不能传0

            RSP_CMD_PUSH_PACK::const_iterator it = pushRsp.cmdRspPack.begin();
            bool bFind = false;
            bool bMatch = false;
            vector<string> vecStr;
            int nRspType = ALL_FAIL_RSP;

            for (; it != pushRsp.cmdRspPack.end(); ++it)
            {
                //遍历2、3、4、5
                char szValue[MAX_RESULT_LEN + 1] = { 0 };

                node1 = doc.first_node();
                //跳过根节点
                //node1 = root->next_sibling();
                if (NULL != node1)
                {
                    CXML::parseXMLValueEx((char*)it->second.szCArgName, szValue, MAX_RESULT_LEN, node1);
					CGlobal::SplitStrEx(it->second.szCArgValue, vecStr, ",", 1024);
                    for (int i = 0; i < vecStr.size(); ++i)
                    {
                        if (0 == strcmp(szValue, vecStr[i].c_str()))
                        {
                            nRspType = it->first;
                            bMatch = true;
                            break;
                        }
                    }
                }

                vecStr.clear();
                if (bMatch)
                {
                    break;
                }
            }

            _snprintf(rsp.stat, sizeof(rsp.stat), "%d", nRspType);

            switch (nRspType)
            {
            case ALL_SUCCESS_RSP:
            case ALL_FAIL_RSP:
            {
                WBSTEMPLATEINFO::const_iterator itAll = mapTmp.find(nRspType);
                FILEDMAP::const_iterator it;
                if (itAll != mapTmp.end())
                {
					//不解析命令
					//const ALLFILED &allFiled = itAll->second;

					//if ((it = allFiled.filedmap.find(g_szCmd)) != allFiled.filedmap.end())
					//{
					//	CXML::parseXMLValueEx((char *)it->second.szCArgName, pchCmd, sizeof(pchCmd)-1, node1);
					//}

					//if (strcmp(pchCmd, "RPT_RESP") == 0)
					//{
					//	rsp.command = MULTIXRPT_RESPONSE;
					//}
					//else if (strcmp(pchCmd, "MO_RESP") == 0)
					//{
					//	rsp.command = MULTIXMO_RESPONSE;
					//}
					//else
					//{
					//	return;
					//}
                }
                else
                {
                    //配置有问题
                }
            }
            break;
            case SUCCESS_FAIL_RSP:
            {
                //根据failedmap或者succesmap解析失败对象数组，遍历数组
                WBSTEMPLATEINFO::const_iterator itAll = mapTmp.find(nRspType);
                FILEDMAP::const_iterator it;
                string strTagNext("");

                if (itAll != mapTmp.end())
                {
                    const ALLFILED &allFiled = itAll->second;

                    //确定标签
                    //msgid
                    if ((it = allFiled.filedmap.find(g_szMsgId)) != allFiled.filedmap.end())
                    {
                        strTagMsgId = it->second.szCArgName;
                    }
                    else
                    {
                        strTagMsgId = g_szMsgId;
                    }

					//不解析命令
					//if ((it = allFiled.filedmap.find(g_szCmd)) != allFiled.filedmap.end())
					//{
					//	CXML::parseXMLValueEx((char *)it->second.szCArgName, pchCmd, sizeof(pchCmd)-1, node1);
					//}

					//if (strcmp(pchCmd, "RPT_RESP") == 0)
					//{
					//	strTagMoRptDetails = g_szRptRspDetails;
					//	rsp.command = MULTIXRPT_RESPONSE;
					//}
					//else if (strcmp(pchCmd, "MO_RESP") == 0)
					//{
					//	strTagMoRptDetails = g_szMoRspDetails;
					//	rsp.command = MULTIXMO_RESPONSE;
					//}
					//else
					//{
					//	return;
					//}
					
					if (MULTIXRPT_RESPONSE == rsp.command)
					{
						strTagMoRptDetails = g_szRptRspDetails;
					}
					else if (MULTIXMO_RESPONSE == rsp.command)
					{
						strTagMoRptDetails = g_szMoRspDetails;
					}
					else
					{
						return;
					}

                    //rpt/modetails
                    if ((it = allFiled.filedmap.find(strTagMoRptDetails)) != allFiled.filedmap.end())
                    {
                        strTagMoRptDetails = it->second.szCArgName;
                    }

                    xml_node<> *nodeMsgId = NULL;
                    xml_node<> *nodeDetails = NULL;
                    //failedmap
                    if ((it = allFiled.filedmap.find(g_szFailMap)) != allFiled.filedmap.end())
                    {
                        //存在失败数组
                        xml_node<> *node2 = node1->first_node(it->second.szCArgName);
                        if (NULL != node2)
                        {
                            nodeDetails = node2->first_node(strTagMoRptDetails.c_str());
                            if (NULL == nodeDetails)
                            {
                                nodeMsgId = node2->first_node(strTagMsgId.c_str());
                            }
                        }
                        else
                        {
                            return;
                        }

                        if (NULL != nodeMsgId)
                        {
                            do
                            {
                                //if (CXML::parseXMLValueEx((char*)strTagMsgId.c_str(), szMsgIdTmp, MAX_RESULT_LEN, nodeMsgId))
                                if (CXML::getXMLValue((char*)strTagMsgId.c_str(), szMsgIdTmp, MAX_RESULT_LEN, nodeMsgId))
                                {
                                    vFailed.push_back(_atoi64(szMsgIdTmp));
                                }

                            } while (nodeMsgId = nodeMsgId->next_sibling(strTagMsgId.c_str()));
                        }

                        if (NULL != nodeDetails)
                        {
                            do
                            {
                                if (CXML::parseXMLValueEx((char*)strTagMsgId.c_str(), szMsgIdTmp, MAX_RESULT_LEN, nodeDetails))
                                {
                                    vFailed.push_back(_atoi64(szMsgIdTmp));
                                }

                            } while (nodeDetails = nodeDetails->next_sibling(strTagMoRptDetails.c_str()));
                        }

                    }

                    //succmap
                    if ((it = allFiled.filedmap.find(g_szSuccesMap)) != allFiled.filedmap.end())
                    {
                        xml_node<> *node2 = node1->first_node(it->second.szCArgName);
                        if (NULL != node2)
                        {
                            nodeDetails = node2->first_node(strTagMoRptDetails.c_str());
                            if (NULL == nodeDetails)
                            {
                                nodeMsgId = node2->first_node(strTagMsgId.c_str());
                            }
                        }
                        else
                        {
                            return;
                        }

                        if (NULL != nodeMsgId)
                        {
                            do
                            {
                                //if (CXML::parseXMLValueEx((char*)strTagMsgId.c_str(), szMsgIdTmp, MAX_RESULT_LEN, nodeMsgId))
                                if (CXML::getXMLValue((char*)strTagMsgId.c_str(), szMsgIdTmp, MAX_RESULT_LEN, nodeMsgId))
                                {
                                    vSuccess.push_back(_atoi64(szMsgIdTmp));
                                }

                            } while (nodeMsgId = nodeMsgId->next_sibling(strTagMsgId.c_str()));
                        }

                        if (NULL != nodeDetails)
                        {
                            do
                            {
                                if (CXML::parseXMLValueEx((char*)strTagMsgId.c_str(), szMsgIdTmp, MAX_RESULT_LEN, nodeDetails))
                                {
                                    vSuccess.push_back(_atoi64(szMsgIdTmp));
                                }

                            } while (nodeDetails = nodeDetails->next_sibling(strTagMoRptDetails.c_str()));
                        }
                    }
                }
                else
                {
                    //配置有问题
                }
            }
            break;
            case DETAILS_RSP:
            {

            }
            break;
            default:
            break;
            }
        }
        else
        {
            //错误
        }

        return;
    }
    catch (...)
    {
        return;
    }
}

bool CParseToolImpl::CheckArgLegal(const char* szCustid, const char* szExdata)
{//private
    bool bRet = true;
    if (strlen(szCustid) > MAX_CUSTID_LEN || strlen(szExdata) > MAX_EXDATA_LEN)
    {
        return bRet = false;
    }

    //字母，数字，下划线，减号
    char chC = '\0';
    char chE = '\0';
    while ((chC = *szCustid++) != '\0')
    {
        if ((chC >= 'a' && chC <= 'z') || (chC >= 'A' && chC <= 'Z')
            || (chC >= '0' && chC <= '9') || '_' == chC || '-' == chC)
        {
            bRet = true;
        }
        else
        {
            return bRet = false;
        }
    }

    while ((chE = *szExdata++) != '\0')
    {
        if ((chE >= 'a' && chE <= 'z') || (chE >= 'A' && chE <= 'Z')
            || (chE >= '0' && chE <= '9') || '_' == chE || '-' == chE)
        {
            bRet = true;
        }
        else
        {
            return bRet = false;
        }
    }
    return bRet;
}

//解析Http-post请求的内容参数以&符号分开的这种形式
int CParseToolImpl::GetCmdValueEx(char *Data, char *CmdItem, int MaxSize, char *CmdVal, BOOL bConvert)
{
    char *p1 = NULL;
    char *p2 = NULL;
    char *p3 = NULL;
    char find[256] = { 0 };

    _snprintf(find, sizeof(find)-1, "%s=", CmdItem);
    //strcat(find, CmdItem);
    //strcat(find, "=");
    p1 = CGlobal::strstri(Data, find);
    if (NULL == p1)
    {
        return -1;
    }
    p2 = p1 + strlen(find);
    // 	memset(find, 0, sizeof(find));
    // 	strcat(find, "=");
    // 	p2 = StrStrI(p1, find);
    // 	if (NULL == p2)
    // 	{
    // 		return -1;
    // 	}
    p1 = p2;

    memset(find, 0, sizeof(find));
    find[0] = '&';
    //strcpy(find, "&");

    p2 = CGlobal::strstri(p1, find);
    if (NULL == p2)
    {
        p2 = &Data[strlen(Data)];
    }

    //截取到的长度，超出最大值，或为负数则放弃之
    if (p2 - p1 > MaxSize || p2 - p1 < 0)
    {
        return -2;
    }

    int nCmdValueLen = 0;
    if (bConvert)
    {
        int nOutLen = p2 - p1;
        char *szTmp = new char[nOutLen + 1];
        if (NULL != szTmp)
        {
            try
            {
                memset(szTmp, 0, nOutLen + 1);
                m_global.DecodeHtmlStr(szTmp, p1, nOutLen);
                if (nOutLen > (unsigned int)MaxSize)
                {
                    if (NULL != szTmp)
                    {
                        delete[] szTmp;
                        szTmp = NULL;
                    }
                    return -2;
                }
                strcpy(CmdVal, szTmp);
                //UTF_8ToGB2312(CmdVal, szTmp, nOutLen, MaxSize);
            }
            catch (...)
            {
            }
        }
        if (NULL != szTmp)
        {
            delete[] szTmp;
            szTmp = NULL;
        }
        nCmdValueLen = strlen(CmdVal);
    }
    else
    {
        if (p2 - p1 > MaxSize)
        {
            return -2;
        }
        memcpy(CmdVal, p1, p2 - p1);
        CmdVal[p2 - p1] = 0x0;
        nCmdValueLen = p2 - p1;
    }
    return nCmdValueLen;/*strlen(CmdVal);*/
}

//解析Http-post请求的内容参数以&符号分开的这种形式
int CParseToolImpl::GetMultiCmdValue(char *Data, char *CmdItem, int MaxSize, char *CmdVal, BOOL bConvert)
{
    char *p1 = NULL;
    char *p2 = NULL;
    char find[256] = { 0 };

    strcat(find, CmdItem);
    strcat(find, "=");
    p1 = CGlobal::strstri(Data, find);
    if (NULL == p1)
    {
        return -1;
    }
    p2 = p1 + strlen(find);
    //  memset(find, 0, sizeof(find));
    //  strcat(find, "=");
    //  p2 = CAdapter::StrStrI(p1, find);
    //  if (NULL == p2)
    //  {
    //      return -1;
    //  }
    p1 = p2;

    memset(find, 0, sizeof(find));
    strcpy(find, "&");

    p2 = CGlobal::strstri(p1, find);
    if (NULL == p2)
    {
        p2 = &Data[strlen(Data)];
    }

    int nCmdValueLen = 0;
    if (bConvert)
    {
        int nOutLen = p2 - p1;
        char *szTmp = new char[nOutLen + 1];
        if (NULL != szTmp)
        {
            try
            {
                memset(szTmp, 0, nOutLen + 1);
                if (p1[0] == '%')
                {
                    //对整体multimt字段进行urlencode
                    m_global.DecodeHtmlStr(szTmp, p1, nOutLen);
                    if (nOutLen > (unsigned int)MaxSize)
                    {
                        if (NULL != szTmp)
                        {
                            delete[] szTmp;
                            szTmp = NULL;
                        }
                        return -2;
                    }
                    strcpy(CmdVal, szTmp);
                }
                else
                {
                    if (nOutLen > (unsigned int)MaxSize)
                    {
                        nOutLen = MaxSize;

                    }
                    memcpy(CmdVal, p1, nOutLen);
                }
            }
            catch (...)
            {
            }
        }
        if (NULL != szTmp)
        {
            delete[] szTmp;
            szTmp = NULL;
        }
        nCmdValueLen = nOutLen;
    }
    else
    {
        if (p2 - p1 > MaxSize)
        {
            return -2;
        }
        memcpy(CmdVal, p1, p2 - p1);
        CmdVal[p2 - p1] = 0x0;
        nCmdValueLen = p2 - p1;
    }
    return nCmdValueLen;/*strlen(CmdVal);*/
}

// 获取国美在线的账号和密码。
void CParseToolImpl::GetNameForConfigFile()
{
    char szTmp[MAX_PATH * 3] = { 0 };
    char szPath[MAX_PATH] = { 0 };
    char szWbsLocalName[MAX_PATH] = { 0 };
	CAdapter::GetCurrentPath(szPath, MAX_PATH);
    strcat(szPath, "wbsconf.ini");  //SP网关程序
    string strPath(szPath);

	if (CAdapter::PathFileExists(szPath))
    {
        //GOMENAME
        //mo
        CAdapter::GetPrivateProfileString("GOME", "GOMEMONAME", "", szTmp, MAX_PATH * 3, szPath);

        m_strMOName = szTmp;

        //GOMEPWD
        memset(szTmp, 0, sizeof(szTmp));
        CAdapter::GetPrivateProfileString("GOME", "GOMEMOPWD", "", szTmp, MAX_PATH * 3, szPath);
        m_strMOPwd = szTmp;

        //GOMERPTNAME
        memset(szTmp, 0, sizeof(szTmp));
        CAdapter::GetPrivateProfileString("GOME", "GOMERPTNAMEBUS", "", szTmp, MAX_PATH * 3, szPath);
        m_strRPTNameBus = szTmp;


        //GOMEPWD
        memset(szTmp, 0, sizeof(szTmp));
        CAdapter::GetPrivateProfileString("GOME", "GOMERPTPWDBUS", "", szTmp, MAX_PATH * 3, szPath);
        m_strRPTPwdBus = szTmp;

        //GOMERPTNAME
        memset(szTmp, 0, sizeof(szTmp));
        CAdapter::GetPrivateProfileString("GOME", "GOMERPTNAME", "", szTmp, MAX_PATH * 3, szPath);
        m_strRPTName = szTmp;

        //GOMERPTPWD
        memset(szTmp, 0, sizeof(szTmp));
        CAdapter::GetPrivateProfileString("GOME", "GOMERPTPWD", "", szTmp, MAX_PATH * 3, szPath);
        m_strRPTPwd = szTmp;

        //RPTURL
        memset(szTmp, 0, sizeof(szTmp));
        CAdapter::GetPrivateProfileString("GOME", "GOMERPTURL", "", szTmp, MAX_PATH * 3, szPath);
        m_strRPTURL_Gome = szTmp;

        //RPTURL_bus
        memset(szTmp, 0, sizeof(szTmp));
        CAdapter::GetPrivateProfileString("GOME", "GOMERPTURLBUS", "", szTmp, MAX_PATH * 3, szPath);
        m_strRPTURL_Gome_BUS = szTmp;

        //MOURL
        memset(szTmp, 0, sizeof(szTmp));
        CAdapter::GetPrivateProfileString("GOME", "GOMEMOURL", "", szTmp, MAX_PATH * 3, szPath);
        m_strMOURL_Gome = szTmp;

        //MOURL_bus
        memset(szTmp, 0, sizeof(szTmp));
        CAdapter::GetPrivateProfileString("GOME", "GOMEMOURLBUS", "", szTmp, MAX_PATH * 3, szPath);
        m_strMOURL_Gome_BUS = szTmp;

		//艺龙账号		
		memset(szTmp, 0, sizeof(szTmp));
		CAdapter::GetPrivateProfileString("GOME", "YLPDNAME", "", szTmp, MAX_PATH * 3, szPath);
		m_vcYLName.clear();
		CGlobal::SplitStrEx(szTmp, m_vcYLName, ",", 128);
		
    }
}

bool CParseToolImpl::IsEnglishMsg(const char * szMsg, int nLen)
{
    bool bRet = true;
    //判断是否是ASCII
    int i = 0;
    while (i < nLen)
    {
        bRet = (0 == szMsg[i] && isascii(szMsg[i + 1]) != 0);
        if (!bRet)
        {
            break;
        }
        i += 2;
    }

    return bRet;
}


bool CParseToolImpl::IsAllNumber(const char*szIn)
{//private
    int nLen = strlen(szIn);
    if (nLen > 6)
    {
        return false;
    }

    for (int i = 0; i < nLen; ++i)
    {
        if (szIn[i] > '9' || szIn[i] < '0')
        {
            return false;
        }
    }

    return true;
}




BOOL CParseToolImpl::ToUTF8(char* szSrc, char* pBuf, int cbBufLen)
{
#if 1
    string strOut("");
    int nMaxOutLen = 0;
    iconvpp::converter  convert(ICONV_GBK, ICONV_UTF_8, nMaxOutLen);
    convert.convert(szSrc, strOut);
    if ((strOut.size() + 1) > cbBufLen)
    {
        return FALSE;
    }
    memcpy(pBuf, strOut.c_str(), strOut.size() + 1);
    pBuf[strOut.size()] = '\0';
#else
    if (szSrc == NULL || pBuf == NULL || cbBufLen <= 0)
        return FALSE;

    size_t len_ascii = strlen(szSrc);
    if (len_ascii == 0)
    {
        pBuf[0] = 0;
        return TRUE;
    }

    //先转换到UTF-8
    int cchWideChar = MultiByteToWideChar(CP_ACP, 0, szSrc, len_ascii, NULL, 0);
    LPWSTR pUnicode = (LPWSTR)malloc((cchWideChar + 1) * sizeof(WCHAR));
    if (pUnicode == NULL)
        return FALSE;
    MultiByteToWideChar(CP_ACP, 0, szSrc, len_ascii, pUnicode, cchWideChar + 1);

    int cbUTF8 = WideCharToMultiByte(CP_UTF8, 0, pUnicode, cchWideChar, NULL, 0, NULL, NULL);
    LPSTR pUTF8 = (LPSTR)malloc((cbUTF8 + 1) * sizeof(CHAR));
    if (pUTF8 == NULL)
    {
        free(pUnicode);
        return FALSE;
    }
    WideCharToMultiByte(CP_UTF8, 0, pUnicode, cchWideChar, pUTF8, cbUTF8 + 1, NULL, NULL);
    pUTF8[cbUTF8] = '\0';
    memcpy(pBuf, pUTF8, MIN2(strlen(pUTF8), cbBufLen - 1));

    free(pUnicode);
    free(pUTF8);

#endif

    return TRUE;
}



//动态加载配置
void CParseToolImpl::LoadConfig(int nType)
{
    try
    {
        if (GOME_USERID == nType)
        {
            GetNameForConfigFile();
        }
    }
    catch (...)
    {
    }
}

int CParseToolImpl::MakeMoRequestV32_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, string &strMo)
{
	int nRet = 0;
	try
	{
		//组推送mo请求包，http post
		char szBody[MAX_PUSHMO_LEN - MAX_HTTP_HEAD - 10] = { 0 };
		char szSendBuf[MAX_PUSHMO_LEN] = { 0 };
		int nBodyLen = 0;
		int nOutMsgLen = 0;
		nBodyLen = _snprintf(szBody, sizeof(szBody), "[");

		std::vector<PACK_DELIVER>::const_iterator it = vMoRptPack.begin();
		//组包体中变化的内容
		for (it; it != vMoRptPack.end(); ++it)
		{
			char szReceiveTime[25] = { 0 };
			if (2 == urlinfo.nPushtimefmt)
			{
				int nYear_MsgId = 0, nMon_MsgId = 0, nDay_MsgId = 0, nHour_MsgId = 0, nMin_MsgId = 0, nSec_MsgId = 0;
				sscanf(it->szTime, "%04d-%02d-%02d %02d:%02d:%02d",
					&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
				sprintf(szReceiveTime, "%04d%02d%02d%02d%02d%02d",
					nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);
			}
			else
			{
				memcpy(szReceiveTime, it->szTime, min(strlen(it->szTime), sizeof(szReceiveTime)-1));
			}

			//采用urlencode加密方式
			std::string strMsgTmp = CGlobal::UrlEncode((char*)it->szMsgContent);

			char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
			if ('*' != it->szSpNumEx[0])
			{
				memcpy(szTmp, it->szSpNumEx, strlen((char*)it->szSpNumEx));
			}

			int nRetLen = _snprintf(szBody + nBodyLen, sizeof(szBody)-nBodyLen, JSON_MO_RSP_BODY,
				CGlobal::TranMsgIdCharToI64(it->szMsgId),
				it->szSrcTerminalId,
				it->szDestId,
				szTmp,
				szReceiveTime,
				strMsgTmp.c_str()
				);
			nBodyLen += nRetLen;
		}

		//去掉最后一个逗号
		if (nBodyLen > 0 && ',' == szBody[nBodyLen - 1])
		{
			szBody[nBodyLen - 1] = ']';
		}

		nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
			"POST %s HTTP/1.1\r\n"
			"Host: %s:%d\r\n"
			"Connection: Close\r\n"
			"Content-Type: application/json\r\n"
			"Content-Length: %d\r\n\r\n"
			"%s",
			urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, nBodyLen, szBody);

		strMo = szSendBuf;

		nRet = nOutMsgLen;
	}
	catch (...)
	{
		nRet = -1;
	}

	return nRet;
}

int CParseToolImpl::MakeRptRequestV32_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, string &strRpt)
{
	if (vMoRptPack.empty())
	{
		return 0;
	}

	char szBody[MAX_PUSHRPT_LEN] = { 0 };
	char szSendBuf[MAX_PUSHRPT_LEN] = { 0 };
	int nBodyLen = 0;
	int nOutMsgLen = 0;
	string strErrDesc("");
	int nStatus = -1;

	try
	{

		nBodyLen = _snprintf(szBody, sizeof(szBody), "[");

		std::vector<PACK_DELIVER>::const_iterator it = vMoRptPack.begin();
		//组包体中变化的内容
		for (it; it != vMoRptPack.end(); ++it)
		{
			char szTmp[MAX_CHANNEL_LEN + 1] = { 0 };
			if ('*' != it->szSpNumEx[0])
			{
				memcpy(szTmp, it->szSpNumEx, strlen((char*)it->szSpNumEx));
			}

			//确定成功和失败描述
			nStatus = _strnicmp((LPCSTR)it->msgcontent.szState, "DELIVRD", 7) == 0 ? 0 : 2;
			if (0 == nStatus)
			{
				strErrDesc = RPTSTATUSDESC_OK;
			}
			else
			{
				strErrDesc = RPTSTATUSDESC_ERR;
			}

			char szSendTime[25] = { 0 };
			char szReceiveTime[25] = { 0 };
			char szSendElongTime[25] = { 0 };
			SYSTEMTIME_EX tt;
            CAdapter::GetLocalTime(&tt);
            SYSTEMTIME_EX_LINUX t(tt);
			
			if (2 == urlinfo.nPushtimefmt)
			{
				int nYear_MsgId = 0, nMon_MsgId = 0, nDay_MsgId = 0, nHour_MsgId = 0, nMin_MsgId = 0, nSec_MsgId = 0;
				sscanf(it->szSendTime, "%04d-%02d-%02d %02d:%02d:%02d",
					&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
				sprintf(szSendTime, "%04d%02d%02d%02d%02d%02d",
					nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);
				sscanf(it->szTime, "%04d-%02d-%02d %02d:%02d:%02d",
					&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
				sprintf(szReceiveTime, "%04d%02d%02d%02d%02d%02d",
					nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);
				
				sprintf(szSendElongTime, "%04d%02d%02d%02d%02d%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
			}
			else
			{
				memcpy(szSendTime, it->szSendTime, min(strlen(it->szSendTime), sizeof(szSendTime)));
				memcpy(szReceiveTime, it->szTime, min(strlen(it->szTime), sizeof(szReceiveTime)));
				sprintf(szSendElongTime, "%04d-%02d-%02d %02d:%02d:%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
			}

			int nRetLen = _snprintf(szBody + nBodyLen, sizeof(szBody)-nBodyLen, JSON_RPT_RSP_BODY_V32,
				CGlobal::TranMsgIdCharToI64(it->szMsgId),
				it->szCustId,
				it->nPknum,
				it->nPkTotal,
				it->msgcontent.szDestTerminalId,
				it->szDestId,
				szTmp,
				szSendTime,
				szReceiveTime,
				szSendElongTime,
				nStatus,
				it->msgcontent.szState,
				strErrDesc.c_str(),
				it->szExData
				);
			nBodyLen += nRetLen;
		}

		//去掉最后一个逗号
		if (nBodyLen > 0 && ',' == szBody[nBodyLen - 1])
		{
			szBody[nBodyLen - 1] = ']';
		}


		nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
			"POST %s HTTP/1.1\r\n"
			"Host: %s:%d\r\n"
			"Connection: Close\r\n"
			"Content-Type: application/json\r\n"
			"Content-Length: %d\r\n\r\n"
			"%s",
			urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, nBodyLen, szBody);

		strRpt = szSendBuf;
	}
	catch (...)
	{
		nOutMsgLen = -1;
	}

	return nOutMsgLen;
}

int CParseToolImpl::MakeMoRequestV32_cstd(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strMo)
{
	//组mo推送报文，模板方式
	if (vMoRptPack.empty())
	{
		return 0;
	}

	int nOutMsgLen = 0;
	char szSendBuf[MAX_PUSHMO_LEN] = { 0 };
	char szTmp[MAX_PUSH_PRE_LEN] = { 0 };
	int nTmp = 0;

	try
	{
		MakePushMoJsonData_V32(allFiled, vMoRptPack, urlinfo, strMo);
		nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
			"POST %s HTTP/1.1\r\n"
			"Host: %s:%d\r\n"
			"Connection: Close\r\n"
			"Content-Type: application/json\r\n"
			"Content-Length: %d\r\n\r\n%s",
			urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, strMo.length(), strMo.c_str());

		strMo = szSendBuf;

	}
	catch (...)
	{
		nOutMsgLen = -1;
	}

	return nOutMsgLen;
}

int CParseToolImpl::MakeRptRequestV32_cstd(const std::vector<PACK_DELIVER> &FileRptPackArr, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strRpt)
{
	int nOutMsgLen = 0;
	char szSendBuf[MAX_PUSHRPT_LEN] = { 0 };
	int nTmp = 0;

	try
	{
		//默认走json
		MakePushRptJsonData_V32(allFiled, FileRptPackArr, urlinfo, strRpt);
		nOutMsgLen = _snprintf(szSendBuf, sizeof(szSendBuf),
			"POST %s HTTP/1.1\r\n"
			"Host: %s:%d\r\n"
			"Connection: Close\r\n"
			"Content-Type: application/json\r\n"
			"Content-Length: %d\r\n\r\n%s",
			urlinfo.szWebPath, urlinfo.szDomain, urlinfo.nPort, strRpt.length(), strRpt.c_str());

		strRpt = szSendBuf;
	}
	catch (...)
	{
		nOutMsgLen = -1;
	}

	return nOutMsgLen;
}

void CParseToolImpl::MakePushMoJsonData_V32(const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileMoPackArr, const HSBC_URLINFO& urlinfo, string &strOut)
{
	//组mo推送报文，模板方式
	if (FileMoPackArr.empty())
	{
		return;
	}

	int nOutMsgLen = 0;
	string strTagMos = "mos";

	try
	{
		std::vector<PACK_DELIVER>::const_iterator itFileMo = FileMoPackArr.begin();
		int nExcpLine = __LINE__;        //记录当前行
		rapidjson::Value array(rapidjson::kArrayType);
		rapidjson::Document document;
		rapidjson::Value v2, v3;
		char szBelong[BELONG_LEN + 1] = { 0 };
		document.SetObject();
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
		rapidjson::Value objectPre(rapidjson::kObjectType);
		FILEDMAP::const_iterator it;

		for (; (itFileMo != FileMoPackArr.end()); ++itFileMo)
		{
			const PACK_DELIVER &FileRptPack = *itFileMo;
			rapidjson::Value object(rapidjson::kObjectType);

			//useridex
			if ((it = allFiled.filedmap.find(g_szUesrIdEx)) != allFiled.filedmap.end())
			{
				if (0 != strlen(it->second.szCArgName))
				{
					//无子节点,无父节点，而且跟梦网标准字段有对应
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3(urlinfo.szMoPwd, allocator);
					object.AddMember(v2, v3, allocator);
				}
			}

			//signex
			if ((it = allFiled.filedmap.find(g_szSign)) != allFiled.filedmap.end())
			{
				if (0 != strlen(it->second.szCArgName))
				{
					//无子节点,无父节点，而且跟梦网标准字段有对应
					rapidjson::Value v2(it->second.szCArgName, allocator);
					char szUtf8[128] = {0};
					string strSrc = (char*)FileRptPack.szSignature;
					//去左右[]【】
					strSrc.erase(0,strSrc.find_first_not_of("["));
					strSrc.erase(0,strSrc.find_first_not_of("【"));
					strSrc.erase(strSrc.find_last_not_of("]") + 1);
					strSrc.erase(strSrc.find_last_not_of("】") + 1);
					ToUTF8((char*)strSrc.c_str(), szUtf8, 128);
					rapidjson::Value v3(szUtf8, allocator);
					object.AddMember(v2, v3, allocator);
				}
			}
		
			if ((it = allFiled.filedmap.find(g_szMsgId)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					//misgid
					if(WBS_CARGTYPE_OTHER == it->second.nCargType)
					{
						int64_t nMsgId = m_global.TranMsgIdCharToI64(FileRptPack.szMsgId);
						char szMsgIdTmp[64+1] = {0};
						CAdapter::_i64toa(nMsgId, szMsgIdTmp, 10);
						rapidjson::Value v2(it->second.szCArgName, allocator);
						rapidjson::Value v3(szMsgIdTmp, allocator);
						object.AddMember(v2, v3, allocator);				
					}
					else
					{
						rapidjson::Value v2(it->second.szCArgName, allocator);
						object.AddMember(v2, (int64_t)m_global.TranMsgIdCharToI64(FileRptPack.szMsgId), allocator);
					}
				}
			}
			if ((it = allFiled.filedmap.find(g_szMobile)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					//mobile
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3((char*)FileRptPack.szSrcTerminalId, allocator);
					object.AddMember(v2, v3, allocator);
				}
			}

			if ((it = allFiled.filedmap.find(g_szSpNo)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					//spno
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3((char*)FileRptPack.szDestId, allocator);
					object.AddMember(v2, v3, allocator);
				}
			}

			if ((it = allFiled.filedmap.find(g_szExNo)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					if ('*' == FileRptPack.szSpNumEx[0])
					{
						//memcpy(szTmpEx, FileRptPack.szSpNumEx, strlen((char*)FileRptPack.szSpNumEx));
						rapidjson::Value v2(it->second.szCArgName, allocator);
						object.AddMember(v2, "", allocator);
					}
					else
					{
						rapidjson::Value v2(it->second.szCArgName, allocator);
						rapidjson::Value v3((char*)FileRptPack.szSpNumEx, allocator);
						object.AddMember(v2, v3, allocator);
					}
				}
			}

			if ((it = allFiled.filedmap.find(g_szRecvTime)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					//rtime
					char szReceiveTime[25] = { 0 };
					if (2 == urlinfo.nPushtimefmt)
					{
						int nYear_MsgId = 0, nMon_MsgId = 0, nDay_MsgId = 0, nHour_MsgId = 0, nMin_MsgId = 0, nSec_MsgId = 0;
						sscanf(FileRptPack.szTime, "%04d-%02d-%02d %02d:%02d:%02d",
							&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
						sprintf(szReceiveTime, "%04d%02d%02d%02d%02d%02d",
							nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);
					}
					else
					{
						memcpy(szReceiveTime, FileRptPack.szTime, min(strlen(FileRptPack.szTime), sizeof(szReceiveTime)-1));
					}

					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3(szReceiveTime, allocator);
					object.AddMember(v2, v3, allocator);
				}
			}

			if ((it = allFiled.filedmap.find(g_szMsg)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					std::string strMsgTmp = CGlobal::UrlEncode((char*)itFileMo->szMsgContent);

					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3(strMsgTmp.c_str(), allocator);
					object.AddMember(v2, v3, allocator);
				}
			}

			//vector
			//zhubo 2017-12-19
			MakeFixedJsonValueToObj(allFiled,  document, object);
			/*
			int nVecSize = allFiled.vecFiled.size();
			for (int i = 0; i < nVecSize; ++i)
			{
				rapidjson::Value v2(allFiled.vecFiled[i].szCArgName, allocator);
				rapidjson::Value v3(allFiled.vecFiled[i].szCArgValue, allocator);
				object.AddMember(v2, v3, allocator);
			}
			*/
			array.PushBack(object, allocator);

		}

		//最后一个包
		rapidjson::Value v(strTagMos.c_str(), allocator);
		document.AddMember(v, array, allocator);

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);
		//可能会崩溃
		strOut = buffer.GetString();

		//去掉{"mos":以及最后的}{"mos":[]}
		int nLenTmp = strOut.length();
		if (nLenTmp >= 10)
		{
			strOut = strOut.substr(7, nLenTmp - 8);
		}

	}
	catch (...)
	{
		nOutMsgLen = -1;
	}

}

void CParseToolImpl::MakePushRptJsonData_V32(const ALLFILED &allFiled, const vector<PACK_DELIVER> &FileRptPackArr, const HSBC_URLINFO& urlinfo, string &strOut)
{
	std::vector<PACK_DELIVER>::const_iterator itFileRpt = FileRptPackArr.begin();
	int nExcpLine = __LINE__;        //记录当前行
	rapidjson::Value array(rapidjson::kArrayType);
	rapidjson::Document document;
	rapidjson::Value v2, v3;
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	char szBelong[BELONG_LEN + 1] = { 0 };
	FILEDMAP::const_iterator it;

	string strTagRpts ="rpts";
	
	document.SetObject();

	try
	{
		int nStatus = -1;
		string strErrDesc("");
		string strState("");

		for (; (itFileRpt != FileRptPackArr.end()); ++itFileRpt)
		{
			const PACK_DELIVER &FileRptPack = *itFileRpt;
			rapidjson::Value object(rapidjson::kObjectType);

			//确定成功和失败描述
			nStatus = _strnicmp((LPCSTR)FileRptPack.msgcontent.szState, "DELIVRD", 7) == 0 ? 0 : 2;
			if (0 == nStatus)
			{
				strErrDesc = RPTSTATUSDESC_OK;
			}
			else
			{
				strErrDesc = RPTSTATUSDESC_ERR;
			}

			if (0 == nStatus)
			{
				strState = "00";
			}
			else
			{
				strState = (char*)FileRptPack.msgcontent.szState;
			}

			//useridex
			if ((it = allFiled.filedmap.find(g_szUesrIdEx)) != allFiled.filedmap.end())
			{
				if (0 != strlen(it->second.szCArgName))
				{
					//无子节点,无父节点，而且跟梦网标准字段有对应
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3(urlinfo.szRptPwd, allocator);
					object.AddMember(v2, v3, allocator);
				}
			}


			//rpt对象
			if ((it = allFiled.filedmap.find(g_szMsgId)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					//misgid
					if(WBS_CARGTYPE_OTHER == it->second.nCargType)
					{
						int64_t nMsgId = m_global.TranMsgIdCharToI64(FileRptPack.szMsgId);
						char szMsgIdTmp[64+1] = {0};
						CAdapter::_i64toa(nMsgId, szMsgIdTmp, 10);
						rapidjson::Value v2(it->second.szCArgName, allocator);
						rapidjson::Value v3(szMsgIdTmp, allocator);
						object.AddMember(v2, v3, allocator);				
					}
					else
					{
						rapidjson::Value v2(it->second.szCArgName, allocator);
						object.AddMember(v2, (int64_t)m_global.TranMsgIdCharToI64(FileRptPack.szMsgId), allocator);
					}
				}
			}

			if ((it = allFiled.filedmap.find(g_szCustID)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3((char*)FileRptPack.szCustId, allocator);
					object.AddMember(v2, v3, allocator);
				}
			}

			if ((it = allFiled.filedmap.find(g_szPkNum)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					//pkno
					rapidjson::Value v2(it->second.szCArgName, allocator);
					object.AddMember(v2, FileRptPack.nPknum, allocator);
				}
			}

			if ((it = allFiled.filedmap.find(g_szPkTotal)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					//
					rapidjson::Value v2(it->second.szCArgName, allocator);
					object.AddMember(v2, FileRptPack.nPkTotal, allocator);
				}
			}

			if ((it = allFiled.filedmap.find(g_szMobile)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					//mobile
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3((char*)FileRptPack.msgcontent.szDestTerminalId, allocator);
					object.AddMember(v2, v3, allocator);
				}
			}

			if ((it = allFiled.filedmap.find(g_szSpNo)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					//spno
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3((char*)FileRptPack.szDestId, allocator);
					object.AddMember(v2, v3, allocator);
				}
			}

			if ((it = allFiled.filedmap.find(g_szExNo)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					//exno
					if ('*' == FileRptPack.szSpNumEx[0])
					{
						rapidjson::Value v2(it->second.szCArgName, allocator);
						object.AddMember(v2, "", allocator);
					}
					else
					{
						rapidjson::Value v2(it->second.szCArgName, allocator);
						rapidjson::Value v3((char*)FileRptPack.szSpNumEx, allocator);
						object.AddMember(v2, v3, allocator);
					}
				}
			}

			char szSendTime[25] = { 0 };
			char szReceiveTime[25] = { 0 };
			char szSendElongTime[25] = { 0 };
			SYSTEMTIME_EX tt;
            CAdapter::GetLocalTime(&tt);
            SYSTEMTIME_EX_LINUX t(tt);
			
			if (2 == urlinfo.nPushtimefmt)
			{
				int nYear_MsgId = 0, nMon_MsgId = 0, nDay_MsgId = 0, nHour_MsgId = 0, nMin_MsgId = 0, nSec_MsgId = 0;
				sscanf(FileRptPack.szSendTime, "%04d-%02d-%02d %02d:%02d:%02d",
					&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
				sprintf(szSendTime, "%04d%02d%02d%02d%02d%02d",
					nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);
				sscanf(FileRptPack.szTime, "%04d-%02d-%02d %02d:%02d:%02d",
					&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
				sprintf(szReceiveTime, "%04d%02d%02d%02d%02d%02d",
					nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);

				sprintf(szSendElongTime, "%04d%02d%02d%02d%02d%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
			}
			else
			{
				memcpy(szSendTime, FileRptPack.szSendTime, min(strlen(FileRptPack.szSendTime), sizeof(szSendTime)));
				memcpy(szReceiveTime, FileRptPack.szTime, min(strlen(FileRptPack.szTime), sizeof(szReceiveTime)));
				sprintf(szSendElongTime, "%04d-%02d-%02d %02d:%02d:%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
			}

			if ((it = allFiled.filedmap.find(g_szSendTime)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					//stime
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3(szSendTime, allocator);
					object.AddMember(v2, v3, allocator);
				}
			}

			if ((it = allFiled.filedmap.find(g_szRecvTime)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					//rtime
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3(szReceiveTime, allocator);
					object.AddMember(v2, v3, allocator);
				}
			}

			if ((it = allFiled.filedmap.find(g_szPushTime)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					//ptime
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3(szSendElongTime, allocator);
					object.AddMember(v2, v3, allocator);
				}
			}

			if ((it = allFiled.filedmap.find(g_szStatus)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					//status
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3(strState.c_str(), allocator);
					object.AddMember(v2, v3, allocator);
				}
			}

			if ((it = allFiled.filedmap.find(g_szErrCode)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					//errcode
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3((char*)FileRptPack.msgcontent.szState, allocator);
					object.AddMember(v2, v3, allocator);
				}
			}

			if ((it = allFiled.filedmap.find(g_szErrCodeDesc)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					//errcodedesc
					if(WBS_CARGTYPE_UPPER_STRING == it->second.nCargType)
					{
						//转换为大写
						strErrDesc = cstr::to_upper_s(strErrDesc.c_str());
					}
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3(strErrDesc.c_str(), allocator);
					object.AddMember(v2, v3, allocator);
				}
			}


			if ((it = allFiled.filedmap.find(g_szExData)) != allFiled.filedmap.end())
			{
				if (strlen(it->second.szCArgName) > 0)
				{
					rapidjson::Value v2(it->second.szCArgName, allocator);
					rapidjson::Value v3(FileRptPack.szExData, allocator);
					object.AddMember(v2, v3, allocator);
				}
			}

			//vector
			//zhubo 2017-12-19
			MakeFixedJsonValueToObj(allFiled, document, object);
			/*
			int nVecSize = allFiled.vecFiled.size();
			for (int i = 0; i < nVecSize; ++i)
			{
				rapidjson::Value v2(allFiled.vecFiled[i].szCArgName, allocator);
				rapidjson::Value v3(allFiled.vecFiled[i].szCArgValue, allocator);
				object.AddMember(v2, v3, allocator);
			}
			*/
			array.PushBack(object, allocator);

		}
		//最后一个包
		//array.PushBack(object, allocator);
		rapidjson::Value v(strTagRpts.c_str(), allocator);
		document.AddMember(v, array, allocator);
		//document.RemoveAllMembers();
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);
		//可能会崩溃
		strOut = buffer.GetString();

		//去掉{"rpts":以及最后的}
		int nLenTmp = strOut.length();
		if (nLenTmp >= 11)
		{
			strOut = strOut.substr(8, nLenTmp - 9);
		}
	}
	catch (...)
	{

	}
}

void CParseToolImpl::GetUrlRequestFixedValue(const ALLFILED &allFiled, string& strFixedFiels, bool bIgnoreLastTag)
{
	strFixedFiels = "";
	vector<string> vecFixed;
	//======组成固定值 zhubo 2017-12-12
	int nVecSize = allFiled.vecFiled.size();
	for (int i = 0; i < nVecSize; ++i)
	{
		if (0 == allFiled.vecFiled[i].nBelongtype 
			&& 0 == strlen(allFiled.vecFiled[i].szBelong))
		{
			string strTmp = cstr::format("%s=%s", allFiled.vecFiled[i].szCArgName, allFiled.vecFiled[i].szCArgValue);
			vecFixed.push_back(strTmp);
		}
	}
	for (int i = 0; i < vecFixed.size(); i++)
	{
		strFixedFiels += vecFixed[i];
		if ((i != vecFixed.size() - 1))
		{
			strFixedFiels += "&";
		}
		else
		{
			if (!bIgnoreLastTag)
			{
				strFixedFiels += "&";
			}
		}
	}
	//======组成固定值 zhubo 2017-12-12

}

void CParseToolImpl::MakeFixedJsonValueToDoc(const ALLFILED &allFiled, rapidjson::Document& doc)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	//zhubo 2017-12-12
	int nVecSize = allFiled.vecFiled.size();
	for (int i = 0; i < nVecSize; ++i)
	{
		rapidjson::Value v2(allFiled.vecFiled[i].szCArgName, allocator);
		rapidjson::Value v3(allFiled.vecFiled[i].szCArgValue, allocator);
		if (0 == allFiled.vecFiled[i].nBelongtype
			&& 0 == strlen(allFiled.vecFiled[i].szBelong))
		{
			doc.AddMember(v2, v3, allocator);
		}
	}
	//zhubo 2017-12-12
}

void CParseToolImpl::MakeFixedJsonValueToObj(const ALLFILED &allFiled, string& tagArrName, rapidjson::Document& doc, rapidjson::Value& obj)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	int nVecSize = allFiled.vecFiled.size();
	for (int i = 0; i < nVecSize; ++i)
	{
		rapidjson::Value v2(allFiled.vecFiled[i].szCArgName, allocator);
		rapidjson::Value v3(allFiled.vecFiled[i].szCArgValue, allocator);
		if (0 == allFiled.vecFiled[i].nBelongtype
			&& strlen(allFiled.vecFiled[i].szBelong) > 0
			&& 0 == strcasecmp(allFiled.vecFiled[i].szBelong, tagArrName.c_str()))
		{
			obj.AddMember(v2, v3, allocator);
		}
	}
}

void CParseToolImpl::MakeFixedJsonValueToObj(const ALLFILED &allFiled, rapidjson::Document& doc, OUT rapidjson::Value& obj)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	int nVecSize = allFiled.vecFiled.size();
	for (int i = 0; i < nVecSize; ++i)
	{
		rapidjson::Value v2(allFiled.vecFiled[i].szCArgName, allocator);
		rapidjson::Value v3(allFiled.vecFiled[i].szCArgValue, allocator);
		if (0 == allFiled.vecFiled[i].nBelongtype)
		{
			obj.AddMember(v2, v3, allocator);
		}
	}
}

void CParseToolImpl::MakeFixedXmlValueToParentNode(const ALLFILED &allFiled, rapidxml::xml_document<>& doc, rapidxml::xml_node<>* pNode)
{
	//vector
	int nVecSize = allFiled.vecFiled.size();
	for (int i = 0; i < nVecSize; i++)
	{
		int nlen = strlen(allFiled.vecFiled[i].szBelong);
		if (0 == allFiled.vecFiled[i].nBelongtype
			&& nlen > 0
			&& 0 == strcasecmp(allFiled.vecFiled[i].szBelong, pNode->name()))
		{
			xml_node<> *node2 = doc.allocate_node(node_element, allFiled.vecFiled[i].szCArgName, allFiled.vecFiled[i].szCArgValue);
			pNode->append_node(node2);
		}

	}
}


void CParseToolImpl::MakeUrlItemValue(const ALLFILED &allFiled, const char* pItemKey,const char* pValue, string& strItem, bool bAddFirstTag)
{
	FILEDMAP::const_iterator it;
	if (PT_PUSH_IS_IN_FIELDMAP(pItemKey))
	{
		if (!bAddFirstTag)
		{
			strItem += "&";
		}
		strItem += it->second.szCArgName;
		strItem += "=";
		strItem += pValue;
	}

}

int CParseToolImpl::MakeMoRequestV5_YL(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, string &strMo)
{
	int nRet = 0;
	try
	{
		char pchBody[2*1024] = { 0 };

		strcpy(pchBody, "GET ");
		strcat(pchBody, urlinfo.szWebPath);
		strcat(pchBody, "?msgtype=0");

		strcat(pchBody, "&");
		strcat(pchBody, "phone=");
		strcat(pchBody, (const char*)MoRptPack.szSrcTerminalId);

		strcat(pchBody, "&");
		strcat(pchBody, "subid=");
		strcat(pchBody, (const char*)MoRptPack.szSpNumEx);

		strcat(pchBody, "&");
		strcat(pchBody, "content=");
		std::string strMsgTmp = CGlobal::UrlEncode((char*)MoRptPack.szMsgContent);
		strcat(pchBody, strMsgTmp.c_str());

		char szReceiveTime[20] = { 0 };
		int nYear_MsgId = 0, nMon_MsgId = 0, nDay_MsgId = 0, nHour_MsgId = 0, nMin_MsgId = 0, nSec_MsgId = 0;
		sscanf(MoRptPack.szTime, "%04d-%02d-%02d %02d:%02d:%02d",
			&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
		sprintf(szReceiveTime, "%04d%02d%02d%02d%02d%02d",
			nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);
		strcat(pchBody, "&");
		strcat(pchBody, "receivetime=");
		strcat(pchBody, szReceiveTime);

		strcat(pchBody, " HTTP/1.1\r\n");
		sprintf(pchBody + strlen(pchBody), "Host: %s:%d\r\n", urlinfo.szDomain, urlinfo.nPort);
		strcat(pchBody, "Connection: Close\r\n\r\n");
		strMo = pchBody;

		nRet = strlen(pchBody);
	}
	catch (...)
	{
		nRet = -1;
	}

	return nRet;
}

int CParseToolImpl::MakeRptRequestV5_YL(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, string &strRpt)
{
	int nRet = 0;
	try
	{
		char pchBody[2 * 1024] = { 0 };

		strcpy(pchBody, "GET ");
		strcat(pchBody, urlinfo.szWebPath);
		strcat(pchBody, "?msgtype=2");

		strcat(pchBody, "&");
		strcat(pchBody, "phone=");
		strcat(pchBody, (const char*)MoRptPack.msgcontent.szDestTerminalId);

		char szMsgId[50] = { 0 };
		strcat(pchBody, "&");
		strcat(pchBody, "reqid=");
		sprintf(szMsgId, "%lld", CGlobal::TranMsgIdCharToI64(MoRptPack.szMsgId));
		strcat(pchBody, szMsgId);

		strcat(pchBody, "&");
		strcat(pchBody, "extend=");
		strcat(pchBody, MoRptPack.szExData);

		char szSendTime[20] = { 0 };
		char szReceiveTime[20] = { 0 };
		int nYear_MsgId = 0, nMon_MsgId = 0, nDay_MsgId = 0, nHour_MsgId = 0, nMin_MsgId = 0, nSec_MsgId = 0;
		sscanf(MoRptPack.szSendTime, "%04d-%02d-%02d %02d:%02d:%02d",
			&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
		sprintf(szSendTime, "%04d%02d%02d%02d%02d%02d",
			nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);
		sscanf(MoRptPack.szTime, "%04d-%02d-%02d %02d:%02d:%02d",
			&nYear_MsgId, &nMon_MsgId, &nDay_MsgId, &nHour_MsgId, &nMin_MsgId, &nSec_MsgId);
		sprintf(szReceiveTime, "%04d%02d%02d%02d%02d%02d",
			nYear_MsgId, nMon_MsgId, nDay_MsgId, nHour_MsgId, nMin_MsgId, nSec_MsgId);

		strcat(pchBody, "&");
		strcat(pchBody, "receivetime=");
		strcat(pchBody, szReceiveTime);

		strcat(pchBody, "&");
		strcat(pchBody, "sendtime=");
		strcat(pchBody, szSendTime);

		//确定成功和失败描述
		int nStatus = _strnicmp((LPCSTR)MoRptPack.msgcontent.szState, "DELIVRD", 7) == 0 ? 0 : 2;
		std::string strState = "";
		if (0 == nStatus)
		{
			strState = "0";
		}
		else
		{
			strState = (char*)MoRptPack.msgcontent.szState;
		}
		strcat(pchBody, "&");
		strcat(pchBody, "state=");
		strcat(pchBody, strState.c_str());

		strcat(pchBody, " HTTP/1.1\r\n");
		sprintf(pchBody + strlen(pchBody), "Host: %s:%d\r\n", urlinfo.szDomain, urlinfo.nPort);
		strcat(pchBody, "Connection: Close\r\n\r\n");
		strRpt = pchBody;

		nRet = strlen(pchBody);
	}
	catch (...)
	{
		nRet = -1;
	}

	return nRet;
}

int CParseToolImpl::DealECErrcode(const WBS_RPTERRCODEMAP& errcodemap, std::string& strErrCode, std::string& strErrDesc)
{
	char szUtf8[256] = {0};
	WBS_RPTERRCODEMAP::const_iterator it = errcodemap.find(strErrCode);
	if (it != errcodemap.end())
	{
		memset(szUtf8,0,sizeof(szUtf8));
		strErrCode = it->second.szECErrcode;
		ToUTF8((char*)it->second.szECDesc, szUtf8, sizeof(szUtf8));
		strErrDesc = CGlobal::UrlEncode(szUtf8);
	}
	else
	{
		//如果找不到，则代表未知，走未知流程
		strErrCode = "UNKNOWN";
		it = errcodemap.find(strErrCode);
		if (it != errcodemap.end())
		{
			strErrCode = it->second.szECErrcode;
			memset(szUtf8,0,sizeof(szUtf8));
			ToUTF8((char*)it->second.szECDesc, szUtf8, sizeof(szUtf8));
			strErrDesc = CGlobal::UrlEncode(szUtf8);
		}
	}
	return 0;
}

/*

void AddFixFiledToDoc(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* root, ALLFILED& allFiled)
{
		
}
*/
