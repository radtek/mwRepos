#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>
#include "adapter/Adapter.h"
using namespace std;

//���ķ�֧��,(0-9������)
#define  MaxBranchNum 10

typedef struct trieNodeTag {
	//�������һ����¼��Ӧ�Ķ���ֵ
	void* value;
	int valuelen;
	//�ڵ��ظ�����
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
	//��ʼ��һ����
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

	//���һ����¼���������Ӧ�Ķ���ֵ
	void TrieAddNode(const char* str, void *paramPtr, int nParamLen)
	{
		int index = 0;
		bool bFlag = false;
		SafeLock lock(&m_csLock);
		trieNodeT *node = m_root;
		for (int i = 0; str[i] != '\0'; ++i)
		{
			index = str[i] - '0';
			if (index < 0 || index > MaxBranchNum - 1)  //���������
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

		//���ü�¼str�����ڵ�ʱ��ű��洫��Ķ���ֵ
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
			//�����֮ǰ��ɾ���ļ�¼������������ֵ
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

	//�������ƥ��λ��,0-û�ҵ�
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

	//���ؽڵ�,NULL-û�ҵ�
	//trieNodeT* FindNodePtr(trieNodeT *root, const char *str)
	bool FindNodePtr(const char *str, void *ptr, int nLen)
	{
		if (NULL == str || *str == '\0' || nLen < 0 || NULL == ptr)
			return false;
		SafeLock lock(&m_csLock);
		char *pTmp = (char*)str;
		trieNodeT *pNode = m_root;
		trieNodeT *pSaveFind = NULL;//������ӽ��Ľ��

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

	//ɾ�����е�һ����¼, 0-ɾ���ɹ�,-1ɾ��ʧ��
	//int DelOneNode(trieNodeT *root, const char *str)
	int DelOneNode(const char *str)
	{
		//�ҵ���¼֮�󣬲�ɾ�ڵ㣬ֻ��������¼�Ƿ���ڣ���ɾ������Ķ���ֵ
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

	//ɾ��������
	void ClearTrie()
	{
		SafeLock lock(&m_csLock);
		Clear(m_root);
		//��������
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
	//���ĸ��ڵ�
	trieNodeT *m_root;
	DerivedLock m_csLock;
	
};






