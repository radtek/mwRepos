/*
* OCIDB.cpp
*
*    Created .: 2010-6-4
*            Author: pxh
*/
#include "OCIDB.h"
#include "OCIException.h"
#include "OCIError.h"

OCIDB::OCIDB()
{
    BindInitVars();
}

OCIDB::~OCIDB()
{

}

void OCIDB::SetDataParam(char *user,char *pwd,char *db)
{
    strcpy(m_sUser,user);
    strcpy(m_sPwd,pwd);
    strcpy(m_sDBName,db);
}

int OCIDB::Single_Conn()
{
    try
    {
        sword errno;
        errno = OCIEnvCreate( &m_pOCIEnv, OCI_OBJECT, (dvoid *)0, (dvoid *(*)(dvoid *, size_t))0, (dvoid *(*)(dvoid *, dvoid *, size_t))0, (void (*)(dvoid *, dvoid *))0, (size_t)0, (dvoid **)0);
        if(errno)
        {
            throw OCIException(errno, (char *)"OCIDB::Single_Conn OCIEnvCreate");
        }

        errno = OCIHandleAlloc( (dvoid *)m_pOCIEnv, (dvoid **)&m_pOCIError, OCI_HTYPE_ERROR, (size_t)0, (dvoid **)0);
            if(errno)
            {
                throw OCIException(errno, (char *)"OCIDB::Single_Conn OCIHandleAlloc");
            }

            //���ӷ�����(���û���ʽ����)
            errno = OCILogon(m_pOCIEnv, m_pOCIError, &m_pOCISvcCtx, (const OraText*)m_sUser,
                strlen(m_sUser), (const OraText*)m_sPwd, strlen(m_sPwd), (const OraText*)m_sDBName,
                strlen(m_sDBName));
            if(errno)
            {
                 throw OCIException(errno, m_pOCIError, (char *)"OCIDB::Single_Conn OCILogin2");
            }
           return 0;
    }
    catch ( OCIException &ex)
    {
            //std::cout << ex.GetErrFunc() << ex.GetErrMsg() << //std::endl;
            return -1;
    }
}

void OCIDB::Single_Disc()
{
    sword errno;
    /* Log off */
    errno = OCILogoff(m_pOCISvcCtx, this ->m_pOCIError);
    if ( errno )
    {
        OCIError::PrintError(errno, (char *)"OCIDB::Single_Disc OCILogoff");
    }

    /* Free errno */
    errno = OCIHandleFree((dvoid *)this ->m_pOCIError, (ub4)OCI_HTYPE_ERROR);
    if ( errno )
    {
        OCIError::PrintError(errno, (char *)"OCIDB::Single_Disc OCIHandleFree");
    }

    /* Free Env Handle */
    errno = OCIHandleFree((dvoid *)this ->m_pOCIEnv, (ub4)OCI_HTYPE_ENV);
    if ( errno )
    {
        OCIError::PrintError(errno, (char *)"OCIDB::Single_Disc OCIHandleFree");
    }
}

