// Eatool.h: interface for the CEatool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EATOOL_H__D7606668_BA52_42E1_84F5_4A6178ACE5A3__INCLUDED_)
#define AFX_EATOOL_H__D7606668_BA52_42E1_84F5_4A6178ACE5A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./adapter/Adapter.h"

//�ⲿ����
//////////////////////////////////////////////////////////////////////////
//���ɼ�Ȩ��
//>0:�ɹ�,�����Ȩ��ĳ��� ����:ʧ��
//szUserId:�ʺ�����
//szPwd:��������
//szVerifyCode:�����֤�� ����8�ֽ�
//szAuthenCode:��Ȩ�� ����32�ֽ�
typedef int (FAR_STDCALL *_GenAuthenCode)(const char* szUserId, const char* szPwd, const char* szVerifyCode, char* szAuthenCode);

//���ܶ�������
//>0:�ɹ�,������ܺ���ŵĳ��� ����:ʧ��
//szUserId:�ʺ�����
//szVerifyCode:�����֤�� ����8�ֽ�
//szInMsg:������������  ��ò�Ҫ����320������ 640���ֽ�
//szOutMsg:������������ ����ҪԤ��1024���ֽ�
typedef int (FAR_STDCALL *_EncryptMsg)(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);

//���ܶ�������
//>0:�ɹ�,������ܺ���ŵĳ��� ����:ʧ��
//szUserId:�ʺ�����
//szVerifyCode:�����֤�� ����8�ֽ�
//szInMsg:������������  �������1024���ֽ�
//szOutMsg:������������ ����ҪԤ��1024���ֽ�
typedef int (FAR_STDCALL *_DecryptMsg)(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);
typedef int (FAR_STDCALL *_DecryptMsgEx)(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);
//////////////////////////////////////////////////////////////////////////

//�ڲ�ʹ��
//////////////////////////////////////////////////////////////////////////
//��Ȩ
//0:�ɹ� ����:ʧ��
//szUserId:�ʺ�����
//szPwd:��������
//szVerifyCode:�����֤�� ����8�ֽ�
//szAuthenCode:��Ȩ�� ����32�ֽ�
typedef int (FAR_STDCALL *_Authen)(const char* szUserId, const char* szPwd, const char* szVerifyCode, const char* szAuthenCode);

//��base64��ȡ������
//>0:�ɹ�,����base64���ܺ������ ����:ʧ��
//szInMsg:����base64�����ܵ�����  ��ò�Ҫ����1024���ֽ�
//szOutMsg:�����ܵ����� ����ҪԤ��720���ֽ�
typedef int (FAR_STDCALL *_GetEncryptMsgFromBase64)(const char* szInMsg, int nInMsgLen, char* szOutMsg);

//������ȡ��base64
//>0:�ɹ�,����base64�����ܺ�ĳ��� ����:ʧ��
//szInMsg:AES���ܺ������  ��ò�Ҫ����720���ֽ�
//szOutMsg:base64�����ܺ������ ����ҪԤ��1024���ֽ�
typedef int (FAR_STDCALL *_GetBase64FromEncryptMsg)(const char* szInMsg, int nInMsgLen, char* szOutMsg);

//����û�о���base64�Ķ�������
//>0:�ɹ�,������ܺ���ŵĳ��� ����:ʧ��
//szUserId:�ʺ�����
//szVerifyCode:�����֤�� ����8�ֽ�
//szInMsg:������������  �������720���ֽ�
//szOutMsg:������������ ����ҪԤ��720���ֽ�
typedef int (FAR_STDCALL *_DecryptMsgNoBase64)(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);

//���ܶ�������
//>0:�ɹ�,������ܺ���ŵĳ��� ����:ʧ��
//szUserId:�ʺ�����
//szVerifyCode:�����֤�� ����8�ֽ�
//szInMsg:������������  ��ò�Ҫ����320������ 640���ֽ�
//szOutMsg:������������ ����ҪԤ��1024���ֽ�
typedef int (FAR_STDCALL *_EncryptMsgNoBase64)(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);
//////////////////////////////////////////////////////////////////////////

