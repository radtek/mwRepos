// AccessMssql.cpp: implementation of the CAccessMssql class.
//
//////////////////////////////////////////////////////////////////////
#include <paranomgr/AccessMssql.h>
#include <string>
#include "Global.h"
#include "LogMgr.h"

#include "ConnPoolMgr.h"
#include <odbc/Exception.h>
#include <odbc/PreparedStatement.h>
#include <odbc/ResultSet.h>
#include <boost/scoped_ptr.hpp>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAccessMssql::CAccessMssql()
{
}

CAccessMssql::~CAccessMssql()
{
}

int CAccessMssql::ReadKfParams(std::string& strMobile, std::string& strUnicom,
                               std::string& strTelecom, std::string& strTelecom_c)
{
	const char* strSql = "EXEC A_GETKFPARAMS";
	ODBC::Connection* conn = NULL;
	try
	{
		conn = CDBConnPoolMgr::GetInstance().GetConnFromPool(DEFAULT_DBPOOL2);
		if (conn == NULL)
		{
			std::string str = "ReadKfParams()--CDBConnPoolMgr::GetInstance().GetConnFromPool() Error! SQL:";
			str += strSql;
			LOGF_ERROR(str.c_str(), "AdoError.txt");
			return MGX_MW_DB_ERROR_99;
		}

		CDBConnectionPtr connPtr(DEFAULT_DBPOOL2, conn);

		boost::scoped_ptr<ODBC::PreparedStatement> pstmtPtr(conn->prepareStatement(strSql));
		if (pstmtPtr == NULL)
		{
			std::string str = "ReadKfParams()--conn->prepareStatement() Error! SQL:";
			str += strSql;
			LOGF_ERROR(str.c_str(), "AdoError.txt");
			return MGX_MW_DB_ERROR_99;
		}

		boost::scoped_ptr<ODBC::ResultSet> rsPtr(pstmtPtr->executeQuery());
		int rowCount = 0;

		while (rsPtr && rsPtr->next())
		{
			++rowCount;
			int iParaCode = rsPtr->getInt("ParaCode");
			int iParaVal1 = rsPtr->getInt("ParaVal1");
			(void)iParaVal1;
			std::string strParaVal2 = rsPtr->getString("ParaVal2");
			strParaVal2.erase(strParaVal2.find_last_not_of(' ') + 1);

			switch (iParaCode)
			{
			case 9001:
			{
				if (strParaVal2.length() > 0)
					strMobile = strParaVal2;
				break;
			}
			case 9002:
			{
				if (strParaVal2.length() > 0)
					strUnicom = strParaVal2;
				break;
			}
			case 9022:
			{
				if (strParaVal2.length() > 0)
					strTelecom = strParaVal2;
				break;
			}
			case 9021:
			{
				if (strParaVal2.length() > 0)
					strTelecom_c = strParaVal2;
				break;
			}
			case 9003:
			{
				break;
			}
			default:
				break;
			}
		}

		if (rowCount == 0)
		{
			// 没有在表中想到记录
			return -1;
		}
	}
	catch (const ODBC::Exception & e)
	{
		char buffer[512] = { 0 };
		snprintf(buffer, sizeof(buffer), "加载号段参数表kf_params错误... SQL:%s reason: %s", strSql, e.what());
		LOGF_ERROR(buffer, "AdoError.txt");
		return MGX_MW_DB_ERROR_99;
	}
	catch (...)
	{
		char buffer[512] = { 0 };
		snprintf(buffer, sizeof(buffer), "ReadKfParams()--System Error! SQL:%s", strSql);
		LOGF_ERROR(buffer, "AdoError.txt");
		return MGX_MW_DB_ERROR_99;
	}

	return 0;
}