int OCIDB::Multiple_Conn()
{
    try
    {
        sword errno;
        //����OCI����
        errno = OCIInitialize( (ub4)OCI_DEFAULT, (dvoid *)0, (dvoid *(*)(dvoid *, size_t))0, (dvoid *(*)(dvoid *, dvoid *, size_t))0, (void (*)(dvoid *, dvoid *))0 );
        if ( errno )
        {
            throw OCIException(errno, (char *)"OCIDB::Multiple_Conn OCIInitialize");
        }
        errno = OCIEnvInit( (OCIEnv **)&this ->m_pOCIEnv, OCI_DEFAULT, (size_t)0, (dvoid **)0);
        if ( errno )
        {
            throw OCIException(errno, (char *)"OCIDB::Multiple_Conn OCIEnvInit");
        }

        //���������ڲ���OCI������Ϣ
        errno = OCIHandleAlloc( (dvoid *)this ->m_pOCIEnv, (dvoid **)&this ->m_pOCIError, OCI_HTYPE_ERROR, (size_t)0, (dvoid **)0);
        if ( errno )
        {
            throw OCIException(errno, (char *)"OCIDB::Multiple_Conn OCIHandleAlloc Error");
        }

        //�����������ľ��OCISvcCtx
        errno = OCIHandleAlloc( (dvoid *)this ->m_pOCIEnv, (dvoid **)&this ->m_pOCISvcCtx, OCI_HTYPE_SVCCTX, (size_t)0, (dvoid **)0);
        if ( errno )
        {
            throw OCIException(errno, (char *)"OCIDB::Multiple_Conn OCIHandleAlloc SvcCtx");
        }

        //���������������OCIServer
        errno = OCIHandleAlloc( (dvoid *)this ->m_pOCIEnv, (dvoid **)&this ->m_pOCIServer, OCI_HTYPE_SERVER, (size_t)0, (dvoid **)0);
        if ( errno )
        {
            throw OCIException(errno, (char *)"OCIDB::Multiple_Conn OCIHandleAlloc Server");
        }

        //���þ���������õ�����Ϊ����
        errno = OCIAttrSet( (dvoid *)this ->m_pOCISvcCtx, OCI_HTYPE_SVCCTX, (dvoid *)this ->m_pOCIServer, 0, OCI_ATTR_SERVER, (OCIError *)this ->m_pOCIError);
        if ( errno )
        {
            throw OCIException(errno, (char *)"OCIDB::Multiple_Conn OCIAttrSet Server");
        }

        //���ӷ�����(���û���ʽ����)
        errno = OCIServerAttach(this ->m_pOCIServer, this ->m_pOCIError, (text *)this ->m_sDBName, strlen(this ->m_sDBName), OCI_DEFAULT);
        if ( errno )
        {
            throw OCIException(errno, (char *)"OCIDB::Multiple_Conn OCIServerAttach");
        }

        //������������OCISession
        errno = OCIHandleAlloc( (dvoid *)this ->m_pOCIEnv, (dvoid **)&this ->m_pOCISession, (ub4)OCI_HTYPE_SESSION, (size_t)0, (dvoid **)0);
        if ( errno )
        {
            throw OCIException(errno, (char *)"OCIDB::Multiple_Conn OCIHandleAlloc Session");
        }

        //���þ���������õ�����Ϊ����
        errno = OCIAttrSet( (dvoid *)this ->m_pOCISvcCtx, (ub4)OCI_HTYPE_SVCCTX, (dvoid *)this ->m_pOCISession, (ub4)0, (ub4)OCI_ATTR_SESSION, (OCIError *)this ->m_pOCIError);
        if ( errno )
        {
            throw OCIException(errno, (char *)"OCIDB::Multiple_Conn OCIAttrSet Session");
        }

        //���þ���������õ�����Ϊ�������ݵ��û���
        errno = OCIAttrSet( (dvoid *)this ->m_pOCISession, OCI_HTYPE_SESSION, (dvoid *)this ->m_sUser, strlen(this ->m_sUser), OCI_ATTR_USERNAME, (OCIError *)this ->m_pOCIError);
        if ( errno )
        {
            throw OCIException(errno, (char *)"OCIDB::Multiple_Conn OCIAttrSet UserName");
        }

        //���þ���������õ�����Ϊ�û�����
        errno = OCIAttrSet( (dvoid *)this ->m_pOCISession, OCI_HTYPE_SESSION, (dvoid *)this ->m_sPwd, (ub4)strlen(this ->m_sPwd), (ub4)OCI_ATTR_PASSWORD, (OCIError *)this ->m_pOCIError);
        if ( errno )
        {
            throw OCIException(errno, (char *)"OCIDB::Multiple_Conn OCIAttrSet Password");
        }

        //��ʼһ���ỰOCI_CRED_RDBMS:�����ݿ��û��������������֤
        errno = OCISessionBegin(this ->m_pOCISvcCtx, this ->m_pOCIError, this ->m_pOCISession, OCI_CRED_RDBMS, OCI_DEFAULT);
        if ( errno )
        {
            throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::Multiple_Conn OCISessionBegin");
        }
        return 0;
    }
    catch (OCIException &ex)
    {
        printf("%s %s\r\n", ex.GetErrFunc(), ex.GetErrMsg());
        //cout << ex.GetErrFunc() << ex.GetErrMsg() << //std::endl;
        return -1;
    }
}

void OCIDB::Multiple_Disc()
{
  sword errno;

  /* End Session */
  errno = OCISessionEnd(this ->m_pOCISvcCtx, this ->m_pOCIError, this ->m_pOCISession, (ub4)OCI_DEFAULT);
  if ( errno )
  {
    OCIError::PrintError(errno, (char *)"OCIDB::Multiple_Disc OCISessionEnd");
  }

  /* Free Session */
  errno = OCIHandleFree( (dvoid *)this ->m_pOCISession, (ub4)OCI_HTYPE_SESSION );
  if ( errno )
  {
    OCIError::PrintError(errno, (char *)"OCIDB::Multiple_Disc OCIHandleFree Session");
  }

  /* Detach Server */
  errno = OCIServerDetach(this ->m_pOCIServer, this ->m_pOCIError, OCI_DEFAULT);
  if ( errno )
  {
    OCIError::PrintError(errno, (char *)"OCIDB::Multiple_Disc OCIServerDetach");
  }

  /* Free Server */
  errno = OCIHandleFree( (dvoid *)this ->m_pOCIServer, (ub4)OCI_HTYPE_SERVER );
  if ( errno )
  {
    OCIError::PrintError(errno, (char *)"OCIDB::Multiple_Disc OCIHandleFree Server");
  }

  /* Free ServerContext */
  errno = OCIHandleFree( (dvoid *)this ->m_pOCISvcCtx, (ub4)OCI_HTYPE_SVCCTX );
  if ( errno )
  {
    OCIError::PrintError(errno, (char *)"OCIDB::Multiple_Disc OCIHandleFree ServerContext");
  }

  /* Free Error */
  errno = OCIHandleFree( (dvoid *)this ->m_pOCIError, (ub4)OCI_HTYPE_ERROR );
  if ( errno )
  {
    OCIError::PrintError(errno, (char *)"OCIDB::Multiple_Disc OCIHandleFree Error");
  }

  /* Free Env */
  errno = OCIHandleFree( (dvoid *)this ->m_pOCIEnv, (ub4)OCI_HTYPE_ENV );
  if ( errno )
  {
    OCIError::PrintError(errno, (char *)"OCIDB::Multiple_Disc OCIHandleFree Env");
  }
}

