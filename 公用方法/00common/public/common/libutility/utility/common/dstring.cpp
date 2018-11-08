#include "utility/common/mwcorepch.h"
#include <utility/common/dstring.h>
#include <string> 
#include "stdarg.h"
using namespace ::UTI::COM;

//Add by lmz 2010-11-03----------------------------
//将字符串中的大写字母变成小写
void String2Lower( char* pString )
{
	int      iCurPos= 0;
	int      i= 'a' - 'A';

	if ( !pString )
		return;

	while ( pString[ iCurPos ] != 0 )
	{
		if ( pString[ iCurPos ] >= 'A' && pString[ iCurPos ] <= 'Z' )
		{
			pString[ iCurPos ] += i;
		}

		iCurPos++;
	}
}

//将字符串中的小写字母变成大写
void String2Upper( char* pString )
{
	int      iCurPos= 0;
	int      i= 'A' - 'a';

	if ( !pString )
		return;

	while ( pString[ iCurPos ] != 0 )
	{
		if ( pString[ iCurPos ] >= 'a' && pString[ iCurPos ] <= 'z' )
		{
			pString[ iCurPos ] += i;
		}

		iCurPos++;
	}
}

//#define MEMPOOL_REF_STR_SWITCH
#if defined(MEMPOOL_REF_STR_SWITCH)
#   define MEMPOOL_REF_STR_NEW(pNewNode, memPool, Class, ...) \
		do{\
			CDynMemPool* pPool = &(memPool); \
			pNewNode = (Class*)(pPool->getMemNode());  \
			::new ((Class*)pNewNode) Class( __VA_ARGS__ );  \
		}while(0)
#   define MEMPOOL_REF_STR_DEL(memPool, pObjPtr)    \
		do{ \
			CDynMemPool* pPool = &(memPool); \
			if(pObjPtr){  \
				pObjPtr->~CRefString();  \
				pPool->putMemNode((void*)pObjPtr);   \
				pObjPtr = NULL; \
			}   \
		}while(0)
#else
#   define MEMPOOL_REF_STR_NEW(pNewNode, memPool, Class, ...) \
		do{\
			pNewNode = (Class*)(::operator new(sizeof(Class)) );  \
			::new ((Class*)pNewNode) Class( __VA_ARGS__ );  \
		}while(0)
#   define MEMPOOL_REF_STR_DEL(memPool, pObjPtr)    \
		do{ \
			if(pObjPtr){  \
				pObjPtr->~CRefString();  \
				::operator delete((void*)pObjPtr);   \
				pObjPtr = NULL; \
			}   \
		}while(0)
#endif


DString operator + (const char* lhs, const DString& rhs)
{
	DString str(lhs);
	return str+rhs;
}
bool operator == ( const char* p, const DString& str )
{
	return 0 == strcmp(p, str.c_str() );
}
bool operator != (const char* p, const DString& str)
{
	return 0 != strcmp(p, str.c_str() );
}

CRefString::CRefString()
	:m_Str()
	,m_nRef(0)
{
}

CRefString::CRefString( const CRefString& src)
	:m_Str(((CRefString&)src).str().c_str() == NULL ? "" : ((CRefString&)src).str().c_str())
	//:m_pStr()
	,m_nRef(0)
{
}

CRefString::CRefString( const char* src)
	:m_Str(src == NULL ? "" : src)
	,m_nRef(0)
{
}

CRefString::CRefString(const char* pSrc, size_t strSize)
	:m_Str(pSrc, strSize)
	,m_nRef(0)
{
}

CRefString::CRefString( const mem_string& str )
	:m_Str(str.c_str() == NULL ? "" : str.c_str())
	,m_nRef(0)
{
}

CRefString::~CRefString()
{
}

mem_string& CRefString::str()
{
	return m_Str;
}

int32 CRefString::addRef()
{
	return ++m_nRef;
}

int32 CRefString::subRef()
{
	if (0 == m_nRef)
	{
		//         delete this;
		CRefString* pObj = this;
		MEMPOOL_REF_STR_DEL(getStringMemPool(), pObj);
		return -1;
	}
	else
	{
		return --m_nRef;
	}
}

