// ParaNoMgrImpl.cpp: implementation of the CParaNoMgrImpl class.
//
//////////////////////////////////////////////////////////////////////

#include <paranomgr/ParaNoMgrImpl.h>
#include <paranomgr/AccessMssql.h>
#include "Global.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char g_countryList[MAX_COUNTRY];          //��Ź�����������
char g_DBcountryList[MAX_COUNTRY];        //������ݿ⵼����������
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

//�Ŷ�Ĭ��ֵ
const char * g_strMobile        = "134,135,136,137,138,139,147,150,151,158,159,157,154,152,188,187,182,183,184,1705"; //�ƶ��Ŷ�
const char * g_strUnicom        = "130,131,132,155,156,185,186,176,1709";//��ͨ�Ŷ�
const char * g_strTelecom_c     = "133,153,189,180,181,1700,177";//���źŶ�

CParaNoMgrImpl::CParaNoMgrImpl()
{

    //��պŶ�
    memset(m_ParaNo, -1, sizeof(m_ParaNo));
    std::string strMobile       = g_strMobile; //�ƶ��Ŷ�
    std::string strUnicom       = g_strUnicom;//��ͨ�Ŷ�
    std::string strTelecom      = "";//С��ͨ�Ŷ�
    std::string strTelecom_c    = g_strTelecom_c;//���źŶ�
    //��ʼ���Ŷ�
    SetPhonCom(strMobile, strUnicom, strTelecom, strTelecom_c);

    //��ʼ����������
    CLEAR(g_countryList);
    int nLenCountryList = sizeof(g_arrCountryList) / sizeof(int);
    InitCountryList(g_countryList, g_arrCountryList, nLenCountryList);

    //��ʼ��ʡ��
    m_mapCity.clear();
    int nIndex = INIT_AREACODE;
    m_mapCity.insert(std::make_pair((std::string)"����", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"����", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"����", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"����", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"�㶫", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"����", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"����", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"����", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"�ӱ�", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"����", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"������", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"����", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"����", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"����", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"����", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"����", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"����", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"���ɹ�", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"����", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"�ຣ", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"ɽ��", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"ɽ��", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"����", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"�Ϻ�", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"�Ĵ�", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"���", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"����", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"�½�", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"����", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"�㽭", nIndex++));
    m_mapCity.insert(std::make_pair((std::string)"����", nIndex++));

    //���뵽ʡ�ݵ�ӳ��
    m_mapCodeCity.clear();
    m_mapCodeCity.insert(std::make_pair(UNKNOWN_AREACODE, (std::string)"unknown"));
    for (std::map<std::string, int>::iterator it = m_mapCity.begin(); it != m_mapCity.end(); ++it)
    {
        m_mapCodeCity.insert(std::make_pair(it->second, it->first));
    }
    //���غ�������ز�ѯ��

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

//У������/С��ͨ����
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
        iMobiType = -1;//����
        return iMobiType;
    }

    strHead = cstr::left(strMobile, 2);
    if ("07" == strHead || "08" == strHead || "09" == strHead)
    {
        iMobiType = 2;//�й�����
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
            iMobiType = 2;//�й�����
        }
        else
        {
            iMobiType = 2;//������ͨ
        }
    }
    return iMobiType;
}

