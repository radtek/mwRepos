// ParaNoMgrImpl.cpp: implementation of the CParaNoMgrImpl class.
//
//////////////////////////////////////////////////////////////////////

#include <paranomgr/ParaNoMgrImpl.h>
#include <paranomgr/AccessMssql.h>
#include "Global.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char g_countryList[MAX_COUNTRY];          //存放国家区号数组
char g_DBcountryList[MAX_COUNTRY];        //存放数据库导出数据数组
int  g_arrCountryList[] = {1, 1264, 1268, 1242, 1246, 1441, 1284, 1345, 1767, 1809, 1473, 1876, 1664, 1721,
                           1787, 1939, 1869, 1758, 1784, 1868, 1649, 1340, 1671, 1670, 1684,
                           20, 210, 211, 212, 213, 216, 218, 220, 221, 222, 223, 224, 225, 226, 227, 228,
                           229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245,
                           246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259,
                           260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 27, 290, 291, 297, 298, 299,
                           30, 31, 32, 33, 34, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 36, 370, 371, 372,
                           373, 374, 375, 376, 377, 378, 380, 381, 382, 384, 385, 386, 387, 388, 389, 39,
                           40, 41, 420, 421, 423, 43, 44, 45, 46, 47, 48, 49,
                           500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 51, 52, 53, 54, 55, 56,
                           57, 58, 590, 591, 592, 593, 594, 595, 596, 597, 598, 5999,
                           60, 61, 62, 63, 64, 65, 66, 670, 673, 674, 675, 676, 677, 678,
                           679, 680, 681, 682, 683, 685, 686, 687, 688, 689, 690, 691, 692,
                           7,
                           800, 81, 82, 84, 850, 852, 853, 855, 856, 86, 880, 886, 90, 91, 92, 93, 94, 95, 960,
                           961, 962, 963, 964, 965, 966, 967, 968, 971, 972, 973, 974,
                           975, 976, 977, 98, 992, 993, 994, 995, 996, 998
                          };

CParaNoMgrImpl& CParaNoMgrImpl::GetInstance()
{
    static CParaNoMgrImpl instance;
    return instance;
}

//号段默认值
const char * g_strMobile        = "134,135,136,137,138,139,147,150,151,158,159,157,154,152,188,187,182,183,184,1705"; //移动号段
const char * g_strUnicom        = "130,131,132,155,156,185,186,176,1709";//联通号段
const char * g_strTelecom_c     = "133,153,189,180,181,1700,177";//电信号段

CParaNoMgrImpl::CParaNoMgrImpl()
{

    //清空号段
    memset(m_ParaNo, -1, sizeof(m_ParaNo));
    std::string strMobile       = g_strMobile; //移动号段
    std::string strUnicom       = g_strUnicom;//联通号段
    std::string strTelecom      = "";//小灵通号段
    std::string strTelecom_c    = g_strTelecom_c;//电信号段
    //初始化号段
    SetPhonCom(strMobile, strUnicom, strTelecom, strTelecom_c);

    //初始化国家区号
    CLEAR(g_countryList);
    int nLenCountryList = sizeof(g_arrCountryList) / sizeof(int);
    InitCountryList(g_countryList, g_arrCountryList, nLenCountryList);

    //初始化省份
    m_mapCity.clear();
    int nIndex = INIT_AREACODE;
    m_mapCity.insert(std::make_pair((std::string)"北京", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"安徽", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"福建", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"甘肃", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"广东", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"广西", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"贵州", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"海南", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"河北", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"河南", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"黑龙江", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"湖北", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"湖南", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"吉林", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"江苏", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"江西", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"辽宁", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"内蒙古", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"宁夏", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"青海", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"山东", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"山西", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"陕西", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"上海", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"四川", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"天津", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"西藏", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"新疆", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"云南", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"浙江", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"重庆", nIndex++));

    //代码到省份的映射
    m_mapCodeCity.clear();
    m_mapCodeCity.insert(std::make_pair(UNKNOWN_AREACODE, (std::string)"unknown"));
    for (std::map<std::string, int>::iterator it = m_mapCity.begin(); it != m_mapCity.end(); ++it)
    {
        m_mapCodeCity.insert(std::make_pair(it->second, it->first));
    }
    //加载号码归属地查询库

    m_pGetMobiArea = NULL;
    HINSTANCE hDll = CAdapter::LoadLibrary(MOBILEAREAQRY_LIB);
    if (NULL != hDll)
    {
        m_pGetMobiArea = (_GetMobiArea)CAdapter::GetProcAddress(hDll, "GetMobiArea");
    }
}

CParaNoMgrImpl::~CParaNoMgrImpl()
{

}

//校验座机/小灵通号码
int CParaNoMgrImpl::ChinatelType(const char* szMobile)
{
    if (NULL == szMobile)
    {
        return -1;
    }
    std::string strMobile("");
    std::string strHead("");
    int iMobiType = -1;
    strMobile = szMobile;
    size_t nMobileLen = strMobile.size();
    if (nMobileLen < 10 || nMobileLen > 12)
    {
        iMobiType = -1;//错误
        return iMobiType;
    }

    strHead = cstr::left(strMobile, 2);
    if ("07" == strHead || "08" == strHead || "09" == strHead)
    {
        iMobiType = 2;//中国电信
    }
    else
    {
        strHead = cstr::left(strMobile, 3);
        if (0 == strHead.compare("020") ||
                0 == strHead.compare("021") ||
                0 == strHead.compare("023") ||
                0 == strHead.compare("027") ||
                0 == strHead.compare("028") ||
                0 == strHead.compare("029") ||
                0 == strHead.compare("055") ||
                0 == strHead.compare("056") ||
                0 == strHead.compare("057") ||
                0 == strHead.compare("058") ||
                0 == strHead.compare("059") ||
                0 == strHead.compare("066") ||
                0 == strHead.compare("069"))
        {
            iMobiType = 2;//中国电信
        }
        else
        {
            iMobiType = 2;//北方网通
        }
    }
    return iMobiType;
}