int32 CRefString::refCnt()
{
	return m_nRef;
}


DString::DString()
{
     MEMPOOL_REF_STR_NEW(m_pRefStr, getStringMemPool(), CRefString);
}

DString::~DString()
{
    subRef();
}

DString::DString(const char *src)
{
    MEMPOOL_REF_STR_NEW(m_pRefStr, getStringMemPool(), CRefString, src);
}
DString::DString(const char *src, size_t strSize)
{
    MEMPOOL_REF_STR_NEW(m_pRefStr, getStringMemPool(), CRefString, src, strSize);
}
DString::DString(uint8 src)
	:m_pRefStr(NULL)
{
    Format("%c", src);
}

DString::DString(uint16 src)
    :m_pRefStr(NULL)
{
    Format("%hu", src);
}

DString::DString(int32 src)
    :m_pRefStr(NULL)
{
    Format("%i", src);
}
DString::DString(uint32 src)
    :m_pRefStr(NULL)
{
    Format("%u", src);
}

DString::DString(int64 src)
    :m_pRefStr(NULL)
{
    Format("%lld", src);
}

DString::DString(uint64 src)
    :m_pRefStr(NULL)
{
    Format("%llu", src);
}

DString::DString(double src)
    :m_pRefStr(NULL)
{
    Format("%f", src);
}

DString::DString(const DString& src)
{
    m_pRefStr = src.refStr();
	if(NULL != m_pRefStr)
    {
		m_pRefStr->addRef();
	}
}

DString::DString(const std::string& src)
{
	MEMPOOL_REF_STR_NEW(m_pRefStr, getStringMemPool(), CRefString, src.c_str() );
}

DString& DString::Format(const char* format, ...)
{
    resetRef();

    va_list paramList;
    va_start(paramList, format);
    int32 size = vsnprintf(NULL, 0, format, paramList);
    va_start(paramList, format);
    m_pRefStr->str().resize(size);
    vsnprintf((char*)this->c_str(), size + 1, format, paramList);
    va_end(paramList);
    return *this;
}

DString& DString::operator = (const char* src)
{
    subRef();
    MEMPOOL_REF_STR_NEW(m_pRefStr, getStringMemPool(), CRefString, src);
    return *this;
}

DString& DString::operator = (const DString& src)
{
    if (m_pRefStr != src.refStr())
    {
        subRef();
        m_pRefStr = src.refStr();
        m_pRefStr->addRef();
    }

    return *this;
}

DString& DString::operator = (const std::string& src)
{
	subRef();
	MEMPOOL_REF_STR_NEW(m_pRefStr, getStringMemPool(), CRefString, src.c_str() );
	return *this;
}

DString DString::operator + (const char* src) const
{
    DString str;
    str.copy(*this);
    str.m_pRefStr->str() += src;
    return str;
}

DString DString::operator + (const DString &src) const
{
    DString str;
    str.copy(*this);
    str.m_pRefStr->str() += src.m_pRefStr->str();
    return str;
}

int32 DString::toint() const
{
    int32 lRet =0;
    if(NULL !=refStr()->str().c_str())
    {
       return atoi(refStr()->str().c_str());
    }
    return 0;
}

uint32 DString::touint32() const
{
    char* _c_str = (char*)c_str();
    uint32 ret = *_c_str - '0';
    _c_str++;

    while(*_c_str != '\0')
    {
        ret = ret * 10 + (*_c_str - '0');
        _c_str++;
    }

    return ret;
}

int64 DString::toint64() const
{
    double lResult = 0.0;
    lResult = todouble();
    return (int64)lResult;
}

double DString::todouble() const
{
    double lResult = 0.0;
    if(NULL != refStr()->str().c_str())
    {
        return atof(refStr()->str().c_str());
    }
    return lResult;
}

DString& DString::tolower()
{
    if(m_pRefStr)
    {
        resetRef();
        String2Lower((char *)m_pRefStr->str().c_str());
    }
    return *this;
}

DString& DString::toupper()
{
    if(m_pRefStr)
    {
        resetRef();
        String2Upper((char *)m_pRefStr->str().c_str());
    }
    return *this;
}

