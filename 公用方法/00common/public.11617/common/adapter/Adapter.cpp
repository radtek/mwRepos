//Adapter.cpp
#include "StdAfx.h"



#include "Adapter.h"
#include "CompatibleEvent.h"
#include "CompatibleThread.h"
//#include "../../../logmgr/LogMgr.h"


//�������Ʊ���ת��
DerivedLock m_csTransFmt;

//**********************************
//Description: ��������ʼ���¼�����
// Parameter:  CCompatibleEvent &event
// Returns:    bool
//************************************
bool CAdapter::InitCompatibleEvent(CCompatibleEvent &event, bool bManualReset)
{
    bool bRet = false;

    bRet = event.Init(bManualReset);

    return bRet;
}

//**********************************
//Description: �����������¼�����
// Parameter:  CCompatibleEvent &event
// Returns:    void
//************************************
void CAdapter::UnInitCompatibleEvent(CCompatibleEvent &event)
{
    event.UnInit();
    return;
}

//**********************************
//Description: �ȴ��¼�
// Parameter:  CCompatibleEvent &event
//             DWORD dwMilliseconds  �ȴ���ʱʱ��/����
// Returns:    int ���� WAIT_OBJECT_0 ��WAIT_TIMEOUT��WAIT_FAILED
//************************************
int CAdapter::WaitForCompatibleEvent(CCompatibleEvent &event, DWORD dwMilliseconds)
{
#ifdef WIN32
    return ::WaitForSingleObject(event.GetEvent(), dwMilliseconds);
#elif defined LINUX
    int iRet        = WAIT_FAILED;
    int nSec        = dwMilliseconds/1000;
    int nMillisec   = dwMilliseconds-nSec*1000;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    timespec waittime = {time(NULL)+nSec, nMillisec*1000*1000};
    //waittime.tv_sec   = time(NULL) + nSec;
    //waittime.tv_nsec  = /*(time(NULL)+nSec)*1000*1000*1000+*/nMillisec*1000*1000;
    //û�ź���ȴ������ź�ֱ�ӷ���
    if(!event.IsSignaled())
    {
        int iWaitRet = pthread_cond_timedwait(&event.GetEvent(),&mutex,(const struct  timespec*)&waittime);
        if(0 == iWaitRet)
        {
            iRet = WAIT_OBJECT_0;
        }
        else if(iWaitRet == ETIMEDOUT)
        {
            iRet = WAIT_TIMEOUT;
        }
        else
        {
            iRet = WAIT_FAILED;
        }
    }
    else
    {
        iRet = WAIT_OBJECT_0;
    }
    if (WAIT_OBJECT_0 == iRet)
    {
        //�����ֶ����ź���ǿ��������Ϊ���ź�
        if (event.IsManualReset())
        {
            SetCompatibleEvent(event);
        }
        else
        {
            event.ResetSignalCnt();
        }
    }
    return iRet;
#endif
}

//**********************************
//Description: �ȴ�����¼�(WaitForMultipleObjects��������)
// Parameter:  nCount �¼������� ���ֵΪMAXIMUM_WAIT_OBJECTS��64��
//             lpHandles �¼������ָ��
//             bWaitAll �ȴ������ͣ����ΪTRUE ��ȴ������ź�����Ч������ִ�У�
//                      FALSE ��������һ���ź�����Чʱ������ִ��(��ʱֻ֧��FALSE�����)
//             dwMilliseconds ��ʱʱ��
// Returns:    int ����0��ʾ���¼�  1��ʾ��ʱ  -1���ź�
//************************************
DWORD CAdapter::WaitForMultipleCompatibleEvent(
             DWORD nCount,                       // number of handles in the handle array
             CCompatibleEvent** lppHandles,         // pointer to the object-handle array
             BOOL fWaitAll,                      // wait flag
             DWORD dwMilliseconds                // time-out interval in milliseconds
             )
{
    //��������¼���
    if (nCount > MAXIMUM_WAIT_OBJECTS)
    {
        return WAIT_FAILED;
    }
#ifdef WIN32
    HANDLE handles[MAXIMUM_WAIT_OBJECTS] = {0};
    for (int i = 0; i < nCount; ++i)
    {
        handles[i] = lppHandles[i]->GetEvent();
    }
    return ::WaitForMultipleObjects(nCount, handles, fWaitAll, dwMilliseconds);
#elif defined LINUX
    DWORD dwRet = WAIT_FAILED;
    int iWaitRet = 0;
    int iLoop = 0;
    if(!fWaitAll)
    {
        CCompatibleEvent mEvent;
        InitCompatibleEvent(mEvent, false);

        //��������¼���ȴ����¼�ָ������ѵ����¼����еı��
        for(iLoop = 0; iLoop < nCount; ++iLoop)
        {
            lppHandles[iLoop]->SetMEvent(&mEvent, iLoop);
        }

        //�ȴ�
        iWaitRet = WaitForCompatibleEvent(mEvent, dwMilliseconds);

        //�ȵ��¼���ȡ���¼����
        if(WAIT_OBJECT_0 == iWaitRet)
        {
            dwRet = WAIT_OBJECT_0 + mEvent.GetMEvent()->me_nNo;
        }
        else if(WAIT_TIMEOUT == iWaitRet)
        {
            dwRet = WAIT_TIMEOUT;
        }
        else
        {
            dwRet = WAIT_FAILED;
        }

        //ѭ����մ����¼���ȴ����¼�ָ������ѵ����¼����еı��
        for(iLoop = 0; iLoop < nCount; ++iLoop)
        {
            lppHandles[iLoop]->SetMEvent(NULL, 0);
        }
    }
    else
    {   //ͬʱ�ȴ�����¼��ݲ�֧��
        dwRet = WAIT_FAILED;
    }
    return dwRet;
#endif
}


//**********************************
//Description: �����¼�״̬
// Parameter:  CCompatibleEvent &event
// Returns:    void
//************************************
void CAdapter::SetCompatibleEvent(CCompatibleEvent &event)
{
    event.SetCompatibleEvent();
    return;
}


//**********************************
//Description: �����¼�����Ϊ���ź�
// Parameter:  CCompatibleEvent &event
// Returns:    void
//************************************
void CAdapter::ResetCompatibleEvent(CCompatibleEvent &event)
{
    event.ResetSignalCnt();
    return;
}

//static int iCount = 0;
//**********************************
//Description: ����WIN32��linux�������ݵ��߳�
// Parameter:  start_address  �̺߳�����ڵ�ַ
//             arglist        �����̵߳Ĳ���
// Returns:    bool  �����ɹ�����true ���򷵻�false
//************************************
bool CAdapter::CreateCompatibleThread(THREAD_RETURN(__STDCALL *start_address )( void * ), void *arglist)
{
//  char szBuf[100] = {0};
//  sprintf(szBuf, "Create Thread %d", iCount++);
//  CLogMgr::GetInstance().WriteLog(szBuf, "test.txt");
    int iRet = CCompatibleThread::CreateThread(start_address, arglist);
    return iRet == 0;
}

void CAdapter::EndCompatibleThread()
{
#ifdef WIN32
    _endthreadex(0);
#elif defined LINUX
    
#endif
}

//**********************************
//Description: ��ȡ��ǰ�߳�ID
// Parameter:  void
// Returns:    unsigned long �߳�ID
//************************************
unsigned long CAdapter::GetCurrentThreadId(void)
{
    //WINDOWS���뻷��
#ifdef WIN32
    return ::GetCurrentThreadId();
    //linux���뻷��
#elif defined LINUX
    return pthread_self();
#endif

}

//**********************************
//Description: ���ض�̬��
// Parameter:  LPCTSTR lpFileName ��̬�����ƻ�·��
//             int nFlag ���ط�ʽ
// Returns:    ʧ�ܷ���NULL
//************************************
HMODULE CAdapter::LoadLibrary(const char* pfilename, int nFlag)
{
    //DEBUG_PRINTF("Call CAdapter::LoadLibrary", 0);
    //WINDOWS���뻷��
#ifdef WIN32
    return ::LoadLibrary(pfilename);
    //linux���뻷��
#elif defined LINUX
    //nFlag = RTLD_NOW;
    nFlag = RTLD_LAZY;
    return dlopen(pfilename, nFlag);
#endif
}

