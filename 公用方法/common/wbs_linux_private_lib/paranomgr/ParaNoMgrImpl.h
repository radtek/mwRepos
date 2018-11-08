// ParaNoMgrImpl.h: interface for the CParaNoMgrImpl class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __PARANOMGRIMPL_H__
#define __PARANOMGRIMPL_H__

#include <paranomgr/ParaNoMgr.h>
#include "Adapter.h"

#ifdef MW_WINDOWS
#ifdef _WIN64
#define MOBILEAREAQRY_LIB        "MobiAreaQry64.dll"
#else
#define MOBILEAREAQRY_LIB        "MobiAreaQry.dll"
#endif
#elif defined LINUX
#ifdef _X64_
#define MOBILEAREAQRY_LIB        "MobiAreaQry64.so"
#else
#define MOBILEAREAQRY_LIB        "MobiAreaQry.so"
#endif
#endif

//��������
#define MAX_COUNTRY 1000000
#define CLEAR(x) memset(x, 0, sizeof(x))

//Я����Я������
struct tMNP
{
	TINYINT nSrcType;	//Я������
	TINYINT nPhoneType;	//Я������
};

//mobile����7������,area����32������
typedef int (FAR_STDCALL *_GetMobiArea)(const char* Mobile, char* Area);
class CParaNoMgrImpl: public CParaNoMgrBase
{
public:
	static CParaNoMgrImpl& GetInstance();//����ʵ����ȡ�ӿ�
	virtual ~CParaNoMgrImpl();
public:
	//���غŶα�
	virtual int LoadParaNo();

	//�����ֻ�ת����
	virtual int LoadMNP();

	//���������ֻ������
	virtual int LoadSpecialPhone(bool bForceReLoad = false);

	virtual int GetSpecPhoneCnt();

	//��ȡ���ݿⲢ��ȡ���ʵ绰���������б�
	virtual int LoadCountryList();

	//************************************
	// Description:��֤һ���ֻ����Ƿ���ͬ���͵�
	// Parameter: [IN] std::vector<std::string> vPhoneNO : һ���ֻ���
	// Returns:   int �ֻ�����(>=0:����������ͬ,���ص�ֵΪ�ֻ���������� -1:���Ͳ���ͬ -2:�����зǷ��ֻ���)
	//************************************
	virtual int IsPhoneNOIdenticalType(const std::vector<std::string>& vPhoneNO);

	virtual bool IsHaveThisType(const std::vector<std::string>& vMobiles, int nMobileType);

	//************************************
	// Description:��֤һ���ֻ����Ƿ�����ͬ������
	// Parameter: [IN] std::vector<std::string> vPhoneNO : һ���ֻ���
	// Returns:   int �ֻ�����(0:����ȫ�� 1-31:������ͬ������ -1:�����ز���ͬ)
	//************************************
	int IsPhoneNOIdenticalArea(const std::vector<std::string>& vPhoneNO);

	//************************************
	// Description:�Ŷβ�ѯ֧�ֽӿ�
	// Parameter: [IN] int &nQueryType ��ѯ����	0:�ƶ� 1:��ͨ 21:���� FE:�ֻ��Ų�ѯ
	//			  [IN] const char*szMobile �ֻ����� nQueryTypeΪFEʱ��Ч
	//			  [OUT] char *szNumSection ��ѯ���
	//			  [OUT] int &nLength ��ѯ����ֽ���
	// Returns:   0:�ɹ� ��0:ʧ��
	// Author:	  liuhuan
	//************************************
	virtual int QueryNumSection(unsigned short& nQueryType, const unsigned char* szMobile, unsigned char* szNumSection, int& nLength);

	//************************************
	// Description:�Ŷι����ز�ѯ�ӿ�(��ֻ�ܲ�ѯ������ʡ��)
	// Parameter: [IN] const char*szMobile �ֻ�����
	// Returns:   0:���������δ֪(������Ϊ������Ϊȫ��/ȫ��) 1-31:����ͬ��ʡ��
	// Author:	  jhb
	//************************************
	virtual int QueryNumAttribution(const char* szMobile);

	//************************************
	// Description:ȡ�����ֻ����������
	// Parameter: [IN] const char*szMobile �ֻ�����
	//			  [IN] const char*szUserId �ʺ�
	//			  [IN] int nCustomID �ͻ�ID
	// Returns:   0:ûʲô������ ����:����
	// Author:	  jhb
	//************************************
	virtual int QueryNumSpeciality(const char* szMobile, const char* szUserId, int nCustomId);

	//************************************
	// Description:��ȡʡ�ݴ���
	// Parameter: [IN] const char* szCityName ʡ������
	// Returns:   0:ʡ��δ֪(������Ϊ������Ϊȫ��/ȫ��) 1-31:����ͬ��ʡ��
	// Author:	  jhb
	//************************************
	virtual int GetCityCode(const char* szCityName);

	//��ȡ����ʡ�ݴ���+0
	int GetNextCityCode(int nArea);

	//************************************
	// Description:��ȡ���ʵ绰�����������
	// Parameter: [IN] const char* szInternationalMobile �����ֻ�����
	//			  [OUT] int &nCountry ��������
	// Returns:   0:�ɹ� -1:ʧ��
	// Author:	  sunzj
	//************************************
	virtual int GetCountryCode(const char* szInternationalMobile, int& nCountry);

	//************************************
	// Description:�ж��ֻ����������
	// Parameter: [IN] const char* szMobile : �ֻ��Ŵ����ʹ��롰00��/��+��
	// Returns:   int �ֻ�����(0:�ƶ� 1:��ͨ 21:�����ֻ� 5:�����ֻ� -1:����Ƿ�)
	//************************************
	virtual int MobileType(const char* szMobile);

