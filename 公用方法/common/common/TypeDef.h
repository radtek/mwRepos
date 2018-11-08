#ifndef _TYPEDEF_H
#define _TYPEDEF_H
//#pragma warning (disable:4103)
//#pragma warning (disable:4503)
//#pragma warning (disable:4786)
//nclude "adapter/Adapter.h"
#include <vector>
#include <string>
#include <map>
#include <deque>
//zhubo 2017-4-7
//#include <winsock2.h>
#include <algorithm>
#include <list>
#include <time.h>
#include <string>
#include <wbs_config.h>
//nclude "ngx_http_mw_define.h"
using namespace std;

//#pragma pack(1)



#ifndef FAR_STDCALL                 //����windows��linux����dllʱ���÷�ʽ�Ĳ�ͬ
#define FAR_STDCALL
#endif

#ifndef FAR_STDCALL                 //����windows��linux����dllʱ���÷�ʽ�Ĳ�ͬ
#define FAR_STDCALL far __stdcall
#endif


#define  LOG_WBSDLL   "ProtoTool.txt"

#define  MAX_MTRSP_LEN        1024*2

#define  MAX_PUSH_PRE_LEN     1024       //����mo��rptǰ�벿�ֵ���󳤶�

#define  MAXLEN       MAX_MSG_LEN*3*3

#ifndef MCHECK_INVALID_VALUE
#define MCHECK_INVALID_VALUE(nData) {\
if (nData < 0) \
		{			   \
		nData = 0; \
		}			   \
}
#endif

#ifndef MCHECK_INVALID_VALUE_EX
#define MCHECK_INVALID_VALUE_EX(nData, nMinData, nMaxData) {\
if (nData < (nMinData))     \
		{							\
		nData = (nMinData);		\
		}							\
		else if (nData >(nMaxData))	\
		{							\
		nData = (nMaxData);		\
		}							\
}
#endif

#define EPSINON  0.000001           //�������������Χ
#define MONEY_CON 1000000

//MISMG�����붨��
#define BASE_ER_CODE				 300					    //ͳһ��������ʼֵ
#define ER_MISMG_SUCCESS             0						    //�ɹ�
#define ER_MISMG_PACKER              (BASE_ER_CODE+1)           //��Ϣ��ʽ����
#define ER_MISMG_NOTLOGINED          (BASE_ER_CODE+52)          //��δ�ɹ���¼
#define ER_MISMG_NOTBINDCHANNEL      (BASE_ER_CODE+53)          //û�а�ͨ��


//��������״̬
enum 
{
	ONLINE,			//����
	OFFLINE,		//����
	ONLINE_UNKNOWN	//δ֪
};

//���ͨ��״̬
enum
{
	STATE_OK,		//����
	ULTRA_FLOW		//������
};

//�û���½״̬
enum 
{
	CONNECTED,		//������
	LOGINED,		//�ѵ�½
	DISCONNECTED	//δ����
};

//�û�����
enum 
{
	PREUSER,	 //ǰ���û�
	ENDSPGATE,	 //���SpGate
	TYPE_UNKNOWN //δ֪����
};

//
//��־SOCKET�ĵ�ǰ״̬
//
enum
{
	SOCKET_STATE_NOT_CONNECT,
	SOCKET_STATE_CONNECT,
	SOCKET_STATE_CONNECT_AND_READ,
	SOCKET_STATE_TIMEOUT,
	SOCKET_WAIT_CLOSE
};

//
//��ɶ˿���ɵ��¼�
//
typedef enum
{
	IOCP_COMPLETE_READ,
	IOCP_COMPLETE_ACCEPT_READ,
	IOCP_COMPLETE_WRITE,
	IOCP_COMPLETE_ACCEPT
} IOCP_COMPLETE_STATE;

//
//�Զ���ö���������ͣ�������ʶ�׽���IO��������
//
typedef enum 
{
	IOCP_INIT,
    IOCP_ACCEPT, //AcceptEx/accept
	IOCP_CONNECT,
	IOCP_CONNWRITE,
	IOCP_READ,   //WSARecv/recv/ReadFile
	IOCP_WRITE,   //WSASend/send/WriteFile
	IOCP_WRITE_OVER,
	IOCP_READ_OVER,
	IOCP_END
} IOCP_OPERATION, *IOCP_OPERATION_PTR;

//socket״̬
typedef enum 
{
	SOCK_STATE_INIT,
	SOCK_STATE_CONNECTING,
	SOCK_STATE_CONNECTED,
	SOCK_STATE_READED,
	SOCK_STATE_LOGINING,
	SOCK_STATE_LOGINED,
	SOCK_STATE_NEEDCLOSE,
	SOCK_STATE_OK,
	SOCK_STATE_PENDING,
	SOCK_STATE_END
} SOCKET_STATE;

//MT״̬
enum 
{
	MT_TERMINAL_STATUS,						//0:CMPPGW�յ�״̬���棬���ɹ����͸�ǰ��SpGate
	CMPPGW_MT_TO_SPGATE_FAILED,				//1:CMPPGW�յ�ǰ��SpGate��Submit����û�гɹ����������SpGate
	CMPPGW_MT_TO_SPGATE_SUCCESS,			//2:CMPPGW�յ�ǰ��SpGate��Submit�����ɹ����������SpGate
	SPGATE_MT_TO_CHINAMOBILE_SUCCESS,		//3:���SpGate�ɹ���Submit��Ͷ�ݸ��ƶ���ҵ����
	SPGATE_RECV_RPT_SUCCESS,				//4:�յ��ƶ����ص�״̬����
	SPGATE_RPT_TO_CMPPGW_SUCCESS,			//5:��״̬����ɹ����͸�CMPPGW
	ERROR_MT,								//6:�쳣���ţ��ֻ����쳣/û�а�ͨ��/Ƿ�ѵȣ�
	VERIFY_MT,								//7:��Ҫ��˵Ķ���(��״̬����Ϣֻд�⣬������)
	DISCARDRPT_MT,							//8:�������е�״̬���汻����
	MODIFYRPT_MT,							//9:�������е�״̬���汻ǿ���޸�ΪDELIVRD
	NULL_MT									//10:�������е�����Ϊ��
};

//MT�ڲ������־
enum
{
	FLAG_RECV_MT = 1,						//1:�յ��û�submit
	FLAG_WRITE_MTDB_SUCCESS,				//2:д��ɹ������ӷ��ͻ��壬���ȴ�submit_rspͶ�ݽ��
	FLAG_MTRSP_SUCCESS,						//3:submit_rspͶ�ݳɹ����ȴ�ת�������SpGate���
	FLAG_MTRSP_FALIED,						//4:submit_rspͶ��ʧ�ܵȴ�д���̴߳���
	FLAG_MT_TO_SPGATE_FAILED				//5:Ͷ�ݸ����SpGateʧ��
};

//MO״̬
enum
{
	MO_TERMINAL_STATUS,					//0:CMPPGW�յ��û��ɹ����յĻ�Ӧ
	SPGATE_MO_TO_CMPPGW_FAILED,			//1:���SpGate�յ�mo(�ֻ����ж���)���͸�CMPPGW��δ�ɹ�
	SPGATE_MO_TO_CMPPGW_SUCCESS,		//2:���SpGate�յ�mo(�ֻ����ж���)���͸�CMPPGW�ɹ�
	CMPPGW_MO_TO_USER_SUCCESS,			//3:CMPPGW�յ����SpGate���͹������ֻ�mo������ת����ǰ��SpGate,��δ�ɹ�����Ϊ״̬1
};

//MO�ڲ�����״̬
enum
{
	FLAG_RECV_MO = 1,					//1:�յ����spgate������mo
	FLAG_MORSP_SUCCESS,					//2:�����spgate��Ӧmorsp�ɹ�
	FLAG_MO_TO_USER_SUCCESS,			//3:����mo��ǰ��spgate�ɹ�
	FLAG_MO_TO_USER_FAILED,				//4:����mo��ǰ��spgateʧ�ܣ������ݿ�״̬Ϊ1
	FLAG_RECV_MORSP						//5:�յ�ǰ��spgate�����ĳɹ�����mo��Ӧ��
};

//Mo/Rpt��־
enum
{
	FLAG_MO,							//�ֻ�MO
	FLAG_REPORT,						//״̬����
	FLAG_MORPT							//mo&rpt
};

//WBS��������
enum
{
	WBS_GET_WSDL = 0,
	WBS_MT_SINGLE,//����
	WBS_MT_EXNO,
	WBS_MT_LONGMSG,//Ⱥ��
	WBS_MO_GET,
	WBS_RPT_GET,
	WBS_QUERY,
	WBS_QUERY_STATUS,                 //add sp 2012-03-17 ��ѯ������״̬
	WBS_SEND_SUBMIT,                  //add sp 2012-03-28 ���û��Զ�����ˮ�ŷ��ͽӿ�
	WBS_SEND_MULTIX,                  //add sp 2012-10-31 �û����Ի�Ⱥ��״̬
	WBS_FILEUPLOAD_REQ,
	WBS_DELIVER_GETMO,				  //��ȡmo��Ϣ
	WBS_DELIVER_GETRPT,				  //��ȡrpt��Ϣ
	WBS_DELIVER_GETBOTH,			  //ͬʱ��ȡmo��rpt��Ϣ
	WBS_DELIVER_GETCHANNEL,			  //��ȡͨ����Ϣspgate
	WBS_DELIVER_GETOTHER,		      //
	WBS_DELIVER_MOBILESEG,          //�ֻ��Ŷ�
	WBS_HSBC_MT_REQ,
	WBS_HTTP_OK,
	WBS_HTTP_ERR,
	WBS_SEND_TEMPLATE,				//ģ�巢�ͽӿ�
	WBS_SEND_MIX,					 //zhubo 2017-12-8
	WBS_SHORT_ADDRESS,                           //zhubo 2018-03-04 
	WBS_MOBILE_INFO,
	WBS_SEND_TPL,				//����ģ�巢�ͽӿ�
};

//WBS�����Ӧ����
enum
{
	WBS_MTREQ_RSP=0,
	WBS_MOGET_RSP,
	WBS_RPTGET_RSP,
	WBS_WSDLGET_RSP,//WSDLGET ������Ӧ
	WBS_QUERY_RSP,
	WBS_FILEUPLOAD_RSP,
	WBS_DELIVERGETMO_RSP,
	WBS_DELIVERGETRPT_RSP,
	WBS_DELIVERGETBOTH_RSP,
	WBS_DELIVERGETCHANNEL_RSP,
	WBS_DELIVERGETOTHER_RSP,
	WBS_HSBC_MTREQ_RSP,
};

//WBS����ģʽ
enum
{
	WBS_SOAP_POST=0,	//Ĭ��Ϊsoap_postģʽ
	WBS_HTTP_POST,
	WBS_HTTP_GET
};

//����WBS����/��Ӧ����
enum COMMAND
{
	MT_REQUEST=1,		//����������Ϣ
	MT_RESPONSE,		//������Ӧ��Ϣ
	MULTI_MT_REQUEST,	//��������������Ϣ(��ͬ����)
	MULTI_MT_RESPONSE,	//����������Ӧ��Ϣ(��ͬ����)
	MULTIX_MT_REQUEST,	//��������������Ϣ(��ͬ����)
	MULTIX_MT_RESPONSE,	//����������Ӧ��Ϣ(��ͬ����)
	BATCH_MT_REQUEST,	//�ļ�Ⱥ������������Ϣ
	BATCH_MT_RESPONSE,	//�ļ�Ⱥ��������Ӧ��Ϣ
	MO_REQUEST,			//����������Ϣ
	MO_RESPONSE,		//������Ӧ��Ϣ
	RT_REQUEST,			//״̬����������Ϣ
	RT_RESPONSE,		//״̬������Ӧ��Ϣ
	MULTIXMO_REQUEST,			//����������Ϣ
	MULTIXMO_RESPONSE,		//������Ӧ��Ϣ
	MULTIXRPT_REQUEST,			//״̬����������Ϣ
	MULTIXRPT_RESPONSE,		//״̬������Ӧ��Ϣ
	ERROR_RESPONSE		//������Ӧ��Ϣ
};

//���ʱ�־
enum
{
	UNKNOWNPAY=0,
	PREPAY,
	POSTPAY
};

//�۷ѷ�ʽ
enum
{
	MSGNUM = 0,
	MSGMONEY = 1,
	MSGNUM_MONEY   = 2 //��ͨѶר��
};


//�ƷѶ���
enum
{
	USERID_CHARGE = 0,
	ECID_CHARGE
};

enum
{
	ROUTE_FORCE=0,
	ROUTE_SUPPSEND,
	ROUTE_RESEND,
	ROUTE_SECTIONNO,
	ROUTE_PROPORTION
};

enum
{
	RUNMODE_CMPP=0,
	RUNMODE_WBS
};

enum
{
	WBS_V1,  //����Э��
	WBS_OLD,
	WBS_V2,
	WBS_V2_5,//  2.0�汾����һ��5.5�汾Э�飬ר����֧��UTF-8����
	WBS_V3
};

typedef enum
{
	HCT_NONE       =  0x00,    //None Ĭ��ֵ
	HCT_JSON_TEXT  =  0x01,   //text/json
	HCT_JSON_APP   =  0x02,   //application/json
	HCT_XML_TEXT   =  0x03,   //text/xml
	HCT_XML_APP    =  0x04	   //application/xml
}HTTP_CONTENT_TYPE;

#ifndef TINYINT
#define TINYINT    unsigned char
#endif

//����������е�����
enum CMD_TASK_TYPE
{
	CMD_TASK_UNKNOWN = 0,	//δ֪
	CMD_TASK_SMSTMPL = 1,	//����ģ�����
	CMD_TASK_USRINFO = 6,	//�û���Ϣ���
	CMD_TASK_SPGATEBIND = 7,	//�û�ͨ������Ϣ���
	CMD_TASK_GATEBIND = 8,	//ͨ����Ϣ���
	CMD_TASK_SPNOSWITCH = 10,	//ͨ���л����
	CMD_TASK_MTROUTE = 11,	//ͨ��·����Ϣ
	CMD_TASK_SMSROUTE = 12,	//ģ��·����Ϣ
	CMD_TASK_SVRROUTE = 13,	//ҵ������·��
	CMD_TASK_ADC = 14,			//ADC�Ӻ���Ϣ
	CMD_TASK_ENDBIND = 15,	//ͨ���ʺŰ���Ϣ
	CMD_TASK_FLASHBIND = 16,	//���Ű󶨱�
	CMD_TASK_GATEGROUP = 17,	//ͨ���������Ϣ
	CMD_TASK_FEE = 18	//���䶯ָ��

};

//�����������
enum CMD_OPTYPE
{
	CMD_OP_UNKNOWN = 0,		//δ֪
	CMD_OP_ADD = 1,			//����
	CMD_OP_DEL = 2,			//ɾ��
	CMD_OP_MODIFY = 3,		//�޸�
	CMD_OP_CLEARALLBY = 4,	//���ָ������������
	CMD_OP_CLEARALL = 5,		//���������ָ����������Ӧ��������Ϣ
	CMD_OP_RELOADALLBY = 6,	//���¼���ָ������������
	CMD_OP_RELOADALL = 7		//���������¼���ָ����������Ӧ��������Ϣ
};


enum PACK_SUBMIT_ENCODE_FLAG
{
	PSEF_GBK    = 0x01,
	PSEF_BASE64 = 0x02
};

//zhubo 2017-5-23
//ָ����Ϣ
struct CMD_TASK_INFO
{
	INT64 nID;
	int nCmdType;
	int nOptType;
	std::string strCmd;
	CMD_TASK_INFO()
	{
		nID = 0;
		nCmdType = 0;
		nOptType = 0;
	}

	friend bool operator<(const CMD_TASK_INFO& a, const CMD_TASK_INFO& b)
	{
		bool bLess = true;

		//��ID����
		if (a.nID == b.nID)
		{
			bLess = false;
		}
		else
		{
			//����
			bLess = (a.nID < b.nID
				&& !(a.nCmdType == b.nCmdType
				&& a.nOptType == b.nOptType
				&& a.strCmd == b.strCmd));
		}

		return bLess;
	}

};
//zhubo 2017-5-23



//////////////////////////////////////////////////////////////////////////
//Э��
#define CMPPV2_0   ((BYTE)0x20)
#define CMPPV3_0   ((BYTE)0x30)
#define EMPPV2_0   ((BYTE)0x40)
#define EMPPSHV2_0 ((BYTE)0x50)
//CMPP����
#define CMPP_CONNECT                     0x00000001	//��������
#define CMPP_CONNECT_RESP                0x80000001	//��������Ӧ��
#define CMPP_TERMINATE                   0x00000002	//��ֹ����
#define CMPP_TERMINATE_RESP              0x80000002	//��ֹ����Ӧ��
#define CMPP_SUBMIT                      0x00000004	//�ύ����
#define CMPP_SUBMIT_RESP                 0x80000004	//�ύ����Ӧ��
#define CMPP_DELIVER                     0x00000005	//�����·�
#define CMPP_DELIVER_RESP                0x80000005	//�·�����Ӧ��
#define CMPP_QUERY                       0x00000006	//���Ͷ���״̬��ѯ
#define CMPP_QUERY_RESP                  0x80000006	//���Ͷ���״̬��ѯӦ��
#define CMPP_CANCEL                      0x00000007	//ɾ������
#define CMPP_CANCEL_RESP                 0x80000007	//ɾ������Ӧ��
#define CMPP_ACTIVE_TEST                 0x00000008	//�������
#define CMPP_ACTIVE_TEST_RESP            0x80000008	//�������Ӧ��
#define CMPP_FWD                         0x00000009	//��Ϣǰת
#define CMPP_FWD_RESP                    0x80000009	//��ϢǰתӦ��
#define CMPP_MT_ROUTE                    0x00000010	//MT·������
#define CMPP_MT_ROUTE_RESP               0x80000010	//MT·������Ӧ��
#define CMPP_MO_ROUTE                    0x00000011	//MO·������
#define CMPP_MO_ROUTE_RESP               0x80000011	//MO·������Ӧ��
#define CMPP_GET_ROUTE                   0x00000012	//��ȡ·������
#define CMPP_GET_ROUTE_RESP              0x80000012	//��ȡ·������Ӧ��
#define CMPP_MT_ROUTE_UPDATE             0x00000013	//MT·�ɸ���
#define CMPP_MT_ROUTE_UPDATE_RESP        0x80000013	//MT·�ɸ���Ӧ��
#define CMPP_MO_ROUTE_UPDATE             0x00000014	//MO·�ɸ���
#define CMPP_MO_ROUTE_UPDATE_RESP        0x80000014	//MO·�ɸ���Ӧ��
#define CMPP_PUSH_MT_ROUTE_UPDATE        0x00000015	//MT·�ɸ���
#define CMPP_PUSH_MT_ROUTE_UPDATE_RESP   0x80000015	//MT·�ɸ���Ӧ��
#define CMPP_PUSH_MO_ROUTE_UPDATE        0x00000016	//MO·�ɸ���
#define CMPP_PUSH_MO_ROUTE_UPDATE_RESP   0x80000016	//MO·�ɸ���Ӧ��
#define CMPP_QUERY_USERFEE				 0x00009901 //�ʺŷ��������ѯ
#define CMPP_QUERY_USERFEE_RESP			 0x80009901 //�ʺŷ��������ѯӦ��
#define CMPP_QUERY_CHANNELINFO			 0x00009902 //�ʺ�ͨ���������ѯ
#define CMPP_QUERY_CHANNELINFO_RESP		 0x80009902 //�ʺ�ͨ���������ѯӦ��

//EMPP����
#define EMPP_SESSION                     0x000000FF //����Ựid
#define EMPP_CONNECT                     0x00000001 //��¼
#define EMPP_CONNECT_RESP                0x80000001 //��¼Ӧ��
#define EMPP_TERMINATE                   0x00000002 //�˳���¼
#define EMPP_TERMINATE_RESP              0x80000002 //�˳���¼Ӧ��
#define EMPP_SUBMIT                      0x00000004 //�ύ����
#define EMPP_SUBMIT_RESP                 0x80000004 //�ύ����Ӧ��
#define EMPP_DELIVER                     0x00000005 //�ݽ�MO
#define EMPP_DELIVER_RESP                0x80000005 //�ݽ�MOӦ��
#define EMPP_REPORT                      0x00000006 //״̬����
#define EMPP_REPORT_RESP                 0x80000006 //״̬����Ӧ��
#define EMPP_ACTIVE_TEST                 0x00000007 //�����·״̬
#define EMPP_ACTIVE_TEST_RESP            0x80000007 //�����·״̬Ӧ��
#define EMPP_UPDATE_PSWD                 0x00000008 //�޸ĵ�¼��������
#define EMPP_UPDATE_PSWD_RESP            0x80000008 //�޸ĵ�¼��������Ӧ��
#define EMPPSH_ACTIVE_TEST               0x00000008 //�����·״̬
#define EMPPSH_ACTIVE_TEST_RESP          0x80000008 //�����·״̬Ӧ��
//�û�������Ȩֵ����
#define PRIVILEGE_NONE					 0x00000000		//û����Ȩ����ͨ�û�
#define PRIVILEGE_MSGID					 0x00000001		//�Ա�MSGIDȨ��		PRIVILEGE�ֶε�1λ
#define PRIVILEGE_AGENT					 0x00000002		//�����ʺ�Ȩ��		PRIVILEGE�ֶε�2λ
#define PRIVILEGE_MONITOR				 0x00000004		//����ʺ�			PRIVILEGE�ֶε�3λ
#define PRIVILEGE_SENDLEVEL				 0x00000008		//�Զ��巢�ͼ���	PRIVILEGE�ֶε�4λ
#define PRIVILEGE_NORPTNEED				 0x00000010		//��������RPT		PRIVILEGE�ֶε�5λ
#define PRIVILEGE_MSG_VERIFY			 0x00000020		//��Ҫ��Ϣ���		PRIVILEGE�ֶε�6λ
#define PRIVILEGE_KEYWRODS_FILTER		 0x00000040		//��Ҫ�ؼ��ֹ���	PRIVILEGE�ֶε�7λ
#define PRIVILEGE_RESEND_CTRL			 0x00000080		//��Ҫ�ط�����		PRIVILEGE�ֶε�8λ
#define PRIVILEGE_MODIFY_RPT			 0x00000100		//��Ҫ��һ����ʧ�ܵ�״̬����ǿ�Ƹ�Ϊdelivrd		PRIVILEGE�ֶε�9λ
#define PRIVILEGE_NOMONEED				 0x00000200		//��������Mo		PRIVILEGE�ֶε�10λ

//////////////////////////////////////////////////////////////////////////
#define SEND_WINDOES_SIZE                50           //���ʹ��ڴ�С
#define SLIDING_WINDOES_SIZE             1000         //���ʹ��ڴ�С
//////////////////////////////////////////////////////////////////////////
//�����С
#define	MAX_RECBUF_LEN				600*1024	//�����ջ���(����������С)
#define MAX_WBS_RSPPACK_LEN			300*1024	//wbs��Ӧ������󳤶�
#define BUFFER_SIZE_SEND			300*1024	//��ɶ˿�һ�η��͵Ĵ�С
#define BUFFER_SIZE_RECV			32*1024		//��ɶ˿�һ�ν��յĴ�С
#define	MAX_PACK_LEN				500*1024	//һ������������ڴ�ֵ
#define MAX_PACK_LEN_V5				630*1024    //V5һ������������ڴ�ֵ
#define POOL_BUF_SIZE				100*1024	//�ڴ�ص��������С
#define MAX_HTTP_LEN                2048        //HTTPЭ��������󳤶� 
#define MAX_USERDATA_LEN            720         //�����û�������Ϣ����
//////////////////////////////////////////////////////////////////////////
//�ֽڳ��ȵ�
#define	MAX_MO_MSG_LEN				360 
#define MAXTHREAD_COUNT				8			//��ɶ˿ڿ������߳���
#define MAX_THREAD_DELAY			5			//ÿ���̵߳ȴ�ʱ��
#define MAX_MTLEVEL_NUM				10			//mt�ֶ��ٸ�����(mt0/mt1/mt2..mt9)
#define MAX_MOLEVEL_NUM				2			//mo�ֶ��ٸ�����(mo/rpt)
#define MAX_PHONE_NUM				5000//1000		//���֧��Ⱥ�����ֻ�����
#define MAX_HTTP_PHONE_NUM			100	    	//HTTP���֧��Ⱥ�����ֻ�����
#define MAX_MULTIX_PHONE_NUM		200			//���֧�ָ��Ի�Ⱥ�����ֻ�����
#define MAX_SMALL_PACK_NUM			30			//С����Ⱥ����������
#define MAX_ROW_NUM					1000		//�ļ�����ʱÿ���ļ����������
#define MAX_CHANNEL_LEN				21			//ͨ������
#define MAX_PHONE_LEN				24			//�ֻ���Ԥ����󳤶�
#define SIGN_MAX_PHONE_LEN			21			//�����ֻ�����󳤶�
//#define MAX_MSG_LEN					360			//ϵͳ֧�ֵ����������������֣�(720�ֽ�)
//#define	MAX_SPLIT_MSG_LEN			360
#define	MAX_MO_MSG_LEN				360 
//zhubo 2017-6-12
#define MAX_MSG_LEN					1000			//ϵͳ֧�ֵ����������������֣�(720�ֽ�)
#define	MAX_SPLIT_MSG_LEN			1000 


