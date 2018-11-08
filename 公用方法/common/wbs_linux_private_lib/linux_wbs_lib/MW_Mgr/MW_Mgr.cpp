// MW_Mgr.cpp : 定义控制台应用程序的入口点。
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