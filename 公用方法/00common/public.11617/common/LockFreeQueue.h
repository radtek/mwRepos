#ifndef _LOCKFREE_LIST_H
#define _LOCKFREE_LIST_H

template <class T>
struct list_node
{
    T val;
    list_node<T> *_next;
};

template <class T>
class LockFreeList
{
public:
    LockFreeList():_head(0),_size(0){}
    //����ͷ����һ���ڵ�
    void push(list_node<T> *newnode)
    {
        while(1)
        {
            list_node<T> *lhead = _head;//���ر���
            newnode->_next = lhead;
            //�ɹ����˳���ʧ�ܾ�����
            if(InterlockedCompareExchange((void**)&_head,newnode,lhead) == lhead)
			{
				InterlockedIncrement((long*)&_size);
                break;
			}
        }
    }
    
    //����ͷ����һ���ڵ�
    list_node<T>* pop()
    {
        while(1)
        {
            list_node<T> *lhead = _head;//���ر���
            if(!lhead) return NULL;
            list_node<T> *ret = _head;
            if(InterlockedCompareExchange((void**)&_head,_head->_next,lhead) == lhead)
            {
				InterlockedDecrement((long*)&_size);
                ret->_next = NULL;
                return ret;
            }
        }
    }

	int size()
	{
		return _size;
	}
private:
    list_node<T> *_head;
	int _size;
};
#endif