#define MAX_SINGLE_LEN			    70			//����������󳤶ȣ����֣�
#define MAX_MULTILEN1				67			//�����ŵ�һ����󳤶ȣ����֣�
#define MAX_MULTILEN2				67			//�����ŵڶ�����󳤶ȣ����֣�
#define MAX_SIGN_LEN				10			//ǩ��������������֣�(20�ֽ�)
#define MAX_IPADDR_LEN				18			//IP��ַ��󳤶�
#define MAX_LOGINIP_LEN				256			//IP��ַ��󳤶�
#define MAX_DOMAIN_LEN				256			//IP��ַ��󳤶�
#define MAX_PWD_LEN					256//32			//�������󳤶�
#define MAX_MSGIDSTR_LEN			21			//MsgIdת��Ϊint64����ַ�������
#define MAX_TITLE_LEN				100			//���ͱ���ĳ���
#define MAX_TIME_LEN				30			//YY-MM-DD HH:MM:SS:SSSS
#define MAX_RSPPACK_LEN				128			//cmppЭ���Ӧ����󳤶�
#define MAX_PARAMS_LEN				128			//·�ɱ��в����ֶ���󳤶�
#define MAX_CHANNELINFO_LEN			64			//ͨ������Ϣ��󳤶�
#define MAX_WBS_ATTIME_LEN			14			//Wbs��attime����
#define MAX_WBS_VALIDTIME_LEN		14			//Wbs��validtime����
#define MAX_WBS_ERCODE_LEN			3			//Wbs�д�����ĳ���
#define CHANNEL_TYPE_ARR			5			//�ж�����ͨ������(��Ӫ������)	
#define CHANNEL_SEQ_ARR				100			//ÿ���û�ÿ�����͵�ͨ�����԰���ٸ�
#define MAX_COPRNAME_LEN			128			//��ҵ����
#define OFFLINE_DELAY			    5			//������ʱʱ��
#define MAX_PUSH_WAIT				30			//������Ӧ��ʱʱ��
#define MAX_OFFLINE_TIME			86400		//�����ʱ��
#define MAX_BINDIP_NUM			    5			//�û������԰󶨶��ٸ�IP
#define MAX_RISELEVEL				9			//�������������
#define MAX_URL_LEN					512
#define MAX_CA_NAME                 255

#define MAX_LOG_LEVEL_DEFAULT		0			//Ĭ����־����4   ��¼�϶����Ϣ
#define MAX_LOG_LEVEL_LOW			1			//����־����1     ��¼������־
#define MAX_LOG_LEVEL_MID			2			//����־����2	  ������¼MAX_LOG_LEVEL_LOW����,���Ҽ�¼������û������İ�
#define MAX_LOG_LEVEL_HIGH			3			//����־����3	  ������¼MAX_LOG_LEVEL_LOW����,���Ҽ�¼��spgate�����İ�
#define MAX_LOG_LEVEL_HIGHER		4			//�ϸ���־����4   ��¼֮ǰ���м���İ�
#define MAX_LOG_LEVEL_HIGHEST		5			//�����־����5	  ��¼�ǳ��ٵİ�

#define PRECISION					10000		//����
#define MAX_LEN_KEYWORDS			50			//�ؼ��ֳ���

#define MAX_DESTID_LEN				21			//DESTID�ֶεĳ���(�ֽ�)
#define MAX_SRCID_LEN				21			//SRCID�ֶεĳ���(�ֽ�)
#define MAX_AUTHENSRC_LEN			16			//Դַ�����ִ�����(�ֽ�)
#define MAX_MSGID_LEN				8			//MsgId�ĳ���(�ֽ�)
#define MAX_SPID_LEN				6			//��ҵSP������󳤶�(�ֽ�)
#define MAX_SRVID_LEN				10			//ҵ���ʶ����(�ֽ�)
#define MAX_FEETMID20_LEN			21			//Fee_terminal_Id CMPP20����(�ֽ�)
#define MAX_FEETMID30_LEN			32			//Fee_terminal_Id CMPP30����(�ֽ�)
#define MAX_FEETYPE_LEN				2			//FeeType����(�ֽ�)
#define MAX_FEECODE_LEN				6			//FeeCode����(�ֽ�)
#define MAX_ATTIME_LEN				17			//At_time����(�ֽ�)
#define MAX_VALIDTIME_LEN			17			//valid_time����(�ֽ�)
#define MAX_RESERVE_LEN				8			//Ԥ���ֶγ���(�ֽ�)
#define MAX_LINKID_LEN				20			//CMPP30��linkid����(�ֽ�)
#define MAX_DESTTMID20_LEN			21			//CMPP20��Dest_terminal_Id����ID�ĳ���(�ֽ�)
#define MAX_DESTTMID30_LEN			32			//CMPP30��Dest_terminal_Id����ID�ĳ���(�ֽ�)

#define MAX_ERSTAT_LEN				7			//״̬���������ĳ���(�ֽ�)
#define MAX_SUBMITTM_LEN			10			//Submit_time ����(�ֽ�)
#define MAX_DONETM_LEN				10			//Done_time ����(�ֽ�)
#define MAX_SMSCSEQ_LEN				4			//SMSC_sequence ����(�ֽ�)
#define MAX_SRCTMID20_LEN			21			//CMPP20��Src_terminal_Id����ID�ĳ���(�ֽ�)
#define MAX_SRCTMID30_LEN			32			//CMPP30��Src_terminal_Id����ID�ĳ���(�ֽ�)

#define MAX_COMMAND_LEN             128         //���յ������������

#define MAX_PUSH_COUNT              1000000000  //��¼��������ʹ���
#define MAX_TIME_STAMP              20//10 
#define MAX_CUSTID_LEN              64
#define MAX_SVRTYPE_LEN             32
#define MAX_PATH_FUNC               100
#define MAX_TMPLCONTENT_LEN			1000
#define MAX_RMS_TMPLCONTENT_LEN		(2000-64)
#define MAX_RMS_TMPLCONTENT_V20_LEN		(3000-64)

#define MAX_LOG_LEN                 1024
#define MAX_HTTP_MSG_LEN            4098
//////////////////////////////////////////////////////////////////////////
//CMPP�����붨��
#define ER_CMPP_SUCCESS				0			//�ɹ�
#define ER_CMPP_PACKER				1			//��Ϣ��ʽ����
#define ER_CMPP_NOTLOGINED			52			//��δ�ɹ���¼
#define ER_CMPP_NOTBINDCHANNEL		53			//û�а�ͨ��
//��½
#define ER_CMPP_CONNECT_SPIDER		2			//�Ƿ�Դַ
#define ER_CMPP_CONNECT_AUTHENER	3			//��֤����
#define ER_CMPP_CONNECT_VERSIONER	4			//�汾̫��
#define ER_CMPP_CONNECT_DBER		5			//�ڲ����ݿ����
#define ER_CMPP_CONNECT_LOGINTOOFAST 6			//ǰ�����ε�½���̫��
#define ER_CMPP_CONNECT_MUCHLINK	56			//���ӳ���
#define ER_CMPP_CONNECT_ERIP		57			//SP��½IP��
#define ER_WBS_CONNECT_ERIP		    -10057		//SP��½IP��
#define ER_WBS_CONNECT_OVERFLOW	    -10056		//SP��½����������

#define ER_WBS_OVERFEE                  -100003     //Ƿ��
#define ER_WBS_CUSTID_EXDATA            -100004     //custid����exdata���Ϸ�
#define ER_WBS_OVERMSG                  -100011     //�������ݳ���
#define ER_WBS_MOBILE                   -100012     //�ֻ����벻�Ϸ�
#define ER_WBS_OVERMOBILE               -100014     //�ֻ����볬��10000
#define ER_WBS_SPBIND                   -100029     //�˿ڰ�ʧ��
#define ER_WBS_CONNECT_ERIP_V5		    -100057		//SP��½IP��
#define ER_WBS_CONNECT_OVERFLOW_V5	    -100056		//SP��½����������
#define ER_WBS_SYSTEM                   -100999     //ϵͳ�ڲ�����
#define ER_WBS_NO_TEMPLATE              -100058     //�Ҳ���ģ��
#define ER_WBS_TMPL_ARGCNT              -100059     //ģ�����������ƥ��
#define ER_WBS_MOBILE_CONTENT_COUNT		-100060 	//�ֻ�������Ϣ���ݸ�����һ��
#define ER_WBS_SVRTYPE                  -100252     //svrtype���Ϸ�

#define ER_WBS_RMS_SEND_ERROR            -100500     //���Žӿڷ��Ǹ���
#define ER_WBS_RMS_CONTENT_OVERFLOW      -100501     //���Žӿ����ݹ���
#define ER_WBS_TMPL_DISABLE				 -100502     //����ģ�屻����

#define ER_WBS_SHORT_URL_FAILED				 -100800     //�̵�ַ��ؽӿ�ʧ��
#define ER_WBS_SHORT_URL_LONG_ADDR_ERROR     -100801     //����ַ����
//����ַ�������ڶ�ַ������  �������Ϊ      -100802
#define ER_WBS_SHORT_URL_DOMAIN_ERROR        -100803     //����������
#define ER_WBS_SHORT_URL_DOMAIN_DISABLED     -100804     //�������Ѿ�������
#define ER_WBS_SHORT_URL_MOBILE_TOO_LONG     -100805     //�ֻ����볬�� 10000��
//�̵�ַ������Դ������  �����ڶ̵�ַ����
//#define ER_WBS_SHORT_URL_MOBILE_TOO_LONG     -100806    //�̵�ַ������Դ����
#define ER_WBS_SHORT_URL_SERVER_BUZY     	  -100807     //������æ

#define ER_WBS_MOBILE_INFO_MOBILE_COUNT     -100900     //�ֻ������������
#define ER_WBS_MOBILE_INFO_FAILED           -100901     //��ȡ�ֻ�������Ϣʧ��
#define ER_WBS_CHARGE_MINFO_FAILED          -100902     //�۷�����ʧ��

#define ER_WBS_MOBILE_INFO_SERVER_BUZY     	-100907     //������æ





//submit
#define ER_CMPP_SUBMIT_CMDER		2			//�����ִ���
#define ER_CMPP_SUBMIT_MSGOVERLONG	6			//���ų���
#define ER_CMPP_SUBMIT_SRCIDER		9			//ͨ���Ű󶨴���
#define ER_CMPP_SUBMIT_DESTIDER		13			//�ֻ�����Ƿ�
#define ER_CMPP_SUBMIT_SENDMIXEDER	14			//�������Ϸ���
#define ER_CMPP_SUBMIT_PKNUMER		21			//PKNUMBER/PKTOTAL����
#define ER_CMPP_SUBMIT_FEEINFOER	206			//�ƶ��û��ʻ������쳣
#define ER_CMPP_SUBMIT_OWEFEEER		208			//�û�����
#define ER_CMPP_SUBMIT_OWEFEEMOREER	210			//�������Ƿ�Ѷ�
#define ER_CMPP_SUBMIT_GENERALERROR	255			//SUBMIT�������,������������ƽ̨
#define ER_CMPP_SUBMIT_ILLEGALMSG	42			//��Ϣ���ݲ���ȫ
#define ER_CMPP_SUBMIT_DUPLICATEMSG	43			//�ظ���ͬһ�ֻ��ݽ���ͬ���ŵ�Ƶ�ʹ���
#define ER_CMPP_SUBMIT_NULLMT		44			//��������Ϊ��
//FWD
#define ER_CMPP_FWD_CMDER			2			//�����ִ�
#define ER_CMPP_FWD_OVERFLOWER		8			//������
#define ER_CMPP_FWD_FEEINFOER		206			//�ƶ��û��ʻ������쳣
#define ER_CMPP_FWD_OWEFEEER		208			//�û�����
#define ER_CMPP_FWD_OWEFEEMOREER	210			//�������Ƿ�Ѷ�
#define ER_CMPP_FWD_OTHERER_9		9			//��������9
#define ER_CMPP_FWD_OTHERER_169		169			//��������169
#define ER_CMPP_FWD_OTHERER_10		10			//��������10
#define ER_CMPP_FWD_OTHERER_69		69			//��������69
#define ER_CMPP_FWD_OTHERER_13		13			//��������13
#define ER_CMPP_FWD_OTHERER_6		6			//��������6

#define RPT_FILE_NUM                80          //ÿ��RPT�ļ�����RPT������
#define MO_FILE_NUM                 25          //ÿ��MO�ļ�����MO������
#define RPT_HIS_OVERTIME            3           //RptPackHis������ʱ��70�룬������д���ļ�,��Ϊ��ʱ60���һ�����м�����ʷ����

//////////////////////////////////////////////////////////////////////////
//���ò��������ֵ����Сֵ����
//////////////////////////////////////////////////////////////////////////
#define MW_PARAM_MAX_MONFREQ        3600        //���Ƶ�����ֵ
#define MW_PARAM_MIN_MONFREQ        3           //���Ƶ����Сֵ
#define MW_PARAM_DEF_MONFREQ        10			//���Ƶ��Ĭ��ֵ

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//MT�ļ��д�ŵ����ݽṹ���ֵ
#define DATA_MT_MAXNUM		1000

//////////////////////////////////////////////////////////////////////////

#define MAX_SERTYPE_LEN 32
#define MAX_EXDATA_LEN 64
#define MAX_APIKEY_LEN 512
//zhubo 2017-5-18
#define MAX_TMPLID_LEN 64
#define MAX_MSGTYPE_LEN 6


#define MAX_MSG_LEN_RECV MAX_MSG_LEN*3*3+100
#define MAX_MOBILES_LEN MAX_PHONE_LEN*MAX_PHONE_NUM
#define MAX_MIFO_MOBILES_LEN MAX_PHONE_LEN*5000



#define MAX_MO_USERID_LEN   256
#define MAX_RPT_USERID_LEN  256

#define MAX_RMS_SHOWWAY_LEN  16
#define MAX_RMS_SHOWTIME_LEN  32
#define MAX_RMS_TITLE_LEN  40

#define RMS_MSG_TYPE  11

//��������
enum REMAIN_TYPE
{
	REMAINT_MEM = 0,		//�ڴ�
	REMAINT_FILE,			//�ļ�
	REMAINT_DT_REMAIN,		//mtlvl/motask/rptwaitb��
	REMAINT_DT_VFY			//mtvfy��
};

//�����ṹ
struct LOGINUSER_INFO 
{
	int  nSockId;		//��½�û���socketid
	UINT nSocket;		//����socket
	int  nUID;			//��½�ʺ�UID
	int  nPort;			//Ŀ��˿�
	char szAddr[18+1];		//Ŀ��IP
	char szUserId[6+1];
	char szLoginId[6+1];	//��½Id
	char szPwd[16+1];		//��½����
	char szSpGate[21+1];	//ͨ����
	time_t tLastActive; //���һ�����ݰ����͵�ʱ��
	int nStatus;		//��ǰ״̬Ϊ���߻������߻���δ֪
	int nVer;			//Э��汾��0x20,0x30
	LOGINUSER_INFO()
	{
		memset(this, 0, sizeof(LOGINUSER_INFO));
		nStatus = DISCONNECTED;
		time(&tLastActive);
	}
	
	LOGINUSER_INFO& operator=(const LOGINUSER_INFO& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(LOGINUSER_INFO));
		memcpy(this, &other, sizeof(LOGINUSER_INFO));
		return *this;
	}
};

typedef struct  
{
	BYTE Ver;
	BYTE nMethods;
	BYTE Methods[2];
}REQ1_SOCK5,*LPREQ1_SOCK5;
typedef struct  
{
	BYTE Ver;	
	BYTE Method;
}ANS1_SOCK5,*LPANS1_SOCK5;

typedef struct  
{
	BYTE Ver;	
	BYTE Status;
}ANS_AUTHEN_SOCK5,*LPANS_AUTHEN_SOCK5;

typedef struct  
{
	BYTE Ver;
	BYTE Cmd;
	BYTE Rsv;
	BYTE Atyp;
	BYTE other;
}REQ2_SOCK5,*LPREQ2_SOCK5;
typedef struct  
{
	BYTE Ver;
	BYTE Rep;
	BYTE nMethods;
	BYTE Methods[2];
}ANS2_SOCK5,*LPANS2_SOCK5;


typedef struct  
{
	BYTE VN;
	BYTE CD;
	BYTE DstPort[2];
	BYTE DstIp;	
	BYTE nMethods;
	BYTE Methods[2];
}REQ1_SOCK4,*LPREQ1_SOCK4;
typedef struct  
{
	BYTE Ver;	
	BYTE Method;
}ANS1_SOCK4,*LPANS1_SOCK4;

//�����Ų�ֹ���
typedef struct  
{
	short gatetype;
	short singlelen;
	short multilen1;
	short multilen2;
}SPLIT_RULE,*LPSPLIT_RULE;

typedef struct ListData
{
	unsigned int nSockId;				//socketId
	unsigned int nSocket;				//socket
	unsigned int PkLen;                //������ 
	unsigned int CmdId;                //����ID
	unsigned int SeqId;                //���
	unsigned int SrcSeqId;			   //��Դ��seqid
	unsigned int Src;                  //��Ϣ��Դ��1-API��2-�ڲ���
	unsigned int Status;               //�����־(0��ʼ, 1������, 2���ͳɹ�, 3����ʧ��)
	unsigned int ReCnt;                //�ط�����
	unsigned int nUID;				   //�û�UID
	HANDLE hEvent;                     //�¼�
	unsigned int LastTime;             //����ʱ��
	unsigned char szSpGate[21+1];
	unsigned char Data[MAX_PACK_LEN+1];  //����������
	
	ListData()
	{
		memset(this, 0, sizeof(ListData));
	}
	
	ListData& operator=(const ListData& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(ListData));
		memcpy(this, &other, sizeof(ListData));
		return *this;
	}
}*LPDATA;

typedef struct TG_NODE                 //������Ͷ���
{
	ListData  data;                    //����������
	TG_NODE  *next;                    //����ָ����
} ListNode;
typedef ListNode *LPNOTE;

typedef struct TG_LIST                //������
{
	ListNode *head;                   //����ͷ�ڵ�ָ��
	ListNode *tail;                   //����β�ڵ�ָ��
	ListNode *rhead;                  //������ͷ�ڵ�ָ��
	ListNode *rtail;                  //������β�ڵ�ָ��
	unsigned int rlen;                //�������ĳ���
	unsigned int length;              //����������
	CRITICAL_SECTION cs;              //������
} LIST;
typedef LIST *LPLIST;

//////////////////////////////////////////////////////////////////////////
//WBS�����붨��
//#define ER_WBS_
//////////////////////////////////////////////////////////////////////////
/*
struct MEMPOOL_BUF 
{
	char szBuf[POOL_BUF_SIZE+1];
	
	MEMPOOL_BUF()
	{
		memset(this, 0, sizeof(MEMPOOL_BUF));
	}
	
	MEMPOOL_BUF& operator=(const MEMPOOL_BUF& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(MEMPOOL_BUF));
		memcpy(this, &other, sizeof(MEMPOOL_BUF));
		return *this;
	}
};
*/
//////////////////////////////////////////////////////////////////////////
//
//��IO���ݣ���չ��WSAOVERLAPPED
//zhubo 2017-4-7
//
/*
struct IOCPIO 
{
	WSAOVERLAPPED				ol;
	char						buf[BUFFER_SIZE_SEND];
	WSABUF						wsaBuf;
	SOCKET						socket;
	IOCP_OPERATION				operation;
	volatile int				state;
	int							nSockId;
	IOCPIO()
	{
		memset(this, 0, sizeof(IOCPIO));
	}
};
typedef IOCPIO IOCP_IO;
typedef IOCPIO* IOCP_IO_PTR;


typedef struct 
{
	WSAOVERLAPPED				ol;
	char						buf[BUFFER_SIZE_SEND];
	WSABUF						wsaBuf;
	SOCKET						socket;
	IOCP_OPERATION				operation;
	volatile int				state;
} IO_SD, *LP_IO_SD;



typedef struct 
{
	WSAOVERLAPPED				ol;
	char						buf[BUFFER_SIZE_RECV];
	WSABUF						wsaBuf;
	SOCKET						socket;
	IOCP_OPERATION				operation;
	volatile int				state;	
} IO_RD, *LP_IO_RD;

#define IOCP_RECVBUFPOOL_SIZE		1000		//����IOCP�����ڴ�صĴ�С

//��ɶ˿ڽ��ջ�����
struct IOCP_RECV_BUF
{
	UINT			m_nRemain;
    char			m_szTotalRecv[MAX_RECBUF_LEN+1];
	
    IOCP_RECV_BUF()
    {
        memset(this, 0, sizeof(IOCP_RECV_BUF));
    }
	
    IOCP_RECV_BUF& operator=(const IOCP_RECV_BUF& other)
    {
        if (this == &other)
        {
            return *this;
        }
        memset(this, 0, sizeof(IOCP_RECV_BUF));
        memcpy(this, &other, sizeof(IOCP_RECV_BUF));
        return *this;
    }
};
typedef IOCP_RECV_BUF* IOCP_RECV_BUF_PTR;

//
//�Զ���ṹ��������ɼ���(���������)
//
struct IOCPKEY 
{
	SOCKET			socket;
	int				socketid;
	short			m_nUsrType;
	int  			nUserPrivilege;     //�Ա���ˮ��Ȩ�� 0-�ޣ���0-��
	int				m_nUID;
	time_t			tLastActive;
	USHORT			m_nVer;
	UINT			m_nRemain;
	SOCKET_STATE	state;
	UINT			nPort;
	UCHAR			cLogin;				//�Ƿ��ѵ�½��־
	UCHAR			m_szSrcAddr[MAX_SPID_LEN+1];
	char			m_szSpGate[MAX_CHANNEL_LEN+1];
	char			m_szIP[MAX_IPADDR_LEN+1];
	char			m_szTotalRecv[MAX_RECBUF_LEN+1];
	int				nSeqId;			//��ˮ��
	IOCP_IO_PTR		pIo;		
	IOCPKEY()
	{
		memset(this, 0, sizeof(IOCPKEY));
		m_nUsrType = TYPE_UNKNOWN;
		cLogin     = 0;
		time(&tLastActive);
	}
};
typedef IOCPKEY IOCP_KEY;
typedef IOCPKEY* IOCP_KEY_PTR;

//�ص�����
typedef INT (CALLBACK* DEALRECVCALLBACK)(IOCP_IO_PTR pIo, IOCP_KEY_PTR pKey, int nRead, DWORD dwUserPtr, BOOL bDisconnect);
//////////////////////////////////////////////////////////////////////////
*/

