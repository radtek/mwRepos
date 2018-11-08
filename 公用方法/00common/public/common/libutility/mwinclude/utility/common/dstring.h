#ifndef DSTRING_H
#define DSTRING_H

#include <string>
#include "utility/common/mwcore_export.h"
#include "utility/common/types.h"

namespace UTI{
namespace COM{

#define DSTRING_NPOS		-1

//#define MEM_STRING
#ifdef MEM_STRING
	typedef std::basic_string <char, std::char_traits<char>, CMcStrAllocator<char> > mem_string;
#else
	typedef std::string mem_string;
#endif

//CRefString
class CRefString
{
public:
	CRefString();
	CRefString(const CRefString&);
	CRefString(const char*);
	CRefString(const char*, size_t);
	CRefString(const mem_string& str);
	~CRefString();
	mem_string& str();
	int32 addRef();
	int32 subRef();
	int32 refCnt();

private:
	volatile uint32 m_nRef;
	mem_string m_Str;
};

class UTILITY_Export DString
{
public:
	enum { npos = DSTRING_NPOS };
	DString& Format(const char* fmt, ...);

public:
	DString(void);
	virtual ~DString(void);
	DString(const char*);
	DString(const char*, size_t);
	DString(uint8);
	DString(uint16);
	DString(int32);
	DString(uint32);
	DString(int64);
	DString(uint64);
	DString(double);
	DString(const DString&);
	DString(const std::string& src);

	int32 toint() const;
	uint32 touint32() const;
	int64 toint64() const;
	double todouble() const;
	DString& tolower();
	DString& toupper();
	DString tohex();

	void trim(const char* chars = " \t");
	void trimleft(const char* chars = " \t");
	void trimright(const char* chars = " \t");


	//! 仅仅拷贝内容，没有拷贝引用计数
	uint32 copy(const DString& src); //

	//把当前串中以pos开始的n个字符拷贝到以s为起始位置的字符数组中，返回实际拷贝的数目
	uint32 copy(char *s, int32 n, int32 pos = 0)const;

	DString& operator = (const char*);
	DString& operator = (const DString&);
	DString& operator = (const std::string&);
	DString operator + (const char*) const;
	DString operator + (const DString&) const;

	DString& operator += (const DString& str);
	DString& operator += (const char* str);
	DString& operator += (const char ch);

	bool operator == ( const char* str ) const;
	bool operator == (const DString& str) const;
	bool operator > (const DString& str) const;
	bool operator < (const DString& str) const;
	bool operator != (const DString& str) const;

	//！ 只取值，不引用， 不能修改内存的值
	const char operator[] (uint32 nPos)const;

	const char at(uint32 nPos)const;
	void setAt(uint32 nPos, uint8 c);

	//! 返回一个以null终止的c字符串, 禁止直接修改个返加指针的内容，因为可能会修改多个对象的值
	const char* c_str() const;
	const char* data() const;
	const std::string& str() const;
	std::string& str_copy();

	//! 当前字符串是否为空
	bool empty() const;

	//! 返回当前字符串的大小
	uint32 size() const;

	//！ 返回当前字符串的长度
	int32 length()const;

	//! 返回pos开始的n个字符组成的字符串
	DString substr(int32 pos = 0 , int32 n = DSTRING_NPOS) const;

	//! 返回当前容量（即string中不必增加内存即可存放的元素个数）
	uint32 capacity()const;

	//! 返回string对象中可存放的最大字符串的长度
	uint32 max_size()const;

	//! 把字符串当前大小置为len，并用字符c填充不足的部分
	void resize(int32 len,char c);

	//! 交换当前字符串与s2的值
	void swap(const DString& str);

	void clear();

	//删除pos开始的n个字符，返回修改后的字符串
	DString& erase(int32 pos = 0, int32 n = npos);

	//! 4个函数在p0位置插入字符串s中pos开始的前n个字符
	DString& insert(int32 p0, const char *s);
	DString& insert(int32 p0, const char *s, int32 n);
	DString& insert(int32 p0,const DString& s);
	DString& insert(int32 p0,const DString& s, int32 pos, int32 n);

	////此函数在p0处插入n个字符c
	DString& insert(int32 p0, int32 n, char c);

	//删除从p0开始的n0个字符，然后在p0处插入串s
	DString& replace(int32 p0, int32 n0,const char *s);

	//删除p0开始的n0个字符，然后在p0处插入字符串s的前n个字符
	DString& replace(int32 p0, int32 n0,const char *s, int32 n);

	//删除从p0开始的n0个字符，然后在p0处插入串s
	DString& replace(int32 p0, int32 n0,const DString &s);

	//删除p0开始的n0个字符，然后在p0处插入串s中从pos开始的n个字符
	DString& replace(int32 p0, int32 n0,const DString &s, int32 pos, int32 n);

	//删除p0开始的n0个字符，然后在p0处插入n个字符c
	DString& replace(int32 p0, int32 n0,int32 n, char c);

	//用c类型字符串s赋值
	DString& assign(const char *s);

	//用c字符串s开始的n个字符赋值
	DString& assign(const char *s,int32 n);

