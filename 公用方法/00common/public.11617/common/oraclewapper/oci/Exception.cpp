/* 
* Exception.cpp 
* 
*    Created .: 2010-6-4 
*            Author: pxh 
*/ 
#include "Exception.h" 

Exception::Exception(int errno) 
{ 
  this ->m_iErrNo = errno; 
  this ->m_sErrBuff[0] = 0; 
  this ->m_sErrFunc[0] = 0; 
} 