//**********************************
//Description: ȡ������ַ
// Parameter:  void* handle loadlibrary���ص�ֵ
//             const char* symbol  ������
// Returns:    ʧ�ܷ���NULL
//************************************
FARPROC CAdapter::GetProcAddress(HMODULE handle, const char* symbol)
{
    //WINDOWS���뻷��
#ifdef WIN32
    return ::GetProcAddress(handle, symbol);
    //linux���뻷��
#elif defined LINUX
    return dlsym(handle, symbol);
#endif
}

//**********************************
//Description: �ͷŶ�̬
// Parameter:  void* handle loadlibrary���ص�ֵ
// Returns:
//************************************
int CAdapter::FreeLibrary(HMODULE handle)
{
    //WINDOWS���뻷��
#ifdef WIN32
    return ::FreeLibrary(handle);
    //linux���뻷��
#elif defined LINUX
    return dlclose(handle);
#endif
}

//**********************************
//Description: ת��Ϊ���ַ�ģʽ
// Parameter:  ����MSDN
// Returns:    int ���ر�ת�����ֽ���
//              ʧ�ܷ���0
//              ��ʱ֧��gbk��utf8����
//ע:cchWideCharΪ0ʱ��ʾҪȡ��Ҫ��������С��
//��ʱlinux�汾������֧��ת������ַ����Ȳ�����8K�Ĳ���
//************************************
int CAdapter::MultiByteToWideChar(
      UINT CodePage,         // code page
      DWORD dwFlags,         // character-type options
      LPCSTR lpMultiByteStr, // string to map
      int cbMultiByte,       // number of bytes in string
      LPWSTR lpWideCharStr,  // wide-character buffer
      int cchWideChar        // size of buffer
      )
{
    int iRet = 0;
//WINDOWS���뻷��
#ifdef WIN32
    iRet = ::MultiByteToWideChar(CodePage,dwFlags,lpMultiByteStr,
                                 cbMultiByte,lpWideCharStr,cchWideChar);
//linux���뻷��
#elif defined LINUX
// 	SafeLock safelock(&m_csTransFmt);
//     if(CP_ACP == CodePage)
//     {
//         setlocale(LC_ALL,LC_NAME_zh_CN_DEFAULT/*"zh_CN.gb18030"*/);
//     }
//     else if(CP_UTF8 == CodePage)
//     {
//         setlocale(LC_ALL,LC_NAME_zh_CN_UTF8);
//     }
//     else
//     {
//         setlocale(LC_ALL,LC_NAME_zh_CN_DEFAULT);
//     }
//     //��ȡ���軺������С
//     if (0 == cchWideChar)
//     {
//         wchar_t dest[8192+1] = {0};
//         iRet = mbstowcs(dest, lpMultiByteStr, 8192);
//     }
//     else
//     {
//         iRet = mbstowcs(lpWideCharStr, lpMultiByteStr, cchWideChar);
//     }
//     if(-1 == iRet)
//     {
//         iRet = 0;
//     }
//     setlocale(LC_ALL,NULL);

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	iconv_t cd;
	int rc;
	char **pin = (char **)&lpMultiByteStr;
	char **pout = (char**)&lpWideCharStr;

	size_t tWideChar = cchWideChar*sizeof(wchar_t);
	size_t tMultiByte = strlen(lpMultiByteStr);

	if (CP_UTF8 == CodePage)
	{
		cd = iconv_open(CSET_UCS4LE, CSET_UTF8);
	}
	else
	{
		cd = iconv_open(CSET_UCS4LE, CSET_GBK);
	}
	if (cd == (void*)0xffffffff)
	{
		//printf("%s %d\n", strerror(errno), errno);
		return 0;
	}

    //��ȡ���軺������С
    if (0 == cchWideChar)
    {
		wchar_t dest[8192+1] = {0};
 		size_t tDestWideChar = 8192*sizeof(wchar_t);
		char *pdest = (char*)dest;
		char **ppdest = &pdest;

		if (iconv(cd, pin, &tMultiByte, ppdest, &tDestWideChar) ==-1 )
		{
			iconv_close(cd);
			return 0;
		}
		iRet = (8192*sizeof(wchar_t) - tDestWideChar)/sizeof(wchar_t);
    }
    else
    {
		if (iconv(cd, pin, &tMultiByte, pout, (size_t*)&tWideChar) ==-1 )
		{
			iconv_close(cd);
			return 0;
		}
		iRet = (cchWideChar*sizeof(wchar_t) - tWideChar)/sizeof(wchar_t);
	}
	iconv_close(cd);

#endif
    return iRet;
}

//**********************************
//Description: ת��Ϊ���ֽ�ģʽ
// Parameter:  ����MSDN
// Returns:    int ���ر�ת�����ֽ���
//              ʧ�ܷ���0
//              ��ʱ֧��gbk��utf8����
//ע:cbMultiByteΪ0ʱ��ʾҪȡ��Ҫ��������С��
//��ʱlinux�汾������֧��ת������ַ����Ȳ�����8K�Ĳ���
//************************************
int CAdapter::WideCharToMultiByte(
        UINT CodePage,            // code page
        DWORD dwFlags,            // performance and mapping flags
        LPCWSTR lpWideCharStr,    // wide-character string
        int cchWideChar,          // number of chars in string
        LPSTR lpMultiByteStr,     // buffer for new string
        int cbMultiByte,          // size of buffer
        LPCSTR lpDefaultChar,     // default for unmappable chars
        LPBOOL lpUsedDefaultChar  // set when default char used
        )
{
    int iRet = 0;
//WINDOWS���뻷��
#ifdef WIN32
    iRet = ::WideCharToMultiByte(CodePage,dwFlags,lpWideCharStr,
                                 cchWideChar,lpMultiByteStr,
                                 cbMultiByte,lpDefaultChar,
                                 lpUsedDefaultChar);
//linux���뻷��
#elif defined LINUX
// 	SafeLock safelock(&m_csTransFmt);
//     if(CP_ACP == CodePage)
//     {
//         setlocale(LC_ALL,LC_NAME_zh_CN_DEFAULT/*"zh_CN.gb18030"*/);
//     }
//     else if(CP_UTF8 == CodePage)
//     {
//         setlocale(LC_ALL,LC_NAME_zh_CN_UTF8);
//     }
//     else
//     {
//         setlocale(LC_ALL,LC_NAME_zh_CN_DEFAULT);
//     }
//     //��ȡ���軺������С
//     if (0 == cbMultiByte)
//     {
//         char dest[8192+1] = {0};
//         iRet = wcstombs(dest, lpWideCharStr, 8192);
//     }
// 	else
// 	{
// 		iRet = wcstombs(lpMultiByteStr, lpWideCharStr, cbMultiByte);
// 	}
//     if(-1 == iRet)
//     {
//         iRet = 0;
//     }
//     setlocale(LC_ALL,NULL);

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	iconv_t cd;
	int rc;
	char **pin = (char **)&lpWideCharStr;
	char **pout = (char**)&lpMultiByteStr;
	size_t tWideChar = wcslen(lpWideCharStr)*sizeof(wchar_t);
	size_t tMultiByte = cbMultiByte;

	if (CP_UTF8 == CodePage)
	{
		cd = iconv_open(CSET_UTF8, CSET_UCS4LE);
	}
	else
	{
		cd = iconv_open(CSET_GBK, CSET_UCS4LE);
	}
	if (cd == (void*)0xffffffff)
	{
		//printf("%s %d\n", strerror(errno), errno);
		return 0;
	}

	//��ȡ���軺������С
    if (0 == cbMultiByte)
    {
        char dest[8192+1] = {0};
		size_t tDestMultiByte = sizeof(dest)-1;
		char *pdest = dest;
		char **ppdest = &pdest;
		if (iconv(cd, pin, &tWideChar, ppdest, &tDestMultiByte) ==-1 )
		{
			iconv_close(cd);
			return 0;
		}
		iRet = sizeof(dest)-1-tDestMultiByte;
    }
    else
    {
		if (iconv(cd, pin, (size_t*)&tWideChar, pout, (size_t*)&tMultiByte) ==-1 )
		{
			iconv_close(cd);
			return 0;
		}
		iRet = cbMultiByte-tMultiByte;
		
	}
	iconv_close(cd);

#endif
    return iRet;
}

char* CAdapter::itoa_linux(int value, char *string, int radix)
{
    char szFormat[10+1] = {0};
    switch (radix)
    {
    case 2:
        break;
    case 8:
        strcpy(szFormat, "%o");
        break;
    case 10:
        strcpy(szFormat, "%d");
        break;
    case 16:
        strcpy(szFormat, "%x");
        break;
    default:
        strcpy(szFormat, "%d");
        break;
    }
    sprintf(string, szFormat, value);
    return string;
}

