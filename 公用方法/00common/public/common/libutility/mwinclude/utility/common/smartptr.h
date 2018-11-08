// smartptr.h: interface for the CSmartPtr class.
// Author: JiangHongbo
// Time: 2015-12-25
// Ver:  1.3

#ifndef _SMART_PTR_H_
#define _SMART_PTR_H_

#include "SafeLock.h"
#include "DerivedLock.h"
#include "stdlib.h"
#include <list>

//////////////////////////////////////////////////////////////////////////
//内存池---用malloc,申请的内存不能调用构造函数,适用于不需要初始化时调用构造函数的场景
//该内存池已支持多线程调用
class CSmartMemPool
{
public:
	//构造
	CSmartMemPool(unsigned int nUintNum=1024/*内存块个数*/, unsigned int nUintSize=1*1024/*每一个内存块的大小*/)
	{
		//保存初始化参数
		m_pMemBlock				= NULL;
		m_pAllocatedMemBlock	= NULL;
		m_pFreeMemBlock			= NULL;
		m_nUintSize				= nUintSize;
		m_nBlockSize			= nUintNum*(nUintSize + sizeof(struct _BLOCK));
		//申请内存
		m_pMemBlock = operator new(m_nBlockSize);            
		if (NULL != m_pMemBlock)
		{
			//将所有内存结点链接起来
			for (unsigned int i = 0; i < nUintNum; ++i)    
			{
				struct _BLOCK *pCurBlock = (struct _BLOCK *)((char *)m_pMemBlock + i*(m_nUintSize + sizeof(struct _BLOCK)));
				
				pCurBlock->pPrev = NULL;
				pCurBlock->pNext = m_pFreeMemBlock;        
				
				if (NULL != m_pFreeMemBlock)
				{
					m_pFreeMemBlock->pPrev = pCurBlock;
				}
				m_pFreeMemBlock = pCurBlock;
			}
		}
		else
		{
			throw;
		}
	}

	//析构
	virtual	~CSmartMemPool()
	{
		if (NULL != m_pMemBlock)
		{
			operator delete(m_pMemBlock);
			m_pMemBlock = NULL;
		}
	}

public:
	//默认申请初始化时的内存大小并使用池子
	void* Alloc()
	{
		return Alloc(m_nBlockSize, true);
	}

	//可以指定申请内存的大小和是否使用池子
	void* Alloc(unsigned int nSize/*需要分配内存单元大小,建议与初始化时的大小相同,若大于则不会使用池子*/, bool bUsePool=true)
	{
		SafeLock safelock(&m_cs);

		if (!bUsePool || nSize > m_nUintSize || NULL == m_pMemBlock || NULL == m_pFreeMemBlock)
		{
			return malloc(nSize);
		}
		
		//从空闲链表中取出一个内存单元，放入已分配链表中，并返回该单元的内存地址
		struct _BLOCK *pCurBlock = m_pFreeMemBlock;
		
		m_pFreeMemBlock = pCurBlock->pNext;            
		if (NULL != m_pFreeMemBlock)
		{
			m_pFreeMemBlock->pPrev = NULL;
		}
		
		pCurBlock->pNext = m_pAllocatedMemBlock;
		
		if (NULL != m_pAllocatedMemBlock)
		{
			m_pAllocatedMemBlock->pPrev = pCurBlock; 
		}
		m_pAllocatedMemBlock = pCurBlock;
		
		return (void *)((char *)pCurBlock + sizeof(struct _BLOCK));
	}

