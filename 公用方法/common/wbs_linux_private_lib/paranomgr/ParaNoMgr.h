// ParaNoMgr.h: interface for the CParaNoMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARANOMGR_H__AF8B94CE_733D_49D1_99B0_79F3F536FBC2__INCLUDED_)
#define AFX_PARANOMGR_H__AF8B94CE_733D_49D1_99B0_79F3F536FBC2__INCLUDED_

#include "TypeDef.h"
#include <string>

//当前的省份个数
#define MAX_AREA_NUM        31
//起始省份代码
#define INIT_AREACODE       10
//未知省份代码
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
    //加载号段表
    virtual int LoadParaNo() = 0;

    //加载手机转网表
    virtual int LoadMNP() = 0;

    //加载特殊手机号码表
    virtual int LoadSpecialPhone(bool bForceReLoad = false) = 0;

    //获取数据库并读取国际电话号码区号列表
    virtual int LoadCountryList() = 0;

    //************************************
    // Description:号段查询支持接口
    // Parameter: [IN] int &nQueryType 查询类型 0:移动 1:联通 21:电信 FE:手机号查询
    //            [IN] const char*szMobile 手机号码 nQueryType为FE时有效
    //            [OUT] char *szNumSection 查询结果
    //            [OUT] int &nLength 查询结果字节数
    // Returns:   0:成功 非0:失败
    // Author:    liuhuan
    //************************************
    virtual int QueryNumSection(unsigned short& nQueryType, const unsigned char* szMobile, unsigned char* szNumSection, int& nLength) = 0;

    //************************************
    // Description:号段归属地查询接口(暂只能查询到所在省份)
    // Parameter: [IN] const char*szMobile 手机号码
    // Returns:   0:号码归属地未知(或可理解为归属地为全球/全国) 1-31:代表不同的省份
    // Author:    jhb
    //************************************
    virtual int QueryNumAttribution(const char* szMobile) = 0;

    //************************************
    // Description:取特殊手机号码的类型
    // Parameter: [IN] const char*szMobile 手机号码
    //            [IN] const char*szUserId 帐号
    //            [IN] int nCustomID 客户ID
    // Returns:   0:没什么特殊性 其他:类型
    // Author:    jhb
    //************************************
    virtual int QueryNumSpeciality(const char* szMobile, const char* szUserId, int nCustomId) = 0;

    //************************************
    // Description:获取省份代码
    // Parameter: [IN] const char* szCityName 省份名称
    // Returns:   0:省份未知(或可理解为归属地为全球/全国) 1-31:代表不同的省份
    // Author:    jhb
    //************************************
    virtual int GetCityCode(const char* szCityName) = 0;

    //************************************
    // Description:获取国际电话号码国家区号
    // Parameter: [IN] const char* szInternationalMobile 国际手机号码
    //            [OUT] int &nCountry 国家区号
    // Returns:   0:成功 -1:失败
    // Author:    sunzj
    //************************************
    virtual int GetCountryCode(const char* szInternationalMobile, int& nCountry) = 0;

    //************************************
    // Description:判断手机号码的类型
    // Parameter: [IN] const char* szMobile       : 手机号
    // Returns:   int  手机类型(0:移动 1:联通 21:电信手机 5:国外手机 -1:号码非法)
    //************************************
    virtual int MobileType(const char* szMobile) = 0;

    //************************************
    // Description:获取电话号码国家代码和类型
    // Parameter: [IN] const char* szMobile  : 手机号
    //            [OUT] nCountry :国家区号
    //            [OUT] mType手机类型(0:移动 1:联通 21:电信手机 5:国外手机 -1:号码非法)
    //Returns:    0:成功   -1:失败
    //************************************
    virtual int GetMobileCountryAndType(const char* szMobile, int& nCountry, int& nType) = 0;

    //************************************
    // Description:验证一批手机号中是否有nMobileType类型的手机号
    //************************************
    virtual bool IsHaveThisType(const std::vector<std::string>& vMobiles, int nMobileType) = 0;

    //获取特殊手机号码的个数
    virtual int GetSpecPhoneCnt() = 0;

    //************************************
    // Description:获取电话号码国家代码和类型,以及携号转网前后的类型
    // Parameter: [IN] const char* szMobile  : 手机号
    //            [OUT] nCountry :国家区号
    //            [OUT] nType手机类型(0:移动 1:联通 21:电信手机 5:国外手机 -1:号码非法)
    //            [OUT] nSrcType如果有携号转网的话,记录携号转网前的类型,如果没有,就与ntype相同
    //Returns:    0:成功   -1:失败
    //************************************
    virtual int GetMobileCountryAndTypeEx(const char* szMobile, int& nCountry, int& nType, int& nSrcType) = 0;
};

/************************************************************************/
/*类功能：产生 CParaNoMgr 单例。                                        */
/*                                                                      */
/************************************************************************/
class CParaNoMgr
{
public:
    static CParaNoMgrBase& GetInstance();
};


/****
*检测手机号码合理性，获取手机号段
*/
class CheckMobileAndGetPhonCom
{
public:
    virtual ~CheckMobileAndGetPhonCom() {};

    static CheckMobileAndGetPhonCom & GetInstance();

    //获得电线，移动，联通手机号码的号段信息
    void GetPhonCom(std::string& strMobile, std::string& strUnicom, std::string& strTelecom, std::string& strTeleom_c);

    //************************************
    // Description:验证一批手机号是否是同类型的
    // Parameter: [IN] std::vector<std::string> vPhoneNO : 一批手机号
    // Returns:   int 手机类型(0:都是移动 1:都是联通 21:都是电信手机 -1:类型不相同 -2:当中有非法手机号)
    //************************************
    int IsPhoneNOIdenticalType(std::vector<std::string>& vPhoneNO);

private:
    CheckMobileAndGetPhonCom();
};

#endif // !defined(AFX_PARANOMGR_H__AF8B94CE_733D_49D1_99B0_79F3F536FBC2__INCLUDED_)