//linux�汾ltoa
char* CAdapter::ltoa_linux(long value, char *string, int radix)
{
    char szFormat[10+1] = {0};
    switch (radix)
    {
    case 2:
        break;
    case 8:
        strcpy(szFormat, "%o");
        break;
    case 10:
        strcpy(szFormat, "%d");
        break;
    case 16:
        strcpy(szFormat, "%x");
        break;
    default:
        strcpy(szFormat, "%d");
        break;
    }
    sprintf(string, szFormat, value);
    return string;
}

//**********************************
//Description: �����Ͱ�radixָ���Ľ���ת���ַ���(linux��ֻ֧��radix=8,10,16)
// Parameter:  ����MSDN
// Returns:    stringָ��
//             û�д����뷵��
//************************************
char* CAdapter::itoa(int value, char *string, int radix)
{
    //WINDOWS���뻷��
#ifdef WIN32
    return ::itoa(value, string, radix);
    //linux���뻷��
#elif defined LINUX
    return itoa_linux(value, string, radix);
#endif
}

//**********************************
//Description: �����Ͱ�radixָ���Ľ���ת���ַ���(linux��ֻ֧��radix=8,10,16)
// Parameter:  ����MSDN
// Returns:    stringָ��
//             û�д����뷵��
//************************************
char* CAdapter::ltoa(int value, char *string, int radix)
{
    //WINDOWS���뻷��
#ifdef WIN32
    return ::ltoa(value, string, radix);
    //linux���뻷��
#elif defined LINUX
    return ltoa_linux(value, string, radix);
#endif
}

char* CAdapter::i64toa_linux(__int64 value, char *string, int radix)
{
    char szFormat[10+1] = {0};
    switch (radix)
    {
    case 2:
        break;
    case 8:
        //strcpy(szFormat, "%llo");
        break;
    case 10:
        strcpy(szFormat, "%lld");
        break;
    case 16:
        //strcpy(szFormat, "%llx");
        break;
    default:
        strcpy(szFormat, "%lld");
        break;
    }
    sprintf(string, szFormat, value);
    return string;
}

char* CAdapter::ui64toa_linux(unsigned _int64 value, char *string, int radix)
{
    char szFormat[10+1] = {0};
    switch (radix)
    {
    case 2:
        break;
    case 8:
        //strcpy(szFormat, "%llo");
        break;
    case 10:
        strcpy(szFormat, "%lld");
        break;
    case 16:
        //strcpy(szFormat, "%llx");
        break;
    default:
        strcpy(szFormat, "%lld");
        break;
    }
    sprintf(string, szFormat, value);
    return string;
}

//**********************************
//Description: �������Ͱ�radixָ���Ľ���ת���ַ���(linux��ֻ֧��radix=10)
// Parameter:  ����MSDN
// Returns:    stringָ��
//             û�д����뷵��
//************************************
char* CAdapter::_i64toa(__int64 value, char *string, int radix)
{
    //WINDOWS���뻷��
#ifdef WIN32
    return ::_i64toa(value, string, radix);
    //linux���뻷��
#elif defined LINUX
    return i64toa_linux(value, string, radix);
#endif
}
char* CAdapter::_ui64toa(unsigned _int64 value, char *string, int radix)
{
    //WINDOWS���뻷��
#ifdef WIN32
    return ::_ui64toa(value, string, radix);
    //linux���뻷��
#elif defined LINUX
    return ui64toa_linux(value, string, radix);
#endif
}

//************************************
// Description:��US2�ַ���תΪASCII�ַ���
// Parameter: [IN] const char *InMsg    : ԭʼ�ַ���
//            [IN] int InLen            : ԭʼ�ַ�������
//            [OUT]char *Data           : ASCII��
//            [OUT]int *Len             : ASCII������
// Returns:   int                       0:�ɹ�  ��0:ʧ��
//************************************
int  CAdapter::US2ToAscii(const char *InMsg, int InLen, char *Data, int *Len)
{
    wchar_t uMsg[2048] = {0};
    int  uLen = 0;
    char temp[2048] = {0};

    try
    {
        uLen = InLen/2;
        memset(uMsg, '\0', sizeof(uMsg));

        for (int i=0; i<uLen; ++i)
        {
            uMsg[i] = InMsg[2*i] & 0xff;
            uMsg[i] = uMsg[i] << 8;
            uMsg[i] = uMsg[i] + (InMsg[2*i+1] & 0xff);
        }
        uLen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, uMsg, uLen, temp, sizeof(temp), NULL, NULL);
        *Len = uLen;
        memcpy(Data, temp, uLen);
        return 0;
    }
    catch(...)
    {
    }
    return -99;
}

//************************************
// Description:��ASCII�ַ���תΪUS2�ַ���
// Parameter: [IN] const char* Msg      : ԭʼ�ַ���
//            [OUT]unsigned char *Data  : US2�ַ���
//            [OUT]unsigned int *Len    : US2������
// Returns:   int                       0:�ɹ�  ��0:ʧ��
//************************************
int  CAdapter::AsciiToUS2(const char* Msg, unsigned char *Data, unsigned int *Len)
{
    wchar_t temp[2048]={0};
    char szUMsg[2048]={0};
    try
    {
        MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,Msg, strlen(Msg), temp, sizeof(temp));
        int len = wcslen(temp);
        for (int i=0;i<len;++i)
        {
            int iFirst = 1;
            for(int j=0;j<2;++j)
            {
                if(1 == iFirst)
                {
                    szUMsg[2*i] = (temp[i]>>8) & 0xff;
                    iFirst = 0;
                }
                else
                {
                    szUMsg[2*i+1] = temp[i]&0xff;
                    iFirst = 1;
                }
            }
        }
        *Len = 2*len;
        memcpy(Data, szUMsg, *Len);
        return 0;
    }
    catch(...)
    {
    }
    return -99;
}

void CAdapter::Sleep(DWORD dwMilliseconds)
{
//WINDOWS���뻷��
#ifdef WIN32
    ::Sleep(dwMilliseconds);
//linux���뻷��
#elif defined LINUX
    usleep(dwMilliseconds*1000);
#endif
}

//**********************************
//Description: ��ȡϵͳ������ĵδ�ʱ�䣬����
// Parameter:  void
// Returns:    DWORD
// note��      �÷��ؼ��������������ȷ��ʹ���޷�����(unsigned long)
//************************************
DWORD CAdapter::GetTickCount (void)
{
//WINDOWS���뻷��
#ifdef WIN32
    return ::GetTickCount();
//linux���뻷��
#elif defined LINUX
	struct sysinfo info;
	if (0 && 0 == sysinfo(&info))
	{
		return info.uptime;
	}
	else
	{
    	struct timeval t;
		gettimeofday(&t, NULL);

		return t.tv_sec*1000 + t.tv_usec/1000;
	}
#endif
}

//**********************************
//Description: ��ȡ��ǰʱ��(HH:MM:SS),��ת��ΪDATE����
// Parameter:  void
// Returns:    DATE
// note��      win�·���0.000000000000-0.99999999999999֮���DATE
//             linux�·�������
//************************************
DATE CAdapter::get_current_time()
{
    //WINDOWS���뻷��
#ifdef WIN32
    SYSTEMTIME t;
    ::GetLocalTime(&t);
    COleDateTime oleTime;
    oleTime.SetTime(t.wHour,t.wMinute,t.wSecond);
    return oleTime.m_dt;
    //linux���뻷��
#elif defined LINUX
    time_t tNow = time(NULL);
    struct tm* t = localtime(&tNow);
    DATE dt = t->tm_hour*3600+t->tm_min*60+t->tm_sec;
    return dt;
#endif
}

void CAdapter::Beep(DWORD dwFreq, DWORD dwDuration)
{
    //WINDOWS���뻷��
#ifdef WIN32
    ::Beep(dwFreq, dwDuration);
    //linux���뻷��
#elif defined LINUX
    usleep(dwDuration*1000);
#endif
}