int OCIDB::ExcuteSQL(char * sql)
{
  sword errno;
  try
  {
    /* Allocate Stmt */
    errno = OCIHandleAlloc( (dvoid *)this ->m_pOCIEnv, (dvoid **)&this ->m_pOCIStmt, OCI_HTYPE_STMT, (size_t)0, (dvoid **)0);
    if ( errno )
    {
      OCIError::PrintError(errno, (char *)"OCIDB::ExcuteSQL OCIHandleAlloc");
    }

    errno = OCIStmtPrepare(this ->m_pOCIStmt, this ->m_pOCIError, (const OraText *)sql, (ub4)strlen(sql), (ub4)OCI_NTV_SYNTAX, (ub4)OCI_DEFAULT);
    if ( errno )
    {
      OCIError::PrintError(errno, (char *)"OCIDB::ExcuteSQL OCIStmtPrepare");
    }

    errno = OCIStmtExecute(this ->m_pOCISvcCtx, this ->m_pOCIStmt, this ->m_pOCIError, (ub4)1, (ub4)0, (const OCISnapshot *)NULL, (OCISnapshot *)NULL, OCI_DEFAULT);
    if ( errno )
    {
      OCIError::PrintError(errno, (char *)"OCIDB::ExcuteSQL OCIStmtExecute");
    }

    errno = OCITransCommit( this ->m_pOCISvcCtx, this ->m_pOCIError, 0);
    if ( errno )
    {
      OCIError::PrintError(errno, (char *)"OCIDB::ExcuteSQL OCITransCommit");
    }

    errno = OCIHandleFree( (dvoid *)this ->m_pOCIStmt, (ub4)OCI_HTYPE_STMT );
    if ( errno )
    {
      OCIError::PrintError(errno, (char *)"OCIDB::ExcuteSQL OCIHandleFree");
    }
    return 0;
  }
  catch (OCIException &ex)
  {
    /* Rollback */
    errno = OCITransRollback( this ->m_pOCISvcCtx, this ->m_pOCIError, 0);
    if ( errno )
    {
      OCIError::PrintError(errno, (char *)"OCIDB::ExcuteSQL OCITransRollback");
    }
    /* Free Stmt */
    errno = OCIHandleFree( (dvoid *)this ->m_pOCIStmt, (ub4)OCI_HTYPE_STMT);
    if ( errno )
    {
      OCIError::PrintError(errno, (char *)"OCIDB::ExcuteSQL OCIHandleFree");
    }
    //std::cout << ex.GetErrFunc() << ex.GetErrMsg() << //std::endl;
    return -1;
  }
}

void OCIDB::BindInitVars()
{
    int i;
    for( i = 0 ; i < MAX_BINDVAR_COUNT - 1 ; i++ )
    {
        this ->m_BindVars[i].VarType    = 0;
        this ->m_BindVars[i].VarName   = 0;
        this ->m_BindVars[i].VarLen      = 0;
        this ->m_BindVars[i].ValueChar = 0;
    }
    this ->m_iBindVarsCount = 0;
}

void OCIDB::BindClearVars()
{
    for(int i = 0 ; i < m_iBindVarsCount - 1 ; i++ )
    {
        delete [] this ->m_BindVars[i].VarName;
        if ( this ->m_BindVars[i].VarType == SQLT_STR )
            delete [] this ->m_BindVars[i].ValueChar;
        if ( this ->m_BindVars[i].VarType == SQLT_INT )
            delete [] this ->m_BindVars[i].ValueInt;

        this ->m_BindVars[i].VarType    = 0;
        this ->m_BindVars[i].VarName   = 0;
        this ->m_BindVars[i].VarLen      = 0;
        this ->m_BindVars[i].ValueChar = 0;
    }
    this ->m_iBindVarsCount = 0;
}

void OCIDB::BindAddVar(char * name, char * value)
{
  if ( this ->m_iBindVarsCount >= MAX_BINDVAR_COUNT )
    return;
  this ->m_BindVars[this ->m_iBindVarsCount].VarType = SQLT_STR;
  this ->m_BindVars[this ->m_iBindVarsCount].VarName = new char[strlen(name) + 1];
  strcpy( this ->m_BindVars[this ->m_iBindVarsCount].VarName, name);

  this ->m_BindVars[this ->m_iBindVarsCount].ValueChar = new char[strlen(value) + 1];
  strcpy( this ->m_BindVars[this ->m_iBindVarsCount].ValueChar, value);
  this ->m_BindVars[this ->m_iBindVarsCount].VarLen = strlen(value) + 1;

  this ->m_iBindVarsCount++;
}

void OCIDB::BindAddVar(char * name, int * value)
{
  if ( this ->m_iBindVarsCount >= MAX_BINDVAR_COUNT )
    return;
  this ->m_BindVars[this ->m_iBindVarsCount].VarType = SQLT_INT;
  this ->m_BindVars[this ->m_iBindVarsCount].VarName = new char[strlen(name) + 1];
  strcpy( this ->m_BindVars[this ->m_iBindVarsCount].VarName, name);

  this ->m_BindVars[this ->m_iBindVarsCount].ValueInt = new int;
  this ->m_BindVars[this ->m_iBindVarsCount].ValueInt = value;
  this ->m_BindVars[this ->m_iBindVarsCount].VarLen = sizeof(int);

  this ->m_iBindVarsCount++;
}

