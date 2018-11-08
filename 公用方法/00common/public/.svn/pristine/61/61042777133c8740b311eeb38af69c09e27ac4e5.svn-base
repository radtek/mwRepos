#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>
#include "adapter/Adapter.h"
using namespace std;

//最大的分支数,(0-9的数字)
#define  MaxBranchNum 10

typedef struct trieNodeTag {
	//用来存放一条记录对应的额外值
	void* value;
	int valuelen;
	//节点重复次数
//	int nTimes;
	bool isFull;
	struct trieNodeTag *next[MaxBranchNum];
	trieNodeTag()
	{
		value = NULL;
		valuelen = 0;
		isFull = false;
		memset(next, 0, sizeof(*next)*MaxBranchNum);
		///*
// 		for (int i = 0; i < MaxBranchNum; ++i)
// 		{
// 			next[i] = NULL;
// 		}
		//*/
	}
} trieNodeT;

class CTrie
{
public:
	CTrie()
	{
		m_root = new trieNodeT;
	}

	~CTrie()
	{
		ClearTrie();
		delete m_root;
	}
	//初始化一棵树
	//trieNodeT* InitTrieNode()
	int InitTrieNode()
	{
		m_root = new trieNodeT;
		if (NULL == m_root)
		{
			return -1;
		}

		m_root->isFull = false;

		memset(m_root->next, 0, sizeof(*(m_root->next))*MaxBranchNum);
		///*
// 		for (int i = 0; i < MaxBranchNum; ++i)
// 		{
// 			m_root->next[i] = NULL;
// 		}
		//*/
		return 0;
	}

	//添加一个记录，并保存对应的额外值
	void TrieAddNode(const char* str, void *paramPtr, int nParamLen)
	{
		int index = 0;
		bool bFlag = false;
		SafeLock lock(&m_csLock);
		trieNodeT *node = m_root;
		for (int i = 0; str[i] != '\0'; ++i)
		{
			index = str[i] - '0';
			if (index < 0 || index > MaxBranchNum - 1)  //错误的数据
			{
				return;
			}
			
			if (NULL == node->next[index])
			{
				node->next[index] = new trieNodeT;
				node = node->next[index];
				bFlag = true;
			}
			else
			{
				node = node->next[index];
			}
		}

		//当该记录str不存在的时候才保存传入的额外值
		if (bFlag)
		{
			node->value = new char[nParamLen];
			if (node->value)
			{
				memcpy(node->value, paramPtr, nParamLen);
				node->valuelen = nParamLen;
			}
		}
		else
		{
			//如果是之前被删除的记录，则允许保存新值
			if (!node->isFull)
			{
				delete[] node->value;
				node->value = new char[nParamLen];
				if (node->value)
				{
					memcpy(node->value, paramPtr, nParamLen);
					node->valuelen = nParamLen;
				}
			}
		}

		node->isFull = true;
	}

	//返回最大匹配位置,0-没找到
	//int FindNodePos(trieNodeT *root, const char *str)
	int FindNodePos(const char *str)
	{
		if (NULL == str || *str == '\0')
			return 0;
		SafeLock lock(&m_csLock);
		char *pTmp = (char*)str;
		trieNodeT *pNode = m_root;

		while (pNode != NULL && *pTmp != '\0' && ((*pTmp - '0') >= 0 && (*pTmp - '0') <= MaxBranchNum - 1))
		{
			if (NULL != pNode->next[*pTmp - '0'])
			{
				pNode = pNode->next[*pTmp - '0'];
			}
			else
			{
				break;
			}
			++pTmp;
		}

		if (pNode != NULL && pNode->isFull)
			return pTmp - str;
		else
			return 0;

	}

	//返回节点,NULL-没找到
	//trieNodeT* FindNodePtr(trieNodeT *root, const char *str)
	bool FindNodePtr(const char *str, void *ptr, int nLen)
	{
		if (NULL == str || *str == '\0' || nLen < 0 || NULL == ptr)
			return false;
		SafeLock lock(&m_csLock);
		char *pTmp = (char*)str;
		trieNodeT *pNode = m_root;
		trieNodeT *pSaveFind = NULL;//保存最接近的结果

		while (pNode != NULL && *pTmp != '\0' && ((*pTmp - '0') >= 0 && (*pTmp - '0') <= MaxBranchNum - 1))
		{
			if (pNode->isFull)
			{			
				pSaveFind = pNode;
			}

			if (NULL != pNode->next[*pTmp - '0'])
			{
				pNode = pNode->next[*pTmp - '0'];
			}
			else
			{
				break;
			}
			++pTmp;
		}

		if (pNode != NULL && pNode->isFull)
		{
			//find last
			memcpy(ptr, pNode->value, nLen);
			return true;
		}
		else if (pSaveFind != NULL)
		{
			memcpy(ptr, pSaveFind->value, nLen);
			return true;
		}
		else
		{
			return false;
		}
	}

	//删除树中的一个记录, 0-删除成功,-1删除失败
	//int DelOneNode(trieNodeT *root, const char *str)
	int DelOneNode(const char *str)
	{
		//找到记录之后，不删节点，只标记这个记录是否存在，并删除保存的额外值
		if (NULL == str || *str == '\0')
			return -1;

		SafeLock lock(&m_csLock);

		char *pTmp = (char*)str;
		trieNodeT *pNode = m_root;

		while (pNode != NULL && *pTmp != '\0' && ((*pTmp - '0') >= 0 && (*pTmp - '0') <= MaxBranchNum - 1))
		{
			if (NULL != pNode->next[*pTmp - '0'])
			{
				pNode = pNode->next[*pTmp - '0'];
			}
			else
			{
				break;
			}
			++pTmp;
		}

		if (pNode != NULL && pNode->isFull)
		{
			pNode->isFull = false;
			if (pNode->value)
			{
				delete[] pNode->value;
				pNode->value = NULL;
				pNode->valuelen = 0;
			}
			return 0;
		}

		return -1;
	}

	//删除整棵树
	void ClearTrie()
	{
		SafeLock lock(&m_csLock);
		Clear(m_root);
		//保留树根
		InitTrieNode();
	}

	trieNodeT* GetRoot()
	{
		return m_root;
	}
private:

	void Clear(trieNodeT *root)
	{
		for (int i = 0; i < MaxBranchNum; ++i)
		{
			if (root->next[i] != NULL)
			{
				if (root->next[i]->value)
				{
					delete root->next[i]->value;
					root->next[i]->value = NULL;
					root->next[i]->valuelen = 0;
				}
				Clear(root->next[i]);
			}
		}
		delete root;
		root = NULL;
	}
	//树的根节点
	trieNodeT *m_root;
	DerivedLock m_csLock;
	
};






