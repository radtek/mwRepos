// Eatool.cpp: implementation of the CEatool class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "eatool.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//����ʵ����ȡ�ӿ�
CEatool& CEatool::GetInstance()
{
	static CEatool instance;
	return instance;
}

CEatool::CEatool()
{
	eatool = NULL;

	IF_GenAuthenCode = NULL;
	IF_EncryptMsg = NULL;
	IF_DecryptMsg = NULL;
	IF_DecryptMsgEx = NULL;
	IF_Authen = NULL;
	IF_GetEncryptMsgFromBase64 = NULL;
	IF_GetBase64FromEncryptMsg = NULL;
	IF_DecryptMsgNoBase64 = NULL;
	IF_EncryptMsgNoBase64 = NULL;
	IF_EmpRegister = NULL;
	IF_EmpAuthen = NULL;
	IF_GetAuthenInfo = NULL;

	Init();
}

CEatool::~CEatool()
{

}

//��ʼ��
bool CEatool::Init()
{
	bool bRet = false;
	try
	{
#if  defined _WIN64
		eatool = CAdapter::LoadLibrary("eatool64.dll");
#elif (defined  MW_WINDOWS) || (defined  WIN32)
		eatool = CAdapter::LoadLibrary("eatool.dll");
#elif defined LINUX
#ifdef _X64_
		eatool = CAdapter::LoadLibrary("./libeatool64.so");
#else
		eatool = CAdapter::LoadLibrary("./libeatool.so");
#endif // _DEBUG
#endif	
		if (NULL == eatool)
		{
			//...
			printf("Open Dll Error! -- 1\n");
			bRet = false;
		}
		else
		{
			IF_GenAuthenCode = (_GenAuthenCode)CAdapter::GetProcAddress(eatool, "GenAuthenCode");
			IF_EncryptMsg = (_EncryptMsg)CAdapter::GetProcAddress(eatool, "EncryptMsg");
			IF_DecryptMsg = (_DecryptMsg)CAdapter::GetProcAddress(eatool, "DecryptMsg");
			IF_DecryptMsgEx = (_DecryptMsgEx)CAdapter::GetProcAddress(eatool, "DecryptMsgEx");
			IF_Authen = (_Authen)CAdapter::GetProcAddress(eatool, "Authen");
			IF_GetEncryptMsgFromBase64 = (_GetEncryptMsgFromBase64)CAdapter::GetProcAddress(eatool, "GetEncryptMsgFromBase64");
			IF_GetBase64FromEncryptMsg = (_GetBase64FromEncryptMsg)CAdapter::GetProcAddress(eatool, "GetBase64FromEncryptMsg");
			IF_DecryptMsgNoBase64 = (_DecryptMsgNoBase64)CAdapter::GetProcAddress(eatool, "DecryptMsgNoBase64");
			IF_EncryptMsgNoBase64 = (_EncryptMsgNoBase64)CAdapter::GetProcAddress(eatool, "EncryptMsgNoBase64");
			IF_EmpRegister = (_EmpRegister)CAdapter::GetProcAddress(eatool, "EmpRegister");
			IF_EmpAuthen = (_EmpAuthen)CAdapter::GetProcAddress(eatool, "EmpAuthen");
			IF_GetAuthenInfo = (_GetAuthenInfo)CAdapter::GetProcAddress(eatool, "GetAuthenInfo");

			if (NULL == IF_GenAuthenCode|| NULL == IF_EncryptMsg|| NULL == IF_DecryptMsg|| NULL == IF_DecryptMsgEx
				|| NULL == IF_Authen|| NULL == IF_GetEncryptMsgFromBase64|| NULL == IF_GetBase64FromEncryptMsg
				|| NULL == IF_DecryptMsgNoBase64|| NULL == IF_EncryptMsgNoBase64|| NULL == IF_EmpRegister
				|| NULL == IF_EmpAuthen|| NULL == IF_GetAuthenInfo)
			{
				//....
				printf("Open Dll Error! -- 2\n");
				bRet = false;
			}
			else
			{
				printf("Open Dll Ok!\n");
				bRet = true;
			}

		}
	}
	catch (...)
	{
		//....
		bRet = false;
	}
	
	return bRet;
}

