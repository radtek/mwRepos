#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include "utility/common/adapter/Adapter.h"
#include "utility/common/adapter/CompatibleEvent.h"
#include "utility/common/adapter/CompatibleThread.h"

using namespace std;

void *ThreadTest(void *arg);
class CAdapterTest;

//Adapter类测试类
class CAdapterTest
{
public:
    CAdapterTest();
    ~CAdapterTest();

    void Run();                                     //测试类得主运行函数
    static THREAD_RETURN __STDCALL ThreadTest1(void *arg);    //创建线程时用的线程函数
    static THREAD_RETURN __STDCALL ThreadTest2(void *arg);    //创建线程时用的线程函数
private:
    CCompatibleEvent m_cEvent;      //创建一个兼容事件类得对象
};

CAdapterTest::CAdapterTest()
{

}

CAdapterTest::~CAdapterTest()
{

}

//测试类得主运行函数
void CAdapterTest::Run()
{
    CAdapter::InitCompatibleEvent(m_cEvent);                //初始化事件对象   CreateEvent()

    //创建两个线程用于测试事件对象
    CAdapter::CreateCompatibleThread(ThreadTest1, this);    //创建一个线程，参数1 线程函数指针  参数2 传给线程的参数
    CAdapter::CreateCompatibleThread(ThreadTest2, this);
    sleep(20);

    //销毁  CloseHandle()
    CAdapter::UnInitCompatibleEvent(m_cEvent);
}

//线程2的函数
THREAD_RETURN __STDCALL CAdapterTest::ThreadTest2(void *arg)
{
    int iRet;
    CAdapterTest *thread1 = (CAdapterTest*)arg;
    cout<<"thread2 begin waite Event"<<endl;
    sleep(1);

    //调用接口等待事件发生
//     iRet = CAdapter::WaitEvent(thread1->m_cEvent, 5 * 1000);
    cout<<"iRet"<<iRet<<endl;
    if(1 == iRet)
    {
        cout<<"超时"<<endl;
    }
    else if(-1 == iRet)
    {
        cout<<"错误"<<endl;
    }
    else if(0 == iRet)
    {
        cout<<"有信号"<<endl;
    }

    cout<<endl<<endl;

    //第二次调用接口等待事件发生
//     iRet = CAdapter::WaitEvent(thread1->m_cEvent, 5 * 1000);
    cout<<"iRet:"<<iRet<<endl;
    if(1 == iRet)
    {
        cout<<"超时"<<endl;
    }
    else if(-1 == iRet)
    {
        cout<<"错误"<<endl;
    }
    else if(0 == iRet)
    {
        cout<<"有信号"<<endl;
    }

    return NULL;
}


//线程1函数用于设置事件信号
THREAD_RETURN __STDCALL CAdapterTest::ThreadTest1(void *arg)
{
    CAdapterTest *thread2 = (CAdapterTest*)arg;
    cout<<"thread1 begin send signal"<<endl;
    //sleep(1);
    //调用适配接口SetCompatibleEvent  设置事件  SetEvent
    CAdapter::SetCompatibleEvent(thread2->m_cEvent);
    cout<<"thread end"<<endl;
    return NULL;
}




//测试PathFileExists
void PathFileExistsTest()
{
    char *path1 = "/etc";               //检测存在的目录
    char *path2 = "/etcabc";        //检测不存在的目录或文件

    char *file1 = "/etc/passwd";            //检测存在的文件
    char *file2 = "/etc/passwdabc";         //检测不存在的目录或文件

    if(TRUE == CAdapter::PathFileExists(path1))
    {
        printf("path1 OK\n");
    }
    else
    {
        printf("path1 FAILED\n");
    }

    if(TRUE == CAdapter::PathFileExists(path2))
    {
        printf("path2 OK\n");
    }
    else
    {
        printf("path2 FAILED\n");
    }

    if(TRUE == CAdapter::PathFileExists(file1))
    {
        printf("file1 OK\n");
    }
    else
    {
        printf("file1 FAILED\n");
    }
    if(TRUE == CAdapter::PathFileExists(file2))
    {
        printf("file2 OK\n");
    }
    else
    {
        printf("file2 FAILED\n");
    }
}