struct PARAMS 
{
    int     m_nGateNO;
	int		m_nMaxDBBufNum;
	int		m_nEndSlideWnd;
	int		m_nPackUpdateNum;
	int		m_nPackWriteNum;
	int		m_nPreSlideWnd;
	int		m_nRspDelay;
	int		m_nSendBufNum;
	int		m_nReWriteNum;
	int		m_nSplitLen;
	int		m_nMaxConnect;
	int		m_nMaxIocpThread;
    int		m_nUnlinkTime;; //�������ӳ�ʱʱ��(s) UnlinkTime = 60
	int		m_nDBCMDDelay;
	int		m_nWaitRspDelay;
	int		m_nPreMaxWnd;
	int		m_nEndMaxWnd;
	int		m_nSyncFeeInterval;
    int		m_nSyncSpGateInterval;//;ͬ���˿ڰ󶨱�ʱ����(m)  ��ʱ�߳���Ҫ���õ�ʱ���� SyncSpGateInterval = 5
	int		m_nMaxCon;
	int		m_nUltraFlowDelay;
	int		m_nReSndCnt;
	int		m_nExpiredTm;
	int		m_nPackInitNum;
	int		m_nCheckFreq;
	int		m_nMonFreq;
	int		m_nBatchRead;
	int		m_nMemWarn;
	int		m_nMemException;
	int		m_nLogSaveDays;
	int		m_nLogSize;
    int		m_nRecycleRptInterval;//RPTѭ����� MaxRptMoRemained ;״̬��������д����ڴ��е�ʱ��(������ֵ����д�����ݿ�)
	int		m_nWbsThrCnt;
    int		m_nWbsSendBuf;//; ���ͻ������� MaxSendBuf = 5000
    int		m_nWbsSlideWnd;//; �������ͻ�������  ����һ���ͻ���ͬʱ���͵Ĳ������� SlideWnd = 5
	int     m_nWbsSendToPtSildeBuf;
    int		m_nWbsReSendCnt;//        ;MO�ط�����  MoReSendCnt =3
	int		m_nWbsSendSpeed;
	int		m_nWbsPackNum;
	int		m_nDllCallType;
	int		m_nMoReSndCnt;
	int		m_nDelaySend;
	UINT	m_nReSendTimeOut;//; �ظ�Ƶ�ʼ������(��)  ReSendTimeOut = 30
    UINT	m_nLogLevel;////��־��������ֵ ;��־��������ֵ��0-5 �����߳���Ĭ�ϼ��� LOG_ERROR=4 ��
	DATE	m_dtVerifyBegin;
	DATE	m_dtVerifyEnd;
	BOOL	m_bStopCharge;
	BOOL	m_bWbsDiscardRpt;
	BOOL	m_bJavaUtf8;
    BOOL	m_bDiscardRpt;      //; �Ƿ���RPT(0 / 1)DiscardRpt = 0
	BOOL	m_bStopMoSend;
	BOOL	m_bStopMtSend;

//	BOOL	m_bStopMt0Send;
	BOOL	m_bStopMoRptRead;
	BOOL	m_bStopMt1Send;
	BOOL	m_bStopMt2Send;
	BOOL	m_bStopMt3Send;
	BOOL	m_bStopMt4Send;
	BOOL	m_bStopMt5Send;
	BOOL	m_bStopMt6Send;
	BOOL	m_bStopMt7Send;
	BOOL	m_bStopMt8Send;
	BOOL	m_bStopMt9Send;
	BOOL	m_bStopRptSend;
	//qiweiadd20150826
	UINT	m_nPendSendNum;
	UINT	m_nPendCheckTime;

	BOOL	m_bStopPush;//;StopPush 1Ϊֹͣ���ͣ�0Ϊ��Ҫ���͡�
    char    m_szWbsLocalName[MAX_PATH];//�����ļ�·��
    int     m_nSlideWndSize;
    int     m_nQueEleCount;

	BOOL	m_bStopMt0Read;
	BOOL	m_bStopMt1Read;
	BOOL	m_bStopMt2Read;
	BOOL	m_bStopMt3Read;
	BOOL	m_bStopMt4Read;
	BOOL	m_bStopMt5Read;
	BOOL	m_bStopMt6Read;
	BOOL	m_bStopMt7Read;
	BOOL	m_bStopMt8Read;
	BOOL	m_bStopMt9Read;
	BOOL	m_bStopTimerSend0;
	BOOL	m_bStopTimerSend1;
	BOOL	m_bStopTimerSend2;
	BOOL	m_bStopTimerSend3;
	BOOL	m_bStopTimerSend4;
	BOOL	m_bStopTimerSend5;
	BOOL	m_bStopTimerSend6;
	BOOL	m_bStopTimerSend7;
	BOOL	m_bStopTimerSend8;
	BOOL	m_bStopTimerSend9;
	BOOL	m_bStopMoRead;
	BOOL	m_bStopRptRead;
	BOOL	m_bAdjustPreWnd;
	BOOL	m_bAdjustEndWnd;
	BOOL	m_bStopDbOperate;
	BOOL	m_bDbDispatch;
	BOOL	m_bSuspendRpt;
	BOOL	m_bEnableLog;
	BOOL	m_bErMtToDB;
	BOOL	m_bLogThrMode;
	BOOL	m_bDiscardNullMo;
	BOOL    m_bFilterKeywords;
	BOOL	m_bResendCtrl;
	BOOL	m_bSmartVerify;

	int     m_nSacnfWaitTime; //��ȡ������Ϣʱ���� //Ĭ��10s
	int     m_nDealMtThrCnt;
	int     m_nDealUserCnt;
	int     m_nDealUserRspCnt;
	int     m_nDealPTMtThrCnt;

	BOOL    m_bStopMtRead;
	BOOL    m_bNoKeepAlive;

	int     m_nVersion; //wbsЭ��汾

	int     m_nMasterOrSlave;
	//zhubo 2017-5-25  ��wbs����mt���� ������������ ��1��ʼ
	int		m_nMasterProcessIndex;

	int     m_nPwdFailedCnt;
	int     m_nPwdLockTime;
	int     m_SubTmLes0;
	int     m_SubTmIn5;
	int     m_SubTmIn10;
	int     m_SubTmIn15;
	int     m_SubTmIn30;
	int     m_SubTmIn60;
	BOOL	m_bSynAccBind;
	int		m_nRecvPackWarn;
	int		m_PerGetMaxDB;
	BOOL	m_bStopCycleWarn;
	BOOL	m_bStopOtherWarn;
	BOOL	m_bStopPushFailWarn;

	//zhubo 2017-06-19
	BOOL	m_nIsSendMtToPt;

	int     m_nSUrlTimeOut;// ���ַ����ͨ�ŵĳ�ʱʱ��
	BOOL	m_bConnectToSUrlCenter;//�Ƿ���Ҫ���϶̵�ַ����
	int		m_nSUrlMaxWinSize;//wbs��̵�ַ����ͨ�ŵ���󴰿ڴ�С
	int		m_nSUrlClientThreadsNum;
	int 	m_nSUrlClientCount;
	BOOL	m_bConnectToMInfoSvr;//�Ƿ���Ҫ���Ϻ����ѯ����
	int		m_nMInfoMaxWinSize;//wbs������ѯ����ͨ�ŵ���󴰿ڴ�С
	int     m_nMInfoTimeOut;// �۷�������ͨ�ŵĳ�ʱʱ��
	int 	m_nLoadAllPtFee;        //�Ƿ��������ƽ̨�ļƷ���Ϣ

	PARAMS()
	{
		memset(this, 0, sizeof(PARAMS));
	}

	PARAMS& operator=(const PARAMS& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(PARAMS));
		memcpy(this, &other, sizeof(PARAMS));
		return *this;
	}
};


struct SOCKET_INFO
{
	int				nSockId;			//Ϊÿ��socket���
	short			nSocketType;		//0-ǰ�� 1-��� 2-δ֪
	int  			nUserPrivilege;     //�Ա���ˮ��Ȩ�� 0-�ޣ���0-��
	int				nUID;				//��ҵ�����ݿ��е�Ωһ���
	int				nGateType;			//��������(���ں��spgate��˵)
	//zhubo 2017-4-7
	//IOCP_KEY_PTR	pKey;				//��ɼ�ָ��
	short			nStatus;			//��ǰ״̬(OK/ULTRA_FLOW)
	time_t			tDelay;				//��¼��socket��ʱ��ʱ��
	UINT			nSocket;			//socket
	USHORT			nVer;				//Э��汾��
	UINT			nPort;				//�˿ں�
	UCHAR			cLogin;				//�Ƿ��ѵ�½��־
	UCHAR			szSrcAddr[MAX_SPID_LEN+1];		//sp_id
	char			szSpGate[MAX_CHANNEL_LEN+1];
	char			szIP[MAX_IPADDR_LEN+1];
	char			cNeedSend;			//0:��Ҫ����  1:���账��
	char			cKeepMsgId;			//0:���豣���Դ���MsgId  1:�����Դ���MsgId
	SOCKET_INFO()
	{
		memset(this, 0, sizeof(SOCKET_INFO));
		nSocketType = TYPE_UNKNOWN;
		nStatus		= STATE_OK;
		cLogin		= 1;
	}
	
	SOCKET_INFO& operator=(const SOCKET_INFO& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(SOCKET_INFO));
		memcpy(this, &other, sizeof(SOCKET_INFO));
		return *this;
	}
};


struct SPGATE_INFO
{
	short	nGateType;			//�������� 0�ƶ� 1��ͨ 21����
	short	nExpTraits;			//��չ���� 0����չ 1������չ 2����
	short	nFeeFlag;			//���ʱ�־ 1Ԥ�� 2�� 0δ֪

	USHORT	usSignlen;			//ǩ������
	USHORT	usMaxwords;			//�����ų���
	USHORT	usSinglelen;		//�������ų���
	USHORT	usMultilen1;		//�����ŵ�һ���ĳ���
	USHORT	usMultilen2;		//�����ŵڶ����ĳ���
	char	szSignature[MAX_SIGN_LEN*2+1];	//ǩ��
	/************* qiwei add ********/

	USHORT	usEnSignlen;			//ǩ������
	USHORT	usEnMaxwords;			//�����ų���
	USHORT	usEnSinglelen;		//�������ų���
	USHORT	usEnMultilen1;		//�����ŵ�һ���ĳ���
	USHORT	usEnMultilen2;		//�����ŵڶ����ĳ���
	//�費��Ҫ����Ӣ��ǩ��
	char	szEnSignature[MAX_SIGN_LEN*2+1];	//ǩ��
	/**************** end qiwei add ****************/

	char	szSpGate[MAX_CHANNEL_LEN+1];     //���˿�
	char	szExNo[MAX_CHANNEL_LEN+1];		//��չ�˿�
	SPGATE_INFO()
	{
		memset(this, 0, sizeof(SPGATE_INFO));
	}
	
	SPGATE_INFO& operator=(const SPGATE_INFO& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(SPGATE_INFO));
		memcpy(this, &other, sizeof(SPGATE_INFO));
		return *this;
	}
};
/*
struct PACK_SLIDEWND
{
	short nSendCount;	//��ǰ���ڴ�С
	short nSockType;	//socket���� 0:ǰ�� 1:���
	short nMaxWnd;		//�������ֵ����̬����
	DWORD tLastSend;	//���һ�η��͵�ʱ��
	DWORD tZeroCnt;		//���ڼ�С��0��ʱ��
	short nDecCnt;		//tNow-tLastSendʱ����ڼ��ٵĴ�����
	PACK_SLIDEWND()
	{
		memset(this, 0, sizeof(PACK_SLIDEWND));
	}

	PACK_SLIDEWND& operator=(const PACK_SLIDEWND& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(PACK_SLIDEWND));
		memcpy(this, &other, sizeof(PACK_SLIDEWND));
		return *this;
	}
};
typedef std::map<int, PACK_SLIDEWND> MAP_SLIDEWND;
*/
/*
struct PACK_RECV
{
	UINT		nPkLen;                //������ 
	UINT		nCmdId;                //����ID
	UINT		nSeqId;                //���
	char		szTime[MAX_TIME_LEN+1];	//���յ�����ʱ��
	SOCKET_INFO	sockInfo;
	UCHAR		szData[MAX_PACK_LEN+1]; //����������

	PACK_RECV()
	{
		memset(this, 0, sizeof(PACK_RECV));
		sockInfo.nSocketType = TYPE_UNKNOWN;
	}

	PACK_RECV& operator=(const PACK_RECV& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(PACK_RECV));
		memcpy(this, &other, sizeof(PACK_RECV));
		return *this;
	}
};
*/
/*
struct PACK_SEND
{
	UINT		nLen;						//���Ͱ�����
	UINT		nCmdId;						//����ID
	UINT		nSeqId;
	SOCKET_INFO	sockInfo;
	UCHAR		szData[MAX_PACK_LEN+1];	//���Ͱ�����
	
	PACK_SEND()
	{
		memset(this, 0, sizeof(PACK_SEND));
		sockInfo.nSocketType = TYPE_UNKNOWN;
	}

	PACK_SEND& operator=(const PACK_SEND& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(PACK_SEND));
		memcpy(this, &other, sizeof(PACK_SEND));
		return *this;
	}
};
*/

struct PACK_RSP
{
	UINT		nLen;					//���Ͱ�����
	UINT		nCmdId;					//����ID
	UINT		nSeqId;
	SOCKET_INFO	sockInfo;
	UCHAR		szData[MAX_RSPPACK_LEN];				//���Ͱ�����
	
	PACK_RSP()
	{
		memset(this, 0, sizeof(PACK_RSP));
		sockInfo.nSocketType = TYPE_UNKNOWN;
	}

	PACK_RSP& operator=(const PACK_RSP& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(PACK_RSP));
		memcpy(this, &other, sizeof(PACK_RSP));
		return *this;
	}
};

/*
struct PACK_QUERY_FEE 
{
	SOCKET_INFO		sockInfo;						//socket��Ϣͷ
	UINT			nPkLen;							//������ 
	UINT			nCmdId;							//����ID
	UINT			nSeqId;							//���
	int				nRemainedNum;					//���
	USHORT			nResult;						//1�ֽ�//���
	UCHAR			szMsgId[MAX_MSGID_LEN+1];		//MSGID
	UCHAR			szSpId[MAX_SPID_LEN+1];			//SPID
	UCHAR			szReserve[MAX_RESERVE_LEN*8+1];	//64�ֽ�//����
	PACK_QUERY_FEE()
	{
		memset(this, 0, sizeof(PACK_QUERY_FEE));
		sockInfo.nSocketType = TYPE_UNKNOWN;
	}
	
	PACK_QUERY_FEE& operator=(const PACK_QUERY_FEE& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(PACK_QUERY_FEE));
		memcpy(this, &other, sizeof(PACK_QUERY_FEE));
		return *this;
	}
};

struct PACK_QUERY_CHANNELINFO 
{
	SOCKET_INFO		sockInfo;						//socket��Ϣͷ
	UINT			nPkLen;							//������ 
	UINT			nCmdId;							//����ID
	UINT			nSeqId;							//���
	USHORT			nQueryType;						//��ѯ������0:�ƶ� 1:��ͨ 2:���� 9��ȫ��
	USHORT			nResult;						//1�ֽ�//���
	UCHAR			szMsgId[MAX_MSGID_LEN+1];		//MSGID
	UCHAR			szSpId[MAX_SPID_LEN+1];			//SPID
	UCHAR			szChannelInfo[MAX_CHANNELINFO_LEN+1];//ͨ������Ϣ
	UCHAR			szReserve[MAX_RESERVE_LEN+1];	//64�ֽ�//����
	PACK_QUERY_CHANNELINFO()
	{
		memset(this, 0, sizeof(PACK_QUERY_CHANNELINFO));
		sockInfo.nSocketType = TYPE_UNKNOWN;
	}
	
	PACK_QUERY_CHANNELINFO& operator=(const PACK_QUERY_CHANNELINFO& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(PACK_QUERY_CHANNELINFO));
		memcpy(this, &other, sizeof(PACK_QUERY_CHANNELINFO));
		return *this;
	}
};

struct VERIFY_TASK
{
	INT64 nID;
	short nVerifyStatus;
	std::string strCmdText;
	std::string strParams;
};
*/
typedef struct MultixMsg
{
	bool			bIsEnglish;
	int				nMsgFmt;							//���뷽ʽ
	//zhubo 2017-06-21  ��ֲ����
	int             nMsgLen;                              //��Ϣ����
	INT64           nUsrMsgId;			                   //�û��Զ�����ˮ��
	char			szPhone[SIGN_MAX_PHONE_LEN+1];         //�����ֻ���
	char			szSubpot[MAX_CHANNEL_LEN+1];	       //�Ӷ˿�
	char			szMsg[2*MAX_MSG_LEN+1];          //��������
	char            szSvrtype[MAX_SVRTYPE_LEN + 1];
	char            szCustId[MAX_CUSTID_LEN + 1];

	MultixMsg()
	{
		memset(this, 0, sizeof(MultixMsg));
	}
	
	MultixMsg& operator=(const MultixMsg& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(MultixMsg));
		memcpy(this, &other, sizeof(MultixMsg));
		return *this;
	}
}MULTIXMSG;

typedef struct MultixMsgV5
{
	bool			bIsEnglish;
	int				nMsgFmt;							//���뷽ʽ
	int				nMsgLen;                             //��Ϣ����
	char			szMobile[SIGN_MAX_PHONE_LEN + 1];         //�����ֻ���
	char			szContent[2 * MAX_MSG_LEN + 1];          //��������
	char            szSvrtype[MAX_SERTYPE_LEN + 1];
	char			szExno[MAX_CHANNEL_LEN + 1];	       //�Ӷ˿�
	char            szCustId[MAX_CUSTID_LEN + 1];		//�û��Զ�����ˮ��
	char            szExData[MAX_EXDATA_LEN + 1];
	//zhubo 2018-03-26
	int				nChargeCount;						//�Ʒ�����
	MultixMsgV5()
	{
		memset(this, 0, sizeof(MultixMsgV5));
	}

	MultixMsgV5& operator=(const MultixMsgV5& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(MultixMsgV5));
		memcpy(this, &other, sizeof(MultixMsgV5));
		return *this;
	}
}MULTIXMSGV5;

typedef struct tagMixMsgV5
{
	bool			bIsEnglish;
	int				nMsgFmt;							//���뷽ʽ
	string			strContent;
	
	tagMixMsgV5()
	{
		bIsEnglish = 0;
		nMsgFmt    = 0;
		strContent = "";
	}
}MixMsgV5;


#define MAX_FMT_LEN 30
//��һ
struct PACK_SUBMIT_V1
{
	
	time_t			t;
	int				nUid;						//��ҵId=�󶨱���UIDEx
	int				nOrgUid;					//ԭʼUID=�󶨱���UID
	int				nDestUID;					//Ŀ��UID=���spgate��UID
	int				nEcId;						//ecid
	int				nTaskId;					//����Id
	short			nSendStatus;				/*	
												1:CMPPGW�յ�ǰ��SpGate��Submit����û�гɹ����������SpGate
												2:CMPPGW�յ�ǰ��SpGate��Submit�����ɹ����������SpGate
												3:���SpGate�ɹ���Submit��Ͷ�ݸ��ƶ���ҵ����
												4:�յ��ƶ����ص�״̬����
												5:��״̬����ɹ����͸�CMPPGW
												0:CMPPGW�յ�״̬���棬���ɹ����͸�ǰ��SpGate
												*/
	short			nFlag;						//�ڲ�״̬��־ 
												/*	
												1:�յ��û�submit�ȴ�д�⣬
												2:д��ɹ������ӷ��ͻ��壬���ȴ�submit_rspͶ�ݽ��
												3:submit_rspͶ�ݳɹ����ȴ�ת�������SpGate���
												4:submit_rspͶ��ʧ�ܵȴ�д���̴߳���
												5:Ͷ�ݸ����SpGateʧ��
												*/
	UCHAR			szSourceAddr[MAX_SPID_LEN+1];			//6�ֽ�//Դ��ַ���˴�ΪSP_Id����SP����ҵ���롣
	UCHAR			szAuthenSource[MAX_AUTHENSRC_LEN+1];		//16�ֽ�//���ڼ���Դ��ַ����ֵͨ������MD5 hash����ó�
	char			szTime[MAX_TIME_LEN+1];
	//���治�Ǳ�׼SUMBIT��Ϣ�����ݣ�Ϊ�˼���CONNECT ACTIVETEST TIMERNATE������Ϣ��������沿�ֶ���


	UCHAR			szMsgId[MAX_MSGID_LEN+1];				//8�ֽ�//��Ϣ��ʶ����SP��������ر��������������ա�
	short			nMsgLevel;					//1�ֽ�//��Ϣ����
	UCHAR			szServiceId[MAX_SRVID_LEN+1];			//10�ֽ�//ҵ�����ͣ������֡���ĸ�ͷ��ŵ���ϡ�
	USHORT			nMsgFmt;					//1�ֽ�//��Ϣ��ʽ  0��ASCII��  3������д������  4����������Ϣ  8��UCS2����15����GB����  ������������
	UCHAR			szSrcId[MAX_SRCID_LEN+1];				//21�ֽ�//Դ����SP�ķ�������ǰ׺Ϊ�������ĳ�����, ���ؽ��ú����������SMPPЭ��Submit_SM��Ϣ��Ӧ��source_addr�ֶΣ��ú����������û��ֻ�����ʾΪ����Ϣ�����к���
	USHORT			nDestUsrTl;					//1�ֽ�//������Ϣ���û�����(С��500���û�)
	char			szPhoneList[MAX_PHONE_LEN*MAX_PHONE_NUM+1];		//��������ֻ��б��á�,������
	USHORT			nMsgLength;					//1�ֽ�//��Ϣ����(Msg_FmtֵΪ0ʱ��<160���ֽڣ�����<=140���ֽ�)
	UCHAR			szMsgContent[2*MAX_SPLIT_MSG_LEN+1];		//��Ϣ����
	UCHAR			szReserve[MAX_RESERVE_LEN+1];				//8�ֽ�//����
	INT64           nUsrMsgId;                         //�û���������ˮ��
	char            szSvrtype[MAX_SERTYPE_LEN + 1];    //�û�ҵ������
	char            szCustId[MAX_CUSTID_LEN+1];     //�½ӿ��û���������ˮ��
	char            szExData[MAX_EXDATA_LEN + 1];
	char            szMsgSrcIp[MAX_IPADDR_LEN+8]; //��ϢԴIP:port
	char			szTmplId[MAX_TMPLID_LEN + 1]; //ģ��ID
	TINYINT			nMsgType;					 //msgtype 1:Ϊ����   11����
	TINYINT			nEncodeFlag;      //�����־ 1�����ģ�2��base64
	PACK_SUBMIT_V1()
	{
		memset(this, 0, sizeof(PACK_SUBMIT_V1));
		nEncodeFlag = PSEF_GBK;
		//sockInfo.nSocketType = TYPE_UNKNOWN;
	}

	PACK_SUBMIT_V1& operator=(const PACK_SUBMIT_V1& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(PACK_SUBMIT_V1));
		memcpy(this, &other, sizeof(PACK_SUBMIT_V1));
		return *this;
	}
};


//****************ע���޸�������ṹ�壬һ��Ҫ�޸��ڴ�س�ʼ���ĵط�********************
struct PACK_SUBMIT
{
	
	time_t			t;
	int				nUid;						//��ҵId=�󶨱���UIDEx
	int				nOrgUid;					//ԭʼUID=�󶨱���UID
	int				nDestUID;					//Ŀ��UID=���spgate��UID
	int				nEcId;						//ecid
	int				nTaskId;					//����Id
	short			nSendStatus;				/*	
												1:CMPPGW�յ�ǰ��SpGate��Submit����û�гɹ����������SpGate
												2:CMPPGW�յ�ǰ��SpGate��Submit�����ɹ����������SpGate
												3:���SpGate�ɹ���Submit��Ͷ�ݸ��ƶ���ҵ����
												4:�յ��ƶ����ص�״̬����
												5:��״̬����ɹ����͸�CMPPGW
												0:CMPPGW�յ�״̬���棬���ɹ����͸�ǰ��SpGate
												*/
	short			nFlag;						//�ڲ�״̬��־ 
												/*	
												1:�յ��û�submit�ȴ�д�⣬
												2:д��ɹ������ӷ��ͻ��壬���ȴ�submit_rspͶ�ݽ��
												3:submit_rspͶ�ݳɹ����ȴ�ת�������SpGate���
												4:submit_rspͶ��ʧ�ܵȴ�д���̴߳���
												5:Ͷ�ݸ����SpGateʧ��
												*/
	UCHAR			szSourceAddr[MAX_SPID_LEN+1];			//6�ֽ�//Դ��ַ���˴�ΪSP_Id����SP����ҵ���롣
	UCHAR			szAuthenSource[MAX_AUTHENSRC_LEN+1];		//16�ֽ�//���ڼ���Դ��ַ����ֵͨ������MD5 hash����ó�
	char			szTime[MAX_TIME_LEN+1];
	//���治�Ǳ�׼SUMBIT��Ϣ�����ݣ�Ϊ�˼���CONNECT ACTIVETEST TIMERNATE������Ϣ��������沿�ֶ���


