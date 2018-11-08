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
	// Description:ͬ���Ŷ���Ϣ
	// Parameter:
	// Returns:	   int 0:�ɹ�  >0:ADO��������ڲ������쳣
	//************************************
	static int ReadKfParams(std::string& strMobile,
	                        std::string& strUnicom,
	                        std::string& strTelecom,
	                        std::string& strTelecom_c);

	static int ReadPbServiceType(std::string& strMobile, std::string& strUnicom,
	                             std::string& strTelecom, std::string& strTelecom_c,
	                             std::string& strWholeNet);

	//************************************
	// Description:Я��ת���ֻ���Ϣ
	// Parameter: [OUT] std::map<INT64,char>& mapMNP
	// Returns:	   int 0:�ɹ�  >0:ADO��������ڲ������쳣
	//************************************
	static int ReadKfMNP(std::list<MNP_INFO>& MNP_list, int& nMaxId);

	//************************************
	// Description:��ȡ���ʵ绰���������б�
	//            [IN] countryList ���������б�
	// Returns:   int 0:�ɹ�  >0:ADO��������ڲ������쳣
	// Author:	  sunzj
	//************************************
	static int ReadCountryList(char* countryList);

	//���������ֻ������
	static int LoadSpecialPhone(std::list<SPEC_PHONE>& PhonesAdd,
	                            std::list<SPEC_PHONE>& PhonesDel, 
	                            int& nCurrID);
};

#endif // !defined(AFX_ACCESSMSSQL_H__2CBBBE03_7626_43D1_9CA4_14A5FF3A4E38__INCLUDED_)
