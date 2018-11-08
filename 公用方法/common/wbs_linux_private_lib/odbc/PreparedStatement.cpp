#include <odbc/PreparedStatement.h>
#include <odbc/Exception.h>

#include <stdio.h>

namespace ODBC
{

PreparedStatement::PreparedStatement(unsigned int paramSetSize)
    : hStmt_(NULL), conn_(NULL), paramNum_(0), paramSetSize_(paramSetSize)
{
}

PreparedStatement::PreparedStatement(SQLHDBC hDbc, const std::string & strSql, Connection* conn, int timeout, unsigned int paramSetSize)
    : hStmt_(NULL), conn_(conn), strSql_(strSql), paramNum_(0), paramSetSize_(paramSetSize)
{
    SQLRETURN retCode;

    retCode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt_);
    if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
    {
        // 抛异常
        handleError(__FILE__, __LINE__);
    }
    
	// 设置查询超时时间
	// 前端传过来的值，经测试如果大于等于9秒时，设置超时会失效，暂未找到原因
    retCode = SQLSetStmtAttr(hStmt_,
							 SQL_ATTR_QUERY_TIMEOUT,
							 reinterpret_cast<SQLPOINTER>(timeout),
							 SQL_IS_INTEGER);
	if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
	{
		handleError(__FILE__, __LINE__);
	}
	
    // 设置列绑定
    retCode = SQLSetStmtAttr(hStmt_,
                             SQL_ATTR_PARAM_BIND_TYPE,
                             reinterpret_cast<SQLPOINTER>(SQL_PARAM_BIND_BY_COLUMN),
                             0);
    if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
    {
        // 抛异常
        handleError(__FILE__, __LINE__);
    }

    // std::cout << "SQL: " << strSql << std::endl;
    retCode = SQLPrepare(hStmt_,
                         static_cast<SQLCHAR* >(static_cast<void* >(const_cast<char*>(strSql.c_str()))),
                         SQL_NTS);
    if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
    {
        // 抛异常
        handleError(__FILE__, __LINE__);
    }

    // 设置绑定参数的次数（默认为1）
    SQLSetStmtAttr(hStmt_, SQL_ATTR_PARAMSET_SIZE, reinterpret_cast<SQLPOINTER>(paramSetSize_), 0);
}

PreparedStatement::~PreparedStatement()
{
    release();

    /**
     * 2017-04-20 12:01:06
     * SQL_CLOSE：关闭与hstmt句柄相关联的游标，废除所有正在处理的结果。关闭游标后，再次执行SELECT语句可以重新打开游标;
     * SQL_DROP：关闭所有与hstmt相关联的游标，废除正在处理的结果，结束一切对hstmt句柄的访问，并释放hstmt 句柄；
     * SQL_UNBIND：解除SQLBindCol函数为hstmt结果集合所关联的所有列缓冲区；
     * SQL_RESET_PARAS：释放SQLBindParameter函数为hstmt句柄所关联的参数缓区。
     */
	// SQLFreeStmt(hStmt_, SQL_CLOSE);  
	// SQLFreeStmt(hStmt_, SQL_UNBIND);  
	// SQLFreeStmt(hStmt_, SQL_RESET_PARAMS); 
    SQLFreeStmt(hStmt_, SQL_DROP);
}

void PreparedStatement::release()
{
    // 释放资源
    for (size_t i = 0; i < bindParamInfoVec_.size(); ++i)
    {
        if (bindParamInfoVec_[i].indArr != NULL)
        {
            delete [] (bindParamInfoVec_[i].indArr);
            bindParamInfoVec_[i].indArr = NULL;
        }

        if (bindParamInfoVec_[i].paramValue != NULL)
        {
            delete [] (bindParamInfoVec_[i].paramValue);
            bindParamInfoVec_[i].paramValue = NULL;
        }
    }

    // 情况绑定参数属性集合
    bindParamInfoVec_.clear();
}