//���ɼ�Ȩ��
//>0:�ɹ�,�����Ȩ��ĳ��� ����:ʧ��
//szUserId:�ʺ�����
//szPwd:��������
//szVerifyCode:�����֤�� ����8�ֽ�
//szAuthenCode:��Ȩ�� ����32�ֽ�
int  CEatool::GenAuthenCode(const char* szUserId, const char* szPwd, const char* szVerifyCode, char* szAuthenCode)
{
	int nRet = 0;
	if (NULL != IF_GenAuthenCode)
	{
		nRet = IF_GenAuthenCode(szUserId, szPwd, szVerifyCode, szAuthenCode);
	}
	else
	{
		nRet = -1;
	}

	return nRet;
}

//���ܶ�������
//>0:�ɹ�,������ܺ���ŵĳ��� ����:ʧ��
//szUserId:�ʺ�����
//szVerifyCode:�����֤�� ����8�ֽ�
//szInMsg:������������  ��ò�Ҫ����320������ 640���ֽ�
//szOutMsg:������������ ����ҪԤ��1024���ֽ�
int  CEatool::EncryptMsg(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg)
{
	int nRet = 0;
	if (NULL != IF_EncryptMsg)
	{
		nRet = IF_EncryptMsg(szUserId, szVerifyCode, szInMsg, nInMsgLen, szOutMsg);
	}
	else
	{
		nRet = -1;
	}

	return nRet;
}

//���ܶ�������
//>0:�ɹ�,������ܺ���ŵĳ��� ����:ʧ��
//szUserId:�ʺ�����
//szVerifyCode:�����֤�� ����8�ֽ�
//szInMsg:������������  �������1024���ֽ�
//szOutMsg:������������ ����ҪԤ��1024���ֽ�
int  CEatool::DecryptMsg(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg)
{
	int nRet = 0;
	if (NULL != IF_DecryptMsg)
	{
		nRet = IF_DecryptMsg(szUserId, szVerifyCode, szInMsg, nInMsgLen, szOutMsg);
	}
	else
	{
		nRet = -1;
	}

	return nRet;
}
int  CEatool::DecryptMsgEx(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg)
{
	int nRet = 0;
	if (NULL != IF_DecryptMsgEx)
	{
		nRet = IF_DecryptMsgEx(szUserId, szVerifyCode, szInMsg, nInMsgLen, szOutMsg);
	}
	else
	{
		nRet = -1;
	}

	return nRet;
}
//////////////////////////////////////////////////////////////////////////

//�ڲ�ʹ��
//////////////////////////////////////////////////////////////////////////
//��Ȩ
//0:�ɹ� ����:ʧ��
//szUserId:�ʺ�����
//szPwd:��������
//szVerifyCode:�����֤�� ����8�ֽ�
//szAuthenCode:��Ȩ�� ����32�ֽ�
int  CEatool::Authen(const char* szUserId, const char* szPwd, const char* szVerifyCode, const char* szAuthenCode)
{
	int nRet = 0;
	if (NULL != IF_Authen)
	{
		nRet = IF_Authen(szUserId, szPwd, szVerifyCode, szAuthenCode);
	}
	else
	{
		nRet = -1;
	}
	
	return nRet;

}

//��base64��ȡ������
//>0:�ɹ�,����base64���ܺ������ ����:ʧ��
//szInMsg:����base64�����ܵ�����  ��ò�Ҫ����1024���ֽ�
//szOutMsg:�����ܵ����� ����ҪԤ��720���ֽ�
int  CEatool::GetEncryptMsgFromBase64(const char* szInMsg, int nInMsgLen, char* szOutMsg)
{

	int nRet = 0;
	if (NULL != IF_GetEncryptMsgFromBase64)
	{
		nRet = IF_GetEncryptMsgFromBase64(szInMsg, nInMsgLen, szOutMsg);
	}
	else
	{
		nRet = -1;
	}

	return nRet;
}

//������ȡ��base64
//>0:�ɹ�,����base64�����ܺ�ĳ��� ����:ʧ��
//szInMsg:AES���ܺ������  ��ò�Ҫ����720���ֽ�
//szOutMsg:base64�����ܺ������ ����ҪԤ��1024���ֽ�
int  CEatool::GetBase64FromEncryptMsg(const char* szInMsg, int nInMsgLen, char* szOutMsg)
{
	int nRet = 0;
	if (NULL != IF_GetBase64FromEncryptMsg)
	{
		nRet = IF_GetBase64FromEncryptMsg(szInMsg, nInMsgLen, szOutMsg);
	}
	else
	{
		nRet = -1;
	}

	return nRet;

}

