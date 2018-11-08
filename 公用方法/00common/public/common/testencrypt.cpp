 #include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "Encrypt.h"
int main(void)
{
    	CEncrypt cEct;
	aes_context ctx;
        unsigned char bufin[16];
        unsigned char bufout[16] = {0};
	char sztemp[100000] = {0};
	;
	strcat(sztemp,"abcag9i304uj444u94iurjrejouspiuig0i0i0ig0i0ig0fig0gi0gi0ig0gi0gif0gi0fig0gi0fig0jlj");
	strcat(sztemp,"abcag9i304uj444u94iurjrejouspiuig0i0i0ig0i0ig0fig0gi0gi0ig0gi0gif0gi0fig0gi0fig0jlj");
	strcat(sztemp,"abcag9i304uj444u94iurjrejouspiuig0i0i0ig0i0ig0fig0gi0gi0ig0gi0gif0gi0fig0gi0fig0jlj");
	
	unsigned char svrbuf[100000] = {0};
        unsigned char dstbuf[100000] = {0};
	unsigned char key[17] = "kok12345678okok";
        memcpy(svrbuf,sztemp,100000);
        svrbuf[4] = 233;
        svrbuf[6] = 13;
	svrbuf[9] = 128;
        cEct.Encrypt(svrbuf,96000,dstbuf,key);
        memset(svrbuf,0,100000);
	cEct.Decrypt(dstbuf,96000,svrbuf,key);
        printf((char*)svrbuf);
        return 0;
}