//**********************************
//Description: ��ȡ��ǰ��ϵͳʱ��
// Parameter:  LPSYSTEMTIME_EX lpSystemTime_ex �Զ������չϵͳʱ��
//              ��windows��SYSTEMTIME��Աһ�£��������͸�Ϊ32λint
// Returns:    void
//
//************************************
void CAdapter::GetLocalTime(LPSYSTEMTIME_EX lpSystemTime_ex)
{
//WINDOWS���뻷��
#ifdef WIN32
    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);
    lpSystemTime_ex->m_iYear = sysTime.wYear;
    lpSystemTime_ex->m_iMonth = sysTime.wMonth;
    lpSystemTime_ex->m_iDayOfWeek = sysTime.wDayOfWeek;
    lpSystemTime_ex->m_iDay = sysTime.wDay;
    lpSystemTime_ex->m_iHour = sysTime.wHour;
    lpSystemTime_ex->m_iMinute = sysTime.wMinute;
    lpSystemTime_ex->m_iSecond = sysTime.wSecond;
    lpSystemTime_ex->m_iMilliseconds = sysTime.wMilliseconds;
//linux���뻷��
#elif defined LINUX
    struct timeval tv;
    struct tm      t;
    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &t);
    //linuxAPI mktime�������Ҫ��1900
    lpSystemTime_ex->m_iYear = t.tm_year + 1900;
    //linux��ȡ���·��Ǵ�0-11��windowsΪ1-12��Ϊ��������1����
    lpSystemTime_ex->m_iMonth = t.tm_mon + 1;
    lpSystemTime_ex->m_iDayOfWeek = t.tm_wday;
    lpSystemTime_ex->m_iDay = t.tm_mday;
    lpSystemTime_ex->m_iHour = t.tm_hour;
    lpSystemTime_ex->m_iMinute = t.tm_min;
    lpSystemTime_ex->m_iSecond = t.tm_sec;
    lpSystemTime_ex->m_iMilliseconds = (int)(tv.tv_usec)/1000;
#endif
}

//**********************************
//Description: ��ȡ��ǰ��ϵͳʱ��(����һ��ʱ��ƫ��)
// Parameter:  LPSYSTEMTIME_EX lpSystemTime_ex �Զ������չϵͳʱ��
//              ��windows��SYSTEMTIME��Աһ�£��������͸�Ϊ32λint
//             delay ��Ҫƫ�Ƶ�ʱ��/��
// Returns:    void
//
//************************************
void CAdapter::GetLocalTimeEx(LPSYSTEMTIME_EX lpSystemTime_ex, unsigned int delay)
{
    //WINDOWS���뻷��
#ifdef WIN32
        time_t t;
        SYSTEMTIME sysTime;
        time(&t);
        t += delay;

        //�㷨����MyCmppApi.cpp�е�TimetToLocalSystemTime����
        {
            FILETIME ft;

            LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000;
            ft.dwLowDateTime = (DWORD) ll;
            ft.dwHighDateTime = (DWORD)(ll >> 32);

            FileTimeToSystemTime(&ft, &sysTime);
        }

        lpSystemTime_ex->m_iYear = sysTime.wYear;
        lpSystemTime_ex->m_iMonth = sysTime.wMonth;
        lpSystemTime_ex->m_iDayOfWeek = sysTime.wDayOfWeek;
        lpSystemTime_ex->m_iDay = sysTime.wDay;
        lpSystemTime_ex->m_iHour = sysTime.wHour;
        lpSystemTime_ex->m_iMinute = sysTime.wMinute;
        lpSystemTime_ex->m_iSecond = sysTime.wSecond;
        lpSystemTime_ex->m_iMilliseconds = sysTime.wMilliseconds;
    //linux���뻷��
#elif defined LINUX
        time_t timep;
        struct tm *p;
        time(&timep);
        timep = timep + delay;

        p=localtime(&timep);

        //linuxAPI mktime�������Ҫ��1900
        lpSystemTime_ex->m_iYear = p->tm_year + 1900;
        //linux��ȡ���·��Ǵ�0-11��windowsΪ1-12��Ϊ��������1����
        lpSystemTime_ex->m_iMonth = p->tm_mon + 1;
        lpSystemTime_ex->m_iDayOfWeek = p->tm_wday;
        lpSystemTime_ex->m_iDay = p->tm_mday;
        lpSystemTime_ex->m_iHour = p->tm_hour;
        lpSystemTime_ex->m_iMinute = p->tm_min;
        lpSystemTime_ex->m_iSecond = p->tm_sec;
        lpSystemTime_ex->m_iMilliseconds = 0;
#endif

}

//**********************************
//Description: ��ȡ��ǰ��ϵͳʱ��(����һ��ʱ��ƫ��)
// Parameter:  LPSYSTEMTIME_EX lpSystemTime_ex �Զ������չϵͳʱ��
//              ��windows��SYSTEMTIME��Աһ�£��������͸�Ϊ32λint
//             delay ��Ҫƫ�Ƶ�ʱ��/��
// Returns:    void
//
//************************************
void CAdapter::GetLocalTimeExEx(LPSYSTEMTIME_EX lpSystemTime_ex, unsigned int delay)
{
    //WINDOWS���뻷��
#ifdef WIN32
        time_t t;
        SYSTEMTIME sysTime;
        time(&t);
        t += delay;

        //�㷨����MyCmppApi.cpp�е�TimetToLocalSystemTime����
        {
            FILETIME ft;

            LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000;
            ft.dwLowDateTime = (DWORD) ll;
            ft.dwHighDateTime = (DWORD)(ll >> 32);

            FileTimeToSystemTime(&ft, &sysTime);
        }

        lpSystemTime_ex->m_iYear = sysTime.wYear;
        lpSystemTime_ex->m_iMonth = sysTime.wMonth;
        lpSystemTime_ex->m_iDayOfWeek = sysTime.wDayOfWeek;
        lpSystemTime_ex->m_iDay = sysTime.wDay;
        lpSystemTime_ex->m_iHour = sysTime.wHour;
        lpSystemTime_ex->m_iMinute = sysTime.wMinute;
        lpSystemTime_ex->m_iSecond = sysTime.wSecond;
        lpSystemTime_ex->m_iMilliseconds = sysTime.wMilliseconds;
    //linux���뻷��
#elif defined LINUX
		struct timeval tv;
        struct tm t;
		gettimeofday(&tv, NULL);
		tv.tv_sec = tv.tv_sec + delay;

        gmtime_r(&tv.tv_sec, &t);

        //linuxAPI mktime�������Ҫ��1900
        lpSystemTime_ex->m_iYear = t.tm_year + 1900;
        //linux��ȡ���·��Ǵ�0-11��windowsΪ1-12��Ϊ��������1����
        lpSystemTime_ex->m_iMonth = t.tm_mon + 1;
        lpSystemTime_ex->m_iDayOfWeek = t.tm_wday;
        lpSystemTime_ex->m_iDay = t.tm_mday;
        lpSystemTime_ex->m_iHour = t.tm_hour;
        lpSystemTime_ex->m_iMinute = t.tm_min;
        lpSystemTime_ex->m_iSecond = t.tm_sec;
        lpSystemTime_ex->m_iMilliseconds = (int)(tv.tv_usec)/1000;
#endif
}

//**********************************
//Description: ����ļ���Ŀ¼�Ƿ����
// Parameter:  LPCTSTR pszPath ָ��Ŀ¼
// Returns:    BOOL
//************************************
BOOL CAdapter::PathFileExists(LPCTSTR pszPath)
{
//WINDOWS���뻷��
#ifdef WIN32
    return ::PathFileExists(pszPath);
//linux���뻷��
#elif defined LINUX
    return 0 == access(pszPath, R_OK);
#endif
}

//**********************************
//Description: ȡ��ǰӦ������Ŀ¼
// Parameter:  char* szPath ȫ·��
//             int nMaxSize ·���ֵ
// Returns:    void
//************************************
void CAdapter::GetCurrentPath(char* szPath, int nMaxSize)
{
    //WINDOWS���뻷��
#ifdef WIN32
    ::GetModuleFileName(NULL, szPath, nMaxSize);
    std::string strPath(szPath);
    size_t found = strPath.find_last_of("/\\");
    memset(szPath+found+1, 0, strPath.size()-found-1);
    int nLen = strlen(szPath);
    for (int i = 0; i < nLen; ++i)
    {
        if (szPath[i] == '\\')
        {
            szPath[i] = '/';
        }
    }
    //linux���뻷��
#elif defined LINUX
    if (NULL != szPath)
    {
        getcwd(szPath, nMaxSize);
        strcat(szPath, "/");
    }
#endif
}

//**********************************
//Description: ����Ŀ¼
// Parameter:  LPCTSTR lpPathName Ҫ������Ŀ¼��
// Returns:    BOOL
//************************************
BOOL CAdapter::CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
//WINDOWS���뻷��
#ifdef WIN32
    return ::CreateDirectory(lpPathName, lpSecurityAttributes);
