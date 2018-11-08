// ParaNoMgr.cpp: implementation of the CParaNoMgr class.
//
//////////////////////////////////////////////////////////////////////
#include <paranomgr/ParaNoMgr.h>
#include <paranomgr/ParaNoMgrImpl.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParaNoMgrBase::CParaNoMgrBase()
{

}

CParaNoMgrBase::~CParaNoMgrBase()
{

}


/************************************************************************/
/* 获取CParaNoMgr 单例                                                */
/************************************************************************/
CParaNoMgrBase& CParaNoMgr::GetInstance()
{
	return CParaNoMgrImpl::GetInstance();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CheckMobileAndGetPhonCom::CheckMobileAndGetPhonCom()
{
}

CheckMobileAndGetPhonCom& CheckMobileAndGetPhonCom::GetInstance()
{
	static CheckMobileAndGetPhonCom sign;
	return sign;
}


void CheckMobileAndGetPhonCom::GetPhonCom(std::string& strMobile, std::string& strUnicom, std::string& strTelecom, std::string& strTeleom_c)
{
	unsigned char szNumSection[1024] = {0};
	int nLength = 0;
	unsigned short nMobileType = 0;
	unsigned short nUnicomType = 1;
	unsigned short nTeleom_cType = 21;

	//获取移动号段
	if ( 0 ==  CParaNoMgr::GetInstance().QueryNumSection(nMobileType, NULL, szNumSection, nLength))
	{
		for (int i = 0; i < nLength; ++i)
		{
			char buffer[16] = { 0 };
			snprintf(buffer, sizeof(buffer), "%d,", szNumSection[i]);
			strMobile += buffer;
		}
		strMobile.erase(strMobile.find_last_not_of(',') + 1);
		memset(szNumSection, 0, sizeof(szNumSection));
	}

	//获取联通号段
	if ( 0 ==  CParaNoMgr::GetInstance().QueryNumSection(nUnicomType, NULL, szNumSection, nLength))
	{
		for (int i = 0; i < nLength; ++i)
		{
			char buffer[16] = { 0 };
			snprintf(buffer, sizeof(buffer), "%d,", szNumSection[i]);
			strUnicom += buffer;
		}
		strUnicom.erase(strUnicom.find_last_not_of(',') + 1);
		memset(szNumSection, 0, sizeof(szNumSection));
	}
	//获取电信C网号段
	if ( 0 ==  CParaNoMgr::GetInstance().QueryNumSection(nTeleom_cType, NULL, szNumSection, nLength))
	{
		for (int i = 0; i < nLength; ++i)
		{
			char buffer[16] = { 0 };
			snprintf(buffer, sizeof(buffer), "%d,", szNumSection[i]);
			strTeleom_c += buffer;
		}
		strTeleom_c.erase(strTeleom_c.find_last_not_of(',') + 1);
		memset(szNumSection, 0, sizeof(szNumSection));
	}
	//电信号码赋值为空
	strTelecom = "";
}


//************************************
// Description:验证一批手机号是否是同类型的
// Parameter: [IN] std::vector<std::string> vPhoneNO : 一批手机号
// Returns:   int 手机类型(0:都是移动 1:都是联通 21:都是电信手机 -1:类型不相同 -2:当中有非法手机号)
//************************************
int CheckMobileAndGetPhonCom::IsPhoneNOIdenticalType(std::vector<std::string>& vPhoneNO)
{
	int nRet = -1;

	if (!vPhoneNO.empty())
	{
		std::vector<std::string>::iterator it = vPhoneNO.begin();
		int nType = CParaNoMgr::GetInstance().MobileType(it->c_str());
		if (-1 != nType)
		{
			nRet = nType;
			//一个个检测，看是否有异常或与第一个手机号类型不一致的号码
			// int nTmp  = -1;
			for ( ; it != vPhoneNO.end(); ++it)
			{
				int nTmp = CParaNoMgr::GetInstance().MobileType(it->c_str());
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
