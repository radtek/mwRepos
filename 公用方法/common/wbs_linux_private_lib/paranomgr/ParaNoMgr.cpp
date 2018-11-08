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
/* ��ȡCParaNoMgr ����                                                */
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

	//��ȡ�ƶ��Ŷ�
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

	//��ȡ��ͨ�Ŷ�
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
	//��ȡ����C���Ŷ�
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
	//���ź��븳ֵΪ��
	strTelecom = "";
}


//************************************
// Description:��֤һ���ֻ����Ƿ���ͬ���͵�
// Parameter: [IN] std::vector<std::string> vPhoneNO : һ���ֻ���
// Returns:   int �ֻ�����(0:�����ƶ� 1:������ͨ 21:���ǵ����ֻ� -1:���Ͳ���ͬ -2:�����зǷ��ֻ���)
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
			//һ������⣬���Ƿ����쳣�����һ���ֻ������Ͳ�һ�µĺ���
			// int nTmp  = -1;
			for ( ; it != vPhoneNO.end(); ++it)
			{
				int nTmp = CParaNoMgr::GetInstance().MobileType(it->c_str());
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