int CParaNoMgrImpl::SetPhonCom(std::string strMobile, std::string strUnicom, std::string strTelecom, std::string strTeleom_c)
{
    if (strMobile.empty())
    {
        strMobile = g_strMobile; //�ƶ��Ŷ�
    }

    if (strUnicom.empty())
    {
        strUnicom = g_strUnicom;//��ͨ�Ŷ�
    }

    if (strTeleom_c.empty())
    {
        strTeleom_c = g_strTelecom_c;
    }

    std::string strTmp("");
    int iErr    = 0;

    //2016-3-3
    //���������,Ȼ��ֵ
    SafeLock safelock(&m_csMobiType);

    memset(m_ParaNo, -1, sizeof(m_ParaNo));

    //�ƶ��Ŷηֽ�
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
        //�ж��Ƿ��Ǵ�����,�������򲻴���
        if (!CGlobal::IsAllDigital(strTmp.c_str()))
        {
            continue;
        }
        //ֻ����3λ��4λ,���ҵ�1λ��1��.
        if (3 == strTmp.size() && 0 == cstr::left(strTmp, 1).compare("1"))
        {
            //�����3λ��ͳһ�����λ
            //�㷨ȡ��2��3λȻ��������,Ȼ��*10+(0~9)
            int nTmp = atoi(strTmp.substr(1, 2).c_str());
            nTmp *= 10;
            //������Ȼ��ֵ
            //SafeLock safelock(&m_csMobiType);
            for (int i = 0; i <= 9; ++i)
            {
                m_ParaNo[nTmp + i] = 0;
            }
        }
        else if (4 == strTmp.size() && 0 == cstr::left(strTmp, 1).compare("1"))
        {
            //������Ȼ��ֵ
            //SafeLock safelock(&m_csMobiType);

            m_ParaNo[atoi(strTmp.substr(1, 3).c_str())] = 0;
        }
        else
        {
            iErr = -1;
        }
    }

    //��ͨ�Ŷηֽ�
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
        //�ж��Ƿ��Ǵ�����,�������򲻴���
        if (!CGlobal::IsAllDigital(strTmp.c_str()))
        {
            continue;
        }
        //ֻ����3λ��4λ,���ҵ�1λ��1��.
        if (3 == strTmp.size() && 0 == cstr::left(strTmp, 1).compare("1"))
        {
            //�����3λ��ͳһ�����λ
            //�㷨ȡ��2��3λȻ��������,Ȼ��*10+(0~9)
            int nTmp = atoi(strTmp.substr(1, 2).c_str());
            nTmp *= 10;
            //������Ȼ��ֵ
            //SafeLock safelock(&m_csMobiType);
            for (int i = 0; i <= 9; ++i)
            {
                m_ParaNo[nTmp + i] = 1;
            }
        }
        else if (4 == strTmp.size() && 0 == cstr::left(strTmp, 1).compare("1"))
        {
            //������Ȼ��ֵ
            //SafeLock safelock(&m_csMobiType);

            m_ParaNo[atoi(strTmp.substr(1, 3).c_str())] = 1;
        }
        else
        {
            iErr = -1;
        }
    }

    //����C���Ŷηֽ�
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
        //�ж��Ƿ��Ǵ�����,�������򲻴���
        if (!CGlobal::IsAllDigital(strTmp.c_str()))
        {
            continue;
        }
        //ֻ����3λ��4λ,���ҵ�1λ��1��.
        if (3 == strTmp.size() && 0 == cstr::left(strTmp, 1).compare("1"))
        {
            //�����3λ��ͳһ�����λ
            //�㷨ȡ��2��3λȻ��������,Ȼ��*10+(0~9)
            int nTmp = atoi(strTmp.substr(1, 2).c_str());
            nTmp *= 10;
            //������Ȼ��ֵ
            //SafeLock safelock(&m_csMobiType);
            for (int i = 0; i <= 9; ++i)
            {
                m_ParaNo[nTmp + i] = 21;
            }
        }
        else if (4 == strTmp.size() && 0 == cstr::left(strTmp, 1).compare("1"))
        {
            //������Ȼ��ֵ
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

//�ж��ֻ����Ƿ�����ͬһ�����������
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
            //���쳣���룬������⣬�����ֻ������쳣����
            if (MobileType(it->c_str()) == nMobileType)
            {
                bRet = true;
                break;
            }
        }
    }
    return bRet;
}