	UCHAR			szMsgId[MAX_MSGID_LEN+1];				//8�ֽ�//��Ϣ��ʶ����SP��������ر��������������ա�
	short			nMsgLevel;					//1�ֽ�//��Ϣ����
	UCHAR			szServiceId[MAX_SRVID_LEN+1];			//10�ֽ�//ҵ�����ͣ������֡���ĸ�ͷ��ŵ���ϡ�
	USHORT			nMsgFmt;					//1�ֽ�//��Ϣ��ʽ  0��ASCII��  3������д������  4����������Ϣ  8��UCS2����15����GB����  ������������
	UCHAR			szSrcId[MAX_SRCID_LEN+1];				//21�ֽ�//Դ����SP�ķ�������ǰ׺Ϊ�������ĳ�����, ���ؽ��ú����������SMPPЭ��Submit_SM��Ϣ��Ӧ��source_addr�ֶΣ��ú����������û��ֻ�����ʾΪ����Ϣ�����к���
	USHORT			nDestUsrTl;					//1�ֽ�//������Ϣ���û�����(С��500���û�)
	char			szPhoneList[MAX_PHONE_LEN*MAX_PHONE_NUM+1];		//��������ֻ��б��á�,������
	USHORT			nMsgLength;					//1�ֽ�//��Ϣ����(Msg_FmtֵΪ0ʱ��<160���ֽڣ�����<=140���ֽ�)
	UCHAR			szMsgContent[2*MAX_SPLIT_MSG_LEN+1];		//��Ϣ����
	UCHAR			szReserve[MAX_RESERVE_LEN+1];				//8�ֽ�//����
	INT64           nUsrMsgId;                         //�û���������ˮ��
	char            szSvrtype[MAX_SERTYPE_LEN + 1];    //�û�ҵ������
	char            szCustId[MAX_CUSTID_LEN+1];     //�½ӿ��û���������ˮ��
	char            szExData[MAX_EXDATA_LEN + 1];
	char            szMsgSrcIp[MAX_IPADDR_LEN+8]; //��ϢԴIP:port
	char			szTmplId[MAX_TMPLID_LEN + 1]; //ģ��ID
	TINYINT			nMsgType;					 //msgtype 1:Ϊ����   11����
	TINYINT			nEncodeFlag;      //�����־ 1�����ģ�2��base64
	TINYINT			nValidTime;//��Чʱ��
	USHORT			nRmsLen;
	char 			szShowway[MAX_RMS_SHOWWAY_LEN+1];
	char			szShowtime[MAX_RMS_SHOWTIME_LEN+1];
	char 			szTitle[MAX_RMS_TITLE_LEN+1];
	char 			szRMSContent[MAX_RMS_TMPLCONTENT_V20_LEN+1];
	int				nCtrBit;       //����λ
	TINYINT 		nIsfree;
	TINYINT         nRmsver;
	
	PACK_SUBMIT()
	{
		memset(this, 0, sizeof(PACK_SUBMIT));
		nEncodeFlag = PSEF_GBK;
		//sockInfo.nSocketType = TYPE_UNKNOWN;
	}

	PACK_SUBMIT& operator=(const PACK_SUBMIT& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(PACK_SUBMIT));
		memcpy(this, &other, sizeof(PACK_SUBMIT));
		return *this;
	}
};
//****************ע���޸�������ṹ�壬һ��Ҫ�޸��ڴ�س�ʼ���ĵط�********************

typedef std::multimap<int, PACK_SUBMIT> MULTIMAP_MT;
typedef std::deque<PACK_SUBMIT> DEQUE_MT;
typedef std::vector<PACK_SUBMIT> VECTOR_MT;
typedef std::vector<PACK_SUBMIT_V1> VECTOR_MT_V1;


//��ISMG��SP�ͽ�״̬����ʱ����Ϣ�����ֶΣ�Msg_Content����ʽ��������
struct MSG_CONTENT
{
	UCHAR szMsgId[MAX_MSGID_LEN+1];				//8�ֽ�//��Ϣ��ʶSP�ύ���ţ�CMPP_SUBMIT������ʱ����SP������ISMG������Msg_Id��
	UCHAR szState[MAX_ERSTAT_LEN+1];				//7�ֽ�//���Ͷ��ŵ�Ӧ������������SMPPЭ��Ҫ����stat�ֶζ�����ͬ�������һ��SP���ݸ��ֶ�ȷ��CMPP_SUBMIT��Ϣ�Ĵ���״̬��
	UCHAR szSubmitTime[MAX_SUBMITTM_LEN+1];		//10�ֽ�//YYMMDDHHMM��YYΪ��ĺ���λ00-99��MM��01-12��DD��01-31��HH��00-23��MM��00-59��
	UCHAR szDoneTime[MAX_DONETM_LEN+1];			//10�ֽ�//YYMMDDHHMM
	UCHAR szDestTerminalId[MAX_DESTTMID30_LEN+1];	//21�ֽ�//Ŀ���ն�MSISDN����(SP����CMPP_SUBMIT��Ϣ��Ŀ���ն�)
	UCHAR szSMSCSequence[MAX_SMSCSEQ_LEN+1];		//4�ֽ�//ȡ��SMSC����״̬�������Ϣ���е���Ϣ��ʶ��
	
	MSG_CONTENT()
	{
		memset(this, 0, sizeof(MSG_CONTENT));
	}

	MSG_CONTENT& operator=(const MSG_CONTENT& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(MSG_CONTENT));
		memcpy(this, &other, sizeof(MSG_CONTENT));
		return *this;
	}
};

/*
State�ֶζ���
Message State	Final Message States	Description
DELIVERED		DELIVRD					Message is delivered to destination
EXPIRED			EXPIRED					Message	validity period hasexpired
DELETED			DELETED					Message	has been deleted.
UNDELIVERABLE	UNDELIV					Message is undeliverable
ACCEPTED		ACCEPTD					Message is in accepted state(i.e. has been manually read on behalf of the subscriber by customer service)
UNKNOWN			UNKNOWN					Message is in invalid state
REJECTED		REJECTD					Message is in a rejected state

  ע�⣺
  1������ACCEPTEDΪ�м�״̬���������Ӷ��������յ���Ӧ�����������κβ�����
  2��Stat�ֶγ���Ϊ7���ֽڣ���дʱӦ���һ��Final Message States�е���д��ʽ����
  ״̬ΪDELIVEREDʱ��дDELIVRD���������ơ�
  3��SP�ȴ�״̬����ȱʡʱ��Ϊ48Сʱ��
*/
//****************ע���޸�������ṹ�壬һ��Ҫ�޸��ڴ�س�ʼ���ĵط�********************

struct PACK_DELIVER
{
	SOCKET_INFO		sockInfo;
	time_t			t;
	PACK_RSP		packRsp;						
	int				nSendStatus;				/*
												1:���SpGate�յ�mo(�ֻ����ж���)���͸�CMPPGW��δ�ɹ�
												2:���SpGate�յ�mo(�ֻ����ж���)���͸�CMPPGW�ɹ�
												3:CMPPGW�յ����SpGate���͹������ֻ�mo������ת����ǰ��SpGate,��δ�ɹ�����Ϊ״̬1
												0:CMPPGW�յ��û��ɹ����յĻ�Ӧ
												*/				
	short			nFlag;						/*	
												//1:�յ���˵�fwd�����ȴ����¿⣬
												//2:���¿�ɹ������ӷ��ͻ��壬���ȴ�deliver��Ͷ�ݸ��û��Ľ��
												1:fwd_rsp��Ͷ�ݳɹ�
												2:fwd_rsp��Ͷ��ʧ�ܵȴ�д���̴߳���
												3:fwdת��deliver��Ͷ��ǰ�˳ɹ��ȴ�д���̴߳���
												4:deliver��Ͷ��ǰ��ʧ��
												5:�յ�ǰ�˵�deliver_rsp
												*/
	short			nResult;					//1�ֽ�//���
	short			nRePack;					//�Ƿ���Ҫ��������������ݿ��ж������ģ������� 0������ 1����

	int				nUID;						//��deliverҪ�����ĸ���½ϵͳ���û�
	int				nOrgUID;					//��deliver�Ǵ��ĸ�spgate������
	int				nUIDEx;						//��deliver�����������ĸ��û���
	int				nEcId;
	UINT			nPkLen;						//������ 
	UINT			nCmdId;						//����ID
	UINT			nSeqId;						//���
	short			nStatus;					//״̬
	char			szTime[MAX_TIME_LEN+1];
	short			nFeeFlag;					//���ʱ�־(1Ԥ��2��)
	short			usReSendNum;				//���ط����� ����ͳ�Ʊ��ṹ������ط�������1.0 v.2.0 v3.0
	UCHAR           cPRI;				        //����Ȩ   2012-06-06 sp add 
//���治�Ǳ�׼DELIVER��Ϣ������Ϊ�˼���REPORT RSP��Ϣ�����

	UCHAR			szUsrId[MAX_SPID_LEN+1];
	UCHAR			szMsgId[MAX_MSGID_LEN+1];				//8�ֽ�//��Ϣ��ʶ�����㷨���£�����64λ��8�ֽڣ�����������1��	ʱ�䣨��ʽΪMMDDHHMMSS��������ʱ���룩��bit64~bit39������bit64~bit61���·ݵĶ����Ʊ�ʾ��bit60~bit56���յĶ����Ʊ�ʾ��bit55~bit51��Сʱ�Ķ����Ʊ�ʾ��bit50~bit45���ֵĶ����Ʊ�ʾ��bit44~bit39����Ķ����Ʊ�ʾ����2��	�������ش��룺bit38~bit17���Ѷ������صĴ���ת��Ϊ������д�����ֶ��С���3��	���кţ�bit16~bit1��˳�����ӣ�����Ϊ1��ѭ��ʹ�á��������粻�����������㣬�Ҷ��롣
	UCHAR			szPtMsgId[MAX_MSGID_LEN+1];				//ƽ̨MsgId
	UCHAR			szDestId[MAX_DESTID_LEN+1];				//21�ֽ�//Ŀ�ĺ��� SP�ķ�����룬һ��4--6λ��������ǰ׺Ϊ�������ĳ����룻�ú������ֻ��û�����Ϣ�ı��к��롣
	UCHAR			szServiceId[MAX_SRVID_LEN+1];			//10�ֽ�//ҵ�����ͣ������֡���ĸ�ͷ��ŵ���ϡ�
	UCHAR           szSpNumEx[MAX_CHANNEL_LEN+1];           //��չ�Ӻ�
	USHORT			nTpPid;						//1�ֽ�//GSMЭ�����͡���ϸ������ο�GSM03.40�е�9.2.3.9
	USHORT			nTpUdhi;					//1�ֽ�//GSMЭ�����͡���ϸ������ο�GSM03.40�е�9.2.3.23����ʹ��1λ���Ҷ���
	USHORT			nMsgFmt;					//1�ֽ�//��Ϣ��ʽ  0��ASCII��  3������д������  4����������Ϣ  8��UCS2����15����GB����
	UCHAR			szSrcTerminalId[MAX_SRCTMID30_LEN+1];		//21�ֽ�//Դ�ն�MSISDN���루״̬����ʱ��ΪCMPP_SUBMIT��Ϣ��Ŀ���ն˺��룩
	USHORT			nSrcTerminalType;
	USHORT			nReportFlag;				//1�ֽ�//�Ƿ�Ϊ״̬����0����״̬����1��״̬����
	USHORT			nMsgLen;					//1�ֽ�//��Ϣ����
	UCHAR			szMsgContent[2*MAX_SPLIT_MSG_LEN+1];		//��Ϣ����
	UCHAR			szReserved[MAX_RESERVE_LEN+1];			//������
	UCHAR			szLinkID[MAX_LINKID_LEN+1];
	MSG_CONTENT		msgcontent; 
	INT64           nUsrMsgId;
//#ifndef SHRINKPACK
//	unsigned char	szData[MAX_PACK_LEN+1];
//#endif
	char			szCustId[MAX_CUSTID_LEN + 1];
	int  nRspFmt;						//��Ӧ��ʽ
	int nReSendCnt;                    //v3.0 uid�˺��Զ����������ʧ�ܴ��� ��HSBC_URLINFO::nPushFailCnt������
	int nPknum;                        //��ǰ����
	int nPkTotal;                      //������
	char			szSendTime[MAX_TIME_LEN + 1];
	char            szRetTime[MAX_TIME_LEN + 1];
	char            szExData[MAX_EXDATA_LEN + 1];
	char			szSignature[2*MAX_SIGN_LEN+1];	//ǩ��

	PACK_DELIVER()
	{
		memset(this, 0, sizeof(PACK_DELIVER));
		sockInfo.nSocketType = TYPE_UNKNOWN;
	}

	PACK_DELIVER& operator=(const PACK_DELIVER& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(PACK_DELIVER));
		memcpy(this, &other, sizeof(PACK_DELIVER));
		return *this;
	}
};
//****************ע���޸�������ṹ�壬һ��Ҫ�޸��ڴ�س�ʼ���ĵط�********************

typedef std::multimap<int, PACK_DELIVER> MULTIMAP_MO;
typedef std::deque<PACK_DELIVER> DEQUE_MO;

struct SPGATE_BIND_INFO
{
	int		nLoginUID;				//��ҵ�����ݿ��е�Ωһ���
	int		nUserUID;				//nUID�������������ʺŵ�UID
	int		nDestUID;			//Ŀ��UID=���spgate��UID
	int		nEcId;				//��ҵID
    //zhubo 2017-5.24
	int		nGateGroupId;       //ͨ����ID
	TINYINT nBindType;			//������ 0��ͨ���˺ţ�1��ͨ����
	TINYINT nGateLevel;         //������0����1��
    //zhubo 2017-5.24
	short	nGateType;			//�������� 0�ƶ� 1��ͨ 2����
	short	nFeeFlag;			//���ʱ�־ 1Ԥ�� 2�� 0δ֪
	short	nExpTraits;			//��չ���� 0����չ 1������չ 2����
	short	nRiseLevel;			//����0Ϊ��������n���� С��0���ͼ���n��������0ά��ԭ��   (�ָ�ֵ��Ϊ���ͼ����ò���������֮��)
	UCHAR	ucExChannel;		//ͨ���Ƿ�Ϊ���˿���չ  0:��ͨ��ֱ�Ӵ��ƶ����������ƶ�ǩ�� 1:���ƶ�����ͨ���Ļ�������˾�����ӵ���չ
	USHORT	usSignlen;			//ǩ������
	USHORT	usMaxwords;			//�����ų���
	USHORT	usSinglelen;		//�������ų���
	USHORT	usMultilen1;		//�����ŵ�һ���ĳ���
	USHORT	usMultilen2;		//�����ŵڶ����ĳ���
	/*******qiwei add*******************/

	USHORT	usEnSignlen;			//ǩ������
	USHORT	usEnMaxwords;			//�����ų���
	USHORT	usEnSinglelen;		//�������ų���
	USHORT	usEnMultilen1;		//�����ŵ�һ���ĳ���
	USHORT	usEnMultilen2;		//�����ŵڶ����ĳ���
	char	szEnSignature[2*MAX_SIGN_LEN+1];	//ǩ��

  /********** end qiwei add **********/
	DATE	dtStart;
	DATE	dtEnd;
	char    cRouteFlag;         //·�ɱ�־ 0:MT/MO/RPT���� 1:MT·�� 2:MO·�� 3:RPT·��
	char	szLoginId[MAX_SPID_LEN+1];		//��½�ʺ�
	char	szUsrId[MAX_SPID_LEN+1];		//�����ʺ�
	char	szSpGate[MAX_CHANNEL_LEN+1];		//���˿�
	char	szExNo[MAX_CHANNEL_LEN+1];		//��չ�˿�
	char	szSpNumber[MAX_CHANNEL_LEN+1];	//���˿�+��չ�˿�
	char	szSignature[2*MAX_SIGN_LEN+1];	//ǩ��
	SPGATE_BIND_INFO()
	{
		memset(this, 0, sizeof(SPGATE_BIND_INFO));
		nExpTraits = -1;
	}

	SPGATE_BIND_INFO& operator=(const SPGATE_BIND_INFO& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(SPGATE_BIND_INFO));
		memcpy(this, &other, sizeof(SPGATE_BIND_INFO));
		return *this;
	}

	bool operator==(const SPGATE_BIND_INFO& other)
	{
		if (this == &other)
		{
			return true;
		}
		bool bRet = false;
		if (other.nLoginUID == this->nLoginUID && other.nUserUID == this->nUserUID)
		{
			if (other.nGateType == this->nGateType)
			{
				//bRet = true;
				//���²�������ͬһ�ʺŰ󶨶����ͬ����ͨ����ֱ�ӷ���true�������������
				//�磺106579999001,10659999002�ǲ������
				if (0 == memcmp(other.szSpGate, this->szSpGate, sizeof(this->szSpGate)))
				{
					if (0 == memcmp(other.szExNo, this->szExNo, sizeof(this->szExNo)))
					{
						if (other.nExpTraits == this->nExpTraits)
						{
							bRet = true;
						}	
					}
				}
			}
		}
		return bRet;
	}
};
typedef std::vector<SPGATE_BIND_INFO> BIND_INFO_VECTOR;
typedef std::list<SPGATE_BIND_INFO> BIND_INFO_LIST;
typedef std::multimap<int/*uid*/, SPGATE_BIND_INFO/*ͨ����Ϣ*/> BIND_MT_INFO_MAP;
typedef std::multimap<int/*uid*/, SPGATE_BIND_INFO/*ͨ����Ϣ*/> BIND_MO_INFO_MAP;

/************************************************************************/
/*                ˫��������(Rpt2PreBindMap)                            */
/************************************************************************/
//�Զ����������
/*
struct DEF_KEY 
{
	int nUID;		//�󶨱��е�nUIDEx��ͬ
	int nDestUID;	//�󶨱��е�nDestUID��ͬ

	DEF_KEY(int x, int y)
	{
		nUID		= x;
		nDestUID	= y;
	}

	DEF_KEY()
	{
		memset(this, 0, sizeof(DEF_KEY));
	}
	
	DEF_KEY& operator=(const DEF_KEY& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(DEF_KEY));
		memcpy(this, &other, sizeof(DEF_KEY));
		return *this;
	}

	friend bool operator < (const DEF_KEY& x ,const DEF_KEY& y)
	{
		return x.nUID < y.nUID || (x.nUID == y.nUID && x.nDestUID < y.nDestUID);
	}
};

typedef std::map<DEF_KEY, SPGATE_BIND_INFO> TWO_KEY_MAP;
*/
/************************************************************************/
/*˫map,˫��������(Mo2PreBindMap)                                       */
/************************************************************************/

struct EXNO_KEY
{
	char szExNo[MAX_CHANNEL_LEN+1];
	short nExpTraits;
	
	EXNO_KEY(const char *szBuf, int nType)
	{
		memset(this, 0, sizeof(EXNO_KEY));
		nExpTraits = nType;
		memcpy(szExNo, szBuf, strlen(szBuf));
	}

	EXNO_KEY()
	{
		memset(this, 0, sizeof(EXNO_KEY));
		nExpTraits = -1;
	}
	
	EXNO_KEY& operator=(const EXNO_KEY& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(EXNO_KEY));
		memcpy(this, &other, sizeof(EXNO_KEY));
		return *this;
	}
	
	friend bool operator < (const EXNO_KEY& x ,const EXNO_KEY& y)
	{
		int nLenX = strlen(x.szExNo);
		int nLenY = strlen(y.szExNo);
		//�Ȱ�����
		if (nLenX < nLenY)
		{
			//������������
			//return true;
			//���Ƚ�������
			return false;
		}
		else
		{
			//�������
			if (nLenX == nLenY)
			{
				int nRet = memcmp(x.szExNo, y.szExNo, nLenX);
				//x.szExNo=y.szExNo
				if (0 == nRet)
				{
					return x.nExpTraits < y.nExpTraits;
				}
				//x.szExNo<y.szExNo
				else if (0 > nRet)
				{
					return true;
				}
				//x.szExNo>y.szExNo
				else
				{
					return false;
				}
			}
			//nLenX>nLenY
			else
			{
				//������������
				return true;
				//���Ƚ�������
				//return false;
			}
		}
	}
};
typedef std::map<EXNO_KEY, SPGATE_BIND_INFO> EXNO_KEY_MAP;
typedef std::map<int, EXNO_KEY_MAP> TWO_KEY_TWO_MAP;


struct SPNO_KEY
{
	char szSpNo[MAX_CHANNEL_LEN+1];
	short nExpTraits;
	
	SPNO_KEY(const char *szBuf, int nType)
	{
		memset(this, 0, sizeof(SPNO_KEY));
		nExpTraits = nType;
		memcpy(szSpNo, szBuf, strlen(szBuf));
	}
	
	SPNO_KEY()
	{
		memset(this, 0, sizeof(SPNO_KEY));
		nExpTraits = -1;
	}
	
	SPNO_KEY& operator=(const SPNO_KEY& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(SPNO_KEY));
		memcpy(this, &other, sizeof(SPNO_KEY));
		return *this;
	}
	
	friend bool operator < (const SPNO_KEY& x ,const SPNO_KEY& y)
	{
		int nLenX = strlen(x.szSpNo);
		int nLenY = strlen(y.szSpNo);
		//�Ȱ�����
		if (nLenX < nLenY)
		{
			//������������
			//return true;
			//���Ƚ�������
			return false;
		}
		else
		{
			//�������
			if (nLenX == nLenY)
			{
				int nRet = memcmp(x.szSpNo, y.szSpNo, nLenX);
				//x.szSpNo=y.szSpNo
				if (0 == nRet)
				{
					return x.nExpTraits < y.nExpTraits;
				}
				//x.szSpNo<y.szSpNo
				else if (0 > nRet)
				{
					return true;
				}
				//x.szSpNo>y.szSpNo
				else
				{
					return false;
				}
			}
			//nLenX>nLenY
			else
			{
				//������������
				return true;
				//���Ƚ�������
				//return false;
			}
		}
	}
};
typedef std::map<SPNO_KEY, SPGATE_BIND_INFO> SPNO_KEY_MAP;
typedef std::map<int, SPNO_KEY_MAP> TWO_KEY_TWO_MAP_EX;

/************************************************************************/
/*˫map,����������(Mt2PreBindMap)                                       */
/************************************************************************/

struct  THREE_KEY
{
	short nGateType;			//0:�ƶ� 1:��ͨ 21:����C��
	char szSpNo[MAX_CHANNEL_LEN+1];			//spnumber
	short nExpTraits;			//��չ���� 0:����չ 1:������չ
	
	THREE_KEY(int nType, const char *szBuf, int nExpTrait)
	{
		memset(this, 0, sizeof(THREE_KEY));
		memcpy(szSpNo, szBuf, strlen(szBuf));
		nGateType	= nType;
		nExpTraits	= nType;
	}
	
	THREE_KEY()
	{
		memset(this, 0, sizeof(THREE_KEY));
		nExpTraits = -1;
	}
	
	THREE_KEY& operator=(const THREE_KEY& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(THREE_KEY));
		memcpy(this, &other, sizeof(THREE_KEY));
		return *this;
	}
	
	friend bool operator < (const THREE_KEY& x ,const THREE_KEY& y)
	{
		//�������ͱȽ�
		if (x.nGateType == y.nGateType)
		{
			int nLenX = strlen(x.szSpNo);
			int nLenY = strlen(y.szSpNo);
			//�Ȱ�����
			if (nLenX < nLenY)
			{
				//������������
				//return true;
				//���Ƚ�������
				return false;
			}
			else
			{
				//�������
				if (nLenX == nLenY)
				{
					int nRet = memcmp(x.szSpNo, y.szSpNo, nLenX);
					//x.szExNo=y.szExNo
					if (0 == nRet)
					{
						return x.nExpTraits < y.nExpTraits;
					}
					//x.szExNo<y.szExNo
					else if (0 > nRet)
					{
						return true;
					}
					//x.szExNo>y.szExNo
					else
					{
						return false;
					}
				}
				//nLenX>nLenY
				else
				{
					//������������
					return true;
					//���Ƚ�������
					//return false;
				}
			}
		}
		else
		{
			return false;
		}
	}
};
typedef std::map<THREE_KEY, SPGATE_BIND_INFO> THREE_KEY_MAP;
typedef std::map<int, THREE_KEY_MAP> THREE_KEY_TWO_MAP;

