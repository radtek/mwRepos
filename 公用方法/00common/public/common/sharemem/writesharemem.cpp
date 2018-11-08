#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ShareMem.h"
#include "Semaphore.h"

//共享内存结构体
struct test_sharemem_struct
{
	char szBuf[1024];
	int n1;
	int n2;
};

typedef CShareMem<test_sharemem_struct,1500000,1024*1024*1000*2> SHAREMEM;
SHAREMEM shm;

int main()
{
	printf("Writesharemem\n");	
	bool bRet = shm.InitShareMem(O_W_ONLY);
	if (bRet)
	{
		int nCnt = 0;
		int nLastCnt = 0;
		time_t tLast = time(NULL);
		while (1)
		{
			test_sharemem_struct pt;
			pt.n1 = pt.n2 = 100;
			bRet = shm.PushBack(&pt);
			if (!bRet)
			{
				printf("Write Full.Total Write:%d\n", nCnt);
				usleep(1000*1000*5);
			}
			else
			{
				++nCnt>0?1:nCnt=0;
			}
			int nDiff = time(NULL) - tLast;
			if (nDiff >= 2)
			{
				printf("Total Write:%d,Spd:%d\n", nCnt, (nCnt-nLastCnt)/nDiff);
				nLastCnt = nCnt;
				tLast = time(NULL);
			}
		}
	}
	return 0;
}
