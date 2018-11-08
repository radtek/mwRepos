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

//�ⲿ����
//////////////////////////////////////////////////////////////////////////
//���ɼ�Ȩ��
//>0:�ɹ�,�����Ȩ��ĳ��� ����:ʧ��
//szUserId:�ʺ�����
//szPwd:��������
//szVerifyCode:�����֤�� ����8�ֽ�
//szAuthenCode:��Ȩ�� ����32�ֽ�
extern "C" int __STDCALL GenAuthenCode(const char* szUserId, const char* szPwd, const char* szVerifyCode, char* szAuthenCode);

//���ܶ�������
//>0:�ɹ�,������ܺ���ŵĳ��� ����:ʧ��
//szUserId:�ʺ�����
//szVerifyCode:�����֤�� ����8�ֽ�
//szInMsg:������������  ��ò�Ҫ����320������ 640���ֽ�
//szOutMsg:������������ ����ҪԤ��1024���ֽ�
extern "C" int __STDCALL EncryptMsg(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);

//���ܶ�������
//>0:�ɹ�,������ܺ���ŵĳ��� ����:ʧ��
//szUserId:�ʺ�����
//szVerifyCode:�����֤�� ����8�ֽ�
//szInMsg:������������  �������1024���ֽ�
//szOutMsg:������������ ����ҪԤ��1024���ֽ�
extern "C" int __STDCALL DecryptMsg(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);
extern "C" int __STDCALL DecryptMsgEx(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);
//////////////////////////////////////////////////////////////////////////

//�ڲ�ʹ��
//////////////////////////////////////////////////////////////////////////
//��Ȩ
//0:�ɹ� ����:ʧ��
//szUserId:�ʺ�����
//szPwd:��������
//szVerifyCode:�����֤�� ����8�ֽ�
//szAuthenCode:��Ȩ�� ����32�ֽ�
extern "C" int __STDCALL Authen(const char* szUserId, const char* szPwd, const char* szVerifyCode, const char* szAuthenCode);

//��base64��ȡ������
//>0:�ɹ�,����base64���ܺ������ ����:ʧ��
//szInMsg:����base64�����ܵ�����  ��ò�Ҫ����1024���ֽ�
//szOutMsg:�����ܵ����� ����ҪԤ��720���ֽ�
extern "C" int __STDCALL GetEncryptMsgFromBase64(const char* szInMsg, int nInMsgLen, char* szOutMsg);

//������ȡ��base64
//>0:�ɹ�,����base64�����ܺ�ĳ��� ����:ʧ��
//szInMsg:AES���ܺ������  ��ò�Ҫ����720���ֽ�
//szOutMsg:base64�����ܺ������ ����ҪԤ��1024���ֽ�
extern "C" int __STDCALL GetBase64FromEncryptMsg(const char* szInMsg, int nInMsgLen, char* szOutMsg);

//����û�о���base64�Ķ�������
//>0:�ɹ�,������ܺ���ŵĳ��� ����:ʧ��
//szUserId:�ʺ�����
//szVerifyCode:�����֤�� ����8�ֽ�
//szInMsg:������������  �������720���ֽ�
//szOutMsg:������������ ����ҪԤ��720���ֽ�
extern "C" int __STDCALL DecryptMsgNoBase64(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);

//���ܶ�������
//>0:�ɹ�,������ܺ���ŵĳ��� ����:ʧ��
//szUserId:�ʺ�����
//szVerifyCode:�����֤�� ����8�ֽ�
//szInMsg:������������  ��ò�Ҫ����320������ 640���ֽ�
//szOutMsg:������������ ����ҪԤ��1024���ֽ�
extern "C" int __STDCALL EncryptMsgNoBase64(const char* szUserId, const char* szVerifyCode, const char* szInMsg, int nInMsgLen, char* szOutMsg);
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
extern "C" int __STDCALL EmpRegister(const AUTHEN_INFO& authenInfo, const char* szAuthenCode, char* szStatusInfo);

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
extern "C" int __STDCALL EmpAuthen(AUTHEN_INFO& authenInfo);

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
extern "C" int __STDCALL GetAuthenInfo(AUTHEN_INFO& authenInfo/*_out*/);

//////////////////////////////////////////////////////////////////////////

#endif

