// KmpSearch.cpp: implementation of the CKmpSearch class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "KmpSearch.h"
#include "string.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKmpSearch::CKmpSearch()
{

}

CKmpSearch::~CKmpSearch()
{

}

//KMP�㷨���Ҵ�S�к���P�ĸ���count
void CKmpSearch::NEXT(const char* T, unsigned int T_size, int next[])
{
    //��ģʽ������vector,next(T.size())
    next[0]=-1;
    for(int i=1;i<T_size;i++ ){
        int j=next[i-1];
        while(T[i]!=T[j+1]&& j>=0 )
            j=next[j] ; //���Ƽ���
        if(T[i]==T[j+1])next[i]=j+1;
        else next[i]=0; //
    }
}

unsigned int CKmpSearch::COUNT_KMP(    const char* S, unsigned int S_size, const char* T, unsigned int T_size, int next[])
{
    //����ģʽ��T��next������T������S�еĸ���count��KMP�㷨
    //����T�ǿ�
    unsigned int index=0,count=0,pos=0,iter=0;
    for(index=0;index<S_size;++index){
        pos=0;
        iter=index;
        while(pos<T_size && iter<S_size){
            if(S[iter]==T[pos]){
                ++iter;++pos;
            }
            else{
                if(pos==0)++iter;
                else pos=next[pos-1]+1;
            }
        }//while end
        if(pos==T_size&&(iter-index)==T_size){
            ++count;
            break;
        }
    } //for end
    return count;
}

int CKmpSearch::main(int argc, char *argv[])
{
    char S[]="abaabcacabaabcacabaabcacabaabcacabaabcac";
    char T[]="ab";
    int next[100] = {0};
    unsigned int count=COUNT_KMP(S,strlen(S),T,strlen(T),next);
    //cout<<count<<endl;
    //system("PAUSE");
    return 0;
}
//////////////////////////////////////////////////////////////////////////
/*
//KMP�㷨���Ҵ�S�к���P�ĸ���count
inline void NEXT(const string& T,vector<int>& next)
{
    //��ģʽ������vector,next(T.size())
    next[0]=-1;
    for(int i=1;i<T.size();i++ ){
        int j=next[i-1];
        while(T[i]!=T[j+1]&& j>=0 )
            j=next[j] ; //���Ƽ���
        if(T[i]==T[j+1])next[i]=j+1;
        else next[i]=0; //
    }
}

inline string::size_type COUNT_KMP(    const string& S, const string& T)
{
    //����ģʽ��T��next������T������S�еĸ���count��KMP�㷨
    //����T�ǿգ�
    vector<int> next(T.size());
    NEXT(T,next);
    string::size_type index,count=0;
    for(index=0;index<S.size();++index){
        int pos=0;
        string::size_type iter=index;
        while(pos<T.size() && iter<S.size()){
            if(S[iter]==T[pos]){
                ++iter;++pos;
            }
            else{
                if(pos==0)++iter;
                else pos=next[pos-1]+1;
            }
        }//while end
        if(pos==T.size()&&(iter-index)==T.size())++count;
    } //for end
    return count;
}

int main(int argc, char *argv[])
{
    string S="abaabcacabaabcacabaabcacabaabcacabaabcac";
    string T="ab";
    string::size_type count=COUNT_KMP(S,T);
    cout<<count<<endl;
    system("PAUSE");
    return 0;
}
*/