struct PB_FEE_INFO
{
	INT64 nPrice_PB;
	unsigned int nNum_PB;
	INT64 nMoney_PB;

	PB_FEE_INFO()
	{
		memset(this, 0, sizeof(PB_FEE_INFO));
	}
	
	PB_FEE_INFO& operator=(const PB_FEE_INFO& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(PB_FEE_INFO));
		memcpy(this, &other, sizeof(PB_FEE_INFO));
		return *this;
	}
};

struct FEE_INFO
{
	int     nEcid;
	int		nUID;				//��ҵ�����ݿ��е�Ωһ���
	int		nLastDbRemained;	//�ϴ������ݿ�ͬ��ʱ���ݿ��еĿ�������
	int		nRemained;			//��������
	int		nPrePayUsed;		//Ԥ������������
	int		nPostPayUsed;		//�󸶷���������
	short	nFeeFlag;			//���ʱ�־
	UCHAR	ucInDebt;			//�Ƿ�Ƿ�� 0:��Ƿ 1:Ƿ��
	char	szUsrId[MAX_SPID_LEN+1];		//�Ʒ��ʺ�
	time_t  tLastSync;			//���һ��ͬ�����õ�ʱ��
	time_t  tLastInDebt;		//���һ��Ƿ����ʱ��
	unsigned int nNum_Inner;    //��������
	unsigned int nNum_Global;   //��������
	INT64	nPrice_Inner;		//���ڶ��ŵ���
	INT64	nPrice_Global;      //���ʶ��ŵ���
	INT64   nMoney;             //�û����
	TINYINT	nChargeType;        //�۷ѷ�ʽ 0��������1�����
	TINYINT nChargeObj;         //�ƷѶ��� 0�����˺ţ�1������ҵ
	TINYINT nFailNums;          //ͬ������ʧ�ܴ���
	PB_FEE_INFO pbInfo;

	FEE_INFO()
	{
		memset(this, 0, sizeof(FEE_INFO));
		time(&tLastSync);
	}
	
	FEE_INFO& operator=(const FEE_INFO& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(FEE_INFO));
		memcpy(this, &other, sizeof(FEE_INFO));
		return *this;
	}
	
	bool operator==(const FEE_INFO& other)
	{
		if (this == &other)
		{
			return true;
		}
		if (0 == memcmp(this, &other, sizeof(FEE_INFO)))
		{
			return true;
		}
		return false;
	}
};
typedef std::map<int,FEE_INFO> FEE_INFO_MAP;

struct STATISTICS  
{
	int			m_nConnState;
	int			m_nMtReq;
	int			m_nRptReq;
	int			m_nMoReq;
	int			m_nMtRsp;
	int			m_nRptRsp;
	int			m_nMoRsp;
	int			m_nQueryReq;
	int			m_nQueryRsp;
	int			m_nFileReq;
	int			m_nFileRsp;
	int			m_nMtSendNum;
	int			m_nMoRecvNum;
	int			m_nRptRecvNum;
	
	STATISTICS()
	{
		memset(this, 0, sizeof(STATISTICS));
	}
	
	STATISTICS& operator=(const STATISTICS& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(STATISTICS));
		memcpy(this, &other, sizeof(STATISTICS));
		return *this;
	}
};

/*
struct SQL_PARAMS
{
	int nUID;			//��ҵUID
	int nDestUID;				
	int nSendLevel;		//���ͼ���
	int nNum;			//ɸѡ����
	int nRemained;		//ʣ����
	char szSpGate[MAX_CHANNEL_LEN+1];//spgate

	SQL_PARAMS()
	{
		memset(this, 0, sizeof(SQL_PARAMS));
	}
	
	SQL_PARAMS& operator=(const SQL_PARAMS& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(SQL_PARAMS));
		memcpy(this, &other, sizeof(SQL_PARAMS));
		return *this;
	}
};
typedef std::vector<SQL_PARAMS> VEC_SQL_PARAMS;
typedef std::vector<SQL_PARAMS>::iterator IT_SQL_PARAMS;
*/
//ƽ̨ϵͳ��Ϣ
/*
struct SYS_INFO
{
	int	nCpu;			//cpuռ����
	int	nMem;			//����ռ����(M)
	int	nVmem;			//�����ڴ�ռ����(M)

	SYS_INFO()
	{
		memset(this, 0, sizeof(SYS_INFO));
	}
	
	SYS_INFO& operator=(const SYS_INFO& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(SYS_INFO));
		memcpy(this, &other, sizeof(SYS_INFO));
		return *this;
	}
};

//�ͻ�����Ϣ
struct CLIENT_INFO
{
	short	nOnlineStatus;		//����״̬ 0:���� 1:���� 2:δ֪
	int		nTotalMtSend;		//�û��ύ��Mt����
	int		nHaveSendMt;		//�ѱ��ɹ�ת����Mt����
	int		nRecvRpt;			//�յ�rpt
	int		nRecvMo;			//�յ���mo
	int		nRemainedMt;		//�û�������Mt
	int		nTimerSendMt;		//�û���ʱ���͵�Mt
	int		nRecvSpeed;			//��ʱ�ٶ�
	char	szUsrId[MAX_SPID_LEN+1];		//�����õĻ���ID������910000

	CLIENT_INFO()
	{
		memset(this, 0, sizeof(CLIENT_INFO));
	}
	
	CLIENT_INFO& operator=(const CLIENT_INFO& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(CLIENT_INFO));
		memcpy(this, &other, sizeof(CLIENT_INFO));
		return *this;
	}
};
*/
//ͨ����Ϣ
/*
struct CHANNEL_INFO
{
	short	nOnlineStatus;			//����״̬ 0:���� 1:���� 2:δ֪
	int		nTotalRptSend;			//��ͨ��������״̬��������
	int		nTotalMoSend;			//��ͨ����������������
	int		nHaveSendMo;			//�ѳɹ�ת�����û���MO
	int		nHaveSendRpt;			//�ѳɹ�ת�����û���Rpt
	int		nRecvMt;				//��ͨ���յ�����ת����Mt
	int		nRemainedMt;			//��ͨ��������Mt
	int		nTimerSendMt;			//��ͨ����ʱ���͵�Mt
	int		nRemainedRpt;			//��ͨ���ж�������RPT
	int		nRemainedMo;			//��ͨ���ж�������MO
	int		nRecvSpeed;				//��ʱ�ٶ�
	char	szUsrId[MAX_SPID_LEN+1];			//�����õĻ���ID������910000
	char	szSpGate[MAX_CHANNEL_LEN+1];			//ͨ����
	int		nUID;					//ͨ��UID(���ֶ�����������ƽ̨���ϵͳ��Ӱ��)

	CHANNEL_INFO()
	{
		memset(this, 0, sizeof(CHANNEL_INFO));
	}
	
	CHANNEL_INFO& operator=(const CHANNEL_INFO& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(CHANNEL_INFO));
		memcpy(this, &other, sizeof(CHANNEL_INFO));
		return *this;
	}
};
*/

struct CHANNEL_QUOTA 
{
	int nChannelNo;			//ͬ����ͨ�����
	int nQuota;				//ͨ�������ѷ�����
	int nRemained;			//��ǰ��������
	char szSpNo[MAX_CHANNEL_LEN+1];
	CHANNEL_QUOTA()
	{
		memset(this, 0, sizeof(CHANNEL_QUOTA));
	}
	
	CHANNEL_QUOTA& operator=(const CHANNEL_QUOTA& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(CHANNEL_QUOTA));
		memcpy(this, &other, sizeof(CHANNEL_QUOTA));
		return *this;
	}
};


struct CHANNEL_QUOTA_ARR
{
	int nGateType;						//��������
	int nTotalSend;						//�����ͷ�������
	int nTotalRemained;					//��������������
	int nChannelNum;					//channel_quota�����ж��ٸ�ͨ��
	CHANNEL_QUOTA channel_quota[CHANNEL_SEQ_ARR];	//ͬ����ͨ������
	CHANNEL_QUOTA_ARR()
	{
		memset(this, 0, sizeof(CHANNEL_QUOTA_ARR));
	}
	
	CHANNEL_QUOTA_ARR& operator=(const CHANNEL_QUOTA_ARR& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(CHANNEL_QUOTA_ARR));
		memcpy(this, &other, sizeof(CHANNEL_QUOTA_ARR));
		return *this;
	}
};


struct USER_INFO
{
	int		nUID;				//��ҵ�����ݿ��е�Ωһ���
	short	nStatus;			//�ʺ�״̬ 0:���� 1����Ϣ�ṹ�� 2���Ƿ�Դ��ַ 3����֤�� 4���汾̫��5~ ����������
	short	nUsrType;			//�����û������ͣ�0-ǰ�� 1-��� 2-δ֪
	short	nOnlineStatus;		//����״̬
	int  	nUserPrivilege;     //�Ա���ˮ��Ȩ�� 0-�ޣ���0-��
	
	int		nSendLevel0;		//ǰ�˷��͵�ƽ̨�ĵ�������
	int		nSendLevel1;		//ǰ�˷��͵�ƽ̨��Ⱥ��(�����ֻ�)
	int		nSendLevel2;		//ǰ�˷��͵�ƽ̨�Ĵ�����Ⱥ��
	int		nSendLevel3;
	int		nSendLevel4;
	int		nSendLevel5;
	int		nSendLevel6;
	int		nSendLevel7;
	int		nSendLevel8;
	int		nSendLevel9;
	int		nHaveSendLevel0;	//�ѳɹ�ת������˵ĵ���
	int		nHaveSendLevel1;	//�ѳɹ�ת������˵�Ⱥ��(�����ֻ�)
	int		nHaveSendLevel2;	//�ѳɹ�ת������˵Ĵ�����Ⱥ��
	int		nHaveSendLevel3;	//�ѳɹ�ת������˵ĵ���
	int		nHaveSendLevel4;	//�ѳɹ�ת������˵�Ⱥ��(�����ֻ�)
	int		nHaveSendLevel5;	//�ѳɹ�ת������˵Ĵ�����Ⱥ��
	int		nHaveSendLevel6;	//�ѳɹ�ת������˵Ĵ�����Ⱥ��
	int		nHaveSendLevel7;	//�ѳɹ�ת������˵ĵ���
	int		nHaveSendLevel8;	//�ѳɹ�ת������˵�Ⱥ��(�����ֻ�)
	int		nHaveSendLevel9;	//�ѳɹ�ת������˵Ĵ�����Ⱥ��
	int		nRecvRpt;			//�յ�rpt
	int		nRecvMo;			//�յ���mo
	
	int		nRemainedLevel0;	//���û��ж���mt0û�з���(����ǰ����˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ������mt0):��ʱ�ɺ��ʹ��
	int		nRemainedLevel1;	//���û��ж���mt1û�з���(����ǰ����˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ������mt1):��ʱ�ɺ��ʹ��
	int		nRemainedLevel2;	//���û��ж���mt2û�з���(����ǰ����˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ������mt2):��ʱ�ɺ��ʹ��
	int		nRemainedLevel3;	//���û��ж���mt0û�з���(����ǰ����˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ������mt0):��ʱ�ɺ��ʹ��
	int		nRemainedLevel4;	//���û��ж���mt1û�з���(����ǰ����˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ������mt1):��ʱ�ɺ��ʹ��
	int		nRemainedLevel5;	//���û��ж���mt2û�з���(����ǰ����˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ������mt2):��ʱ�ɺ��ʹ��
	int		nRemainedLevel6;	//���û��ж���mt0û�з���(����ǰ����˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ������mt0):��ʱ�ɺ��ʹ��
	int		nRemainedLevel7;	//���û��ж���mt1û�з���(����ǰ����˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ������mt1):��ʱ�ɺ��ʹ��
	int		nRemainedLevel8;	//���û��ж���mt2û�з���(����ǰ����˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ������mt2):��ʱ�ɺ��ʹ��
	int		nRemainedLevel9;	//���û��ж���mt2û�з���(����ǰ����˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ������mt2):��ʱ�ɺ��ʹ��

	int		nTimerSendLevel0;	//���û��ж��ٶ�ʱ���͵�mt0(����ǰ���û���˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ��ʱ���͵�����)
	int		nTimerSendLevel1;	//���û��ж��ٶ�ʱ���͵�mt1(����ǰ���û���˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ��ʱ���͵�����)
	int		nTimerSendLevel2;	//���û��ж��ٶ�ʱ���͵�mt2(����ǰ���û���˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ��ʱ���͵�����)
	int		nTimerSendLevel3;	//���û��ж��ٶ�ʱ���͵�mt0(����ǰ���û���˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ��ʱ���͵�����)
	int		nTimerSendLevel4;	//���û��ж��ٶ�ʱ���͵�mt1(����ǰ���û���˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ��ʱ���͵�����)
	int		nTimerSendLevel5;	//���û��ж��ٶ�ʱ���͵�mt2(����ǰ���û���˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ��ʱ���͵�����)
	int		nTimerSendLevel6;	//���û��ж��ٶ�ʱ���͵�mt0(����ǰ���û���˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ��ʱ���͵�����)
	int		nTimerSendLevel7;	//���û��ж��ٶ�ʱ���͵�mt1(����ǰ���û���˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ��ʱ���͵�����)
	int		nTimerSendLevel8;	//���û��ж��ٶ�ʱ���͵�mt2(����ǰ���û���˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ��ʱ���͵�����)
	int		nTimerSendLevel9;	//���û��ж��ٶ�ʱ���͵�mt0(����ǰ���û���˵�Ǹ�ͨ�����ܺͣ��Ժ����˵ָ��ʱ���͵�����)

	int		nRemainedRpt;		//ǰ���û��ж�������RPT
	int		nRemainedMo;		//ǰ���û��ж�������MO

	int		nTotalRptSend;		//����ܷ���ƽ̨��RPT����
	int		nTotalMoSend;		//����ܷ���ƽ̨��MO����
	int		nHaveSendMo;		//�ѳɹ�ת����ǰ�˵�MO
	int		nHaveSendRpt;		//�ѳɹ�ת����ǰ�˵�Rpt
	int		nRecvLevel0;		//����յ�ƽ̨ת���ĵ�������
	int		nRecvLevel1;		//����յ�ƽ̨ת����Ⱥ����(�����ֻ�)
	int		nRecvLevel2;		//����յ�ƽ̨ת���Ĵ�����Ⱥ��
	int		nRecvLevel3;		//����յ�ƽ̨ת���ĵ�������
	int		nRecvLevel4;		//����յ�ƽ̨ת����Ⱥ����(�����ֻ�)
	int		nRecvLevel5;		//����յ�ƽ̨ת���Ĵ�����Ⱥ��
	int		nRecvLevel6;		//����յ�ƽ̨ת���ĵ�������
	int		nRecvLevel7;		//����յ�ƽ̨ת����Ⱥ����(�����ֻ�)
	int		nRecvLevel8;		//����յ�ƽ̨ת���Ĵ�����Ⱥ��
	int		nRecvLevel9;		//����յ�ƽ̨ת���ĵ�������

	int		nRecvSpeed;			//��ʱ�����ٶ�
// 	int		nConNum;			//���û�������
	int		nFailureRate;		//���û���ʧ����(0���󲻿��� ����0��ʾ���ƣ�nFailureRate/PRECISION����ʾ����)
	int		nFailureNum;		//���û���ʧ����(�����ֲ���գ�ѭ��ʹ��)
	int		nTotalMtSubmit;		//���û��ܹ��ݽ��˶���MT(�����ֲ���գ�ѭ��ʹ��)
	UINT	nPort;
	time_t	tLogin;							//����ʱ��
	time_t	tLoginout;						//����ʱ��
	time_t  tLastMoTimeOut;					//���һ�ζ�ȡMo��ʱʱ��
	time_t	tLastRptTimeOut;				//���һ�ζ�ȡRpt��ʱʱ��
	int		nMoTimeOutCnt;					//MO��ʱ������ÿ��ʱһ�ξ���1��ÿ��������һ�κ��0
	int		nRptTimeOutCnt;					//RPT��ʱ������ÿ��ʱһ�ξ���1��ÿ��������һ�κ��0
	char    cMoTimeOut;						//�Ƿ��ȡMO��ʱ
	char	cRptTimeOut;					//�Ƿ��ȡRPT��ʱ
	char	szUsrId[MAX_SPID_LEN+1];		//�����õĻ���ID������910000
	char	szPwd[MAX_PWD_LEN+1];			//���롣����910000
	char	szSpGate[MAX_CHANNEL_LEN+1];	//����û���ͨ����
	char	szIp[MAX_IPADDR_LEN+1];
	char	szCorpName[MAX_COPRNAME_LEN+1];	//��ҵ����
	CHANNEL_QUOTA_ARR channel_arr[CHANNEL_TYPE_ARR];
	USER_INFO()
	{
		memset(this, 0, sizeof(USER_INFO));
		nUsrType = TYPE_UNKNOWN;
		nOnlineStatus = OFFLINE;
		time(&tLogin);
		time(&tLoginout);
		time(&tLastMoTimeOut);
		time(&tLastRptTimeOut);
	}
	
	USER_INFO& operator=(const USER_INFO& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(USER_INFO));
		memcpy(this, &other, sizeof(USER_INFO));
		return *this;
	}
};
typedef USER_INFO* USER_INFO_PTR;
typedef std::map<int, USER_INFO> MAP_USERINFO;
/*
struct REMAINED_INFO 
{
	int nUID;
	short nLevel;
	int nRemainedNum;
	REMAINED_INFO()
	{
		memset(this, 0, sizeof(REMAINED_INFO));
	}
	
	REMAINED_INFO& operator=(const REMAINED_INFO& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(REMAINED_INFO));
		memcpy(this, &other, sizeof(REMAINED_INFO));
		return *this;
	}
};
typedef std::vector<REMAINED_INFO> VECTOR_REMAINED_INFO;
*/
/*
struct RESEND_MSG
{
	int nReSendCnt; //�ط�����
	time_t tSndTm;  //����ʱ��

	RESEND_MSG()
	{
		memset(this, 0, sizeof(RESEND_MSG));
	}
	
	RESEND_MSG& operator=(const RESEND_MSG& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(RESEND_MSG));
		memcpy(this, &other, sizeof(RESEND_MSG));
		return *this;
	}
};
typedef std::map<INT64, RESEND_MSG> MAP_RESEND;
*/

enum PACK_FMT//�������ʽ
{
	UNKNOWN_FMT = 0,
	XML,
	JSON,
	URLENCODED = 4,
	USERDEFINE = 5  //���ļ���
};

struct WBS_REQ_MT  //�����·�MT
{
	int		nUID;							//��ҵ�����ݿ��е�Ωһ���
	char	szUsrId[MAX_SPID_LEN+1];		//�����õĻ���ID������910000
	char	szLoginId[MAX_SPID_LEN+1];		//�����ʺ�ID
	char	szPwd[MAX_PWD_LEN+1];			//���롣����910000
	char	szMobis[MAX_PHONE_NUM*MAX_PHONE_LEN+1];
	char	*szMsg;//[MAX_PACK_LEN+1];
	char	szExNo[MAX_CHANNEL_LEN+1];
	char	szIp[MAX_IPADDR_LEN+1];
	char    szSoapXmlns[MAX_PATH+1];
	int		nPort;
	int		nMobiCount;
	short	nConClose;					//�û�������ģʽ
	short	nReqMode;					//����ģʽsoap/post/get
	short	nRspType;					//������������
	int		nSockId;
	int		nSocket;
	INT64   nUsrMsgId;                     //�û���������ˮ��
	//zhubo 2017-4-7
	//IOCP_KEY_PTR pkey;
	time_t tRecvTime;

	char   szTimestamp[MAX_TIME_STAMP + 1];                       //ʱ�������10λ
	int    nFmtReq;
	char   szFuncTmp[MAX_PATH_FUNC+1];
	char   szCustId[MAX_CUSTID_LEN+1];                //�û��Զ�����ˮ��
//	INT64  nPackId;
	char   szSvrtype[MAX_SVRTYPE_LEN + 1];                      //��������
	TINYINT nVersion;                   //wbsV5.0�е�Э��汾
	TINYINT nTemplate;                  //std,cstd
	char    szExData[MAX_EXDATA_LEN + 1];

	char    szApiKey[MAX_APIKEY_LEN + 1];  //ApiKey

	/***************zhubo 2017-4-14*******************/
//	void *pNgxHttpRequest;	
	/***************zhubo 2017-4-14*******************/

	WBS_REQ_MT()
	{
		memset(this, 0, sizeof(WBS_REQ_MT));
	}
	
	WBS_REQ_MT& operator=(const WBS_REQ_MT& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(WBS_REQ_MT));
		memcpy(this, &other, sizeof(WBS_REQ_MT));
		return *this;
	}
};
typedef std::multimap<int, WBS_REQ_MT> MULTIMAP_WBSREQMT;

//////////////////////////////////////////////////////////////////////////
//�ļ��ϴ�������ؽṹ��
/*
struct WBS_REQ_FILEUPLOAD  //�����ϴ��ļ�
{
	int		nUID;							//��ҵ�����ݿ��е�Ωһ���
	char	szUsrId[MAX_SPID_LEN+1];		//�����õĻ���ID������910000
	char	szLoginId[MAX_SPID_LEN+1];		//�����ʺ�ID
	char	szPwd[MAX_PWD_LEN+1];			//���롣����910000
	char	szIp[MAX_IPADDR_LEN+1];
	int		nPort;
	short	nConClose;					//�û�������ģʽ
	short	nReqMode;					//����ģʽsoap/post/get
	short	nRspType;					//������������
	int		nSockId;
	int		nSocket;
	WBS_REQ_FILEUPLOAD()
	{
		memset(this, 0, sizeof(WBS_REQ_FILEUPLOAD));
	}
	
	WBS_REQ_FILEUPLOAD& operator=(const WBS_REQ_FILEUPLOAD& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(WBS_REQ_FILEUPLOAD));
		memcpy(this, &other, sizeof(WBS_REQ_FILEUPLOAD));
		return *this;
	}
};
typedef std::multimap<int, WBS_REQ_FILEUPLOAD> MULTIMAP_WBSREQFILEUPLOAD;
*/
//////////////////////////////////////////////////////////////////////////

struct WBS_REQ_QUERY					//����mo/rpt/��� 
{
	int		nUID;						//��ҵ�����ݿ��е�Ωһ���
	char	szUsrId[MAX_SPID_LEN+1];	//�����õĻ���ID������910000
	char	szLoginId[MAX_SPID_LEN+1];	//�����ʺ�ID
	char	szPwd[MAX_PWD_LEN+1];		//���롣����910000
	char	szIp[MAX_IPADDR_LEN+1];
	char    szSoapXmlns[MAX_PATH+1];
	int		nPort;
	short	nConClose;					//�û�������ģʽ
	short	nReqMode;					//����ģʽsoap/post/get
	short	nRspType;					//������������
	int		nSockId;
	int		nSocket;
	//zhubo 2017-4-7
	//IOCP_KEY_PTR pkey;
	time_t  tRecvTime;

	INT64  nPackId;
	int    nRetSize;
	char   szTimestamp[MAX_TIME_STAMP + 1];

	int    nFmtReq;
	char   szFuncTmp[MAX_PATH_FUNC + 1];
	TINYINT nVersion;                   //wbsV5.0�е�Э��汾
	TINYINT nTemplate;                  //std,cstd
	char    szExData[MAX_EXDATA_LEN + 1];

	char    szApiKey[MAX_APIKEY_LEN + 1];  //ApiKey


	WBS_REQ_QUERY()
	{
		memset(this, 0, sizeof(WBS_REQ_QUERY));
	}
	
	WBS_REQ_QUERY& operator=(const WBS_REQ_QUERY& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(WBS_REQ_QUERY));
		memcpy(this, &other, sizeof(WBS_REQ_QUERY));
		return *this;
	}
};
typedef std::multimap<int, WBS_REQ_QUERY> MULTIMAP_WBSQUERY;

