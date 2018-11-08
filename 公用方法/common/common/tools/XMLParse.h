#ifndef _MW_XML_PARSE_H
#define _MW_XML_PARSE_H

#include "rapidxml/rapidxml.h"
#include <map>
#include <vector>
#include <string>
using namespace std;
using namespace rapidxml;

typedef std::map<std::string, std::string>  str_strMap;

typedef std::multimap<std::string, std::string>  str_strMultiMap;

class CXML
{
public:


	static CXML& getInstance()
	{
		static CXML xml;
		return xml;
	}

	void parseXMLText(const char *pStart, str_strMap &strMap);

	void parseXMLTextEx(const char *pStart, str_strMultiMap &strMap);

	void parseXMLText2(const char *pStart, vector<string> &vecStr);

	static int parseXMLValue(char *szIn, char *szOut,size_t nMaxSize, xml_document<> &doc);

	static int parseXMLValueEx(char *szIn, char *szOut, size_t nMaxSize, xml_node<> *doc);

	static int getXMLValue(char *szIn, char *szOut, size_t nMaxSize, xml_node<> *node);
	

private:
	// 只被ParseXMLText_EX调用
	void parseSub(xml_node<> *curNode, xml_node<> *parentNode, xml_node<> *rootNode, str_strMap &strMap);

	void parseSub2(xml_node<> *curNode, xml_node<> *parentNode, xml_node<> *rootNode, vector<string> &vecStr);
	
	void parseSubEx(xml_node<> *curNode, xml_node<> *parentNode, xml_node<> *rootNode, str_strMultiMap &strMap);

	xml_document<>  m_doc;

};

#endif
