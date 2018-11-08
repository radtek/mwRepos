// AccessMssql.h: interface for the CAccessMssql class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCESSMSSQL_H__2CBBBE03_7626_43D1_9CA4_14A5FF3A4E38__INCLUDED_)
#define AFX_ACCESSMSSQL_H__2CBBBE03_7626_43D1_9CA4_14A5FF3A4E38__INCLUDED_

#include "TypeDef.h"
#include <string>
#include <list>

class CAccessMssql
{
public:
	CAccessMssql();
	~CAccessMssql();

public:
	//************************************
	// Description:同步号段信息
	// Parameter:
	// Returns:	   int 0:成功  >0:ADO错误或函数内部出现异常
	//************************************
	static int ReadKfParams(std::string& strMobile,
	                        std::string& strUnicom,
	                        std::string& strTelecom,
	                        std::string& strTelecom_c);

	static int ReadPbServiceType(std::string& strMobile, std::string& strUnicom,
	                             std::string& strTelecom, std::string& strTelecom_c,
	                             std::string& strWholeNet);

	//************************************
	// Description:携号转网手机信息
	// Parameter: [OUT] std::map<INT64,char>& mapMNP
	// Returns:	   int 0:成功  >0:ADO错误或函数内部出现异常
	//************************************
	static int ReadKfMNP(std::list<MNP_INFO>& MNP_list, int& nMaxId);

	//************************************
	// Description:读取国际电话号码区号列表
	//            [IN] countryList 国家区号列表
	// Returns:   int 0:成功  >0:ADO错误或函数内部出现异常
	// Author:	  sunzj
	//************************************
	static int ReadCountryList(char* countryList);

	//加载特殊手机号码表
	static int LoadSpecialPhone(std::list<SPEC_PHONE>& PhonesAdd,
	                            std::list<SPEC_PHONE>& PhonesDel, 
	                            int& nCurrID);
};

#endif // !defined(AFX_ACCESSMSSQL_H__2CBBBE03_7626_43D1_9CA4_14A5FF3A4E38__INCLUDED_)
