// Encrypt.h: interface for the CEncrypt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined ENCRYPT_H
#define ENCRYPT_H

//#ifndef _AES_H
//#define _AES_H

#ifndef uint8
#define uint8  unsigned char
#endif

#ifndef uint32
#define uint32 unsigned long int
#endif

typedef struct
{
    uint32 erk[64];     /* encryption round keys */
    uint32 drk[64];     /* decryption round keys */
    int nr;             /* number of rounds */
} aes_context;

class CEncrypt  
{
public:
	CEncrypt();
	virtual ~CEncrypt();

	//ucInput���ٵĻ����С������16����������������16�����������㷨���ں���ǿ�Ʋ�0
	//ucOutPut�Ĵ�СҪ��16���������������ucInput�Ļ����Сһ��
	//ucKey�ĳ��ȱ���Ϊ16�ֽ�
	//����>0:�ɹ�,��ʾ���ĵĳ���(16��������)������-1:����
	int Encrypt(unsigned char *ucInput, unsigned int nInputLen, unsigned char *ucKey, unsigned char *ucOutput);
	
	//ucInput���ٵĻ����С������16��������
	//nInputLen������16��������
	//ucOutput���շ��س��ȿ��ܲ���ԭ���ȣ���Ϊ�����в�0
	//ucKey�ĳ��ȱ���Ϊ16�ֽ�
	//����>0:�ɹ�,��ʾ���ĵĳ���(16��������)������-1:����
	int Decrypt(unsigned char *ucInput, unsigned int nInputLen, unsigned char *ucKey, unsigned char *ucOutput);
	
private:
	void aes_gen_tables( void );
	int  aes_set_key( aes_context *ctx, uint8 *key, int nbits );
	void aes_encrypt( aes_context *ctx, uint8 input[16], uint8 output[16] );
    void aes_decrypt( aes_context *ctx, uint8 input[16], uint8 output[16] );
private:
	int do_init;
	int KT_init;
	uint32 KT0[256];
	uint32 KT1[256];
	uint32 KT2[256];
    uint32 KT3[256];
	/* forward S-box & tables */
	
	uint32 FSb[256];
	uint32 FT0[256]; 
	uint32 FT1[256]; 
	uint32 FT2[256]; 
	uint32 FT3[256]; 
	
	/* reverse S-box & tables */
	
	uint32 RSb[256];
	uint32 RT0[256];
	uint32 RT1[256];
	uint32 RT2[256];
	uint32 RT3[256];
	
    /* round constants */
	uint32 RCON[10];
};

#endif // !defined(AFX_ENCRYPT_H__5510E115_7462_490E_B1DA_FF812A8E08FA__INCLUDED_)