struct WBS_UID_USERID_INFO
{
	int		nEcid;
	int		nUID;						    //��ҵ�����ݿ��е�Ωһ���
	char	szUsrId[MAX_SPID_LEN + 1];	    //�����õĻ���ID������910000
	char	szLoginId[MAX_SPID_LEN + 1];	    //wbs�����ʺ�

	WBS_UID_USERID_INFO()
	{
		memset(this, 0, sizeof(WBS_UID_USERID_INFO));
	}

	WBS_UID_USERID_INFO& operator=(const WBS_UID_USERID_INFO& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(WBS_UID_USERID_INFO));
		memcpy(this, &other, sizeof(WBS_UID_USERID_INFO));
		return *this;
	}
};

struct WBS_USER_INFO
{
	int     nEcid;                          //��ҵID
	int		nUID;						    //��ҵ�����ݿ��е�Ωһ���
	unsigned char uMaxCnn;                  //�û����������
	unsigned char uCnn;                     //�û���ǰ������
	unsigned char uMsgLevel;               //�û����ͼ��𣬴�0��10
	char	szUsrId[MAX_SPID_LEN+1];	    //�����õĻ���ID������910000
	char	szLoginId[MAX_SPID_LEN+1];	    //wbs�����ʺ�
	char	szPwd[MAX_PWD_LEN+1];	 	    //���롣����910000
	char	szIp[MAX_IPADDR_LEN+1];	        //Ip��ַ
	char	szIpList[MAX_LOGINIP_LEN+1];    //Ip��ַ
	char    szIPSegment[1024+1];                //IP��
	int		nPort;						    //�˿ں�
	int		nMoReq;						    //����Mo����
	int		nRptReq;					    //����rpt����
	int		nQueryReq;					    //�����Ѵ���
	int		nMtReq;					 	    //������mt����
	int		nMtReqEx;					    //�û���Mt�����ʵ�ʵİ���
	int		nMtNum;						    //��ǰ�û���ϵͳ��������Mt(�ڴ���+DB)
	int		nMoNum;						    //��ǰ�û���ϵͳ��������Mo(�ڴ���+DB)
	int		nRptNum;					    //��ǰ�û���ϵͳ��������Rpt(�ڴ���+DB)
	int     nUserPrivilege;                 //�û���Ȩ
	//SPGATE_INFO SpGateInfo[CHANNEL_TYPE_ARR*CHANNEL_SEQ_ARR];
	time_t  tLastActive;				//��ʱ���¼�û������һ������ʱ�䣬��ʱ��û�ж����ͻᱻ������û���
	time_t  tUseTime;					//������Ӧ��ʱ
	time_t	tRspUseTime;				//ƽ����Ӧʱ��(60��ͳ��һ��)
	int nReqNum;						//�������
	int nRspSpeed;                      //��Ӧ�ٶ�

	bool nKeepAlive;                    //��֧�ֳ�����   

	int nPwdEncode;                     //������ܷ�ʽ
	char szEncodestr[256 + 1];          //���ܴ�
	int nMsgCodeType;                   //�������ݵı����ʽ
	int nMsgEncode;                     //�������ݵļ��ܷ�ʽ
	int nGetrptmaxcnt;
	int nGetmomaxcnt;
	int nLoadFeeFlag;
	
	int nStatus;                       //�˺�״̬0������
	TINYINT nChargeObj;                //�ƷѶ���0���˺� 1����ҵ 
	TINYINT nChargeType;               //�Ʒѷ�ʽ0������ 1����� 
	TINYINT nFeeFlag;                  //�Ʒѱ�׼1��Ԥ���ѣ�2���󸶷�
	TINYINT nAccountType;              //�˺�����1���Ʒ��˺�
	//����ռ��4λ������ռǰ4λ
	TINYINT				 nMtCntType;

	WBS_USER_INFO()
	{
		memset(this, 0, sizeof(WBS_USER_INFO));
		nKeepAlive = 1;
	}
	
	WBS_USER_INFO& operator=(const WBS_USER_INFO& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(WBS_USER_INFO));
		memcpy(this, &other, sizeof(WBS_USER_INFO));
		return *this;
	}
};
typedef std::map<int, WBS_USER_INFO> WBS_USERINFO_MAP;
typedef std::vector<SPGATE_INFO> V_SPGATE_INFO;
typedef std::map<int/*uid*/, V_SPGATE_INFO> SPGATE_INFO_MAP;

struct WBS_RSP_PACK 
{
	int nUID;						//�û�UID
	int nCMD;						//����������
	int nPackLen;					//��Ӧ������
	int nSockId;					//socket id
	int nSocket;					//socket
	UINT nConClose;					//�Ƿ����ӶϿ�
	UINT nReqMode;					//����ģʽsoap/post/get
	int  nRspNUm;					//����Rpt/Mo��˵ָһ������ĸ���
	BOOL b100Continue;				//�û�Ӧ���Ƿ���100continue
	char szUserId[MAX_SPID_LEN+1];	//�û�ID
	char szRspData[MAX_WBS_RSPPACK_LEN+1];	//��Ӧ������
	unsigned int nFailNum;			//����ʧ�ܴ���
	time_t nLastPendTime;				//���һ�η���ʱ��
	time_t tRecvTime;				//�ص��������յ�����ʱ��

	int  nRspMode;                  //��Ӧģʽ 1ȫ�ɹ���2ȫʧ�ܣ�3�ɹ���ʧ�ܣ�4�ɹ�ʧ������
	
	/***************zhubo 2017-4-14*******************/
	int				   nRetMode;//��Ӧ��ʽ json xml
	UINT			   nstatusCode;

//	void 			  *pNgxHttpRequest;	
	/***************zhubo 2017-4-14*******************/

	//zhubo 2017-06-16
	int				   nRspType;

	WBS_RSP_PACK()
	{
		memset(this, 0, sizeof(WBS_RSP_PACK));
	}
	
	WBS_RSP_PACK& operator=(const WBS_RSP_PACK& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(WBS_RSP_PACK));
		memcpy(this, &other, sizeof(WBS_RSP_PACK));
		return *this;
	}
};

struct WBS_PACK_RECV 
{
	UINT nSockId;
	SOCKET nSocket;
	UINT nHttpHeaderLen; //httpͷ�ĳ���
	UINT nDataLen;		//���ݳ���
	UINT nConClose;		//�Ƿ�Ҫ���ӶϿ�
	UINT nReqMode;		//soap/post/getģʽ����(Ĭ��Ϊ0=soap,1=post,2=get)
	UINT nCMD;			//��������
	UINT nPort;
	UINT nSeqId;
	//zhubo 2017-4-7
	//IOCP_KEY_PTR pkey;
	char szIp[MAX_IPADDR_LEN+1];
	//zhubo 2017-4-14  ֻ����body������
	char *szData;//[MAX_PACK_LEN+1];//��������
	time_t tRecvTime;
	TINYINT nVersion;                   //wbsV5.0�е�Э��汾
	
	/***************zhubo 2017-4-14*******************/
	char    szFuncTmp[MAX_PATH_FUNC+1];
	TINYINT nFmtReq;// xml json url-encoded
	TINYINT nTemplate;//std cstd
//	void *pNgxHttpRequest;	
	/***************zhubo 2017-4-14*******************/
	WBS_PACK_RECV()
	{
		memset(this, 0, sizeof(WBS_PACK_RECV));
	}
	WBS_PACK_RECV& operator=(const WBS_PACK_RECV& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(WBS_PACK_RECV));
		memcpy(this, &other, sizeof(WBS_PACK_RECV));
		return *this;
	}
};

struct ROUTE
{
	int nSrcUID;			//Դ�ʺ�UID
	int nDestUID;			//Ŀ���ʺ�UID
	int nRouteType;			//·������
	int nGateType;			//ͨ������
	int nFeeFlag;			//ͨ������
	int nExNoHold;			//�Ƿ�����չ�Ӻű�־
	int nExpiredTm;			//��ʱʱ��
	int nRouteSeq;			//·�����
	int nProportion;		//���ͱ���
	int nSrcChannelNo;		//Դͨ��ID
	int nDestChannelNo;		//Ŀ��ͨ��ID
	int nMaxRemained;		//��ͨ���������������  ��ֵΪ<0ʱ��ʾ��������������
	int nUpperLen;			//���ŷ�Χ����
	int nLowerLen;			//���ŷ�Χ����
	char szSrcUserId[MAX_SPID_LEN+1];		//Դ�ʺ�
	char szDestUserId[MAX_SPID_LEN+1];		//Ŀ���ʺ�
	char szSrcSpNumber[MAX_CHANNEL_LEN+1];	//Դͨ����
	char szSrcSpGate[MAX_CHANNEL_LEN+1];	//Դ��ͨ��
	char szDestSpNumber[MAX_CHANNEL_LEN+1];	//Ŀ��ͨ����
	char szDestSpGate[MAX_CHANNEL_LEN+1];	//Ŀ����ͨ��
	char szParams[MAX_PARAMS_LEN+1];		//·�ɲ���
	ROUTE()
	{
		memset(this, 0, sizeof(ROUTE));
	}
	ROUTE& operator=(const ROUTE& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(ROUTE));
		memcpy(this, &other, sizeof(ROUTE));
		return *this;
	}
};
typedef std::map<INT64, ROUTE> MAP_SUPPSEND;


struct KeyWords
{
	char szWords[MAX_LEN_KEYWORDS+1];
	int  iType;
	int  iLevel;
	KeyWords()
	{
		memset(this, 0, sizeof(KeyWords));
	}
};


//·�ɱ�����
struct  ROUTE_KEY
{
	int nSrcUID;
	int nRouteType;
	int nGateType;
	int nRouteSeq;
	ROUTE_KEY(int nUID, int nRType, int nGtype=0, int nRSeq=0)
	{
		memset(this, 0, sizeof(ROUTE_KEY));
		nSrcUID		= nUID;
		nRouteType	= nRType;
		nGateType   = nGtype;
		nRouteSeq	= nRSeq;
	}
	
	ROUTE_KEY()
	{
		memset(this, 0, sizeof(ROUTE_KEY));
	}
	
	ROUTE_KEY& operator=(const ROUTE_KEY& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(ROUTE_KEY));
		memcpy(this, &other, sizeof(ROUTE_KEY));
		return *this;
	}
	
	friend bool operator < (const ROUTE_KEY& x ,const ROUTE_KEY& y)
	{
		//UID�Ƚ�
		if (x.nSrcUID == y.nSrcUID)
		{
			//·�����ͱȽ�
			if (x.nRouteType == y.nRouteType)
			{
				//���ڰ����ͱ���·�������⴦��
				if (4 == x.nRouteType)
				{
					//�������ͱȽ�
					if (x.nGateType == y.nGateType)
					{
						return x.nRouteSeq < y.nRouteSeq;
					}
					else
					{
						return x.nGateType < y.nGateType;
					}
				}
				else
				{
					return x.nGateType < y.nGateType;
				}
			}
			else
			{
				return x.nRouteType < y.nRouteType;
			}
		}
		else
		{
			return x.nSrcUID < y.nSrcUID;
		}
	}
};
typedef std::multimap<ROUTE_KEY, ROUTE> MT_ROUTE_MAP;

struct TMP_ROUTE
{
	int nUID;
	short nType;
	//�û��ʺţ���������˵�ǽ��������ľ�ͨ���Ż�����ͨ���ţ���������˵���ǽ���ͨ���Ż��ɾ�ͨ���Ų��ҽ������͸����ʺ�
	char szUserId[MAX_SPID_LEN+1];
	char szOldSpNo[MAX_CHANNEL_LEN+1]; //��ͨ��
	char szNewSpNo[MAX_CHANNEL_LEN+1]; //��ͨ��	
	TMP_ROUTE()
	{
		memset(this, 0, sizeof(TMP_ROUTE));
	}
	TMP_ROUTE& operator=(const TMP_ROUTE& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(TMP_ROUTE));
		memcpy(this, &other, sizeof(TMP_ROUTE));
		return *this;
	}
/*
	bool operator==(const TMP_ROUTE& other)
	{
		if (this == &other)
		{
			return true;
		}
		bool bRet = false;
		if (0 == memcmp(other.szUserId, this->szUserId, sizeof(this->szUserId))
			&& 0 == memcmp(other.szOldSpNo, this->szOldSpNo, sizeof(this->szOldSpNo)))
		{
			bRet = true;
		}
		return bRet;
	}
*/
};
typedef std::multimap<std::string,TMP_ROUTE> TMP_ROUTE_MAP;


/************************************************************************/
							/* ����webservice */
/************************************************************************/

/*
//��������������Ϣ����һ���룬��һ���ݣ�
struct MT_REQ_PACK
{
	COMMAND  command;		//  ��������
	char spid[6+1];			//	SP��ID	
	char sppassword[32+1];	//	SP����	
	char spsc[10+1];		//	�������
	char sa[21+1];			//	Դ��ַ	
	char da[13+1];			//	Ŀ���ַ
	char ec;				//	esm_class
	char pid;				//	protocol_id	
	char dc;				//	��Ϣ�����ʽ
	char sm[720];			//	��Ϣ����
	int	 nMsgLen;			//  ��Ϣ����
	char priority;			//	���ȼ�		
	char attime[14+1];		//  ʱ����ʱ��	
	char validtime[14+1];	//  ��Чʱ��	
	MT_REQ_PACK()
	{
		memset(this, 0, sizeof(MT_REQ_PACK));
	}
	MT_REQ_PACK& operator=(const MT_REQ_PACK& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(MT_REQ_PACK));
		memcpy(this, &other, sizeof(MT_REQ_PACK));
		return *this;
	}
};
*/
//������ͬ������������������Ϣ����ͬ���룬��ͬ���ݣ�
struct MULTI_MT_REQ_PACK
{
	COMMAND  command;						//  ��������
	char spid[MAX_SPID_LEN+1];				//	SP��ID	
	char sppassword[MAX_PWD_LEN+1];			//	SP����	
	char spsc[MAX_SRVID_LEN+1];				//	�������
	char sa[MAX_SRCID_LEN+1];				//	Դ��ַ	
	char das[SIGN_MAX_PHONE_LEN*MAX_HTTP_PHONE_NUM+1];//	Ŀ���ַ�б�(��","����������������100)
	int  nDaNum;							//  �ֻ��Ÿ���
	int	 taskid;							//	����id��
	char ec;								//	esm_class
	char pid;								//	protocol_id	
	char dc;								//	��Ϣ�����ʽ
	char sm[2*MAX_MSG_LEN+1];				//	��Ϣ����
	int  nMsgLen;							//  ��Ϣ����
	char priority;							//	���ȼ�		
	char attime[MAX_WBS_ATTIME_LEN+1];		//  ʱ����ʱ��	
	char validtime[MAX_WBS_VALIDTIME_LEN+1];//  ��Чʱ��	
	MULTI_MT_REQ_PACK()
	{
		memset(this, 0, sizeof(MULTI_MT_REQ_PACK));
	}
	MULTI_MT_REQ_PACK& operator=(const MULTI_MT_REQ_PACK& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(MULTI_MT_REQ_PACK));
		memcpy(this, &other, sizeof(MULTI_MT_REQ_PACK));
		return *this;
	}
};

struct MULTIX_MT
{
	char das[SIGN_MAX_PHONE_LEN+1];		//	Ŀ���ַ�б�
	char sm[MAX_MSG_LEN*2*2+1];		//	��Ϣ����
	int  nMsgLen;					//  ��Ϣ����
	MULTIX_MT()
	{
		memset(this, 0, sizeof(MULTIX_MT));
	}
	MULTIX_MT& operator=(const MULTIX_MT& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(MULTIX_MT));
		memcpy(this, &other, sizeof(MULTIX_MT));
		return *this;
	}
};

//���Ͳ�ͬ������������������Ϣ����ͬ���룬��ͬ���ݣ�
struct MULTIX_MT_REQ_PACK
{
	COMMAND  command;						//  ��������
	char spid[MAX_SPID_LEN+1];				//	SP��ID	
	char sppassword[MAX_PWD_LEN+1];			//	SP����	
	char spsc[MAX_SRVID_LEN+1];				//	�������
	char sa[MAX_SRCID_LEN+1];				//	Դ��ַ	
	MULTIX_MT dasmarr[MAX_HTTP_PHONE_NUM];		//  �ֻ���+�������ݶ�Ӧ�б�
	int  nDaNum;							//  �ֻ��Ÿ���(dasmarr����Ĵ�С)
	int	 taskid;							//	����id��
	char dc;								//	��Ϣ�����ʽ
	char ec;								//	esm_class
	char pid;								//	protocol_id	
	char priority;							//	���ȼ�		
	char attime[MAX_WBS_ATTIME_LEN+1];		//  ʱ����ʱ��	
	char validtime[MAX_WBS_VALIDTIME_LEN+1];//  ��Чʱ��	
	MULTIX_MT_REQ_PACK()
	{
		memset(this, 0, sizeof(MULTIX_MT_REQ_PACK));
	}
	MULTIX_MT_REQ_PACK& operator=(const MULTIX_MT_REQ_PACK& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(MULTIX_MT_REQ_PACK));
		memcpy(this, &other, sizeof(MULTIX_MT_REQ_PACK));
		return *this;
	}
};

//�����ļ�Ⱥ������������Ϣ
struct BATCH_MT_REQ_PACK
{
	COMMAND  command;						//  ��������
	char spid[MAX_SPID_LEN+1];				//	SP��ID	
	char sppassword[MAX_PWD_LEN+1];			//	SP����	
	char spsc[MAX_SRVID_LEN+1];				//	�������
	char sa[MAX_CHANNEL_LEN+1];				//  Դ��ַ
	char bmttype;							//  Ⱥ������ 1:��ͬ����Ⱥ�� 2:��ͬ����Ⱥ�� 3:��̬ģ��Ⱥ��
	int	 taskid;							//	����id��
	char title[MAX_TITLE_LEN+1];			//	Ⱥ������ ��ҪEncode���룬����ǰ�Ϊ20�ַ�
	char content[MAX_MSG_LEN*2+1];			//	Ⱥ������(��ѡ)
	int  nMsgLen;							//  ��Ϣ����(û��������0)
	char url[MAX_PATH+1];					//	Ⱥ���ļ�url
	char dc;								//	��Ϣ�����ʽ
	char priority;							//	���ȼ�		
	char attime[MAX_WBS_ATTIME_LEN+1];		//  ʱ����ʱ��	
	char validtime[MAX_WBS_VALIDTIME_LEN+1];//  ��Чʱ��	
	
	TINYINT			nEncodeFlag;      //�����־ 1�����ģ�2��base64
	BATCH_MT_REQ_PACK()
	{
		memset(this, 0, sizeof(BATCH_MT_REQ_PACK));
	}
	BATCH_MT_REQ_PACK& operator=(const BATCH_MT_REQ_PACK& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(BATCH_MT_REQ_PACK));
		memcpy(this, &other, sizeof(BATCH_MT_REQ_PACK));
		return *this;
	}
};

//��Ӧ��Ӧ��Ϣ
struct RSP_PACK
{
	COMMAND  command;								//  ��������
	char spid[MAX_SPID_LEN+1];						//	SP��ID	
	char msgids[MAX_MSGIDSTR_LEN*MAX_PHONE_NUM+1];	//  ��ϢID�б���","������������100�� ���ʹ�����Ӧ��Ϣʱ������Ч
	int	 nMsgIdNum;									//  ��ϢID���� ���ʹ�����Ӧ��Ϣʱ������Ч
	char stat[MAX_ERSTAT_LEN+1];					//  ��Ϣ״̬
	char errcode[MAX_WBS_ERCODE_LEN+1];				//	������

	int  nRspMode;  //��Ӧģʽ
	int  nRspFmt;  //��Ӧ��ʽ  
	RSP_PACK()
	{
		memset(this, 0, sizeof(RSP_PACK));
	}
	RSP_PACK& operator=(const RSP_PACK& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(RSP_PACK));
		memcpy(this, &other, sizeof(RSP_PACK));
		return *this;
	}
};

//����Mo����
struct MO_REQ_PACK
{
	COMMAND  command;					//  ��������
	char spid[MAX_SPID_LEN+1];			//	SP��ID	
	char sppassword[MAX_PWD_LEN+1];		//  ����()
	char spsc[MAX_SRVID_LEN+1];			//  ҵ�����
	char momsgid[MAX_MSGIDSTR_LEN+1];	//  ��ϢID
	char sa[MAX_SRCID_LEN+1];			//  Դ��ַ
	char da[MAX_SRCID_LEN+1];			//  Ŀ�ĵ�ַ
	char ec;							//	esm_class
	char pid;							//	protocol_id	
	char dc;							//	��Ϣ�����ʽ
	char sm[MAX_MSG_LEN*2+1];			//	��Ϣ����
	int  nMsgLen;						//  ��Ϣ����
	MO_REQ_PACK()
	{
		memset(this, 0, sizeof(MO_REQ_PACK));
	}
	MO_REQ_PACK& operator=(const MO_REQ_PACK& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(MO_REQ_PACK));
		memcpy(this, &other, sizeof(MO_REQ_PACK));
		return *this;
	}
};

//����RPT����
struct RPT_REQ_PACK
{
	COMMAND  command;						//  ��������
	char spid[MAX_SPID_LEN+1];				//	SP��ID	
	char sppassword[MAX_PWD_LEN+1];			//  ����()
	char spsc[MAX_SRVID_LEN+1];				//  ҵ�����
	char mtmsgid[MAX_MSGIDSTR_LEN+1];		//  MT��ϢID
	char mtstat[MAX_ERSTAT_LEN+1];			//  MT��Ϣ״̬
	char mterrcode[MAX_WBS_ERCODE_LEN+1];	//	MT�������
	RPT_REQ_PACK()
	{
		memset(this, 0, sizeof(RPT_REQ_PACK));
	}
	RPT_REQ_PACK& operator=(const RPT_REQ_PACK& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(RPT_REQ_PACK));
		memcpy(this, &other, sizeof(RPT_REQ_PACK));
		return *this;
	}
};

//����ģʽMT�����(�������ļ�����)
struct HSBC_MT_REQ_PACK
{
	int		nUID;						//��ҵ�����ݿ��е�Ωһ���
	char	szUsrId[MAX_SPID_LEN+1];	//�����õĻ���ID������910000
	char	szLoginId[MAX_SPID_LEN+1];	//�����ʺ�ID
	char	szPwd[MAX_PWD_LEN+1];		//���롣����910000
	char	szIp[MAX_IPADDR_LEN+1];
	int		nPort;
	short	nConClose;					//�û�������ģʽ
	short	nReqMode;					//����ģʽsoap/post/get
	short	nCMD;						//������������
	int		nSockId;
	int		nSocket;
	MULTI_MT_REQ_PACK multi_mt_req;
	MULTIX_MT_REQ_PACK multix_mt_req;
	time_t  tRecvTime;
	void*	pNgxRequest;
	HSBC_MT_REQ_PACK()
	{
		memset(this, 0, sizeof(HSBC_MT_REQ_PACK));
	}
	HSBC_MT_REQ_PACK& operator=(const HSBC_MT_REQ_PACK& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(HSBC_MT_REQ_PACK));
		memcpy(this, &other, sizeof(HSBC_MT_REQ_PACK));
		return *this;
	}
};

//����ģʽ�ļ��ύmt�����
struct HSBC_BATCH_MT_REQ_PACK
{
	int		nUID;						//��ҵ�����ݿ��е�Ωһ���
	char	szUsrId[MAX_SPID_LEN+1];	//�����õĻ���ID������910000
	char	szLoginId[MAX_SPID_LEN+1];	//�����ʺ�ID
	char	szPwd[MAX_PWD_LEN+1];		//���롣����910000
	char	szIp[MAX_IPADDR_LEN+1];
	int		nPort;
	short	nConClose;					//�û�������ģʽ
	short	nReqMode;					//����ģʽsoap/post/get
	short	nCMD;						//������������
	int		nSockId;
	int		nSocket;
	INT64   nbmtmsgid;					//�ļ�msgid
	INT64   nMsgIdTmp;					//��Ϊ���������MSGID������������Ч
	char	cDownload;					//�ļ��Ƿ��ѱ����� 0:δ���� 1:������
	char	szLocalPath[MAX_PATH+1];	//�����ļ�·��
	BATCH_MT_REQ_PACK batch_mt_req;
	time_t tRecvTime;
	void*	pNgxRequest;
	HSBC_BATCH_MT_REQ_PACK()
	{
		memset(this, 0, sizeof(HSBC_BATCH_MT_REQ_PACK));
	}
	HSBC_BATCH_MT_REQ_PACK& operator=(const HSBC_BATCH_MT_REQ_PACK& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(HSBC_BATCH_MT_REQ_PACK));
		memcpy(this, &other, sizeof(HSBC_BATCH_MT_REQ_PACK));
		return *this;
	}
};
typedef std::deque<HSBC_BATCH_MT_REQ_PACK> DEQUE_BATCH_MT;