	//把字符串s赋给当前字符串
	DString& assign(const DString& s);

	//用n个字符c赋值给当前字符串
	DString& assign(int32 n,char c);

	//把字符串s中从start开始的n个字符赋给当前字符串
	DString& assign(const DString& s,int32 start,int32 n);

	//类型字符串s连接到当前字符串结尾
	DString& append(const char *s);

	//把c类型字符串s的前n个字符连接到当前字符串结尾
	DString& append(const char *s,int32 n);

	//同operator+=()
	DString& append(const DString& s);

	//把字符串s中从pos开始的n个字符连接到当前字符串的结尾
	DString& append(const DString& s,int32 pos,int32 n);

	//在当前字符串结尾添加n个字符c
	DString& append(int32 n,char c);

	//从pos开始查找字符c在当前字符串的位置
	int32 find(char c, int32 pos = 0) const;

	//从pos开始查找字符串s在当前串中的位置
	int32 find(const char *s, int32 pos = 0) const;

	//从pos开始查找字符串s中前n个字符在当前串中的位置
	int32 find(const char *s, int32 pos, int32 n) const;

	//从pos开始查找字符串s在当前串中的位置//查找成功时返回所在位置，失败返回string::npos的值
	int32 find(const DString &s, int32 pos = 0) const;

	//从pos开始从后向前查找字符c在当前串中的位置
	//从pos开始从后向前查找字符串s中前n个字符组成的字符串在当前串中的位置，成功返回所在位置，失败时返回string::npos的值
	int32 rfind(char c, int32 pos = DSTRING_NPOS) const;
	int32 rfind(const char *s, int32 pos = DSTRING_NPOS) const;
	int32 rfind(const char *s, int32 pos, int32 n = DSTRING_NPOS) const;
	int32 rfind(const DString& s,int32 pos = DSTRING_NPOS) const;

	//从pos开始查找字符c第一次出现的位置
	int32 find_first_of(char c, int32 pos = 0) const;
	int32 find_first_of(const char *s, int32 pos = 0) const;
	int32 find_first_of(const char *s, int32 pos, int32 n) const;
	int32 find_first_of(const DString& s,int32 pos = 0) const;

	//从pos开始查找当前串中第一个在s的前n个字符组成的数组里的字符的位置。查找失败返回string::npos
	int32 find_first_not_of(char c, int32 pos = 0) const;
	int32 find_first_not_of(const char *s, int32 pos = 0) const;
	int32 find_first_not_of(const char *s, int32 pos,int32 n) const;
	int32 find_first_not_of(const DString& s,int32 pos = 0) const;

	//从当前串中查找第一个不在串s中的字符出现的位置，失败返回string::npos
	int32 find_last_of(char c, int32 pos = DSTRING_NPOS) const;
	int32 find_last_of(const char *s, int32 pos = DSTRING_NPOS) const;
	int32 find_last_of(const char *s, int32 pos, int32 n = DSTRING_NPOS) const;
	int32 find_last_of(const DString& s,int32 pos = DSTRING_NPOS) const;

	int32 find_last_not_of(char c, int32 pos = DSTRING_NPOS) const;
	int32 find_last_not_of(const char *s, int32 pos = DSTRING_NPOS) const;
	int32 find_last_not_of(const char *s, int32 pos, int32 n) const;
	int32 find_last_not_of(const DString& s,int32 pos = DSTRING_NPOS) const;

	//比较当前字符串和s的大小
	int32 compare(const DString& s) const;

	//比较当前字符串从pos开始的n个字符组成的字符串与s的大小
	int32 compare(int32 pos, int32 n,const DString& s)const;

	//比较当前字符串从pos开始的n个字符组成的字符串与s中pos2开始的n2个字符组成的字符串的大小
	int32 compare(const char *s) const;
	int32 compare(int32 pos, int32 n,const DString& s,int32 pos2,int32 n2)const;
	int32 compare(int32 pos, int32 n,const char *s) const;
	int32 compare(int32 pos, int32 n,const char *s, int32 pos2) const;

	//! 大小写不敏感的比较
	int32 icompare(const DString& s) const;
	int32 icompare(const char *s) const;

	//! 判断是否以指定字符串开头
	bool begins(const DString& s) const;
	//! 判断是否以指定字符串结尾
	bool ends(const DString& s) const;

protected:
	CRefString* refStr() const;

	//! 判断是否需要重新拷贝数，仅仅当要改变内容时调用，防止修改其它对象的值
	void resetRef();

private:
	int32 refCnt();
	void subRef();

private:
	CRefString*  m_pRefStr;
};//end class

UTILITY_Export DString operator + (const char* lhs, const DString& rhs);
UTILITY_Export bool operator == (const char* p, const DString& str);
UTILITY_Export bool operator != (const char* p, const DString& str);

class CNullStr
{
public:
	static const DString& getNullStrObj_const();
	static DString& getNullStrObj();
};

#define MC_STRING_NULL_CONST    CNullStr::getNullStrObj_const()



}}//end namespace
#endif // DSTRING_H



