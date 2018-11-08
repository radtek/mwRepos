
#include "stdio.h"
#include "filebuffer.h"





int main()
{
	//printf("this is test");
	CFileBuffer* pfileBuffer = CFileBufferFactory::New();
	if (false == pfileBuffer->InitFileBuffer("/work"))
		return -1;

	return 1;
}