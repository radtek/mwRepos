#include "ConnPoolMgr.h"

#include <iostream>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <odbc/Connection.h>
#include <odbc/Exception.h>
#include <odbc/PreparedStatement.h>
#include <odbc/ResultSet.h>
#include <boost/scoped_ptr.hpp>

#define DEFAULT_DBPOOL1  0
#define DEFAULT_DBPOOL2  1
#define MAX_SIGN_LEN     10
#define MAX_SPID_LEN     6
#define MAX_CHANNEL_LEN	 21

#ifndef TINYINT
#define TINYINT unsigned char
#endif

#ifndef USHORT
#define USHORT unsigned short
#endif

#ifndef UCHAR
#define UCHAR unsigned char
#endif

#ifndef DATE
#define DATE long
#endif

using namespace std;
using namespace ODBC;

struct SPGATE_BIND_INFO
{
	int		nLoginUID;				//企业在数据库中的惟一编号
	int		nUserUID;				//nUID所关联的其他帐号的UID
	int		nDestUID;			//目标UID=后端spgate的UID
	int		nEcId;				//企业ID
    //zhubo 2017-5.24
	int		nGateGroupId;       //通道组ID
	TINYINT nBindType;			//绑定类型 0：通道账号，1：通道组
	TINYINT nGateLevel;         //主备：0主，1备
    //zhubo 2017-5.24
	short	nGateType;			//网关类型 0移动 1联通 2电信
	short	nFeeFlag;			//费率标志 1预付 2后付 0未知
	short	nExpTraits;			//扩展特性 0可扩展 1不可扩展 2共用
	short	nRiseLevel;			//大于0为提升级别n级， 小于0降低级别n级，等于0维持原级   (现该值做为发送级别用不再做提升之用)
	UCHAR	ucExChannel;		//通道是否为主端口扩展  0:该通道直接从移动定购的由移动签名 1:在移动定购通道的基础上我司再增加的扩展
	USHORT	usSignlen;			//签名长度
	USHORT	usMaxwords;			//最大短信长度
	USHORT	usSinglelen;		//单条短信长度
	USHORT	usMultilen1;		//长短信第一条的长度
	USHORT	usMultilen2;		//长短信第二条的长度
	/*******qiwei add*******************/

	USHORT	usEnSignlen;			//签名长度
	USHORT	usEnMaxwords;			//最大短信长度
	USHORT	usEnSinglelen;		//单条短信长度
	USHORT	usEnMultilen1;		//长短信第一条的长度
	USHORT	usEnMultilen2;		//长短信第二条的长度
	char	szEnSignature[2*MAX_SIGN_LEN+1];	//签名

  /********** end qiwei add **********/
	DATE	dtStart;
	DATE	dtEnd;
	char    cRouteFlag;         //路由标志 0:MT/MO/RPT不分 1:MT路由 2:MO路由 3:RPT路由
	char	szLoginId[MAX_SPID_LEN+1];		//登陆帐号
	char	szUsrId[MAX_SPID_LEN+1];		//关联帐号
	char	szSpGate[MAX_CHANNEL_LEN+1];		//主端口
	char	szExNo[MAX_CHANNEL_LEN+1];		//扩展端口
	char	szSpNumber[MAX_CHANNEL_LEN+1];	//主端口+扩展端口
	char	szSignature[2*MAX_SIGN_LEN+1];	//签名
	SPGATE_BIND_INFO()
	{
		memset(this, 0, sizeof(SPGATE_BIND_INFO));
		nExpTraits = -1;
	}

	SPGATE_BIND_INFO& operator=(const SPGATE_BIND_INFO& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(SPGATE_BIND_INFO));
		memcpy(this, &other, sizeof(SPGATE_BIND_INFO));
		return *this;
	}

	bool operator==(const SPGATE_BIND_INFO& other)
	{
		if (this == &other)
		{
			return true;
		}
		bool bRet = false;
		if (other.nLoginUID == this->nLoginUID && other.nUserUID == this->nUserUID)
		{
			if (other.nGateType == this->nGateType)
			{
				//bRet = true;
				//以下部分允许同一帐号绑定多个相同类型通道，直接返回true则不允许这种情况
				//如：106579999001,10659999002是不允许的
				if (0 == memcmp(other.szSpGate, this->szSpGate, sizeof(this->szSpGate)))
				{
					if (0 == memcmp(other.szExNo, this->szExNo, sizeof(this->szExNo)))
					{
						if (other.nExpTraits == this->nExpTraits)
						{
							bRet = true;
						}	
					}
				}
			}
		}
		return bRet;
	}
};

typedef std::list<SPGATE_BIND_INFO> BIND_INFO_LIST;