int OCIDB::BindSQL(char * sql)
{
  if( this ->m_iBindVarsCount == 0 )
    return -1;
  sword errno;
  try
  {
    /* Allocate Stmt */
    errno = OCIHandleAlloc( (dvoid *)this ->m_pOCIEnv, (dvoid **)&this ->m_pOCIStmt, OCI_HTYPE_STMT, (size_t)0, (dvoid **)0);
    if ( errno )
    {
      throw OCIException(errno, (char *)"OCIDB::BindSQL OCIHandleAlloc Stmt");
    }
    /* Prepare Stmt */
    errno = OCIStmtPrepare( this ->m_pOCIStmt, this ->m_pOCIError, (const OraText *)sql, (ub4)strlen(sql), (ub4)OCI_NTV_SYNTAX, (ub4)OCI_DEFAULT );
    if ( errno )
    {
      throw OCIException(errno, (char *)"OCIDB::BindSQL OCIStmtPrepare");
    }
    /* Bind Vars */
    int i;
    for ( i = 0 ; i < m_iBindVarsCount - 1 ; i++ )
    {
      /* Bind By Name */
      errno = OCIBindByName( this ->m_pOCIStmt, &this ->m_pOCIBind, this ->m_pOCIError, (text *)this ->m_BindVars[i].VarName, -1,
          (dvoid *)this ->m_BindVars[i].ValueChar, this ->m_BindVars[i].VarLen, this ->m_BindVars[i].VarType, (dvoid *)0, (ub2 *)0, (ub2 *)0, (ub4)0, (ub4 *)0, OCI_DEFAULT);

      if ( errno )
      {
        throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::BindSQL OCIBindByName");
      }
      /*
      errno = OCIBindByPos( this ->m_pOCIStmt, &this ->m_pOCIBind, this ->m_pOCIError, i + 1, (dvoid *)this ->m_BindVars[i].ValueChar,
          this ->m_BindVars[i].VarLen, this ->m_BindVars[i].VarType, (dvoid *)0, (ub2 *)0, (ub2 *)0, (ub4)0, (ub4 *)0, OCI_DEFAULT);

      if ( errno )
      {
        throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::BindSQL OCIBindByPos");
      }
      */
    }
    /* Execute SQL */
    errno = OCIStmtExecute( this ->m_pOCISvcCtx, this ->m_pOCIStmt, this ->m_pOCIError, (ub4)1, (ub4)0, (const OCISnapshot *)NULL, (OCISnapshot *)NULL, OCI_DEFAULT);
    if ( errno )
    {
      throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::BindSQL OCIStmtExecute");
    }
    /*  Commit */
    errno = OCITransCommit( this ->m_pOCISvcCtx, this ->m_pOCIError, 0);
    if ( errno )
    {
      throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::BindSQL OCITransCommit");
    }
    /* Free Stmt */
    errno = OCIHandleFree( (dvoid *)this ->m_pOCIStmt, (ub4)OCI_HTYPE_STMT );
    if ( errno )
    {
      throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::BindSQL OCIHandleFree");
    }
    /* Clear Vars */
    this ->BindClearVars();
    return 0;
  }
  catch ( OCIException &ex )
  {
    /* Rollback */
    errno = OCITransRollback( this ->m_pOCISvcCtx, this ->m_pOCIError, 0);
    if ( errno )
    {
      OCIError::PrintError(errno, (char *)"OCIDB::BindSQL OCITransRollback");
    }
    /* Free Stmt */
    errno = OCIHandleFree( (dvoid *)this ->m_pOCIStmt, (ub4)OCI_HTYPE_STMT);
    if ( errno )
    {
      OCIError::PrintError(errno, (char *)"OCIDB::BindSQL OCIHandleFree");
    }
    this ->BindClearVars();
    //std::cout << ex.GetErrFunc() << ex.GetErrMsg() << //std::endl;
    return -1;
  }
}

int OCIDB::UserPrepare(char * sql)
{
  sword errno;
  try
  {
    /* Allocate Stmt */
    errno = OCIHandleAlloc( (dvoid *)this ->m_pOCIEnv, (dvoid **)&this ->m_pOCIStmt, OCI_HTYPE_STMT, (size_t)0, (dvoid **)0 );
    if ( errno )
    {
      throw OCIException(errno, (char *)"OCIDB::UserPrepare OCIHandleAlloc");
    }
    /* Prepare Stmt */
    errno = OCIStmtPrepare( this ->m_pOCIStmt, this ->m_pOCIError, (const OraText *)sql, (ub4)strlen(sql), (ub4)OCI_NTV_SYNTAX, (ub4)OCI_DEFAULT );
    if ( errno )
    {
      throw OCIException(errno, (char *)"OCIDB::UserPrepare OCIStmtPrepare");
    }
    return 0;
  }
  catch ( OCIException &ex )
  {
    /* Free Stmt */
    errno = OCIHandleFree( (dvoid *)this ->m_pOCIStmt, (ub4)OCI_HTYPE_STMT );
    if ( errno )
    {
      OCIError::PrintError(errno, (char *)"OCIDB::UserPrepare Ex OCIHandleFree");
    }
    //std::cout << ex.GetErrFunc() << ex.GetErrMsg() << //std::endl;
    return -1;
  }
}

int OCIDB::UserFree()
{
  sword errno;
  /* Free Stmt */
  errno = OCIHandleFree( (dvoid *)this ->m_pOCIStmt, (ub4)OCI_HTYPE_STMT );
  if ( errno )
  {
    OCIError::PrintError(errno, (char *)"OCIDB::UserFree OCIHandleFree");
    return -1;
  }
  return 0;
}

int OCIDB::UserBind(char * name, char * value)
{
  sword errno;
  try
  {
    errno = OCIBindByName( this ->m_pOCIStmt, &this ->m_pOCIBind, this ->m_pOCIError, (text *)name, -1,
        (dvoid *)value, strlen(value) + 1, SQLT_STR, (dvoid *)0, (ub2 *)0, (ub2 *)0, (ub4)0, (ub4 *)0, OCI_DEFAULT );
    if ( errno )
    {
      throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserBind OCIBindByName");
    }
    return 0;
  }
  catch ( OCIException &ex )
  {
    /* Free Stmt */
    errno = OCIHandleFree( (dvoid *)this ->m_pOCIStmt, (ub4)OCI_HTYPE_STMT );
    if ( errno )
    {
      OCIError::PrintError(errno, (char *)"OCIDB::UserBind Ex OCIHandleFree");
    }
    //std::cout << ex.GetErrFunc() << ex.GetErrMsg() << //std::endl;
    return -1;
  }
}

