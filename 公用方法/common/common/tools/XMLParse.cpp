//#include "StdAfx.h"
#include "XMLParse.h"
#include <algorithm>
#include <string.h>


void CXML::parseXMLText(const char *pStart, str_strMap &strMap)
{
// 	assert(NULL != pStart);
// 	
// 	char *p = const_cast<char *>(pStart);
// 	m_doc.parse(p, 1);	// 此处必须传1，不能传0
// 	
// 	xml_node<> *rootNode = m_doc.first_node();
// 	
// 	xml_node<> *curNode = rootNode->first_node();
// 	parseSub(curNode, rootNode, rootNode, strMap);

	assert(NULL != pStart);
	xml_document<> doc;
	
	char *p = const_cast<char *>(pStart);
	doc.parse(p, 1);	// 此处必须传1，不能传0
	
	xml_node<> *rootNode = doc.first_node();
	
	xml_node<> *curNode = rootNode->first_node();
	parseSub(curNode, rootNode, rootNode, strMap);
}


void CXML::parseXMLTextEx(const char *pStart, str_strMultiMap &strMap)
{
	assert(NULL != pStart);
	//xml_document<> doc;

	char *p = const_cast<char *>(pStart);
	m_doc.parse(p, 1);	// 此处必须传1，不能传0

	xml_node<> *rootNode = m_doc.first_node();

	xml_node<> *curNode = rootNode->first_node();
	parseSubEx(curNode, rootNode, rootNode, strMap);
}

void CXML::parseSub(xml_node<> *curNode, xml_node<> *parentNode, xml_node<> *rootNode, str_strMap &strMap)
{
	if(NULL == rootNode)
	{
		return;
	}
	std::string  strName("");
	std::string  strValue("");
	int flg = 1;
	
	while(flg)
	{
		if(curNode == NULL)
		{
			if(parentNode != rootNode)
			{
				// 跳到下一个sibling(子节点)
				curNode = parentNode->next_sibling(); 
				// 上一个节点的上一个节点才是真的父节点	
				parentNode = parentNode->parent();		
				continue;
			}
			else
			{
				flg = 0;
				continue;
			}
		}
		else
		{
			strName = curNode->name();
			strValue = curNode->value();
			strMap.insert(make_pair(strName, strValue));
			
			// 当前节点变为父节点
			parentNode = curNode;
			
			// 变更当前节点
			curNode = curNode->first_node();  
		}
	}
}

void CXML::parseSubEx(xml_node<> *curNode, xml_node<> *parentNode, xml_node<> *rootNode, str_strMultiMap &strMultiMap)
{
	if (NULL == rootNode)
	{
		return;
	}
	std::string  strName("");
	std::string  strValue("");
	int flg = 1;

	while (flg)
	{
		if (curNode == NULL)
		{
			if (parentNode != rootNode)
			{
				// 跳到下一个sibling(子节点)
				curNode = parentNode->next_sibling();
				// 上一个节点的上一个节点才是真的父节点	
				parentNode = parentNode->parent();
				continue;
			}
			else
			{
				flg = 0;
				continue;
			}
		}
		else
		{
			strName = curNode->name();
			strValue = curNode->value();
			strMultiMap.insert(make_pair(strName, strValue));

			// 当前节点变为父节点
			parentNode = curNode;

			// 变更当前节点
			curNode = curNode->first_node();
		}
	}
}

void CXML::parseSub2( xml_node<> *curNode, xml_node<> *parentNode, xml_node<> *rootNode, vector<string> &vecMap )
{
	if(NULL == rootNode)
	{
		return;
	}
	std::string  strName("");
	std::string  strValue("");
	int flg = 1;
	
	while(flg)
	{
		if(curNode == NULL)
		{
			if(parentNode != rootNode)
			{
				// 跳到下一个sibling(子节点)
				curNode = parentNode->next_sibling(); 
				// 上一个节点的上一个节点才是真的父节点	
				parentNode = parentNode->parent();		
				continue;
			}
			else
			{
				flg = 0;
				continue;
			}
		}
		else
		{
			strName = curNode->name();
			strValue = curNode->value();
			//strMap.insert(make_pair(strName, strValue));
			vecMap.push_back(strValue);
			// 当前节点变为父节点
			parentNode = curNode;
			
			// 变更当前节点
			curNode = curNode->first_node();  
		}
	}	
}

void CXML::parseXMLText2( const char *pStart, vector<string> &vecStr )
{
	assert(NULL != pStart);
	xml_document<> doc;
	
	char *p = const_cast<char *>(pStart);
	doc.parse(p, 1);	// 此处必须传1，不能传0
	
	xml_node<> *rootNode = doc.first_node();
	
	xml_node<> *curNode = rootNode->first_node();
	parseSub2(curNode, rootNode, rootNode, vecStr);	
}

int CXML::parseXMLValue(char *szIn,char *szOut, size_t nMaxSize, xml_document<> &doc)
{
	int nLen = 0;
	xml_node<> *node = doc.first_node(szIn);
	if (NULL != node)
	{
		nLen = node->value_size();
		memcpy(szOut, node->value(), std::min((size_t)nLen, nMaxSize));
	}

	return nLen;
}

int CXML::parseXMLValueEx(char *szIn, char *szOut, size_t nMaxSize, xml_node<> *node)
{
	int nLen = 0;
	xml_node<> *node2 = node->first_node(szIn);
	if (NULL != node2)
	{
		nLen = node2->value_size();
		memcpy(szOut, node2->value(), std::min((size_t)nLen, nMaxSize));
	}

	return nLen;
}

int CXML::getXMLValue(char *szIn, char *szOut, size_t nMaxSize, xml_node<> *node)
{
	int nLen = 0;
	if (NULL != node)
	{
		nLen = node->value_size();
		memcpy(szOut, node->value(), std::min((size_t)nLen, nMaxSize));
	}

	return nLen;
}