DString DString::tohex()
{
	DString str;
	str.m_pRefStr->str().resize(size()*2);
	char szTemp[10];
	memset(szTemp, 0, 10);
	const char *p = c_str();

	for(uint32 i = 0; i < length(); i++)
	{
		sprintf(szTemp, "%x ", (unsigned char)p[i]);
		str += szTemp;
	}
	return str;
}

void DString::trim(const char* chars)
{
    trimleft(chars);
    trimright(chars);
}

void DString::trimleft(const char* chars)
{
    if (m_pRefStr)
    {
        resetRef();
        m_pRefStr->str().erase(0, m_pRefStr->str().find_first_not_of(chars));
    }
}

void DString::trimright(const char* chars)
{
    if (m_pRefStr)
    {
        resetRef();
        m_pRefStr->str().erase(m_pRefStr->str().find_last_not_of(chars)+1);
    }
}

const char* DString::c_str() const
{
    if (m_pRefStr)
    {
        return m_pRefStr->str().c_str();
    }

    return "";
}

const char* DString::data() const
{
    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().data();
    }

    return "";
}

const std::string& DString::str() const
{
	return m_pRefStr->str();
}

std::string& DString::str_copy()
{
	CRefString* pNew = NULL;
	MEMPOOL_REF_STR_NEW(pNew, getStringMemPool(), CRefString);
	pNew->str() = m_pRefStr->str();

	subRef();
	m_pRefStr = pNew;
	return m_pRefStr->str();
}

CRefString* DString::refStr() const
{
    return m_pRefStr;
}

void DString::subRef()
{
    if (m_pRefStr)
    {
        m_pRefStr->subRef();
        m_pRefStr = NULL;
    }
}

uint32 DString::copy( const DString& src )
{
    if (m_pRefStr != src.refStr())
    {
        subRef();
//         m_pRefStr = new CRefString(*(src.refStr()));
        MEMPOOL_REF_STR_NEW(m_pRefStr, getStringMemPool(), CRefString, *(src.refStr() ) );
    }

    //! solve compile error
    return 0;
}

uint32 DString::copy( char *s, int32 n, int32 pos /*= 0*/ ) const
{
    int32 nRet = 0;

	if(NULL == s)
	{
		return 0;
	}

    if(NULL != m_pRefStr)
    {
        nRet = m_pRefStr->str().copy(s, n, pos);
    }

    return nRet;
}


bool DString::empty() const
{
    if (m_pRefStr)
    {
        return m_pRefStr->str().empty();
    }
    else
    {
        return true;
    }
}

uint32 DString::size() const
{
    if (m_pRefStr)
    {
        return m_pRefStr->str().size();
    }
    else
    {
        return 0;
    }
}

int32 DString::length() const
{
    return size();
}

int32 DString::refCnt()
{
    if (m_pRefStr)
    {
        return m_pRefStr->refCnt();
    }
    else
    {
        return -1;
    }
}

void DString::resetRef()
{
    if(0 < refCnt())
    {
        //当有多个对象引用这个值,先生成一个副本，再再清除引用计算，防止改了其它对象的值
        DString str;
        str.copy(*this);

        //清除引用计数，
        subRef();
        *this = str;
    }

    //! 如果为空，分配一个变量
    if(NULL == m_pRefStr)
    {
//         m_pRefStr = new CRefString();
        MEMPOOL_REF_STR_NEW(m_pRefStr, getStringMemPool(), CRefString);
    }
}

bool DString::operator==( const char* str ) const
{
    if(NULL == str || NULL == m_pRefStr)
    {
        return true;
    }

    if(NULL != m_pRefStr && NULL != str)
    {
        return (0 == strcmp(m_pRefStr->str().c_str(), str));
    }

    return false;
}

bool DString::operator==( const DString& str )const
{
    //如果比较自己或相同的引用，真接返回
    if(this == &str || (empty() && str.empty()) || m_pRefStr == str.m_pRefStr)
    {
        return true;
    }

    if(NULL != m_pRefStr && NULL != str.m_pRefStr)
    {
        return 0 == strcmp(m_pRefStr->str().c_str(), str.m_pRefStr->str().c_str());
    }

    return false;
}