int OCIDB::UserBind(char * name, int value)
{
  sword errno;
  try
  {
    int val = value;
    errno = OCIBindByName( this ->m_pOCIStmt, &this ->m_pOCIBind, this ->m_pOCIError, (text *)name, -1,
        (dvoid *)&val, sizeof(int), SQLT_INT, (dvoid *)0, (ub2 *)0, (ub2 *)0, (ub4)0, (ub4 *)0, OCI_DEFAULT );
    if ( errno )
    {
      throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserBind OCIBindByName");
    }
    return 0;
  }
  catch ( OCIException &ex )
  {
    /* Free Stmt */
    errno = OCIHandleFree( (dvoid *)this ->m_pOCIStmt, (ub4)OCI_HTYPE_STMT );
    if ( errno )
    {
      OCIError::PrintError(errno, (char *)"OCIDB::UserBind Ex OCIHandleFree");
    }
    //std::cout << ex.GetErrFunc() << ex.GetErrMsg() << //std::endl;
    return -1;
  }
}

int OCIDB::UserExecute()
{
  sword errno;
  try
  {
    errno = OCIStmtExecute( this ->m_pOCISvcCtx, this ->m_pOCIStmt, this ->m_pOCIError, (ub4)1, (ub4)0,
        (const OCISnapshot *)NULL, (OCISnapshot *)NULL, OCI_DEFAULT );
    if ( errno )
    {
      throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserExecute OCIBindByName");
    }
    return 0;
  }
  catch ( OCIException &ex )
  {
    /* Free Stmt */
    errno = OCIHandleFree( (dvoid *)this ->m_pOCIStmt, (ub4)OCI_HTYPE_STMT );
    if ( errno )
    {
      OCIError::PrintError(errno, (char *)"OCIDB::UserExecute Ex OCIHandleFree");
    }
    //std::cout << ex.GetErrFunc() << ex.GetErrMsg() << //std::endl;
    return -1;
  }
}

int OCIDB::UserCommit()
{
  sword errno;
  errno = OCITransCommit( this ->m_pOCISvcCtx, this ->m_pOCIError, 0);
  if ( errno )
  {
    OCIError::PrintError(errno, (char *)"OCIDB::UserCommit OCITransCommit");
    return -1;
  }
  return 0;
}

