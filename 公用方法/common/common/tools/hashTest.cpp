/////////////////////////////////////////////////////////////////////////////   
// Name:        DebugMain.cpp   
// Purpose:     测试Hash算法封装的类，完成索引表的填充和查找功能的测试。   
// Author:      陈相礼   
// Modified by:   
// Created:     07/30/09   
// RCS-ID:      $Id: treetest.h 43021 2009-07-30 16:36:51Z VZ $   
// Copyright:   (C) Copyright 2009, TSong Corporation, All Rights Reserved.   
// Licence:        
/////////////////////////////////////////////////////////////////////////////   
  
//////////////////////////////////////////////////////////////////////////   
// 测试参数设定宏   
#define TESTNUM 10000  
  
#include <iostream>   
#include <fstream>   
#include "HashAlgo.h"   
#include "MW_Time.h"
  
using   namespace  std; 
//using namespace MWTIME;
 
  
//////////////////////////////////////////////////////////////////////////   
// 测试主函数开始   
int  main(  int  argc,  char  **argv )  
{  
    CHashAlgo hash_test( TESTNUM );  
  
    cout << "取得初始化散列索引表长为："  << hash_test.GetTableLength() << endl;  
  
    bool  is_success = hash_test.SetHashTable(  "test"  );  
    if  ( is_success )  
    {  
        cout << "散列结果一：成功！"  << endl;  
    }  
    else   
    {  
        cout << "散列结果一：失败！"  << endl;  
    }  
      
    is_success = hash_test.SetHashTable( "测试"  );  
    if  ( is_success )  
    {  
        cout << "散列结果二：成功！"  << endl;  
    }  
    else   
    {  
        cout << "散列结果二：失败！"  << endl;  
    }  
  
	//时间
	MWTIME::CTime timeTest;
	double t_use = 0;
	timeTest.initTime();
    long  pos = hash_test.GetHashTablePos(  "test"  ); 
	t_use = timeTest.getUseTime();
    cout << "查找测试字符串：test 的散列位置："  << pos << endl; 
	cout << "用时:" << t_use << endl;
	timeTest.initTime();
    pos = hash_test.GetHashTablePos( "测试"  ); 
	t_use = timeTest.getUseTime();
    cout << "查找测试字符串：“测试” 的散列位置："  << pos << endl;  
	cout << "用时:" << t_use << endl;

	int i = 0;
    //////////////////////////////////////////////////////////////////////////   
    // 散列测试   
    for  ( i = 0; i < TESTNUM; i++ )  
    {  
        char  buff[32];  
        sprintf(buff, "abcdefg%d." , i);  
        is_success = hash_test.SetHashTable(buff);  
        //is_success ? cout << buff << "散列结果：成功！位置："  << hash_test.testid << endl : cout << buff <<  "散列结果：失败！"  << endl;        
    }  
    //system( "pause"  );  

    //////////////////////////////////////////////////////////////////////////   
    // 查找测试   
//     for  (i = 0; i < TESTNUM; i++ )  
//     {  
//         char  buff[32];  
//         sprintf(buff, "abcdefg%d." , i);
// 		CTime timeTest;
// 		timeTest.initTime();
//         pos = hash_test.GetHashTablePos( buff ); 
		//t_use = timeTest.getUseTime();
// 		if(-1 != pos)
// 		{
// 			cout << "查找测试字符串：" << buff << " 的散列位置："  << pos << "用时:"<< timeTest.getUseTime() << endl;
// 		}
// 		else
// 		{
// 			cout << buff <<  "存在冲突！"  << "用时:"<< timeTest.getUseTime() << endl;
// 		}
        //pos != -1 ?  cout << "查找测试字符串：" << buff << " 的散列位置："  << pos << endl : cout << buff <<  "存在冲突！"  << endl;     
//    }  
	
    //system( "pause"  );

	string strTest("abcdefg2200.");
	//CTime timeTest;
	timeTest.initTime();
	pos = hash_test.GetHashTablePos( (char*)strTest.c_str() ); 
	//timeTest.sleepTime(1);
	t_use = timeTest.getUseTime();
	if(-1 != pos)
	{
		cout << "查找测试字符串：" << strTest.c_str() << " 的散列位置："  << pos << "用时:"<< t_use << endl;
	}
    return  0;  
}