	//释放内存
	void  DeAlloc(void* p)
	{
		SafeLock safelock(&m_cs);
		if (NULL != p && p > m_pMemBlock && p < (void *)((char *)m_pMemBlock + m_nBlockSize))
		{
			struct _BLOCK *pCurBlock = (struct _BLOCK *)((char *)p - sizeof(struct _BLOCK));
			
			m_pAllocatedMemBlock = pCurBlock->pNext;  //从已分配链表取出回收的内存单元，放入空闲链表中
			if (NULL != m_pAllocatedMemBlock)
			{
				m_pAllocatedMemBlock->pPrev = NULL;
			}
			
			pCurBlock->pNext = m_pFreeMemBlock;
			if (NULL != m_pFreeMemBlock)
			{
				m_pFreeMemBlock->pPrev = pCurBlock;
			}
			
			m_pFreeMemBlock = pCurBlock;
		}
		else
		{
			if (NULL != p)
			{
				operator delete(p);
				p = NULL;
			}
		}
	}

private:
	//链表结点--将一块一块的内存串起来
    struct _BLOCK                            
    {
        struct _BLOCK *pPrev, *pNext;
    };

    void*				m_pMemBlock;            //内存池的首地址
    struct _BLOCK*		m_pAllocatedMemBlock;   //指向已分配出去的内存结点
    struct _BLOCK*		m_pFreeMemBlock;        //指向未分配出去的内存结点
    unsigned int		m_nUintSize;            //每一个内存块的大小
    unsigned int		m_nBlockSize;           //所有内存块的大小
	DerivedLock			m_cs;					//多线程访问锁                         
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//内存池---用new,申请T类型的内存,可以调用构造函数,适用于各种结构体和类
//该内存池已支持多线程调用
//池子内部采用了STL的链表,后期有时间可换成普通链表
template <class T>
class CSmartMemPoolEx
{
public:
	//默认构造
	CSmartMemPoolEx()
	{
		m_nDefPoolSize = 100;
		m_nCurPoolSize = 0;
	}

	//构造
	CSmartMemPoolEx(unsigned int nPoolSize/*设定的池子大小,运行时若实际大小超出该值,空闲时会delete掉*/)
	{
		//不做预分配,运行时申请后不超出设定值就不释放
		m_nDefPoolSize = nPoolSize;
		m_nCurPoolSize = 0;
	}
	
	//析构
	virtual	~CSmartMemPoolEx()
	{
		//析构时系统也要退出了,清理不清理都无所谓了
	}
	
public:
	//申请内存
	T* Alloc()
	{
		T* p = NULL;

		SafeLock safelock(&m_cs);

		if (!m_MemPool.empty())
		{
			p = m_MemPool.front();
			m_MemPool.pop_front();
		}
		else
		{
			p = new T;
			if (NULL != p)
			{
				++m_nCurPoolSize;
			}
		}

		return p;
	}

	//释放内存
	void  DeAlloc(T* p)
	{
		if (NULL != p)
		{
			SafeLock safelock(&m_cs);

			//当空闲数量超过设定值并且空闲池中有数据,直接释放掉,否则回收
			if (m_MemPool.size() >= m_nDefPoolSize)
			{
				delete p;
				p = NULL;
				--m_nCurPoolSize;
			}
			else
			{
				memset(p, 0, sizeof(T));

				m_MemPool.push_back(p);
			}
		}
	}

	//重置池子的设定值
	void ReSetPoolSize(unsigned int nPoolSize)
	{
		int nSize = 0;
		int nDefSize = 0;
		{
			SafeLock safelock(&m_cs);
			nDefSize = m_nDefPoolSize = nPoolSize;
			nSize = m_MemPool.size();
		}

		//当前值超过设定值,直接释放掉,否则回收
		while (--nSize > nDefSize)
		{
			SafeLock safelock(&m_cs);
		
			if (!m_MemPool.empty())
			{
				T* p = m_MemPool.front();
				if (NULL != p)
				{
					delete p;
				}
				m_MemPool.pop_front();
				--m_nCurPoolSize;
			}
			else
			{
				break;
			}
		}
	}

	//获取当前池子的大小
	unsigned int GetCurPoolSize() const
	{
		return m_nCurPoolSize;
	}

