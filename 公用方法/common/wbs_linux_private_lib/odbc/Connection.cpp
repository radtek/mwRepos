#include <odbc/Connection.h>
#include <odbc/Exception.h>
#include <odbc/PreparedStatement.h>
#include <stdio.h>

namespace ODBC
{

Connection::Connection()
    : hEnv_(NULL),
      hDbc_(NULL),
      state_(false),
      check_(false),
      dbDSName_(""),
      userName_(""),
      passWord_("")
{
}

Connection::~Connection()
{
    close();
}

void Connection::init(const std::string & dbDSName,
                      const std::string & userName,
                      const std::string & passWord)
{
    state_  = false;
    check_  = false;
    dbDSName_ = dbDSName;
    userName_ = userName;
    passWord_ = passWord;

    SQLRETURN retCode;
    retCode = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &hEnv_);
    if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
    {
        // std::cout << "Erro AllocHandle: " << retCode << std::endl;
        return;
    }

    retCode = SQLSetEnvAttr(hEnv_,
                            SQL_ATTR_ODBC_VERSION,
                            reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3),
                            SQL_IS_INTEGER);
    if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
    {
        // std::cout << "Erro AllocHandle: " << retCode << std::endl;
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv_);
        return;
    }

    retCode = SQLAllocHandle(SQL_HANDLE_DBC, hEnv_, &hDbc_);
    if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
    {
        // std::cout << "Erro AllocHandle: " << retCode << std::endl;
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv_);
        return;
    }
}

bool Connection::connect(unsigned int seconds)
{
    if (state_ == false)
    {
    	SQLSetConnectAttr(hDbc_, SQL_LOGIN_TIMEOUT, reinterpret_cast<SQLPOINTER>(LOGINTIMEOUT), SQL_IS_INTEGER);
        SQLRETURN retCode = SQLConnect(hDbc_,
                                       static_cast<SQLCHAR* >(static_cast<void* >(const_cast<char*>(dbDSName_.c_str()))),
                                       SQL_NTS,
                                       static_cast<SQLCHAR* >(static_cast<void* >(const_cast<char*>(userName_.c_str()))),
                                       SQL_NTS,
                                       static_cast<SQLCHAR* >(static_cast<void* >(const_cast<char*>(passWord_.c_str()))),
                                       SQL_NTS);
        if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
        {
        	handleError(__FILE__, __LINE__);
            SQLFreeHandle(SQL_HANDLE_DBC, hDbc_);
            SQLFreeHandle(SQL_HANDLE_ENV, hEnv_);
            return false;
        }
        SQLSetConnectAttr(hDbc_, SQL_ATTR_CONNECTION_TIMEOUT, reinterpret_cast<SQLPOINTER>(CONNECTIONTIMEOUT), SQL_IS_INTEGER);
    }

    state_ = true;
    return true;
}

bool Connection::isClosed() const
{
    return (state_ == false);
}

bool Connection::checkCnn() const
{
	return (check_ == true);
}

void Connection::setCheck(bool check)
{
	check_ = check;
}

void Connection::close()
{
    if (hDbc_)
    {
        SQLDisconnect(hDbc_);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc_);
        hDbc_ = NULL;
    }

    if (hEnv_)
    {
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv_);
        hDbc_ = NULL;
    }
    state_ = false;
}

PreparedStatement* Connection::prepareStatement(const char* strSql, int timeout)
{
    return new PreparedStatement(hDbc_, strSql, this, timeout);
}

void Connection::setAutoCommit(bool b)
{
    SQLRETURN retCode;
    if (b)
    {
        // 设置为自动提交
        retCode = SQLSetConnectAttr(hDbc_,
                                    SQL_ATTR_AUTOCOMMIT,
                                    reinterpret_cast<SQLPOINTER>(SQL_AUTOCOMMIT_ON),
                                    SQL_IS_POINTER);
    }
    else
    {
        // 关闭自动提交，需要执行commit()/rollback(),打开事务属性
        retCode = SQLSetConnectAttr(hDbc_,
                                    SQL_ATTR_AUTOCOMMIT,
                                    reinterpret_cast<SQLPOINTER>(SQL_AUTOCOMMIT_OFF),
                                    SQL_IS_POINTER);
    }

    if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
    {
        handleError(__FILE__, __LINE__);
    }
}

void Connection::commit()
{
    // 提交事务
    // std::cout << "hDbc_: " << static_cast<void* >(hDbc_) << std::endl;
    SQLRETURN retCode = SQLEndTran(SQL_HANDLE_DBC, hDbc_, SQL_COMMIT);
    if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
    {
        handleError(__FILE__, __LINE__);
    }
}

void Connection::rollback()
{
    // 回滚事务
    // std::cout << "hDbc_: " << static_cast<void* >(hDbc_) << std::endl;
    SQLRETURN retCode = SQLEndTran(SQL_HANDLE_DBC, hDbc_, SQL_ROLLBACK);
    if ((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))
    {
        handleError(__FILE__, __LINE__);
    }
}

void Connection::handleError(const char* FILE, int LINE)
{
	setCheck(true);
	
    SQLSMALLINT errmsglen;
    SQLINTEGER errnative;

    UCHAR errmsg[255];
    UCHAR errstate[5];

    char buffer[1024] = { 0 };

    SQLGetDiagRec(SQL_HANDLE_DBC, hDbc_, 1, errstate, &errnative, errmsg, sizeof(errmsg), &errmsglen);
    snprintf(buffer, sizeof(buffer), "(%s:%d) errstate(%s), errnative(%d), errmsg(%s)",
             FILE, LINE, errstate, errnative, errmsg);

    throw ODBC::Exception(buffer);
}

} // namespace ODBC
