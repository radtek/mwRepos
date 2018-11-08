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

//最大国家数
#define MAX_COUNTRY 1000000
#define CLEAR(x) memset(x, 0, sizeof(x))

//携出和携入类型
struct tMNP
{
	TINYINT nSrcType;	//携出类型
	TINYINT nPhoneType;	//携入类型
};

//mobile至少7个长度,area至少32个长度
typedef int (FAR_STDCALL *_GetMobiArea)(const char* Mobile, char* Area);
class CParaNoMgrImpl: public CParaNoMgrBase
{
public:
	static CParaNoMgrImpl& GetInstance();//单件实例获取接口
	virtual ~CParaNoMgrImpl();
public:
	//加载号段表
	virtual int LoadParaNo();

	//加载手机转网表
	virtual int LoadMNP();

	//加载特殊手机号码表
	virtual int LoadSpecialPhone(bool bForceReLoad = false);

	virtual int GetSpecPhoneCnt();

	//获取数据库并读取国际电话号码区号列表
	virtual int LoadCountryList();

	//************************************
	// Description:验证一批手机号是否是同类型的
	// Parameter: [IN] std::vector<std::string> vPhoneNO : 一批手机号
	// Returns:   int 手机类型(>=0:代表种类相同,返回的值为手机号码的类型 -1:类型不相同 -2:当中有非法手机号)
	//************************************
	virtual int IsPhoneNOIdenticalType(const std::vector<std::string>& vPhoneNO);

	virtual bool IsHaveThisType(const std::vector<std::string>& vMobiles, int nMobileType);

	//************************************
	// Description:验证一批手机号是否是相同归属地
	// Parameter: [IN] std::vector<std::string> vPhoneNO : 一批手机号
	// Returns:   int 手机类型(0:都是全网 1-31:代表相同归属地 -1:归属地不相同)
	//************************************
	int IsPhoneNOIdenticalArea(const std::vector<std::string>& vPhoneNO);

	//************************************
	// Description:号段查询支持接口
	// Parameter: [IN] int &nQueryType 查询类型	0:移动 1:联通 21:电信 FE:手机号查询
	//			  [IN] const char*szMobile 手机号码 nQueryType为FE时有效
	//			  [OUT] char *szNumSection 查询结果
	//			  [OUT] int &nLength 查询结果字节数
	// Returns:   0:成功 非0:失败
	// Author:	  liuhuan
	//************************************
	virtual int QueryNumSection(unsigned short& nQueryType, const unsigned char* szMobile, unsigned char* szNumSection, int& nLength);

	//************************************
	// Description:号段归属地查询接口(暂只能查询到所在省份)
	// Parameter: [IN] const char*szMobile 手机号码
	// Returns:   0:号码归属地未知(或可理解为归属地为全球/全国) 1-31:代表不同的省份
	// Author:	  jhb
	//************************************
	virtual int QueryNumAttribution(const char* szMobile);

	//************************************
	// Description:取特殊手机号码的类型
	// Parameter: [IN] const char*szMobile 手机号码
	//			  [IN] const char*szUserId 帐号
	//			  [IN] int nCustomID 客户ID
	// Returns:   0:没什么特殊性 其他:类型
	// Author:	  jhb
	//************************************
	virtual int QueryNumSpeciality(const char* szMobile, const char* szUserId, int nCustomId);

	//************************************
	// Description:获取省份代码
	// Parameter: [IN] const char* szCityName 省份名称
	// Returns:   0:省份未知(或可理解为归属地为全球/全国) 1-31:代表不同的省份
	// Author:	  jhb
	//************************************
	virtual int GetCityCode(const char* szCityName);

	//获取所有省份代码+0
	int GetNextCityCode(int nArea);

	//************************************
	// Description:获取国际电话号码国家区号
	// Parameter: [IN] const char* szInternationalMobile 国际手机号码
	//			  [OUT] int &nCountry 国家区号
	// Returns:   0:成功 -1:失败
	// Author:	  sunzj
	//************************************
	virtual int GetCountryCode(const char* szInternationalMobile, int& nCountry);

	//************************************
	// Description:判断手机号码的类型
	// Parameter: [IN] const char* szMobile : 手机号带国际冠码“00”/“+”
	// Returns:   int 手机类型(0:移动 1:联通 21:电信手机 5:国外手机 -1:号码非法)
	//************************************
	virtual int MobileType(const char* szMobile);

	//************************************
	// Description:获取电话号码国家代码和类型
	// Parameter: [IN] const char* szMobile  : 手机号
	//            [OUT] nCountry :国家区号
	//			  [OUT] nType(0:移动 1:联通 21:电信手机 5:国外手机 -1:号码非法)
	//Returns:    0:成功   -1:失败
	//************************************
	virtual int GetMobileCountryAndType(const char* szMobile, int& nCountry, int& nType);