//���к�ע��&��֤
//////////////////////////////////////////////////////////////////////////
#define EA_MAX_SERIALNUM_LEN		32				//���кų���
#define EA_MAX_CORPNAME_LEN			128				//��˾���Ƴ���
#define EA_MAX_CONTACTMAN_LEN		32				//��ϵ�����ֳ���
#define EA_MAX_CONTACTPHONE_LEN		16				//��ϵ�˵绰
#define EA_MAX_EMAIL_LEN			32				//��ϵ������
#define EA_MAX_RESERVE_LEN			256				//Ԥ������
#define EA_MAX_STATUSINFO_LEN		1024			//״̬����
#define EA_MAX_AUTHENIP_LEN			32				//��֤������IP/����
#define EA_MAX_AUTHENWEBPATH_LEN	256				//��֤������WEBPATH

#pragma pack(1)
//��֤��Ϣ
struct AUTHEN_INFO 
{
	//���к�
	char szSerialNum[EA_MAX_SERIALNUM_LEN+1];
	//״̬
	int nStatus;
	//״̬����
	char szStatusInfo[EA_MAX_STATUSINFO_LEN+1];
	//������
	int nTotalDays;
	//��Ч����
	int nValidDays;
	//�ϴγɹ���֤��ʱ��
	time_t tAuthen;
	//��˾����
	char szCorpName[EA_MAX_CORPNAME_LEN+1];
	//��˾��ϵ��
	char szContactMan[EA_MAX_CONTACTMAN_LEN+1];
	//��ϵ�˵绰
	char szContactPhone[EA_MAX_CONTACTPHONE_LEN+1];
	//��ϵ������
	char szEmail[EA_MAX_EMAIL_LEN+1];
	//Ԥ��
	char szReserve[EA_MAX_RESERVE_LEN+1];
	//��֤״̬ 
	//0:������֤ 
	//1:���ذ�EMP���������ʱ�������� 
	//2:֮ǰ�ų��ı�׼��EMP�����,��ʱ��Ϊ�߼��������֤��ʽ 
	//3:����+��ͨ�������֤��ʽ 
	//4:�߼��������֤��ʽ
	//5:license��֤
	int nAuthenType;
	//��������
	int nDogSeqId;
	//�������
	int nEmpSpeed;
	//�������
	int nSoftType;
	//��֤�������˿�
	long nAuthenPort;
	//��֤������IP
	char szAuthenIp[EA_MAX_AUTHENIP_LEN+1];
	//��֤������WEBPATH
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
typedef int (FAR_STDCALL *_EmpRegister)(const AUTHEN_INFO& authenInfo, const char* szAuthenCode, char* szStatusInfo);

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
typedef int (FAR_STDCALL *_EmpAuthen)(AUTHEN_INFO& authenInfo);

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
typedef int (FAR_STDCALL *_GetAuthenInfo)(AUTHEN_INFO& authenInfo/*_out*/);

//////////////////////////////////////////////////////////////////////////

class CEatool  
{
public:
	static CEatool& GetInstance();//����ʵ����ȡ�ӿ�
	virtual ~CEatool();
private:
	CEatool();

public:
	//��ʼ��
	bool Init();

	//���ɼ�Ȩ��
	//>0:�ɹ�,�����Ȩ��ĳ��� ����:ʧ��
	//szUserId:�ʺ�����
	//szPwd:��������
	//szVerifyCode:�����֤�� ����8�ֽ�
	//szAuthenCode:��Ȩ�� ����32�ֽ�
	int  GenAuthenCode(const char* szUserId, const char* szPwd, const char* szVerifyCode, char* szAuthenCode);

	//���ܶ�������
	//>0:�ɹ�,������ܺ���ŵĳ��� ����:ʧ��
	//szUserId:�ʺ�����
	//szVerifyCode:�����֤�� ����8�ֽ�
	//szInMsg:������������  ��ò�Ҫ����320������ 640���ֽ�
	//szOutMsg:������������ ����ҪԤ��1024���ֽ�
	int  EncryptMsg(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);