void PreparedStatement::resizeParamTable(int maxSize)
{
    if (static_cast<unsigned int>(maxSize) > paramNum_)
    {
        int offSet = maxSize - paramNum_;
        for (int i = 0; i < offSet; ++i)
        {
            BIND_PARAM_INFO paramInfo;
            paramInfo.indArr = new SQLLEN[paramSetSize_];
            memset(paramInfo.indArr, 0, sizeof(SQLLEN) * paramSetSize_);
            bindParamInfoVec_.push_back(paramInfo);
        }
    }
}

void PreparedStatement::setParamAttr(int index, SQLSMALLINT sqlType, int maxSize)
{
    int nIndex = index - 1;
    if (nIndex < 0)
    {
        char buffer[256] = { 0 };
        snprintf(buffer, sizeof(buffer), "(%s:%d) set param index must be > 0.", __FILE__, __LINE__);
        throw ODBC::Exception(buffer);
    }

    resizeParamTable(index);

    SQLULEN     columnSize = 0;
    SQLLEN      columnMax  = 0;
    SQLSMALLINT sql_c_type    = SQL_C_CHAR;
    SQLLEN      sql_indicator = 0;
    switch (sqlType)
    {
    case SQL_CHAR:
    case SQL_VARCHAR:
    {
        columnSize = maxSize - 1;
        columnMax  = maxSize;
        sql_c_type = SQL_C_CHAR;
        sql_indicator = SQL_NTS;
    }
    break;
    case SQL_INTEGER:
    {
        columnSize = 0;
        columnMax  = sizeof(SQLINTEGER);
        sql_c_type = SQL_C_LONG;
        sql_indicator = 0;
    }
    break;
    case SQL_SMALLINT:
    {
        columnSize = 0;
        columnMax  = sizeof(SQLSMALLINT);
        sql_c_type = SQL_C_SHORT;
        sql_indicator = 0;
    }
    break;
    case SQL_BIGINT:
    {
        sqlType = SQL_CHAR;
        columnSize = 21;
        columnMax  = 20;
        sql_c_type = SQL_C_CHAR;
        sql_indicator = 0;
    }
    break;
    case SQL_REAL:
    {
        columnSize = 0;
        columnMax  = sizeof(SQLREAL);
        sql_c_type = SQL_C_FLOAT;
        sql_indicator = 0;
    }
    break;
    case SQL_FLOAT:
    case SQL_DOUBLE:
    {
        columnSize = 0;
        columnMax  = sizeof(SQLDOUBLE);
        sql_c_type = SQL_C_DOUBLE;
        sql_indicator = 0;
    }
    break;
    case SQL_NUMERIC:
    case SQL_DECIMAL:
    case SQL_TYPE_TIMESTAMP:
    {
        // sqlType = SQL_VARCHAR;
        columnSize = maxSize - 1;
        columnMax  = maxSize;
        sql_c_type = SQL_C_CHAR;
        sql_indicator = SQL_NTS;
    }
    break;
    default:
    {
        columnSize = maxSize - 1;
        columnMax  = maxSize;
        sql_c_type = SQL_C_CHAR;
        sql_indicator = SQL_NTS;
    }
    break;
    }

    bindParamInfoVec_[nIndex].columnSize = columnSize;
    bindParamInfoVec_[nIndex].sql_c_type = sql_c_type;
    bindParamInfoVec_[nIndex].sql_type   = sqlType;

    for (unsigned int i = 0; i < paramSetSize_; ++i)
    {
        bindParamInfoVec_[nIndex].indArr[i] = sql_indicator;
    }

    unsigned int nMemSize = static_cast<unsigned int>(columnMax) * paramSetSize_;
    if (NULL == bindParamInfoVec_[nIndex].paramValue)
    {
        bindParamInfoVec_[nIndex].memSize    = nMemSize;
        bindParamInfoVec_[nIndex].columnMax  = columnMax;
        bindParamInfoVec_[nIndex].paramValue = new SQLCHAR[nMemSize];
    }
    else
    {
        if (columnMax > bindParamInfoVec_[nIndex].columnMax)
        {
            delete [] (bindParamInfoVec_[nIndex].paramValue);
            bindParamInfoVec_[nIndex].memSize    = nMemSize;
            bindParamInfoVec_[nIndex].columnMax  = columnMax;
            bindParamInfoVec_[nIndex].paramValue = new SQLCHAR[nMemSize];
        }
    }

    memset(bindParamInfoVec_[nIndex].paramValue, 0, nMemSize);
}

