// ParaNoMgr.h: interface for the CParaNoMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARANOMGR_H__AF8B94CE_733D_49D1_99B0_79F3F536FBC2__INCLUDED_)
#define AFX_PARANOMGR_H__AF8B94CE_733D_49D1_99B0_79F3F536FBC2__INCLUDED_

#include "TypeDef.h"
#include <string>

//��ǰ��ʡ�ݸ���
#define MAX_AREA_NUM        31
//��ʼʡ�ݴ���
#define INIT_AREACODE       10
//δ֪ʡ�ݴ���
#define UNKNOWN_AREACODE    0

/************************************************************************/
/* CParaNoMgr                                                           */
/************************************************************************/
class CParaNoMgrBase
{
public:
    CParaNoMgrBase();
    virtual ~CParaNoMgrBase();

public:
    //���غŶα�
    virtual int LoadParaNo() = 0;

    //�����ֻ�ת����
    virtual int LoadMNP() = 0;

    //���������ֻ������
    virtual int LoadSpecialPhone(bool bForceReLoad = false) = 0;

    //��ȡ���ݿⲢ��ȡ���ʵ绰���������б�
    virtual int LoadCountryList() = 0;

    //************************************
    // Description:�Ŷβ�ѯ֧�ֽӿ�
    // Parameter: [IN] int &nQueryType ��ѯ���� 0:�ƶ� 1:��ͨ 21:���� FE:�ֻ��Ų�ѯ
    //            [IN] const char*szMobile �ֻ����� nQueryTypeΪFEʱ��Ч
    //            [OUT] char *szNumSection ��ѯ���
    //            [OUT] int &nLength ��ѯ����ֽ���
    // Returns:   0:�ɹ� ��0:ʧ��
    // Author:    liuhuan
    //************************************
    virtual int QueryNumSection(unsigned short& nQueryType, const unsigned char* szMobile, unsigned char* szNumSection, int& nLength) = 0;

    //************************************
    // Description:�Ŷι����ز�ѯ�ӿ�(��ֻ�ܲ�ѯ������ʡ��)
    // Parameter: [IN] const char*szMobile �ֻ�����
    // Returns:   0:���������δ֪(������Ϊ������Ϊȫ��/ȫ��) 1-31:����ͬ��ʡ��
    // Author:    jhb
    //************************************
    virtual int QueryNumAttribution(const char* szMobile) = 0;

    //************************************
    // Description:ȡ�����ֻ����������
    // Parameter: [IN] const char*szMobile �ֻ�����
    //            [IN] const char*szUserId �ʺ�
    //            [IN] int nCustomID �ͻ�ID
    // Returns:   0:ûʲô������ ����:����
    // Author:    jhb
    //************************************
    virtual int QueryNumSpeciality(const char* szMobile, const char* szUserId, int nCustomId) = 0;

    //************************************
    // Description:��ȡʡ�ݴ���
    // Parameter: [IN] const char* szCityName ʡ������
    // Returns:   0:ʡ��δ֪(������Ϊ������Ϊȫ��/ȫ��) 1-31:����ͬ��ʡ��
    // Author:    jhb
    //************************************
    virtual int GetCityCode(const char* szCityName) = 0;

    //************************************
    // Description:��ȡ���ʵ绰�����������
    // Parameter: [IN] const char* szInternationalMobile �����ֻ�����
    //            [OUT] int &nCountry ��������
    // Returns:   0:�ɹ� -1:ʧ��
    // Author:    sunzj
    //************************************
    virtual int GetCountryCode(const char* szInternationalMobile, int& nCountry) = 0;

    //************************************
    // Description:�ж��ֻ����������
    // Parameter: [IN] const char* szMobile       : �ֻ���
    // Returns:   int  �ֻ�����(0:�ƶ� 1:��ͨ 21:�����ֻ� 5:�����ֻ� -1:����Ƿ�)
    //************************************
    virtual int MobileType(const char* szMobile) = 0;

    //************************************
    // Description:��ȡ�绰������Ҵ��������
    // Parameter: [IN] const char* szMobile  : �ֻ���
    //            [OUT] nCountry :��������
    //            [OUT] mType�ֻ�����(0:�ƶ� 1:��ͨ 21:�����ֻ� 5:�����ֻ� -1:����Ƿ�)
    //Returns:    0:�ɹ�   -1:ʧ��
    //************************************
    virtual int GetMobileCountryAndType(const char* szMobile, int& nCountry, int& nType) = 0;

    //************************************
    // Description:��֤һ���ֻ������Ƿ���nMobileType���͵��ֻ���
    //************************************
    virtual bool IsHaveThisType(const std::vector<std::string>& vMobiles, int nMobileType) = 0;

    //��ȡ�����ֻ�����ĸ���
    virtual int GetSpecPhoneCnt() = 0;

    //************************************
    // Description:��ȡ�绰������Ҵ��������,�Լ�Я��ת��ǰ�������
    // Parameter: [IN] const char* szMobile  : �ֻ���
    //            [OUT] nCountry :��������
    //            [OUT] nType�ֻ�����(0:�ƶ� 1:��ͨ 21:�����ֻ� 5:�����ֻ� -1:����Ƿ�)
    //            [OUT] nSrcType�����Я��ת���Ļ�,��¼Я��ת��ǰ������,���û��,����ntype��ͬ
    //Returns:    0:�ɹ�   -1:ʧ��
    //************************************
    virtual int GetMobileCountryAndTypeEx(const char* szMobile, int& nCountry, int& nType, int& nSrcType) = 0;
};

/************************************************************************/
/*�๦�ܣ����� CParaNoMgr ������                                        */
/*                                                                      */
/************************************************************************/
class CParaNoMgr
{
public:
    static CParaNoMgrBase& GetInstance();
};


/****
*����ֻ���������ԣ���ȡ�ֻ��Ŷ�
*/
class CheckMobileAndGetPhonCom
{
public:
    virtual ~CheckMobileAndGetPhonCom() {};

    static CheckMobileAndGetPhonCom & GetInstance();

    //��õ��ߣ��ƶ�����ͨ�ֻ�����ĺŶ���Ϣ
    void GetPhonCom(std::string& strMobile, std::string& strUnicom, std::string& strTelecom, std::string& strTeleom_c);

    //************************************
    // Description:��֤һ���ֻ����Ƿ���ͬ���͵�
    // Parameter: [IN] std::vector<std::string> vPhoneNO : һ���ֻ���
    // Returns:   int �ֻ�����(0:�����ƶ� 1:������ͨ 21:���ǵ����ֻ� -1:���Ͳ���ͬ -2:�����зǷ��ֻ���)
    //************************************
    int IsPhoneNOIdenticalType(std::vector<std::string>& vPhoneNO);

private:
    CheckMobileAndGetPhonCom();
};

#endif // !defined(AFX_PARANOMGR_H__AF8B94CE_733D_49D1_99B0_79F3F536FBC2__INCLUDED_)
