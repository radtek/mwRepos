/////////////////////////////////////////////////////////////////////////////   
// Name:        DebugMain.cpp   
// Purpose:     ����Hash�㷨��װ���࣬�������������Ͳ��ҹ��ܵĲ��ԡ�   
// Author:      ������   
// Modified by:   
// Created:     07/30/09   
// RCS-ID:      $Id: treetest.h 43021 2009-07-30 16:36:51Z VZ $   
// Copyright:   (C) Copyright 2009, TSong Corporation, All Rights Reserved.   
// Licence:        
/////////////////////////////////////////////////////////////////////////////   
  
//////////////////////////////////////////////////////////////////////////   
// ���Բ����趨��   
#define TESTNUM 10000  
  
#include <iostream>   
#include <fstream>   
#include "HashAlgo.h"   
#include "MW_Time.h"
  
using   namespace  std; 
//using namespace MWTIME;
 
  
//////////////////////////////////////////////////////////////////////////   
// ������������ʼ   
int  main(  int  argc,  char  **argv )  
{  
    CHashAlgo hash_test( TESTNUM );  
  
    cout << "ȡ�ó�ʼ��ɢ��������Ϊ��"  << hash_test.GetTableLength() << endl;  
  
    bool  is_success = hash_test.SetHashTable(  "test"  );  
    if  ( is_success )  
    {  
        cout << "ɢ�н��һ���ɹ���"  << endl;  
    }  
    else   
    {  
        cout << "ɢ�н��һ��ʧ�ܣ�"  << endl;  
    }  
      
    is_success = hash_test.SetHashTable( "����"  );  
    if  ( is_success )  
    {  
        cout << "ɢ�н�������ɹ���"  << endl;  
    }  
    else   
    {  
        cout << "ɢ�н������ʧ�ܣ�"  << endl;  
    }  
  
	//ʱ��
	MWTIME::CTime timeTest;
	double t_use = 0;
	timeTest.initTime();
    long  pos = hash_test.GetHashTablePos(  "test"  ); 
	t_use = timeTest.getUseTime();
    cout << "���Ҳ����ַ�����test ��ɢ��λ�ã�"  << pos << endl; 
	cout << "��ʱ:" << t_use << endl;
	timeTest.initTime();
    pos = hash_test.GetHashTablePos( "����"  ); 
	t_use = timeTest.getUseTime();
    cout << "���Ҳ����ַ����������ԡ� ��ɢ��λ�ã�"  << pos << endl;  
	cout << "��ʱ:" << t_use << endl;

	int i = 0;
    //////////////////////////////////////////////////////////////////////////   
    // ɢ�в���   
    for  ( i = 0; i < TESTNUM; i++ )  
    {  
        char  buff[32];  
        sprintf(buff, "abcdefg%d." , i);  
        is_success = hash_test.SetHashTable(buff);  
        //is_success ? cout << buff << "ɢ�н�����ɹ���λ�ã�"  << hash_test.testid << endl : cout << buff <<  "ɢ�н����ʧ�ܣ�"  << endl;        
    }  
    //system( "pause"  );  

    //////////////////////////////////////////////////////////////////////////   
    // ���Ҳ���   
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
// 			cout << "���Ҳ����ַ�����" << buff << " ��ɢ��λ�ã�"  << pos << "��ʱ:"<< timeTest.getUseTime() << endl;
// 		}
// 		else
// 		{
// 			cout << buff <<  "���ڳ�ͻ��"  << "��ʱ:"<< timeTest.getUseTime() << endl;
// 		}
        //pos != -1 ?  cout << "���Ҳ����ַ�����" << buff << " ��ɢ��λ�ã�"  << pos << endl : cout << buff <<  "���ڳ�ͻ��"  << endl;     
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
		cout << "���Ҳ����ַ�����" << strTest.c_str() << " ��ɢ��λ�ã�"  << pos << "��ʱ:"<< t_use << endl;
	}
    return  0;  
}