//linux���뻷��
#elif defined LINUX
    //�����Ѿ����ڵ�Ŀ¼��ݹ鴴��Ŀ¼ʱ��ʧ��
    lpSecurityAttributes = 0777;
    if(0 == mkdir(lpPathName, lpSecurityAttributes))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif
}

//**********************************
//Description: ö��ָ��Ŀ¼�µ��ļ���
// Parameter:  const char* szFolderName ��Ŀ¼
//             std::vector<std::string>& vFileName �����������ļ���·������
// Returns:    void
// Note��
//************************************
void CAdapter::EnumDir(const char* szFolderName, std::vector<std::string>& vFileName)
{
//WINDOWS���뻷��
#ifdef WIN32
    CFileFind finder;
    BOOL bFileExsit = finder.FindFile(szFolderName);
    if(!bFileExsit)
    {
        return ;
    }
    CTime ct;
    CTimeSpan ts;
    while (bFileExsit)
    {
        bFileExsit = finder.FindNextFile();
        if (finder.IsDirectory()
            && !finder.IsSystem()
            && !finder.IsHidden()
            && !finder.IsDots()
            && !finder.IsTemporary())
        {
            vFileName.push_back((LPCSTR)finder.GetFilePath());//�����ļ���  ·��+�ļ���
        }
    }
//linux���뻷��
#elif defined LINUX
    struct dirent* ent = NULL;
    DIR *pDir = opendir(szFolderName);
    if (pDir)
    {
        while (NULL != (ent=readdir(pDir)))
        {
            //4��ʾ�ļ��� 8��ʾ�ļ�
            if (4 != ent->d_type
                || ('.' == ent->d_name[0] && 1 == strlen(ent->d_name))
                || ('.' == ent->d_name[0] && '.' == ent->d_name[1] && 2 == strlen(ent->d_name)))
            {
                continue;
            }

            std::string str = szFolderName;
            str += "/";
            str += ent->d_name;
            struct stat st;
            if (0 == stat(str.c_str(), &st))
            {
                vFileName.push_back(str);
                //printf("%d\r\n", st.st_ctime);
            }
        }
        closedir(pDir);
    }
#endif
}

//**********************************
//Description: ö��ָ��Ŀ¼����ָ����չ�����ļ�
// Parameter:  const char* szFolderName ��Ŀ¼
//             std::vector<std::string>& vFileName �����������ļ�·������
// Returns:    void
// Note��
//************************************
void CAdapter::EnumFile(const char* szFolderName,const char *szExName, std::vector<std::string>& vFileName)
{
    //WINDOWS���뻷��
#ifdef WIN32
    CFileFind finder;
    BOOL bFileExsit = finder.FindFile(szFolderName);
    if(!bFileExsit)
    {
        return ;
    }
    CTime ct;
    CTimeSpan ts;
    while (bFileExsit)
    {
        bFileExsit = finder.FindNextFile();
        CString strFilePath = finder.GetFilePath();
        if (0 == strFilePath.Right(strlen(szExName)).CompareNoCase(szExName))
        {
            vFileName.push_back((LPCSTR)strFilePath);
        }
    }
//linux���뻷��
#elif defined LINUX
    struct dirent* ent = NULL;
    DIR *pDir = opendir(szFolderName);
    if (pDir)
    {
        while (NULL != (ent=readdir(pDir)))
        {
            //4��ʾ�ļ��� 8��ʾ�ļ�
            if (8 != ent->d_type
                || ('.' == ent->d_name[0] && 1 == strlen(ent->d_name))
                || ('.' == ent->d_name[0] && '.' == ent->d_name[1] && 2 == strlen(ent->d_name)))
            {
                continue;
            }

            std::string str = szFolderName;
            int iPos = -1;
            str += "/";
            str += ent->d_name;

            if((iPos=str.find_last_of('.', str.length()))<0)
            {
                continue;
            }
            if(0 == str.compare(iPos, strlen(szExName), szExName))
            {
                struct stat st;
                if (0 == stat(str.c_str(), &st))
                {
                    vFileName.push_back(str);
                    //printf("%d\r\n", st.st_ctime);
                }
            }

        }
        closedir(pDir);
    }
#endif
}

//**********************************
//Description: ö��ָ��Ŀ¼�µ��ļ�
// Parameter:  const char* szFolderName ��Ŀ¼
//             std::vector<std::string>& vFileName �����������ļ�·������
// Returns:    void
// Note��
//************************************
void CAdapter::EnumFile(const char* szFolderName, const char *szExName, int nDay, std::vector<std::string>& vFileName)
{
    //WINDOWS���뻷��
#ifdef WIN32
    CFileFind finder;
    BOOL bFileExsit = finder.FindFile(szFolderName);
    if(!bFileExsit)
    {
        return ;
    }
    CTime ct;
    CTimeSpan ts;
    while (bFileExsit)
    {
        bFileExsit = finder.FindNextFile();
        CString strFilePath = finder.GetFilePath();
		if (finder.GetCreationTime(ct))
		{
			ts = CTime::GetCurrentTime() - ct;
			if (ts.GetDays() >= nDay && 0 == strFilePath.Right(strlen(szExName)).CompareNoCase(szExName))
			{
				vFileName.push_back((LPCSTR)strFilePath);
			}
        }
    }
	//linux���뻷��
#elif defined LINUX
    struct dirent* ent = NULL;
    DIR *pDir = opendir(szFolderName);
    if (pDir)
    {
        while (NULL != (ent=readdir(pDir)))
        {
            //4��ʾ�ļ��� 8��ʾ�ļ�
            if (8 != ent->d_type
                || ('.' == ent->d_name[0] && 1 == strlen(ent->d_name))
                || ('.' == ent->d_name[0] && '.' == ent->d_name[1] && 2 == strlen(ent->d_name)))
            {
                continue;
            }
			
            std::string str = szFolderName;
            int iPos = -1;
            str += "/";
            str += ent->d_name;
			
            if((iPos=str.find_last_of('.', str.length()))<0)
            {
                continue;
            }
            if(0 == str.compare(iPos, strlen(szExName), szExName))
            {
                struct stat st;
                if (0 == stat(str.c_str(), &st))
                {
					time_t tNow = time(NULL);
					//printf("%d\r\n", st.st_ctime);
					if (tNow - st.st_ctime > 3600*24*nDay)
					{
						vFileName.push_back(str);
						//printf("%d\r\n", st.st_ctime);
					}
                }
            }
			
        }
        closedir(pDir);
    }
#endif
}

//**********************************
//Description: ö�ٴ���ʱ��൱ǰһ���������ļ���
// Parameter:  const char* szFolderName ��Ŀ¼
//             int nDay ����
//             std::vector<std::string>& vFileName �����������ļ���·������
// Returns:    void
// Note��
//************************************
void CAdapter::EnumDirEx(const char* szFolderName, int nDay, std::vector<std::string>& vFileName)
{
//WINDOWS���뻷��
#ifdef WIN32
    CFileFind finder;
    BOOL bFileExsit = finder.FindFile(szFolderName);
    if(!bFileExsit)
    {
        return ;
    }
    CTime ct;
    CTimeSpan ts;
    while (bFileExsit)
    {
        bFileExsit = finder.FindNextFile();
        if (finder.IsDirectory()
            && !finder.IsSystem()
            && !finder.IsHidden()
            && !finder.IsDots()
            && !finder.IsTemporary())
        {
            if (finder.GetCreationTime(ct))
            {
                ts = CTime::GetCurrentTime() - ct;
                if (ts.GetDays() >= nDay)
                {
                    vFileName.push_back((LPCSTR)finder.GetFilePath());//�����ļ���  ·��+�ļ���
                }
            }
        }
    }
//linux���뻷��
#elif defined LINUX
    struct dirent* ent = NULL;
    DIR *pDir = opendir(szFolderName);
    if (pDir)
    {
        while (NULL != (ent=readdir(pDir)))
        {
            //4��ʾ�ļ��� 8��ʾ�ļ�
            if (4 != ent->d_type
                || ('.' == ent->d_name[0] && 1 == strlen(ent->d_name))
                || ('.' == ent->d_name[0] && '.' == ent->d_name[1] && 2 == strlen(ent->d_name)))
            {
                continue;
            }

            std::string str = szFolderName;
            str += "/";
            str += ent->d_name;
            struct stat st;
            if (0 == stat(str.c_str(), &st))
            {
                time_t tNow = time(NULL);
                //printf("%d\r\n", st.st_ctime);
                if (tNow - st.st_ctime > 3600*24*nDay)
                {
                    vFileName.push_back(str);
                    //printf("%d\r\n", st.st_ctime);
                }
            }
        }
        closedir(pDir);
    }
#endif
}

