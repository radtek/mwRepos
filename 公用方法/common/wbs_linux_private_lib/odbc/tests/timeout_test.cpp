#include <stdio.h>
#include <sql.h>
#include <sqlext.h>

int main(int argc, char* argv[])
{
	SQLHENV		hEnv;			// Handle ODBC environment
	SQLRETURN	nRet;			// result of functions
	SQLHDBC		hDBC;			// Handle connection
	SQLHSTMT	hStmt;			// Handle Statement

	SQLUINTEGER	nTimeout = 10;
	SQLUINTEGER	nVersion = 0;

	SQLCHAR pszSqlState[SQL_MAX_MESSAGE_LENGTH] = "";
	SQLCHAR pszErrorMsg[SQL_MAX_MESSAGE_LENGTH] = "";

	SQLINTEGER	nNativeError = 0L;
	SQLSMALLINT nErrorMsg = 0;

	nRet = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	if ((nRet != SQL_SUCCESS) && (nRet != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error AllocHandle\n");
		return -1;
	}

	nRet = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION,
	                     reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3),
	                     SQL_IS_UINTEGER);
	if ((nRet != SQL_SUCCESS) && (nRet != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error SetEnv\n");
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
		return -1;
	}

	nRet = SQLGetEnvAttr(hEnv,
	                     SQL_ATTR_ODBC_VERSION,
	                     reinterpret_cast<SQLPOINTER>(&nVersion),
	                     SQL_IS_INTEGER,
	                     NULL);
	if ((nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO))
	{
		printf("ODBC Version is: %u\n", nVersion);
	}
	else
	{
		printf("Error Get ODBC Version %d\n", nRet);
		SQLGetDiagRec(SQL_HANDLE_DBC, hEnv, 1,  pszSqlState, &nNativeError,
		              pszErrorMsg, SQL_MAX_MESSAGE_LENGTH - 1, &nErrorMsg);
		printf("Error %s (%d)\n State %s \n", pszErrorMsg, nErrorMsg, pszSqlState);
	}

	nRet = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDBC);
	if ((nRet != SQL_SUCCESS) && (nRet != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error AllocHDB %d\n", nRet);
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
		return 0;
	}

	nRet = SQLSetConnectAttr(hDBC, SQL_ATTR_LOGIN_TIMEOUT,
	                         reinterpret_cast<SQLPOINTER>(nTimeout),
	                         SQL_IS_INTEGER);
	if ((nRet != SQL_SUCCESS) && (nRet != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error SetConnectAttr %d\n", nRet);
	}

	nRet = SQLGetConnectAttr(hDBC, SQL_ATTR_LOGIN_TIMEOUT,
	                         reinterpret_cast<SQLPOINTER>(&nTimeout),
	                         SQL_IS_INTEGER,
	                         NULL);
	if ((nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO))
	{
		printf("Login Timeout is: %u\n", nTimeout);
	}
	else
	{
		printf("Error Get Login Timeout %d\n", nRet);
		SQLGetDiagRec(SQL_HANDLE_DBC, hDBC, 1,  pszSqlState, &nNativeError,
		              pszErrorMsg, SQL_MAX_MESSAGE_LENGTH - 1, &nErrorMsg);
		printf("Error %s (%d)\n State %s \n", pszErrorMsg, nErrorMsg, pszSqlState);
	}

	nRet = SQLConnect(hDBC,
	                  static_cast<SQLCHAR* >(static_cast<void* >(const_cast<char*>("test_liang_pt"))), SQL_NTS,
	                  static_cast<SQLCHAR* >(static_cast<void* >(const_cast<char*>("developer"))), SQL_NTS,
	                  static_cast<SQLCHAR* >(static_cast<void* >(const_cast<char*>("developer"))), SQL_NTS);
	if ((nRet != SQL_SUCCESS) && (nRet != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error SQLConnect %d\n", nRet);
		SQLGetDiagRec(SQL_HANDLE_DBC, hDBC, 1,  pszSqlState, &nNativeError,
		              pszErrorMsg, SQL_MAX_MESSAGE_LENGTH - 1, &nErrorMsg);
		printf("%s (%d)\n", pszErrorMsg, nErrorMsg);
		SQLFreeHandle(SQL_HANDLE_DBC, hDBC);
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
		return -1;
	}

	printf("Connected !\n");

	nRet = SQLSetConnectAttr(hDBC,
	                         SQL_ATTR_CONNECTION_TIMEOUT,
	                         reinterpret_cast<SQLPOINTER>(nTimeout),
	                         SQL_IS_INTEGER);
	if ((nRet != SQL_SUCCESS) && (nRet != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error SetConnectAttr %d\n", nRet);
	}

	nRet = SQLGetConnectAttr(hDBC,
	                         SQL_ATTR_CONNECTION_TIMEOUT,
	                         reinterpret_cast<SQLPOINTER>(&nTimeout),
	                         SQL_IS_INTEGER,
	                         NULL);
	if ((nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO))
	{
		printf("Connection timeout is: %u\n", nTimeout);
	}
	else
	{
		printf("Error Get Connection timeout %d\n", nRet);
		SQLGetDiagRec(SQL_HANDLE_DBC, hDBC, 1,  pszSqlState, &nNativeError,
		              pszErrorMsg, SQL_MAX_MESSAGE_LENGTH - 1, &nErrorMsg);
		printf("Error %s (%d)\n State %s \n", pszErrorMsg, nErrorMsg, pszSqlState);
	}

	nRet = SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt);
	if ((nRet != SQL_SUCCESS) && (nRet != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error AllocStatement %d\n", nRet);
		SQLGetDiagRec(SQL_HANDLE_DBC, hDBC, 1,  pszSqlState, &nNativeError,
		              pszErrorMsg, SQL_MAX_MESSAGE_LENGTH - 1, &nErrorMsg);
		printf("%s (%d)\n", pszErrorMsg, nErrorMsg);
		SQLFreeHandle(SQL_HANDLE_DBC, hDBC);
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
		return 0;
	}

	nRet = SQLSetStmtAttr(hStmt,
	                      SQL_ATTR_QUERY_TIMEOUT,
	                      reinterpret_cast<SQLPOINTER>(nTimeout),
	                      SQL_IS_UINTEGER);
	if ((nRet != SQL_SUCCESS) && (nRet != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error Set Query Timeout %d\n", nRet);
		SQLGetDiagRec(SQL_HANDLE_DBC, hDBC, 1,  pszSqlState, &nNativeError,
		              pszErrorMsg, SQL_MAX_MESSAGE_LENGTH - 1, &nErrorMsg);
		printf("Error %s (%d)\n State %s \n", pszErrorMsg, nErrorMsg, pszSqlState);
		SQLFreeHandle(SQL_HANDLE_DBC, hDBC);
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

		return 0;
	}

	nRet = SQLGetStmtAttr(hStmt,
	                      SQL_ATTR_QUERY_TIMEOUT,
	                      reinterpret_cast<SQLPOINTER>(&nTimeout),
	                      SQL_IS_UINTEGER,
	                      NULL);
	if ((nRet != SQL_SUCCESS) && (nRet != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error Get Query Timeout %d\n", nRet);
		SQLGetDiagRec(SQL_HANDLE_DBC, hDBC, 1,  pszSqlState, &nNativeError,
		              pszErrorMsg, SQL_MAX_MESSAGE_LENGTH - 1, &nErrorMsg);
		printf("Error %s (%d)\n State %s \n", pszErrorMsg, nErrorMsg, pszSqlState);
	}
	else
	{
		printf("Query Timeout is %d\n", nTimeout);
	}

	//.............................

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

	SQLDisconnect(hDBC);

	SQLFreeHandle(SQL_HANDLE_DBC, hDBC);
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

	return 0;
}