bool DString::operator>( const DString& str )const
{
    //如果比较自己或相同的引用，真接返回
    if(this == &str || m_pRefStr == str.m_pRefStr)
    {
        return false;
    }

    if(0 != size() && 0 == str.size())
    {
        return true;
    }

    if(NULL != m_pRefStr && NULL != str.m_pRefStr)
    {
        return strcmp(m_pRefStr->str().c_str(), str.m_pRefStr->str().c_str()) > 0;
    }

    return false;
}

bool DString::operator<( const DString& str )const
{
    //如果比较自己或相同的引用，真接返回
    if(this == &str || m_pRefStr == str.m_pRefStr)
    {
        return false;
    }

    if(0 == size() && 0 < str.size())
    {
        return true;
    }

    if(NULL != m_pRefStr && NULL != str.m_pRefStr)
    {
        return strcmp(m_pRefStr->str().c_str(), str.m_pRefStr->str().c_str()) < 0;
    }

    return false;
}

bool DString::operator != ( const DString& str ) const
{
    return !(*this == str);
}

const char DString::operator[](uint32 nPos) const
{
    if(NULL != m_pRefStr && nPos < size())
    {
        return m_pRefStr->str()[nPos];
    }

    return '\0';
}

DString DString::substr( int32 pos /*= 0*/,int32 n /*= npos*/ ) const
{
    DString str;
    if((n == DSTRING_NPOS || n > 0) && pos < size())
    {
        mem_string strSub = m_pRefStr->str().substr(pos, n);
        str = strSub.c_str();
    }

    return str;
}

uint32 DString::capacity() const
{
    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().capacity();
    }

    return 0;
}

uint32 DString::max_size() const
{
    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().max_size();
    }

    return 0;
}

void DString::resize(int32 len, char c)
{
    resetRef();
    m_pRefStr->str().resize(len, c);
}

void DString::swap(const DString& str)
{
    //！ 进行指针切换
    CRefString* pTemp = str.m_pRefStr;
    ((DString&)str).m_pRefStr = m_pRefStr;
    m_pRefStr = pTemp;
}

//string::const_iterator McString_::begin() const
//{
//    string::const_iterator iter;
//    if(NULL != m_pRefStr)
//    {
//        iter = m_pRefStr->str().cbegin();
//    }
//
//    return iter;
//}
//
//string::iterator McString_::begin()
//{
//    string::iterator iter;
//    if(NULL != m_pRefStr)
//    {
//        iter = m_pRefStr->str().begin();
//    }
//
//    return iter;
//}
//
//string::const_iterator McString_::end() const
//{
//    string::const_iterator iter;
//    if(NULL != m_pRefStr)
//    {
//        iter = m_pRefStr->str().end();
//    }
//
//    return iter;
//}
//
//string::iterator McString_::end()
//{
//    string::iterator iter;
//    if(NULL != m_pRefStr)
//    {
//        iter = m_pRefStr->str().end();
//    }
//
//    return iter;
//}
//
//string::const_reverse_iterator McString_::rbegin() const
//{
//    string::const_reverse_iterator iter;
//    if(NULL != m_pRefStr)
//    {
//        iter = m_pRefStr->str().crbegin();
//    }
//
//    return iter;
//}
//
//string::reverse_iterator McString_::rbegin()
//{
//    string::reverse_iterator iter;
//    if(NULL != m_pRefStr)
//    {
//        iter = m_pRefStr->str().rbegin();
//    }
//
//    return iter;
//}
//
//string::const_reverse_iterator McString_::rend() const
//{
//    string::const_reverse_iterator iter;
//    if(NULL != m_pRefStr)
//    {
//        iter = m_pRefStr->str().crend();
//    }
//
//    return iter;
//}
//
//string::reverse_iterator McString_::rend()
//{
//    string::reverse_iterator iter;
//    if(NULL != m_pRefStr)
//    {
//        iter = m_pRefStr->str().rend();
//    }
//
//    return iter;
//}
//
//string::iterator McString_::erase( string::iterator first, string::iterator last )
//{
//    resetRef();
//    string::iterator iter;
//    if(NULL != m_pRefStr)
//    {
//        iter = m_pRefStr->str().erase(first, last);
//    }
//
//    return iter;
//}
//
//string::iterator McString_::erase( string::iterator it )
//{
//    resetRef();
//    string::iterator iter;
//    if(NULL != m_pRefStr)
//    {
//        iter = m_pRefStr->str().erase(it);
//    }
//
//    return iter;
//}

