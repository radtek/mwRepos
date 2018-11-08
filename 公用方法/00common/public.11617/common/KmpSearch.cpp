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

//KMP算法查找串S中含串P的个数count
void CKmpSearch::NEXT(const char* T, unsigned int T_size, int next[])
{
    //按模式串生成vector,next(T.size())
    next[0]=-1;
    for(int i=1;i<T_size;i++ ){
        int j=next[i-1];
        while(T[i]!=T[j+1]&& j>=0 )
            j=next[j] ; //递推计算
        if(T[i]==T[j+1])next[i]=j+1;
        else next[i]=0; //
    }
}

unsigned int CKmpSearch::COUNT_KMP(    const char* S, unsigned int S_size, const char* T, unsigned int T_size, int next[])
{
    //利用模式串T的next函数求T在主串S中的个数count的KMP算法
    //其中T非空
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
//KMP算法查找串S中含串P的个数count
inline void NEXT(const string& T,vector<int>& next)
{
    //按模式串生成vector,next(T.size())
    next[0]=-1;
    for(int i=1;i<T.size();i++ ){
        int j=next[i-1];
        while(T[i]!=T[j+1]&& j>=0 )
            j=next[j] ; //递推计算
        if(T[i]==T[j+1])next[i]=j+1;
        else next[i]=0; //
    }
}

inline string::size_type COUNT_KMP(    const string& S, const string& T)
{
    //利用模式串T的next函数求T在主串S中的个数count的KMP算法
    //其中T非空，
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
