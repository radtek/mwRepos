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

using namespace std;
using namespace ODBC;

const char* kUserNameBase = "dedede";

void PrintConnPoolInfo()
{
	int nMinSize, nMaxSize, nIdle, nBusy;
	CDBConnPoolMgr::GetInstance().GetConnPoolInfo(DEFAULT_DBPOOL1, nMinSize, nMaxSize, nIdle, nBusy);
	cout << "--------------------" << endl;
	cout << "nMinSize = " << nMinSize << endl;
	cout << "nMaxSize = " << nMaxSize << endl;
	cout << "nIdle = " << nIdle << endl;
	cout << "nBusy = " << nBusy << endl;
	cout << "--------------------" << endl;
}

struct DATA_TEST
{
	short int tinyflag;
	short int smallflag;
	int intflag;
	int64_t bigintflag;
	float floatflag;
	double doubleflag;
	double decimalflag;
};

std::vector<DATA_TEST> datas;

string getTimeOfDay()
{
	time_t t;
	struct tm* p;
	time(&t);
	p = localtime(&t);

	char buffer[32] = { 0 };
	snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d.%03d",
	         p->tm_year + 1900, p->tm_mon + 1, p->tm_mday,
	         p->tm_hour, p->tm_min, p->tm_sec, 0);
	return buffer;
}

void test_insert(short int tinyflag, short int smallflag, int intflag,
                 int64_t bigintflag, float floatflag, double doubleflag, double decimalflag)
{
	Connection* conn = NULL;

	try
	{
		const char* strSql = "insert into test(tinyintflag,smallintflag,intflag,bigintflag,floatflag,doubleflag,decimalflag)"
		                     "values(?,?,?,?,?,?,?)";
		conn = CDBConnPoolMgr::GetInstance().GetConnFromPool(DEFAULT_DBPOOL1);
		std::cout << "conn = " << static_cast<void* >(conn) << std::endl;
		CDBConnectionPtr connPtr(DEFAULT_DBPOOL1, conn);
		conn->setAutoCommit(false);
		boost::scoped_ptr<PreparedStatement> pstmtPtr(conn->prepareStatement(strSql));
		if (pstmtPtr == NULL)
		{
			cout << "生成PreparedStatement对象失败！！" << endl;
			return;
		}

		pstmtPtr->setSmallInt(1, tinyflag);
		pstmtPtr->setSmallInt(2, smallflag);
		pstmtPtr->setInt(3, intflag);
		pstmtPtr->setInt64(4, bigintflag);
		pstmtPtr->setFloat(5, floatflag);
		pstmtPtr->setDouble(6, doubleflag);
		pstmtPtr->setDecimal(7, decimalflag, 1);
		long number = pstmtPtr->executeUpdate();
		cout << "number = " << number << endl;
		std::cout << "conn: " << static_cast<void* >(conn) << std::endl;
		conn->commit();
	}
	catch (const ODBC::Exception & e)
	{
		cout << e.what() << endl;
		conn->rollback();
		return;
	}
	catch (...)
	{
		cout << "system error!!" << endl;
		conn->rollback();
		return;
	}
}

void test_insert2()
{
	Connection* conn = NULL;
	try
	{
		const char* strSql = "insert into AAAA(id,str,userid) values(?,?,?)";
		conn = CDBConnPoolMgr::GetInstance().GetConnFromPool(DEFAULT_DBPOOL1);
		std::cout << "conn = " << static_cast<void* >(conn) << std::endl;
		CDBConnectionPtr connPtr(DEFAULT_DBPOOL1, conn);
		conn->setAutoCommit(false);
		boost::scoped_ptr<PreparedStatement> pstmtPtr(conn->prepareStatement(strSql));
		if (pstmtPtr == NULL)
		{
			cout << "生成PreparedStatement对象失败！！" << endl;
			return;
		}
		pstmtPtr->setInt(1, 123);
		pstmtPtr->setString(2, "哈哈");
		pstmtPtr->setString(3, "123456");
		long number = pstmtPtr->executeUpdate();
		cout << "number = " << number << endl;
		std::cout << "conn: " << static_cast<void* >(conn) << std::endl;
		conn->commit();
	}
	catch (const ODBC::Exception & e)
	{
		cout << e.what() << endl;
		conn->rollback();
		return;
	}
	catch (...)
	{
		cout << "system error!!" << endl;
		conn->rollback();
		return;
	}
}

