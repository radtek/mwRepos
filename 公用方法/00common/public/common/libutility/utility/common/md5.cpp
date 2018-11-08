//md5.cpp: implementation of the MD5Checksum class.
#include "utility/common/mwcorepch.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utility/common/md5.h"

unsigned char PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*****************************************************************************************
FUNCTION:       GetMD5
DETAILS:        static, public
DESCRIPTION:    Gets the MD5 checksum for data in a BYTE array
RETURNS:        CString : the hexadecimal MD5 checksum for the specified data
ARGUMENTS:      BYTE* pBuf  :   pointer to the BYTE array
                unsigned int nLength :  number of BYTEs of data to be checksumed
NOTES:          Provides an interface to the CMD5Checksum class. Any data that can
                be cast to a BYTE array of known length can be checksummed by this
                function. Typically, CString and char arrays will be checksumed,
                although this function can be used to check the integrity of any BYTE array.
                A buFFer of zero length can be checksummed; all buFFers of zero length
                will return the same checksum.
*****************************************************************************************/
void GetMD5(unsigned char* pBuf, unsigned int nLength, unsigned char* pMD5)
{
    unsigned char m_lpszBuFFer[64];     //input buFFer
    unsigned int m_nCount[2];          //number of bits, modulo 2^64 (lsb first)
    unsigned int m_lMD5[4];            //MD5 checksum

    InitMd5Var(m_lpszBuFFer, m_nCount, m_lMD5);

    //calculate and return the checksum
    //CMD5Checksum MD5Checksum;
    //MD5Checksum.Update( pBuf, nLength );
    //MD5Checksum.Final(pMD5);
    Update(pBuf, nLength, m_nCount, m_lpszBuFFer, m_lMD5);
    Final(pMD5, m_nCount, m_lMD5, m_lpszBuFFer);
    return;
}

void Md5ToHexStr(const unsigned char* pMD5, unsigned int nLength, char* szHexStr)
{
	int i = 0;
	int nLen = 0;
	for (i = 0; i < nLength; ++i)
	{
		nLen += sprintf(szHexStr+nLen, "%02x", pMD5[i]);
	}	
}

/*****************************************************************************************
FUNCTION:       RotateLeft
DETAILS:        private
DESCRIPTION:    Rotates the bits in a 32 bit DWORD left by a specified amount
RETURNS:        The rotated DWORD
ARGUMENTS:      DWORD x : the value to be rotated
                int n   : the number of bits to rotate by
*****************************************************************************************/
unsigned int RotateLeft(unsigned int x, int n)
{
    //check that DWORD is 4 bytes long - true in Visual C++ 6 and 32 bit Windows

    //rotate and return x
    return (x << n) | (x >> (32-n));
}


/*****************************************************************************************
FUNCTION:       FF
DETAILS:        protected
DESCRIPTION:    Implementation of basic MD5 Transformation algorithm
RETURNS:        none
ARGUMENTS:      DWORD &A, B, C, D : Current (partial) checksum
                DWORD X           : Input data
                DWORD S           : MD5_SXX Transformation constant
                DWORD T           : MD5_TXX Transformation constant
NOTES:          None
*****************************************************************************************/
void FF(unsigned int& A, unsigned int B, unsigned int C, unsigned int D, unsigned int X, unsigned int S, unsigned int T)
{
	unsigned int F = (B & C) | (~B & D);
    A += F + X + T;
    A = RotateLeft(A, S);
    A += B;
}


/*****************************************************************************************
FUNCTION:       GG
DETAILS:        protected
DESCRIPTION:    Implementation of basic MD5 Transformation algorithm
RETURNS:        none
ARGUMENTS:      DWORD &A, B, C, D : Current (partial) checksum
                DWORD X           : Input data
                DWORD S           : MD5_SXX Transformation constant
                DWORD T           : MD5_TXX Transformation constant
NOTES:          None
*****************************************************************************************/
void GG(unsigned int& A, unsigned int B, unsigned int C, unsigned int D, unsigned int X, unsigned int S, unsigned int T)
{
	unsigned int G = (B & D) | (C & ~D);
    A += G + X + T;
    A = RotateLeft(A, S);
    A += B;
}


/*****************************************************************************************
FUNCTION:       HH
DETAILS:        protected
DESCRIPTION:    Implementation of basic MD5 Transformation algorithm
RETURNS:        none
ARGUMENTS:      DWORD &A, B, C, D : Current (partial) checksum
                DWORD X           : Input data
                DWORD S           : MD5_SXX Transformation constant
                DWORD T           : MD5_TXX Transformation constant
NOTES:          None
*****************************************************************************************/
void HH(unsigned int& A, unsigned int B, unsigned int C, unsigned int D, unsigned int X, unsigned int S, unsigned int T)
{
	unsigned int H = (B ^ C ^ D);
    A += H + X + T;
    A = RotateLeft(A, S);
    A += B;
}


