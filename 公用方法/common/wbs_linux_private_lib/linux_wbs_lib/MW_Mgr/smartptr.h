#ifndef _SMARTPTR_H_
#define _SMARTPTR_H_

template <class T>
class ref_ptr
{
public:
	explicit ref_ptr()
	{ 
		ptr    = NULL;
		m_nCnt = 0;
	}

	//从 new 得到指针的构造函数
	explicit ref_ptr(T* p)
	{ 
		if (NULL != p)
		{
			ptr		= p;
			m_nCnt	= 1;
		}
	}

	//从引用得到指针的构造函数
	explicit ref_ptr(const T& t)
	{ 
		ptr  = new T;
		*ptr = t;
		m_nCnt = 1;
	}

	//从另一个指针得到数据的构造函数
	ref_ptr(const ref_ptr<T>& p) throw()
	{ 
		ptr    = p.ptr;      
		m_nCnt = p.m_nCnt;
		++m_nCnt;
	}

	//析构函数
	~ref_ptr()
	{ 
		dispose(); 
	}

	//赋值运算符
	ref_ptr<T>& operator=(const ref_ptr<T>& p) throw() 
	{
		if(this != &p)
		{
			dispose();
			ptr    = p.ptr;
			m_nCnt = p.m_nCnt;
			++m_nCnt;
		}
		return *this;
	}
	//赋值,从原生对象
	ref_ptr<T>& operator=(const T& t) throw() 
	{
		dispose();
		ptr  = new T;
		*ptr = t;
		m_nCnt = 1;
		return *this;
	}
	//复制新对象,建立新拷贝,前缀运算符
	const ref_ptr<T>& operator++() throw() 
	{
		if(m_nCnt > 1)
		{
			T* tmp = new T;
			long ntct = 1;
			*tmp = *ptr;
			dispose();
			ptr = tmp;
			m_nCnt = ntct;
		}
		return *this;
	}
	//复制新对象,建立新拷贝,后缀运算符
	const ref_ptr<T>& operator++(int) throw() 
	{
		return ++(*this);
	}
	//指针对象运算符,返回对象引用
	T& operator*() const throw() 
	{
		return *ptr;
	}
	//返回对象指针
	T* operator->() const throw() 
	{
		return ptr;
	}
	//返回对象引用,类型转换
	operator T() const throw() 
	{
        return *ptr;
	}

private:
	//回收资源
	void dispose() 
	{
		if(--m_nCnt <= 0) 
		{
			if (NULL != ptr)
			{
				delete ptr;
				ptr  = NULL;
				m_nCnt = 0;
			}
		}
	}
	//增加引用计数
	int IncRefCnt()
	{
		return ++m_nCnt;
	}

	//减少引用计数
	int DecRefCnt()
	{
		--m_nCnt;
		m_nCnt<0?m_nCnt=0:1;
		return m_nCnt;
	}

	//设置引用计数
	int SetRefCnt(int nCnt)
	{
		return m_nCnt = nCnt;
	}

	//返回引用计数
	int GetRefCnt()
	{
		return m_nCnt;
	}
private:
	T* ptr;
	long m_nCnt;
	//CRITICAL_SECTION	m_cs;
	//DerivedLock	m_cs;
};
#endif