int OCIDB::UserSelect(char * sql)
{
    sword errno;
    try
    {
         //������
        errno = OCIHandleAlloc(
                (dvoid *)this ->m_pOCIEnv,  //���������ĸ������һ��ΪOCI�������
                (dvoid **)&this ->m_pOCIStmt, //������¾��
                OCI_HTYPE_STMT, //�������SQL �������OCI_HTYPE_STMT
                (size_t)0, //������ڴ���
                (dvoid **)0 //���뵽���ڴ��ָ��
                );
        if ( errno )
        {
            throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserSelect OCIHandleAlloc");
        }
        //׼��SQL���
        errno = OCIStmtPrepare(
                this ->m_pOCIStmt, //�����
                this ->m_pOCIError,
                (const OraText *)sql, //SQL���
                (ub4)strlen(sql), //��䳤��
                (ub4)OCI_NTV_SYNTAX, //�����﷨��ʽ=OCI_NTV_SYNTAX
                (ub4)OCI_DEFAULT //=OCI_DEFAULT
                );
        if ( errno )
        {
            throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserSelect OCIStmtPrepare");
        }
        //ִ��SQL���
        errno = OCIStmtExecute(
                this ->m_pOCISvcCtx, //���񻷾����
                this ->m_pOCIStmt, //�����
                this ->m_pOCIError,
                (ub4)0, //����select��䣬��˵��һ��ִ�ж�ȡ��buffer�еļ�¼�������������ȷ��select��������ص��������ɽ�iters����Ϊ0,��������������䣬iters��ʾ��Щ����ִ�д�������ʱiters����Ϊ0
                (ub4)0, //�ڶ���ִ��ʱ���ò�����ʾ������ϵ����ݱ����еĵڼ�����¼��ʼִ��(����¼ƫ����)��
                (OCISnapshot *)NULL,
                (OCISnapshot *)NULL,
                OCI_DEFAULT //ģʽOCI_DESCRIBE_ONLY������ģʽOCI_COMMIT_ON_SUCCESS���Զ��ύģOCI_EXACT_FETCH:��ȷ��ȡģʽOCI_BATCH_ERRORS��������ִ��ģʽ
                );
        if ( errno )
        {
            throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserSelect OCIStmtExecute");
        }
        //��ȡ�������
        errno = OCIAttrGet(
            this ->m_pOCIStmt, //���ȡ�ľ����
            (ub4)OCI_HTYPE_STMT, //�������
            (dvoid *)&this ->m_iSelectVarsCount, //��ȡ��������
            (ub4 *)0, //����ֵ����
            (ub4)OCI_ATTR_PARAM_COUNT, //��������OCI_ATTR_PARAM_COUNT���������ѡ���б��е�����
            this ->m_pOCIError //������
            );
        if ( errno )
        {
            throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserSelect OCIAttrGet");
        }
        int iLen;
        char * sName;
        ub2 iSize, iPrecision, iType;
        ub1 iScale, iIsNull;

        printf("-----clonumn %d----\n",m_iSelectVarsCount);

        //ȡ�е���Ϣ
        for (int i = 0 ; i < this ->m_iSelectVarsCount ; i++ )
        {
            sName = 0;
            //�õ�����Ϣ
            errno = OCIParamGet( this ->m_pOCIStmt, OCI_HTYPE_STMT, this ->m_pOCIError, (dvoid **)&this ->m_pOCIParam, i+ 1 );
            if ( errno )
            {
                throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserSelect OCIParamGet");
            }
            //�õ�����
            errno = OCIAttrGet( (dvoid *)this ->m_pOCIParam, OCI_DTYPE_PARAM, (dvoid **)&sName, (ub4 *)&iLen, OCI_ATTR_NAME, this ->m_pOCIError );
            if ( errno )
            {
                throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserSelect OCIAttrGet Column Name");
            }
            strncpy( this ->m_SelectVars[i].VarName, sName, iLen );
            this ->m_SelectVars[i].VarName[iLen] = 0;
            //�õ��е���󳤶�
            errno = OCIAttrGet( (dvoid *)this ->m_pOCIParam, OCI_DTYPE_PARAM, (dvoid *)&iSize, (ub4 *)0, OCI_ATTR_DATA_SIZE, this ->m_pOCIError );
            if ( errno )
            {
                throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserSelect OCIAttrGet Data Size");
            }
            this ->m_SelectVars[i].VarSize = iSize;
            //�õ��еľ�ȷλ��
            errno = OCIAttrGet( (dvoid *)this ->m_pOCIParam, OCI_DTYPE_PARAM, (dvoid *)&iPrecision, (ub4 *)0, OCI_ATTR_PRECISION, this ->m_pOCIError );
            if ( errno )
            {
                throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserSelect OCIAttrGet Data Precision");
            }
            this ->m_SelectVars[i].VarPrecision = iPrecision;
            //�õ��е�С������λ��
            errno = OCIAttrGet( (dvoid *)this ->m_pOCIParam, OCI_DTYPE_PARAM, (dvoid *)&iScale, (ub4 *)0, OCI_ATTR_SCALE, this ->m_pOCIError );
            if ( errno )
            {
                throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserSelect OCIAttrGet Data Scale");
            }
            this ->m_SelectVars[i].VarScale = iScale;
            //  Data Is Null
            errno = OCIAttrGet( (dvoid *)this ->m_pOCIParam, OCI_DTYPE_PARAM, (dvoid *)&iIsNull, (ub4 *)0, OCI_ATTR_IS_NULL, this ->m_pOCIError );
            if ( errno )
            {
                throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserSelect OCIAttrGet Data Is Null");
            }
            this ->m_SelectVars[i].VarIsNull = iIsNull;
            //�õ��е�����
            errno = OCIAttrGet( (dvoid *)this ->m_pOCIParam, OCI_DTYPE_PARAM, (dvoid *)&iType, (ub4 *)0, OCI_ATTR_DATA_TYPE, this ->m_pOCIError );
            if ( errno )
            {
                throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserSelect OCIAttrGet Data Type");
            }
                this ->m_SelectVars[i].VarType = iType;
            switch ( this ->m_SelectVars[i].VarType )
            {
                case SQLT_NUM:
                    {
                        if ( this ->m_SelectVars[i].VarScale == 0 )
                        {
                            this ->m_SelectVars[i].ValueInt = new int;
                            this ->m_SelectVars[i].VarType = TYPE_INT;
                              //�����������ִ�������󣬾Ϳ��Ը��ݻ�ȡ�����ֶ������ߵ�������Ŵ��α굱ǰָ���m_SelectVars�м����ˡ�OCIStmtFetchÿִ��һ�Σ��α��Զ�����һ��
                            errno = OCIDefineByPos(
                                this ->m_pOCIStmt, //�����
                                &this ->m_pOCIDefine, //�������������������
                                this ->m_pOCIError,
                                i + 1, //λ�����(��1 ��ʼ)
                                (dvoid *)this ->m_SelectVars[i].ValueInt,  //����ı�����
                                sizeof(int), //��������
                                SQLT_INT, //��������
                                (dvoid *)0, //ָʾ������/ָʾ���������飬������ֶο��ܴ��ڿ�ֵ����Ҫָʾ������������������ʱΪNULL
                                (ub2 *)0, //��ȡ�����ݳ���
                                (ub2 *)0, //�м�����������ָ��
                                OCI_DEFAULT
                                );
                            if ( errno )
                            {
                                throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserSelect OCIDefineByPos");
                            }
                            }
                        else
                        {
                            this ->m_SelectVars[i].ValueFloat = new float;
                            this ->m_SelectVars[i].VarType = TYPE_FLOAT;
                            errno = OCIDefineByPos( this ->m_pOCIStmt, &this ->m_pOCIDefine, this ->m_pOCIError, i + 1,
                                        (dvoid *)this ->m_SelectVars[i].ValueFloat,  sizeof(float), SQLT_FLT, (dvoid *)0, (ub2 *)0, (ub2 *)0, OCI_DEFAULT );
                            if ( errno )
                            {
                                throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserSelect OCIDefineByPos");
                            }
                        }
                            break;
                    }
                case SQLT_CHR:
                    {
                        this ->m_SelectVars[i].ValueChar = new char[this ->m_SelectVars[i].VarSize + 1];
                        this ->m_SelectVars[i].VarType = TYPE_STRING;
                        errno = OCIDefineByPos(
                                this ->m_pOCIStmt,
                                &this ->m_pOCIDefine,
                                this ->m_pOCIError,
                                i + 1,
                                (dvoid *)this ->m_SelectVars[i].ValueChar,
                                this ->m_SelectVars[i].VarSize + 1,
                                SQLT_STR,
                                (dvoid *)0,
                                (ub2 *)0,
                                (ub2 *)0,
                                OCI_DEFAULT
                                );
                        if ( errno )
                        {
                            throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserSelect OCIDefineByPos");
                        }
                        break;
                    }
                case SQLT_DAT:
                    {
                        this ->m_SelectVars[i].ValueChar = new char[this ->m_SelectVars[i].VarSize + 1];
                        this ->m_SelectVars[i].VarType = TYPE_DATE;
                        errno = OCIDefineByPos(
                                    this ->m_pOCIStmt,
                                    &this ->m_pOCIDefine,
                                    this ->m_pOCIError,
                                    i + 1,
                                    (dvoid *)this ->m_SelectVars[i].ValueChar,
                                    this ->m_SelectVars[i].VarSize + 1,
                                    SQLT_DAT,
                                    (dvoid *)0,
                                    (ub2 *)0,
                                    (ub2 *)0,
                                    OCI_DEFAULT
                                    );
                        if ( errno )
                        {
                            throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserSelect OCIDefineByPos");
                        }
                        break;
                    }
                case SQLT_BLOB:
                    {
                        /* Allocate lobLocator */
                        errno = OCIDescriptorAlloc( (dvoid *)this ->m_pOCIEnv, (dvoid **)&this ->m_SelectVars[i].lobLocator, (ub4)OCI_DTYPE_LOB, (size_t)0, (dvoid **)0 );
                        if ( errno )
                        {
                            throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserSelect OCIDe.orAlloc");
                        }
                        this ->m_SelectVars[i].VarType = TYPE_BLOB;
                        errno = OCIDefineByPos( this ->m_pOCIStmt, &this ->m_pOCIDefine, this ->m_pOCIError, (ub4)i + 1, (dvoid *)&this ->m_SelectVars[i].lobLocator, (sb4)0,
                                    (ub2)SQLT_BLOB, (dvoid *)0, (ub2 *)0, (ub2 *)0, (ub4)OCI_DEFAULT);
                        if ( errno )
                        {
                            throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserSelect OCIDefineByPos");
                        }
                        break;
                    }
                case SQLT_CLOB:
                    {
                        /* Allocate lobLocator */
                        errno = OCIDescriptorAlloc( (dvoid *)this ->m_pOCIEnv, (dvoid **)&this ->m_SelectVars[i].lobLocator, (ub4)OCI_DTYPE_LOB, (size_t)0, (dvoid **)0 );
                        if ( errno )
                        {
                            throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserSelect OCIDe.orAlloc");
                        }
                        this ->m_SelectVars[i].VarType = TYPE_BLOB;
                        errno = OCIDefineByPos( this ->m_pOCIStmt, &this ->m_pOCIDefine, this ->m_pOCIError, (ub4)i + 1, (dvoid *)&this ->m_SelectVars[i].lobLocator, (sb4)0,
                        (ub2)SQLT_BLOB, (dvoid *)0, (ub2 *)0, (ub2 *)0, (ub4)OCI_DEFAULT);
                        if ( errno )
                        {
                            throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserSelect OCIDefineByPos");
                        }
                        break;
                    }
                default:
                    break;
            }
        }
        return 0;
    }
    catch ( OCIException &ex )
    {
        /* Free Stmt */
        errno = OCIHandleFree( (dvoid *)this ->m_pOCIStmt, (ub4)OCI_HTYPE_STMT );
        if ( errno )
        {
            OCIError::PrintError(errno, (char *)"OCIDB::UserExecute Ex OCIHandleFree");
        }
        //std::cout << ex.GetErrFunc() << ex.GetErrMsg() << //std::endl;
        return -1;
    }
}

