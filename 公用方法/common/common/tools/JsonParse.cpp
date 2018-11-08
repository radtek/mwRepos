#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include "JsonParse.h"
using namespace std;


void CJSON::parseJsonText(const char *pStart, str_strMap &strMap)
{
	assert(NULL != pStart);

	char *p = const_cast<char *>(pStart);
	m_doc.Parse<0>(p);

	using rapidjson::Value;
	Value v;

	if (!m_doc.IsArray())
	{
		return;
	}

	int nCnt = m_doc.Size();

	for (int i = 0; i < nCnt; ++i)
	{
		rapidjson::Value::MemberIterator iter = m_doc[i].MemberBegin();
		rapidjson::Value::MemberIterator iterEnd = m_doc[i].MemberEnd();

		for (; iter != iterEnd; ++iter)
		{
			cout << iter->name.GetString() << ":" << iter->value.GetString();
			strMap.insert(make_pair(iter->name.GetString(), iter->value.GetString()));
		}
		cout << "........................." << endl;
	}
}

void CJSON::parseJsonArryText(const char *pStart, str_strMultiMap &strMultiMap)
{
	assert(NULL != pStart);

	char *p = const_cast<char *>(pStart);
	m_doc.Parse<0>(p);

	using rapidjson::Value;
	Value v;

	if (!m_doc.IsArray())
	{
		return;
	}

	int nCnt = m_doc.Size();

	for (int i = 0; i < nCnt; ++i)
	{
		rapidjson::Value::MemberIterator iter = m_doc[i].MemberBegin();
		rapidjson::Value::MemberIterator iterEnd = m_doc[i].MemberEnd();

		for (; iter != iterEnd; ++iter)
		{
			cout << iter->name.GetString() << ":" << iter->value.GetString();
			strMultiMap.insert(make_pair(iter->name.GetString(), iter->value.GetString()));
		}
		cout << "........................." << endl;
	}
}

int CJSON::parseJsonStrValue(char *szIn, char *szOut, size_t nMaxSize, Document &doc)
{
	int nLen = 0;
	using rapidjson::Value;

	try
	{
		if (doc.HasMember(szIn))
		{
			Value & v = doc[szIn];
			if (v.IsString())
			{
				nLen = v.GetStringLength();
				memcpy(szOut, v.GetString(), MIN2((size_t)nLen, nMaxSize));
				szOut[nLen] = '\0';
			}
		}
	}
	catch (...)
	{
		nLen = 0;
		szOut[nLen] = '\0';
	}

	return nLen;
}

int CJSON::parseJsonStrValue(char *szIn, char *szOut, size_t nMaxSize, Value &val)
{
	int nLen = 0;
	using rapidjson::Value;

	try
	{
		if (val.HasMember(szIn))
		{
			Value & v = val[szIn];
			if (v.IsString())
			{
				nLen = v.GetStringLength();
				memcpy(szOut, v.GetString(), MIN2((size_t)nLen, nMaxSize));
				szOut[nLen] = '\0';
			}
		}
	}
	catch (...)
	{
		nLen = 0;
		szOut[nLen] = '\0';
	}

	return nLen;
}

// int CJSON::parseJsonIntValue(char *szIn, Document &doc, int &nErr)
// {
// 	int nRet = 0;
// 	using rapidjson::Value;
// 	nErr = -1;
// 
// 	try
// 	{
// 		if (doc.HasMember(szIn))
// 		{
// 			Value & v = doc[szIn];
// 			if (v.IsInt())
// 			{
// 				nRet = v.GetInt();
// 				nErr = 0;
// 			}
// 		}
// 	}
// 	catch (...)
// 	{
// 		nErr = -1;
// 	}
// 
// 	return nRet;
// }

int CJSON::parseJsonIntValue(char *szIn, Document &doc, int &nValue)
{
	int nErr = -1;
	int nTmp = nValue;
	try
	{
		if (doc.HasMember(szIn))
		{
			Value & v = doc[szIn];
			if (v.IsInt())
			{
				nTmp = v.GetInt();
				nErr = 0;
			}
		}
	}
	catch (...)
	{
		nErr = -1;
	}
	
	if (0 == nErr)
	{
		nValue = nTmp;
	}
	
	return nErr;
}

// int CJSON::parseJsonIntValue(char *szIn, Value &val, int &nErr)
// {
// 	int nLen = 0;
// 	using rapidjson::Value;
// 	nErr = -1;
// 	if (val.HasMember(szIn))
// 	{
// 		Value & v = val[szIn];
// 		if (v.IsInt())
// 		{
// 			nErr = 0;
// 			return v.GetInt();
// 		}
// 	}
// 
// 	return -1;
// }

int CJSON::parseJsonIntValue(char *szIn, Value &val, int &nValue)
{
	int nErr = -1;
	int nTmp = nValue;
	try
	{
		if (val.HasMember(szIn))
		{
			Value & v = val[szIn];
			if (v.IsInt())
			{
				nTmp = v.GetInt();
				nErr = 0;
			}
		}
	}
	catch (...)
	{
		nErr = -1;
	}
	
	if (0 == nErr)
	{
		nValue = nTmp;
	}
	
	return nErr;
}

int CJSON::parseJsonInt64Value(char *szIn, Document &doc, __int64 &nValue)
{
	int nErr = -1;
	__int64 nTmp = nValue;
	try
	{
		if (doc.HasMember(szIn))
		{
			Value & v = doc[szIn];
			if (v.IsInt64())
			{
				nTmp = v.GetInt64();
				nErr = 0;
			}
		}
	}
	catch (...)
	{
		nErr = -1;
	}
	
	if (0 == nErr)
	{
		nValue = nTmp;
	}
	
	return nErr;
}

int CJSON::parseJsonInt64Value(char *szIn, Value &val, __int64 &nValue)
{
	int nErr = -1;
	__int64 nTmp = nValue;
	try
	{
		if (val.HasMember(szIn))
		{
			Value & v = val[szIn];
			if (v.IsInt64())
			{
				nTmp = v.GetInt64();
				nErr = 0;
			}
		}
	}
	catch (...)
	{
		nErr = -1;
	}

	if (0 == nErr)
	{
		nValue = nTmp;
	}

	return nErr;
}


//__int64 CJSON::parseJsonInt64Value(char *szIn, Value &val, int &nErr)
//{
//	nErr = 0;
//	int nRet = 0;
//
//	try
//	{
//		if (val.HasMember(szIn))
//		{
//			Value & v = val[szIn];
//			if (v.IsInt64())
//			{
//				nRet = v.GetInt64();
//				nErr = 0;
//			}
//		}
//	}
//	catch (...)
//	{
//		nErr = -1;
//	}
//
//
//	return nRet;
//}