//���н�������/״̬����ķ�������Ϣ/����������Ϣ
struct HSBC_URLINFO
{
	int  nEcid;                      //�˺�������ҵID
	int  nUID;						//�ʺŵ�UID
	char szUsrId[MAX_SPID_LEN+1];	//�ʺ�
	char szPwd[MAX_PWD_LEN+1];		//����
	char szMoUsrId[MAX_MO_USERID_LEN + 1];	//����MO�ʺ�
	char szMoPwd[MAX_PWD_LEN + 1];		//����MO����
	char szRptUsrId[MAX_RPT_USERID_LEN + 1];	//����RPT�ʺ�
	char szRptPwd[MAX_PWD_LEN + 1];		//����RPT����
	char szIp[MAX_DOMAIN_LEN+1];	//��IP
	char szDomain[MAX_DOMAIN_LEN+1];//����
	int  nPort;						//�󶨶˿�
	char szWebPath[MAX_PATH+1];		//webĿ¼
	char cRptFlag;					//rpt/mo��־ 0:mo 1:rpt
	short nWndCnt;					//��ǰ���ڴ�С��Ϊ0˵�����У�>0˵������δ��ɵ�����
	short nState;					//����/���ߣ�Ĭ��Ϊ���� OFFLINE ONLINE �����ݿ������ʱĬ�����ߣ���������Ҳ��Ĭ������
	time_t tOffline;				//�������ߵ�ʱ��
	time_t tWndFlow;				//���ڴ�С�����ʱ��
	int    nPushCnt;                //���ʹ���
    int    nPushFail;               //uid�ʺŵ�����ʧ�ܴ��� ���� .EXEC WS_GETURLBINDINFOV1 userdata��
	unsigned int  nOfftime;            //������ʱ�䵥λS

	int nPushVersion;              //���Ͱ汾
	int nPushmoversion;
	int nPushrptversion;
	int nPushtimefmt;

	int nPushMoFmt;                //nPushVersionv=48���ϣ�Э��3.0����,nPushMoFmt����Ч��pushmofmt : WebServiceTool-V5.3  = 4 URL:JSON, 2 JSON:JSON, 1 XML:XML ��
	int nPushRptFmt;
	int nPushMsgEncode;                //�������ݵļ��ܷ�ʽ
	int nPushCodeType;                 //�������ݵı����ʽ
	int nPushFailCnt;                  //v3.0�˺��Զ����������ʧ�ܴ�����  �����ݿ⵽�ڴ棬�ٸ�ֵ��PACK_DELIVER::nReSendCnt��
	int nPushSlideWnd;                 //���ͻ������ڵĴ�С

	int nPushrptmaxcnt;
	int nPushmomaxcnt;

    int nHttpMoFlag;                     //0��http 1��https
    int nHttpRptFlag;                     //0��http 1��https
    char szMoUrl[MAX_URL_LEN];		//mourl
    char szRptUrl[MAX_URL_LEN];		//rpturl
    char pCACertName[MAX_CA_NAME + 1]; //CA֤���׼���,��https://curl.haxx.se/docs/caextract.html��ȡ�����ļ���������Ϊpem��ʽ����Ϊlibcurlֻ֧��pem��ʽ��������"E:\\ZWF\\https_client\\test_https\\cert\\ca-bundle.pem")
    int iVerifyPeer; //�Ƿ�У��Զˣ�1:���Է����֤����ݺϷ���У�飬 2���Է����֤����ݺϷ���У��
    int iVerifyHost; //�Ƿ����������У�飺1����ʾ�����֤�飬2:��ʾ���֤�����Ƿ���CN(common name)�ֶ�, 3:��ʾ��1�Ļ�����У�鵱ǰ�������Ƿ���CNƥ��

	TINYINT				 nPushCntType;
	TINYINT				 nSignType;    //ǩ����ʽ
	
	HSBC_URLINFO()
	{
		memset(this, 0, sizeof(HSBC_URLINFO));
	}
	HSBC_URLINFO& operator=(const HSBC_URLINFO& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(HSBC_URLINFO));
		memcpy(this, &other, sizeof(HSBC_URLINFO));
		return *this;
	}
};
typedef std::multimap<int,HSBC_URLINFO> HSBC_URLINFO_MAP;


//��ȡϵͳ��ǰʱ����Զ�����չ�ṹ��
//
/*
#ifndef SYSTEMTIMEEX
typedef struct SYSTEMTIME_EX
{
    int m_iYear;
    int m_iMonth;
    int m_iDayOfWeek;
    int m_iDay;
    int m_iHour;
    int m_iMinute;
    int m_iSecond;
    int m_iMilliseconds;
	
    SYSTEMTIME_EX()
    {
        memset(this, 0, sizeof(struct SYSTEMTIME_EX));
    }
	
    SYSTEMTIME_EX& operator=(const struct SYSTEMTIME_EX& other)
    {
        if(this == &other)
        {
            return *this;
        }
        memset(this, 0, sizeof(struct SYSTEMTIME_EX));
        memcpy(this, &other, sizeof(struct SYSTEMTIME_EX));
        return *this;
    }
	
}SYSTEMTIME_EX,*LPSYSTEMTIME_EX;
#endif
*/
//////////////////////////////////////////////////////////////////////////
//ƽ̨�����ؽṹ��
//////////////////////////////////////////////////////////////////////////
enum
{	
	TYPE_MON_USERINFO=130,				//�û��ʺ���Ϣ
		TYPE_MON_SPGATEINFO,				//ͨ���ʺ���Ϣ
		TYPE_MON_THREADINFO,				//ϵͳ�̹߳������
		TYPE_MON_BUFINFO,					//ϵͳ��ǰ������Ϣ
		TYPE_MON_SYSINFO,					//ϵͳ��Դռ����Ϣ
		TYPE_MON_LOGINFO,					//ƽ̨�����־��Ϣ
		TYPE_MON_ENDINFOR,						//������ϱ�ʶ
		TYPE_MON_EXCEPTION,						//ͻ���쳣��Ϣ
};

enum
{
	THRSTATUS_INIT=0,					//�߳�����������(��ʼ״̬)
	THRSTATUS_IDLE=10,					//����
	THRSTATUS_BUSY=20,					//��æ
	THRSTATUS_VERYBUSY=30,				//�ǳ���æ
	THRSTATUS_DEADLOOP=40,				//��ѭ��
	THRSTATUS_NORESPONSE=50,			//��ʱ������Ӧ
	THRSTATUS_ERROR=60,					//�����쳣
	THRSTATUS_EXIT=70					//���˳�
};

#define LOGTYPE_LOGIN_BASE		100			//��½��־
enum
{
	LOG_LOGIN_SUCCESS=LOGTYPE_LOGIN_BASE,	//��½�ɹ�
	LOG_LOGIN_FAILED						//��½ʧ��
};

#define LOGTYPE_DBOPT_BASE		500			//db������־
enum
{
	LOG_DB_WR_SUCCESS=LOGTYPE_DBOPT_BASE,		//д��ɹ�
	LOG_DB_WR_FAILED,							//д��ʧ��
	LOG_DB_RD_SUCCESS=LOGTYPE_DBOPT_BASE+10,	//����ɹ�
	LOG_DB_RD_FAILED,							//����ʧ��
	LOG_DB_UPD_SUCCESS=LOGTYPE_DBOPT_BASE+20,	//���¿�ɹ�
	LOG_DB_UPD_FAILED,							//���¿�ʧ��
	LOG_DB_DEL_SUCCESS=LOGTYPE_DBOPT_BASE+30,	//ɾ����¼�ɹ�	
	LOG_DB_DEL_FAILED,							//ɾ����¼ʧ��
	LOG_DB_OPT_SUCCESS=LOGTYPE_DBOPT_BASE+40,	//������ɹ�(ͨ��)
	LOG_DB_OPT_FAILED							//������ʧ��(ͨ��)
};

#define LOGTYPE_SYSER_BASE		600			//ϵͳ�ڲ��쳣
enum
{
	LOG_SYS_NORMAL=LOGTYPE_SYSER_BASE,		//ϵͳ����
	LOG_SYS_ERROR,							//ϵͳ�쳣
	LOG_SYS_BUSSINESS_ERROR					//ϵͳ��ĳ��ҵ���쳣
};

//�����Ϣ���ౣ��
typedef std::map<int, DEQUE_MO> MAP_MONINFO;

//ƽ̨��ؽṹ
//ƽ̨ϵͳ��Ϣ
struct MON_SYSINFO
{
    int nCpuUsage;					//CPUʹ�������б�Ҫʱ��ʽ��XX%���ַ�������ʾ
    unsigned int nMemUsage;				//�����ڴ�ʹ��������MΪ��λ
    unsigned int nVmemUsage;			//�����ڴ�ʹ��������MΪ��λ
	int nDiskFreeSpace;				//����ʣ��������MΪ��λ
    MON_SYSINFO()
    {
        memset(this, 0, sizeof(MON_SYSINFO));
    }

    MON_SYSINFO& operator=(const MON_SYSINFO& other)
    {
        if (this == &other)
        {
            return *this;
        }
        memset(this, 0, sizeof(MON_SYSINFO));
        memcpy(this, &other, sizeof(MON_SYSINFO));
        return *this;
    }
};

//ƽ̨��־��Ϣ
struct MON_LOGINFO
{
	int nLogType;
	char szLog[1024+1];
    MON_LOGINFO()
    {
		memset(this, 0, sizeof(MON_LOGINFO));
    }
	
    MON_LOGINFO& operator=(const MON_LOGINFO& other)
    {
        if (this == &other)
        {
            return *this;
        }
        memset(this, 0, sizeof(MON_LOGINFO));
        memcpy(this, &other, sizeof(MON_LOGINFO));
        return *this;
    }
};

//�û��ʺż����Ϣ
struct MON_USERINFO
{
	int		nLinkNum;						//�ʺ�������
	char	cOnlineStatus;					//����״̬ 0:���� 1:���� 2:δ֪
	int		nUserFee;						//�ʺŷ���
	int		nSuccessRate;					//�ɹ���
	int		nMoTotal;						//MO����
	int		nRptTotal;						//RPT����
	int		nMtTotalSnd;					//MT�ύ���� �ͻ��ӽ�MT��������
	int		nMtHaveSnd;						//MT��ת���� �ѳɹ�ת����MT����
	int		nMtRemained;					//MT������ ��ǰ����ƽ̨δת����MT����
	int		nMtSndSpd;						//MT�ύ�ٶ� �ͻ��ύMT���ٶ�
	int		nMoHaveRecv;					//MO�������� �ͻ���ǰ�յ���Mo���� 
	int		nMoRemained;					//MO������ �ͻ���ǰ������ƽ̨��Mo����
	int		nMoRptRecvSpd;					//Mo/Rpt�����ٶ� �ͻ�����Mo/Rpt���ٶ�
	char	cMoTmOutCnt;					//����MO��ʱ���� �ͻ�����Moʱ����ʱ�Ĵ���
	int		nRptHaveRecv;					//Rpt�������� �ͻ���ǰ�յ���Rpt����
	int		nRptRemained;					//Rpt������	�ͻ���ǰ������ƽ̨��Rpt����
	char	cRptTmOutCnt;					//����Rpt��ʱ���� �ͻ�����Rptʱ����ʱ�Ĵ���
	char    szUserId[MAX_SPID_LEN+1];       //�����õĻ���ID������910000
	unsigned char ucJType;					//�û�ʹ�õĽ��뷽ʽ������Э��
	char	szIp[MAX_IPADDR_LEN+1];			//�ʺŵ�½IP
	char	szLoginInTm[MAX_TIME_LEN+1];	//���һ�ε�½ʱ��	���һ�ε�½��ʱ��
	char	szLoginOutTm[MAX_TIME_LEN+1];   //���һ������ʱ��	���һ�����ߵ�ʱ��
	char	szMtSndInfo[1024+1];			//���ַ�������ʽ�����û�ÿ��ͨ���ķ�����/������

    MON_USERINFO()
    {
        memset(this, 0, sizeof(MON_USERINFO));
    }

    MON_USERINFO& operator=(const MON_USERINFO& other)
    {
        if (this == &other)
        {
            return *this;
        }
        memset(this, 0, sizeof(MON_USERINFO));
        memcpy(this, &other, sizeof(MON_USERINFO));
        return *this;
    }
};

//ͨ�������Ϣ
struct MON_SPGATEINFO
{
	int		nLinkNum;						//�ʺ�������
	char	cOnlineStatus;					//����״̬ 0:���� 1:���� 2:δ֪
	int		nMtTotal;						//MT����
	int		nMtHaveSnd;						//MT�������� �����ͨ�������͵�MT����
	int		nMtRemained;					//MT������	��ǰƽ̨������ͨ����MT����
	int		nMtRecvSpd;						//MT�����ٶ� ��ͨ����������ת��MT���ٶ�
	int		nMoTotalRecv;					//MO�������� ��ͨ���յ���MO����
	int		nMoHaveSnd;						//MOת����	��ת�����û��ĸ�ͨ����MO����
	int		nMoRemained;					//MO������	��ͨ��������MO����
	int		nMoSndSpd;						//MOת���ٶ� ��ͨ��������ת��MO���ٶ�
	int		nRptTotalRecv;					//Rpt�������� ��ͨ���յ���Rpt����
	int		nRptHaveSnd;					//Rptת����	��ת�����û��ĸ�ͨ����Rpt����
	int		nRptRemained;					//Rpt������	��ͨ��������Rpt����
	int		nRptSndSpd;						//Rptת���ٶ� ��ͨ��������ת��Rpt���ٶ�
	int		nSndErCnt;						//ͨ�������쳣����
	unsigned char ucErType;					//�쳣����,0����ʾ���쳣 ����ֵ����ʾ��ͨ����ǰ��������쳣״̬
	char    szUserId[MAX_SPID_LEN+1];       //�����õĻ���ID������910000
	char	szIp[MAX_IPADDR_LEN+1];			//�ʺŵ�½IP
	char	szLastSndErTm[MAX_TIME_LEN+1];	//���һ��ͨ�������쳣��ʱ��
	char	szLoginInTm[MAX_TIME_LEN+1];	//���һ�ε�½ʱ��	���һ�ε�½��ʱ��
	char	szLoginOutTm[MAX_TIME_LEN+1];   //���һ������ʱ��	���һ�����ߵ�ʱ��
	char	szMtSndInfo[1024+1];			//���ַ�������ʽ�����û�ÿ��ͨ���ķ�����/������/��ʡ������/��ʡ�ĳɹ���

    MON_SPGATEINFO()
    {
        memset(this, 0, sizeof(MON_SPGATEINFO));
    }

    MON_SPGATEINFO& operator=(const MON_SPGATEINFO& other)
    {
        if (this == &other)
        {
            return *this;
        }
        memset(this, 0, sizeof(MON_SPGATEINFO));
        memcpy(this, &other, sizeof(MON_SPGATEINFO));
        return *this;
    }
};

//ϵͳ������Ϣ
struct MON_BUFINFO
{
	int		nEndCnt;				//���������
	int		nMoTotalRecv;			//MO�������� ���ת�������ص�MO����
	int		nMtTotalSnd;			//MTת������ ���سɹ�ת������˵�MT����
	int		nWrMoBuf;				//MOд�⻺�� дMO_TASK�����С
	int		nUpdMoBuf;				//MO���»��� ����MO_TASK�����С
	int		nUpdRptBuf;				//RPT���»��� ����Mt_Task����SendStatus�ֶ�
	int		nWrRptBuf;				//Rptд�⻺�� дRpt_wait_b�����С
	int		nEndRspBuf;				//��˻�Ӧ���� ��˸߼����Ӧ����
	char 	szMtSndBuf[256+1];		//MT���ͻ��� ��������ת��MT�ķ��ͻ��壨0-9�����м���/�ָ�����0/1/2/3/4/5/6/7/8/9
	int		nMtSndBuf;				//MT���ͻ��� ��������ת��MT�ķ��ͻ��壨0-9����0-9�����ܺ�
	int		nMtWaitBuf;				//MT���͵ȴ����� ��������ת��MT�ķ��͵ȴ�����
	int		nPreCnt;				//ǰ��������
	int		nMtTotalRecv;			//MT�������� ���յ�ǰ��MT������
	int		nMoTotalSnd;			//MOת������ �ɹ�ת����ǰ���û���MO����
	int		nWrMtTaskBuf;			//дMTTASK���� дMt_task�����С
	int		nWrMtTmBuf;				//дMTTMR���� дMt_timer_que�����С
	int		nWrMtVfyBuf;			//дMTVFY���� дMt_verify_wait�����С
	int		nWrMtLvlBuf;			//дMTLVL���� дMt_level_que�����С
	int		nPreRspBuf;				//ǰ�˻�Ӧ���� ǰ�˸߼����Ӧ����
	int		nPreRspTmpBuf;			//ǰ�˻�Ӧ��ʱ���� ǰ�˸߼����Ӧ��ʱ����
	int		nMoSndBuf;				//Mo���ͻ��� Mo���ͻ���
	int		nRptSndBuf;				//Rpt���ͻ��� Rpt���ͻ���
	int		nMoRptWaitBuf;			//MO/RPT���͵ȴ����� MO/RPT���͵ȴ�����
	int		nLogFileNum;			//��־�ļ�����
	int		nLogBuf;				//��־����
	int		nRecvBuf;				//���ջ���
	int		nReSndBuf;				//�ط�����
	int		nSuppSndBuf;			//��������
	int		nMonLogBuf;				//�����־����
	int		nMtRemained;			//MT������
	int		nMoRemained;			//MO������
	int		nRptRemained;			//RPT������
    MON_BUFINFO()
    {
        memset(this, 0, sizeof(MON_BUFINFO));
    }
	
    MON_BUFINFO& operator=(const MON_BUFINFO& other)
    {
        if (this == &other)
        {
            return *this;
        }
        memset(this, 0, sizeof(MON_BUFINFO));
        memcpy(this, &other, sizeof(MON_BUFINFO));
        return *this;
    }
};

//�����Ϣ���������Ϣ
typedef int MON_ENDINFO;

/*
//ͻ���쳣�������
enum MON_EXCEPTION_TYPE
{
	MON_EXCEPTION_UNKNOWN,
	MON_EXCEPTION_USER_FEE,	//�û�Ƿ��
	MON_EXCEPTION_FWD_ERR,//��Ӫ�̶���
	MON_EXCEPTION_USER_OFFLINE,//�û�����
	MON_EXCEPTION_FWD_OFFLINE,//SPGATE����
};
struct EXCEPTION_USER_FEE
{
	bool IsFeeLower;
	int UID;	
};
struct EXCEPTION_USER_OFFLINE
{
	bool IsOnline;
	int UID;	
};
struct EXCEPTION_FWD_OFFLINE
{
	bool IsOnline;
	int UID;	
};
struct EXCEPTION_FWD_ERR
{
	int UID;	
	unsigned char ErrCode;
};
//ͻ���쳣�����������
struct  MON_EXCEPTION_INFO
{
	MON_EXCEPTION_TYPE ExceptionType;            
	time_t MonTime;
    union 
	{
		EXCEPTION_USER_FEE UserFeeInfo; 
		EXCEPTION_FWD_ERR FwdErrInfo;
		EXCEPTION_USER_OFFLINE UserOfflineInfo;
		EXCEPTION_FWD_OFFLINE FwdOfflineInfo;
	};
	MON_EXCEPTION_INFO()
	{
		memset(this, 0, sizeof(MON_EXCEPTION_INFO));	
		time(&MonTime);
	}

	friend bool operator == (const MON_EXCEPTION_INFO& x ,const MON_EXCEPTION_INFO& y)
    {
		if (x.ExceptionType == y.ExceptionType)
		{			
			switch(x.ExceptionType)
			{
			case MON_EXCEPTION_USER_FEE:
				{
					return 0==memcmp(&x.UserFeeInfo, &y.UserFeeInfo, sizeof(EXCEPTION_USER_FEE));
				}
				break;
			case MON_EXCEPTION_FWD_ERR:
				{
					return 0==memcmp(&x.FwdErrInfo, &y.FwdErrInfo, sizeof(EXCEPTION_FWD_ERR));
				}
				break;
			case MON_EXCEPTION_USER_OFFLINE:
				{
					return 0==memcmp(&x.UserOfflineInfo, &y.UserOfflineInfo, sizeof(EXCEPTION_USER_OFFLINE));
				}
				break;
			case MON_EXCEPTION_FWD_OFFLINE:
				{
					return 0==memcmp(&x.FwdOfflineInfo, &y.FwdOfflineInfo, sizeof(EXCEPTION_FWD_OFFLINE));
				}
				break;
			default:
				break;
			}
		}
		return false;
    }
};
*/

//ͻ���쳣�������
enum MON_EXCEPTION_TYPE
{
	MON_EXCEPTION_UNKNOWN,
	MON_EXCEPTION_USERFEE_INDEBT,	//�û�Ƿ��
	MON_EXCEPTION_USERFEE_THRESHOLD,	//���ڷ�ֵ
	MON_EXCEPTION_FWD_ERR,//��Ӫ�̶���
	MON_EXCEPTION_USER_OFFLINE,//�û�����
	MON_EXCEPTION_FWD_OFFLINE,//SPGATE����
};
struct EXCEPTION_USER_FEE
{
	bool IsFeeLower;
	int UID;	
};
struct EXCEPTION_USER_OFFLINE
{
	bool IsOnline;
	int UID;
	int nLinkNum;
	char szIPAndPort[32];
};
struct EXCEPTION_FWD_OFFLINE
{
	bool IsOnline;
	int  UID;
	int  nLinkNum;
	char szIPAndPort[32];	
};
struct EXCEPTION_FWD_ERR
{
	int UID;	
	unsigned char ErrCode;
};
//ͻ���쳣�����������
struct  MON_EXCEPTION_INFO
{
	MON_EXCEPTION_TYPE ExceptionType;            
	time_t MonTime;
    union 
	{
		EXCEPTION_USER_FEE UserFeeInfo; 
		EXCEPTION_FWD_ERR FwdErrInfo;
		EXCEPTION_USER_OFFLINE UserOfflineInfo;
		EXCEPTION_FWD_OFFLINE FwdOfflineInfo;
	};
	MON_EXCEPTION_INFO()
	{
		memset(this, 0, sizeof(MON_EXCEPTION_INFO));	
		time(&MonTime);
	}

	friend bool operator == (const MON_EXCEPTION_INFO& x ,const MON_EXCEPTION_INFO& y)
    {
		if (x.ExceptionType == y.ExceptionType)
		{			
			switch(x.ExceptionType)
			{
			case MON_EXCEPTION_USERFEE_INDEBT:
				{
					return 0==memcmp(&x.UserFeeInfo, &y.UserFeeInfo, sizeof(EXCEPTION_USER_FEE));
				}
				break;
			case MON_EXCEPTION_FWD_ERR:
				{
					return 0==memcmp(&x.FwdErrInfo, &y.FwdErrInfo, sizeof(EXCEPTION_FWD_ERR));
				}
				break;
			case MON_EXCEPTION_USER_OFFLINE:
				{
					return 0==memcmp(&x.UserOfflineInfo, &y.UserOfflineInfo, sizeof(EXCEPTION_USER_OFFLINE));
				}
				break;
			case MON_EXCEPTION_FWD_OFFLINE:
				{
					return 0==memcmp(&x.FwdOfflineInfo, &y.FwdOfflineInfo, sizeof(EXCEPTION_FWD_OFFLINE));
				}
				break;
			default:
				break;
			}
		}
		return false;
    }
};