int OCIDB::UserFetch()
{
    sword errno;
    try
    {
        //��ȡ���
        errno = OCIStmtFetch(
                this ->m_pOCIStmt, //�����
                this ->m_pOCIError,
                1, //�ӵ�ǰλ�ô���ʼһ����ȡ�ļ�¼�����������ݱ���������>;1,������>;1
                OCI_FETCH_NEXT, //��ȡ�ķ���OCI_FETCH_NEXT
                OCI_DEFAULT
                );
        if ( errno && errno != OCI_NO_DATA )
        {
            throw OCIException(errno, this ->m_pOCIError, (char *)"OCIDB::UserFetch OCIStmtFetch");
        }
        if ( errno == OCI_NO_DATA )
        {
            return 1;
        }
        return 0;
    }
    catch ( OCIException &ex )
    {
        //std::cout << ex.GetErrFunc() << ex.GetErrMsg() << //std::endl;
        printf("OCIStmtFetch error : %s,%s\n",ex.GetErrFunc(),ex.GetErrMsg());
        return -1;
    }
}

int OCIDB::UserSelectFree()
{
  int i;
  for ( i = 0 ; i < this ->m_iSelectVarsCount ; i++ )
  {
    switch ( this ->m_SelectVars[i].VarType )
    {
    case TYPE_STRING:
      delete [] this ->m_SelectVars[i].ValueChar;
      break;
    case TYPE_FLOAT:
      delete this ->m_SelectVars[i].ValueFloat;
      break;
    case TYPE_INT:
      delete this ->m_SelectVars[i].ValueInt;
      break;
    }
  }
  return this ->UserFree();
}

int OCIDB::UserGetInt(int index)
{
  switch ( this ->m_SelectVars[index].VarType )
  {
  case TYPE_FLOAT:
    return (int)*m_SelectVars[index].ValueFloat;
    break;
  case TYPE_INT:
    return *m_SelectVars[index].ValueInt;
    break;
  default:
    return 0;
  }
}

