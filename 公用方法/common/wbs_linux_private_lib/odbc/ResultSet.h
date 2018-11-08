#ifndef __ODBC_RESULTSET_H__
#define __ODBC_RESULTSET_H__

#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <map>
#include <string.h>
#include <stdint.h>
#include <odbc/Connection.h>

namespace ODBC
{

const int kDateTimeLength = 32;
const int kIntLength      = 10;
const int kBigIntLength   = 20;
const int kDefualtLength  = 128;

struct CasestringCompare
{
	int operator() (const std::string & x, const std::string & k) const
	{
		int ret = strcasecmp(x.c_str(), k.c_str());
		return  ret < 0;
	}
};

typedef std::map<std::string, int, CasestringCompare > Kvsmap;

class ResultSet;

class ResultSetMetaData
{
public:
	friend class ResultSet;
	ResultSetMetaData() : columnType_(0), columnLength_(0), columnValue_(NULL)
	{
		memset(columnName_, 0, sizeof(columnName_));
	}
    
	~ResultSetMetaData()
	{
	}
    
	void setColumnName(const char* columnName)
	{
		size_t copyLength = (strlen(columnName) < sizeof(columnName_)) ? strlen(columnName) : sizeof(columnName_);
		memcpy(columnName_, columnName, copyLength);
		columnName_[copyLength] = 0;
	}
    
	const char* getColumnName() const
	{
		return columnName_;
	}
    
	void setColumnType(SQLLEN columnType)
	{
		columnType_ = columnType;
	}
    
	SQLLEN getColumnType() const
	{
		return columnType_;
	}

	void setColumnLength(SQLLEN columnLength)
	{
		columnLength_ = columnLength;
	}

	SQLLEN getColumnLength() const
	{
		return columnLength_;
	}

private:
	SQLLEN columnType_;
	SQLLEN columnLength_;
	char   columnName_[64];
	char*  columnValue_;
};

class ResultSet
{
public:
	ResultSet();
	ResultSet(SQLHSTMT hStmt, Connection* conn);
	~ResultSet();

	bool next();

	int         getInt(int index);
	int64_t     getInt64(int index);
	short int   getSmallInt(int index);
	float       getFloat(int index);
	double      getDouble(int index);
	double      getDecimal(int index);
	std::string getString(int index);
    
	int         getInt(const char* columnName);
	int64_t     getInt64(const char* columnName);
	short int   getSmallInt(const char* columnName);
	float       getFloat(const char* columnName);
	double      getDouble(const char* columnName);
	double      getDecimal(const char* columnName);
	std::string getString(const char* columnName);

private:
	ResultSet(const ResultSet &);
	ResultSet & operator = (const ResultSet &);
    
	SQLLEN getColumnLength(int index);
	SQLLEN getColumnType(int index);
	double getDoubleFromHexStruct(SQL_NUMERIC_STRUCT & numericValue);
	void   handleError(const char* FILE = __FILE__, int LINE = __LINE__);
    
private:
	SQLHSTMT    hStmt_;
	Connection* conn_;
	SQLINTEGER  rowCount_; 
	SQLSMALLINT colCount_;
	ResultSetMetaData* metaData_;

	// 保存列名称跟索引的Map
	Kvsmap nameIndexMap_;
};

} // namespace ODBC

#endif // __ODBC_RESULTSET_H__

