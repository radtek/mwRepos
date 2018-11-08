/////////////////////////////////////////////////////////////////////////////   
// Name:        HashAlgo.h   
// Purpose:     使用魔兽Hash算法，实现索引表的填充和查找功能。   
// Author:      陈相礼   
// Modified by: yby  04/19/14
// Created:     07/30/09   
// RCS-ID:      $Id: treetest.h 43021 2009-07-30 16:36:51Z VZ $   
// Copyright:   (C) Copyright 2009, TSong Corporation, All Rights Reserved.   
// Licence:        
/////////////////////////////////////////////////////////////////////////////   
#define MAXFILENAME 25     // 最大文件名长度   
#define MAXTABLELEN 1024    // 默认哈希索引表大小   

//////////////////////////////////////////////////////////////////////////   
// 测试宏定义，正式使用时关闭   
#define DEBUGTEST 0   
//#define NULL 0
//////////////////////////////////////////////////////////////////////////   
// 哈希索引表定义   
typedef   struct   
{  
    long  nHashA;  
    long  nHashB;  
    bool  bExists;
	
	//删除该成员，因为不需要处理冲突
    //char  test_filename[MAXFILENAME];  
} MPQHASHTABLE;  

//////////////////////////////////////////////////////////////////////////   
// 对哈希索引表的算法进行封装   
class  CHashAlgo  
{  
public :  
	
#if DEBUGTEST   
    long   testid;    // 测试之用   
#endif   
	
	// 创建指定大小的哈希索引表，不带参数的构造函数创建默认大小的哈希索引表   
    CHashAlgo( const   long  nTableLength = MAXTABLELEN ) 
    {  
        prepareCryptTable();  
        m_tablelength = nTableLength;  
		
        m_HashIndexTable = new  MPQHASHTABLE[nTableLength];  
        for  (  int  i = 0; i < nTableLength; i++ )  
        {  
            m_HashIndexTable[i].nHashA = -1;  
            m_HashIndexTable[i].nHashB = -1;  
            m_HashIndexTable[i].bExists = false ; 
            //m_HashIndexTable[i].test_filename[0] = '\0' ;
        }          
    }  
	
	// 对哈希索引表预处理   
    void  prepareCryptTable();
	// 求取哈希值       
    unsigned long  HashString( char  *lpszFileName, unsigned  long  dwHashType);
	// 得到在定长表中的位置   
    long  GetHashTablePos(  char  *lpszString );
	// 将字符串散列到哈希表中   
    bool  SetHashTable(  char  *lpszString );                                   
	
    unsigned long  GetTableLength( void );
    void  SetTableLength(  const  unsigned  long  nLength );

	//重置hash表的状态
	void  ResetTable();
	
    ~CHashAlgo()  
    {  
        if  ( NULL != m_HashIndexTable )  
        {  
            delete  []m_HashIndexTable;  
            m_HashIndexTable = NULL;  
            m_tablelength = 0;  
        }  
    }  
protected :  
	
private :  
    unsigned long  cryptTable[0x500];  
    unsigned long  m_tablelength;     // 哈希索引表长度   
    MPQHASHTABLE *m_HashIndexTable;  
};
