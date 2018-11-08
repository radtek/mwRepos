/* 
* OCIException.h 
* 
*    Created .: 2010-6-4 
*            Author: pxh  
*/ 

#ifndef OCIEXCEPTION_H_ 
#define OCIEXCEPTION_H_ 

#include "Common.h" 
#include "Exception.h" 

class OCIException : public Exception 
{ 
public: 
  OCIException(sb4 errno); 
  OCIException(sb4 errno, char * errfunc); 
  OCIException(sb4 errno, dvoid * erroci); 
  OCIException(sb4 errno, dvoid * erroci, char * errfunc); 

  char * GetErrMsg(); 
  char * GetErrFunc(); 
  int GetErrNo(); 
private: 
  void CheckError(sb4 errno); 
  void CheckError(sb4 errno, dvoid * erroci); 
}; 

#endif /* OCIEXCEPTION_H_ */ 
