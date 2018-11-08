/* 
* OCIDB.h 
* 
*    Created .: 2010-6-4 
*            Author: pxh 
*/ 

#ifndef OCIDB_H_ 
#define OCIDB_H_ 

#include "Common.h" 

#define MAX_VAR_LEN 10 
#define MAX_BUFFER_SIZE 32767 

typedef struct { 
  ub2    VarType; 
  char *  VarName; 
  int    VarLen; 
  union { 
    char * ValueChar; 
    float * ValueFloat; 
    int * ValueInt; 
    OCILobLocator *lobLocator; 
  }; 
} TBindVar; 

typedef struct{ 
  char VarName[MAX_VAR_LEN]; 
  unsigned char VarType; 
  unsigned int VarSize; 
  unsigned char VarPrecision; 
  unsigned char VarScale; 
  unsigned char VarIsNull; 
  union { 
    char * ValueChar; 
    float * ValueFloat; 
    int * ValueInt; 
    OCILobLocator *lobLocator; 
  }; 
} TSelectVar; 

#define MAX_BINDVAR_COUNT 10 
#define MAX_SELECTVAR_COUNT 10 

#define TYPE_INT 0 
#define TYPE_FLOAT 1 
#define TYPE_STRING 2 
#define TYPE_BLOB 3 
#define TYPE_DATE 4 

class OCIDB 
{ 
public: 
  dvoid * indp; 
  OCIEnv * m_pOCIEnv; 
  OCIError * m_pOCIError; 

  OCISvcCtx * m_pOCISvcCtx; 
  OCIServer * m_pOCIServer; 
  OCISession * m_pOCISession; 

  OCIStmt * m_pOCIStmt; 
  OCIBind * m_pOCIBind; 
  OCIDefine * m_pOCIDefine; 
  OCIParam * m_pOCIParam; 

  TBindVar m_BindVars[MAX_BINDVAR_COUNT]; 
  TSelectVar m_SelectVars[MAX_SELECTVAR_COUNT]; 

  int m_iBindVarsCount; 
  int m_iSelectVarsCount; 

  char m_sUser[10]; 
  char m_sPwd[10]; 
  char m_sDBName[10]; 

  int UserFetch(); 
  int UserGetInt(int index); 
  int UserGetInt(char * name); 
  char * UserGetString(int index); 
  char * UserGetString(char * name); 
  void UserGetDate(char * name,int &year, int &month, int &day, int &hour, int &minute, int &second); 
  char * UserGetBlob(int index); 
  char * UserGetBlob(char * name); 
  float UserGetFloat(int index); 
  float UserGetFloat(char * name); 
  void inline StrUpper(char *str); 
  bool inline StrCmp(const char *ori, const char *des); 
  vector< vector<string> > resultList; 
protected: 
  char * getBlobValue(); 
public: 
  OCIDB(); 
  ~OCIDB(); 

  void SetDataParam(char *user,char *pwd,char *db);

  /* Single User, Single Connection */ 
  int Single_Conn(); 
  void Single_Disc(); 

  /* Multiple Sessions or Connections */ 
  int Multiple_Conn(); 
  void Multiple_Disc(); 

  /* Execute SQL with none query */ 
  int ExcuteSQL(char * sql); 

  /* Execute SQL with bind vars */ 
  void BindInitVars(); 
  void BindClearVars(); 
  void BindAddVar(char * name, char * value); 
  void BindAddVar(char * name, int * value); 
  int BindSQL(char * sql); 

  /* Prepare SQL*/ 
  int UserPrepare(char * sql); 
  int UserFree(); 
  int UserBind(char * name, char * value); 
  int UserBind(char * name, int value); 
  int UserExecute(); 
  int UserCommit(); 
  int UserRollback(); 

  int UserSelect(char * sql); 
  int UserSelectFree(); 

  vector< vector<string> > getResult(); 
}; 

#endif /* OCIDB_H_ */ 