	//获取池子允许的大小
	unsigned int GetDefPoolSize() const
	{
		return m_nDefPoolSize;
	}
private:
	std::list<T*>		m_MemPool;				//内存池
	int					m_nCurPoolSize;			//当前池子的大小
	int					m_nDefPoolSize;			//池子设定的大小
	DerivedLock			m_cs;					//多线程访问锁                         
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//智能指针
template <class T>
class RefCnt
{
public:
	RefCnt();
	RefCnt(CSmartMemPoolEx<T>* pMemPool);
	virtual ~RefCnt();
public:
	//增加引用计数
	int IncCnt();
	//减少引用计数--计数为0时,自动释放
	int DecCnt();
public:
	//实体指针
	T* m_p;
private:
	DerivedLock m_cs;
	//引用计数
	int m_nCnt;
	//内存池指针
	CSmartMemPoolEx<T>* m_pMemPool;
};

//调用默认构造函数的话不使用内存池
template<class T>
RefCnt<T>::RefCnt()
{
	SafeLock safelock(&m_cs);
	m_pMemPool = NULL;//调用默认构造函数的话不使用内存池
	m_nCnt = 1;
	m_p = new T;
	if (NULL == m_p)
	{
		throw;
	}
}

//该构造函数可以使用内存池
template<class T>
RefCnt<T>::RefCnt(CSmartMemPoolEx<T>* pMemPool)
{
	SafeLock safelock(&m_cs);
	m_pMemPool = pMemPool;
	//如果池子的预设值为0,按无池子处理
	if (NULL != m_pMemPool 
		&& m_pMemPool->GetDefPoolSize() > 0)
	{
		m_p = m_pMemPool->Alloc();
		if (NULL == m_p)
		{
			throw;
		}
		m_nCnt = 1;
	}
	else
	{
		m_pMemPool = NULL;
		m_p = new T;
		if (NULL == m_p)
		{
			throw;
		}
		m_nCnt = 1;
	}
}

template<class T>
RefCnt<T>::~RefCnt()
{
	SafeLock safelock(&m_cs);
	m_nCnt = 0;
	if (m_p != NULL)
	{
		//用了池子就回收
		if (NULL != m_pMemPool)
		{
			m_pMemPool->DeAlloc(m_p);
			m_p = NULL;
		}
		//没用就delete
		else
		{
			delete m_p;
			m_p = NULL;
		}
	}
}

template<class T>
int RefCnt<T>::IncCnt()
{
	SafeLock safelock(&m_cs);
	return ++m_nCnt;
}

template<class T>
int RefCnt<T>::DecCnt()
{
	SafeLock safelock(&m_cs);
	--m_nCnt;
	if (0 == m_nCnt && NULL != m_p)
	{
		//用了池子就回收
		if (NULL != m_pMemPool)
		{
			m_pMemPool->DeAlloc(m_p);
			m_p = NULL;
		}
		//没用就delete
		else
		{
			delete m_p;
			m_p = NULL;
		}
	}
	return m_nCnt;
}

template <class T>
class CSmartPtr
{
public:
	//默认构造
	CSmartPtr()
	{
		m_p = NULL;
	}

	//自定义构造
	//内部分配T----第一次分配内存时都用这个
	CSmartPtr(CSmartMemPoolEx<T>* pMemPool)
	{
		if (NULL != pMemPool)
		{
			m_p = new RefCnt<T>(pMemPool);
		}
		else
		{
			m_p = new RefCnt<T>;
		}
		if (NULL == m_p)
		{
			throw;
		}
	}

	//拷贝构造
	//如果参数中的p是空的没有初始化过的,是允许的,空赋给空
	CSmartPtr(const CSmartPtr& p)
	{
		//如果入参不为空则赋值
		if (NULL != p.m_p)
		{
			m_p = p.m_p;
			m_p->IncCnt();
		}
		else
		{	
			//赋空
			m_p = p.m_p;
		}
	}

