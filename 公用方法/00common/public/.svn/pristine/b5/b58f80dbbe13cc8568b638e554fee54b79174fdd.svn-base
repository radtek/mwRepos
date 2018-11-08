Modify By jianghb 2011-04-15
若不需要使用MFC框架请包含宏NO_USE_MFC

Modify By liuhuan 2011-04-13
新增适配接口：
1.字符转换接口
    //Description: 转换为宽字符模式
    // Parameter:  参照MSDN (与windows API定义一致，现支持GBK，UTF-8)
    static int MultiByteToWideChar(
            UINT CodePage,         // code page
            DWORD dwFlags,         // character-type options
            LPCSTR lpMultiByteStr, // string to map
            int cbMultiByte,       // number of bytes in string
            LPWSTR lpWideCharStr,  // wide-character buffer
            int cchWideChar        // size of buffer
            );

    //Description: 转换为多字节模式
    // Parameter:  参照MSDN  (与windows API定义一致，现支持GBK，UTF-8)
    static int WideCharToMultiByte(
            UINT CodePage,            // code page
            DWORD dwFlags,            // performance and mapping flags
            LPCWSTR lpWideCharStr,    // wide-character string
            int cchWideChar,          // number of chars in string
            LPSTR lpMultiByteStr,     // buffer for new string
            int cbMultiByte,          // size of buffer
            LPCSTR lpDefaultChar,     // default for unmappable chars
            LPBOOL lpUsedDefaultChar  // set when default char used
            );
2.原子锁接口：
    //Description: 原子操作-递增  (现需要在支持__gnu_cxx::命名空间)
    // Parameter:  [IN] PLONG  Addend 要递增的数
    // Returns:    LONG  32位有符号整数
    static LONG InterlockedIncrement(IN PLONG  Addend);

    //Description: 原子操作-递减   （现需要在支持__gnu_cxx::命名空间）
    // Parameter:  [IN] PLONG  Addend （long*)
    // Returns:    LONG  32位有符号整数
    static LONG InterlockedDecrement(IN PLONG  Addend);

Add by liuhuan 2011-04-11
1.linux平台编译时直接使用makefile 接口使用请参考test.cpp
#如make SYS=-DLINUX  会生产test的可执行文件   SYS=-DLINUX为设置编译环境为linux 平台
#支持windows平台时，请在vc6做设置：Project->setting -> C/C++ ->Category中选择General  在Project options加上 /D "WINDOWS_COMPILER"

Modify By liuhuan 2011-04-12
增加适配接口：
    //Description: 适配windows下的Sleep和linux下的sleep（参数前者是毫秒，后者是秒）
    static void Sleep(DWORD dwMilliseconds);

    //Description: 获取当前的系统时间
    // Parameter:  LPSYSTEMTIME_EX lpSystemTime_ex 自定义的拓展系统时间
    //              和windows下SYSTEMTIME成员一致，数据类型改为32位int
    static void GetLocalTime(LPSYSTEMTIME_EX lpSystemTime_ex);

    //Description: 检测文件或目录是否存在
    // Parameter:  LPCTSTR pszPath 指定目录
    static BOOL PathFileExists(LPCTSTR pszPath);

    //Description: 创建目录
    // Parameter:  LPCTSTR lpPathName 要创建的目录名
    // Returns:    BOOL
    // Note：      创建多级目录和已经存在的目录会失败
    static BOOL CreateDirectory(LPCTSTR lpPathName);
note:函数原型基本与windows一致，新增时间结构体SYSTEMTIME_EX

Modify By liuhuan 2011-04-13
新增接口：（用于处理在linux下一次SetEvent函数可能调用两次的情况）
    //Description: 重置事件对象为无信号
    // Parameter:  CCompatibleEvent &event
    static void ResetCompatibleEvent(CCompatibleEvent &event);