int CAccessMssql::ReadPbServiceType(std::string& strMobile, std::string& strUnicom,
                                    std::string& strTelecom, std::string& strTelecom_c, std::string& strWholeNet)
{
	const char* strSql = "EXEC A_GETPBSVRTYPE";
	ODBC::Connection* conn = NULL;
	try
	{
		conn = CDBConnPoolMgr::GetInstance().GetConnFromPool(DEFAULT_DBPOOL2);
		if (conn == NULL)
		{
			std::string str = "ReadKfParams()--CDBConnPoolMgr::GetInstance().GetConnFromPool() Error! SQL:";
			str += strSql;
			LOGF_ERROR(str.c_str(), "AdoError.txt");
			return MGX_MW_DB_ERROR_99;
		}

		CDBConnectionPtr connPtr(DEFAULT_DBPOOL2, conn);

		boost::scoped_ptr<ODBC::PreparedStatement> pstmtPtr(conn->prepareStatement(strSql));
		if (pstmtPtr == NULL)
		{
			std::string str = "ReadKfParams()--conn->prepareStatement() Error! SQL:";
			str += strSql;
			LOGF_ERROR(str.c_str(), "AdoError.txt");
			return MGX_MW_DB_ERROR_99;
		}

		boost::scoped_ptr<ODBC::ResultSet> rsPtr(pstmtPtr->executeQuery());
		int rowCount = 0;

		while (rsPtr && rsPtr->next())
		{
			++rowCount;
			int nGateType = rsPtr->getInt("SPISUNCM");

			std::string strServiceNo   = rsPtr->getString("SERVICENO");
			// 去掉左右空格
			strServiceNo.erase(0, strServiceNo.find_first_not_of(' '));
			strServiceNo.erase(strServiceNo.find_last_not_of(' ') + 1);

			std::string strServiceInfo = rsPtr->getString("SERVICEINFO");
			strServiceInfo.erase(strServiceInfo.find_last_not_of(' ') + 1);

			switch (nGateType)
			{
			case 0: //移动
			{
				if (!strServiceNo.empty())
					strMobile = strServiceNo;
				break;
			}
			case 1: //联通
			{
				if (!strServiceNo.empty())
					strUnicom = strServiceNo;
				break;
			}
			case 2: //电信
			{
				if (!strServiceNo.empty())
					strTelecom = strServiceNo;
				break;
			}
			case 21://电信C网
			{
				if (!strServiceNo.empty())
					strTelecom_c = strServiceNo;
				break;
			}
			case 9://全网
			{
				if (!strServiceNo.empty())
					strWholeNet = strServiceNo;
				break;
			}
			default:
				break;
			}
		}

		if (rowCount == 0)
		{
			// 没有在表中想到记录
			return -1;
		}
	}
	catch (const ODBC::Exception & e)
	{
		char buffer[512] = { 0 };
		snprintf(buffer, sizeof(buffer), "加载号段参数表pb_servicetype错误... SQL:%s reason: %s", strSql, e.what());
		LOGF_ERROR(buffer, "AdoError.txt");
		return MGX_MW_DB_ERROR_99;
	}
	catch (...)
	{
		char buffer[512] = { 0 };
		snprintf(buffer, sizeof(buffer), "ReadPbServiceType()--System Error! SQL:%s", strSql);
		LOGF_ERROR(buffer, "AdoError.txt");
		return MGX_MW_DB_ERROR_99;
	}

	return 0;
}