DString& DString::erase( int32 pos /*= 0*/, int32 n /*= npos*/ )
{
    if(NULL != m_pRefStr)
    {
        resetRef();
        m_pRefStr->str().erase(pos, n);
    }

    return *this;
}

DString& DString::insert( int32 p0, const char *s )
{
	if(NULL == s)
	{
		return *this;
	}

    resetRef();
    m_pRefStr->str().insert(p0, s);
    return *this;
}

DString& DString::insert( int32 p0, const char *s, int32 n )
{
	if(NULL == s)
	{
		return *this;
	}

    resetRef();
    m_pRefStr->str().insert(p0, s, n);
    return *this;
}

DString& DString::insert( int32 p0,const DString& s )
{
    resetRef();
    m_pRefStr->str().insert(p0, s.c_str());
    return *this;
}

DString& DString::insert( int32 p0, const DString &s, int32 pos, int32 n )
{
    if(NULL != s.m_pRefStr)
    {
        resetRef();
        m_pRefStr->str().insert(p0, s.m_pRefStr->str(), pos, n);
    }

    return *this;
}

DString& DString::insert( int32 p0, int32 n, char c )
{
    resetRef();
    m_pRefStr->str().insert(p0, n, c);
    return *this;
}

DString& DString::replace( int32 p0, int32 n0,const char *s )
{
    resetRef();
    m_pRefStr->str().replace(p0, n0, s);
    return *this;
}

DString& DString::replace( int32 p0, int32 n0,const char *s, int32 n )
{
    resetRef();
    m_pRefStr->str().replace(p0, n0, s, n);
    return *this;
}

DString& DString::replace( int32 p0, int32 n0,const DString &s )
{
    if(NULL != s.m_pRefStr)
    {
        resetRef();
        m_pRefStr->str().replace(p0, n0, s.m_pRefStr->str());
    }

    return *this;
}

DString& DString::replace( int32 p0, int32 n0,const DString &s, int32 pos, int32 n )
{
    if(NULL != s.m_pRefStr)
    {
        resetRef();
        m_pRefStr->str().replace(p0, n0, s.m_pRefStr->str(), pos, n);
    }

    return *this;
}

DString& DString::replace( int32 p0, int32 n0,int32 n, char c )
{
    resetRef();
    m_pRefStr->str().replace(p0, n0, n, c);
    return *this;
}

DString& DString::assign( const char *s )
{
	if(NULL == s)
	{
		return *this;
	}

    resetRef();
    m_pRefStr->str().assign(s);
    return *this;
}

DString& DString::assign( const char *s,int32 n )
{
	if(NULL == s)
	{
		return *this;
	}

    resetRef();
    m_pRefStr->str().assign(s, n);
    return *this;
}

DString& DString::assign( const DString& s )
{
    if(NULL != s.m_pRefStr)
    {
        resetRef();
        m_pRefStr->str().assign(s.m_pRefStr->str());
    }

    return *this;
}

DString& DString::assign( int32 n,char c )
{
    resetRef();
    m_pRefStr->str().assign(n, c);
    return *this;
}

DString& DString::assign( const DString& s,int32 start,int32 n )
{
    if(NULL != s.m_pRefStr)
    {
        resetRef();
        m_pRefStr->str().assign(s.m_pRefStr->str(), start, n);
    }

    return *this;
}

DString& DString::append( const char *s )
{
	if(NULL == s)
	{
		return *this;
	}

    resetRef();
    m_pRefStr->str().append(s);
    return *this;
}

DString& DString::append( const char *s,int32 n )
{
	if(NULL == s)
	{
		return *this;
	}

    resetRef();
    m_pRefStr->str().append(s, n);
    return *this;
}