//测试CreateDirectory
void CreateDirectoryTest()
{
    char * path1 = "/temp/dir1";                //创建目录dir1
    char * path2 = "/temp/dir2/dir3/dir4";      //创建多级目录
    char * path3 = "/temp/中文dir";             //创建含有中文的目录
    char * path4 = "/temp";                     //创建已经存在的目录

    if(CAdapter::CreateDirectory(path1,0))
    {
        cout<<"Create path1 is OK"<<endl;
    }
    else
    {
        cout<<"Create path1 is failed"<<endl;
    }

    if(CAdapter::CreateDirectory(path2,0))
    {
        cout<<"创建多级目录成功"<<endl;
    }
    else
    {
        cout<<"创建多级目录失败"<<endl;
    }

    if(CAdapter::CreateDirectory(path3,0))
    {
        cout<<"创建中文目录成功"<<endl;
    }
    else
    {
        cout<<"创建中文目录失败"<<endl;
    }

    if(CAdapter::CreateDirectory(path4,0))
    {
        cout<<"创建已经存在的目录成功"<<endl;
    }
    else
    {
        cout<<"创建已经存在的目录失败"<<endl;
    }

}


//测试GetLocalTime
void GetLocalTimeTest()
{
    SYSTEMTIME_EX now;
    CAdapter::GetLocalTime(&now);

    cout<<now.m_iYear<<":"<<now.m_iMonth <<":"<<now.m_iDay <<":"
        <<now.m_iHour <<":"<<now.m_iMinute <<":"<<now.m_iSecond <<":"<<now.m_iMilliseconds <<"   "<<now.m_iDayOfWeek<<endl;

}

//测试宽字符和多字节之间的转换
void MtoWTest()
{
    wchar_t wcWideCharStr[4][30] = {L"abc",L"",L"",L""};
    char szMultiByteStr[4][70] = {{0}};

    int WtoMRet = 0;
    int MtoWRet = 0;

    memset(wcWideCharStr[1],0,3*30*sizeof(wchar_t));

    int iLoop;
    for(iLoop = 0;iLoop < 3;iLoop++)
    {

        WtoMRet = 0;
        MtoWRet = 0;

        cout<<"转换前宽字符 iLoop :"<<iLoop<<"  "<<wcWideCharStr[iLoop]<<endl;
        WtoMRet = CAdapter::WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,wcWideCharStr[iLoop],
                                                wcslen(wcWideCharStr[iLoop]),szMultiByteStr[iLoop],70,NULL,NULL);

        cout<<"转换后多字节 iLoop :"<<iLoop<<"  "<<szMultiByteStr[iLoop]<<endl;

        MtoWRet = CAdapter::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szMultiByteStr[iLoop],WtoMRet,
                                      wcWideCharStr[iLoop+1],30);

//        cout<<"转换前宽字符 iLoop :"<<iLoop<<"  "<<wcWideCharStr[iLoop]<<endl;

        cout<<endl;
    }

}

//原子锁接口测试
void InterlockedTest()
{
    long a = 1;
    long b = 1;
    long c = 1;
    long d = 1;

    long lReta = 0;
    long lRetb = 0;
    long lRetc = 0;
    long lRetd = 0;

    lReta = CAdapter::InterlockedIncrement(&a);

    cout<<"lReta :"<<lReta<<"\t a :"<<a<<endl;

    lRetb = CAdapter::InterlockedDecrement(&b);

    cout<<"lRetb :"<<lRetb<<"\t b :"<<b<<endl;

    lRetc = CAdapter::InterlockedExchange(&c, 2);

    cout<<"lRetc :"<<lRetc<<"\t c :"<<c<<endl;

    lRetd = CAdapter::InterlockedCompareExchangeEx(&d, 2, 3);

    cout<<"lRetd :"<<lRetd<<"\t d :"<<d<<endl;
}

//读INI配置文件接口测试
void ReadIniProfileTest()
{
    /*
    char *lpFileName = "cmppgw.ini";
    char *lpAppName1 = "ReSendOpt";
    char *lpKeyName1 = "ReSendCnt";
    int iRet1 = 0;

    char *lpAppName2 = "WebService";
    char *lpKeyName2 = "WbsPackNu";
    int iRet2 = 0;

    char *lpAppName3 = "OtherSetting";
    char *lpKeyName3 = "AppTitle";
    int iRet3 = 0;
    char szRet1[260] = {0};

    char *lpAppName4 = "LoginDB";
    char *lpKeyName4 = "ConnectString";
    int iRet4 = 0;
    char szRet2[260] = {0};

    iRet1 = CAdapter::GetPrivateProfileInt(lpAppName1, lpKeyName1, 12, lpFileName);
    iRet2 = CAdapter::GetPrivateProfileInt(lpAppName2, lpKeyName2, 23, lpFileName);

    iRet3 = CAdapter::GetPrivateProfileString(lpAppName3, lpKeyName3, "szRet1", szRet1, 260, lpFileName);
    iRet4 = CAdapter::GetPrivateProfileString(lpAppName4, lpKeyName4, "szRet2", szRet2, 260, lpFileName);

    cout<<"iRet1 : "<<iRet1<<endl;
    cout<<"iRet2 : "<<iRet2<<endl;
    cout<<"szRet1 : "<<szRet1<<"\tiRet3 : "<<iRet3<<endl;
    cout<<"szRet2 : "<<szRet2<<"\tiRet4 : "<<iRet4<<endl;
*/

    char szPath[200] = "cmppgw.ini";
    char szTmp[260] = {0};

    CAdapter::GetPrivateProfileString("OtherSetting", "AppTitle", "CMPP短信网关", szTmp, 260, szPath);
    printf("%s\n",szTmp);
    //GetPrivateProfileString("OtherSetting", "AppTitle", "CMPP短信网关", szTmp, 260, szPath);
    //memset(szTmp, 0, 260);
    printf("%s\n",szTmp);
    CAdapter::GetPrivateProfileString("ServerSetting", "VerifyBegin", "00:00:00", szTmp, 260, szPath);
    printf("%s\n",szTmp);
    //GetPrivateProfileString("OtherSetting", "LogPath", "", szTmp, MAX_PATH, szPath);
    printf("%s\n",szTmp);
    CAdapter::GetPrivateProfileString("DataBase", "ConnectString", "", szTmp, 260, szPath);
    printf("%s\n",szTmp);
    //GetPrivateProfileString("DataBase", "ConnectString", "", szTmp, MAX_PATH, szPath);
    printf("%s\n",szTmp);
}

