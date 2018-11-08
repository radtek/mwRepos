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

// �󶨲�����Ϣ�ṹ
struct BIND_PARAM_INFO
{
	SQLCHAR*     paramValue; // ���������ַ
	SQLLEN*      indArr;     // IND����ָ��
	SQLSMALLINT  sql_c_type; // C�������ͱ�ʶ
	SQLSMALLINT  sql_type;   // SQL�������ͱ�ʶ
	SQLLEN       columnSize; // ��Ӧ�ֶε�ʵ�ʳ���
	SQLLEN       columnMax;  // ��Ӧ�ֶε���󳤶�
	unsigned int memSize;    // ������ڴ��С

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