int CParaNoMgrImpl::SetPhonCom(std::string strMobile, std::string strUnicom, std::string strTelecom, std::string strTeleom_c)
{
    if (strMobile.empty())
    {
        strMobile = g_strMobile; //移动号段
    }

    if (strUnicom.empty())
    {
        strUnicom = g_strUnicom;//联通号段
    }

    if (strTeleom_c.empty())
    {
        strTeleom_c = g_strTelecom_c;
    }

    std::string strTmp("");
    int iErr    = 0;

    //2016-3-3
    //锁定，清空,然后赋值
    SafeLock safelock(&m_csMobiType);

    memset(m_ParaNo, -1, sizeof(m_ParaNo));

    //移动号段分解
    while (strMobile.size() > 0)
    {
        size_t iPos = strMobile.find(',');
        if (iPos != std::string::npos)
        {
            strTmp  = cstr::left(strMobile, iPos);
            strMobile = cstr::right(strMobile, strMobile.size() - iPos - 1);
        }
        else
        {
            strTmp = strMobile;
            strMobile = "";
        }
        cstr::trimright(strTmp);
        //判断是否是纯数字,若不是则不处理
        if (!CGlobal::IsAllDigital(strTmp.c_str()))
        {
            continue;
        }
        //只处理3位和4位,并且第1位是1的.
        if (3 == strTmp.size() && 0 == cstr::left(strTmp, 1).compare("1"))
        {
            //如果是3位，统一变成四位
            //算法取第2和3位然后变成整型,然后*10+(0~9)
            int nTmp = atoi(strTmp.substr(1, 2).c_str());
            nTmp *= 10;
            //锁定，然后赋值
            //SafeLock safelock(&m_csMobiType);
            for (int i = 0; i <= 9; ++i)
            {
                m_ParaNo[nTmp + i] = 0;
            }
        }
        else if (4 == strTmp.size() && 0 == cstr::left(strTmp, 1).compare("1"))
        {
            //锁定，然后赋值
            //SafeLock safelock(&m_csMobiType);

            m_ParaNo[atoi(strTmp.substr(1, 3).c_str())] = 0;
        }
        else
        {
            iErr = -1;
        }
    }

    //联通号段分解
    while (strUnicom.size() > 0)
    {
        size_t iPos = strUnicom.find(',');
        if (iPos != std::string::npos)
        {
            strTmp  = cstr::left(strUnicom, iPos);
            strUnicom = cstr::right(strUnicom, strUnicom.size() - iPos - 1);
        }
        else
        {
            strTmp  = strUnicom;
            strUnicom = "";
        }
        cstr::trimright(strTmp);
        //判断是否是纯数字,若不是则不处理
        if (!CGlobal::IsAllDigital(strTmp.c_str()))
        {
            continue;
        }
        //只处理3位和4位,并且第1位是1的.
        if (3 == strTmp.size() && 0 == cstr::left(strTmp, 1).compare("1"))
        {
            //如果是3位，统一变成四位
            //算法取第2和3位然后变成整型,然后*10+(0~9)
            int nTmp = atoi(strTmp.substr(1, 2).c_str());
            nTmp *= 10;
            //锁定，然后赋值
            //SafeLock safelock(&m_csMobiType);
            for (int i = 0; i <= 9; ++i)
            {
                m_ParaNo[nTmp + i] = 1;
            }
        }
        else if (4 == strTmp.size() && 0 == cstr::left(strTmp, 1).compare("1"))
        {
            //锁定，然后赋值
            //SafeLock safelock(&m_csMobiType);

            m_ParaNo[atoi(strTmp.substr(1, 3).c_str())] = 1;
        }
        else
        {
            iErr = -1;
        }
    }

    //电信C网号段分解
    while (strTeleom_c.size() > 0)
    {
        size_t iPos = strTeleom_c.find(',');
        if (iPos != std::string::npos)
        {
            strTmp        = cstr::left(strTeleom_c, iPos);
            strTeleom_c   = cstr::right(strTeleom_c, strTeleom_c.size() - iPos - 1);
        }
        else
        {
            strTmp  = strTeleom_c;
            strTeleom_c = "";
        }
        cstr::trimright(strTmp);
        //判断是否是纯数字,若不是则不处理
        if (!CGlobal::IsAllDigital(strTmp.c_str()))
        {
            continue;
        }
        //只处理3位和4位,并且第1位是1的.
        if (3 == strTmp.size() && 0 == cstr::left(strTmp, 1).compare("1"))
        {
            //如果是3位，统一变成四位
            //算法取第2和3位然后变成整型,然后*10+(0~9)
            int nTmp = atoi(strTmp.substr(1, 2).c_str());
            nTmp *= 10;
            //锁定，然后赋值
            //SafeLock safelock(&m_csMobiType);
            for (int i = 0; i <= 9; ++i)
            {
                m_ParaNo[nTmp + i] = 21;
            }
        }
        else if (4 == strTmp.size() && 0 == cstr::left(strTmp, 1).compare("1"))
        {
            //锁定，然后赋值
            //SafeLock safelock(&m_csMobiType);

            m_ParaNo[atoi(strTmp.substr(1, 3).c_str())] = 21;
        }
        else
        {
            iErr = -1;
        }
    }

    return iErr;
}

