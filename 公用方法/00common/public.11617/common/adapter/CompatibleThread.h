#ifndef __COMPATIBLETHREAD__H__
#define __COMPATIBLETHREAD__H__

#include "Adapter.h"

//����WINDOWS��linux�Ĵ����߳���
class CCompatibleThread
{
	friend class CAdapter;
public:
    CCompatibleThread();
    ~CCompatibleThread();
private:
    //**********************************
    //Description: ��������WINDOWS��linux���߳�
    // Parameter:  THREAD_RETURN(*start_address)(void *)  �����߳����к�������ڵ�ַ
    //              void*arg Ϊ�����߳�ʹ�õĲ���
    // Returns:    int
    //************************************
    static int CreateThread(THREAD_RETURN(__STDCALL *start_address)(void *), void *arg);
private:
};


#endif