//**********************************
//Description: shellɾ���ļ����ļ���
// Parameter:  const char *szFilePath �ļ����ļ���ȫ·��
// Returns:    BOOL
// Note��      ����ɾ���ļ���·����������ʾ  C:\\aa\bb\*.*
//************************************
BOOL CAdapter::ShellDeleteFiles(const char *szFilePath)
{
    //WINDOWS���뻷��
#ifdef WIN32
	CString strPath = szFilePath;
	strPath.Replace('/', '\\');
    TCHAR src[MAX_PATH + 1];
    _stprintf(src, "%s", (LPCSTR)strPath);
    src[_tcslen(src)+1]=0;
    SHFILEOPSTRUCT fileOp;
    memset(&fileOp, 0, sizeof(fileOp));
    fileOp.wFunc = FO_DELETE ;
    fileOp.fFlags |= FOF_SILENT;         //��̨����
    //fileOp.fFlags |= FOF_NOERRORUI;      //û�г�����ʾ
    fileOp.fFlags |= FOF_NOCONFIRMATION; //û��ȷ����ʾ
    fileOp.pFrom = src;
    fileOp.pTo = NULL;
    if (::SHFileOperation(&fileOp))
    {
        return FALSE;
    }
    return TRUE;
//linux���뻷��
#elif defined LINUX
    char szCMD[MAX_PATH+1] = {0};
    sprintf(szCMD, "rm -rf %s", szFilePath);
    return 0 == system(szCMD);
#endif
}

char* strstri(const char* pString, const char* pFind)
{
    char* char1 = NULL;
    char* char2 = NULL;
    if((pString == NULL) || (pFind == NULL) || (strlen(pString) < strlen(pFind)))
    {
        return NULL;
    }

    for(char1 = (char*)pString; (*char1) != '\0'; ++char1)
    {
        char* char3 = char1;
        for(char2 = (char*)pFind; (*char2) != '\0' && (*char1) != '\0'; ++char2, ++char1)
        {
            char c1 = (*char1) & 0xDF;
            char c2 = (*char2) & 0xDF;
            if((c1 != c2) || (((c1 > 0x5A) || (c1 < 0x41)) && (*char1 != *char2)))
                break;
        }

        if((*char2) == '\0')
            return char3;

        char1 = char3;
    }
    return NULL;
}

LPTSTR CAdapter::StrStrI(LPCTSTR lpFirst, LPCTSTR lpSrch)
{
    //WINDOWS���뻷��
#ifdef WIN32
    return ::StrStrI(lpFirst, lpSrch);
    //linux���뻷��
#elif defined LINUX
    //�����Ѿ����ڵ�Ŀ¼��ݹ鴴��Ŀ¼ʱ��ʧ��
    return strstri(lpFirst, lpSrch);
#endif
}


////////////////////////////////////////////////////////////////////////////
///        ԭ�Ӳ���ϵ�к�������                                          ///
///                                                                      ///
///                      WINDOWS���뻷������                             ///
////////////////////////////////////////////////////////////////////////////
#ifdef WIN32

//**********************************
//Description: ԭ�Ӳ���-����
// Parameter:  [IN] PLONG  Addend Ҫ��������
// Returns:    LONG  32λ�з�������
//
//************************************
LONG CAdapter::InterlockedIncrement(IN PLONG  Addend)
{
    return ::InterlockedIncrement(Addend);
}

//**********************************
//Description: ԭ�Ӳ���-�ݼ�
// Parameter:  [IN] PLONG  Addend ��long*)
// Returns:    LONG  32λ�з�������
//************************************
LONG CAdapter::InterlockedDecrement(IN PLONG  Addend)
{
    return ::InterlockedDecrement(Addend);
}

//**********************************
//Description: ԭ�Ӳ���-����Addend ��Value���
// Parameter:  IN OUT PLONG  Addend,
//             IN LONG  Value
// Returns:    LONG  Addend ���ǰ��ֵ
//************************************
LONG CAdapter::InterlockedExchangeAdd(IN OUT PLONG  Addend,IN LONG  Value)
{
    return ::InterlockedExchangeAdd(Addend, Value);
}

//**********************************
//Description: ԭ�Ӳ���-�ȽϽ���
// Parameter:  [IN] [OUT] PLONG  Destination ��long*)
//             [IN] LONG  Exchange
//             [IN] LONG  Comparand
// Returns:    LONG  32λ�з�������
//************************************
LONG CAdapter::InterlockedCompareExchangeEx(
                IN OUT PLONG  Destination,
                IN LONG  Exchange,
                IN LONG  Comparand
                )
{
    //return InterlockedCompareExchange(Destination,Exchange,Comparand);
    return 0;
}

//**********************************
//Description: ԭ�Ӳ���-����
// Parameter:  [IN] [OUT] PLONG  Target ��long*)
//             [IN] LONG  Value
// Returns:    LONG  32λ�з�������
//************************************
LONG CAdapter::InterlockedExchange(
                IN OUT PLONG  Target,
                IN LONG  Value
                )
{
    return ::InterlockedExchange(Target,Value);
}

////////////////////////////////////////////////////////////////////////////
///        ԭ�Ӳ���ϵ�к�������                                          ///
///                                                                      ///
//                      linux���뻷������                                 //
////////////////////////////////////////////////////////////////////////////
#elif defined LINUX

//**********************************
//Description: ԭ�Ӳ���-����
// Parameter:  [IN] PLONG  Addend Ҫ��������
// Returns:    LONG  32λ�з�������
//
//************************************
LONG CAdapter::InterlockedIncrement(IN PLONG  Addend)
{
    //return __gnu_cxx::__exchange_and_add((_Atomic_word*)Addend, 1);
    __gnu_cxx::__atomic_add((_Atomic_word*)Addend,1);
    //__atomic_add((_Atomic_word*)Addend,1);
    return *Addend;
    //return InterlockedIncrement(Addend);
    //return __gnu_cxx::__exchange_and_add((_Atomic_word*)Addend, 1);
}

//**********************************
//Description: ԭ�Ӳ���-�ݼ�
// Parameter:  [IN] PLONG  Addend ��long*)
// Returns:    LONG  32λ�з�������
//************************************
LONG CAdapter::InterlockedDecrement(IN PLONG  Addend)
{
    __gnu_cxx::__atomic_add((_Atomic_word*)Addend,-1);
    //__atomic_add((_Atomic_word*)Addend,-1);
    return *Addend;
    //return InterlockedDecrement(Addend);
    //return __gnu_cxx::__exchange_and_add((_Atomic_word*)Addend, -1);
}

//**********************************
//Description: ԭ�Ӳ���-����Addend ��Value���
// Parameter:  IN OUT PLONG  Addend,
//             IN LONG  Value
// Returns:    LONG  Addend ���ǰ��ֵ
//************************************
LONG CAdapter::InterlockedExchangeAdd(IN OUT PLONG  Addend,IN LONG  Value)
{
    return __gnu_cxx::__exchange_and_add((_Atomic_word*)Addend, Value);
    //return __exchange_and_add((_Atomic_word*)Addend, Value);
}

//**********************************
//Description: ԭ�Ӳ���-�ȽϽ���
// Parameter:  [IN] [OUT] PLONG  Destination ��long*)
//             [IN] LONG  Exchange
//             [IN] LONG  Comparand
// Returns:    LONG  32λ�з�������
//************************************
LONG CAdapter::InterlockedCompareExchangeEx(
                IN OUT PLONG  Destination,
                IN LONG  Exchange,
                IN LONG  Comparand
                )
{
    //return ::InterlockedCompareExchange(Destination,Exchange,Comparand);
    //return __sync_val_compare_and_swap(Destination, Exchange, Comparand);
    return 0;
}

//**********************************
//Description: ԭ�Ӳ���-����
// Parameter:  [IN] [OUT] PLONG  Target ��long*)
//             [IN] LONG  Value
// Returns:    LONG  32λ�з�������
//************************************
LONG CAdapter::InterlockedExchange(
                IN OUT PLONG  Target,
                IN LONG  Value
                )
{
    return __gnu_cxx::__exchange_and_add((_Atomic_word*)Target, Value-*Target);

    //return __exchange_and_add((_Atomic_word*)Target, Value-*Target);
}

