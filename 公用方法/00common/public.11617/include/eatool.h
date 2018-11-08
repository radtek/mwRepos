#ifndef __EATOOL_H__
#define __EATOOL_H__

#ifdef LINUX
#define __STDCALL
#include <sys/time.h>
#include <string.h>
#include <time.h>
#else
#define __STDCALL __stdcall
#endif

//外部导出
//////////////////////////////////////////////////////////////////////////
//生成鉴权码
//>0:成功,代表鉴权码的长度 其他:失败
//szUserId:帐号明文
//szPwd:密码明文
//szVerifyCode:随机验证码 定长8字节
//szAuthenCode:鉴权码 定义32字节
extern "C" int __STDCALL GenAuthenCode(const char* szUserId, const char* szPwd, const char* szVerifyCode, char* szAuthenCode);

//加密短信内容
//>0:成功,代表加密后短信的长度 其他:失败
//szUserId:帐号明文
//szVerifyCode:随机验证码 定长8字节
//szInMsg:短信内容明文  最好不要超过320个汉字 640个字节
//szOutMsg:短信内容密文 至少要预留1024个字节
extern "C" int __STDCALL EncryptMsg(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);

//解密短信内容
//>0:成功,代表解密后短信的长度 其他:失败
//szUserId:帐号明文
//szVerifyCode:随机验证码 定长8字节
//szInMsg:短信内容密文  最长不超过1024个字节
//szOutMsg:短信内容明文 至少要预留1024个字节
extern "C" int __STDCALL DecryptMsg(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);
extern "C" int __STDCALL DecryptMsgEx(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);
//////////////////////////////////////////////////////////////////////////

//内部使用
//////////////////////////////////////////////////////////////////////////
//鉴权
//0:成功 其他:失败
//szUserId:帐号明文
//szPwd:密码明文
//szVerifyCode:随机验证码 定长8字节
//szAuthenCode:鉴权码 定义32字节
extern "C" int __STDCALL Authen(const char* szUserId, const char* szPwd, const char* szVerifyCode, const char* szAuthenCode);

//由base64码取得密文
//>0:成功,代表base64解密后的密文 其他:失败
//szInMsg:经过base64加了密的密文  最好不要超过1024个字节
//szOutMsg:解了密的密文 至少要预留720个字节
extern "C" int __STDCALL GetEncryptMsgFromBase64(const char* szInMsg, int nInMsgLen, char* szOutMsg);

//由密文取得base64
//>0:成功,代表base64加了密后的长度 其他:失败
//szInMsg:AES加密后的密文  最好不要超过720个字节
//szOutMsg:base64加了密后的密文 至少要预留1024个字节
extern "C" int __STDCALL GetBase64FromEncryptMsg(const char* szInMsg, int nInMsgLen, char* szOutMsg);

//解密没有经过base64的短信内容
//>0:成功,代表解密后短信的长度 其他:失败
//szUserId:帐号明文
//szVerifyCode:随机验证码 定长8字节
//szInMsg:短信内容密文  最长不超过720个字节
//szOutMsg:短信内容明文 至少要预留720个字节
extern "C" int __STDCALL DecryptMsgNoBase64(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);

//加密短信内容
//>0:成功,代表加密后短信的长度 其他:失败
//szUserId:帐号明文
//szVerifyCode:随机验证码 定长8字节
//szInMsg:短信内容明文  最好不要超过320个汉字 640个字节
//szOutMsg:短信内容密文 至少要预留1024个字节
extern "C" int __STDCALL EncryptMsgNoBase64(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);
//////////////////////////////////////////////////////////////////////////

//序列号注册&认证
//////////////////////////////////////////////////////////////////////////
#define EA_MAX_SERIALNUM_LEN		32				//序列号长度
#define EA_MAX_CORPNAME_LEN			128				//公司名称长度
#define EA_MAX_CONTACTMAN_LEN		32				//联系人名字长度
#define EA_MAX_CONTACTPHONE_LEN		16				//联系人电话
#define EA_MAX_EMAIL_LEN			32				//联系人邮箱
#define EA_MAX_RESERVE_LEN			256				//预留长度
#define EA_MAX_STATUSINFO_LEN		1024			//状态描述
#define EA_MAX_AUTHENIP_LEN			32				//认证服务器IP/域名
#define EA_MAX_AUTHENWEBPATH_LEN	256				//认证服务器WEBPATH

#pragma pack(1)
//认证信息
struct AUTHEN_INFO 
{
	//序列号
	char szSerialNum[EA_MAX_SERIALNUM_LEN+1];
	//状态
	int nStatus;
	//状态描述
	char szStatusInfo[EA_MAX_STATUSINFO_LEN+1];
	//总天数
	int nTotalDays;
	//有效天数
	int nValidDays;
	//上次成功认证的时间
	time_t tAuthen;
	//公司名称
	char szCorpName[EA_MAX_CORPNAME_LEN+1];
	//公司联系人
	char szContactMan[EA_MAX_CONTACTMAN_LEN+1];
	//联系人电话
	char szContactPhone[EA_MAX_CONTACTPHONE_LEN+1];
	//联系人邮箱
	char szEmail[EA_MAX_EMAIL_LEN+1];
	//预留
	char szReserve[EA_MAX_RESERVE_LEN+1];
	//认证状态 
	//0:网络认证 
	//1:网关版EMP软件狗，暂时不做考虑 
	//2:之前放出的标准版EMP软件狗,暂时作为高级软件狗认证方式 
	//3:网络+普通软件狗认证方式 
	//4:高级软件狗认证方式
	//5:license认证
	int nAuthenType;
	//软件狗序号
	int nDogSeqId;
	//软件流速
	int nEmpSpeed;
	//软件类型
	int nSoftType;
	//认证服务器端口
	long nAuthenPort;
	//认证服务器IP
	char szAuthenIp[EA_MAX_AUTHENIP_LEN+1];
	//认证服务器WEBPATH
	char szAuthenWebPath[EA_MAX_AUTHENWEBPATH_LEN+1];
	AUTHEN_INFO()
    {
        memset(this, 0, sizeof(AUTHEN_INFO));
		time(&tAuthen);
    }
	
    AUTHEN_INFO& operator=(const AUTHEN_INFO& other)
    {
        if (this == &other)
        {
            return *this;
        }
        memset(this, 0, sizeof(AUTHEN_INFO));
        memcpy(this, &other, sizeof(AUTHEN_INFO));
        return *this;
    }
};
#pragma pack()

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
extern "C" int __STDCALL EmpRegister(const AUTHEN_INFO& authenInfo, const char* szAuthenCode, char* szStatusInfo);

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
extern "C" int __STDCALL EmpAuthen(AUTHEN_INFO& authenInfo);

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
extern "C" int __STDCALL GetAuthenInfo(AUTHEN_INFO& authenInfo/*_out*/);

//////////////////////////////////////////////////////////////////////////

#endif

