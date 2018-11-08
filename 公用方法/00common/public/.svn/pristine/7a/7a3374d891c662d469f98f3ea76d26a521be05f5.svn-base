//#include "UTF-8.h"

#ifndef UTF_8_H
#define UTF_8_H
#include "wchar.h"
#include <string>
#include "adapter/Adapter.h"

// 把UTF-8转换成Unicode
void UTF_8ToUnicode(WCHAR* pOut,char *pText);
// Unicode 转换成UTF-8 
void UnicodeToUTF_8(char* pOut, WCHAR* pText);

// 把Unicode 转换成 GB2312 
void UnicodeToGB2312(char* pOut,unsigned short uData); 

// GB2312 转换成　Unicode
void Gb2312ToUnicode(WCHAR* pOut, char *gbBuffer);

//GB2312 转为 UTF-8
char* GB2312ToUTF_8(char *pOut, char *pText, int pLen);

//UTF-8 转为 GB2312
char* UTF_8ToGB2312(char *pOut, char *pText, int pLen);

void ConvertGBKToUtf8(std::string& strGBK);

char *u2s( unsigned char *buf, unsigned short length, char *tmpbuf );

char *utf82ascii( unsigned char *utf8, unsigned short length, char *buf );

#endif