//判断手机号是否属于同一个号码归属地
int CParaNoMgrImpl::IsPhoneNOIdenticalArea(const std::vector<std::string>& vPhoneNO)
{
    int nRet = -1;

    if (!vPhoneNO.empty())
    {
        std::vector<std::string>::const_iterator it = vPhoneNO.begin();
        int nArea = QueryNumAttribution(it->c_str());
        for ( ; it != vPhoneNO.end(); ++it)
        {
            int nAreaTmp = QueryNumAttribution(it->c_str());
            if (nAreaTmp != nArea)
            {
                nRet = -1;
                break;
            }
            else
            {
                nArea = nAreaTmp;
                nRet  = nArea;
            }
        }
    }
    return nRet;
}

bool CParaNoMgrImpl::IsHaveThisType(const std::vector<std::string>& vMobiles, int nMobileType)
{
    bool bRet = false;
    if (!vMobiles.empty())
    {
        std::vector<std::string>::const_iterator it = vMobiles.begin();
        for ( ; it != vMobiles.end(); ++it)
        {
            //有异常号码，跳出检测，返回手机号码异常错误
            if (MobileType(it->c_str()) == nMobileType)
            {
                bRet = true;
                break;
            }
        }
    }
    return bRet;
}

//判断手机号是否是同一种运营商类型
int CParaNoMgrImpl::IsPhoneNOIdenticalType(const std::vector<std::string>& vPhoneNO)
{
    int nRet = -1;

    if (!vPhoneNO.empty())
    {
        std::vector<std::string>::const_iterator it = vPhoneNO.begin();
        int nType = MobileType(it->c_str());
        if (-1 != nType)
        {
            nRet = nType;
            //一个个检测，看是否有异常或与第一个手机号类型不一致的号码
            for (; it != vPhoneNO.end(); ++it)
            {
                int nTmp = MobileType(it->c_str());
                //有异常号码，跳出检测，返回手机号码异常错误
                if (-1 == nTmp)
                {
                    nRet = -2;
                    break;
                }
                else
                {
                    //类型不一致，置不一致标志，继续检测下一个手机号
                    if (nType != nTmp)
                    {
                        if (nRet != -2)
                        {
                            nRet = -1;
                        }
                        continue;
                    }
                    //号码正常且与第一个手机号类型一致,继续检测下一个
                    else
                    {
                        nType = nTmp;
                        continue;
                    }
                }
            }
        }
        else
        {
            //号码异常
            nRet = -2;
        }
        //没有异常且手机号码类型一致(nType=0,1,21)，返回该批手机号的类型nType
        if (nRet >= 0)
        {
            nRet = nType;
        }
    }
    else
    {
        nRet = -2;
    }
    return nRet;
}

//************************************
// Description:号段查询支持接口
// Parameter: [IN] int &nQueryType 查询类型    0:移动 1:联通 21:电信 FE:手机号查询
//              [IN] const char*szMobile 手机号码 nQueryType为FE时有效
//              [OUT] char *szNumSection 查询结果
//              [OUT] int &nLength 查询结果字节数
// Returns:   0:成功 非0:失败
// Author:      liuhuan
//************************************
int CParaNoMgrImpl::QueryNumSection(unsigned short& nQueryType, const unsigned char* szMobile, unsigned char* szNumSection, int &nLength)
{
    int nRet = ER_MISMG_SUCCESS;
    unsigned char ucQueryType;
    nLength = 0;
    //查询电信号段时，将类型修改为21
    if (nQueryType == 2)
    {
        ucQueryType = 21;
    }
    else
    {
        ucQueryType = (unsigned char)(nQueryType & 0xFF);
    }

    //取parano
    char ParaNoTmp[1001] = { -1};
    GetParaNo(ParaNoTmp);
    char ParaNo[101] = { -1};
    //把1001个号段转成101个,170号段按移动处理
    for (int nTmp = 0; nTmp < 1001; ++nTmp)
    {
        int nParaNo = nTmp / 10;
        ParaNo[nParaNo] = ParaNoTmp[nTmp];
        if (70 == nParaNo && -1 != ParaNo[nParaNo])
        {
            ParaNo[nParaNo] = 0;
        }
    }

    switch (ucQueryType)
    {
    //查询0:移动 1:联通 21:电信 统一处理
    case 0:
    case 1:
    case 21:
    {
        int iPos = 0;
        //对程序中加载的号段信息进行轮询
        for (int i = 0; i < 101; i++)
        {
            if (ucQueryType == ParaNo[i])
            {
                //ParaNo 中存储的是号段的后两位，所以加上100
                //例如: 移动号段为135 则在ParaNo[35] 填入0
                szNumSection[iPos++] = i + 100;
            }
        }
        nLength = iPos;
    }
    break;
    //查询全网
    case 9:
    {
        int iPos = 0;
        int i = 0;
        //对程序中加载的号段信息进行轮询

        //先列出移动号段
        szNumSection[iPos++] = 0;
        for (i = 0; i < 101; i++)
        {
            if (0 == ParaNo[i])
            {
                //ParaNo 中存储的是号段的后两位，所以加上100
                //例如: 移动号段为135 则在ParaNo[35] 填入0
                szNumSection[iPos++] = i + 100;
            }
        }
        //不同段得分割
        szNumSection[iPos++] = 0xFF;

        //联通号段
        szNumSection[iPos++] = 1;
        for (i = 0; i < 101; i++)
        {
            if (1 == ParaNo[i])
            {
                //ParaNo 中存储的是号段的后两位，所以加上100
                //例如: 移动号段为135 则在ParaNo[35] 填入0
                szNumSection[iPos++] = i + 100;
            }
        }

        //不同段得分割
        szNumSection[iPos++] = 0xFF;

        //电信号段
        szNumSection[iPos++] = 2;
        for (i = 0; i < 101; i++)
        {
            if (21 == ParaNo[i])
            {
                //ParaNo 中存储的是号段的后两位，所以加上100
                //例如: 移动号段为135 则在ParaNo[35] 填入0
                szNumSection[iPos++] = i + 100;
            }
        }

        nLength = iPos;
    }
    break;
    //查询单个手机号号段
    case 0xFE:
    {
        if (NULL == szMobile)
        {
            nRet = ER_MISMG_PACKER;    //查询手机号码为空
        }
        else
        {
            int nQueryResult = MobileType((char *)szMobile);
            //电信
            if (21 == nQueryResult)
            {
                szNumSection[0] = 2;
            }
            //平台不支持或手机号无效
            else if (-1 == nQueryResult)
            {
                nRet = nQueryResult;
                szNumSection[0] = 0xFE;
            }
            //其他
            else
            {
                szNumSection[0] = (unsigned char)(nQueryResult & 0xFF);
            }
        }
        nLength = 1;
    }
    break;
    //查询所有号段，支持1701等
    case 0xFF:
    {
        nRet = QueryNumSection_Ex(szNumSection, nLength);
    }
    break;

    default:
        nRet = ER_MISMG_PACKER;        //查询类型错误
        break;
    }

    return nRet;
}