int CAccessMssql::ReadKfMNP(std::list<MNP_INFO>& MNP_list, int& nMaxId)
{
	const char* strSql = "EXEC A_LOADMNP01 ?";
	ODBC::Connection* conn = NULL;
	try
	{
		conn = CDBConnPoolMgr::GetInstance().GetConnFromPool(DEFAULT_DBPOOL2);
		if (conn == NULL)
		{
			std::string str = "ReadKfMNP()--CDBConnPoolMgr::GetInstance().GetConnFromPool() Error! SQL:";
			str += strSql;
			LOGF_ERROR(str.c_str(), "AdoError.txt");
			return MGX_MW_DB_ERROR_99;
		}

		CDBConnectionPtr connPtr(DEFAULT_DBPOOL2, conn);

		boost::scoped_ptr<ODBC::PreparedStatement> pstmtPtr(conn->prepareStatement(strSql));
		if (pstmtPtr == NULL)
		{
			std::string str = "ReadKfMNP()--conn->prepareStatement() Error! SQL:";
			str += strSql;
			LOGF_ERROR(str.c_str(), "AdoError.txt");
			return MGX_MW_DB_ERROR_99;
		}

		pstmtPtr->setInt(1, nMaxId);
		boost::scoped_ptr<ODBC::ResultSet> rsPtr(pstmtPtr->executeQuery());
		int rowCount = 0;

		while (rsPtr && rsPtr->next())
		{
			++rowCount;
			MNP_INFO mnp_info;

			mnp_info.nPhone     = rsPtr->getInt64("SPISUNCM");
			mnp_info.nSrcType   = static_cast<TINYINT>(rsPtr->getSmallInt("SRCTYPE") && 0xFF);
			mnp_info.nPhoneType = static_cast<TINYINT>(rsPtr->getSmallInt("PHONETYPE") && 0xFF);
			mnp_info.nOpType    = static_cast<TINYINT>(rsPtr->getSmallInt("OPTYPE") && 0xFF);
			int nId             = rsPtr->getInt("ID");
			nId > nMaxId ? nMaxId = nId : 1;

			MNP_list.push_back(mnp_info);
		}

		if (rowCount == 0)
		{
			// 没有在表中想到记录
			return -1;
		}
	}
	catch (const ODBC::Exception & e)
	{
		char buffer[512] = { 0 };
		snprintf(buffer, sizeof(buffer), "加载携号转网表A_MNP错误... SQL:%s reason: %s", strSql, e.what());
		LOGF_ERROR(buffer, "AdoError.txt");
		return MGX_MW_DB_ERROR_99;
	}
	catch (...)
	{
		char buffer[512] = { 0 };
		snprintf(buffer, sizeof(buffer), "ReadKfMNP()--System Error! SQL:%s", strSql);
		LOGF_ERROR(buffer, "AdoError.txt");
		return MGX_MW_DB_ERROR_99;
	}

	return 0;
}

//加载特殊号码表
int CAccessMssql::LoadSpecialPhone(std::list<SPEC_PHONE>& PhonesAdd, std::list<SPEC_PHONE>& PhonesDel, int& nCurrID)
{
	const char* strSql = "EXEC A_LOADSPECPHONE ?";
	ODBC::Connection* conn = NULL;
	try
	{
		conn = CDBConnPoolMgr::GetInstance().GetConnFromPool(DEFAULT_DBPOOL2);
		if (conn == NULL)
		{
			std::string str = "LoadSpecialPhone()--CDBConnPoolMgr::GetInstance().GetConnFromPool() Error! SQL:";
			str += strSql;
			LOGF_ERROR(str.c_str(), "AdoError.txt");
			return MGX_MW_DB_ERROR_99;
		}

		CDBConnectionPtr connPtr(DEFAULT_DBPOOL2, conn);

		boost::scoped_ptr<ODBC::PreparedStatement> pstmtPtr(conn->prepareStatement(strSql));
		if (pstmtPtr == NULL)
		{
			std::string str = "LoadSpecialPhone()--conn->prepareStatement() Error! SQL:";
			str += strSql;
			LOGF_ERROR(str.c_str(), "AdoError.txt");
			return MGX_MW_DB_ERROR_99;
		}

		pstmtPtr->setInt(1, nCurrID);
		boost::scoped_ptr<ODBC::ResultSet> rsPtr(pstmtPtr->executeQuery());
		int rowCount = 0;
		SPEC_PHONE specphone;

		while (rsPtr && rsPtr->next())
		{
			++rowCount;
			memset(&specphone, 0, sizeof(specphone));
			int id = rsPtr->getInt("ID");
			if (id > nCurrID)
			{
				nCurrID = id;
			}
			specphone.nPhone = rsPtr->getInt64("PHONE");
			specphone.nCustomId = rsPtr->getInt("CUSTID");
			std::string strUserId = rsPtr->getString("USERID");
			size_t minLen =
			    strUserId.length() < sizeof(specphone.szUserId) ? strUserId.length() : sizeof(specphone.szUserId);
			memcpy(specphone.szUserId, strUserId.c_str(), minLen);

			specphone.ucType = static_cast<TINYINT>(rsPtr->getSmallInt("SPECTYPE") && 0xFF);
			int optType = rsPtr->getInt("OPTTYPE");
			if (optType == 0)
			{
				PhonesAdd.push_back(specphone);
			}
			else
			{
				PhonesDel.push_back(specphone);
			}
		}

		if (rowCount == 0)
		{
			// 没有在表中想到记录
			return -1;
		}
	}
	catch (const ODBC::Exception & e)
	{
		char buffer[512] = { 0 };
		snprintf(buffer, sizeof(buffer), "LoadSpecialPhone() -- Error! SQL:%s reason: %s", strSql, e.what());
		LOGF_ERROR(buffer, "AdoError.txt");
		return MGX_MW_DB_ERROR_99;
	}
	catch (...)
	{
		char buffer[512] = { 0 };
		snprintf(buffer, sizeof(buffer), "LoadSpecialPhone()--System Error! SQL:%s", strSql);
		LOGF_ERROR(buffer, "AdoError.txt");
		return MGX_MW_DB_ERROR_99;
	}

	return 0;
}

