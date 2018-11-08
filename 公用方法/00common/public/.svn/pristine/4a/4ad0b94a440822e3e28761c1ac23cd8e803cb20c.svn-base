// Eatool.cpp: implementation of the CEatool class.
//
//////////////////////////////////////////////////////////////////////
#include "utility/common/mwcorepch.h"
#include "utility/common/eatool.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//单件实例获取接口
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

//初始化
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

//生成鉴权码
//>0:成功,代表鉴权码的长度 其他:失败
//szUserId:帐号明文
//szPwd:密码明文
//szVerifyCode:随机验证码 定长8字节
//szAuthenCode:鉴权码 定义32字节
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

//加密短信内容
//>0:成功,代表加密后短信的长度 其他:失败
//szUserId:帐号明文
//szVerifyCode:随机验证码 定长8字节
//szInMsg:短信内容明文  最好不要超过320个汉字 640个字节
//szOutMsg:短信内容密文 至少要预留1024个字节
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

//解密短信内容
//>0:成功,代表解密后短信的长度 其他:失败
//szUserId:帐号明文
//szVerifyCode:随机验证码 定长8字节
//szInMsg:短信内容密文  最长不超过1024个字节
//szOutMsg:短信内容明文 至少要预留1024个字节
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

//内部使用
//////////////////////////////////////////////////////////////////////////
//鉴权
//0:成功 其他:失败
//szUserId:帐号明文
//szPwd:密码明文
//szVerifyCode:随机验证码 定长8字节
//szAuthenCode:鉴权码 定义32字节
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

//由base64码取得密文
//>0:成功,代表base64解密后的密文 其他:失败
//szInMsg:经过base64加了密的密文  最好不要超过1024个字节
//szOutMsg:解了密的密文 至少要预留720个字节
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

//由密文取得base64
//>0:成功,代表base64加了密后的长度 其他:失败
//szInMsg:AES加密后的密文  最好不要超过720个字节
//szOutMsg:base64加了密后的密文 至少要预留1024个字节
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

//解密没有经过base64的短信内容
//>0:成功,代表解密后短信的长度 其他:失败
//szUserId:帐号明文
//szVerifyCode:随机验证码 定长8字节
//szInMsg:短信内容密文  最长不超过720个字节
//szOutMsg:短信内容明文 至少要预留720个字节
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

//加密短信内容
//>0:成功,代表加密后短信的长度 其他:失败
//szUserId:帐号明文
//szVerifyCode:随机验证码 定长8字节
//szInMsg:短信内容明文  最好不要超过320个汉字 640个字节
//szOutMsg:短信内容密文 至少要预留1024个字节
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

//软件序列号等信息注册
//1:成功
//100：软件未注册,需重新注册
//101：软件已到期
//102：软件认证失败
//103：软件狗缺失
//104：软件序列号类型错误(狗类型与序列号类型不符)
//105：软件序列号不合法
//106：网络错误
//107：收到认证包结构不合法
//108：注册信息服务器校验不通过
//109：序列号类型与认证方式不匹配
//110：无效的license或该license不能应用于当前服务器
//111：无效的license或该license不能应用于当前服务器
//authenInfo:入参，认证信息结构体
//szAuthenCode:入参，认证码
//szStatusInfo:出参，返回值描述
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

//软件序列号等信息认证
//1:成功
//100：软件未注册,需重新注册
//101：软件已到期
//102：软件认证失败
//103：软件狗缺失
//104：软件序列号类型错误(狗类型与序列号类型不符)
//105：软件序列号不合法
//106：网络错误
//107：收到认证包结构不合法
//108：注册信息服务器校验不通过
//109：序列号类型与认证方式不匹配
//110：无效的license或该license不能应用于当前服务器
//111：无效的license或该license不能应用于当前服务器
//其他:失败
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

//获取本地认证信息（返回结构体）
//1:成功
//100：软件未注册,需重新注册
//101：软件已到期
//102：软件认证失败
//103：软件狗缺失
//104：软件序列号类型错误(狗类型与序列号类型不符)
//105：软件序列号不合法
//106：网络错误
//107：收到认证包结构不合法
//108：注册信息服务器校验不通过
//109：序列号类型与认证方式不匹配
//110：无效的license或该license不能应用于当前服务器
//111：无效的license或该license不能应用于当前服务器
//其他:失败
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