//************************************
// Description:号段查询所有支持接口(包括1700号段)
// Parameter:
//              [OUT] unsigned short *szNumSection 查询结果
//              [OUT] int &nLength 查询结果nNumSection数组结果集的大小
// Returns:   0:成功 非0:失败
// Author:      fanjie
//************************************
int CParaNoMgrImpl::QueryNumSection_Ex(unsigned char* szNumSection, int& nLength)
{
    unsigned short nNumSection = 0;
    int nRet = ER_MISMG_SUCCESS;
    nLength = 0;

    //取parano
    char ParaNoTmp[1001] = { -1};
    GetParaNo(ParaNoTmp);
    char ParaNo[1001] = { -1};
    memset(ParaNo, -1, sizeof(ParaNo));
    //遍历ParaNoTmp提取号段信息
    for (int nTmp = 0; nTmp < 100; ++nTmp)
    {
        int nCount = 0;
        //每10个元素为单位判断，是否完全相等，是则只需提取其中一个元素的值进行保存,
        //否则，将这10个元素中是有效号段的话就分别进行保存
        for (nCount = 0; nCount < 9; nCount++)
        {
            if (!(-1 != ParaNoTmp[nTmp * 10 + nCount] && ParaNoTmp[nTmp * 10 + nCount] == ParaNoTmp[nTmp * 10 + nCount + 1]))
            {
                break;
            }
        }

        if (9 == nCount)
        {
            ParaNo[nTmp] = ParaNoTmp[nTmp * 10 + nCount];
        }
        else
        {
            for (nCount = 0; nCount <= 9; nCount++)
            {
                if (-1 != ParaNoTmp[nTmp * 10 + nCount])
                {
                    ParaNo[nTmp * 10 + nCount] = ParaNoTmp[nTmp * 10 + nCount];
                }
            }
        }
    }

    int iPos = 0;
    int i = 0;
    //对程序中加载的号段信息进行轮询

    //先列出移动号段
    nNumSection = 0;
    memcpy(szNumSection + iPos * 2, &nNumSection, 2);
    iPos++;
    for (i = 0; i < 1001; i++)
    {
        if (0 == ParaNo[i])
        {
            //ParaNo 中存储的是号段的后两位加上100，存储号段的后三位加上1000
            if (100 <= i)
            {
                nNumSection = i + 1000;
            }
            else
            {
                nNumSection = i + 100;
            }
            memcpy(szNumSection + iPos * 2, &nNumSection, 2);
            iPos++;
        }
    }
    //不同段得分割
    nNumSection = 0xFFFF;
    memcpy(szNumSection + iPos * 2, &nNumSection, 2);
    iPos++;
    //联通号段
    nNumSection = 1;
    memcpy(szNumSection + iPos * 2, &nNumSection, 2);
    iPos++;
    for (i = 0; i < 1001; i++)
    {
        if (1 == ParaNo[i])
        {
            //ParaNo 中存储的是号段的后两位加上100，存储号段的后三位加上1000
            if (100 <= i)
            {
                nNumSection = i + 1000;
            }
            else
            {
                nNumSection = i + 100;
            }
            memcpy(szNumSection + iPos * 2, &nNumSection, 2);
            iPos++;
        }
    }

    //不同段得分割
    nNumSection = 0xFFFF;
    memcpy(szNumSection + iPos * 2, &nNumSection, 2);
    iPos++;

    //电信号段
    nNumSection = 2;
    memcpy(szNumSection + iPos * 2, &nNumSection, 2);
    iPos++;
    for (i = 0; i < 1001; i++)
    {
        if (21 == ParaNo[i])
        {
            //ParaNo 中存储的是号段的后两位加上100，存储号段的后三位加上1000
            if (100 <= i)
            {
                nNumSection = i + 1000;
            }
            else
            {
                nNumSection = i + 100;
            }
            memcpy(szNumSection + iPos * 2, &nNumSection, 2);
            iPos++;
        }
    }
    nNumSection = 0xFFFF;
    memcpy(szNumSection + iPos * 2, &nNumSection, 2);
    iPos++;
    nLength = iPos * 2;

    return nRet;
}

