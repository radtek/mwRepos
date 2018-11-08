#include <odbc/ResultSet.h>
#include <odbc/Exception.h>

#include <stdio.h>
#include <stdlib.h>

namespace ODBC
{

ResultSet::ResultSet() : hStmt_(NULL),conn_(NULL), rowCount_(0), colCount_(0), metaData_(NULL)
{
}

ResultSet::ResultSet(SQLHSTMT hStmt, Connection* conn) 
	: hStmt_(hStmt), conn_(conn), rowCount_(0), colCount_(0), metaData_(NULL)
{
	SQLRETURN retCode;
	retCode = SQLNumResultCols(hStmt_, &colCount_);
	if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
	{
		// 抛异常
		handleError(__FILE__, __LINE__);
	}

	if (colCount_ < 1)
	{
		char errMsg[1024] = { 0 };
		snprintf(errMsg, sizeof(errMsg), "(%s:%d) columns number must be >= 1.", __FILE__, __LINE__);
		throw ODBC::Exception(errMsg);
	}

	metaData_ = new ResultSetMetaData[colCount_];
	if (NULL == metaData_)
	{
		char errMsg[1024] = { 0 };
		snprintf(errMsg, sizeof(errMsg), "(%s:%d) system new ResultSetMetaData[] fail.", __FILE__, __LINE__);
		throw ODBC::Exception(errMsg);
	}

	// 设置每一列的属性
	char        columnName[128];
	SQLLEN      columnType;
	SQLLEN      columnLength;
	SQLSMALLINT bufferLen;

	// 初始化前清空
	nameIndexMap_.clear();

	for (SQLSMALLINT i = 1; i <= colCount_; i++)
	{
		SQLColAttribute(hStmt_, i,
						SQL_DESC_NAME, columnName, 128,
						static_cast<SQLSMALLINT* >(&bufferLen), 0);

		SQLColAttribute(hStmt_, i,
						SQL_DESC_TYPE, NULL, 0, NULL,
						static_cast<SQLLEN* >(&columnType));

		SQLColAttribute(hStmt_, i,
						SQL_DESC_LENGTH, NULL, 0, 0,
						static_cast<SQLLEN* >(&columnLength));

		// std::cout << "columnName = " << columnName
		//          << ", bufferLen = " << bufferLen
		//          << ", columnType = " << columnType
		//          << ", columnLength = " << columnLength << std::endl;

		// 把<columnName, index>插入到MAP中，方便建立按columnName查找相应域
		nameIndexMap_.insert(std::make_pair(columnName, i));

		switch (columnType)
		{
		case SQL_VARCHAR:
		case SQL_INTEGER:
		case SQL_SMALLINT:
		case SQL_BIGINT:
		case SQL_CHAR:
		case SQL_TINYINT:
		case SQL_BIT:
		case SQL_REAL:
		case SQL_FLOAT:
		case SQL_DOUBLE:
		{
			if (columnLength <= 0)
			{
				char errMsg[1024] = { 0 };
				snprintf(errMsg, sizeof(errMsg), "(%s:%d) column length must be > 0.", __FILE__, __LINE__);
				throw ODBC::Exception(errMsg);
			}
			metaData_[i - 1].columnValue_ = new char[columnLength + 2];
		}
		break;
		case SQL_WCHAR:
    	case SQL_WVARCHAR:
    	case SQL_WLONGVARCHAR:
    	{
    		if (columnLength <= 0)
			{
				char errMsg[1024] = { 0 };
				snprintf(errMsg, sizeof(errMsg), "(%s:%d) column length must be > 0.", __FILE__, __LINE__);
				throw ODBC::Exception(errMsg);
			}
			metaData_[i - 1].columnValue_ = new char[columnLength * 2 + 2];
    	}
    	break;
		case SQL_NUMERIC:
		{
			if (columnLength <= 0)
			{
				char errMsg[1024] = { 0 };
				snprintf(errMsg, sizeof(errMsg), "(%s:%d) column length must be > 0.", __FILE__, __LINE__);
				throw ODBC::Exception(errMsg);
			}
			metaData_[i - 1].columnValue_ = new char[columnLength + 2];
		}
		break;
		case SQL_DATETIME:
        case SQL_TYPE_DATE:
		case SQL_TYPE_TIME:
		case SQL_TYPE_TIMESTAMP:
		{
			metaData_[i - 1].columnValue_ = new char[kDateTimeLength + 1];
			columnLength = kDateTimeLength;
		}
		break;
		default:
		{
			metaData_[i - 1].columnValue_ = new char[kDefualtLength + 1];
			columnLength = kDefualtLength;
		}
		break;
		}

		metaData_[i - 1].setColumnType(columnType);
		metaData_[i - 1].setColumnLength(columnLength + 2);
		metaData_[i - 1].setColumnName(columnName);
	}
}

ResultSet::~ResultSet()
{
	if (metaData_)
	{
		for (int i = 0; i < colCount_; i++)
		{
			if (metaData_[i].columnValue_)
				delete [] metaData_[i].columnValue_;
		}
		delete [] metaData_;
	}

	nameIndexMap_.clear();
}

bool ResultSet::next()
{
	return (SQLFetch(hStmt_) != SQL_NO_DATA);
}

SQLLEN ResultSet::getColumnType(int index)
{
	if (index < 1 || index > colCount_)
	{
		char errMsg[1024] = { 0 };
		snprintf(errMsg, sizeof(errMsg), "(%s:%d) columns index must be >= 1 or <= colCount_.", __FILE__, __LINE__);
		throw ODBC::Exception(errMsg);
	}

	if (NULL == metaData_)
	{
		char errMsg[1024] = { 0 };
		snprintf(errMsg, sizeof(errMsg), "(%s:%d) metaData_ can't is null.", __FILE__, __LINE__);
		throw ODBC::Exception(errMsg);
	}
	return metaData_[index - 1].getColumnType();
}

SQLLEN ResultSet::getColumnLength(int index)
{
	if (index < 1 || index > colCount_)
	{
		char errMsg[1024] = { 0 };
		snprintf(errMsg, sizeof(errMsg), "(%s:%d) columns index must be >= 1 or <= colCount_.", __FILE__, __LINE__);
		throw ODBC::Exception(errMsg);
	}

	if (NULL == metaData_)
	{
		char errMsg[1024] = { 0 };
		snprintf(errMsg, sizeof(errMsg), "(%s:%d) metaData_ can't is null.", __FILE__, __LINE__);
		throw ODBC::Exception(errMsg);
	}
	return metaData_[index - 1].getColumnLength();
}

int ResultSet::getInt(int index)
{
	if (index < 1 || index > colCount_)
	{
		char errMsg[1024] = { 0 };
		snprintf(errMsg, sizeof(errMsg), "(%s:%d) columns index must be >= 1 or <= colCount_.", __FILE__, __LINE__);
		throw ODBC::Exception(errMsg);
	}

	long value = 0;
	SQLLEN pIndicators;
	SQLRETURN retCode;
	retCode = SQLGetData(hStmt_,
						 static_cast<SQLUSMALLINT>(index),
						 SQL_C_LONG,
						 static_cast<SQLPOINTER>(&value),
						 sizeof(long),
						 &pIndicators);
	if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
	{
		handleError(__FILE__, __LINE__);
	}
	return static_cast<int>(value);
}

int64_t ResultSet::getInt64(int index)
{
	std::string buffer = getString(index);
	int64_t value = static_cast<int64_t>(atol(buffer.c_str()));
	return value;
}

short int ResultSet::getSmallInt(int index)
{
	if (index < 1 || index > colCount_)
	{
		char errMsg[1024] = { 0 };
		snprintf(errMsg, sizeof(errMsg), "(%s:%d) columns index must be >= 1 or <= colCount_.", __FILE__, __LINE__);
		throw ODBC::Exception(errMsg);
	}

	short int value = 0;
	SQLLEN pIndicators;
	SQLRETURN retCode;
	retCode = SQLGetData(hStmt_,
						 static_cast<SQLUSMALLINT>(index),
						 SQL_C_SHORT,
						 static_cast<SQLPOINTER>(&value),
						 sizeof(short int),
						 &pIndicators);
	if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
	{
		handleError(__FILE__, __LINE__);
	}
	return value;
}

float ResultSet::getFloat(int index)
{
	if (index < 1 || index > colCount_)
	{
		char errMsg[1024] = { 0 };
		snprintf(errMsg, sizeof(errMsg), "(%s:%d) columns index must be >= 1 or <= colCount_.", __FILE__, __LINE__);
		throw ODBC::Exception(errMsg);
	}

	float value = 0.0f;
	SQLLEN pIndicators;
	SQLRETURN retCode;
	retCode = SQLGetData(hStmt_,
						 static_cast<SQLUSMALLINT>(index),
						 SQL_C_FLOAT,
						 static_cast<SQLPOINTER>(&value),
						 sizeof(value),
						 &pIndicators);
	if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
	{
		handleError(__FILE__, __LINE__);
	}
	return value;
}

double ResultSet::getDouble(int index)
{
	if (index < 1 || index > colCount_)
	{
		char errMsg[1024] = { 0 };
		snprintf(errMsg, sizeof(errMsg), "(%s:%d) columns index must be >= 1 or <= colCount_.", __FILE__, __LINE__);
		throw ODBC::Exception(errMsg);
	}

	double value = 0.0;
	SQLLEN pIndicators;
	SQLRETURN retCode;
	retCode = SQLGetData(hStmt_,
						 static_cast<SQLUSMALLINT>(index),
						 SQL_C_DOUBLE,
						 static_cast<SQLPOINTER>(&value),
						 sizeof(value),
						 &pIndicators);
	if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
	{
		handleError(__FILE__, __LINE__);
	}
	return value;
}

double ResultSet::getDecimal(int index)
{
	if (index < 1 || index > colCount_)
	{
		char errMsg[1024] = { 0 };
		snprintf(errMsg, sizeof(errMsg), "(%s:%d) columns index must be >= 1 or <= colCount_.", __FILE__, __LINE__);
		throw ODBC::Exception(errMsg);
	}

	double value = 0.0;
	SQLLEN pIndicators;
	SQLRETURN retCode;

	retCode = SQLGetData(hStmt_,
						 static_cast<SQLUSMALLINT>(index),
						 SQL_C_CHAR,
						 metaData_[index - 1].columnValue_,
						 metaData_[index - 1].getColumnLength(),
						 &pIndicators);
	if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
	{
		handleError(__FILE__, __LINE__);
	}

	value = strtod(metaData_[index - 1].columnValue_, NULL);
	return value;
}

std::string ResultSet::getString(int index)
{
	if (index < 1 || index > colCount_)
	{
		char errMsg[1024] = { 0 };
		snprintf(errMsg, sizeof(errMsg), "(%s:%d) columns index must be >= 1 or <= colCount_.", __FILE__, __LINE__);
		throw ODBC::Exception(errMsg);
	}

	SQLRETURN retCode;
	long valueLength = 0;
	memset(metaData_[index - 1].columnValue_, 0, metaData_[index - 1].getColumnLength());
	retCode = SQLGetData(hStmt_,
						 static_cast<SQLUSMALLINT>(index),
						 SQL_C_CHAR,
						 metaData_[index - 1].columnValue_,
						 metaData_[index - 1].getColumnLength(),
						 static_cast<SQLLEN* >(&valueLength));
	if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
	{
		handleError(__FILE__, __LINE__);
	}

	return metaData_[index - 1].columnValue_;
}

int ResultSet::getInt(const char* columnName)
{
	Kvsmap::iterator it = nameIndexMap_.find(columnName);
	if (it != nameIndexMap_.end())
	{
		int index = it->second;
		return getInt(index);
	}

	char errMsg[1024] = { 0 };
	snprintf(errMsg, sizeof(errMsg), "(%s:%d) Can not find column-related(%s) data.", __FILE__, __LINE__, columnName);
	throw ODBC::Exception(errMsg);
}

short int ResultSet::getSmallInt(const char* columnName)
{
	Kvsmap::iterator it = nameIndexMap_.find(columnName);
	if (it != nameIndexMap_.end())
	{
		int index = it->second;
		return getSmallInt(index);
	}

	char errMsg[1024] = { 0 };
	snprintf(errMsg, sizeof(errMsg), "(%s:%d) Can not find column-related(%s) data.", __FILE__, __LINE__, columnName);
	throw ODBC::Exception(errMsg);
}

int64_t ResultSet::getInt64(const char* columnName)
{
	Kvsmap::iterator it = nameIndexMap_.find(columnName);
	if (it != nameIndexMap_.end())
	{
		int index = it->second;
		return getInt64(index);
	}

	char errMsg[1024] = { 0 };
	snprintf(errMsg, sizeof(errMsg), "(%s:%d) Can not find column-related(%s) data.", __FILE__, __LINE__, columnName);
	throw ODBC::Exception(errMsg);
}

float ResultSet::getFloat(const char* columnName)
{
	Kvsmap::iterator it = nameIndexMap_.find(columnName);
	if (it != nameIndexMap_.end())
	{
		int index = it->second;
		return getFloat(index);
	}

	char errMsg[1024] = { 0 };
	snprintf(errMsg, sizeof(errMsg), "(%s:%d) Can not find column-related(%s) data.", __FILE__, __LINE__, columnName);
	throw ODBC::Exception(errMsg);
}

double ResultSet::getDouble(const char* columnName)
{
    Kvsmap::iterator it = nameIndexMap_.find(columnName);
    if (it != nameIndexMap_.end())
    {
        int index = it->second;
        return getDouble(index);
    }

    char errMsg[1024] = { 0 };
    snprintf(errMsg, sizeof(errMsg), "(%s:%d) Can not find column-related(%s) data.", __FILE__, __LINE__, columnName);
    throw ODBC::Exception(errMsg);
}

double ResultSet::getDecimal(const char* columnName)
{
	Kvsmap::iterator it = nameIndexMap_.find(columnName);
	if (it != nameIndexMap_.end())
	{
		int index = it->second;
		return getDecimal(index);
	}

	char errMsg[1024] = { 0 };
	snprintf(errMsg, sizeof(errMsg), "(%s:%d) Can not find column-related(%s) data.", __FILE__, __LINE__, columnName);
	throw ODBC::Exception(errMsg);
}

std::string ResultSet::getString(const char* columnName)
{
	Kvsmap::iterator it = nameIndexMap_.find(columnName);
	if (it != nameIndexMap_.end())
	{
		int index = it->second;
		return getString(index);
	}

	char errMsg[1024] = { 0 };
	snprintf(errMsg, sizeof(errMsg), "(%s:%d) Can not find column-related(%s) data.", __FILE__, __LINE__, columnName);
	throw ODBC::Exception(errMsg);
}

double ResultSet::getDoubleFromHexStruct(SQL_NUMERIC_STRUCT & numericValue)
{
	long value = 0;
	int last = 1, current = 0;
	int a = 0, b = 0;

	for (int i = 0; i < 16; i++)
	{
		current = static_cast<int>(numericValue.val[i]);
		// std::cout << "current = " << current << std::endl;
		a = current % 16;
		b = current / 16;

		value += last * a;
		last  =  last * 16;
		value += last * b;
		last  =  last * 16;
	}

	long divisor = 1;
	for (int i = 0; i < numericValue.scale; i++)
	{
		divisor = divisor * 10;
    }
	// std::cout << "value = " << value << ", divisor = " << divisor << std::endl;

	return static_cast<double>(value) / static_cast<double>(divisor);
}

void ResultSet::handleError(const char* FILE, int LINE)
{
	if (conn_)
	{
		conn_->setCheck(true);
	}
	
	SQLSMALLINT errmsglen;
	SQLINTEGER errnative;

	UCHAR errmsg[255];
	UCHAR errstate[5];

	char buffer[1024] = { 0 };

	SQLGetDiagRec(SQL_HANDLE_STMT, hStmt_, 1, errstate, &errnative, errmsg, sizeof(errmsg), &errmsglen);
	snprintf(buffer, sizeof(buffer), "(%s:%d) errstate(%s), errnative(%d),errmsg(%s)",
			FILE, LINE, errstate, errnative, errmsg);

	throw ODBC::Exception(buffer);
}

}