	//析构
	virtual ~CSmartPtr()
	{
		//减少计数,如果计数减为0,则释放refcnt指针
		//(注意这里写的是计数为0,refcnt中可以减为负数,但这里写的是0,意味着这里只能有一次释放)
		if (NULL != m_p && 0 == m_p->DecCnt())
		{
			if (NULL != m_p)
			{
				delete m_p;
				m_p = NULL;
			}
		}
	}

public:
	//赋值运算符
	CSmartPtr& operator = (const CSmartPtr& p)
	{
		//如果两个相同的对象进行赋值,什么也不做
		if (&p == this)
		{
			return *this;
		}
		//如果已构造过(拷贝构造或自定义构造),不允许再次被赋值
		else if (m_p != NULL)
		{
			throw;
		}
		else
		{
			//如果入参不为空则赋值
			if (NULL != p.m_p)
			{
				m_p = p.m_p;
				m_p->IncCnt();
			}
			else
			{
				//如果参数是空的,不做处理	
			}
			
			return *this;
		}
	}

	//重载->用来访问T*
	//注意这里取到的结果不一定就一定不为NULL,用指针之前请自行做安全检查
	T*	operator -> () const
	{
		T* pTmp = NULL;
		if (NULL != m_p)
		{
			pTmp = m_p->m_p;
		}
		return pTmp;
	}

	//没有构造时不要用它
	//注意这里取到的结果不一定就一定不为NULL,用指针之前请自行做安全检查
	T* GetPointer() const
	{
		T* pTmp = NULL;
		if (NULL != m_p)
		{
			pTmp = m_p->m_p;
		}
		return pTmp;
	}

	//重载*号
	T&	operator * ()
	{
		ASSERT(!IsNull());
		return *m_p->m_p;
	}

	/*
	//重载&号
	T*	operator & ()
	{
		T* pTmp = NULL;
		if (NULL != m_p)
		{
			pTmp = m_p->m_p;
		}
		return pTmp;
	}
	*/

	//判断指针是否为空
	bool IsNull()
	{
		return (NULL == m_p || NULL == m_p->m_p);
	}
private:
	//引用计数器
	RefCnt<T>* m_p;
};

#endif


//用法:
/*
struct AAA 
{
	int nTest;
};
typedef CSmartPtr<AAA> SMART_PTR;
  
void swap(const std::deque<SMART_PTR>& dIn,std::deque<SMART_PTR>& vOut)
{
	vOut.insert(vOut.begin(),dIn.begin(),dIn.end());
}

//池子必须是全局或类成功变量
CSmartMemPoolEx<AAA> mempool(5);

void main()
{
//	{
		//将池子的指针初始化给智能指针,这样智能指针内部会自动使用池子分配内存
		//若不使用池子就填NULL,内部会自动使用new delete
		//SMART_PTR s0(&mempool);
		SMART_PTR s1(NULL);
		//int kkk = mempool.GetCurPoolSize();
		// 	}
		// 	int kkk = mempool.GetCurPoolSize();
		// 	{
		// 		//将池子的指针初始化给智能指针,这样智能指针内部会自动使用池子分配内存
		// 		SMART_PTR s0(&mempool);
		// 		SMART_PTR s1(&mempool);
		// 		SMART_PTR s2(&mempool);
		// 		int kkk = mempool.GetCurPoolSize();
		// 	}
		{
		SMART_PTR s2 = s1;
		std::deque<SMART_PTR> test;
		s2->nTest = 99;
		printf("test=%d\r\n",s2->nTest);
		test.push_back(s2);
		std::deque<SMART_PTR> dOut;
		swap(test,dOut);
		std::deque<SMART_PTR>::iterator it = dOut.begin();
		//SMART_PTR& s3 = (*it);
		(*it)->nTest = 0;
		
		  AAA * pB = it->GetPointer();
		  if (NULL != pB)
		  {
		  printf("%d\r\n", pB->nTest);
		  }
		  //test.erase(it);
		  
			if (!it->IsNull())
			{
			printf("%d\r\n", it->GetPointer()->nTest);
			}
			
			  AAA * pA = s2.GetPointer();
			  if (NULL != pA)
			  {
			  pA->nTest = 0;
			  printf("test=%d\r\n",s2->nTest);
			  }
			  
				if (!s2.IsNull())
				{
				s2->nTest = 0;
				printf("test=%d\r\n",s2->nTest);
				}
				
				  printf("test\r\n");
				  }
}
*/