//************************************
// Description:号段归属地查询接口(暂只能查询到所在省份)
// Parameter: [IN] const char*szMobile 手机号码
// Returns:   0:号码归属地未知(或可理解为归属地为全球/全国) 1-31:代表不同的省份
// Author:    jhb
//************************************
int CParaNoMgrImpl::QueryNumAttribution(const char* szMobile)
{
    int nRet = 0;
    if (NULL == szMobile || NULL == m_pGetMobiArea)
    {
        return nRet;
    }
    char szCityName[64 + 1] = {0};

    SafeLock safelock(&m_csMobiArea);

    //int iRet = m_pGetMobiArea(szMobile, szCityName);
    if (0 == m_pGetMobiArea/*GetMobiArea*/(szMobile, szCityName))
    {
        nRet = GetCityCode(szCityName);
    }
    return nRet;
}

//判断手机号是否具有某种特殊属性
int CParaNoMgrImpl::QueryNumSpeciality(const char* szMobile, const char* szUserId, int nCustomId)
{
    int nRet = 0;

    SafeLock safelock(&m_csSpecPhone);
    INT64 nPhone = _atoi64(szMobile);
    std::multimap<INT64, SPEC_PHONE_ATTR>::iterator it = m_mapSpecPhone.find(nPhone);
    if (it != m_mapSpecPhone.end())
    {
        size_t nCount = m_mapSpecPhone.count(nPhone);
        for (size_t i = 0; i < nCount; ++i, ++it)
        {
            if ((nCustomId == it->second.nCustomId && 0 == strnicmp(szUserId, it->second.szUserId, sizeof(it->second.szUserId)))
                    || (nCustomId == it->second.nCustomId && 0 == strnicmp(it->second.szUserId, "000000", 6))
                    || (0 == it->second.nCustomId && 0 == strnicmp(szUserId, it->second.szUserId, sizeof(it->second.szUserId)))
                    || (0 == it->second.nCustomId && 0 == strnicmp(it->second.szUserId, "000000", 6)))
            {
                nRet = it->second.ucType;
                break;
            }
        }
    }
    return nRet;
}

//取parano
void CParaNoMgrImpl::GetParaNo(char ParaNo[])
{
    SafeLock safelock(&m_csMobiType);
    memcpy(ParaNo, m_ParaNo, sizeof(m_ParaNo));
}

//************************************
// Description:获取省份代码
// Parameter: [IN] const char* szCityName 省份名称
// Returns:   0:省份未知(或可理解为归属地为全球/全国) 1-31:代表不同的省份
// Author:    jhb
//************************************
int CParaNoMgrImpl::GetCityCode(const char* szCityName)
{
    int nRet = 0;
    if (NULL == szCityName)
    {
        return nRet;
    }
    std::map<std::string, int>::iterator it = m_mapCity.find((std::string)szCityName);
    if (it != m_mapCity.end())
    {
        nRet = it->second;
    }
    return nRet;
}

//获取当前省份的下一个省份
int CParaNoMgrImpl::GetNextCityCode(int nArea)
{
    int nRet = 0;
    std::map<int, std::string>::iterator it = m_mapCodeCity.find(nArea);
    if (m_mapCodeCity.end() == it)
    {
        it = m_mapCodeCity.begin();
    }
    else
    {
        ++it;
        if (m_mapCodeCity.end() == it)
        {
            it = m_mapCodeCity.begin();
        }
    }
    if (m_mapCodeCity.end() != it)
    {
        nRet = it->first;
    }
    return nRet;
}

