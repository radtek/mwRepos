/* 
* OCIError.h 
* 
*    Created .: 2010-6-4 
*            Author: pxh  
*/ 

#ifndef OCIERROR_H_ 
#define OCIERROR_H_ 

#include "Common.h" 

class OCIError 
{ 
public: 
  static void PrintError(int errno); 
  static void PrintError(int errno, char * errfunc); 
private: 
  static void CheckError(); 
  static int m_iErrNo; 
  static char m_sErrBuff[512]; 
  static char m_sErrFunc[128]; 
}; 

#endif /* OCIERROR_H_ */ 

