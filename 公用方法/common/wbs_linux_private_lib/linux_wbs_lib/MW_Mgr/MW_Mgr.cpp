// MW_Mgr.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include <iostream>
#include <time.h>
#include <stdio.h>
#include "LogMgr.h"


using namespace std;
int main()
{
	CLogMgr::GetInstance().WriteLog("test.test");
	return 0;
}