/*****************************************************************************************
FUNCTION:       II
DETAILS:        protected
DESCRIPTION:    Implementation of basic MD5 Transformation algorithm
RETURNS:        none
ARGUMENTS:      DWORD &A, B, C, D : Current (partial) checksum
                DWORD X           : Input data
                DWORD S           : MD5_SXX Transformation constant
                DWORD T           : MD5_TXX Transformation constant
NOTES:          None
*****************************************************************************************/
void II(unsigned int& A, unsigned int B, unsigned int C, unsigned int D, unsigned int X, unsigned int S, unsigned int T)
{
	unsigned int I = (C ^ (B | ~D));
    A += I + X + T;
    A = RotateLeft(A, S);
    A += B;
}


/*****************************************************************************************
FUNCTION:       ByteToDWord
DETAILS:        private
DESCRIPTION:    Transfers the data in an 8 bit array to a 32 bit array
RETURNS:        void
ARGUMENTS:      DWORD* Output : the 32 bit (unsigned long) destination array
                BYTE* Input   : the 8 bit (unsigned char) source array
                unsigned int nLength  : the number of 8 bit data items in the source array
NOTES:          Four BYTES from the input array are transferred to each DWORD entry
                of the output array. The first BYTE is transferred to the bits (0-7)
                of the output DWORD, the second BYTE to bits 8-15 etc.
                The algorithm assumes that the input array is a multiple of 4 bytes long
                so that there is a perfect fit into the array of 32 bit words.
*****************************************************************************************/
void ByteToDWord(unsigned int* Output, unsigned char* Input, unsigned int nLength)
{
    //entry invariants

    //initialisations
    unsigned int i=0;   //index to Output array
    unsigned int j=0;   //index to Input array

    //transfer the data by shifting and copying
    for ( ; j < nLength; i++, j += 4)
    {
		Output[i] = (unsigned int)Input[j] |
					(unsigned int)Input[j + 1] << 8 |
					(unsigned int)Input[j + 2] << 16 |
					(unsigned int)Input[j + 3] << 24;
    }
}

