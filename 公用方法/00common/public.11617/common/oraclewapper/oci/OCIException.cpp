/* 
* OCIException.cpp 
* 
*    Created .: 2010-6-4 
*            Author: pxh  
*/ 
#include "OCIException.h" 

void OCIException::CheckError(sb4 errno) 
{ 
  text errBuff[512]; 
  switch ( errno ) 
  { 
  case OCI_SUCCESS: 
    break; 
  case OCI_SUCCESS_WITH_INFO: 
    sprintf( this ->m_sErrBuff, "OCI_SUCCESS_WITH_INFO" ); 
    break; 
  case OCI_NEED_DATA: 
    sprintf( this ->m_sErrBuff, "OCI_NEED_DATA" ); 
    break; 
  case OCI_NO_DATA: 
    sprintf( this ->m_sErrBuff, "OCI_NO_DATA" ); 
    break; 
  case OCI_ERROR: 
    sprintf( this ->m_sErrBuff, "OCI_ERROR" ); 
    break; 
  case OCI_INVALID_HANDLE: 
    sprintf( this ->m_sErrBuff, "OCI_INVALID_HANDLE" ); 
    break; 
  case OCI_STILL_EXECUTING: 
    sprintf( this ->m_sErrBuff, "OCI_STILL_EXCUTING" ); 
    break; 
  case OCI_CONTINUE: 
    sprintf( this ->m_sErrBuff, "OCI_CONTINUE" ); 
    break; 
  default: 
    break; 
  } 
} 

void OCIException::CheckError(sb4 errno, dvoid * erroci) 
{ 
  text errBuff[512]; 
  switch ( errno ) 
  { 
  case OCI_SUCCESS: 
    break; 
  case OCI_SUCCESS_WITH_INFO: 
    sprintf( this ->m_sErrBuff, "OCI_SUCCESS_WITH_INFO" ); 
    break; 
  case OCI_NEED_DATA: 
    sprintf( this ->m_sErrBuff, "OCI_NEED_DATA" ); 
    break; 
  case OCI_NO_DATA: 
    sprintf( this ->m_sErrBuff, "OCI_NO_DATA" ); 
    break; 
  case OCI_ERROR: 
    OCIErrorGet( (dvoid*)erroci, (ub4)1, (text*)NULL, &this ->m_iErrNo, errBuff, (ub4)sizeof(errBuff), OCI_HTYPE_ERROR); 
    sprintf( this ->m_sErrBuff, "%.*s", strlen((char*) errBuff) - 1, errBuff); 
    break; 
  case OCI_INVALID_HANDLE: 
    sprintf( this ->m_sErrBuff, "OCI_INVALID_HANDLE" ); 
    break; 
  case OCI_STILL_EXECUTING: 
    sprintf( this ->m_sErrBuff, "OCI_STILL_EXCUTING" ); 
    break; 
  case OCI_CONTINUE: 
    sprintf( this ->m_sErrBuff, "OCI_CONTINUE" ); 
    break; 
  default: 
    break; 
  } 
} 

OCIException::OCIException(sb4 errno) : Exception( (int)errno ) 
{ 
  this ->CheckError(errno); 
} 

OCIException::OCIException(sb4 errno, char * errfunc) : Exception( (int)errno ) 
{ 
  strcpy( this ->m_sErrBuff, errfunc); 
  this ->CheckError(errno); 
} 

OCIException::OCIException(sb4 errno, dvoid * erroci) : Exception( (int)errno ) 
{ 
  this ->CheckError(errno, erroci); 
} 

OCIException::OCIException(sb4 errno, dvoid * erroci, char * errfunc) : Exception( (int)errno ) 
{ 
  strcpy( this ->m_sErrBuff, errfunc); 
  this ->CheckError(errno, erroci); 
} 

char * OCIException::GetErrMsg() 
{ 
  return this ->m_sErrBuff; 
} 

char * OCIException::GetErrFunc() 
{ 
  return this ->m_sErrFunc; 
} 

int OCIException::GetErrNo() 
{ 
  return this ->m_iErrNo; 
} 
 