//�ж��ֻ����Ƿ���ͬһ����Ӫ������
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
            //һ������⣬���Ƿ����쳣�����һ���ֻ������Ͳ�һ�µĺ���
            for (; it != vPhoneNO.end(); ++it)
            {
                int nTmp = MobileType(it->c_str());
                //���쳣���룬������⣬�����ֻ������쳣����
                if (-1 == nTmp)
                {
                    nRet = -2;
                    break;
                }
                else
                {
                    //���Ͳ�һ�£��ò�һ�±�־�����������һ���ֻ���
                    if (nType != nTmp)
                    {
                        if (nRet != -2)
                        {
                            nRet = -1;
                        }
                        continue;
                    }
                    //�������������һ���ֻ�������һ��,���������һ��
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
            //�����쳣
            nRet = -2;
        }
        //û���쳣���ֻ���������һ��(nType=0,1,21)�����ظ����ֻ��ŵ�����nType
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
// Description:�Ŷβ�ѯ֧�ֽӿ�
// Parameter: [IN] int &nQueryType ��ѯ����    0:�ƶ� 1:��ͨ 21:���� FE:�ֻ��Ų�ѯ
//              [IN] const char*szMobile �ֻ����� nQueryTypeΪFEʱ��Ч
//              [OUT] char *szNumSection ��ѯ���
//              [OUT] int &nLength ��ѯ����ֽ���
// Returns:   0:�ɹ� ��0:ʧ��
// Author:      liuhuan
//************************************
int CParaNoMgrImpl::QueryNumSection(unsigned short& nQueryType, const unsigned char* szMobile, unsigned char* szNumSection, int &nLength)
{
    int nRet = ER_MISMG_SUCCESS;
    unsigned char ucQueryType;
    nLength = 0;
    //��ѯ���źŶ�ʱ���������޸�Ϊ21
    if (nQueryType == 2)
    {
        ucQueryType = 21;
    }
    else
    {
        ucQueryType = (unsigned char)(nQueryType & 0xFF);
    }

    //ȡparano
    char ParaNoTmp[1001] = { -1};
    GetParaNo(ParaNoTmp);
    char ParaNo[101] = { -1};
    //��1001���Ŷ�ת��101��,170�Ŷΰ��ƶ�����
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
    //��ѯ0:�ƶ� 1:��ͨ 21:���� ͳһ����
    case 0:
    case 1:
    case 21:
    {
        int iPos = 0;
        //�Գ����м��صĺŶ���Ϣ������ѯ
        for (int i = 0; i < 101; i++)
        {
            if (ucQueryType == ParaNo[i])
            {
                //ParaNo �д洢���ǺŶεĺ���λ�����Լ���100
                //����: �ƶ��Ŷ�Ϊ135 ����ParaNo[35] ����0
                szNumSection[iPos++] = i + 100;
            }
        }
        nLength = iPos;
    }
    break;
    //��ѯȫ��
    case 9:
    {
        int iPos = 0;
        int i = 0;
        //�Գ����м��صĺŶ���Ϣ������ѯ

        //���г��ƶ��Ŷ�
        szNumSection[iPos++] = 0;
        for (i = 0; i < 101; i++)
        {
            if (0 == ParaNo[i])
            {
                //ParaNo �д洢���ǺŶεĺ���λ�����Լ���100
                //����: �ƶ��Ŷ�Ϊ135 ����ParaNo[35] ����0
                szNumSection[iPos++] = i + 100;
            }
        }
        //��ͬ�ε÷ָ�
        szNumSection[iPos++] = 0xFF;

        //��ͨ�Ŷ�
        szNumSection[iPos++] = 1;
        for (i = 0; i < 101; i++)
        {
            if (1 == ParaNo[i])
            {
                //ParaNo �д洢���ǺŶεĺ���λ�����Լ���100
                //����: �ƶ��Ŷ�Ϊ135 ����ParaNo[35] ����0
                szNumSection[iPos++] = i + 100;
            }
        }

        //��ͬ�ε÷ָ�
        szNumSection[iPos++] = 0xFF;

        //���źŶ�
        szNumSection[iPos++] = 2;
        for (i = 0; i < 101; i++)
        {
            if (21 == ParaNo[i])
            {
                //ParaNo �д洢���ǺŶεĺ���λ�����Լ���100
                //����: �ƶ��Ŷ�Ϊ135 ����ParaNo[35] ����0
                szNumSection[iPos++] = i + 100;
            }
        }

        nLength = iPos;
    }
    break;
    //��ѯ�����ֻ��źŶ�
    case 0xFE:
    {
        if (NULL == szMobile)
        {
            nRet = ER_MISMG_PACKER;    //��ѯ�ֻ�����Ϊ��
        }
        else
        {
            int nQueryResult = MobileType((char *)szMobile);
            //����
            if (21 == nQueryResult)
            {
                szNumSection[0] = 2;
            }
            //ƽ̨��֧�ֻ��ֻ�����Ч
            else if (-1 == nQueryResult)
            {
                nRet = nQueryResult;
                szNumSection[0] = 0xFE;
            }
            //����
            else
            {
                szNumSection[0] = (unsigned char)(nQueryResult & 0xFF);
            }
        }
        nLength = 1;
    }
    break;
    //��ѯ���кŶΣ�֧��1701��
    case 0xFF:
    {
        nRet = QueryNumSection_Ex(szNumSection, nLength);
    }
    break;

    default:
        nRet = ER_MISMG_PACKER;        //��ѯ���ʹ���
        break;
    }

    return nRet;
}