//读取国际电话号码区号列表
int CAccessMssql::ReadCountryList(char* countryList)
{
	const char* strSql = "EXEC A_LOADCOUNTRYLIST";
	ODBC::Connection* conn = NULL;
	try
	{
		conn = CDBConnPoolMgr::GetInstance().GetConnFromPool(DEFAULT_DBPOOL2);
		if (conn == NULL)
		{
			std::string str = "ReadCountryList()--CDBConnPoolMgr::GetInstance().GetConnFromPool() Error! SQL:";
			str += strSql;
			LOGF_ERROR(str.c_str(), "AdoError.txt");
			return MGX_MW_DB_ERROR_99;
		}

		CDBConnectionPtr connPtr(DEFAULT_DBPOOL2, conn);

		boost::scoped_ptr<ODBC::PreparedStatement> pstmtPtr(conn->prepareStatement(strSql));
		if (pstmtPtr == NULL)
		{
			std::string str = "ReadCountryList()--conn->prepareStatement() Error! SQL:";
			str += strSql;
			LOGF_ERROR(str.c_str(), "AdoError.txt");
			return MGX_MW_DB_ERROR_99;
		}

		boost::scoped_ptr<ODBC::ResultSet> rsPtr(pstmtPtr->executeQuery());
		int rowCount = 0;

		while (rsPtr && rsPtr->next())
		{
			++rowCount;
			int nCountry = rsPtr->getInt("COUNTRYCODE");
			if (nCountry < 1000000)
			{
				//zhubo 2017-11-02
				if(nCountry < 0)
				{
					continue;
				}
				//zhubo 2017-11-02
				countryList[nCountry] = 1;
			}
			else
			{
				char buffer[64] = { 0 };
				snprintf(buffer, sizeof(buffer), "COUNTRYCODE=%d,数据库记录行溢出!\n", nCountry);
				LOGF_ERROR(buffer, "AdoError.txt");
			}
		}

		if (rowCount == 0)
		{
			// 没有在表中想到记录
			return -1;
		}
	}
	catch (const ODBC::Exception & e)
	{
		char buffer[512] = { 0 };
		snprintf(buffer, sizeof(buffer), "加载电话号码区号列表A_COUNTRYLIST错误...SQL:%s reason: %s", strSql, e.what());
		LOGF_ERROR(buffer, "AdoError.txt");
		return MGX_MW_DB_ERROR_99;
	}
	catch (...)
	{
		char buffer[512] = { 0 };
		snprintf(buffer, sizeof(buffer), "ReadCountryList()--System Error! SQL:%s", strSql);
		LOGF_ERROR(buffer, "AdoError.txt");
		return MGX_MW_DB_ERROR_99;
	}

	return 0;
}
