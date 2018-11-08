#ifndef __ODBC_PREPARED_STATEMENT_H__
#define __ODBC_PREPARED_STATEMENT_H__

#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <odbc/ResultSet.h>
#include <odbc/Connection.h>

namespace ODBC
{

// 绑定参数信息结构
struct BIND_PARAM_INFO
{
	SQLCHAR*     paramValue; // 参数缓存地址
	SQLLEN*      indArr;     // IND数组指针
	SQLSMALLINT  sql_c_type; // C数据类型标识
	SQLSMALLINT  sql_type;   // SQL数据类型标识
	SQLLEN       columnSize; // 对应字段的实际长度
	SQLLEN       columnMax;  // 对应字段的最大长度
	unsigned int memSize;    // 申请的内存大小

	BIND_PARAM_INFO()
	{
		memset(this, 0, sizeof(BIND_PARAM_INFO));
	}
};

class PreparedStatement
{
public:
	PreparedStatement(unsigned int paramSetSize = 1);
	PreparedStatement(SQLHDBC hDbc, const std::string & strSql, Connection* conn, int timeout, unsigned int paramSetSize = 1);
	~PreparedStatement();

	ResultSet* executeQuery();
	long executeUpdate();

	void setInt(int index, int value);
	void setInt64(int index, int64_t value);
	void setSmallInt(int index, short int value);
	void setString(int index, const std::string & value);
	void setFloat(int index, float value, int precise = 2);
	void setDouble(int index, double value, int precise = 2);
	void setDecimal(int index, double value, int precise = 2);

private:
	PreparedStatement(const PreparedStatement &);
	PreparedStatement & operator = (const PreparedStatement &);

	void release();
	void resizeParamTable(int maxSize);
	void setParamAttr(int index, SQLSMALLINT sqlType, int maxSize);
	void handleError(const char* FILE = __FILE__, int LINE = __LINE__);

private:

	SQLHSTMT hStmt_;
	Connection* conn_;
	std::string strSql_;
	unsigned int paramNum_;
	unsigned int paramSetSize_;
	std::vector<BIND_PARAM_INFO> bindParamInfoVec_;
};

} // namespace ODBC

#endif // __ODBC_PREPARED_STATEMENT_H__