//去除等号'='两边的空格
void RemoveSpaceTest()
{
    char str1[30] = "abc  =  ec d a";
    char str2[30] = " abc=  ec d a";
    char str3[30] = "a bc  =ec d a";
    char str4[30] = " abc   =ec d a";
    char str5[30] = "\r\n";
    char str6[30] = "ab dc";
    char str7[30] = "abc=ecda ";

/*
    CAdapter::RemoveSpace(str1);
    CAdapter::RemoveSpace(str2);
    CAdapter::RemoveSpace(str3);
    CAdapter::RemoveSpace(str4);
    CAdapter::RemoveSpace(str5);
    CAdapter::RemoveSpace(str6);
    CAdapter::RemoveSpace(str7);
*/

    printf("str1 %s\n", str1);
    printf("str2 %s\n", str2);
    printf("str3 %s\n", str3);
    printf("str4 %s\n", str4);
    printf("str5 %s\n", str5);
    printf("str6 %s\n", str6);
    printf("str6 %s\n", str7);
}

//WSA函数测试
void WSAsocketTest()
{
    WORD wVersionRequired;
    WSADATA wsaData;
    wVersionRequired = MAKEWORD(2,2);

    int err = CAdapter::WSAStartup(wVersionRequired, &wsaData);
    if (0 != err)
    {
        cout<<"WSAStartup Error"<<endl;
        return ;
    }
    unsigned char lbyte = LOBYTE(wsaData.wVersion);
    unsigned char hbyte = HIBYTE(wsaData.wVersion);
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        cout<<"LOBYTE HIBYTE Error"<<endl;
        //support
        CAdapter::WSACleanup();
        return ;
    }

    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        cout<<"socket error"<<endl;
    }

    CAdapter::closesocket(sockfd);

    CAdapter::WSACleanup();
}


int main(int argc, char *argv[])
{
    CCompatibleEvent event_test;
    CAdapter::InitCompatibleEvent(event_test);
    CAdapter::SetCompatibleEvent(event_test);
//     CAdapter::WaitEvent(event_test, 0);
    while (1)
    {
//         CAdapter::WaitEvent(event_test, 0);
        printf("waite 2s...\r\n");
    }
    //WSA函数测试
    //WSAsocketTest();

//去除等号'='两边的空格
//    RemoveSpaceTest();
    //读INI配置文件接口测试
//    ReadIniProfileTest();
    //原子锁接口测试
//    InterlockedTest();
    /*
    CCompatibleEvent ce;

    CAdapter::InitCompatibleEvent(ce);

    CAdapter::SetCompatibleEvent(ce);

    CAdapter::WaitForCompatibleEvent(ce, 5 * 1000);
    cout<<"test..."<<endl;

    CAdapterTest adaptTest;
    adaptTest.Run();
    */

/*
    wchar_t wcStr[20] = L"abc有";
    char szStr[100] = {0};
    int iRet = 0;

    cout<<"wcStr :"<<wcStr<<endl;

    unsigned int CP_ACP = 0;
    long unsigned WC_COMPOSITECHECK = 0;

    iRet = CAdapter::WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,wcStr,20,szStr,100,NULL,NULL);

    cout<<"iRet :"<<iRet<<endl;
    cout<<"szStr :"<<szStr<<endl;
*/

//测试PathFileExists
//    PathFileExistsTest();

//测试CreateDirectory
//    CreateDirectoryTest();

//测试GetLocalTime
//    GetLocalTimeTest();

//测试宽字符和多字节之间的转换
//    MtoWTest();
    return 0;
}


