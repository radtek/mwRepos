/* 
* OCIError.cpp 
* 
*    Created .: 2010-6-4 
*            Author: pxh 
*/ 
#include "OCIError.h" 

int OCIError::m_iErrNo = 0; 
char OCIError::m_sErrBuff[512] = {0}; 
char OCIError::m_sErrFunc[128] = {0}; 

void OCIError::CheckError() 
{ 
  switch ( OCIError::m_iErrNo ) 
  { 
  case OCI_SUCCESS: 
    break; 
  case OCI_SUCCESS_WITH_INFO: 
    sprintf( OCIError::m_sErrBuff, "OCI_SUCCESS_WITH_INFO" ); 
    break; 
  case OCI_NEED_DATA: 
    sprintf( OCIError::m_sErrBuff, "OCI_NEED_DATA" ); 
    break; 
  case OCI_NO_DATA: 
    sprintf( OCIError::m_sErrBuff, "OCI_NO_DATA" ); 
    break; 
  case OCI_ERROR: 
    sprintf( OCIError::m_sErrBuff, "OCI_ERROR" ); 
    break; 
  case OCI_INVALID_HANDLE: 
    sprintf( OCIError::m_sErrBuff, "OCI_INVALID_HANDLE" ); 
    break; 
  case OCI_STILL_EXECUTING: 
    sprintf( OCIError::m_sErrBuff, "OCI_STILL_EXCUTING" ); 
    break; 
  case OCI_CONTINUE: 
    sprintf( OCIError::m_sErrBuff, "OCI_CONTINUE" ); 
    break; 
  default: 
    break; 
  } 
} 

void OCIError::PrintError(int errno) 
{ 
  OCIError::m_iErrNo = errno; 
  OCIError::CheckError(); 
  printf("OCIError::PrintError  %s\n",OCIError::m_sErrBuff);
} 

void OCIError::PrintError(int errno, char * errfunc ) 
{ 
  OCIError::m_iErrNo = errno; 
  strcpy(OCIError::m_sErrFunc, errfunc); 
  OCIError::CheckError(); 
  printf("OCIError::PrintError  %s %s\n",OCIError::m_sErrFunc,OCIError::m_sErrBuff);
} 