void test_select(const string & userId)
{
	try
	{
		const char* strSql = "select top 1 uid from userdata where userid=?";
		Connection* conn = CDBConnPoolMgr::GetInstance().GetConnFromPool(DEFAULT_DBPOOL2);
		CDBConnectionPtr connPtr(DEFAULT_DBPOOL2, conn);
		boost::scoped_ptr<PreparedStatement> pstmtPtr(conn->prepareStatement(strSql));
		if (pstmtPtr == NULL)
		{
			cout << "生成PreparedStatement对象失败！！" << endl;
			return;
		}

		pstmtPtr->setString(1, userId);
		boost::scoped_ptr<ResultSet> rsPtr(pstmtPtr->executeQuery());

		int rowCount = 0;
		while (rsPtr && rsPtr->next())
		{
			++rowCount;
			int uid = rsPtr->getInt(1);
			cout << "uid = " << uid  << endl;
			cout << "--------------------------" << endl;
		}

		if (rowCount == 0)
		{
			cout << "userId = " << userId << " not ResultSet! " << endl;
		}
	}
	catch (const ODBC::Exception & e)
	{
		cout << e.what() << endl;
		return;
	}
	catch (...)
	{
		cout << "system error!!" << endl;
		return;
	}
}

void test_select2()
{
	try
	{
		const char* strSql = "select top 10 id,str,userid from AAAA";
		Connection* conn = CDBConnPoolMgr::GetInstance().GetConnFromPool(DEFAULT_DBPOOL1);
		CDBConnectionPtr connPtr(DEFAULT_DBPOOL1, conn);
		boost::scoped_ptr<PreparedStatement> pstmtPtr(conn->prepareStatement(strSql));
		if (pstmtPtr == NULL)
		{
			cout << "生成PreparedStatement对象失败！！" << endl;
			return;
		}

		boost::scoped_ptr<ResultSet> rsPtr(pstmtPtr->executeQuery());

		int rowCount = 0;
		while (rsPtr && rsPtr->next())
		{
			++rowCount;
			int idn = rsPtr->getInt("id");
			string str = rsPtr->getString("str");
			string userid = rsPtr->getString("userid");
			
			cout << "idn = " << idn << ", str = " << str << ", userid = " << userid << endl;
			cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
		}

		cout << "rowCount = " << rowCount << endl;
	}
	catch (const ODBC::Exception & e)
	{
		cout << e.what() << endl;
		return;
	}
	catch (...)
	{
		cout << "system error!!" << endl;
		return;
	}
}

int main()
{
	cout << "sizeof(SQLBIGINT): " << sizeof(SQLBIGINT) << endl;
	cout << "sizeof(int64_t): " << sizeof(int64_t) << endl;

	// 启动数据库1
	if (CDBConnPoolMgr::GetInstance().CreateConnPool("montnetdb131",
	        "montnetdb131", "developer", "developer", "select 1", 5, 10) == false)
	{
		cout << "初始化连接池管理类失败1！！！！" << endl;
		return -1;
	}
	
	test_insert2();
	
	test_select2();
	
	// 启动数据库2
	//if (CDBConnPoolMgr::GetInstance().CreateConnPool(DEFAULT_DBPOOL2,
	//        "liang_pt_198", "sa", "123456", "select 1", 5, 10) == false)
	//{
	//    cout << "初始化连接池管理类失败2！！！！" << endl;
	//    return -1;
	//}
	
	//for (int i = 1; i <= 1; i++)
	//{
	//	short int tinyflag = 10;
	//	short int smallflag = 20;
	//	int intflag = 30;
	//	int64_t bigintflag = 5294967295;
	//	float floatflag = 5.9f;
	//	double doubleflag = 20.13;
	//	double decimalflag = 3009.154;
	//	test_insert(tinyflag, smallflag, intflag, bigintflag, floatflag, doubleflag, decimalflag);
	//}

	//time_t start, end;
	//time(&start);
	//for (int i = 0; i < 1; i++)
	//{
	//	test_select2();
	//}
	//time(&end);
	//cout << "datas.size() = " << datas.size() << endl;
	
	
	PrintConnPoolInfo();

	/*
	// 存储过程
	{
	    const char* strSql = "exec testUsers";
	    Connection* conn = CDBConnPoolMgr::GetInstance().GetConnFromPool(DEFAULT_DBPOOL1);
	    CDBConnectionPtr connPtr(DEFAULT_DBPOOL1, conn);
	    boost::scoped_ptr<PreparedStatement> pstmtPtr(conn->prepareStatement(strSql));
	    if (pstmtPtr == NULL)
	    {
	        cout << "生成PreparedStatement对象失败！！" << endl;
	        return -1;
	    }

	    boost::scoped_ptr<ResultSet> rsPtr(pstmtPtr->executeQuery());
	    while (rsPtr && rsPtr->next())
	    {
	        int userId = rsPtr->getInt(1);
	        string userName = rsPtr->getString(2);
	        string loginDate = rsPtr->getString(3);
	        cout << endl;
	        cout << "userId = " << userId << ", userName = " << userName << ",loginDate = " << loginDate << endl;
	        cout << "--------------------------------------------------------------------" << endl;
	    }
	    PrintConnPoolInfo();
	}
	*/
	return 0;

}