//����û�о���base64�Ķ�������
//>0:�ɹ�,������ܺ���ŵĳ��� ����:ʧ��
//szUserId:�ʺ�����
//szVerifyCode:�����֤�� ����8�ֽ�
//szInMsg:������������  �������720���ֽ�
//szOutMsg:������������ ����ҪԤ��720���ֽ�
int  CEatool::DecryptMsgNoBase64(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg)
{
	int nRet = 0;
	if (NULL != IF_DecryptMsgNoBase64)
	{
		nRet = IF_DecryptMsgNoBase64(szUserId, szVerifyCode, szInMsg, nInMsgLen, szOutMsg);
	}
	else
	{
		nRet = -1;
	}

	return nRet;

}

//���ܶ�������
//>0:�ɹ�,������ܺ���ŵĳ��� ����:ʧ��
//szUserId:�ʺ�����
//szVerifyCode:�����֤�� ����8�ֽ�
//szInMsg:������������  ��ò�Ҫ����320������ 640���ֽ�
//szOutMsg:������������ ����ҪԤ��1024���ֽ�
int  CEatool::EncryptMsgNoBase64(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg)
{
	int nRet = 0;
	if (NULL != IF_EncryptMsgNoBase64)
	{
		nRet = IF_EncryptMsgNoBase64(szUserId, szVerifyCode, szInMsg, nInMsgLen, szOutMsg);
	}
	else
	{
		nRet = -1;
	}

	return nRet;

}
//////////////////////////////////////////////////////////////////////////

//������кŵ���Ϣע��
//1:�ɹ�
//100�����δע��,������ע��
//101������ѵ���
//102�������֤ʧ��
//103�������ȱʧ
//104��������к����ʹ���(�����������к����Ͳ���)
//105��������кŲ��Ϸ�
//106���������
//107���յ���֤���ṹ���Ϸ�
//108��ע����Ϣ������У�鲻ͨ��
//109�����к���������֤��ʽ��ƥ��
//110����Ч��license���license����Ӧ���ڵ�ǰ������
//111����Ч��license���license����Ӧ���ڵ�ǰ������
//authenInfo:��Σ���֤��Ϣ�ṹ��
//szAuthenCode:��Σ���֤��
//szStatusInfo:���Σ�����ֵ����
int  CEatool::EmpRegister(const AUTHEN_INFO& authenInfo, const char* szAuthenCode, char* szStatusInfo)
{
	int nRet = 0;
	if (NULL != IF_EmpRegister)
	{
		nRet = IF_EmpRegister(authenInfo, szAuthenCode, szStatusInfo);
	}
	else
	{
		nRet = -1;
	}

	return nRet;

}

//������кŵ���Ϣ��֤
//1:�ɹ�
//100�����δע��,������ע��
//101������ѵ���
//102�������֤ʧ��
//103�������ȱʧ
//104��������к����ʹ���(�����������к����Ͳ���)
//105��������кŲ��Ϸ�
//106���������
//107���յ���֤���ṹ���Ϸ�
//108��ע����Ϣ������У�鲻ͨ��
//109�����к���������֤��ʽ��ƥ��
//110����Ч��license���license����Ӧ���ڵ�ǰ������
//111����Ч��license���license����Ӧ���ڵ�ǰ������
//����:ʧ��
int  CEatool::EmpAuthen(AUTHEN_INFO& authenInfo)
{
	int nRet = 0;
	if (NULL != IF_EmpAuthen)
	{
		nRet = IF_EmpAuthen(authenInfo);
	}
	else
	{
		nRet = -1;
	}

	return nRet;

}

//��ȡ������֤��Ϣ�����ؽṹ�壩
//1:�ɹ�
//100�����δע��,������ע��
//101������ѵ���
//102�������֤ʧ��
//103�������ȱʧ
//104��������к����ʹ���(�����������к����Ͳ���)
//105��������кŲ��Ϸ�
//106���������
//107���յ���֤���ṹ���Ϸ�
//108��ע����Ϣ������У�鲻ͨ��
//109�����к���������֤��ʽ��ƥ��
//110����Ч��license���license����Ӧ���ڵ�ǰ������
//111����Ч��license���license����Ӧ���ڵ�ǰ������
//����:ʧ��
int  CEatool::GetAuthenInfo(AUTHEN_INFO& authenInfo/*_out*/)
{
	int nRet = 0;
	if (NULL != IF_GetAuthenInfo)
	{
		nRet = IF_GetAuthenInfo(authenInfo);
	}
	else
	{
		nRet = -1;
	}

	return nRet;
}