DString& DString::append( const DString& s )
{
    if(NULL != s.m_pRefStr)
    {
        resetRef();
        m_pRefStr->str().append(s.m_pRefStr->str());
    }

    return *this;
}

DString& DString::append( const DString& s,int32 pos,int32 n )
{
    if(NULL != s.m_pRefStr)
    {
        resetRef();
        m_pRefStr->str().append(s.m_pRefStr->str(), pos, n);
    }

    return *this;
}

DString& DString::append( int32 n,char c )
{
    resetRef();
    m_pRefStr->str().append(n, c);
    return *this;
}

int32 DString::find( char c, int32 pos /*= 0*/ ) const
{
    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().find(c, pos);
    }

    return DSTRING_NPOS;
}

int32 DString::find( const char *s, int32 pos /*= 0*/ ) const
{
	if(NULL == s)
	{
		return DSTRING_NPOS;
	}

    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().find(s, pos);
    }

    return DSTRING_NPOS;
}

int32 DString::find( const char *s, int32 pos, int32 n ) const
{
	if(NULL == s)
	{
		return DSTRING_NPOS;
	}

    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().find(s, pos, n);
    }

    return DSTRING_NPOS;
}

int32 DString::find( const DString& s, int32 pos /*= 0*/ ) const
{
    if(NULL != m_pRefStr && NULL != s.m_pRefStr)
    {
        return m_pRefStr->str().find(s.m_pRefStr->str(), pos);
    }

    return DSTRING_NPOS;
}

int32 DString::rfind( char c, int32 pos /*= npos*/ ) const
{
    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().rfind(c, pos);
    }

    return DSTRING_NPOS;
}

int32 DString::rfind( const char *s, int32 pos /*= DSTRING_NPOS*/ ) const
{
	if(NULL == s)
	{
		return DSTRING_NPOS;
	}

    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().rfind(s, pos);
    }

    return DSTRING_NPOS;
}

int32 DString::rfind( const char *s, int32 pos, int32 n /*= DSTRING_NPOS*/ ) const
{
	if(NULL == s)
	{
		return DSTRING_NPOS;
	}

    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().rfind(s, pos, n);
    }

    return DSTRING_NPOS;
}

int32 DString::rfind( const DString& s,int32 pos /*= DSTRING_NPOS*/ ) const
{
    if(NULL != m_pRefStr && NULL != s.m_pRefStr)
    {
        return m_pRefStr->str().rfind(s.m_pRefStr->str(), pos);
    }

    return DSTRING_NPOS;
}

int32 DString::find_first_of( char c, int32 pos /*= 0*/ ) const
{
    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().find_first_of(c, pos);
    }

    return DSTRING_NPOS;
}

int32 DString::find_first_of( const char *s, int32 pos /*= 0*/ ) const
{
	if(NULL == s)
	{
		return DSTRING_NPOS;
	}

    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().find_first_of(s, pos);
    }

    return DSTRING_NPOS;
}

int32 DString::find_first_of( const char *s, int32 pos, int32 n ) const
{
	if(NULL == s)
	{
		return DSTRING_NPOS;
	}

    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().find_first_of(s, pos, n);
    }

    return DSTRING_NPOS;
}

int32 DString::find_first_of( const DString& s,int32 pos /*= 0*/ ) const
{
    if(NULL != m_pRefStr || NULL != s.m_pRefStr)
    {
        return m_pRefStr->str().find_first_of(s.m_pRefStr->str(), pos);
    }

    return DSTRING_NPOS;
}

int32 DString::find_first_not_of( char c, int32 pos /*= 0*/ ) const
{
    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().find_first_not_of(c, pos);
    }

    return DSTRING_NPOS;
}

int32 DString::find_first_not_of( const char *s, int32 pos /*= 0*/ ) const
{
	if(NULL == s)
	{
		return DSTRING_NPOS;
	}

    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().find_first_not_of(s, pos);
    }

    return DSTRING_NPOS;
}

int32 DString::find_first_not_of( const char *s, int32 pos,int32 n ) const
{
	if(NULL == s)
	{
		return DSTRING_NPOS;
	}

    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().find_first_not_of(s, pos, n);
    }

    return DSTRING_NPOS;
}