/*****************************************************************************************
FUNCTION:       Transform
DETAILS:        protected
DESCRIPTION:    MD5 basic Transformation algorithm;  Transforms 'm_lMD5'
RETURNS:        void
ARGUMENTS:      BYTE Block[64]
NOTES:          An MD5 checksum is calculated by four rounds of 'Transformation'.
                The MD5 checksum currently held in m_lMD5 is merged by the
                Transformation process with data passed in 'Block'.
*****************************************************************************************/
void Transform(unsigned char Block[64], unsigned int* m_lMD5)
{
    //initialise local data with current checksum
	unsigned int a = m_lMD5[0];
	unsigned int b = m_lMD5[1];
	unsigned int c = m_lMD5[2];
	unsigned int d = m_lMD5[3];

    //copy BYTES from input 'Block' to an array of ULONGS 'X'
	unsigned int X[16];
    ByteToDWord(X, Block, 64);

    //Perform Round 1 of the Transformation
    FF(a, b, c, d, X[0], MD5_S11, MD5_T01);
    FF(d, a, b, c, X[1], MD5_S12, MD5_T02);
    FF(c, d, a, b, X[2], MD5_S13, MD5_T03);
    FF(b, c, d, a, X[3], MD5_S14, MD5_T04);
    FF(a, b, c, d, X[4], MD5_S11, MD5_T05);
    FF(d, a, b, c, X[5], MD5_S12, MD5_T06);
    FF(c, d, a, b, X[6], MD5_S13, MD5_T07);
    FF(b, c, d, a, X[7], MD5_S14, MD5_T08);
    FF(a, b, c, d, X[8], MD5_S11, MD5_T09);
    FF(d, a, b, c, X[9], MD5_S12, MD5_T10);
    FF(c, d, a, b, X[10], MD5_S13, MD5_T11);
    FF(b, c, d, a, X[11], MD5_S14, MD5_T12);
    FF(a, b, c, d, X[12], MD5_S11, MD5_T13);
    FF(d, a, b, c, X[13], MD5_S12, MD5_T14);
    FF(c, d, a, b, X[14], MD5_S13, MD5_T15);
    FF(b, c, d, a, X[15], MD5_S14, MD5_T16);

    //Perform Round 2 of the Transformation
    GG(a, b, c, d, X[1], MD5_S21, MD5_T17);
    GG(d, a, b, c, X[6], MD5_S22, MD5_T18);
    GG(c, d, a, b, X[11], MD5_S23, MD5_T19);
    GG(b, c, d, a, X[0], MD5_S24, MD5_T20);
    GG(a, b, c, d, X[5], MD5_S21, MD5_T21);
    GG(d, a, b, c, X[10], MD5_S22, MD5_T22);
    GG(c, d, a, b, X[15], MD5_S23, MD5_T23);
    GG(b, c, d, a, X[4], MD5_S24, MD5_T24);
    GG(a, b, c, d, X[9], MD5_S21, MD5_T25);
    GG(d, a, b, c, X[14], MD5_S22, MD5_T26);
    GG(c, d, a, b, X[3], MD5_S23, MD5_T27);
    GG(b, c, d, a, X[8], MD5_S24, MD5_T28);
    GG(a, b, c, d, X[13], MD5_S21, MD5_T29);
    GG(d, a, b, c, X[2], MD5_S22, MD5_T30);
    GG(c, d, a, b, X[7], MD5_S23, MD5_T31);
    GG(b, c, d, a, X[12], MD5_S24, MD5_T32);

    //Perform Round 3 of the Transformation
    HH(a, b, c, d, X[5], MD5_S31, MD5_T33);
    HH(d, a, b, c, X[8], MD5_S32, MD5_T34);
    HH(c, d, a, b, X[11], MD5_S33, MD5_T35);
    HH(b, c, d, a, X[14], MD5_S34, MD5_T36);
    HH(a, b, c, d, X[1], MD5_S31, MD5_T37);
    HH(d, a, b, c, X[4], MD5_S32, MD5_T38);
    HH(c, d, a, b, X[7], MD5_S33, MD5_T39);
    HH(b, c, d, a, X[10], MD5_S34, MD5_T40);
    HH(a, b, c, d, X[13], MD5_S31, MD5_T41);
    HH(d, a, b, c, X[0], MD5_S32, MD5_T42);
    HH(c, d, a, b, X[3], MD5_S33, MD5_T43);
    HH(b, c, d, a, X[6], MD5_S34, MD5_T44);
    HH(a, b, c, d, X[9], MD5_S31, MD5_T45);
    HH(d, a, b, c, X[12], MD5_S32, MD5_T46);
    HH(c, d, a, b, X[15], MD5_S33, MD5_T47);
    HH(b, c, d, a, X[2], MD5_S34, MD5_T48);

    //Perform Round 4 of the Transformation
    II(a, b, c, d, X[0], MD5_S41, MD5_T49);
    II(d, a, b, c, X[7], MD5_S42, MD5_T50);
    II(c, d, a, b, X[14], MD5_S43, MD5_T51);
    II(b, c, d, a, X[5], MD5_S44, MD5_T52);
    II(a, b, c, d, X[12], MD5_S41, MD5_T53);
    II(d, a, b, c, X[3], MD5_S42, MD5_T54);
    II(c, d, a, b, X[10], MD5_S43, MD5_T55);
    II(b, c, d, a, X[1], MD5_S44, MD5_T56);
    II(a, b, c, d, X[8], MD5_S41, MD5_T57);
    II(d, a, b, c, X[15], MD5_S42, MD5_T58);
    II(c, d, a, b, X[6], MD5_S43, MD5_T59);
    II(b, c, d, a, X[13], MD5_S44, MD5_T60);
    II(a, b, c, d, X[4], MD5_S41, MD5_T61);
    II(d, a, b, c, X[11], MD5_S42, MD5_T62);
    II(c, d, a, b, X[2], MD5_S43, MD5_T63);
    II(b, c, d, a, X[9], MD5_S44, MD5_T64);

    //add the Transformed values to the current checksum
    m_lMD5[0] += a;
    m_lMD5[1] += b;
    m_lMD5[2] += c;
    m_lMD5[3] += d;
}


/*****************************************************************************************
CONSTRUCTOR:    CMD5Checksum
DESCRIPTION:    Initialises member data
ARGUMENTS:      None
NOTES:          None
*****************************************************************************************/
void InitMd5Var(unsigned char* m_lpszBuFFer, unsigned int* m_nCount, unsigned int* m_lMD5)
{
    // zero members
    memset( m_lpszBuFFer, 0, 64 );
    m_nCount[0] = m_nCount[1] = 0;

    // Load magic state initialization constants
    m_lMD5[0] = MD5_INIT_STATE_0;
    m_lMD5[1] = MD5_INIT_STATE_1;
    m_lMD5[2] = MD5_INIT_STATE_2;
    m_lMD5[3] = MD5_INIT_STATE_3;
}