//************************************
// Description:�Ŷβ�ѯ����֧�ֽӿ�(����1700�Ŷ�)
// Parameter:
//              [OUT] unsigned short *szNumSection ��ѯ���
//              [OUT] int &nLength ��ѯ���nNumSection���������Ĵ�С
// Returns:   0:�ɹ� ��0:ʧ��
// Author:      fanjie
//************************************
int CParaNoMgrImpl::QueryNumSection_Ex(unsigned char* szNumSection, int& nLength)
{
    unsigned short nNumSection = 0;
    int nRet = ER_MISMG_SUCCESS;
    nLength = 0;

    //ȡparano
    char ParaNoTmp[1001] = { -1};
    GetParaNo(ParaNoTmp);
    char ParaNo[1001] = { -1};
    memset(ParaNo, -1, sizeof(ParaNo));
    //����ParaNoTmp��ȡ�Ŷ���Ϣ
    for (int nTmp = 0; nTmp < 100; ++nTmp)
    {
        int nCount = 0;
        //ÿ10��Ԫ��Ϊ��λ�жϣ��Ƿ���ȫ��ȣ�����ֻ����ȡ����һ��Ԫ�ص�ֵ���б���,
        //���򣬽���10��Ԫ��������Ч�ŶεĻ��ͷֱ���б���
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
    //�Գ����м��صĺŶ���Ϣ������ѯ

    //���г��ƶ��Ŷ�
    nNumSection = 0;
    memcpy(szNumSection + iPos * 2, &nNumSection, 2);
    iPos++;
    for (i = 0; i < 1001; i++)
    {
        if (0 == ParaNo[i])
        {
            //ParaNo �д洢���ǺŶεĺ���λ����100���洢�Ŷεĺ���λ����1000
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
    //��ͬ�ε÷ָ�
    nNumSection = 0xFFFF;
    memcpy(szNumSection + iPos * 2, &nNumSection, 2);
    iPos++;
    //��ͨ�Ŷ�
    nNumSection = 1;
    memcpy(szNumSection + iPos * 2, &nNumSection, 2);
    iPos++;
    for (i = 0; i < 1001; i++)
    {
        if (1 == ParaNo[i])
        {
            //ParaNo �д洢���ǺŶεĺ���λ����100���洢�Ŷεĺ���λ����1000
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

    //��ͬ�ε÷ָ�
    nNumSection = 0xFFFF;
    memcpy(szNumSection + iPos * 2, &nNumSection, 2);
    iPos++;

    //���źŶ�
    nNumSection = 2;
    memcpy(szNumSection + iPos * 2, &nNumSection, 2);
    iPos++;
    for (i = 0; i < 1001; i++)
    {
        if (21 == ParaNo[i])
        {
            //ParaNo �д洢���ǺŶεĺ���λ����100���洢�Ŷεĺ���λ����1000
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
// Description:�Ŷι����ز�ѯ�ӿ�(��ֻ�ܲ�ѯ������ʡ��)
// Parameter: [IN] const char*szMobile �ֻ�����
// Returns:   0:���������δ֪(������Ϊ������Ϊȫ��/ȫ��) 1-31:����ͬ��ʡ��
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

//�ж��ֻ����Ƿ����ĳ����������
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

//ȡparano
void CParaNoMgrImpl::GetParaNo(char ParaNo[])
{
    SafeLock safelock(&m_csMobiType);
    memcpy(ParaNo, m_ParaNo, sizeof(m_ParaNo));
}

//************************************
// Description:��ȡʡ�ݴ���
// Parameter: [IN] const char* szCityName ʡ������
// Returns:   0:ʡ��δ֪(������Ϊ������Ϊȫ��/ȫ��) 1-31:����ͬ��ʡ��
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

//��ȡ��ǰʡ�ݵ���һ��ʡ��
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

//����Я��ת����
int CParaNoMgrImpl::LoadMNP()
{
    //LNP��ָ�û��ڹ�����Ӫ��֮��Я��ת����MNP����ָ�û����ƶ���Ӫ��֮��Я��ת��
    //m_mapMNPÿ�μ���ʱ������մ�����readkfmnp��ȡ���޼�¼��readkfmnp�л��Զ����m_mapMNP
    //�����ظ���¼��m_mapMNP���Զ����أ��������²���

    // bool bReLoad = false; //�Ƿ���Ҫȫ�¼���
    //���淵�ص����ID
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

    // //����ʱ���ؿ�,������Ҫ���
    // if (bReLoad && -1 == nRet)
    // {
    //     SafeLock safelock(&m_csMNP);
    //     m_mapMNP.clear();
    // }
    // //����ʱ���طǿ�,��պ�ֵ
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
    // //����ͬ��
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
    //     //��������
    // }
    return (nRet <= 0);
}

//�����ֻ��Ŷα�
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

    //�������ͬ���������ݺ��ϴ�һ��,��������ȥ�����ڴ�
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

//��ȡ�����ֻ�����ĸ���
int CParaNoMgrImpl::GetSpecPhoneCnt()
{
    //RISK_1
    return (int)m_mapSpecPhone.size();
}

//���������ֻ������
int CParaNoMgrImpl::LoadSpecialPhone(bool bForceReLoad)
{
    int nRet = 0;
    // bool bReLoad = false; //�Ƿ���Ҫȫ�¼��������ֻ�����
    //�����������飬��ʾ�������ӵ������ֻ�����ͱ���ɾ���������ֻ�����
    // std::list<SPEC_PHONE> PhonesAdd, PhonesDel;
    //���淵�ص����ID
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

    //ǿ�Ƽ���
    // if (bForceReLoad) nCurrID = 0;

    // int nResult = CDataAccess::LoadSpecialPhone(PhonesAdd, PhonesDel, nCurrID, dbparams);

    // if (bForceReLoad || bReLoad || (nResult <= 0 && (PhonesAdd.size() > 0 || PhonesDel.size() > 0)))
    // {
    //     SafeLock safelock(&m_csSpecPhone);

    //     //��Ҫ������,���������
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
    //��������
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
    //��������(11��12)
    iNumLen = strMobile.size();
    if (iNumLen < 11 || iNumLen > 12)
    {
        nSrcType = -1;
        return nSrcType;
    }
    //����Ϊ������
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
    if ("0" == strFrst)//С��ͨ
    {
        if (0x30 == strMobile[1]) //�ڶ�λҲ��0
        {
            nSrcType = -1;
            return nSrcType;
        }
        else if (0x31 == strMobile[1] && 0x30 != strMobile[2]) //����010
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
        //�����ж��Ƿ�Я��ת��
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

        //���жϺ�������
        {
            //ȡ��2~4λ
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

//��ȡ�������ţ������Ǵ��������ŵĹ��ʵ绰���룩
int CParaNoMgrImpl::GetCountryCode(const char* szInternationalMobile, int &nCountry)
{
    if (NULL == szInternationalMobile)
    {
        return -1;
    }

    //����Ϊ������
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
    //�ж��Ƿ�Ϊ����绰���루�й���ֻ�е绰���룬�����������ţ�
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

    //��ȡ���ʵ绰���루���������ţ�����
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
    int iResMobileType = -1;   //�ֻ����ͷ���ֵ
    // int iResNationational = -1;  //�������ŷ���ֵ
    int iCountryTemp = -1; //����������ʱ����

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

//�ж��ֻ���������
int CParaNoMgrImpl::MobileType(const char* szMobile)
{
    int iResMobileType = -1;   //�ֻ����ͷ���ֵ
    // int iResChinaMobileType = -1; //�й������ֻ����뷵������
    // int iResNationational = -1;  //�������ŷ���ֵ
    // int iCountryTemp = -1; //����������ʱ����
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
    // int iResNationational = -1;  //�������ŷ���ֵ
    int iCountryTemp = -1; //����������ʱ����

    //�ж��Ƿ���Ի�ù������ţ�0�ɹ���-1ʧ��
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
    // int iResChinaMobileType = -1; //�й������ֻ����뷵������
    // int iResNationational = -1;  //�������ŷ���ֵ
    // int iCountryTemp = -1; //����������ʱ����
    int iSrcType = -1;

    std::string strMobile = szMobile;

    //ȥ��szMobile����ǰ��ġ�+��/��00��
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
    //�����һλ������1��9
    else
    {
        //�Ժ�������й�У��
        nCountry = 86;
        nType = ChinaMobileType(szMobile, iSrcType);
    }

    return 0;
}

int CParaNoMgrImpl::GetMobileCountryAndTypeEx(const char* szMobile, int& nCountry, int& nType, int& nSrcType)
{
    // int iResChinaMobileType = -1; //�й������ֻ����뷵������
    // int iResNationational = -1;  //�������ŷ���ֵ
    // int iCountryTemp = -1; //����������ʱ����

    std::string strMobile = szMobile;

    //ȥ��szMobile����ǰ��ġ�+��/��00��
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
    //�����һλ������1��9
    else
    {
        //�Ժ�������й�У��
        nCountry = 86;
        nType = ChinaMobileType(szMobile, nSrcType);
    }

    return 0;
}

//��ʼ�����ʵ绰�����������
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
