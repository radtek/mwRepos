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
    //在链头插入一个节点
    void push(list_node<T> *newnode)
    {
        while(1)
        {
            list_node<T> *lhead = _head;//本地保存
            newnode->_next = lhead;
            //成功就退出，失败就重做
            if(InterlockedCompareExchange((void**)&_head,newnode,lhead) == lhead)
			{
				InterlockedIncrement((long*)&_size);
                break;
			}
        }
    }
    
    //从链头弹出一个节点
    list_node<T>* pop()
    {
        while(1)
        {
            list_node<T> *lhead = _head;//本地保存
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