#endif


//**********************************
//Description: дINI�����ļ� д���ַ���
// Parameter:  ��windows�ӿ�һ�£�����MSDN
// Returns:    bool
// Note��      linux�´�Сд����
//************************************
BOOL CAdapter::WritePrivateProfileString( LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpString, LPCSTR lpFileName )
{
#ifdef WIN32
	return ::WritePrivateProfileString(lpAppName, lpKeyName, lpString, lpFileName);
#elif defined LINUX	
	char szsection[100] = {0};
	char szentry[100] = {0};
	char sztmp[MAXLINELENGTH] = {0};
	
	sprintf(szsection, "[%s]", lpAppName);
	sprintf(szentry , "%s=" , lpKeyName);
	
	//read file conn
	if (!PathFileExists(lpFileName))
	{
		//file not exist
		FILE* pfile = fopen(lpFileName, "w");
		if (!pfile)
		{
			return false;
		}
		sprintf(sztmp, "%s\n%s%s\n", szsection, szentry, lpString);
		fwrite(sztmp, sizeof(char), strlen(sztmp), pfile);
		fclose(pfile);
		return true;
	}
	std::string strConn = "";
	std::string strRow = "";
	size_t nAppPos = std::string::npos;
	size_t nKeyPos = 0;
	bool bfindapp = true;
	bool bfindkey = false;
	FILE* pfile = NULL;
	pfile = fopen(lpFileName, "r");
	
	while (!feof(pfile))
	{
		memset(sztmp, 0, sizeof(sztmp));
		ReadString(sztmp, MAXLINELENGTH, pfile);		
		RemoveSpace(sztmp);
		RemoveChar(sztmp, '\n');
		RemoveChar(sztmp, '\r');
		strRow = sztmp;
		if (nAppPos == std::string::npos && (nAppPos = strRow.find(szsection)) == 0)
		{	
			strConn += szsection;
			strConn += "\n";
			nAppPos = strConn.length();	
			nKeyPos = std::string::npos;
		}
		else if (nKeyPos == std::string::npos && (nKeyPos = strRow.find(szentry)) == 0)
		{			
			strConn += szentry;
			strConn += lpString;
			strConn += "\n";
		}
		else
		{
			strConn += strRow;
			strConn += "\n";
		}		
	}
	if (nAppPos == std::string::npos && nKeyPos == 0)
	{
		memset(sztmp, 0, sizeof(sztmp));
		sprintf(sztmp, "%s\n%s%s\n", szsection, szentry, lpString);
		strConn += sztmp;
	}
	else if (nAppPos != std::string::npos && nKeyPos == std::string::npos)
	{
		std::string strBack = strConn.substr(nAppPos);
		strConn = strConn.substr(0, nAppPos);
		strConn += szentry;
		strConn += lpString;
		strConn += "\n" + strBack;
	}
	fclose(pfile);
	pfile = fopen(lpFileName, "w");	
	fwrite(strConn.c_str(), sizeof(char), strConn.length(), pfile);
	fclose(pfile);
	return true;
#endif
}

//**********************************
//Description: ��INI�����ļ� ��ȡint����
// Parameter:  ��windows�ӿ�һ�£�����MSDN
// Returns:    UINT
// Note��      linux�´�Сд����
//************************************
UINT CAdapter::GetPrivateProfileInt(
          LPCTSTR lpAppName,  // section name
          LPCTSTR lpKeyName,  // key name
          INT nDefault,       // return value if key name not found
          LPCTSTR lpFileName  // initialization file name
           )
{
    int iRet = -1;
//WINDOWS���뻷��
#ifdef WIN32
//linux���뻷��
    return ::GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, lpFileName);
#elif defined LINUX

    short bfindsection = false;
	short bfindkey = false;
    char szsection[100] = {0};
    char szentry[100] = {0};
    char sztmp[MAXLINELENGTH] = {0};
    char szresult[MAXLINELENGTH] = {0};
    //ȥ�ҿո�
    //TrimRight(lpAppName);
    //TrimRight(lpKeyName);
    //TrimRight(lpFileName);
    if(false == CAdapter::PathFileExists(lpFileName))
    {
        return nDefault;
    }

    sprintf(szsection , "[%s]" , lpAppName);
    sprintf(szentry , "%s" , lpKeyName);

    //��ָ����INI�����ļ�
    FILE* stream;
    stream = fopen( lpFileName, "r" );
    if ( stream != NULL )
    {
        //�ҵ�section
        while (!feof(stream) && !bfindsection)
        {
            memset(sztmp,0x00,MAXLINELENGTH);
            fscanf(stream , "%s" , sztmp);
            bfindsection = !strcmp(sztmp,szsection);
        }

        memset(sztmp,0x00,MAXLINELENGTH);
        //�ҵ�entry
        while (!feof(stream) && bfindsection && sztmp[0] != '[')
        {
            memset(sztmp,0x00,MAXLINELENGTH);
            ReadString(sztmp , MAXLINELENGTH, stream);
            RemoveChar(sztmp,' ');

            if (strncmp(sztmp,szentry,strlen(szentry)) == 0)
            {
				bfindkey = true;
                strncpy(szresult , &sztmp[strlen(szentry)], sizeof(szresult));
                RemoveChar(szresult,'=');
                RemoveChar(szresult,'\n');
                RemoveChar(szresult,'\r');
                iRet=atoi(szresult);
                if(iRet < 0)
                {
                    //Ϊ����windows��API GetPrivateProfileInt������
                    //If the value of the key is less than zero,
                    //the return value is zero(from MSDN GetPrivateProfileInt)
                    iRet = 0;
                }
                break; //�ҵ�lpKeyName ��������
            }
        }
    }
    //����ļ���ʧ�� ��ڵ�δ�ҵ� ����δ�ҵ�ʱ������APIĬ��ֵ
    if((NULL == stream) || !bfindsection || (bfindkey == false) || (!feof(stream) && iRet == -1))
    {
        iRet = nDefault;
    }

    if(stream != NULL)
    {
        fclose( stream );
        stream = NULL;
    }
    return iRet;

#endif
}

//**********************************
//Description: ��INI�����ļ� ��ȡ�ַ���
// Parameter:  ��windows�ӿ�һ�£�����MSDN
// Returns:    DWORD
// Note��      linux�´�Сд����
//************************************
DWORD CAdapter::GetPrivateProfileString(
          LPCTSTR lpAppName,        // section name
          LPCTSTR lpKeyName,        // key name
          LPCTSTR lpDefault,        // default string
          LPTSTR lpReturnedString,  // destination buffer
          DWORD nSize,              // size of destination buffer
          LPCTSTR lpFileName        // initialization file name
           )
{
    int iRet = -1;
    //WINDOWS���뻷��
#ifdef WIN32
    return ::GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, lpFileName);
//linux���뻷��
#elif defined LINUX
    short bfindsection = false;
	short bfindkey = false;
    char szsection[100] = {0};
    char szentry[100] = {0};
    char sztmp[MAXLINELENGTH] = {0};
    char sztmpb[MAXLINELENGTH] = {0};
    char szresult[MAXLINELENGTH] = {0};
    memset(lpReturnedString, 0, strlen(lpReturnedString));
    //ȥ�ҿո�
    //TrimRight(lpAppName);
    //TrimRight(lpKeyName);
    //TrimRight(lpFileName);
    if(false == CAdapter::PathFileExists(lpFileName))
    {
        strcpy(lpReturnedString,lpDefault);
        return strlen(lpDefault);
    }

    sprintf(szsection , "[%s]" , lpAppName);
    sprintf(szentry , "%s=" , lpKeyName);

    FILE* stream;
    stream = fopen( lpFileName, "r" );
    if( stream != NULL )
    {
        //�ҵ�pcsection
        while(!feof(stream) && !bfindsection)
        {
            memset(sztmp,0x00,MAXLINELENGTH);
            fscanf(stream , "%s" , sztmp) ;
            bfindsection = !strcmp(sztmp , szsection) ;
        }

        memset(sztmp,0x00,MAXLINELENGTH);
        while (!feof(stream) && bfindsection && sztmp[0] != '[')
        {
            memset(sztmp,0x00,sizeof(sztmp));
            ReadString(sztmp , MAXLINELENGTH, stream);
            //RemoveChar(sztmp,' ');
            //printf("%s\n", sztmp);
            RemoveSpace(sztmp);
            //printf("%s\n\n", sztmp);
            strncpy(sztmpb, sztmp, sizeof(sztmpb));
            if (strncmp(sztmp,szentry,strlen(szentry)) == 0)
            {
				bfindkey = true;
                strncpy(szresult , &sztmpb[strlen(szentry)], sizeof(szresult)-1);
                //RemoveChar(szresult,'=');
                RemoveChar(szresult,'\n');
                RemoveChar(szresult,'\r');
                if(nSize < strlen(szresult) && lpReturnedString != NULL)
                {
                    strncpy(lpReturnedString, szresult, nSize-1);
                    lpReturnedString[nSize-1] = '\0';
                    iRet = nSize-1;
                }
                else if(lpReturnedString != NULL)
                {
                    strncpy(lpReturnedString, szresult, strlen(szresult));
                    iRet = strlen(lpReturnedString);
                }

                break;
            }
        }
    }
    //ȡĬ��ֵ�������
    //1.���ļ�ʧ��
    //2.AppNameδ�ҵ�
    //3.KeyNameδ�ҵ�
    if(((NULL == stream) || !bfindsection || (bfindkey == false) || (!feof(stream) && iRet==-1)) && (lpDefault != NULL))
    {
        strncpy(lpReturnedString, lpDefault, nSize);
        iRet = strlen(lpDefault);
    }
    if(stream != NULL)
    {
        fclose( stream );
        stream = NULL;
    }