ResultSet* PreparedStatement::executeQuery()
{
    SQLRETURN retCode = SQLExecute(hStmt_);
    if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
    {
        handleError(__FILE__, __LINE__);
        return NULL;
    }
	
    return new ResultSet(hStmt_, conn_);
}

long PreparedStatement::executeUpdate()
{
    SQLRETURN retCode = SQLExecute(hStmt_);
    if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO) && (retCode != SQL_NO_DATA))
    {
        handleError(__FILE__, __LINE__);
    }

    // 开始使用int result作为返回值，在加了-O2优化编译后运行存在问题
    // 改成long result后问题解决！（内存溢出） 2017-04-21 15:12:27
    SQLLEN result = -1;
    retCode = SQLRowCount(hStmt_, static_cast<SQLLEN* >(&result));
    if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
    {
        handleError(__FILE__, __LINE__);
    }
    return static_cast<long>(result);
}

void PreparedStatement::setSmallInt(int index, short int value)
{
    setParamAttr(index, SQL_SMALLINT, sizeof(SQLSMALLINT));

    SQLRETURN retCode = SQLBindParameter(hStmt_,
                                         static_cast<SQLUSMALLINT>(index),
                                         SQL_PARAM_INPUT,
                                         bindParamInfoVec_[index - 1].sql_c_type,
                                         bindParamInfoVec_[index - 1].sql_type,
                                         bindParamInfoVec_[index - 1].columnSize,
                                         0,
                                         bindParamInfoVec_[index - 1].paramValue,
                                         bindParamInfoVec_[index - 1].columnMax,
                                         bindParamInfoVec_[index - 1].indArr);
    if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
    {
        handleError(__FILE__, __LINE__);
        return;
    }
    memcpy(bindParamInfoVec_[index - 1].paramValue, &value, sizeof(value));
    paramNum_++;
}

void PreparedStatement::setInt(int index, int value)
{
    setParamAttr(index, SQL_INTEGER, sizeof(SQLINTEGER));

    SQLRETURN retCode = SQLBindParameter(hStmt_,
                                         static_cast<SQLUSMALLINT>(index),
                                         SQL_PARAM_INPUT,
                                         bindParamInfoVec_[index - 1].sql_c_type,
                                         bindParamInfoVec_[index - 1].sql_type,
                                         bindParamInfoVec_[index - 1].columnSize,
                                         0,
                                         bindParamInfoVec_[index - 1].paramValue,
                                         bindParamInfoVec_[index - 1].columnMax,
                                         bindParamInfoVec_[index - 1].indArr);
    if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
    {
        handleError(__FILE__, __LINE__);
        return;
    }
    memcpy(bindParamInfoVec_[index - 1].paramValue, &value, sizeof(value));
    paramNum_++;
}

void PreparedStatement::setInt64(int index, int64_t value)
{
    setParamAttr(index, SQL_BIGINT, sizeof(SQLBIGINT));
    char buffer[32] = { 0 };
    snprintf(buffer, sizeof(buffer), "%ld", value);
    setString(index, buffer);
}

void PreparedStatement::setString(int index, const std::string & value)
{
    setParamAttr(index, SQL_VARCHAR, static_cast<int>(value.size()) + 1);
    SQLRETURN retCode = SQLBindParameter(hStmt_,
                                         static_cast<SQLUSMALLINT>(index),
                                         SQL_PARAM_INPUT,
                                         bindParamInfoVec_[index - 1].sql_c_type,
                                         bindParamInfoVec_[index - 1].sql_type,
                                         bindParamInfoVec_[index - 1].columnSize,
                                         0,
                                         bindParamInfoVec_[index - 1].paramValue,
                                         bindParamInfoVec_[index - 1].columnMax,
                                         bindParamInfoVec_[index - 1].indArr);
    if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
    {
        handleError(__FILE__, __LINE__);
        return;
    }

    memcpy(bindParamInfoVec_[index - 1].paramValue, value.c_str(), value.length());
    paramNum_++;
}

