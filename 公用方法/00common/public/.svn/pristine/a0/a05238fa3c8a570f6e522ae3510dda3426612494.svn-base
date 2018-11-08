/* 
* Exception.h 
* 
*    Created .: 2010-6-4 
*            Author: pxh  
*/ 

#ifndef EXCEPTION_H_ 
#define EXCEPTION_H_ 

class Exception 
{ 
public: 
  Exception(int errno); 
  virtual char * GetErrMsg() = 0; 
  virtual char * GetErrFunc() = 0; 
  virtual int GetErrNo() = 0; 
protected: 
  int m_iErrNo; 
  char m_sErrBuff[512]; 
  char m_sErrFunc[128]; 
}; 

#endif /* EXCEPTION_H_ */ 