#endif
    return iRet;
}

//**********************************
//Description: ��ʼ��socket����
// Parameter:  void
// Returns:    BOOL
//************************************
BOOL CAdapter::InitSocket(void)
{
    WORD wVersionRequired;
    WSADATA wsaData;
    wVersionRequired = MAKEWORD(2,2);

    int err = WSAStartup(wVersionRequired, &wsaData);
    if (0 != err)
    {
        return FALSE;
    }
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        //support
        WSACleanup();
        return FALSE;
    }
    return TRUE;
}

//**********************************
//Description: ��ֹsocket����
// Parameter:  void
// Returns:    BOOL
//************************************
BOOL CAdapter::UninitSocket(void)
{
    if(0 == WSACleanup())
    {
        return TRUE;
    }
    return FALSE;
}

//**********************************
//Description: �ر�socket�׽���
// Parameter:  SOCKET& sockfd
// Returns:    int �ɹ�����0
//************************************
int CAdapter::CloseSocket(SOCKET& sockfd)
{
    return closesocket(sockfd);
}

//**********************************
//Description: �ر�socket�׽���
// Parameter:  SOCKET s
// Returns:    BOOL
//************************************
int CAdapter::closesocket(SOCKET s)
{
#ifdef WIN32
    return ::closesocket(s);
#elif defined LINUX
    return close(s);
#endif
}

//**********************************
//Description: ��ʼ�����绷������
// Parameter:  WORD wVersionRequested,LPWSADATA lpWSAData
// Returns:    int
//************************************
int CAdapter::WSAStartup(WORD wVersionRequested,LPWSADATA lpWSAData)
{
    int iRet = 0;
#ifdef WIN32
    iRet = ::WSAStartup(wVersionRequested, lpWSAData);
#elif defined LINUX
    lpWSAData->wVersion = wVersionRequested;
    lpWSAData->wHighVersion = wVersionRequested;
#endif
    return iRet;
}

//**********************************
//Description: ��ֹsocket����
// Parameter:  void
// Returns:    int �ɹ�����0
//************************************
int CAdapter::WSACleanup(void)
{
    int iRet = 0;
#ifndef LINUX
    iRet = ::WSACleanup();
#endif
    return iRet;
}

//**********************************
//Description: �ַ�����Сдת��
// Parameter:  char* szStr
// Returns:    char*
//************************************
char* CAdapter::strupr(char *szStr)
{
    if (NULL == szStr)
    {
        return NULL;
    }
    for (int i = 0; i < strlen(szStr); i++)
    {
        if (szStr[i] >= 'a' && szStr[i] <= 'z')
        {
            szStr[i] = toupper(szStr[i]);
        }
    }

    return szStr;
}

char* CAdapter::strlow(char *szStr)
{
    if (NULL == szStr)
    {
        return NULL;
    }
    for (int i = 0; i < strlen(szStr); i++)
    {
        if (szStr[i] >= 'a' && szStr[i] <= 'z')
        {
            szStr[i] = tolower(szStr[i]);
        }
    }

    return szStr;
}

//protected function
//ȥ���ַ�����ָ�����ַ�
void CAdapter::RemoveChar(char *from, char Del)
{
    char *End = from;
    if (from)
    {
        for ( ; *from; ++from )
        {
            if ( *from == Del ) break;
        }
        //�ҵ���һ���ո�
        End = from;
        if ( *from == Del )
        {
            //����ҵ��˿ո�
            for ( ++from; *from; ++from )
            {
                //��from�Ժ�Ĳ����ڿո�Ĵ��ݸ�End
                if ( *from != Del ) *(End++) = *from;
            }
        }
    }
    //�ַ����µ�ĩβ
    if (End) *End = '\0';
}

//����ȥ��ָ���ַ����Ŀո񣬵�'='֮��Ŀո�ȥ��
void CAdapter::RemoveSpace(char *from)
{
    char Del = ' ';
    char *pEqual = NULL;
    char *pPreEqual = NULL;
    char *pAftEqual = NULL;
    char *pTemp = from;
    char *pEnd = from;

    if(pTemp)
    {
        for(;*pTemp ; ++pTemp)
        {
            if(*pTemp == ' ')
            {
                pPreEqual = (pPreEqual==NULL)?pTemp:pPreEqual;      //�ҵ��Ⱥ�֮ǰ�Ŀո�
            }
            if(*pTemp == '=')
            {
                pEqual = pTemp;
                pAftEqual = pEqual;
                break;
            }
        }

        if(*pTemp)
        {
            for(pTemp++ ; (*pTemp == ' '); pAftEqual=(pTemp++));    //�ҵ��Ⱥ�֮��ո�Ľ���λ��
        }

        //pEnd = (pPreEqual==NULL)?((pEqual==NULL)?NULL:(pEqual+1)):pPreEqual;          //ȷ����һ���ո��λ��
        pEnd = (pPreEqual==NULL)?((pEqual==NULL)?NULL:(pEqual+1)):((pEqual==NULL)?NULL:pPreEqual);

        //�ҵ���һ���ո�
        if(pEnd != NULL)
        {
            from = pEnd;
            if ( *from == Del )
            {
                //����ҵ��˿ո�
                for ( ++from; *from; ++from )
                {
                    //��from�Ժ�Ĳ����ڿո�Ĵ��ݸ�End
                    if(from > pAftEqual)
                    {
                        *(pEnd++) = *from;
                    }
                    else if ( *from != Del ) *(pEnd++) = *from;
                }
                *pEnd = '\0';
            }
        }
    }
}

//ȥ��ո�
void CAdapter::TrimLeft(char *pchScr)
{
    char *szTmp = pchScr;
    while (*szTmp == 32)szTmp++;

    strcpy(pchScr, szTmp);
}

void CAdapter::GlobalMemoryStatus(LPMEMORYSTATUSEX lpBuffer)
{
#ifdef WIN32
    ::GlobalMemoryStatus((LPMEMORYSTATUS)lpBuffer);
#elif defined LINUX
    memset(lpBuffer, 0, sizeof(MEMORYSTATUSEX));
#endif
}

void CAdapter::TrimRight(char *pchScr)
{
    if (pchScr != NULL)
    {
        int nLen = strlen(pchScr);
        while (nLen > 0)
        {
            if (pchScr[nLen -1] != 32)
            {
                break;
            }
            else
            {
                pchScr[nLen -1] = '\0';
                --nLen;
            }
        }
    }
}

char* CAdapter::ReadString(char *string, int n, FILE *stream)
{
    if(NULL == stream && NULL == string && n < 0)
    {
        return NULL;
    }

    char *pRet = NULL;
    char szpBuff[1024+1] = {0};

    if (NULL != (pRet = fgets(string, n, stream)))
    {
        if ('\n' != string[strlen(string) - 1])
        {
            while(fgets(szpBuff, sizeof(szpBuff), stream))
            {
                if('\n' == szpBuff[strlen(szpBuff) - 1])
                {
                    break;
                }
                else
                {
                    memset(szpBuff, 0, sizeof(szpBuff));
                }
            }
        }
    }

    return  pRet;
}


