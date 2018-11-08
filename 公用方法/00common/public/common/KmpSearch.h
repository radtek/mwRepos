// KmpSearch.h: interface for the CKmpSearch class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __KMPSEARCH_H__
#define __KMPSEARCH_H__

class CKmpSearch  
{
public:
	CKmpSearch();
	virtual ~CKmpSearch();
	void NEXT(const char* T, unsigned int T_size, int next[]);
	unsigned int COUNT_KMP(	const char* S, unsigned int S_size, const char* T, unsigned int T_size, int next[]);
	int main(int argc, char *argv[]);
};

#endif 
