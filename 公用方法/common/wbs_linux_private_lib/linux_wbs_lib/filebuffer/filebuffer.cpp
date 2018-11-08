// filebuffer.cpp : Defines the entry point for the DLL application.
//


#include "filebuffer.h"
#include "FileBufferManage.h"

CFileBuffer::CFileBuffer()
{
	
}

CFileBuffer::~CFileBuffer()
{
	
}

/************************************************************************/
/* 产生CFileBufferManage对象											*/
/************************************************************************/
CFileBuffer* CFileBufferFactory::New()
{
	return new CFileBufferManage();
}

/************************************************************************/
/* free                                                                 */
/************************************************************************/
void CFileBufferFactory::Destroy(CFileBuffer* pBase)
{
	//delete[] pBase;
	//zhubo 2017-5-31
	delete pBase;
}