//加载携号转网表
int CParaNoMgrImpl::LoadMNP()
{
    //LNP是指用户在固网运营商之间携号转网，MNP则是指用户在移动运营商之间携号转网
    //m_mapMNP每次加载时不做清空处理，若readkfmnp读取返无记录，readkfmnp中会自动清空m_mapMNP
    //若有重复记录，m_mapMNP会自动滤重，并做更新操作

    // bool bReLoad = false; //是否需要全新加载
    //保存返回的最大ID
    // static int nCurrID = 0;
    static bool bClear = false;
    SYSTEMTIME_EX systime;
    CAdapter::GetLocalTime(&systime);
    if (0 == systime.m_iHour && bClear)
    {
        // nCurrID = 0;
        // bReLoad = true;
        bClear = false;
    }

    if (0 != systime.m_iHour)
    {
        bClear = true;
    }

    int nRet = -1;

    // std::list<MNP_INFO> mnp_list;
    // int nRet = CDataAccess::ReadKfMNP(dbparams, mnp_list, nCurrID);

    // //重载时返回空,代表需要清空
    // if (bReLoad && -1 == nRet)
    // {
    //     SafeLock safelock(&m_csMNP);
    //     m_mapMNP.clear();
    // }
    // //重载时返回非空,清空后赋值
    // else if (bReLoad && 0 == nRet)
    // {
    //     tMNP mnp;
    //     SafeLock safelock(&m_csMNP);
    //     m_mapMNP.clear();
    //     std::list<MNP_INFO>::iterator it = mnp_list.begin();
    //     for (it; it != mnp_list.end(); ++it)
    //     {
    //         //ADD
    //         if (0 == it->nOpType)
    //         {
    //             mnp.nPhoneType = it->nPhoneType;
    //             mnp.nSrcType   = it->nSrcType;
    //             m_mapMNP.insert(std::make_pair(it->nPhone, mnp));
    //         }
    //         //DEL
    //         else
    //         {
    //             m_mapMNP.erase(it->nPhone);
    //         }
    //     }
    // }
    // //增量同步
    // else if (nRet <= 0)
    // {
    //     tMNP mnp;
    //     std::list<MNP_INFO>::iterator it = mnp_list.begin();
    //     for (it; it != mnp_list.end(); ++it)
    //     {
    //         //ADD
    //         if (0 == it->nOpType)
    //         {
    //             mnp.nPhoneType = it->nPhoneType;
    //             mnp.nSrcType   = it->nSrcType;

    //             SafeLock safelock(&m_csMNP);
    //             m_mapMNP.insert(std::make_pair(it->nPhone, mnp));
    //         }
    //         //DEL
    //         else
    //         {
    //             SafeLock safelock(&m_csMNP);
    //             m_mapMNP.erase(it->nPhone);
    //         }
    //     }
    // }
    // else
    // {
    //     //不做处理
    // }
    return (nRet <= 0);
}

//加载手机号段表
int CParaNoMgrImpl::LoadParaNo()
{
    int nRet = -1;
    // std::cout << "-------------------------" << std::endl;
    SafeLock safelock(&m_csSync);

    static std::string s_strMobile    = g_strMobile;
    static std::string s_strUnicom    = g_strUnicom;
    // static std::string s_strTelecom   = "";
    static std::string s_strTelecom_c = g_strTelecom_c;
    // static std::string s_strWholeNet  = "";

    std::string strMobile    = "";
    std::string strUnicom    = "";
    std::string strTelecom   = "";
    std::string strTelecom_c = "";
    std::string strWholeNet  = "";

    //如果本次同步到的数据和上次一样,就无需再去覆盖内存
    if (CAccessMssql::ReadKfParams(strMobile, strUnicom, strTelecom, strTelecom_c) <= 0
            && (strMobile != s_strMobile || strUnicom != s_strUnicom || strTelecom_c != s_strTelecom_c))
    {
        nRet = SetPhonCom(strMobile, strUnicom, strTelecom, strTelecom_c);
        s_strMobile    = strMobile;
        s_strUnicom    = strUnicom;
        s_strTelecom_c = strTelecom_c;
    }

    if (CAccessMssql::ReadPbServiceType(strMobile, strUnicom, strTelecom, strTelecom_c, strWholeNet) <= 0
            && (strMobile != s_strMobile || strUnicom != s_strUnicom || strTelecom_c != s_strTelecom_c))
    {
        nRet = SetPhonCom(strMobile, strUnicom, strTelecom, strTelecom_c);
        s_strMobile    = strMobile;
        s_strUnicom    = strUnicom;
        s_strTelecom_c = strTelecom_c;
    }

    return nRet;
}

//获取特殊手机号码的个数
int CParaNoMgrImpl::GetSpecPhoneCnt()
{
    //RISK_1
    return (int)m_mapSpecPhone.size();
}

//加载特殊手机号码表
int CParaNoMgrImpl::LoadSpecialPhone(bool bForceReLoad)
{
    int nRet = 0;
    // bool bReLoad = false; //是否需要全新加载特殊手机号码
    //定义两个数组，表示本次增加的特殊手机号码和本次删除的特殊手机号码
    // std::list<SPEC_PHONE> PhonesAdd, PhonesDel;
    //保存返回的最大ID
    // static int nCurrID = 0;
    static bool bClear = false;
    SYSTEMTIME_EX systime;
    CAdapter::GetLocalTime(&systime);
    if (0 == systime.m_iHour && bClear)
    {
        // nCurrID = 0;
        // bReLoad = true;
        bClear = false;
    }

    if (0 != systime.m_iHour)
    {
        bClear = true;
    }

    //强制加载
    // if (bForceReLoad) nCurrID = 0;

    // int nResult = CDataAccess::LoadSpecialPhone(PhonesAdd, PhonesDel, nCurrID, dbparams);

    // if (bForceReLoad || bReLoad || (nResult <= 0 && (PhonesAdd.size() > 0 || PhonesDel.size() > 0)))
    // {
    //     SafeLock safelock(&m_csSpecPhone);

    //     //若要求重载,则需先清空
    //     if (bForceReLoad || bReLoad) m_mapSpecPhone.clear();
    //     SPEC_PHONE_ATTR attr;
    //     std::list<SPEC_PHONE>::iterator it = PhonesAdd.begin();
    //     for (it; it != PhonesAdd.end(); ++it)
    //     {
    //         attr.nCustomId = it->nCustomId;
    //         attr.ucType    = it->ucType;
    //         strcpy(attr.szUserId, it->szUserId);
    //         m_mapSpecPhone.insert(std::make_pair(it->nPhone, attr));
    //     }
    //     PhonesAdd.clear();

    //     it = PhonesDel.begin();
    //     for (it; it != PhonesDel.end(); ++it)
    //     {
    //         std::multimap<INT64, SPEC_PHONE_ATTR>::iterator it2 = m_mapSpecPhone.find(it->nPhone);
    //         if (it2 != m_mapSpecPhone.end())
    //         {
    //             size_t nCount = m_mapSpecPhone.count(it->nPhone);
    //             for (size_t i = 0; i < nCount; ++i)
    //             {
    //                 if (it->nCustomId == it2->second.nCustomId && 0 == strnicmp(it->szUserId, it2->second.szUserId, sizeof(it->szUserId)))
    //                 {
    //                     m_mapSpecPhone.erase(it2++);
    //                 }
    //                 else
    //                 {
    //                     ++it2;
    //                 }
    //             }
    //         }
    //     }
    //     PhonesDel.clear();
    // }
    // else
    // {
    //     if (nResult > 0) nRet = -1;
    // }

    return nRet;
}