int32 DString::find_first_not_of( const DString& s,int32 pos /*= 0*/ ) const
{
    if(NULL != m_pRefStr && NULL != s.m_pRefStr)
    {
        return m_pRefStr->str().find_first_not_of(s.m_pRefStr->str(), pos);
    }

    return DSTRING_NPOS;
}

int32 DString::find_last_of( char c, int32 pos /*= DSTRING_NPOS*/ ) const
{
    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().find_last_of(c, pos);
    }

    return DSTRING_NPOS;
}

int32 DString::find_last_of( const char *s, int32 pos /*= DSTRING_NPOS*/ ) const
{
	if(NULL == s)
	{
		return DSTRING_NPOS;
	}

    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().find_last_of(s, pos);
    }

    return DSTRING_NPOS;
}

int32 DString::find_last_of( const char *s, int32 pos, int32 n /*= DSTRING_NPOS*/ ) const
{
	if(NULL == s)
	{
		return DSTRING_NPOS;
	}

    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().find_last_of(s, pos, n);
    }

    return DSTRING_NPOS;
}

int32 DString::find_last_of( const DString& s,int32 pos /*= DSTRING_NPOS*/ ) const
{
    if(NULL != m_pRefStr && NULL != s.m_pRefStr)
    {
        return m_pRefStr->str().find_last_of(s.m_pRefStr->str(), pos);
    }

    return DSTRING_NPOS;
}

int32 DString::find_last_not_of( char c, int32 pos /*= DSTRING_NPOS*/ ) const
{
    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().find_last_not_of(c, pos);
    }

    return DSTRING_NPOS;
}

int32 DString::find_last_not_of( const char *s, int32 pos /*= DSTRING_NPOS*/ ) const
{
	if(NULL == s)
	{
		return DSTRING_NPOS;
	}

    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().find_last_not_of(s, pos);
    }

    return DSTRING_NPOS;
}

int32 DString::find_last_not_of( const char *s, int32 pos, int32 n ) const
{
	if(NULL == s)
	{
		return DSTRING_NPOS;
	}

    if(NULL != m_pRefStr)
    {
        return m_pRefStr->str().find_last_not_of(s, pos, n);
    }

    return DSTRING_NPOS;
}

int32 DString::find_last_not_of( const DString& s,int32 pos /*= DSTRING_NPOS*/ ) const
{
    if(NULL != m_pRefStr && NULL != s.m_pRefStr)
    {
        m_pRefStr->str().find_last_not_of(s.m_pRefStr->str(), pos);
    }

    return DSTRING_NPOS;
}

const char DString::at( uint32 nPos ) const
{
    if(NULL != m_pRefStr && size() > nPos)
    {
        return m_pRefStr->str()[nPos];
    }

    return '\0';
}

void DString::setAt( uint32 nPos, uint8 c )
{
    if(size() > nPos)
    {
        resetRef();
        m_pRefStr->str()[nPos] = c;
    }
}

int32 DString::compare( const DString& s ) const
{
    if(NULL == m_pRefStr && NULL != s.m_pRefStr)
    {
        return -1;
    }

    if(NULL == m_pRefStr && NULL == s.m_pRefStr)
    {
        return 0;
    }

    if(NULL != m_pRefStr && NULL == s.m_pRefStr)
    {
        return 1;
    }

    return m_pRefStr->str().compare(s.m_pRefStr->str());
}

int32 DString::compare( int32 pos, int32 n,const DString& s ) const
{
    if(NULL == m_pRefStr && NULL != s.m_pRefStr)
    {
        return -1;
    }

    if(NULL == m_pRefStr && NULL == s.m_pRefStr)
    {
        return 0;
    }

    if(NULL != m_pRefStr && NULL == s.m_pRefStr)
    {
        return 1;
    }

    return m_pRefStr->str().compare(pos, n, s.m_pRefStr->str());
}

