#ifndef __ODBC_CONNECTION_H__
#define __ODBC_CONNECTION_H__

#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <string>

#define LOGINTIMEOUT      5
#define CONNECTIONTIMEOUT 60
#define QUERYTIMEOUT      5*60

namespace ODBC
{

class PreparedStatement;

class Connection
{
public:
	Connection();
	~Connection();
	void init(const std::string & dbDSName,
		  const std::string & userName,
		  const std::string & passWord);
		  void close();
		  bool connect(unsigned int seconds = 60);
	bool isClosed() const;
	bool checkCnn() const;
	void setCheck(bool check);
	void setAutoCommit(bool b);
	void commit();
	void rollback();
	PreparedStatement* prepareStatement(const char* strSql, int timeout = QUERYTIMEOUT);

private:
	Connection( const Connection & );
	Connection & operator = (const Connection &);
	void handleError(const char* FILE = __FILE__, int LINE = __LINE__);

private:
	SQLHENV hEnv_;
	SQLHDBC hDbc_;

	bool state_;
	bool check_;
	
	std::string dbDSName_;
	std::string userName_;
	std::string passWord_;
};

} // namespace ODBC

#endif // __ODBC_CONNECTION_H__

