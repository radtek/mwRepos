
#ifndef _MW_JSON_PARSE_H
#define _MW_JSON_PARSE_H

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include "adapter/Adapter.h"
#include "Global.h"
using namespace std;
using namespace rapidjson;

typedef std::map<std::string, std::string>  str_strMap;

typedef std::multimap<std::string, std::string>  str_strMultiMap;

// 基础变量的校验  
#define json_check_is_bool(value, strKey) (value.HasMember(strKey) && value[strKey].IsBool())  
#define json_check_is_string(value, strKey) (value.HasMember(strKey) && value[strKey].IsString())  
#define json_check_is_int32(value, strKey) (value.HasMember(strKey) && value[strKey].IsInt())  
#define json_check_is_uint32(value, strKey) (value.HasMember(strKey) && value[strKey].IsUint())  
#define json_check_is_int64(value, strKey) (value.HasMember(strKey) && value[strKey].IsInt64())  
#define json_check_is_uint64(value, strKey) (value.HasMember(strKey) && value[strKey].IsUint64())  
#define json_check_is_float(value, strKey) (value.HasMember(strKey) && value[strKey].IsFloat())  
#define json_check_is_double(value, strKey) (value.HasMember(strKey) && value[strKey].IsDouble())  

#define json_check_is_number(value, strKey) (value.HasMember(strKey) && value[strKey].IsNumber())  
#define json_check_is_array(value, strKey) (value.HasMember(strKey) && value[strKey].IsArray())  

// 得到对应类型的数据，如果数据不存在则得到一个默认值  
#define json_check_bool(value, strKey) (json_check_is_bool(value, strKey) && value[strKey].GetBool())  
#define json_check_string(value, strKey) (json_check_is_string(value, strKey) ? value[strKey].GetString() : "")  
#define json_check_int32(value, strKey) (json_check_is_int32(value, strKey) ? value[strKey].GetInt() : 0)  
#define json_check_uint32(value, strKey) (json_check_is_uint32(value, strKey) ? value[strKey].GetUint() : 0)  
#define json_check_int64(value, strKey) (json_check_is_int64(value, strKey) ? ((value)[strKey]).GetInt64() : 0)  
#define json_check_uint64(value, strKey) (json_check_is_uint64(value, strKey) ? ((value)[strKey]).GetUint64() : 0)  
#define json_check_float(value, strKey) (json_check_is_float(value, strKey) ? ((value)[strKey]).GetFloat() : 0)  
#define json_check_double(value, strKey) (json_check_is_double(value, strKey) ? ((value)[strKey]).GetDouble() : 0)  

class CJSON
{
public:


	static CJSON& getInstance()
	{
		static CJSON json;
		return json;
	}

	void parseJsonText(const char *pStart, str_strMap &strMap);

	void parseJsonArryText(const char *pStart, str_strMultiMap &strMap);

	static int parseJsonStrValue(char *szIn, char *szOut, size_t nMaxSize, Document &doc);

	static int parseJsonStrValue(char *szIn, char *szOut, size_t nMaxSize, Value &val);

	static int parseJsonIntValue(char *szIn, Document &doc, int &nErr);

	static int parseJsonIntValue(char *szIn, Value &val, int &nValue);

	static int parseJsonInt64Value(char *szIn, Document &doc, __int64 &nValue);

	static int parseJsonInt64Value(char *szIn, Value &val, __int64 &nValue);

private:



	Document  m_doc;

};

#endif