	//���ܶ�������
	//>0:�ɹ�,������ܺ���ŵĳ��� ����:ʧ��
	//szUserId:�ʺ�����
	//szVerifyCode:�����֤�� ����8�ֽ�
	//szInMsg:������������  �������1024���ֽ�
	//szOutMsg:������������ ����ҪԤ��1024���ֽ�
	int  DecryptMsg(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);
	int  DecryptMsgEx(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);
	//////////////////////////////////////////////////////////////////////////

	//�ڲ�ʹ��
	//////////////////////////////////////////////////////////////////////////
	//��Ȩ
	//0:�ɹ� ����:ʧ��
	//szUserId:�ʺ�����
	//szPwd:��������
	//szVerifyCode:�����֤�� ����8�ֽ�
	//szAuthenCode:��Ȩ�� ����32�ֽ�
	int  Authen(const char* szUserId, const char* szPwd, const char* szVerifyCode, const char* szAuthenCode);

	//��base64��ȡ������
	//>0:�ɹ�,����base64���ܺ������ ����:ʧ��
	//szInMsg:����base64�����ܵ�����  ��ò�Ҫ����1024���ֽ�
	//szOutMsg:�����ܵ����� ����ҪԤ��720���ֽ�
	int  GetEncryptMsgFromBase64(const char* szInMsg, int nInMsgLen, char* szOutMsg);

	//������ȡ��base64
	//>0:�ɹ�,����base64�����ܺ�ĳ��� ����:ʧ��
	//szInMsg:AES���ܺ������  ��ò�Ҫ����720���ֽ�
	//szOutMsg:base64�����ܺ������ ����ҪԤ��1024���ֽ�
	int  GetBase64FromEncryptMsg(const char* szInMsg, int nInMsgLen, char* szOutMsg);

	//����û�о���base64�Ķ�������
	//>0:�ɹ�,������ܺ���ŵĳ��� ����:ʧ��
	//szUserId:�ʺ�����
	//szVerifyCode:�����֤�� ����8�ֽ�
	//szInMsg:������������  �������720���ֽ�
	//szOutMsg:������������ ����ҪԤ��720���ֽ�
	int  DecryptMsgNoBase64(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);

	//���ܶ�������
	//>0:�ɹ�,������ܺ���ŵĳ��� ����:ʧ��
	//szUserId:�ʺ�����
	//szVerifyCode:�����֤�� ����8�ֽ�
	//szInMsg:������������  ��ò�Ҫ����320������ 640���ֽ�
	//szOutMsg:������������ ����ҪԤ��1024���ֽ�
	int  EncryptMsgNoBase64(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);
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
	int  EmpRegister(const AUTHEN_INFO& authenInfo, const char* szAuthenCode, char* szStatusInfo);

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
	int  EmpAuthen(AUTHEN_INFO& authenInfo);

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
	int  GetAuthenInfo(AUTHEN_INFO& authenInfo/*_out*/);

private:

	HMODULE eatool;
	_GenAuthenCode IF_GenAuthenCode;
	_EncryptMsg	IF_EncryptMsg;
	_DecryptMsg IF_DecryptMsg;
	_DecryptMsgEx IF_DecryptMsgEx;
	_Authen IF_Authen;
	_GetEncryptMsgFromBase64 IF_GetEncryptMsgFromBase64;
	_GetBase64FromEncryptMsg IF_GetBase64FromEncryptMsg;
	_DecryptMsgNoBase64 IF_DecryptMsgNoBase64;
	_EncryptMsgNoBase64 IF_EncryptMsgNoBase64;
	_EmpRegister IF_EmpRegister;
	_EmpAuthen IF_EmpAuthen;
	_GetAuthenInfo IF_GetAuthenInfo;

};

#endif // !defined(AFX_EATOOL_H__D7606668_BA52_42E1_84F5_4A6178ACE5A3__INCLUDED_)