DATE StrToDATE(const char* strTime)
{
	DATE result = 0;
	int hour = 0, min = 0, sec = 0;
	sscanf(strTime, "%d:%d:%d", &hour, &min, &sec);
	result = hour * 3600 + min * 60 + sec;
	return result;
}

void UpdateSpGateBindList(BIND_INFO_LIST& BindListMt, BIND_INFO_LIST& BindListMo, const char* loginId)
{
	const char* strSql = "EXEC WS_LOADBINDLISTV01 ?,?,?,?";
	ODBC::Connection* conn = NULL;
	int nStartUId = 0;
	
	try
	{
		conn = CDBConnPoolMgr::GetInstance().GetConnFromPool(DEFAULT_DBPOOL1);
		CDBConnectionPtr connPtr(DEFAULT_DBPOOL1, conn);
		boost::scoped_ptr<PreparedStatement> pstmtPtr(conn->prepareStatement(strSql, 8));
		
		if (!pstmtPtr)
		{
			cout << "生成PreparedStatement对象失败！！" << endl;
			return;
		}
		pstmtPtr->setString(1, loginId);
		pstmtPtr->setInt(2, 0);
		pstmtPtr->setInt(3, 200000);
		pstmtPtr->setString(4, "");
		
		boost::scoped_ptr<ODBC::ResultSet> rsPtr(pstmtPtr->executeQuery());
		int rowCount = 0;
		int minLen = 0;
        int nRouteFlag = 0;

        std::string strSpNumber;
        std::string strSignature;
        std::string strTimeStart;
        std::string strTimeEnd;

		SPGATE_BIND_INFO bindInfo;
		
		while (rsPtr && rsPtr->next())
		{
			++rowCount;
            memset(&bindInfo, 0, sizeof(bindInfo));
            bindInfo.nLoginUID   = rsPtr->getInt("LOGINUID");
            bindInfo.nUserUID    = rsPtr->getInt("USERUID");
            bindInfo.nDestUID    = rsPtr->getInt("DESTUID");
            bindInfo.nGateType   = rsPtr->getInt("SpIsUncm");
            bindInfo.ucExChannel = rsPtr->getInt("IsExChannel");
            nRouteFlag           = rsPtr->getInt("RouteFlag");
            bindInfo.nEcId       = rsPtr->getInt("EcId");

            std::string strLoginId = rsPtr->getString("LoginId");
            strLoginId.erase(strLoginId.find_last_not_of(' ') + 1);
            minLen = strLoginId.length() < (sizeof(bindInfo.szLoginId) - 1) ? strLoginId.length() : (sizeof(bindInfo.szLoginId) - 1);
            memcpy(bindInfo.szLoginId, strLoginId.c_str(), minLen);

            std::string strUserId = rsPtr->getString("UserId");
            strUserId.erase(strUserId.find_last_not_of(' ') + 1);
            minLen = strUserId.length() < (sizeof(bindInfo.szUsrId) - 1) ? strUserId.length() : (sizeof(bindInfo.szUsrId) - 1);
            memcpy(bindInfo.szUsrId, strUserId.c_str(), minLen);

            bindInfo.nExpTraits = rsPtr->getInt("PortType");
            int nGTID = rsPtr->getInt("GTID");
            if (nGTID > nStartUId)
            {
                // 保存最大GTID
                nStartUId = nGTID;
            }

            std::string strSpGate = rsPtr->getString("SpGate");
            strSpGate.erase(strSpGate.find_last_not_of(' ') + 1);
            minLen = strSpGate.length() < (sizeof(bindInfo.szSpGate) - 1) ? strSpGate.length() : (sizeof(bindInfo.szSpGate) - 1);
            memcpy(bindInfo.szSpGate, strSpGate.c_str(), minLen);

            std::string strCpNo = rsPtr->getString("CpNo");
            strCpNo.erase(strCpNo.find_last_not_of(' ') + 1);
            minLen = strCpNo.length() < (sizeof(bindInfo.szExNo) - 1) ? strCpNo.length() : (sizeof(bindInfo.szExNo) - 1);
            memcpy(bindInfo.szExNo, strCpNo.c_str(), minLen);

            strSpNumber = strSpGate + strCpNo;
            // strSpNumber.erase(strSpNumber.find_last_not_of(' ') + 1);
            minLen = strSpNumber.length() < (sizeof(bindInfo.szExNo) - 1) ? strSpNumber.length() : (sizeof(bindInfo.szExNo) - 1);
            memcpy(bindInfo.szSpNumber, strSpNumber.c_str(), minLen);

            bindInfo.nFeeFlag = rsPtr->getInt("FeeFlag");

            // 通道签名信息
            bindInfo.usSignlen = rsPtr->getInt("signlen");

            strSignature = rsPtr->getString("signstr");
            strSignature.erase(strSignature.find_last_not_of(' ') + 1);
            minLen = strSignature.length() < (sizeof(bindInfo.szSignature) - 1) ? strSignature.length() : (sizeof(bindInfo.szSignature) - 1);
            memcpy(bindInfo.szSignature, strSignature.c_str(), minLen);

            bindInfo.usMaxwords  = rsPtr->getInt("maxwords");
            bindInfo.usSinglelen = rsPtr->getInt("singlelen");
            bindInfo.usSinglelen -= bindInfo.usSignlen;
            bindInfo.usMultilen1 = rsPtr->getInt("multilen1");
            bindInfo.usMultilen2 = bindInfo.usMultilen1 - bindInfo.usSignlen;
            bindInfo.usMaxwords  -= bindInfo.usSignlen;

            // 通道签名信息
            bindInfo.usEnSignlen = rsPtr->getInt("ensignlen");
            strSignature = rsPtr->getString("ensignstr");
            minLen = strSignature.length() < (sizeof(bindInfo.szEnSignature) - 1) ? strSignature.length() : (sizeof(bindInfo.szEnSignature) - 1);
            memcpy(bindInfo.szEnSignature, strSignature.c_str(), minLen);

            bindInfo.usEnMaxwords  = rsPtr->getInt("enmaxwords");
            bindInfo.usEnSinglelen = rsPtr->getInt("ensinglelen");
            bindInfo.usEnSinglelen -= bindInfo.usEnSignlen;
            bindInfo.usEnMultilen1 = rsPtr->getInt("enmultilen1");
            bindInfo.usEnMultilen2 = bindInfo.usEnMultilen1 - bindInfo.usEnSignlen;

            bindInfo.usEnMaxwords -= bindInfo.usEnSignlen;

            strTimeStart = rsPtr->getString("timestart");
            strTimeStart.erase(strTimeStart.find_last_not_of(' ') + 1);
            // FIXME: strTimeStart时间格式转化
            bindInfo.dtStart = StrToDATE(strTimeStart.c_str());

            strTimeEnd = rsPtr->getString("timeend");
            strTimeEnd.erase(strTimeEnd.find_last_not_of(' ') + 1);
            // FIXME: strTimeEnd时间格式转化
            bindInfo.dtEnd = StrToDATE(strTimeEnd.c_str());

            bindInfo.nRiseLevel = rsPtr->getInt("RiseLevel");

            bindInfo.cRouteFlag = nRouteFlag;

            // 不分上下行
            if (0 == nRouteFlag)
            {
                BindListMt.push_back(bindInfo);
                BindListMo.push_back(bindInfo);
                // AddToBindList(BindListMt, bindInfo);
                // AddToBindList(BindListMo, bindInfo);

            }
            // 下行路由
            else if (1 == nRouteFlag)
            {
                BindListMt.push_back(bindInfo);
                // AddToBindList(BindListMt, bindInfo);
            }
            // 上行路由
            else if (2 == nRouteFlag)
            {
                // AddToBindList(BindListMo, bindInfo);
                BindListMo.push_back(bindInfo);
            }
            else
            {
            }
		}
		
		if (rowCount == 0)
        {
            // 没有在表中想到记录
            cout << "ResultSet is NULL";
            return;
        }
		cout << "rowCount = " << rowCount << endl;
	}
	catch (const ODBC::Exception & e)
    {
		char buffer[512] = { 0 };
		snprintf(buffer, sizeof(buffer), "UpdateSpGateBindList()--DB operate  Error! SQL:%s reason: %s", strSql, e.what());
		cout << buffer << endl;
		return;
	}
	catch (...)
	{
		char buffer[512] = { 0 };
		snprintf(buffer, sizeof(buffer), "UpdateSpGateBindList()--System Error! SQL:%s", strSql);
		cout << buffer << endl;
		return;
    }
    return;
}

int main(int argc, char* argv[])
{
	// 启动数据库1
	if (CDBConnPoolMgr::GetInstance().CreateConnPool("PT_CloudJR",
	        "PT_CloudJR", "sa", "123zxcvbnm,./", "select 1", 5, 10) == false)
	{
		cout << "初始化连接池管理类失败1！！！！" << endl;
		return -1;
	}
	
	time_t start, end;
	time(&start);
	BIND_INFO_LIST mt;
	BIND_INFO_LIST mo;
	UpdateSpGateBindList(mt, mo, "WBS00D");
	
	time(&end);
	int64_t bbbb = static_cast<int64_t>(end - start);
	cout << "bbbb = " << bbbb << endl;
	return 0;
}