int CParaNoMgrImpl::ChinaMobileType(const char* szMobile, int &nSrcType)
{
    //负数错误。
    std::string strMobile(szMobile);
    std::string strFrst("");
    std::string strHead("");

    int iMobiType = -1;
    size_t iNumLen = 0;
    // std::string strLeft4 = cstr::left(strMobile, 4);
    /*
    if (0 == strLeft3.compare("118") || 0 == strLeft3.compare("116"))
    {
        strMobile = cstr::right(strMobile, strMobile.size()-3);
    }
    else
    {
        strMobile = szMobile;
    }
    */
    //长度限制(11或12)
    iNumLen = strMobile.size();
    if (iNumLen < 11 || iNumLen > 12)
    {
        nSrcType = -1;
        return nSrcType;
    }
    //必须为纯数字
    if (!CGlobal::IsAllDigital(strMobile.c_str()))
    {
        nSrcType = -1;
        return nSrcType;
    }
    /*
    for (int i = 0; i < iNumLen; ++i)
    {
        if (strMobile[i] < 0x30 || strMobile[i] > 0x39)
        {
            nSrcType = -1;
            return nSrcType;
        }
    }
    */

    strFrst = cstr::left(strMobile, 1);
    if ("0" == strFrst)//小灵通
    {
        if (0x30 == strMobile[1]) //第二位也是0
        {
            nSrcType = -1;
            return nSrcType;
        }
        else if (0x31 == strMobile[1] && 0x30 != strMobile[2]) //不是010
        {
            nSrcType = -1;
            return nSrcType;
        }
        else
        {
            nSrcType = iMobiType = ChinatelType(strMobile.c_str());
            return iMobiType;
        }
    }
    else if ("1" == strFrst && 11 == iNumLen)
    {
        //首先判断是否携号转网
        {
            INT64 iMobile = _atoi64( strMobile.c_str() );
            SafeLock safelock(&m_csMNP);
            std::map<INT64, tMNP>::iterator it = m_mapMNP.find( iMobile );
            if ( it != m_mapMNP.end() )
            {
                nSrcType = it->second.nSrcType;
                return it->second.nPhoneType;
            }
        }

        //再判断号码类型
        {
            //取第2~4位
            strHead = strMobile.substr(1, 3);
            SafeLock safelock(&m_csMobiType);
            nSrcType = iMobiType = m_ParaNo[atoi(strHead.c_str())];
        }
    }
    else
    {
        nSrcType = iMobiType = -1;
    }
    return iMobiType;
}

int CParaNoMgrImpl::LoadCountryList()
{

    CLEAR(g_DBcountryList);

    int nRet = CAccessMssql::ReadCountryList(g_DBcountryList);

    if (nRet == 0)
    {
        SafeLock safelock(&m_csCountryList);
        CLEAR(g_countryList);
        g_DBcountryList[86] = 1;
        memcpy(g_countryList, g_DBcountryList, MAX_COUNTRY);
    }

    return nRet;
}

//获取国家区号（必须是带国家区号的国际电话号码）
int CParaNoMgrImpl::GetCountryCode(const char* szInternationalMobile, int &nCountry)
{
    if (NULL == szInternationalMobile)
    {
        return -1;
    }

    //必须为纯数字
    if (!CGlobal::IsAllDigital(szInternationalMobile))
    {
        return -1;
    }

    if (strlen(szInternationalMobile) < 6)
    {
        return -1;
    }

    std::string strTemp    = szInternationalMobile;

    int iCountry1 = atoi(strTemp.substr(0, 1).c_str());
    int iCountry2 = atoi(strTemp.substr(0, 2).c_str());
    int iCountry3 = atoi(strTemp.substr(0, 3).c_str());
    int iCountry4 = atoi(strTemp.substr(0, 4).c_str());
    int iCountry5 = atoi(strTemp.substr(0, 5).c_str());
    int iCountry6 = atoi(strTemp.substr(0, 6).c_str());

    SafeLock safelock(&m_csCountryList);

    /*
    //判断是否为特殊电话号码（中国，只有电话号码，不带国际区号）
    if ( 1 == iCountry1 && 11 == strlen(szInternationalMobile) )
    {
        int iRes = ChinaMobileType(szInternationalMobile);
        if ( -1 != iRes)
        {
            nCountry = 86;
            return 0;
        }
    }
    */

    //获取国际电话号码（带国际区号）区号
    if (g_countryList[iCountry6] == 1)
    {
        nCountry = iCountry6;
    }
    else if (g_countryList[iCountry5] == 1)
    {
        nCountry = iCountry5;
    }
    else if (g_countryList[iCountry4] == 1)
    {
        nCountry = iCountry4;
    }
    else if (g_countryList[iCountry3] == 1)
    {
        nCountry = iCountry3;
    }
    else if (g_countryList[iCountry2] == 1)
    {
        nCountry = iCountry2;
    }
    else if (g_countryList[iCountry1] == 1)
    {
        nCountry = iCountry1;
    }
    else
    {
        return -1;
    }

    return 0;

}