int32 DString::compare( const char *s ) const
{
    if(NULL == m_pRefStr && NULL != s)
    {
        return -1;
    }

    if(NULL == m_pRefStr && NULL == s)
    {
        return 0;
    }

    if(NULL != m_pRefStr && NULL == s)
    {
        return 1;
    }

    return m_pRefStr->str().compare(s);
}

int32 DString::compare( int32 pos, int32 n,const DString& s,int32 pos2,int32 n2 ) const
{
    if(NULL == m_pRefStr && NULL != s.m_pRefStr)
    {
        return -1;
    }

    if(NULL == m_pRefStr && NULL == s.m_pRefStr)
    {
        return 0;
    }

    if(NULL != m_pRefStr && NULL == s.m_pRefStr)
    {
        return 1;
    }

    return m_pRefStr->str().compare(pos, n, s.m_pRefStr->str(), pos2, n2);
}

int32 DString::compare( int32 pos, int32 n,const char *s ) const
{
    if(NULL == m_pRefStr && NULL != s)
    {
        return -1;
    }

    if(NULL == m_pRefStr && NULL == s)
    {
        return 0;
    }

    if(NULL != m_pRefStr && NULL == s)
    {
        return 1;
    }

    return m_pRefStr->str().compare(pos, n, s);
}

int32 DString::compare( int32 pos, int32 n,const char *s, int32 pos2 ) const
{
    if(NULL == m_pRefStr && NULL != s)
    {
        return -1;
    }

    if(NULL == m_pRefStr && NULL == s)
    {
        return 0;
    }

    if(NULL != m_pRefStr && NULL == s)
    {
        return 1;
    }

    return m_pRefStr->str().compare(pos, n, s, pos);
}

void DString::clear()
{
    resetRef();
	if (m_pRefStr)
	{
		m_pRefStr->str().clear();
	}
}

DString& DString::operator+=( const DString& str )
{
	resetRef();
	m_pRefStr->str() += str.m_pRefStr->str();

	return *this;
}

DString& DString::operator+=( const char* str )
{
	if(NULL != str)
	{
		resetRef();
		m_pRefStr->str() += str;
	}

	return *this;
}

DString& DString::operator+=( const char ch )
{
    resetRef();
    m_pRefStr->str() += ch;

    return *this;
}

int32 DString::icompare( const DString& s ) const
{
    if(NULL == m_pRefStr && NULL != s.m_pRefStr)
    {
        return -1;
    }

    if(NULL == m_pRefStr && NULL == s.m_pRefStr)
    {
        return 0;
    }

    if(NULL != m_pRefStr && NULL == s.m_pRefStr)
    {
        return 1;
    }

#ifndef MW_WIN
	return strcasecmp(m_pRefStr->str().c_str(), s.c_str());
#else
	return stricmp(m_pRefStr->str().c_str(), s.c_str());
#endif
}

int32 DString::icompare( const char *s ) const
{
    if(NULL == m_pRefStr && NULL != s)
    {
        return -1;
    }

    if(NULL == m_pRefStr && NULL == s)
    {
        return 0;
    }

    if(NULL != m_pRefStr && NULL == s)
    {
        return 1;
    }

#ifndef MW_WIN
    return strcasecmp(m_pRefStr->str().c_str(), s);
#else
	return stricmp(m_pRefStr->str().c_str(), s);
#endif
}

bool DString::begins(const DString& s) const
{
	if(0 == s.size())
	{
		return false;
	}

	if(s.size() > size())
	{
		return false;
	}

	if(0 == strncmp(m_pRefStr->str().c_str(), s.c_str(), s.size()))
	{
		return true;
	}

	return false;
}

bool DString::ends( const DString& s ) const
{
    int32 len = s.size();
    if(0 == len)
    {
        return false;
    }

    if(len > size())
    {
        return false;
    }

    char *str1 = (char*)m_pRefStr->str().c_str() + length() - len;
    char *str2 = (char*)s.c_str();
    do
    {
        if (*(str1++) != *(str2++))
        {
            return false;
        }
    }while(--len);

    return true;
}


static DString g_sNullStrObj;

const DString& CNullStr::getNullStrObj_const()
{
    return g_sNullStrObj;
}
DString& CNullStr::getNullStrObj()
{
    return g_sNullStrObj;
}