	//************************************
	// Description:获取电话号码国家代码和类型,以及携号转网前后的类型
	// Parameter: [IN] const char* szMobile  : 手机号
	//            [OUT] nCountry :国家区号
	//			  [OUT] nType手机类型(0:移动 1:联通 21:电信手机 5:国外手机 -1:号码非法)
	//			  [OUT] nSrcType如果有携号转网的话,记录携号转网前的类型,如果没有,就与ntype相同
	//Returns:    0:成功   -1:失败
	//************************************
	virtual int GetMobileCountryAndTypeEx(const char* szMobile, int& nCountry, int& nType, int& nSrcType);

private:
	CParaNoMgrImpl();

	//************************************
	// Description:装载号段表
	// Parameter: [IN] std::string strMobile        : 移动号段
	//            [IN] std::string strUnicom        : 联通号段
	//            [IN] std::string strTelecom       : 电信号段
	//            [IN] std::string strTeleom_c      : 电信C网号段
	// Returns:   int                       0:成功 非0:失败
	//************************************
	int SetPhonCom(std::string strMobile, std::string strUnicom, std::string strTelecom, std::string strTeleom_c);

	//判断电信号码
	//************************************
	// Description:判断电信电话号码
	// Parameter: [IN] const char* szMobile      : 电话号码
	// Returns:   int                      -1:非电信号码 2:电信号码
	//************************************
	int ChinatelType(const char* szMobile);

	//************************************
	// Description:判断中国手机号码的类型
	// Parameter: [IN] const char* szMobile : 手机号
	// Returns:   int 手机类型(0:移动 1:联通 21:电信手机 -1:号码非法)
	//************************************
	int ChinaMobileType(const char* szMobile, int& nSrcType);

	//************************************
	// Description:初始化国际电话号码国家区号
	// Parameter: [IN] int *arrCountryList 国际区号数组
	//			  [IN] int nLenCountryList 国际区号数组长度
	//			  [OUT] char *szCountryList 国家区号列表
	// Returns:   0:成功 -1:失败
	// Author:	  sunzj
	//************************************
	int InitCountryList(char* szCountryList, const int* arrCountryList, int nLenCountryList);

	//************************************
	// Description:判断手机号码的类型
	// Parameter: [IN] const char* szMobile : 手机号不带国际冠码
	// Returns:   int 手机类型(0:移动 1:联通 21:电信手机 5:国外手机 -1:号码非法)
	//************************************
	int NationalMobileType(const char* szMobile);

	//************************************
	// Description:获取电话号码国家代码和类型
	// Parameter: [IN] const char* szMobile  : 手机号
	//            [OUT] nCountry :国家区号
	//			  [OUT] nType(0:移动 1:联通 21:电信手机 5:国外手机 -1:号码非法)
	// Author:	  sunzj
	//************************************
	void GetNationalMobileCountryAndType(const char* szMobile, int& nCountry, int& nType, int& nSrcType);

	//取parano
	void GetParaNo(char ParaNo[]);

	//************************************
	// Description:号段查询所有支持接口(包括1700号段)
	// Parameter:
	//              [OUT] unsigned short *szNumSection 查询结果
	//              [OUT] int &nLength 查询结果nNumSection数组结果集的大小
	// Returns:   0:成功 非0:失败
	// Author:      fanjie
	//************************************
	int QueryNumSection_Ex(unsigned char* szNumSection, int& nLength);

private:
	_GetMobiArea m_pGetMobiArea;	//号码归属地DLL指针
	char m_ParaNo[1001];			//1000个号段
	std::map<std::string, int> m_mapCity;	//31个省份及自编号省码
	std::map<int, std::string> m_mapCodeCity;	//城市代码
	std::map<INT64, tMNP>	   m_mapMNP;	//携号转网map
	std::multimap<INT64, SPEC_PHONE_ATTR>	m_mapSpecPhone; //特殊号码map
	DerivedLock m_csMNP;					//携号转网map对应的锁
	DerivedLock m_csMobiArea;				//号码归属地库对应的锁
	DerivedLock m_csMobiType;				//手机类型查询对应的锁
	DerivedLock m_csSpecPhone;				//特殊手机号码表对应的锁
	DerivedLock m_csSync;					//号段同步锁
//     std::string m_strMobile;    //中国移动
//     std::string m_strUnicom;    //中国联通
//     std::string m_strTelecom;   //中国电信
//     std::string m_strTelecom_c;  //中国电信C网
//     std::string m_strWholeNet;  //全网

	DerivedLock m_csCountryList;					//国际电话号码区号列表对应的锁
};

#endif // !defined(AFX_PARANOMGRIMPL_H__8D87476A_F4DE_44C6_AAFF_649E4C02BB6F__INCLUDED_)