int CParaNoMgrImpl::NationalMobileType(const char* szMobile)
{
    int iResMobileType = -1;   //手机类型返回值
    // int iResNationational = -1;  //国际区号返回值
    int iCountryTemp = -1; //国家区号临时变量

    int iResNationational = GetCountryCode(szMobile, iCountryTemp);
    if (iResNationational == 0)
    {
        if ( 86 == iCountryTemp)
        {
            int iSrcType = -1;
            iResMobileType = ChinaMobileType(szMobile + 2, iSrcType);
        }
        else
        {
            iResMobileType = 5;
        }
    }
    else
    {
        iResMobileType = -1;
    }

    return iResMobileType;
}

//判断手机号码类型
int CParaNoMgrImpl::MobileType(const char* szMobile)
{
    int iResMobileType = -1;   //手机类型返回值
    // int iResChinaMobileType = -1; //中国区域手机号码返回类型
    // int iResNationational = -1;  //国际区号返回值
    // int iCountryTemp = -1; //国家区号临时变量
    int iSrcType = -1;

    std::string strMobile = szMobile;
    if ("+" == strMobile.substr(0, 1) && '0' != *(szMobile + 1))
    {
        iResMobileType = NationalMobileType(szMobile + 1);
    }
    else if ("00" == strMobile.substr(0, 2) && '0' != *(szMobile + 2))
    {
        iResMobileType = NationalMobileType(szMobile + 2);
    }
    else
    {
        iResMobileType = ChinaMobileType(szMobile, iSrcType);
    }

    return iResMobileType;
}

void  CParaNoMgrImpl::GetNationalMobileCountryAndType(const char* szMobile, int &nCountry, int &nType, int &nSrcType)
{
    // int iResNationational = -1;  //国际区号返回值
    int iCountryTemp = -1; //国家区号临时变量

    //判断是否可以获得国家区号，0成功，-1失败
    int iResNationational = GetCountryCode(szMobile, iCountryTemp);
    if (iResNationational == 0)
    {
        if ( 86 == iCountryTemp)
        {
            nCountry = iCountryTemp;
            nType = ChinaMobileType(szMobile + 2, nSrcType);
        }
        else
        {
            nCountry = iCountryTemp;
            nSrcType = nType = 5;
        }
    }
    else
    {
        nCountry = 0;
        nSrcType = nType = -1;
    }
}

int CParaNoMgrImpl::GetMobileCountryAndType(const char* szMobile, int &nCountry, int & nType)
{
    // int iResChinaMobileType = -1; //中国区域手机号码返回类型
    // int iResNationational = -1;  //国际区号返回值
    // int iCountryTemp = -1; //国家区号临时变量
    int iSrcType = -1;

    std::string strMobile = szMobile;

    //去掉szMobile号码前面的“+”/“00”
    if ("+" == strMobile.substr(0, 1) && '0' != *(szMobile + 1))
    {
        const char* szMobileTemp = szMobile + 1;
        GetNationalMobileCountryAndType(szMobileTemp, nCountry, nType, iSrcType);
    }
    else if ("00" == strMobile.substr(0, 2) && '0' != *(szMobile + 2))
    {
        const char* szMobileTemp = szMobile + 2;
        GetNationalMobileCountryAndType(szMobileTemp, nCountry, nType, iSrcType);
    }
    //如果第一位是数字1到9
    else
    {
        //对号码进行中国校验
        nCountry = 86;
        nType = ChinaMobileType(szMobile, iSrcType);
    }

    return 0;
}

int CParaNoMgrImpl::GetMobileCountryAndTypeEx(const char* szMobile, int& nCountry, int& nType, int& nSrcType)
{
    // int iResChinaMobileType = -1; //中国区域手机号码返回类型
    // int iResNationational = -1;  //国际区号返回值
    // int iCountryTemp = -1; //国家区号临时变量

    std::string strMobile = szMobile;

    //去掉szMobile号码前面的“+”/“00”
    if ("+" == strMobile.substr(0, 1) && '0' != *(szMobile + 1))
    {
        const char* szMobileTemp = szMobile + 1;
        GetNationalMobileCountryAndType(szMobileTemp, nCountry, nType, nSrcType);
    }
    else if ("00" == strMobile.substr(0, 2) && '0' != *(szMobile + 2))
    {
        const char* szMobileTemp = szMobile + 2;
        GetNationalMobileCountryAndType(szMobileTemp, nCountry, nType, nSrcType);
    }
    //如果第一位是数字1到9
    else
    {
        //对号码进行中国校验
        nCountry = 86;
        nType = ChinaMobileType(szMobile, nSrcType);
    }

    return 0;
}

//初始化国际电话号码国家区号
int CParaNoMgrImpl::InitCountryList(char* szCountryList, const int* arrCountryList, int nLenCountryList)
{
    if (NULL == arrCountryList)
    {
        return -1;
    }
    for (int i = 0; i < nLenCountryList; i++)
    {
        int temp = arrCountryList[i];
        szCountryList[temp] = 1;
    }
    return 0;
}
