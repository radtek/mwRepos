//#include "UTF-8.h"

#ifndef UTF_8_H
#define UTF_8_H
#include "wchar.h"
#include <string>
#include "adapter/Adapter.h"

// ��UTF-8ת����Unicode
void UTF_8ToUnicode(WCHAR* pOut,char *pText);
// Unicode ת����UTF-8 
void UnicodeToUTF_8(char* pOut, WCHAR* pText);

// ��Unicode ת���� GB2312 
void UnicodeToGB2312(char* pOut,unsigned short uData); 

// GB2312 ת���ɡ�Unicode
void Gb2312ToUnicode(WCHAR* pOut, char *gbBuffer);

//GB2312 תΪ UTF-8
char* GB2312ToUTF_8(char *pOut, char *pText, int pLen);

//UTF-8 תΪ GB2312
char* UTF_8ToGB2312(char *pOut, char *pText, int pLen);

void ConvertGBKToUtf8(std::string& strGBK);

char *u2s( unsigned char *buf, unsigned short length, char *tmpbuf );

char *utf82ascii( unsigned char *utf8, unsigned short length, char *buf );

#endif
