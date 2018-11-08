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

	//�� new �õ�ָ��Ĺ��캯��
	explicit ref_ptr(T* p)
	{ 
		if (NULL != p)
		{
			ptr		= p;
			m_nCnt	= 1;
		}
	}

	//�����õõ�ָ��Ĺ��캯��
	explicit ref_ptr(const T& t)
	{ 
		ptr  = new T;
		*ptr = t;
		m_nCnt = 1;
	}

	//����һ��ָ��õ����ݵĹ��캯��
	ref_ptr(const ref_ptr<T>& p) throw()
	{ 
		ptr    = p.ptr;      
		m_nCnt = p.m_nCnt;
		++m_nCnt;
	}

	//��������
	~ref_ptr()
	{ 
		dispose(); 
	}

	//��ֵ�����
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
	//��ֵ,��ԭ������
	ref_ptr<T>& operator=(const T& t) throw() 
	{
		dispose();
		ptr  = new T;
		*ptr = t;
		m_nCnt = 1;
		return *this;
	}
	//�����¶���,�����¿���,ǰ׺�����
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
	//�����¶���,�����¿���,��׺�����
	const ref_ptr<T>& operator++(int) throw() 
	{
		return ++(*this);
	}
	//ָ����������,���ض�������
	T& operator*() const throw() 
	{
		return *ptr;
	}
	//���ض���ָ��
	T* operator->() const throw() 
	{
		return ptr;
	}
	//���ض�������,����ת��
	operator T() const throw() 
	{
        return *ptr;
	}

private:
	//������Դ
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
	//�������ü���
	int IncRefCnt()
	{
		return ++m_nCnt;
	}

	//�������ü���
	int DecRefCnt()
	{
		--m_nCnt;
		m_nCnt<0?m_nCnt=0:1;
		return m_nCnt;
	}

	//�������ü���
	int SetRefCnt(int nCnt)
	{
		return m_nCnt = nCnt;
	}

	//�������ü���
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