void PreparedStatement::setFloat(int index, float value, int precise)
{
    setParamAttr(index, SQL_REAL, sizeof(SQLREAL));

    SQLRETURN retCode = SQLBindParameter(hStmt_,
                                         static_cast<SQLUSMALLINT>(index),
                                         SQL_PARAM_INPUT,
                                         bindParamInfoVec_[index - 1].sql_c_type,
                                         bindParamInfoVec_[index - 1].sql_type,
                                         bindParamInfoVec_[index - 1].columnSize,
                                         static_cast<SQLSMALLINT>(precise),
                                         bindParamInfoVec_[index - 1].paramValue,
                                         bindParamInfoVec_[index - 1].columnMax,
                                         bindParamInfoVec_[index - 1].indArr);
    if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
    {
        handleError(__FILE__, __LINE__);
        return;
    }
    memcpy(bindParamInfoVec_[index - 1].paramValue, &value, sizeof(value));
    paramNum_++;
}

void PreparedStatement::setDouble(int index, double value, int precise)
{
    setParamAttr(index, SQL_DOUBLE, sizeof(SQLDOUBLE));

    SQLRETURN retCode = SQLBindParameter(hStmt_,
                                         static_cast<SQLUSMALLINT>(index),
                                         SQL_PARAM_INPUT,
                                         bindParamInfoVec_[index - 1].sql_c_type,
                                         bindParamInfoVec_[index - 1].sql_type,
                                         bindParamInfoVec_[index - 1].columnSize,
                                         static_cast<SQLSMALLINT>(precise),
                                         bindParamInfoVec_[index - 1].paramValue,
                                         bindParamInfoVec_[index - 1].columnMax,
                                         bindParamInfoVec_[index - 1].indArr);
    if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
    {
        handleError(__FILE__, __LINE__);
        return;
    }
    memcpy(bindParamInfoVec_[index - 1].paramValue, &value, sizeof(value));
    paramNum_++;
}

void PreparedStatement::setDecimal(int index, double value, int precise)
{
    if (precise <= 0)
    {
        return;
    }

    int number = 1;
    // std::cout << "precise = " << precise << ", number = " << number << std::endl;
    while (precise-- != 0)
    {
        number *= 10;
    }
    // std::cout << "precise = " << precise << ", number = " << number << std::endl;
    value = static_cast<int>(value * number) / static_cast<double>(number);
    char buffer[32] = { 0 };
    snprintf(buffer, sizeof(buffer), "%lf", value);

    setParamAttr(index, SQL_VARCHAR, static_cast<int>(sizeof(buffer) + 1));
    SQLRETURN retCode = SQLBindParameter(hStmt_,
                                         static_cast<SQLUSMALLINT>(index),
                                         SQL_PARAM_INPUT,
                                         bindParamInfoVec_[index - 1].sql_c_type,
                                         bindParamInfoVec_[index - 1].sql_type,
                                         bindParamInfoVec_[index - 1].columnSize,
                                         0,
                                         bindParamInfoVec_[index - 1].paramValue,
                                         bindParamInfoVec_[index - 1].columnMax,
                                         bindParamInfoVec_[index - 1].indArr);
    if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
    {
        handleError(__FILE__, __LINE__);
        return;
    }

    memcpy(bindParamInfoVec_[index - 1].paramValue, buffer, strlen(buffer));
    paramNum_++;
}

void PreparedStatement::handleError(const char* FILE, int LINE)
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
    snprintf(buffer, sizeof(buffer), "(%s:%d) errstate(%s), errnative(%d), errmsg(%s)",
             FILE, LINE, errstate, errnative, errmsg);

    throw ODBC::Exception(buffer);
}

} // namespace ODBC