/*****************************************************************************************
FUNCTION:       DWordToByte
DETAILS:        private
DESCRIPTION:    Transfers the data in an 32 bit array to a 8 bit array
RETURNS:        void
ARGUMENTS:      BYTE* Output  : the 8 bit destination array
                DWORD* Input  : the 32 bit source array
                unsigned int nLength  : the number of 8 bit data items in the source array
NOTES:          One DWORD from the input array is transferred into four BYTES
                in the output array. The first (0-7) bits of the first DWORD are
                transferred to the first output BYTE, bits bits 8-15 are transferred from
                the second BYTE etc.

                The algorithm assumes that the output array is a multiple of 4 bytes long
                so that there is a perfect fit of 8 bit BYTES into the 32 bit DWORDs.
*****************************************************************************************/
void DWordToByte(unsigned char* Output, unsigned int* Input, unsigned int nLength)
{
    //entry invariants
    //transfer the data by shifting and copying
	unsigned int i = 0;
	unsigned int j = 0;
    for ( ; j < nLength; i++, j += 4)
    {
		Output[j] = (unsigned char)(Input[i] & 0xFF);
		Output[j + 1] = (unsigned char)((Input[i] >> 8) & 0xFF);
		Output[j + 2] = (unsigned char)((Input[i] >> 16) & 0xFF);
		Output[j + 3] = (unsigned char)((Input[i] >> 24) & 0xFF);
    }
}


/*****************************************************************************************
FUNCTION:       Final
DETAILS:        protected
DESCRIPTION:    Implementation of main MD5 checksum algorithm; ends the checksum calculation.
RETURNS:        CString : the Final hexadecimal MD5 checksum result
ARGUMENTS:      None
NOTES:          Performs the Final MD5 checksum calculation ('Update' does most of the work,
                this function just finishes the calculation.)
*****************************************************************************************/
void Final(unsigned char *strMD5, unsigned int* m_nCount, unsigned int* m_lMD5, unsigned char *m_lpszBuFFer)
{
    //Save number of bits
	unsigned char Bits[8];

    if(!strMD5)
        return;
    DWordToByte(Bits, m_nCount, 8);

    //Pad out to 56 mod 64.
    unsigned int nIndex = (unsigned int)((m_nCount[0] >> 3) & 0x3f);
    unsigned int nPadLen = (nIndex < 56) ? (56 - nIndex) : (120 - nIndex);
    Update(PADDING, nPadLen, m_nCount, m_lpszBuFFer, m_lMD5);

    //Append length (before PADDING)
    Update(Bits, 8, m_nCount, m_lpszBuFFer, m_lMD5);

    //Store Final state in 'lpszMD5'
    const int nMD5Size = 16;
    DWordToByte(strMD5, m_lMD5, nMD5Size);
/*
    unsigned char lpszMD5[ nMD5Size ];
    DWordToByte( lpszMD5, m_lMD5, nMD5Size );
    //Convert the hexadecimal checksum to a CString
    for ( int i=0; i < nMD5Size; i++)
    {
        char Str[1024] = {0x00};
        if (lpszMD5[i] == 0) {
            strcpy(Str, "00");
        }
        else if (lpszMD5[i] <= 15)  {
            sprintf(Str, "0%x",lpszMD5[i]);
        }
        else {
            sprintf(Str, "%x",lpszMD5[i]);
        }

        strcat((char *)strMD5, Str);
    }
*/
}


/*****************************************************************************************
FUNCTION:       Update
DETAILS:        protected
DESCRIPTION:    Implementation of main MD5 checksum algorithm
RETURNS:        void
ARGUMENTS:      BYTE* Input    : input block
                unsigned int nInputLen : length of input block
NOTES:          Computes the partial MD5 checksum for 'nInputLen' bytes of data in 'Input'
*****************************************************************************************/
void Update(unsigned char* Input, unsigned int nInputLen, unsigned int* m_nCount, unsigned char* m_lpszBuFFer, unsigned int* m_lMD5)
{
    //Compute number of bytes mod 64
    unsigned int nIndex = (unsigned int)((m_nCount[0] >> 3) & 0x3F);

    //Update number of bits
    if ( ( m_nCount[0] += nInputLen << 3 )  <  ( nInputLen << 3) )
    {
        m_nCount[1]++;
    }
    m_nCount[1] += (nInputLen >> 29);

    //Transform as many times as possible.
    unsigned int i=0;
    unsigned int nPartLen = 64 - nIndex;
    if (nInputLen >= nPartLen)
    {
        memcpy( &m_lpszBuFFer[nIndex], Input, nPartLen );
        Transform(m_lpszBuFFer, m_lMD5);
        for (i = nPartLen; i + 63 < nInputLen; i += 64)
        {
            Transform(&Input[i], m_lMD5);
        }
        nIndex = 0;
    }
    else
    {
        i = 0;
    }

    // BuFFer remaining input
    memcpy( &m_lpszBuFFer[nIndex], &Input[i], nInputLen-i);
}