struct MON_USER_FEE_INFO
{
	int nUID;	 //���ݿ��û����
	int nFee;	 //�������
	int nGlbFee; //�������,������
	double dGlbFee;    //�������,�����
	int  nMonType;     //��������,10/19
	char nCalcFeeType; //�Ʒ����� 1:��������2:�����
	char nFeeType;	   //���ñ������ͣ�1�����ڣ�2�����⣬3������
	char szUsrId[MAX_SPID_LEN+1]; //�����õĻ���ID������910000
	char szInnerThreshold[64];    //�������澯��ֵ
	char szGlobahreshold[64];     //�������澯��ֵ


	MON_USER_FEE_INFO ()
	{
		memset(szUsrId, 0, sizeof(szUsrId));
		nFee = 0;
		nGlbFee = 0;
		dGlbFee = 0;
		nUID = 0;
		nMonType = 10;
		nFeeType = 0;
		nCalcFeeType = 1;
	}
};

typedef std::map<int, MON_USER_FEE_INFO> MUID_MON_USER_FEE_INFO;

#define FUNTYPE_LEN			 30
#define FUNMAE_LEN			 20
#define	CFUNMAE_LEN          50
#define CARGNAME_LEN		 50
#define MARGNAME_LEN		 50
#define CARGVALUE_LEN		 1024//64
#define BELONG_LEN			 50
#define POST_VERSION_PATH "POST /sms/v2/"
#define GET_VERSION_PATH "GET /sms/v2/"
#define POST_VERSION_PATH_LEN 13
#define GET_VERSION_PATH_LEN 12
#define ALL_SUCCESS_RSP	2           //ȫ�ɹ�
#define ALL_FAIL_RSP 3              //ȫʧ��
#define SUCCESS_FAIL_RSP 4          //���ֳɹ�������ʧ��
#define DETAILS_RSP 5				//����
#define DEFAULT_MORPT_RETSIZE1 100
#define DEFAULT_MO_MAX_RETSIZE 200
#define DEFAULT_RPT_MAX_RETSIZE 500
#define MAX_PUSHMO_CNT		200
#define MAX_PUSHRPT_CNT		500
#define MAX_CPUSHMO_CNT	    200       //�û��Զ����������MO����
#define MAX_CPUSHRPT_CNT	500       //�û��Զ����������RPT����

#define WBS_GBK 1
#define WBS_URLENCODE 2
#define WBS_BASE64 3
#define WBS_UTF8 4
#define WBS_URLENCODE_X2 5
#define WBS_UTF8_ONLY 6
#define WBS_UNICODE 7
#define WBS_UTF8_UTF16 8
#define WBS_GBK_HEX 9
#define WBS_UNICODE_HEX 10

#define WBS_CARGTYPE_OTHER 8
#define WBS_CARGTYPE_UPPER_STRING 64

enum
{
	WBS_REQ = 1,
	WBS_RSP
};
enum
{
	WBS_MO = 1,
	WBS_RPT
};
struct WBS_TMPGROUP
{
	int	 nEcid;                     //��ҵID               
	char szFunType[FUNTYPE_LEN + 1];         //��������
	char szFunName[FUNMAE_LEN + 1];         //������
	char szCFunName[CFUNMAE_LEN + 1];         //������
	TINYINT  nCmdType;                  //��������
	TINYINT  nRetMode;                  //����ģʽ 0δ֪��1xml��2json
	char szMArgName[MARGNAME_LEN + 1];        //�����ֶ���
	TINYINT  nPushFlag;

	WBS_TMPGROUP()
	{
		memset(this, 0, sizeof(WBS_TMPGROUP));
	}

	WBS_TMPGROUP& operator=(const WBS_TMPGROUP& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(WBS_TMPGROUP));
		memcpy(this, &other, sizeof(WBS_TMPGROUP));
		return *this;
	}
};

struct WBS_TEMPLATE_INFO
{
	char szCArgName[CARGNAME_LEN + 1];		//�ͻ��ֶ���
	char szBelong[BELONG_LEN + 1];			//�ڵ��ϵ
	TINYINT  nBelongtype;					//�ڵ�����
	TINYINT  nCargType;                     //�ͻ���������

	WBS_TEMPLATE_INFO()
	{
		memset(this, 0, sizeof(WBS_TEMPLATE_INFO));
	}

	WBS_TEMPLATE_INFO& operator=(const WBS_TEMPLATE_INFO& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(WBS_TEMPLATE_INFO));
		memcpy(this, &other, sizeof(WBS_TEMPLATE_INFO));
		return *this;
	}
};
typedef std::map<std::string, WBS_TEMPLATE_INFO> FILEDMAP;  //�����ֶ���ͻ��ֶ�ӳ��ṹ

//�̶��ֶ�ֵ�ṹ
struct FIX_FILED
{
	char szCArgValue[CARGVALUE_LEN+1];	 //�ͻ��̶��ֶ�ֵ
	char szCArgName[CARGNAME_LEN+1];					//�ͻ��ֶ���
	char szBelong[BELONG_LEN + 1];
	TINYINT nBelongtype;    //�ڵ�����, �����ӽڵ�

	FIX_FILED()
	{
		memset(this, 0, sizeof(FIX_FILED));
	}

	FIX_FILED& operator=(const FIX_FILED& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(FIX_FILED));
		memcpy(this, &other, sizeof(FIX_FILED));
		return *this;
	}
};

//�ֶνṹ
struct ALLFILED
{
	FILEDMAP filedmap;
	std::vector<FIX_FILED> vecFiled;
};

typedef std::map<int/*����ģʽ*/, ALLFILED> WBSTEMPLATEINFO; //����ģʽ�µ�ӳ��ṹ
struct WBSTEMPLATE  //ģ��ṹ
{
	TINYINT retMode;           //����ģʽ 0δ֪��1xml��2json
	WBSTEMPLATEINFO tmpInfo;

	WBSTEMPLATE()
	{
		retMode = JSON;
	}

	WBSTEMPLATE& operator=(const WBSTEMPLATE& other)
	{
		if (this == &other)
		{
			return *this;
		}

		this->retMode = other.retMode;
		this->tmpInfo = other.tmpInfo;
		return *this;
	}
};

typedef std::map<std::string/*����·��*/, WBSTEMPLATE/*ģ��*/> PATHTMPLINKMAP;

typedef std::map<int/*ECID*/, WBSTEMPLATE/*ģ��*/> PUSHTEMPLATE;

//����������
typedef  std::vector<std::string/*������������*/>  M_BASE_ARG_PARAMS;
typedef std::map <std::string/*�ӿ�single_send*/, M_BASE_ARG_PARAMS> M_BASE_ARG;

//�ͻ�������������������ӳ���
typedef std::map <std::string/*FUNTYPE/CFUNNAME*/, std::string/*FUNTYPE/FUNNAME*/> CM_NAME;

struct RSP_PUSH_PACK
{
	char szCArgName[CARGNAME_LEN+1];					//�ͻ��ֶ���
	char szCArgValue[CARGVALUE_LEN+1];	                //�ͻ������ֶ�ֵ

	RSP_PUSH_PACK()
	{
		memset(this, 0, sizeof(RSP_PUSH_PACK));
	}

	RSP_PUSH_PACK& operator=(const RSP_PUSH_PACK& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(RSP_PUSH_PACK));
		memcpy(this, &other, sizeof(RSP_PUSH_PACK));
		return *this;
	}
};

typedef std::map<TINYINT/*��Ӧ״̬2345*/, RSP_PUSH_PACK> RSP_CMD_PUSH_PACK;

struct RSP_CMD_PUSH_TMP  //ģ��ṹ
{
	TINYINT retMode;           //����ģʽ 0δ֪��1xml��2json
	RSP_CMD_PUSH_PACK cmdRspPack;

	RSP_CMD_PUSH_TMP()
	{
		retMode = JSON;
	}

	RSP_CMD_PUSH_TMP& operator=(const RSP_CMD_PUSH_TMP& other)
	{
		if (this == &other)
		{
			return *this;
		}

		this->retMode = other.retMode;
		this->cmdRspPack = other.cmdRspPack;
		return *this;
	}
};
typedef std::map<int/*ECID*/, RSP_CMD_PUSH_TMP> WBS_TPL_PUSH_RSP;

struct WBS_ERRCODE
{
	char szECErrcode[17];
	char szECDesc[65];

	WBS_ERRCODE()
	{
		memset(this, 0, sizeof(WBS_ERRCODE));
	}

	WBS_ERRCODE& operator=(const WBS_ERRCODE& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(WBS_ERRCODE));
		memcpy(this, &other, sizeof(WBS_ERRCODE));
		return *this;
	}
};
typedef std::map<std::string, WBS_ERRCODE> WBS_RPTERRCODEMAP;
typedef std::map<int/*ECID*/, WBS_RPTERRCODEMAP> WBS_TPL_RPTEM;

/*
typedef std::pair<int, int> def_value_type;
struct DEF_COMPARE
{	
	bool operator () (const def_value_type & ls, const def_value_type &rs)	
	{	
		return ls.first < rs.first || (ls.first == rs.first && ls.second < rs.second);
	}
	friend bool operator < (const def_value_type & ls, const def_value_type &rs)	
	{	
		return ls.first < rs.first || (ls.first == rs.first && ls.second < rs.second);
	}
};
*/
//mos�ṹ��
typedef struct mosbody
{
	INT64 nMsgId;
	char szMobile[22 + 1];
	char szSpNo[21 + 1];
	char szExNo[22 + 1];
	char szRetTime[20];
	char szContent[MAXLEN];

}MOSBODY_DATA;

const int nPushRPTBody1Len = 11 + 32 + 14 + 10 + 10;

//rpts�ṹ��
typedef struct rptsbody
{
	INT64 nMsgId;
	char szCustId[64 + 1];				  //��Ӧ��������ʱ��д��custid
	int nPkNum;							  //��ǰ����
	int nPkTotal;						  //������
	char szMobile[22 + 1];
	char szSpNo[21 + 1];				  //������ͨ����
	char szExNo[22 + 1];				  //����ʱ��д��exno
	char szRetTime[20];                   //״̬���淵��ʱ��   YYYY-MM-DD HH:MM:SS
	int  nMtStat;						  //����״̬�� 0:�ɹ�  ��0:ʧ��
	char MtErrCode[7 + 1];				  //״̬����������, eg��DELIVRD
	char ExData[64 + 1];				  //����ʱ��д��exdata

}RPTSBODY_DATA;


#define MAX_SENDMT_CNT	    1000

#define FILED_MT_CNT    6    //MT�ֶθ���

#define FILED_VER_LEN   25   //�ֶ���ƽ������

#define FILED_MO_CNT    6    //MOS�ֶθ���

#define FILED_RPT_CNT   12   //RPTS�ֶθ���

#define MAX_HTTP_HEAD   1024

#define MAX_PUSHRPT_LEN  (MAX_CPUSHRPT_CNT * (sizeof(RPTSBODY_DATA)+FILED_VER_LEN*FILED_RPT_CNT) + MAX_HTTP_HEAD)

#define MAX_PUSHMO_LEN  (MAX_CPUSHMO_CNT * (sizeof(MOSBODY_DATA)+FILED_VER_LEN*FILED_MO_CNT) + MAX_HTTP_HEAD)

#define MAX_MUTI_LEN   (MAX_SENDMT_CNT * (sizeof(MULTIXMSGV5) + FILED_VER_LEN*FILED_MT_CNT))

#define  MAX_PUSH_PRE_LEN     1024       //����mo��rptǰ�벿�ֵ���󳤶�

enum CONFIG_TYPE_USERID
{
    GOME_USERID = 1, //���������˺�

    OTHER_USERID = 255
};



struct PT_INFO
{
	char szPtCode[10 + 1];	//��ǰƽ̨��ƽ̨���
	int  nPtCode;			//��ǰƽ̨��ƽ̨���
	std::map<int, std::string> AllPt;//����ƽ̨��ƽ̨��Ŷ�Ӧ��ϵ
	DerivedLock m_cs;

	std::string GetPtCode(int n)
	{
		SafeLock safelock(&m_cs);

		std::map<int, std::string>::iterator it = AllPt.find(n);
		if (it != AllPt.end())
		{
			return it->second;
		}
		else
		{
			return std::string(this->szPtCode);
		}
	}

	void UpdateAllPt(std::map<int, std::string>& x)
	{
		SafeLock safelock(&m_cs);
		AllPt.clear();
		AllPt = x;
	}

	PT_INFO()
	{
		memset(szPtCode, 0, sizeof(szPtCode));
		nPtCode = 0;
	}

	PT_INFO& operator=(const PT_INFO& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(szPtCode, 0, sizeof(szPtCode));
		memcpy(szPtCode, other.szPtCode, sizeof(szPtCode));
		nPtCode = other.nPtCode;
		AllPt = other.AllPt;
		return *this;
	}
};


// add by dengyu 2017-4-28

//////////////////////////////////////////////////////////////////////////
//Я��ת����ؽṹ
//////////////////////////////////////////////////////////////////////////

struct MNP_INFO
{
	INT64 nPhone;
	TINYINT nSrcType;
	TINYINT nPhoneType;
	TINYINT nOpType;

	MNP_INFO()
    {
        memset(this, 0, sizeof(MNP_INFO));
    }
	
    MNP_INFO& operator=(const MNP_INFO& other)
    {
        if (this == &other)
        {
            return *this;
        }
        memset(this, 0, sizeof(MNP_INFO));
        memcpy(this, &other, sizeof(MNP_INFO));
        return *this;
    }
};


//////////////////////////////////////////////////////////////////////////
//�����ֻ���·����ؽṹ
//////////////////////////////////////////////////////////////////////////
struct SPEC_PHONE 
{
	INT64 nPhone;
	int nCustomId;
	unsigned char ucType;
	char szUserId[MAX_SPID_LEN+1];
	
	SPEC_PHONE()
    {
        memset(this, 0, sizeof(SPEC_PHONE));
    }
	
    SPEC_PHONE& operator=(const SPEC_PHONE& other)
    {
        if (this == &other)
        {
            return *this;
        }
        memset(this, 0, sizeof(SPEC_PHONE));
        memcpy(this, &other, sizeof(SPEC_PHONE));
        return *this;
    }
};

//�����ֻ����������
struct SPEC_PHONE_ATTR 
{
	int nCustomId;
	unsigned char ucType;
	char szUserId[MAX_SPID_LEN+1];
	
	SPEC_PHONE_ATTR()
    {
        memset(this, 0, sizeof(SPEC_PHONE_ATTR));
    }
	
    SPEC_PHONE_ATTR& operator=(const SPEC_PHONE_ATTR& other)
    {
        if (this == &other)
        {
            return *this;
        }
        memset(this, 0, sizeof(SPEC_PHONE_ATTR));
        memcpy(this, &other, sizeof(SPEC_PHONE_ATTR));
        return *this;
    }
};

// end add by dengyu

struct WBS_SENDTMPL
{
	int		nEcId;				//��ҵID
	TINYINT nProType;			//��Ʒ���� 1����ģ�壬6����  11����
	int		nParamsNum;			//��������
	char	szTmplContent[MAX_TMPLCONTENT_LEN+1];	//ģ������
	TINYINT TmplStatus;         //zhubo 2018-02-05  0����  ��0����
	TINYINT nTmplVer;
	
	WBS_SENDTMPL()
	{
		memset(this, 0, sizeof(WBS_SENDTMPL));
	}

	WBS_SENDTMPL& operator=(const WBS_SENDTMPL& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(WBS_SENDTMPL));
		memcpy(this, &other, sizeof(WBS_SENDTMPL));
		return *this;
	}
};
typedef std::map<INT64/*ģ��ID*/, WBS_SENDTMPL> WBS_SENDTMPL_ID;


//*****************zhubo 2017-5.24


//��Ӫ��<--->BIND_INFO_VECTOR
typedef std::map<int, BIND_INFO_VECTOR> MAPMOBILE_GATE_GROUP;

//UID---MAPGGUID_MOBILE
typedef std::map<int, MAPMOBILE_GATE_GROUP> MAPGGUID_MOBILE;

//ͨ����ID<--->MAPGGUID_MOBILE
typedef std::map<int, MAPGGUID_MOBILE> MAPGATE_GROUP;

//UID<--->ͨ����ID
typedef std::map<int, int> MAPUID_GATEGROUPID;


//��Ӫ��<--->V_SPGATE_INFO
typedef std::map<int, V_SPGATE_INFO> MOBILE_SPGATE_MAP;

//UID<--->MOBILE_SPGATE_MAP
typedef std::map<int, MOBILE_SPGATE_MAP> UID_MOBILE_SPGATE_MAP;

//ͨ����ID<--->MOBILE_SPGATE_MAP
typedef std::map<int, UID_MOBILE_SPGATE_MAP> GATEID_VSPGATE_MAP;

//*****************zhubo 2017-5.24

#ifdef DBPARAMS
#undef DBPARAMS
#else
#define DBPARAMS DB_PARAMS
#endif
struct DBPARAMS
{
    std::string strHost;                //���ݿ��ַ
    std::string strUser;                //���ݿ��û���
    std::string strPwd;                 //���ݿ�����
    std::string strDBName;              //���ݿ�����
    std::string strConStr;				//���ݿ������ַ���
    int             nPort;              //���ݿ�˿�
    int             nDBType;            //���ݿ�����
    int         nDBMinSize;             //���ݿ����ӳ���С������
    int         nDBMaxSize;             //���ݿ����ӳ����������
    int             CommandTimeout;     //ִ�г�ʱʱ��

    void* pInvoker;
    DBPARAMS()
    {
        strHost = "";
        strUser = "";
        strPwd = "";
        strDBName = "";
        strConStr = "";
        nPort = 1433;
        nDBType = 0;
        CommandTimeout = 60;
        pInvoker = NULL;
    }
    DBPARAMS& operator=(const DBPARAMS& other)
    {
        if (this != &other)
        {
            strHost = other.strHost;
            strUser = other.strUser;
            strPwd = other.strPwd;
            strDBName = other.strDBName;
            strConStr = other.strConStr;
            nPort = other.nPort;
            nDBType = other.nDBType;
            CommandTimeout = other.CommandTimeout;
            pInvoker = other.pInvoker;
        }
        return *this;
    }
};
// end add by dengyu


//zhubo 2018-03-7

#define SURL_CONNECT	 "login"
#define SURL_CONNECT_RESP "loginrsp"

#define SURL_URIREQ						"geturi" //����URI
#define SURL_URIREQRSP	                "geturirsp" //����URIӦ��
#define SURL_ACTIVE_TEST                 "active"   //�������
#define SURL_ACTIVE_TEST_RESP            "activersp" //�������Ӧ��
#define SURL_SET_LONG_ADDR_REQ          "setlastatus" //���ó���ַ״̬
#define SURL_SET_LONG_ADDR_RSP          "setlastatusrsp" //���ó���ַ״̬��Ӧ


#define MINFO_CONNECT	 "pqlogin"
#define MINFO_CONNECT_RESP "pqloginrsp"
#define MINFO_ACTIVE_TEST                 "pqactive"   //�������
#define MINFO_ACTIVE_TEST_RESP            "pqactiversp" //�������Ӧ��
#define MINFO_INFOREQ						"pqgetms" //�����ֻ�������Ϣ
#define MINFO_INFOREQRSP	                "pqgetmsrsp" //�����ֻ�������ϢӦ��

#define MRMS_VERSION_V1                 0x01
#define MRMS_VERSION_V2                 0x02
#define MRMS_VERSION_V3                 0x03


#define MRMS_CONNECT                    0x00000001 //��������
#define MRMS_CONNECT_RESP               0x80000001 //��������Ӧ��
#define MRMS_LOGIN	                    0x00000002 //�����¼
#define MRMS_LOGIN_RESP	                0x80000002 //�����¼Ӧ��
#define MRMS_SUBMIT                     0x00000004 //�ύ����
#define MRMS_SUBMIT_RESP                0x80000004 //�ύ����Ӧ��
#define MRMS_DELIVER                    0x00000005 //�����·�
#define MRMS_DELIVER_RESP               0x80000005 //�·�����Ӧ��
#define MRMS_ACTIVE_TEST                0x00000006 //�������
#define MRMS_ACTIVE_TEST_RESP           0x80000006 //�������Ӧ��
#define MRMS_CMDTOURI					0x00000007 //��uri����ָ��
#define MRMS_CMDTOURI_RESP				0x80000007 //uri����ָ��Ӧ��

struct RMS_LINK_INFO
{
	int             nNodeId;            //�ڵ��
	int				nMobileType;		//��Ӫ��
	int				nArea;				//����
	UINT            nPort;              //�˿ں�
	char            szIP[MAX_IPADDR_LEN + 1];
	char            szDown[25];			//����IP������
	int				nNetUpWidth;		//�ϴ��������
	int				nNetDwWidth;		//�����������

	RMS_LINK_INFO()
	{
		memset(this, 0, sizeof(RMS_LINK_INFO));
	}
};

struct RMS_NODE_NETWIDTH_INFO
{
	int             nNodeId;            //�ڵ��
	int				nError;				//������
	int				nOnline;			//����״̬
	int				nNetUpWidth;		//�ϴ��������
	int				nNetDwWidth;		//�����������
	time_t			tLastActiveTime;	//������ʱ��

	RMS_NODE_NETWIDTH_INFO()
	{
		memset(this, 0, sizeof(RMS_NODE_NETWIDTH_INFO));
		time(&tLastActiveTime);
	}
};

struct RMS_WIND_INFO
{
	int             nNodeId;            //�ڵ��
	int				nWindowCnt;			//��������

	RMS_WIND_INFO()
	{
		memset(this, 0, sizeof(RMS_WIND_INFO));
	}
};

typedef std::map<int, RMS_LINK_INFO> RMSNODE_MAP_LINKINFO; //�ڵ��Ϊ����
typedef std::map<int, RMS_WIND_INFO> RMSNODE_MAP_WINDINFO; //�ڵ��Ϊ����
typedef std::map<int, RMS_NODE_NETWIDTH_INFO> RMSNODE_MAP_NETWIDINFO; //�ڵ��Ϊ����
typedef std::map<INT64, std::string> MAPRMSPACK_MSGIDSTRING;



//////////////////////////////////////////////////////////////////////////
struct MWRMS_LINKNODE
{
	int nNodeNum;//�ڵ��� ע���ڵ���Ψһ
	int nPort; //�˿�
	char szIp[MAX_IPADDR_LEN + 1]; //ip  ע��MAX_IPADDR_LEN=18  

	MWRMS_LINKNODE()
	{
		memset(this, 0, sizeof(MWRMS_LINKNODE));
	}
};

struct RMS_MWPARAMS
{
	int iClientsPerNode;
	std::vector<MWRMS_LINKNODE> vecLinkNode;

};

//zhubo 2018-03-16
struct SURL_DOMAIN_BIND
{
	int 	    nECID;//��ҵID
	char	    chUserID[MAX_SPID_LEN+1];//�û�ID
	TINYINT     nURILen;
	TINYINT     nBindStatus;//��״̬
	//int         nDomainID;//����ID
	char        chDomain[256];//����
	INT64       nPrivilege;//Ȩ��
	TINYINT     nDomainStatus;//����״̬
	TINYINT     nURIMaxLen;//URI��󳤶�
	int 	    nValidDay;//��Ч����
	//char        chDes[1024];//��������

	SURL_DOMAIN_BIND()	
	{
		memset(this, 0, sizeof(SURL_DOMAIN_BIND));
	}
};
//typedef std::vector<SURL_DOMAIN_BIND> VEC_DOMAIN_BIND;
typedef std::multimap<int,SURL_DOMAIN_BIND> MAP_DOMAIN_BIND;
typedef std::multimap<int,SURL_DOMAIN_BIND>::iterator MAP_DOMAIN_BIND_ITER;


//zhubo 2018-03-16

struct ExtendParams
{
	TINYINT nRMSVer;
	TINYINT nIsfree;
	int	 nCtrBit;
	time_t	tShowtime;
	char szShowway[16+1];
	char szTitle[40+1];
	int  nRmsLen;
	char szRmsContent[3000+1];

	ExtendParams()
	{
		memset(this, 0, sizeof(ExtendParams));
		nRMSVer = 1;
	}

	ExtendParams& operator=(const ExtendParams& other)
	{
		if (this == &other)
		{
			return *this;
		}
		memset(this, 0, sizeof(ExtendParams));
		memcpy(this, &other, sizeof(ExtendParams));
		return *this;
	}
};


#endif