int OCIDB::UserGetInt(char * name)
{
  int i;
  for ( i = 0 ; i < this ->m_iSelectVarsCount ; i++ )
  {
    if ( this ->StrCmp(m_SelectVars[i].VarName, name) )
    {
      switch ( this ->m_SelectVars[i].VarType )
      {
      case TYPE_FLOAT:
        return (int)*m_SelectVars[i].ValueFloat;
        break;
      case TYPE_INT:
        return *m_SelectVars[i].ValueInt;
        break;
      default:
        return 0;
      }
    }
  }
  return 0;
}

float OCIDB::UserGetFloat(int index)
{
  switch ( this ->m_SelectVars[index].VarType )
  {
  case TYPE_FLOAT:
    return *m_SelectVars[index].ValueFloat;
    break;
  case TYPE_INT:
    return (float)*m_SelectVars[index].ValueInt;
    break;
  default:
    return 0;
  }
}

float OCIDB::UserGetFloat(char * name)
{
  int i;
  for ( i = 0 ; i < this ->m_iSelectVarsCount ; i++ )
  {
    if ( this ->StrCmp(m_SelectVars[i].VarName, name) )
    {
      switch ( this ->m_SelectVars[i].VarType )
      {
          case TYPE_FLOAT:
            return *m_SelectVars[i].ValueFloat;
            break;
          case TYPE_INT:
            return (float)*m_SelectVars[i].ValueInt;
            break;
          default:
            return 0;
      }
    }
  }
  return 0;
}

char * OCIDB::UserGetString(int index)
{
  if ( this ->m_SelectVars[index].VarType == TYPE_STRING )
  {
    return this ->m_SelectVars[index].ValueChar;
  }
  else
  {
    return 0;
  }
}

char * OCIDB::UserGetString(char * name)
{
  for (int i = 0 ; i < this ->m_iSelectVarsCount ; i++ )
  {
    if ( this ->StrCmp(m_SelectVars[i].VarName, name) )
    {
      if ( this ->m_SelectVars[i].VarType == TYPE_STRING )
      {
        return this ->m_SelectVars[i].ValueChar;
      }
    }
  }
  return 0;
}

void OCIDB::UserGetDate(char * name,int &year, int &month, int &day, int &hour, int &minute, int &second)
{
    for (int  i = 0 ; i < this ->m_iSelectVarsCount ; i++ )
    {
        if ( this ->StrCmp(m_SelectVars[i].VarName, name) )
        {
            if ( this ->m_SelectVars[i].VarType == TYPE_DATE)
            {
                year   = 0;
                month  = 0;
                day    = 0;
                hour   = 0;
                minute = 0;
                second = 0;
                unsigned char cc,yy,mm,dd,hh,mi,ss;
                cc = m_SelectVars[i].ValueChar[0];
                yy = m_SelectVars[i].ValueChar[1];
                mm = m_SelectVars[i].ValueChar[2];
                dd = m_SelectVars[i].ValueChar[3];
                hh = m_SelectVars[i].ValueChar[4]-1;
                mi = m_SelectVars[i].ValueChar[5]-1;
                ss = m_SelectVars[i].ValueChar[6]-1;
                cc = (unsigned char)abs(cc-100);
                yy = (unsigned char)abs(yy-100);
                year   = (unsigned int)cc*100 + (unsigned int)yy;
                month  = mm;
                day    = dd;
                hour   = hh;
                minute = mi;
                second = ss;
            }
        }
    }
}

char * OCIDB::UserGetBlob(int index)
{
  if ( this ->m_SelectVars[index].VarType == TYPE_BLOB )
  {
    return this ->m_SelectVars[index].ValueChar;
  }
  else
  {
    return 0;
  }
}

char * OCIDB::UserGetBlob(char * name)
{
  int i;
  for ( i = 0 ; i < this ->m_iSelectVarsCount ; i++ )
  {
    if ( this ->StrCmp(m_SelectVars[i].VarName, name) )
    {
      if ( this ->m_SelectVars[i].VarType == TYPE_BLOB )
      {
        return this ->m_SelectVars[i].ValueChar;
      }
    }
  }
  return 0;
}

vector< vector<string> > OCIDB::getResult()
{
  int i;
  vector<string> objectValue;
  /*
  StringTools stringTools;
  while ( this ->UserFetch() == 0 )
  {
    objectValue.clear();
    for ( i = 0 ; i < this ->m_iSelectVarsCount ; i++ )
    {
      switch ( this ->m_SelectVars[i].VarType )
      {
      case TYPE_STRING:
        objectValue.push_back(this ->UserGetString(i));
        break;
      case TYPE_FLOAT:
        objectValue.push_back(stringTools.ToString(this ->UserGetFloat(i)));
        break;
      case TYPE_INT:
        objectValue.push_back(stringTools.ToString(this ->UserGetInt(i)));
        break;
      case TYPE_BLOB:
        break;
      }
    }

    resultList.push_back(objectValue);
  }
*/
  return resultList;
}

void inline OCIDB::StrUpper(char * str)
{
  int i;
  int sLen = strlen(str);
  for ( i = 0 ; i < sLen ; i++ )
  {
    str[i] = toupper(str[i]);
  }
}

bool inline OCIDB::StrCmp(const char * ori, const char * des)
{
  int iLenOri, iLenDes;
  int j;

  iLenOri = strlen(ori);
  iLenDes = strlen(des);

  if ( iLenOri != iLenDes )
    return false;

  for ( j = 0 ; j < iLenOri ; j++ )
  {
    if ( toupper(ori[j]) != toupper(des[j]) )
    {
      return false;
    }
  }
  return true;
}

