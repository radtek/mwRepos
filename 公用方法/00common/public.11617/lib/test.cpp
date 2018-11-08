#include "stdio.h" 
#include "stdlib.h" 
#include "dlfcn.h"    

int main(void) 
{          
void *handle;          
int (*fcn)(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);          
const char *errmsg;                    
/* open the library */         
handle = dlopen("./eatool.so", RTLD_NOW);          
if(handle == NULL)          
{                    
fprintf(stderr, "Failed to load libsthc.so: %s\n", dlerror());                    
return 1;          
}          
dlerror();             
*(void **)(&fcn) = dlsym(handle, "EncryptMsg");     
//fcn = dlsym(handle, "add");                        / t ok in c++          
if((errmsg = dlerror()) != NULL)          
{                    
printf("%s\n", errmsg);                    
return 1;          
}          
char szBuf[1024] = {0};
printf("%d\n", fcn("123456", "12345678", "123456", 6, szBuf));
printf(szBuf);                    
dlclose(handle);          
return 0; 
} 