	//************************************
	// Description:��ȡ�绰������Ҵ��������
	// Parameter: [IN] const char* szMobile  : �ֻ���
	//            [OUT] nCountry :��������
	//			  [OUT] nType(0:�ƶ� 1:��ͨ 21:�����ֻ� 5:�����ֻ� -1:����Ƿ�)
	//Returns:    0:�ɹ�   -1:ʧ��
	//************************************
	virtual int GetMobileCountryAndType(const char* szMobile, int& nCountry, int& nType);

	//************************************
	// Description:��ȡ�绰������Ҵ��������,�Լ�Я��ת��ǰ�������
	// Parameter: [IN] const char* szMobile  : �ֻ���
	//            [OUT] nCountry :��������
	//			  [OUT] nType�ֻ�����(0:�ƶ� 1:��ͨ 21:�����ֻ� 5:�����ֻ� -1:����Ƿ�)
	//			  [OUT] nSrcType�����Я��ת���Ļ�,��¼Я��ת��ǰ������,���û��,����ntype��ͬ
	//Returns:    0:�ɹ�   -1:ʧ��
	//************************************
	virtual int GetMobileCountryAndTypeEx(const char* szMobile, int& nCountry, int& nType, int& nSrcType);

private:
	CParaNoMgrImpl();

	//************************************
	// Description:װ�غŶα�
	// Parameter: [IN] std::string strMobile        : �ƶ��Ŷ�
	//            [IN] std::string strUnicom        : ��ͨ�Ŷ�
	//            [IN] std::string strTelecom       : ���źŶ�
	//            [IN] std::string strTeleom_c      : ����C���Ŷ�
	// Returns:   int                       0:�ɹ� ��0:ʧ��
	//************************************
	int SetPhonCom(std::string strMobile, std::string strUnicom, std::string strTelecom, std::string strTeleom_c);

	//�жϵ��ź���
	//************************************
	// Description:�жϵ��ŵ绰����
	// Parameter: [IN] const char* szMobile      : �绰����
	// Returns:   int                      -1:�ǵ��ź��� 2:���ź���
	//************************************
	int ChinatelType(const char* szMobile);

	//************************************
	// Description:�ж��й��ֻ����������
	// Parameter: [IN] const char* szMobile : �ֻ���
	// Returns:   int �ֻ�����(0:�ƶ� 1:��ͨ 21:�����ֻ� -1:����Ƿ�)
	//************************************
	int ChinaMobileType(const char* szMobile, int& nSrcType);

	//************************************
	// Description:��ʼ�����ʵ绰�����������
	// Parameter: [IN] int *arrCountryList ������������
	//			  [IN] int nLenCountryList �����������鳤��
	//			  [OUT] char *szCountryList ���������б�
	// Returns:   0:�ɹ� -1:ʧ��
	// Author:	  sunzj
	//************************************
	int InitCountryList(char* szCountryList, const int* arrCountryList, int nLenCountryList);

	//************************************
	// Description:�ж��ֻ����������
	// Parameter: [IN] const char* szMobile : �ֻ��Ų������ʹ���
	// Returns:   int �ֻ�����(0:�ƶ� 1:��ͨ 21:�����ֻ� 5:�����ֻ� -1:����Ƿ�)
	//************************************
	int NationalMobileType(const char* szMobile);

	//************************************
	// Description:��ȡ�绰������Ҵ��������
	// Parameter: [IN] const char* szMobile  : �ֻ���
	//            [OUT] nCountry :��������
	//			  [OUT] nType(0:�ƶ� 1:��ͨ 21:�����ֻ� 5:�����ֻ� -1:����Ƿ�)
	// Author:	  sunzj
	//************************************
	void GetNationalMobileCountryAndType(const char* szMobile, int& nCountry, int& nType, int& nSrcType);

	//ȡparano
	void GetParaNo(char ParaNo[]);

	//************************************
	// Description:�Ŷβ�ѯ����֧�ֽӿ�(����1700�Ŷ�)
	// Parameter:
	//              [OUT] unsigned short *szNumSection ��ѯ���
	//              [OUT] int &nLength ��ѯ���nNumSection���������Ĵ�С
	// Returns:   0:�ɹ� ��0:ʧ��
	// Author:      fanjie
	//************************************
	int QueryNumSection_Ex(unsigned char* szNumSection, int& nLength);

private:
	_GetMobiArea m_pGetMobiArea;	//���������DLLָ��
	char m_ParaNo[1001];			//1000���Ŷ�
	std::map<std::string, int> m_mapCity;	//31��ʡ�ݼ��Ա��ʡ��
	std::map<int, std::string> m_mapCodeCity;	//���д���
	std::map<INT64, tMNP>	   m_mapMNP;	//Я��ת��map
	std::multimap<INT64, SPEC_PHONE_ATTR>	m_mapSpecPhone; //�������map
	DerivedLock m_csMNP;					//Я��ת��map��Ӧ����
	DerivedLock m_csMobiArea;				//��������ؿ��Ӧ����
	DerivedLock m_csMobiType;				//�ֻ����Ͳ�ѯ��Ӧ����
	DerivedLock m_csSpecPhone;				//�����ֻ�������Ӧ����
	DerivedLock m_csSync;					//�Ŷ�ͬ����
//     std::string m_strMobile;    //�й��ƶ�
//     std::string m_strUnicom;    //�й���ͨ
//     std::string m_strTelecom;   //�й�����
//     std::string m_strTelecom_c;  //�й�����C��
//     std::string m_strWholeNet;  //ȫ��

	DerivedLock m_csCountryList;					//���ʵ绰���������б��Ӧ����
};

#endif // !defined(AFX_PARANOMGRIMPL_H__8D87476A_F4DE_44C6_AAFF_649E4C02BB6F__INCLUDED_)
