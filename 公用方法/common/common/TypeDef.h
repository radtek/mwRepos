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



#ifndef FAR_STDCALL                 //区别windows和linux调用dll时调用方式的不同
#define FAR_STDCALL
#endif

#ifndef FAR_STDCALL                 //区别windows和linux调用dll时调用方式的不同
#define FAR_STDCALL far __stdcall
#endif


#define  LOG_WBSDLL   "ProtoTool.txt"

#define  MAX_MTRSP_LEN        1024*2

#define  MAX_PUSH_PRE_LEN     1024       //推送mo和rpt前半部分的最大长度

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

#define EPSINON  0.000001           //浮点型误差允许范围
#define MONEY_CON 1000000

//MISMG错误码定义
#define BASE_ER_CODE				 300					    //统一错误码起始值
#define ER_MISMG_SUCCESS             0						    //成功
#define ER_MISMG_PACKER              (BASE_ER_CODE+1)           //消息格式错误
#define ER_MISMG_NOTLOGINED          (BASE_ER_CODE+52)          //尚未成功登录
#define ER_MISMG_NOTBINDCHANNEL      (BASE_ER_CODE+53)          //没有绑定通道


//连接在线状态
enum 
{
	ONLINE,			//在线
	OFFLINE,		//离线
	ONLINE_UNKNOWN	//未知
};

//后端通道状态
enum
{
	STATE_OK,		//正常
	ULTRA_FLOW		//超流量
};

//用户登陆状态
enum 
{
	CONNECTED,		//已连接
	LOGINED,		//已登陆
	DISCONNECTED	//未连接
};

//用户类型
enum 
{
	PREUSER,	 //前端用户
	ENDSPGATE,	 //后端SpGate
	TYPE_UNKNOWN //未知类型
};

//
//标志SOCKET的当前状态
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
//完成端口完成的事件
//
typedef enum
{
	IOCP_COMPLETE_READ,
	IOCP_COMPLETE_ACCEPT_READ,
	IOCP_COMPLETE_WRITE,
	IOCP_COMPLETE_ACCEPT
} IOCP_COMPLETE_STATE;

//
//自定义枚举数据类型，用来标识套接字IO动作类型
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

//socket状态
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

//MT状态
enum 
{
	MT_TERMINAL_STATUS,						//0:CMPPGW收到状态报告，并成功发送给前端SpGate
	CMPPGW_MT_TO_SPGATE_FAILED,				//1:CMPPGW收到前端SpGate的Submit包但没有成功发送至后端SpGate
	CMPPGW_MT_TO_SPGATE_SUCCESS,			//2:CMPPGW收到前端SpGate的Submit包并成功发送至后端SpGate
	SPGATE_MT_TO_CHINAMOBILE_SUCCESS,		//3:后端SpGate成功将Submit包投递给移动行业网关
	SPGATE_RECV_RPT_SUCCESS,				//4:收到移动返回的状态报告
	SPGATE_RPT_TO_CMPPGW_SUCCESS,			//5:将状态报告成功发送给CMPPGW
	ERROR_MT,								//6:异常短信（手机号异常/没有绑定通道/欠费等）
	VERIFY_MT,								//7:需要审核的短信(该状态的信息只写库，不发送)
	DISCARDRPT_MT,							//8:该条下行的状态报告被丢弃
	MODIFYRPT_MT,							//9:该条下行的状态报告被强制修改为DELIVRD
	NULL_MT									//10:该条下行的内容为空
};

//MT内部处理标志
enum
{
	FLAG_RECV_MT = 1,						//1:收到用户submit
	FLAG_WRITE_MTDB_SUCCESS,				//2:写库成功，增加发送缓冲，并等待submit_rsp投递结果
	FLAG_MTRSP_SUCCESS,						//3:submit_rsp投递成功，等待转发至后端SpGate结果
	FLAG_MTRSP_FALIED,						//4:submit_rsp投递失败等待写库线程处理
	FLAG_MT_TO_SPGATE_FAILED				//5:投递给后端SpGate失败
};

//MO状态
enum
{
	MO_TERMINAL_STATUS,					//0:CMPPGW收到用户成功接收的回应
	SPGATE_MO_TO_CMPPGW_FAILED,			//1:后端SpGate收到mo(手机上行短信)发送给CMPPGW但未成功
	SPGATE_MO_TO_CMPPGW_SUCCESS,		//2:后端SpGate收到mo(手机上行短信)发送给CMPPGW成功
	CMPPGW_MO_TO_USER_SUCCESS,			//3:CMPPGW收到后端SpGate发送过来的手机mo，并成转发至前端SpGate,若未成功重置为状态1
};

//MO内部处理状态
enum
{
	FLAG_RECV_MO = 1,					//1:收到后端spgate发来的mo
	FLAG_MORSP_SUCCESS,					//2:给后端spgate回应morsp成功
	FLAG_MO_TO_USER_SUCCESS,			//3:发送mo至前端spgate成功
	FLAG_MO_TO_USER_FAILED,				//4:发送mo至前端spgate失败，置数据库状态为1
	FLAG_RECV_MORSP						//5:收到前端spgate发来的成功接收mo回应包
};

//Mo/Rpt标志
enum
{
	FLAG_MO,							//手机MO
	FLAG_REPORT,						//状态报告
	FLAG_MORPT							//mo&rpt
};

//WBS请求命令
enum
{
	WBS_GET_WSDL = 0,
	WBS_MT_SINGLE,//单发
	WBS_MT_EXNO,
	WBS_MT_LONGMSG,//群发
	WBS_MO_GET,
	WBS_RPT_GET,
	WBS_QUERY,
	WBS_QUERY_STATUS,                 //add sp 2012-03-17 查询服务器状态
	WBS_SEND_SUBMIT,                  //add sp 2012-03-28 带用户自定义流水号发送接口
	WBS_SEND_MULTIX,                  //add sp 2012-10-31 用户个性化群发状态
	WBS_FILEUPLOAD_REQ,
	WBS_DELIVER_GETMO,				  //获取mo信息
	WBS_DELIVER_GETRPT,				  //获取rpt信息
	WBS_DELIVER_GETBOTH,			  //同时获取mo和rpt信息
	WBS_DELIVER_GETCHANNEL,			  //获取通道信息spgate
	WBS_DELIVER_GETOTHER,		      //
	WBS_DELIVER_MOBILESEG,          //手机号段
	WBS_HSBC_MT_REQ,
	WBS_HTTP_OK,
	WBS_HTTP_ERR,
	WBS_SEND_TEMPLATE,				//模板发送接口
	WBS_SEND_MIX,					 //zhubo 2017-12-8
	WBS_SHORT_ADDRESS,                           //zhubo 2018-03-04 
	WBS_MOBILE_INFO,
	WBS_SEND_TPL,				//富信模板发送接口
};

//WBS请求回应命令
enum
{
	WBS_MTREQ_RSP=0,
	WBS_MOGET_RSP,
	WBS_RPTGET_RSP,
	WBS_WSDLGET_RSP,//WSDLGET 请求响应
	WBS_QUERY_RSP,
	WBS_FILEUPLOAD_RSP,
	WBS_DELIVERGETMO_RSP,
	WBS_DELIVERGETRPT_RSP,
	WBS_DELIVERGETBOTH_RSP,
	WBS_DELIVERGETCHANNEL_RSP,
	WBS_DELIVERGETOTHER_RSP,
	WBS_HSBC_MTREQ_RSP,
};

//WBS请求模式
enum
{
	WBS_SOAP_POST=0,	//默认为soap_post模式
	WBS_HTTP_POST,
	WBS_HTTP_GET
};

//交行WBS请求/响应命令
enum COMMAND
{
	MT_REQUEST=1,		//下行请求消息
	MT_RESPONSE,		//下行响应消息
	MULTI_MT_REQUEST,	//批量下行请求消息(相同内容)
	MULTI_MT_RESPONSE,	//批量下行响应消息(相同内容)
	MULTIX_MT_REQUEST,	//批量下行请求消息(不同内容)
	MULTIX_MT_RESPONSE,	//批量下行响应消息(不同内容)
	BATCH_MT_REQUEST,	//文件群发下行请求消息
	BATCH_MT_RESPONSE,	//文件群发下行响应消息
	MO_REQUEST,			//上行请求消息
	MO_RESPONSE,		//上行响应消息
	RT_REQUEST,			//状态报告请求消息
	RT_RESPONSE,		//状态报告响应消息
	MULTIXMO_REQUEST,			//上行请求消息
	MULTIXMO_RESPONSE,		//上行响应消息
	MULTIXRPT_REQUEST,			//状态报告请求消息
	MULTIXRPT_RESPONSE,		//状态报告响应消息
	ERROR_RESPONSE		//错误响应消息
};

//费率标志
enum
{
	UNKNOWNPAY=0,
	PREPAY,
	POSTPAY
};

//扣费方式
enum
{
	MSGNUM = 0,
	MSGMONEY = 1,
	MSGNUM_MONEY   = 2 //云通讯专用
};


//计费对象
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
	WBS_V1,  //交行协议
	WBS_OLD,
	WBS_V2,
	WBS_V2_5,//  2.0版本增加一个5.5版本协议，专门做支持UTF-8编码
	WBS_V3
};

typedef enum
{
	HCT_NONE       =  0x00,    //None 默认值
	HCT_JSON_TEXT  =  0x01,   //text/json
	HCT_JSON_APP   =  0x02,   //application/json
	HCT_XML_TEXT   =  0x03,   //text/xml
	HCT_XML_APP    =  0x04	   //application/xml
}HTTP_CONTENT_TYPE;

#ifndef TINYINT
#define TINYINT    unsigned char
#endif

//命令任务表中的类型
enum CMD_TASK_TYPE
{
	CMD_TASK_UNKNOWN = 0,	//未知
	CMD_TASK_SMSTMPL = 1,	//短信模板相关
	CMD_TASK_USRINFO = 6,	//用户信息相关
	CMD_TASK_SPGATEBIND = 7,	//用户通道绑定信息相关
	CMD_TASK_GATEBIND = 8,	//通道信息相关
	CMD_TASK_SPNOSWITCH = 10,	//通道切换相关
	CMD_TASK_MTROUTE = 11,	//通用路由信息
	CMD_TASK_SMSROUTE = 12,	//模板路由信息
	CMD_TASK_SVRROUTE = 13,	//业务类型路由
	CMD_TASK_ADC = 14,			//ADC子号信息
	CMD_TASK_ENDBIND = 15,	//通道帐号绑定信息
	CMD_TASK_FLASHBIND = 16,	//闪信绑定表
	CMD_TASK_GATEGROUP = 17,	//通道组相关信息
	CMD_TASK_FEE = 18	//余额变动指令

};

//命令操作类型
enum CMD_OPTYPE
{
	CMD_OP_UNKNOWN = 0,		//未知
	CMD_OP_ADD = 1,			//增加
	CMD_OP_DEL = 2,			//删除
	CMD_OP_MODIFY = 3,		//修改
	CMD_OP_CLEARALLBY = 4,	//清空指定条件下所有
	CMD_OP_CLEARALL = 5,		//无条件清空指令类型所对应的所有信息
	CMD_OP_RELOADALLBY = 6,	//重新加载指定条件下所有
	CMD_OP_RELOADALL = 7		//无条件重新加载指令类型所对应的所有信息
};


enum PACK_SUBMIT_ENCODE_FLAG
{
	PSEF_GBK    = 0x01,
	PSEF_BASE64 = 0x02
};

//zhubo 2017-5-23
//指令信息
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

		//按ID排序
		if (a.nID == b.nID)
		{
			bLess = false;
		}
		else
		{
			//滤重
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
//协议
#define CMPPV2_0   ((BYTE)0x20)
#define CMPPV3_0   ((BYTE)0x30)
#define EMPPV2_0   ((BYTE)0x40)
#define EMPPSHV2_0 ((BYTE)0x50)
//CMPP命令
#define CMPP_CONNECT                     0x00000001	//请求连接
#define CMPP_CONNECT_RESP                0x80000001	//请求连接应答
#define CMPP_TERMINATE                   0x00000002	//终止连接
#define CMPP_TERMINATE_RESP              0x80000002	//终止连接应答
#define CMPP_SUBMIT                      0x00000004	//提交短信
#define CMPP_SUBMIT_RESP                 0x80000004	//提交短信应答
#define CMPP_DELIVER                     0x00000005	//短信下发
#define CMPP_DELIVER_RESP                0x80000005	//下发短信应答
#define CMPP_QUERY                       0x00000006	//发送短信状态查询
#define CMPP_QUERY_RESP                  0x80000006	//发送短信状态查询应答
#define CMPP_CANCEL                      0x00000007	//删除短信
#define CMPP_CANCEL_RESP                 0x80000007	//删除短信应答
#define CMPP_ACTIVE_TEST                 0x00000008	//激活测试
#define CMPP_ACTIVE_TEST_RESP            0x80000008	//激活测试应答
#define CMPP_FWD                         0x00000009	//消息前转
#define CMPP_FWD_RESP                    0x80000009	//消息前转应答
#define CMPP_MT_ROUTE                    0x00000010	//MT路由请求
#define CMPP_MT_ROUTE_RESP               0x80000010	//MT路由请求应答
#define CMPP_MO_ROUTE                    0x00000011	//MO路由请求
#define CMPP_MO_ROUTE_RESP               0x80000011	//MO路由请求应答
#define CMPP_GET_ROUTE                   0x00000012	//获取路由请求
#define CMPP_GET_ROUTE_RESP              0x80000012	//获取路由请求应答
#define CMPP_MT_ROUTE_UPDATE             0x00000013	//MT路由更新
#define CMPP_MT_ROUTE_UPDATE_RESP        0x80000013	//MT路由更新应答
#define CMPP_MO_ROUTE_UPDATE             0x00000014	//MO路由更新
#define CMPP_MO_ROUTE_UPDATE_RESP        0x80000014	//MO路由更新应答
#define CMPP_PUSH_MT_ROUTE_UPDATE        0x00000015	//MT路由更新
#define CMPP_PUSH_MT_ROUTE_UPDATE_RESP   0x80000015	//MT路由更新应答
#define CMPP_PUSH_MO_ROUTE_UPDATE        0x00000016	//MO路由更新
#define CMPP_PUSH_MO_ROUTE_UPDATE_RESP   0x80000016	//MO路由更新应答
#define CMPP_QUERY_USERFEE				 0x00009901 //帐号费用情况查询
#define CMPP_QUERY_USERFEE_RESP			 0x80009901 //帐号费用情况查询应答
#define CMPP_QUERY_CHANNELINFO			 0x00009902 //帐号通道绑定情况查询
#define CMPP_QUERY_CHANNELINFO_RESP		 0x80009902 //帐号通道绑定情况查询应答

//EMPP命令
#define EMPP_SESSION                     0x000000FF //分配会话id
#define EMPP_CONNECT                     0x00000001 //登录
#define EMPP_CONNECT_RESP                0x80000001 //登录应答
#define EMPP_TERMINATE                   0x00000002 //退出登录
#define EMPP_TERMINATE_RESP              0x80000002 //退出登录应答
#define EMPP_SUBMIT                      0x00000004 //提交短信
#define EMPP_SUBMIT_RESP                 0x80000004 //提交短信应答
#define EMPP_DELIVER                     0x00000005 //递交MO
#define EMPP_DELIVER_RESP                0x80000005 //递交MO应答
#define EMPP_REPORT                      0x00000006 //状态报告
#define EMPP_REPORT_RESP                 0x80000006 //状态报告应答
#define EMPP_ACTIVE_TEST                 0x00000007 //检查链路状态
#define EMPP_ACTIVE_TEST_RESP            0x80000007 //检查链路状态应答
#define EMPP_UPDATE_PSWD                 0x00000008 //修改登录网关密码
#define EMPP_UPDATE_PSWD_RESP            0x80000008 //修改登录网关密码应答
#define EMPPSH_ACTIVE_TEST               0x00000008 //检查链路状态
#define EMPPSH_ACTIVE_TEST_RESP          0x80000008 //检查链路状态应答
//用户属性特权值定义
#define PRIVILEGE_NONE					 0x00000000		//没有特权的普通用户
#define PRIVILEGE_MSGID					 0x00000001		//自编MSGID权限		PRIVILEGE字段第1位
#define PRIVILEGE_AGENT					 0x00000002		//代理帐号权限		PRIVILEGE字段第2位
#define PRIVILEGE_MONITOR				 0x00000004		//监控帐号			PRIVILEGE字段第3位
#define PRIVILEGE_SENDLEVEL				 0x00000008		//自定义发送级别	PRIVILEGE字段第4位
#define PRIVILEGE_NORPTNEED				 0x00000010		//无需推送RPT		PRIVILEGE字段第5位
#define PRIVILEGE_MSG_VERIFY			 0x00000020		//需要信息审核		PRIVILEGE字段第6位
#define PRIVILEGE_KEYWRODS_FILTER		 0x00000040		//需要关键字过滤	PRIVILEGE字段第7位
#define PRIVILEGE_RESEND_CTRL			 0x00000080		//需要重发控制		PRIVILEGE字段第8位
#define PRIVILEGE_MODIFY_RPT			 0x00000100		//需要将一部分失败的状态报告强制改为delivrd		PRIVILEGE字段第9位
#define PRIVILEGE_NOMONEED				 0x00000200		//无需推送Mo		PRIVILEGE字段第10位

//////////////////////////////////////////////////////////////////////////
#define SEND_WINDOES_SIZE                50           //发送窗口大小
#define SLIDING_WINDOES_SIZE             1000         //发送窗口大小
//////////////////////////////////////////////////////////////////////////
//缓冲大小
#define	MAX_RECBUF_LEN				600*1024	//最大接收缓冲(三个单包大小)
#define MAX_WBS_RSPPACK_LEN			300*1024	//wbs回应包的最大长度
#define BUFFER_SIZE_SEND			300*1024	//完成端口一次发送的大小
#define BUFFER_SIZE_RECV			32*1024		//完成端口一次接收的大小
#define	MAX_PACK_LEN				500*1024	//一个单包的最大内存值
#define MAX_PACK_LEN_V5				630*1024    //V5一个单包的最大内存值
#define POOL_BUF_SIZE				100*1024	//内存池单个缓冲大小
#define MAX_HTTP_LEN                2048        //HTTP协议数包最大长度 
#define MAX_USERDATA_LEN            720         //接收用户上行信息长度
//////////////////////////////////////////////////////////////////////////
//字节长度等
#define	MAX_MO_MSG_LEN				360 
#define MAXTHREAD_COUNT				8			//完成端口开启的线程数
#define MAX_THREAD_DELAY			5			//每个线程等待时间
#define MAX_MTLEVEL_NUM				10			//mt分多少个级别(mt0/mt1/mt2..mt9)
#define MAX_MOLEVEL_NUM				2			//mo分多少个级别(mo/rpt)
#define MAX_PHONE_NUM				5000//1000		//最多支持群发的手机个数
#define MAX_HTTP_PHONE_NUM			100	    	//HTTP最多支持群发的手机个数
#define MAX_MULTIX_PHONE_NUM		200			//最多支持个性化群发的手机个数
#define MAX_SMALL_PACK_NUM			30			//小批量群发个数上限
#define MAX_ROW_NUM					1000		//文件发送时每个文件的最大行数
#define MAX_CHANNEL_LEN				21			//通道长度
#define MAX_PHONE_LEN				24			//手机号预留最大长度
#define SIGN_MAX_PHONE_LEN			21			//单个手机号最大长度
//#define MAX_MSG_LEN					360			//系统支持的最大短信字数（汉字）(720字节)
//#define	MAX_SPLIT_MSG_LEN			360
#define	MAX_MO_MSG_LEN				360 
//zhubo 2017-6-12
#define MAX_MSG_LEN					1000			//系统支持的最大短信字数（汉字）(720字节)
#define	MAX_SPLIT_MSG_LEN			1000 


#define MAX_SINGLE_LEN			    70			//单条短信最大长度（汉字）
#define MAX_MULTILEN1				67			//长短信第一条最大长度（汉字）
#define MAX_MULTILEN2				67			//长短信第二条最大长度（汉字）
#define MAX_SIGN_LEN				10			//签名最大字数（汉字）(20字节)
#define MAX_IPADDR_LEN				18			//IP地址最大长度
#define MAX_LOGINIP_LEN				256			//IP地址最大长度
#define MAX_DOMAIN_LEN				256			//IP地址最大长度
#define MAX_PWD_LEN					256//32			//密码的最大长度
#define MAX_MSGIDSTR_LEN			21			//MsgId转化为int64后的字符串长度
#define MAX_TITLE_LEN				100			//发送标题的长度
#define MAX_TIME_LEN				30			//YY-MM-DD HH:MM:SS:SSSS
#define MAX_RSPPACK_LEN				128			//cmpp协议回应包最大长度
#define MAX_PARAMS_LEN				128			//路由表中参数字段最大长度
#define MAX_CHANNELINFO_LEN			64			//通道绑定信息最大长度
#define MAX_WBS_ATTIME_LEN			14			//Wbs中attime长度
#define MAX_WBS_VALIDTIME_LEN		14			//Wbs中validtime长度
#define MAX_WBS_ERCODE_LEN			3			//Wbs中错误码的长度
#define CHANNEL_TYPE_ARR			5			//有多少种通道类型(运营商类型)	
#define CHANNEL_SEQ_ARR				100			//每个用户每种类型的通道可以绑多少个
#define MAX_COPRNAME_LEN			128			//企业名称
#define OFFLINE_DELAY			    5			//离线延时时间
#define MAX_PUSH_WAIT				30			//推送响应超时时间
#define MAX_OFFLINE_TIME			86400		//最长离线时间
#define MAX_BINDIP_NUM			    5			//用户最多可以绑定多少个IP
#define MAX_RISELEVEL				9			//最大提升级别数
#define MAX_URL_LEN					512
#define MAX_CA_NAME                 255

#define MAX_LOG_LEVEL_DEFAULT		0			//默认日志级别4   记录较多的信息
#define MAX_LOG_LEVEL_LOW			1			//低日志级别1     记录最多的日志
#define MAX_LOG_LEVEL_MID			2			//中日志级别2	  不仅记录MAX_LOG_LEVEL_LOW级别,并且记录与接入用户交互的包
#define MAX_LOG_LEVEL_HIGH			3			//高日志级别3	  不仅记录MAX_LOG_LEVEL_LOW级别,并且记录与spgate交互的包
#define MAX_LOG_LEVEL_HIGHER		4			//较高日志级别4   记录之前所有级别的包
#define MAX_LOG_LEVEL_HIGHEST		5			//最高日志级别5	  记录非常少的包

#define PRECISION					10000		//精度
#define MAX_LEN_KEYWORDS			50			//关键字长度

#define MAX_DESTID_LEN				21			//DESTID字段的长度(字节)
#define MAX_SRCID_LEN				21			//SRCID字段的长度(字节)
#define MAX_AUTHENSRC_LEN			16			//源址鉴别字串长度(字节)
#define MAX_MSGID_LEN				8			//MsgId的长度(字节)
#define MAX_SPID_LEN				6			//企业SP代码最大长度(字节)
#define MAX_SRVID_LEN				10			//业务标识长度(字节)
#define MAX_FEETMID20_LEN			21			//Fee_terminal_Id CMPP20长度(字节)
#define MAX_FEETMID30_LEN			32			//Fee_terminal_Id CMPP30长度(字节)
#define MAX_FEETYPE_LEN				2			//FeeType长度(字节)
#define MAX_FEECODE_LEN				6			//FeeCode长度(字节)
#define MAX_ATTIME_LEN				17			//At_time长度(字节)
#define MAX_VALIDTIME_LEN			17			//valid_time长度(字节)
#define MAX_RESERVE_LEN				8			//预留字段长度(字节)
#define MAX_LINKID_LEN				20			//CMPP30中linkid长度(字节)
#define MAX_DESTTMID20_LEN			21			//CMPP20中Dest_terminal_Id单个ID的长度(字节)
#define MAX_DESTTMID30_LEN			32			//CMPP30中Dest_terminal_Id单个ID的长度(字节)

#define MAX_ERSTAT_LEN				7			//状态报告错误码的长度(字节)
#define MAX_SUBMITTM_LEN			10			//Submit_time 长度(字节)
#define MAX_DONETM_LEN				10			//Done_time 长度(字节)
#define MAX_SMSCSEQ_LEN				4			//SMSC_sequence 长度(字节)
#define MAX_SRCTMID20_LEN			21			//CMPP20中Src_terminal_Id单个ID的长度(字节)
#define MAX_SRCTMID30_LEN			32			//CMPP30中Src_terminal_Id单个ID的长度(字节)

#define MAX_COMMAND_LEN             128         //接收到的请求命令长度

#define MAX_PUSH_COUNT              1000000000  //记录的最大推送次数
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
//CMPP错误码定义
#define ER_CMPP_SUCCESS				0			//成功
#define ER_CMPP_PACKER				1			//消息格式错误
#define ER_CMPP_NOTLOGINED			52			//尚未成功登录
#define ER_CMPP_NOTBINDCHANNEL		53			//没有绑定通道
//登陆
#define ER_CMPP_CONNECT_SPIDER		2			//非法源址
#define ER_CMPP_CONNECT_AUTHENER	3			//认证错误
#define ER_CMPP_CONNECT_VERSIONER	4			//版本太高
#define ER_CMPP_CONNECT_DBER		5			//内部数据库错误
#define ER_CMPP_CONNECT_LOGINTOOFAST 6			//前后两次登陆间隔太短
#define ER_CMPP_CONNECT_MUCHLINK	56			//连接超限
#define ER_CMPP_CONNECT_ERIP		57			//SP登陆IP错
#define ER_WBS_CONNECT_ERIP		    -10057		//SP登陆IP错
#define ER_WBS_CONNECT_OVERFLOW	    -10056		//SP登陆连接数超限

#define ER_WBS_OVERFEE                  -100003     //欠费
#define ER_WBS_CUSTID_EXDATA            -100004     //custid或者exdata不合法
#define ER_WBS_OVERMSG                  -100011     //短信内容超长
#define ER_WBS_MOBILE                   -100012     //手机号码不合法
#define ER_WBS_OVERMOBILE               -100014     //手机号码超过10000
#define ER_WBS_SPBIND                   -100029     //端口绑定失败
#define ER_WBS_CONNECT_ERIP_V5		    -100057		//SP登陆IP错
#define ER_WBS_CONNECT_OVERFLOW_V5	    -100056		//SP登陆连接数超限
#define ER_WBS_SYSTEM                   -100999     //系统内部错误
#define ER_WBS_NO_TEMPLATE              -100058     //找不到模板
#define ER_WBS_TMPL_ARGCNT              -100059     //模板参数个数不匹配
#define ER_WBS_MOBILE_CONTENT_COUNT		-100060 	//手机号与信息内容个数不一致
#define ER_WBS_SVRTYPE                  -100252     //svrtype不合法

#define ER_WBS_RMS_SEND_ERROR            -100500     //富信接口发非富信
#define ER_WBS_RMS_CONTENT_OVERFLOW      -100501     //富信接口内容过长
#define ER_WBS_TMPL_DISABLE				 -100502     //短信模板被禁用

#define ER_WBS_SHORT_URL_FAILED				 -100800     //短地址相关接口失败
#define ER_WBS_SHORT_URL_LONG_ADDR_ERROR     -100801     //长地址错误
//长地址被禁用在短址中心做  错误代码为      -100802
#define ER_WBS_SHORT_URL_DOMAIN_ERROR        -100803     //短域名错误
#define ER_WBS_SHORT_URL_DOMAIN_DISABLED     -100804     //短域名已经被禁用
#define ER_WBS_SHORT_URL_MOBILE_TOO_LONG     -100805     //手机号码超过 10000个
//短地址中心资源够不够  代码在短地址中心
//#define ER_WBS_SHORT_URL_MOBILE_TOO_LONG     -100806    //短地址中心资源不够
#define ER_WBS_SHORT_URL_SERVER_BUZY     	  -100807     //服务器忙

#define ER_WBS_MOBILE_INFO_MOBILE_COUNT     -100900     //手机号码个数错误
#define ER_WBS_MOBILE_INFO_FAILED           -100901     //获取手机号码信息失败
#define ER_WBS_CHARGE_MINFO_FAILED          -100902     //扣费请求失败

#define ER_WBS_MOBILE_INFO_SERVER_BUZY     	-100907     //服务器忙





//submit
#define ER_CMPP_SUBMIT_CMDER		2			//命令字错误
#define ER_CMPP_SUBMIT_MSGOVERLONG	6			//短信超长
#define ER_CMPP_SUBMIT_SRCIDER		9			//通道号绑定错误
#define ER_CMPP_SUBMIT_DESTIDER		13			//手机号码非法
#define ER_CMPP_SUBMIT_SENDMIXEDER	14			//不允许混合发送
#define ER_CMPP_SUBMIT_PKNUMER		21			//PKNUMBER/PKTOTAL不符
#define ER_CMPP_SUBMIT_FEEINFOER	206			//移动用户帐户数据异常
#define ER_CMPP_SUBMIT_OWEFEEER		208			//用户余额不足
#define ER_CMPP_SUBMIT_OWEFEEMOREER	210			//超过最高欠费额
#define ER_CMPP_SUBMIT_GENERALERROR	255			//SUBMIT错误概括,具体错误码详查平台
#define ER_CMPP_SUBMIT_ILLEGALMSG	42			//信息内容不安全
#define ER_CMPP_SUBMIT_DUPLICATEMSG	43			//重复对同一手机递交相同短信的频率过高
#define ER_CMPP_SUBMIT_NULLMT		44			//下行内容为空
//FWD
#define ER_CMPP_FWD_CMDER			2			//命令字错
#define ER_CMPP_FWD_OVERFLOWER		8			//超流量
#define ER_CMPP_FWD_FEEINFOER		206			//移动用户帐户数据异常
#define ER_CMPP_FWD_OWEFEEER		208			//用户余额不足
#define ER_CMPP_FWD_OWEFEEMOREER	210			//超过最高欠费额
#define ER_CMPP_FWD_OTHERER_9		9			//其他错误9
#define ER_CMPP_FWD_OTHERER_169		169			//其他错误169
#define ER_CMPP_FWD_OTHERER_10		10			//其他错误10
#define ER_CMPP_FWD_OTHERER_69		69			//其他错误69
#define ER_CMPP_FWD_OTHERER_13		13			//其他错误13
#define ER_CMPP_FWD_OTHERER_6		6			//其他错误6

#define RPT_FILE_NUM                80          //每个RPT文件存入RPT的条数
#define MO_FILE_NUM                 25          //每个MO文件存入MO的条数
#define RPT_HIS_OVERTIME            3           //RptPackHis超接收时间70秒，无条件写入文件,因为超时60秒从一级队列挤入历史队列

//////////////////////////////////////////////////////////////////////////
//配置参数的最大值、最小值定义
//////////////////////////////////////////////////////////////////////////
#define MW_PARAM_MAX_MONFREQ        3600        //监控频率最大值
#define MW_PARAM_MIN_MONFREQ        3           //监控频率最小值
#define MW_PARAM_DEF_MONFREQ        10			//监控频率默认值

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//MT文件中存放的数据结构最大值
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

//滞留类型
enum REMAIN_TYPE
{
	REMAINT_MEM = 0,		//内存
	REMAINT_FILE,			//文件
	REMAINT_DT_REMAIN,		//mtlvl/motask/rptwaitb表
	REMAINT_DT_VFY			//mtvfy表
};

//外连结构
struct LOGINUSER_INFO 
{
	int  nSockId;		//登陆用户的socketid
	UINT nSocket;		//连接socket
	int  nUID;			//登陆帐号UID
	int  nPort;			//目标端口
	char szAddr[18+1];		//目标IP
	char szUserId[6+1];
	char szLoginId[6+1];	//登陆Id
	char szPwd[16+1];		//登陆密码
	char szSpGate[21+1];	//通道号
	time_t tLastActive; //最后一个数据包发送的时间
	int nStatus;		//当前状态为离线还是连线还是未知
	int nVer;			//协议版本号0x20,0x30
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

//长短信拆分规则
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
	unsigned int PkLen;                //包长度 
	unsigned int CmdId;                //命令ID
	unsigned int SeqId;                //序号
	unsigned int SrcSeqId;			   //来源的seqid
	unsigned int Src;                  //信息来源（1-API，2-内部）
	unsigned int Status;               //处理标志(0初始, 1发送中, 2发送成功, 3发送失败)
	unsigned int ReCnt;                //重发次数
	unsigned int nUID;				   //用户UID
	HANDLE hEvent;                     //事件
	unsigned int LastTime;             //过期时间
	unsigned char szSpGate[21+1];
	unsigned char Data[MAX_PACK_LEN+1];  //结点的数据域
	
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

typedef struct TG_NODE                 //结点类型定义
{
	ListData  data;                    //结点的数据域
	TG_NODE  *next;                    //结点的指针域
} ListNode;
typedef ListNode *LPNOTE;

typedef struct TG_LIST                //链表定义
{
	ListNode *head;                   //数据头节点指针
	ListNode *tail;                   //数据尾节点指针
	ListNode *rhead;                  //回收链头节点指针
	ListNode *rtail;                  //回收链尾节点指针
	unsigned int rlen;                //回收链的长度
	unsigned int length;              //数据链表长度
	CRITICAL_SECTION cs;              //链表锁
} LIST;
typedef LIST *LPLIST;

//////////////////////////////////////////////////////////////////////////
//WBS错误码定义
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
//单IO数据，扩展的WSAOVERLAPPED
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

#define IOCP_RECVBUFPOOL_SIZE		1000		//申请IOCP接收内存池的大小

//完成端口接收缓冲区
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
//自定义结构，即“完成键”(单句柄数据)
//
struct IOCPKEY 
{
	SOCKET			socket;
	int				socketid;
	short			m_nUsrType;
	int  			nUserPrivilege;     //自编流水号权限 0-无，非0-有
	int				m_nUID;
	time_t			tLastActive;
	USHORT			m_nVer;
	UINT			m_nRemain;
	SOCKET_STATE	state;
	UINT			nPort;
	UCHAR			cLogin;				//是否已登陆标志
	UCHAR			m_szSrcAddr[MAX_SPID_LEN+1];
	char			m_szSpGate[MAX_CHANNEL_LEN+1];
	char			m_szIP[MAX_IPADDR_LEN+1];
	char			m_szTotalRecv[MAX_RECBUF_LEN+1];
	int				nSeqId;			//流水号
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

//回调函数
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
    int		m_nUnlinkTime;; //最大空连接超时时间(s) UnlinkTime = 60
	int		m_nDBCMDDelay;
	int		m_nWaitRspDelay;
	int		m_nPreMaxWnd;
	int		m_nEndMaxWnd;
	int		m_nSyncFeeInterval;
    int		m_nSyncSpGateInterval;//;同步端口绑定表时间间隔(m)  定时线程需要设置的时间间隔 SyncSpGateInterval = 5
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
    int		m_nRecycleRptInterval;//RPT循环间隔 MaxRptMoRemained ;状态报告和上行存留内存中的时间(超过该值将被写入数据库)
	int		m_nWbsThrCnt;
    int		m_nWbsSendBuf;//; 发送缓冲上限 MaxSendBuf = 5000
    int		m_nWbsSlideWnd;//; 主动推送滑动窗口  就是一个客户，同时推送的并发数。 SlideWnd = 5
	int     m_nWbsSendToPtSildeBuf;
    int		m_nWbsReSendCnt;//        ;MO重发次数  MoReSendCnt =3
	int		m_nWbsSendSpeed;
	int		m_nWbsPackNum;
	int		m_nDllCallType;
	int		m_nMoReSndCnt;
	int		m_nDelaySend;
	UINT	m_nReSendTimeOut;//; 重复频率间隔控制(秒)  ReSendTimeOut = 30
    UINT	m_nLogLevel;////日志级别配制值 ;日志级别配制值：0-5 。上线程序默认级别 LOG_ERROR=4 。
	DATE	m_dtVerifyBegin;
	DATE	m_dtVerifyEnd;
	BOOL	m_bStopCharge;
	BOOL	m_bWbsDiscardRpt;
	BOOL	m_bJavaUtf8;
    BOOL	m_bDiscardRpt;      //; 是否丢弃RPT(0 / 1)DiscardRpt = 0
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

	BOOL	m_bStopPush;//;StopPush 1为停止推送，0为需要推送。
    char    m_szWbsLocalName[MAX_PATH];//共享文件路径
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

	int     m_nSacnfWaitTime; //读取滞留信息时间间隔 //默认10s
	int     m_nDealMtThrCnt;
	int     m_nDealUserCnt;
	int     m_nDealUserRspCnt;
	int     m_nDealPTMtThrCnt;

	BOOL    m_bStopMtRead;
	BOOL    m_bNoKeepAlive;

	int     m_nVersion; //wbs协议版本

	int     m_nMasterOrSlave;
	//zhubo 2017-5-25  主wbs处理mt滞留 工作进程索引 以1开始
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

	int     m_nSUrlTimeOut;// 与短址中心通信的超时时间
	BOOL	m_bConnectToSUrlCenter;//是否需要连上短地址中心
	int		m_nSUrlMaxWinSize;//wbs与短地址中心通信的最大窗口大小
	int		m_nSUrlClientThreadsNum;
	int 	m_nSUrlClientCount;
	BOOL	m_bConnectToMInfoSvr;//是否需要连上号码查询服务
	int		m_nMInfoMaxWinSize;//wbs与号码查询服务通信的最大窗口大小
	int     m_nMInfoTimeOut;// 扣费与网关通信的超时时间
	int 	m_nLoadAllPtFee;        //是否加载所有平台的计费信息

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
	int				nSockId;			//为每个socket编号
	short			nSocketType;		//0-前端 1-后端 2-未知
	int  			nUserPrivilege;     //自编流水号权限 0-无，非0-有
	int				nUID;				//企业在数据库中的惟一编号
	int				nGateType;			//网关类型(对于后端spgate来说)
	//zhubo 2017-4-7
	//IOCP_KEY_PTR	pKey;				//完成键指针
	short			nStatus;			//当前状态(OK/ULTRA_FLOW)
	time_t			tDelay;				//记录该socket延时的时刻
	UINT			nSocket;			//socket
	USHORT			nVer;				//协议版本号
	UINT			nPort;				//端口号
	UCHAR			cLogin;				//是否已登陆标志
	UCHAR			szSrcAddr[MAX_SPID_LEN+1];		//sp_id
	char			szSpGate[MAX_CHANNEL_LEN+1];
	char			szIP[MAX_IPADDR_LEN+1];
	char			cNeedSend;			//0:需要处理  1:无需处理
	char			cKeepMsgId;			//0:无需保留自带的MsgId  1:保留自带的MsgId
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
	short	nGateType;			//网关类型 0移动 1联通 21电信
	short	nExpTraits;			//扩展特性 0可扩展 1不可扩展 2共用
	short	nFeeFlag;			//费率标志 1预付 2后付 0未知

	USHORT	usSignlen;			//签名长度
	USHORT	usMaxwords;			//最大短信长度
	USHORT	usSinglelen;		//单条短信长度
	USHORT	usMultilen1;		//长短信第一条的长度
	USHORT	usMultilen2;		//长短信第二条的长度
	char	szSignature[MAX_SIGN_LEN*2+1];	//签名
	/************* qiwei add ********/

	USHORT	usEnSignlen;			//签名长度
	USHORT	usEnMaxwords;			//最大短信长度
	USHORT	usEnSinglelen;		//单条短信长度
	USHORT	usEnMultilen1;		//长短信第一条的长度
	USHORT	usEnMultilen2;		//长短信第二条的长度
	//需不需要增加英文签名
	char	szEnSignature[MAX_SIGN_LEN*2+1];	//签名
	/**************** end qiwei add ****************/

	char	szSpGate[MAX_CHANNEL_LEN+1];     //主端口
	char	szExNo[MAX_CHANNEL_LEN+1];		//扩展端口
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
	short nSendCount;	//当前窗口大小
	short nSockType;	//socket类型 0:前端 1:后端
	short nMaxWnd;		//窗口最大值，动态调整
	DWORD tLastSend;	//最后一次发送的时间
	DWORD tZeroCnt;		//窗口减小至0的时间
	short nDecCnt;		//tNow-tLastSend时间段内减少的窗口数
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
	UINT		nPkLen;                //包长度 
	UINT		nCmdId;                //命令ID
	UINT		nSeqId;                //序号
	char		szTime[MAX_TIME_LEN+1];	//接收到包的时间
	SOCKET_INFO	sockInfo;
	UCHAR		szData[MAX_PACK_LEN+1]; //结点的数据域

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
	UINT		nLen;						//发送包长度
	UINT		nCmdId;						//命令ID
	UINT		nSeqId;
	SOCKET_INFO	sockInfo;
	UCHAR		szData[MAX_PACK_LEN+1];	//发送包数据
	
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
	UINT		nLen;					//发送包长度
	UINT		nCmdId;					//命令ID
	UINT		nSeqId;
	SOCKET_INFO	sockInfo;
	UCHAR		szData[MAX_RSPPACK_LEN];				//发送包数据
	
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
	SOCKET_INFO		sockInfo;						//socket信息头
	UINT			nPkLen;							//包长度 
	UINT			nCmdId;							//命令ID
	UINT			nSeqId;							//序号
	int				nRemainedNum;					//余额
	USHORT			nResult;						//1字节//结果
	UCHAR			szMsgId[MAX_MSGID_LEN+1];		//MSGID
	UCHAR			szSpId[MAX_SPID_LEN+1];			//SPID
	UCHAR			szReserve[MAX_RESERVE_LEN*8+1];	//64字节//保留
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
	SOCKET_INFO		sockInfo;						//socket信息头
	UINT			nPkLen;							//包长度 
	UINT			nCmdId;							//命令ID
	UINT			nSeqId;							//序号
	USHORT			nQueryType;						//查询绑定类型0:移动 1:联通 2:电信 9：全网
	USHORT			nResult;						//1字节//结果
	UCHAR			szMsgId[MAX_MSGID_LEN+1];		//MSGID
	UCHAR			szSpId[MAX_SPID_LEN+1];			//SPID
	UCHAR			szChannelInfo[MAX_CHANNELINFO_LEN+1];//通道绑定信息
	UCHAR			szReserve[MAX_RESERVE_LEN+1];	//64字节//保留
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
	int				nMsgFmt;							//编码方式
	//zhubo 2017-06-21  移植过来
	int             nMsgLen;                              //消息长度
	INT64           nUsrMsgId;			                   //用户自定义流水号
	char			szPhone[SIGN_MAX_PHONE_LEN+1];         //上行手机号
	char			szSubpot[MAX_CHANNEL_LEN+1];	       //子端口
	char			szMsg[2*MAX_MSG_LEN+1];          //短信内容
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
	int				nMsgFmt;							//编码方式
	int				nMsgLen;                             //消息长度
	char			szMobile[SIGN_MAX_PHONE_LEN + 1];         //上行手机号
	char			szContent[2 * MAX_MSG_LEN + 1];          //短信内容
	char            szSvrtype[MAX_SERTYPE_LEN + 1];
	char			szExno[MAX_CHANNEL_LEN + 1];	       //子端口
	char            szCustId[MAX_CUSTID_LEN + 1];		//用户自定义流水号
	char            szExData[MAX_EXDATA_LEN + 1];
	//zhubo 2018-03-26
	int				nChargeCount;						//计费条数
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
	int				nMsgFmt;							//编码方式
	string			strContent;
	
	tagMixMsgV5()
	{
		bIsEnglish = 0;
		nMsgFmt    = 0;
		strContent = "";
	}
}MixMsgV5;


#define MAX_FMT_LEN 30
//这一
struct PACK_SUBMIT_V1
{
	
	time_t			t;
	int				nUid;						//企业Id=绑定表中UIDEx
	int				nOrgUid;					//原始UID=绑定表中UID
	int				nDestUID;					//目标UID=后端spgate的UID
	int				nEcId;						//ecid
	int				nTaskId;					//任务Id
	short			nSendStatus;				/*	
												1:CMPPGW收到前端SpGate的Submit包但没有成功发送至后端SpGate
												2:CMPPGW收到前端SpGate的Submit包并成功发送至后端SpGate
												3:后端SpGate成功将Submit包投递给移动行业网关
												4:收到移动返回的状态报告
												5:将状态报告成功发送给CMPPGW
												0:CMPPGW收到状态报告，并成功发送给前端SpGate
												*/
	short			nFlag;						//内部状态标志 
												/*	
												1:收到用户submit等待写库，
												2:写库成功，增加发送缓冲，并等待submit_rsp投递结果
												3:submit_rsp投递成功，等待转发至后端SpGate结果
												4:submit_rsp投递失败等待写库线程处理
												5:投递给后端SpGate失败
												*/
	UCHAR			szSourceAddr[MAX_SPID_LEN+1];			//6字节//源地址，此处为SP_Id，即SP的企业代码。
	UCHAR			szAuthenSource[MAX_AUTHENSRC_LEN+1];		//16字节//用于鉴别源地址。其值通过单向MD5 hash计算得出
	char			szTime[MAX_TIME_LEN+1];
	//上面不是标准SUMBIT消息的内容，为了兼容CONNECT ACTIVETEST TIMERNATE三种消息添加了上面部分定义


	UCHAR			szMsgId[MAX_MSGID_LEN+1];				//8字节//信息标识，由SP侧短信网关本身产生，本处填空。
	short			nMsgLevel;					//1字节//信息级别
	UCHAR			szServiceId[MAX_SRVID_LEN+1];			//10字节//业务类型，是数字、字母和符号的组合。
	USHORT			nMsgFmt;					//1字节//信息格式  0：ASCII串  3：短信写卡操作  4：二进制信息  8：UCS2编码15：含GB汉字  。。。。。。
	UCHAR			szSrcId[MAX_SRCID_LEN+1];				//21字节//源号码SP的服务代码或前缀为服务代码的长号码, 网关将该号码完整的填到SMPP协议Submit_SM消息相应的source_addr字段，该号码最终在用户手机上显示为短消息的主叫号码
	USHORT			nDestUsrTl;					//1字节//接收信息的用户数量(小于500个用户)
	char			szPhoneList[MAX_PHONE_LEN*MAX_PHONE_NUM+1];		//解析后的手机列表，用“,”隔开
	USHORT			nMsgLength;					//1字节//信息长度(Msg_Fmt值为0时：<160个字节；其它<=140个字节)
	UCHAR			szMsgContent[2*MAX_SPLIT_MSG_LEN+1];		//信息内容
	UCHAR			szReserve[MAX_RESERVE_LEN+1];				//8字节//保留
	INT64           nUsrMsgId;                         //用户自生成流水号
	char            szSvrtype[MAX_SERTYPE_LEN + 1];    //用户业务类型
	char            szCustId[MAX_CUSTID_LEN+1];     //新接口用户自生成流水号
	char            szExData[MAX_EXDATA_LEN + 1];
	char            szMsgSrcIp[MAX_IPADDR_LEN+8]; //消息源IP:port
	char			szTmplId[MAX_TMPLID_LEN + 1]; //模板ID
	TINYINT			nMsgType;					 //msgtype 1:为闪信   11富信
	TINYINT			nEncodeFlag;      //编码标志 1：明文，2：base64
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


//****************注意修改了这个结构体，一定要修改内存池初始化的地方********************
struct PACK_SUBMIT
{
	
	time_t			t;
	int				nUid;						//企业Id=绑定表中UIDEx
	int				nOrgUid;					//原始UID=绑定表中UID
	int				nDestUID;					//目标UID=后端spgate的UID
	int				nEcId;						//ecid
	int				nTaskId;					//任务Id
	short			nSendStatus;				/*	
												1:CMPPGW收到前端SpGate的Submit包但没有成功发送至后端SpGate
												2:CMPPGW收到前端SpGate的Submit包并成功发送至后端SpGate
												3:后端SpGate成功将Submit包投递给移动行业网关
												4:收到移动返回的状态报告
												5:将状态报告成功发送给CMPPGW
												0:CMPPGW收到状态报告，并成功发送给前端SpGate
												*/
	short			nFlag;						//内部状态标志 
												/*	
												1:收到用户submit等待写库，
												2:写库成功，增加发送缓冲，并等待submit_rsp投递结果
												3:submit_rsp投递成功，等待转发至后端SpGate结果
												4:submit_rsp投递失败等待写库线程处理
												5:投递给后端SpGate失败
												*/
	UCHAR			szSourceAddr[MAX_SPID_LEN+1];			//6字节//源地址，此处为SP_Id，即SP的企业代码。
	UCHAR			szAuthenSource[MAX_AUTHENSRC_LEN+1];		//16字节//用于鉴别源地址。其值通过单向MD5 hash计算得出
	char			szTime[MAX_TIME_LEN+1];
	//上面不是标准SUMBIT消息的内容，为了兼容CONNECT ACTIVETEST TIMERNATE三种消息添加了上面部分定义


	UCHAR			szMsgId[MAX_MSGID_LEN+1];				//8字节//信息标识，由SP侧短信网关本身产生，本处填空。
	short			nMsgLevel;					//1字节//信息级别
	UCHAR			szServiceId[MAX_SRVID_LEN+1];			//10字节//业务类型，是数字、字母和符号的组合。
	USHORT			nMsgFmt;					//1字节//信息格式  0：ASCII串  3：短信写卡操作  4：二进制信息  8：UCS2编码15：含GB汉字  。。。。。。
	UCHAR			szSrcId[MAX_SRCID_LEN+1];				//21字节//源号码SP的服务代码或前缀为服务代码的长号码, 网关将该号码完整的填到SMPP协议Submit_SM消息相应的source_addr字段，该号码最终在用户手机上显示为短消息的主叫号码
	USHORT			nDestUsrTl;					//1字节//接收信息的用户数量(小于500个用户)
	char			szPhoneList[MAX_PHONE_LEN*MAX_PHONE_NUM+1];		//解析后的手机列表，用“,”隔开
	USHORT			nMsgLength;					//1字节//信息长度(Msg_Fmt值为0时：<160个字节；其它<=140个字节)
	UCHAR			szMsgContent[2*MAX_SPLIT_MSG_LEN+1];		//信息内容
	UCHAR			szReserve[MAX_RESERVE_LEN+1];				//8字节//保留
	INT64           nUsrMsgId;                         //用户自生成流水号
	char            szSvrtype[MAX_SERTYPE_LEN + 1];    //用户业务类型
	char            szCustId[MAX_CUSTID_LEN+1];     //新接口用户自生成流水号
	char            szExData[MAX_EXDATA_LEN + 1];
	char            szMsgSrcIp[MAX_IPADDR_LEN+8]; //消息源IP:port
	char			szTmplId[MAX_TMPLID_LEN + 1]; //模板ID
	TINYINT			nMsgType;					 //msgtype 1:为闪信   11富信
	TINYINT			nEncodeFlag;      //编码标志 1：明文，2：base64
	TINYINT			nValidTime;//有效时长
	USHORT			nRmsLen;
	char 			szShowway[MAX_RMS_SHOWWAY_LEN+1];
	char			szShowtime[MAX_RMS_SHOWTIME_LEN+1];
	char 			szTitle[MAX_RMS_TITLE_LEN+1];
	char 			szRMSContent[MAX_RMS_TMPLCONTENT_V20_LEN+1];
	int				nCtrBit;       //控制位
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
//****************注意修改了这个结构体，一定要修改内存池初始化的地方********************

typedef std::multimap<int, PACK_SUBMIT> MULTIMAP_MT;
typedef std::deque<PACK_SUBMIT> DEQUE_MT;
typedef std::vector<PACK_SUBMIT> VECTOR_MT;
typedef std::vector<PACK_SUBMIT_V1> VECTOR_MT_V1;


//当ISMG向SP送交状态报告时，信息内容字段（Msg_Content）格式定义如下
struct MSG_CONTENT
{
	UCHAR szMsgId[MAX_MSGID_LEN+1];				//8字节//信息标识SP提交短信（CMPP_SUBMIT）操作时，与SP相连的ISMG产生的Msg_Id。
	UCHAR szState[MAX_ERSTAT_LEN+1];				//7字节//发送短信的应答结果，含义与SMPP协议要求中stat字段定义相同，详见表一。SP根据该字段确定CMPP_SUBMIT消息的处理状态。
	UCHAR szSubmitTime[MAX_SUBMITTM_LEN+1];		//10字节//YYMMDDHHMM（YY为年的后两位00-99，MM：01-12，DD：01-31，HH：00-23，MM：00-59）
	UCHAR szDoneTime[MAX_DONETM_LEN+1];			//10字节//YYMMDDHHMM
	UCHAR szDestTerminalId[MAX_DESTTMID30_LEN+1];	//21字节//目的终端MSISDN号码(SP发送CMPP_SUBMIT消息的目标终端)
	UCHAR szSMSCSequence[MAX_SMSCSEQ_LEN+1];		//4字节//取自SMSC发送状态报告的消息体中的消息标识。
	
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
State字段定义
Message State	Final Message States	Description
DELIVERED		DELIVRD					Message is delivered to destination
EXPIRED			EXPIRED					Message	validity period hasexpired
DELETED			DELETED					Message	has been deleted.
UNDELIVERABLE	UNDELIV					Message is undeliverable
ACCEPTED		ACCEPTD					Message is in accepted state(i.e. has been manually read on behalf of the subscriber by customer service)
UNKNOWN			UNKNOWN					Message is in invalid state
REJECTED		REJECTD					Message is in a rejected state

  注意：
  1．其中ACCEPTED为中间状态，网关若从短信中心收到后应丢弃，不做任何操作。
  2．Stat字段长度为7个字节，填写时应填表一中Final Message States中的缩写形式，如
  状态为DELIVERED时填写DELIVRD，依此类推。
  3．SP等待状态报告缺省时间为48小时。
*/
//****************注意修改了这个结构体，一定要修改内存池初始化的地方********************

struct PACK_DELIVER
{
	SOCKET_INFO		sockInfo;
	time_t			t;
	PACK_RSP		packRsp;						
	int				nSendStatus;				/*
												1:后端SpGate收到mo(手机上行短信)发送给CMPPGW但未成功
												2:后端SpGate收到mo(手机上行短信)发送给CMPPGW成功
												3:CMPPGW收到后端SpGate发送过来的手机mo，并成转发至前端SpGate,若未成功重置为状态1
												0:CMPPGW收到用户成功接收的回应
												*/				
	short			nFlag;						/*	
												//1:收到后端的fwd包，等待更新库，
												//2:更新库成功，增加发送缓冲，并等待deliver包投递给用户的结果
												1:fwd_rsp包投递成功
												2:fwd_rsp包投递失败等待写库线程处理
												3:fwd转成deliver包投递前端成功等待写库线程处理
												4:deliver包投递前端失败
												5:收到前端的deliver_rsp
												*/
	short			nResult;					//1字节//结果
	short			nRePack;					//是否需要重组包，若从数据库中读出来的，需重组 0：不组 1：组

	int				nUID;						//该deliver要发给哪个登陆系统的用户
	int				nOrgUID;					//该deliver是从哪个spgate送来的
	int				nUIDEx;						//该deliver是最终属于哪个用户的
	int				nEcId;
	UINT			nPkLen;						//包长度 
	UINT			nCmdId;						//命令ID
	UINT			nSeqId;						//序号
	short			nStatus;					//状态
	char			szTime[MAX_TIME_LEN+1];
	short			nFeeFlag;					//费率标志(1预付2后付)
	short			usReSendNum;				//已重发次数 用于统计本结构体的已重发次数。1.0 v.2.0 v3.0
	UCHAR           cPRI;				        //优先权   2012-06-06 sp add 
//上面不是标准DELIVER信息的内容为了兼容REPORT RSP信息而添加

	UCHAR			szUsrId[MAX_SPID_LEN+1];
	UCHAR			szMsgId[MAX_MSGID_LEN+1];				//8字节//信息标识生成算法如下：采用64位（8字节）的整数：（1）	时间（格式为MMDDHHMMSS，即月日时分秒）：bit64~bit39，其中bit64~bit61：月份的二进制表示；bit60~bit56：日的二进制表示；bit55~bit51：小时的二进制表示；bit50~bit45：分的二进制表示；bit44~bit39：秒的二进制表示；（2）	短信网关代码：bit38~bit17，把短信网关的代码转换为整数填写到该字段中。（3）	序列号：bit16~bit1，顺序增加，步长为1，循环使用。各部分如不能填满，左补零，右对齐。
	UCHAR			szPtMsgId[MAX_MSGID_LEN+1];				//平台MsgId
	UCHAR			szDestId[MAX_DESTID_LEN+1];				//21字节//目的号码 SP的服务代码，一般4--6位，或者是前缀为服务代码的长号码；该号码是手机用户短消息的被叫号码。
	UCHAR			szServiceId[MAX_SRVID_LEN+1];			//10字节//业务类型，是数字、字母和符号的组合。
	UCHAR           szSpNumEx[MAX_CHANNEL_LEN+1];           //扩展子号
	USHORT			nTpPid;						//1字节//GSM协议类型。详细解释请参考GSM03.40中的9.2.3.9
	USHORT			nTpUdhi;					//1字节//GSM协议类型。详细解释请参考GSM03.40中的9.2.3.23，仅使用1位，右对齐
	USHORT			nMsgFmt;					//1字节//信息格式  0：ASCII串  3：短信写卡操作  4：二进制信息  8：UCS2编码15：含GB汉字
	UCHAR			szSrcTerminalId[MAX_SRCTMID30_LEN+1];		//21字节//源终端MSISDN号码（状态报告时填为CMPP_SUBMIT消息的目的终端号码）
	USHORT			nSrcTerminalType;
	USHORT			nReportFlag;				//1字节//是否为状态报告0：非状态报告1：状态报告
	USHORT			nMsgLen;					//1字节//消息长度
	UCHAR			szMsgContent[2*MAX_SPLIT_MSG_LEN+1];		//消息内容
	UCHAR			szReserved[MAX_RESERVE_LEN+1];			//保留项
	UCHAR			szLinkID[MAX_LINKID_LEN+1];
	MSG_CONTENT		msgcontent; 
	INT64           nUsrMsgId;
//#ifndef SHRINKPACK
//	unsigned char	szData[MAX_PACK_LEN+1];
//#endif
	char			szCustId[MAX_CUSTID_LEN + 1];
	int  nRspFmt;						//回应格式
	int nReSendCnt;                    //v3.0 uid账号自定义最大推送失败次数 从HSBC_URLINFO::nPushFailCnt来数据
	int nPknum;                        //当前条数
	int nPkTotal;                      //总条数
	char			szSendTime[MAX_TIME_LEN + 1];
	char            szRetTime[MAX_TIME_LEN + 1];
	char            szExData[MAX_EXDATA_LEN + 1];
	char			szSignature[2*MAX_SIGN_LEN+1];	//签名

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
//****************注意修改了这个结构体，一定要修改内存池初始化的地方********************

typedef std::multimap<int, PACK_DELIVER> MULTIMAP_MO;
typedef std::deque<PACK_DELIVER> DEQUE_MO;

struct SPGATE_BIND_INFO
{
	int		nLoginUID;				//企业在数据库中的惟一编号
	int		nUserUID;				//nUID所关联的其他帐号的UID
	int		nDestUID;			//目标UID=后端spgate的UID
	int		nEcId;				//企业ID
    //zhubo 2017-5.24
	int		nGateGroupId;       //通道组ID
	TINYINT nBindType;			//绑定类型 0：通道账号，1：通道组
	TINYINT nGateLevel;         //主备：0主，1备
    //zhubo 2017-5.24
	short	nGateType;			//网关类型 0移动 1联通 2电信
	short	nFeeFlag;			//费率标志 1预付 2后付 0未知
	short	nExpTraits;			//扩展特性 0可扩展 1不可扩展 2共用
	short	nRiseLevel;			//大于0为提升级别n级， 小于0降低级别n级，等于0维持原级   (现该值做为发送级别用不再做提升之用)
	UCHAR	ucExChannel;		//通道是否为主端口扩展  0:该通道直接从移动定购的由移动签名 1:在移动定购通道的基础上我司再增加的扩展
	USHORT	usSignlen;			//签名长度
	USHORT	usMaxwords;			//最大短信长度
	USHORT	usSinglelen;		//单条短信长度
	USHORT	usMultilen1;		//长短信第一条的长度
	USHORT	usMultilen2;		//长短信第二条的长度
	/*******qiwei add*******************/

	USHORT	usEnSignlen;			//签名长度
	USHORT	usEnMaxwords;			//最大短信长度
	USHORT	usEnSinglelen;		//单条短信长度
	USHORT	usEnMultilen1;		//长短信第一条的长度
	USHORT	usEnMultilen2;		//长短信第二条的长度
	char	szEnSignature[2*MAX_SIGN_LEN+1];	//签名

  /********** end qiwei add **********/
	DATE	dtStart;
	DATE	dtEnd;
	char    cRouteFlag;         //路由标志 0:MT/MO/RPT不分 1:MT路由 2:MO路由 3:RPT路由
	char	szLoginId[MAX_SPID_LEN+1];		//登陆帐号
	char	szUsrId[MAX_SPID_LEN+1];		//关联帐号
	char	szSpGate[MAX_CHANNEL_LEN+1];		//主端口
	char	szExNo[MAX_CHANNEL_LEN+1];		//扩展端口
	char	szSpNumber[MAX_CHANNEL_LEN+1];	//主端口+扩展端口
	char	szSignature[2*MAX_SIGN_LEN+1];	//签名
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
				//以下部分允许同一帐号绑定多个相同类型通道，直接返回true则不允许这种情况
				//如：106579999001,10659999002是不允许的
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
typedef std::multimap<int/*uid*/, SPGATE_BIND_INFO/*通道信息*/> BIND_MT_INFO_MAP;
typedef std::multimap<int/*uid*/, SPGATE_BIND_INFO/*通道信息*/> BIND_MO_INFO_MAP;

/************************************************************************/
/*                双主键索引(Rpt2PreBindMap)                            */
/************************************************************************/
//自定义排序规则
/*
struct DEF_KEY 
{
	int nUID;		//绑定表中的nUIDEx相同
	int nDestUID;	//绑定表中的nDestUID相同

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
/*双map,双主键索引(Mo2PreBindMap)                                       */
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
		//先按长度
		if (nLenX < nLenY)
		{
			//长度升序排列
			//return true;
			//长度降序排序
			return false;
		}
		else
		{
			//长度相等
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
				//长度升序排列
				return true;
				//长度降序排序
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
		//先按长度
		if (nLenX < nLenY)
		{
			//长度升序排列
			//return true;
			//长度降序排序
			return false;
		}
		else
		{
			//长度相等
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
				//长度升序排列
				return true;
				//长度降序排序
				//return false;
			}
		}
	}
};
typedef std::map<SPNO_KEY, SPGATE_BIND_INFO> SPNO_KEY_MAP;
typedef std::map<int, SPNO_KEY_MAP> TWO_KEY_TWO_MAP_EX;

/************************************************************************/
/*双map,三主键索引(Mt2PreBindMap)                                       */
/************************************************************************/

struct  THREE_KEY
{
	short nGateType;			//0:移动 1:联通 21:电信C网
	char szSpNo[MAX_CHANNEL_LEN+1];			//spnumber
	short nExpTraits;			//扩展属性 0:可扩展 1:不可扩展
	
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
		//网关类型比较
		if (x.nGateType == y.nGateType)
		{
			int nLenX = strlen(x.szSpNo);
			int nLenY = strlen(y.szSpNo);
			//先按长度
			if (nLenX < nLenY)
			{
				//长度升序排列
				//return true;
				//长度降序排序
				return false;
			}
			else
			{
				//长度相等
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
					//长度升序排列
					return true;
					//长度降序排序
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
	int		nUID;				//企业在数据库中的惟一编号
	int		nLastDbRemained;	//上次与数据库同步时数据库中的可用数量
	int		nRemained;			//可用数量
	int		nPrePayUsed;		//预付费已用数量
	int		nPostPayUsed;		//后付费已用数量
	short	nFeeFlag;			//费率标志
	UCHAR	ucInDebt;			//是否欠费 0:不欠 1:欠费
	char	szUsrId[MAX_SPID_LEN+1];		//计费帐号
	time_t  tLastSync;			//最后一次同步费用的时间
	time_t  tLastInDebt;		//最后一次欠费用时间
	unsigned int nNum_Inner;    //国内条数
	unsigned int nNum_Global;   //国外条数
	INT64	nPrice_Inner;		//国内短信单价
	INT64	nPrice_Global;      //国际短信单价
	INT64   nMoney;             //用户金额
	TINYINT	nChargeType;        //扣费方式 0：条数，1：金额
	TINYINT nChargeObj;         //计费对象 0：按账号，1：按企业
	TINYINT nFailNums;          //同步费用失败次数
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
	int nUID;			//企业UID
	int nDestUID;				
	int nSendLevel;		//发送级别
	int nNum;			//筛选条数
	int nRemained;		//剩余量
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
//平台系统信息
/*
struct SYS_INFO
{
	int	nCpu;			//cpu占用量
	int	nMem;			//物理占用量(M)
	int	nVmem;			//虚拟内存占用量(M)

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

//客户端信息
struct CLIENT_INFO
{
	short	nOnlineStatus;		//在线状态 0:在线 1:离线 2:未知
	int		nTotalMtSend;		//用户提交的Mt总量
	int		nHaveSendMt;		//已被成功转发的Mt总量
	int		nRecvRpt;			//收到rpt
	int		nRecvMo;			//收到的mo
	int		nRemainedMt;		//用户滞留的Mt
	int		nTimerSendMt;		//用户定时发送的Mt
	int		nRecvSpeed;			//即时速度
	char	szUsrId[MAX_SPID_LEN+1];		//登入用的户的ID。例：910000

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
//通道信息
/*
struct CHANNEL_INFO
{
	short	nOnlineStatus;			//在线状态 0:在线 1:离线 2:未知
	int		nTotalRptSend;			//该通道送来的状态报告总量
	int		nTotalMoSend;			//该通道送来的上行总量
	int		nHaveSendMo;			//已成功转发至用户的MO
	int		nHaveSendRpt;			//已成功转发至用户的Rpt
	int		nRecvMt;				//该通道收到并已转发的Mt
	int		nRemainedMt;			//该通道滞留的Mt
	int		nTimerSendMt;			//该通道定时发送的Mt
	int		nRemainedRpt;			//该通道有多少滞留RPT
	int		nRemainedMo;			//该通道有多少滞留MO
	int		nRecvSpeed;				//即时速度
	char	szUsrId[MAX_SPID_LEN+1];			//登入用的户的ID。例：910000
	char	szSpGate[MAX_CHANNEL_LEN+1];			//通道号
	int		nUID;					//通道UID(该字段新增对梦网平台监控系统有影响)

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
	int nChannelNo;			//同类型通道序号
	int nQuota;				//通道发送已分配额度
	int nRemained;			//当前的滞留量
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
	int nGateType;						//网关类型
	int nTotalSend;						//该类型发送总量
	int nTotalRemained;					//该类型总滞留量
	int nChannelNum;					//channel_quota中现有多少个通道
	CHANNEL_QUOTA channel_quota[CHANNEL_SEQ_ARR];	//同类型通道序列
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
	int		nUID;				//企业在数据库中的惟一编号
	short	nStatus;			//帐号状态 0:正常 1：消息结构错 2：非法源地址 3：认证错 4：版本太高5~ ：其他错误
	short	nUsrType;			//登入用户的类型，0-前端 1-后端 2-未知
	short	nOnlineStatus;		//在线状态
	int  	nUserPrivilege;     //自编流水号权限 0-无，非0-有
	
	int		nSendLevel0;		//前端发送到平台的单发数量
	int		nSendLevel1;		//前端发送到平台的群发(少量手机)
	int		nSendLevel2;		//前端发送到平台的大批量群发
	int		nSendLevel3;
	int		nSendLevel4;
	int		nSendLevel5;
	int		nSendLevel6;
	int		nSendLevel7;
	int		nSendLevel8;
	int		nSendLevel9;
	int		nHaveSendLevel0;	//已成功转发至后端的单发
	int		nHaveSendLevel1;	//已成功转发至后端的群发(少量手机)
	int		nHaveSendLevel2;	//已成功转发至后端的大批量群发
	int		nHaveSendLevel3;	//已成功转发至后端的单发
	int		nHaveSendLevel4;	//已成功转发至后端的群发(少量手机)
	int		nHaveSendLevel5;	//已成功转发至后端的大批量群发
	int		nHaveSendLevel6;	//已成功转发至后端的大批量群发
	int		nHaveSendLevel7;	//已成功转发至后端的单发
	int		nHaveSendLevel8;	//已成功转发至后端的群发(少量手机)
	int		nHaveSendLevel9;	//已成功转发至后端的大批量群发
	int		nRecvRpt;			//收到rpt
	int		nRecvMo;			//收到的mo
	
	int		nRemainedLevel0;	//该用户有多少mt0没有发走(对于前端来说是各通道的总和，对后端来说指滞留的mt0):暂时由后端使用
	int		nRemainedLevel1;	//该用户有多少mt1没有发走(对于前端来说是各通道的总和，对后端来说指滞留的mt1):暂时由后端使用
	int		nRemainedLevel2;	//该用户有多少mt2没有发走(对于前端来说是各通道的总和，对后端来说指滞留的mt2):暂时由后端使用
	int		nRemainedLevel3;	//该用户有多少mt0没有发走(对于前端来说是各通道的总和，对后端来说指滞留的mt0):暂时由后端使用
	int		nRemainedLevel4;	//该用户有多少mt1没有发走(对于前端来说是各通道的总和，对后端来说指滞留的mt1):暂时由后端使用
	int		nRemainedLevel5;	//该用户有多少mt2没有发走(对于前端来说是各通道的总和，对后端来说指滞留的mt2):暂时由后端使用
	int		nRemainedLevel6;	//该用户有多少mt0没有发走(对于前端来说是各通道的总和，对后端来说指滞留的mt0):暂时由后端使用
	int		nRemainedLevel7;	//该用户有多少mt1没有发走(对于前端来说是各通道的总和，对后端来说指滞留的mt1):暂时由后端使用
	int		nRemainedLevel8;	//该用户有多少mt2没有发走(对于前端来说是各通道的总和，对后端来说指滞留的mt2):暂时由后端使用
	int		nRemainedLevel9;	//该用户有多少mt2没有发走(对于前端来说是各通道的总和，对后端来说指滞留的mt2):暂时由后端使用

	int		nTimerSendLevel0;	//该用户有多少定时发送的mt0(对于前端用户来说是各通道的总和，对后端来说指定时发送的数据)
	int		nTimerSendLevel1;	//该用户有多少定时发送的mt1(对于前端用户来说是各通道的总和，对后端来说指定时发送的数据)
	int		nTimerSendLevel2;	//该用户有多少定时发送的mt2(对于前端用户来说是各通道的总和，对后端来说指定时发送的数据)
	int		nTimerSendLevel3;	//该用户有多少定时发送的mt0(对于前端用户来说是各通道的总和，对后端来说指定时发送的数据)
	int		nTimerSendLevel4;	//该用户有多少定时发送的mt1(对于前端用户来说是各通道的总和，对后端来说指定时发送的数据)
	int		nTimerSendLevel5;	//该用户有多少定时发送的mt2(对于前端用户来说是各通道的总和，对后端来说指定时发送的数据)
	int		nTimerSendLevel6;	//该用户有多少定时发送的mt0(对于前端用户来说是各通道的总和，对后端来说指定时发送的数据)
	int		nTimerSendLevel7;	//该用户有多少定时发送的mt1(对于前端用户来说是各通道的总和，对后端来说指定时发送的数据)
	int		nTimerSendLevel8;	//该用户有多少定时发送的mt2(对于前端用户来说是各通道的总和，对后端来说指定时发送的数据)
	int		nTimerSendLevel9;	//该用户有多少定时发送的mt0(对于前端用户来说是各通道的总和，对后端来说指定时发送的数据)

	int		nRemainedRpt;		//前端用户有多少滞留RPT
	int		nRemainedMo;		//前端用户有多少滞留MO

	int		nTotalRptSend;		//后端总发往平台的RPT数量
	int		nTotalMoSend;		//后端总发往平台的MO数量
	int		nHaveSendMo;		//已成功转发至前端的MO
	int		nHaveSendRpt;		//已成功转发至前端的Rpt
	int		nRecvLevel0;		//后端收到平台转发的单发数量
	int		nRecvLevel1;		//后端收到平台转发的群发量(少量手机)
	int		nRecvLevel2;		//后端收到平台转发的大批量群发
	int		nRecvLevel3;		//后端收到平台转发的单发数量
	int		nRecvLevel4;		//后端收到平台转发的群发量(少量手机)
	int		nRecvLevel5;		//后端收到平台转发的大批量群发
	int		nRecvLevel6;		//后端收到平台转发的单发数量
	int		nRecvLevel7;		//后端收到平台转发的群发量(少量手机)
	int		nRecvLevel8;		//后端收到平台转发的大批量群发
	int		nRecvLevel9;		//后端收到平台转发的单发数量

	int		nRecvSpeed;			//即时接收速度
// 	int		nConNum;			//该用户连接数
	int		nFailureRate;		//该用户的失败率(0表求不控制 大于0表示控制，nFailureRate/PRECISION来表示精度)
	int		nFailureNum;		//该用户的失败数(该数字不清空，循环使用)
	int		nTotalMtSubmit;		//该用户总共递交了多少MT(该数字不清空，循环使用)
	UINT	nPort;
	time_t	tLogin;							//登入时间
	time_t	tLoginout;						//下线时间
	time_t  tLastMoTimeOut;					//最后一次读取Mo超时时间
	time_t	tLastRptTimeOut;				//最后一次读取Rpt超时时间
	int		nMoTimeOutCnt;					//MO超时次数，每超时一次就增1，每正常接收一次后归0
	int		nRptTimeOutCnt;					//RPT超时次数，每超时一次就增1，每正常接收一次后归0
	char    cMoTimeOut;						//是否读取MO超时
	char	cRptTimeOut;					//是否读取RPT超时
	char	szUsrId[MAX_SPID_LEN+1];		//登入用的户的ID。例：910000
	char	szPwd[MAX_PWD_LEN+1];			//密码。例：910000
	char	szSpGate[MAX_CHANNEL_LEN+1];	//后端用户的通道号
	char	szIp[MAX_IPADDR_LEN+1];
	char	szCorpName[MAX_COPRNAME_LEN+1];	//企业名称
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
	int nReSendCnt; //重发次数
	time_t tSndTm;  //发送时间

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

enum PACK_FMT//请求包格式
{
	UNKNOWN_FMT = 0,
	XML,
	JSON,
	URLENCODED = 4,
	USERDEFINE = 5  //传文件用
};

struct WBS_REQ_MT  //请求下发MT
{
	int		nUID;							//企业在数据库中的惟一编号
	char	szUsrId[MAX_SPID_LEN+1];		//登入用的户的ID。例：910000
	char	szLoginId[MAX_SPID_LEN+1];		//代理帐号ID
	char	szPwd[MAX_PWD_LEN+1];			//密码。例：910000
	char	szMobis[MAX_PHONE_NUM*MAX_PHONE_LEN+1];
	char	*szMsg;//[MAX_PACK_LEN+1];
	char	szExNo[MAX_CHANNEL_LEN+1];
	char	szIp[MAX_IPADDR_LEN+1];
	char    szSoapXmlns[MAX_PATH+1];
	int		nPort;
	int		nMobiCount;
	short	nConClose;					//用户的请求模式
	short	nReqMode;					//哪种模式soap/post/get
	short	nRspType;					//何种请求命令
	int		nSockId;
	int		nSocket;
	INT64   nUsrMsgId;                     //用户自生成流水号
	//zhubo 2017-4-7
	//IOCP_KEY_PTR pkey;
	time_t tRecvTime;

	char   szTimestamp[MAX_TIME_STAMP + 1];                       //时间戳定长10位
	int    nFmtReq;
	char   szFuncTmp[MAX_PATH_FUNC+1];
	char   szCustId[MAX_CUSTID_LEN+1];                //用户自定义流水号
//	INT64  nPackId;
	char   szSvrtype[MAX_SVRTYPE_LEN + 1];                      //服务类型
	TINYINT nVersion;                   //wbsV5.0中的协议版本
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
//文件上传请求相关结构等
/*
struct WBS_REQ_FILEUPLOAD  //请求上传文件
{
	int		nUID;							//企业在数据库中的惟一编号
	char	szUsrId[MAX_SPID_LEN+1];		//登入用的户的ID。例：910000
	char	szLoginId[MAX_SPID_LEN+1];		//代理帐号ID
	char	szPwd[MAX_PWD_LEN+1];			//密码。例：910000
	char	szIp[MAX_IPADDR_LEN+1];
	int		nPort;
	short	nConClose;					//用户的请求模式
	short	nReqMode;					//哪种模式soap/post/get
	short	nRspType;					//何种请求命令
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

struct WBS_REQ_QUERY					//请求mo/rpt/查费 
{
	int		nUID;						//企业在数据库中的惟一编号
	char	szUsrId[MAX_SPID_LEN+1];	//登入用的户的ID。例：910000
	char	szLoginId[MAX_SPID_LEN+1];	//代理帐号ID
	char	szPwd[MAX_PWD_LEN+1];		//密码。例：910000
	char	szIp[MAX_IPADDR_LEN+1];
	char    szSoapXmlns[MAX_PATH+1];
	int		nPort;
	short	nConClose;					//用户的请求模式
	short	nReqMode;					//哪种模式soap/post/get
	short	nRspType;					//何种请求命令
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
	TINYINT nVersion;                   //wbsV5.0中的协议版本
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
	int		nUID;						    //企业在数据库中的惟一编号
	char	szUsrId[MAX_SPID_LEN + 1];	    //登入用的户的ID。例：910000
	char	szLoginId[MAX_SPID_LEN + 1];	    //wbs代理帐号

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
	int     nEcid;                          //企业ID
	int		nUID;						    //企业在数据库中的惟一编号
	unsigned char uMaxCnn;                  //用户最大连接数
	unsigned char uCnn;                     //用户当前连接数
	unsigned char uMsgLevel;               //用户发送级别，从0到10
	char	szUsrId[MAX_SPID_LEN+1];	    //登入用的户的ID。例：910000
	char	szLoginId[MAX_SPID_LEN+1];	    //wbs代理帐号
	char	szPwd[MAX_PWD_LEN+1];	 	    //密码。例：910000
	char	szIp[MAX_IPADDR_LEN+1];	        //Ip地址
	char	szIpList[MAX_LOGINIP_LEN+1];    //Ip地址
	char    szIPSegment[1024+1];                //IP段
	int		nPort;						    //端口号
	int		nMoReq;						    //请求Mo次数
	int		nRptReq;					    //请求rpt次数
	int		nQueryReq;					    //请求查费次数
	int		nMtReq;					 	    //请求发送mt次数
	int		nMtReqEx;					    //用户的Mt被拆成实际的包数
	int		nMtNum;						    //当前用户在系统中滞留的Mt(内存中+DB)
	int		nMoNum;						    //当前用户在系统中滞留的Mo(内存中+DB)
	int		nRptNum;					    //当前用户在系统中滞留的Rpt(内存中+DB)
	int     nUserPrivilege;                 //用户特权
	//SPGATE_INFO SpGateInfo[CHANNEL_TYPE_ARR*CHANNEL_SEQ_ARR];
	time_t  tLastActive;				//该时间记录用户的最后一次在线时间，长时间没有动作就会被清除出用户表
	time_t  tUseTime;					//接收响应耗时
	time_t	tRspUseTime;				//平均响应时间(60秒统计一次)
	int nReqNum;						//请求次数
	int nRspSpeed;                      //响应速度

	bool nKeepAlive;                    //不支持长连接   

	int nPwdEncode;                     //密码加密方式
	char szEncodestr[256 + 1];          //加密串
	int nMsgCodeType;                   //请求内容的编码格式
	int nMsgEncode;                     //请求内容的加密方式
	int nGetrptmaxcnt;
	int nGetmomaxcnt;
	int nLoadFeeFlag;
	
	int nStatus;                       //账号状态0：可用
	TINYINT nChargeObj;                //计费对象0：账号 1：企业 
	TINYINT nChargeType;               //计费方式0：条数 1：金额 
	TINYINT nFeeFlag;                  //计费标准1：预付费，2：后付费
	TINYINT nAccountType;              //账号类型1：计费账号
	//下行占后4位，上行占前4位
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
	int nUID;						//用户UID
	int nCMD;						//回命包命令
	int nPackLen;					//回应包长度
	int nSockId;					//socket id
	int nSocket;					//socket
	UINT nConClose;					//是否连接断开
	UINT nReqMode;					//哪种模式soap/post/get
	int  nRspNUm;					//对于Rpt/Mo来说指一次组包的个数
	BOOL b100Continue;				//该回应包是否是100continue
	char szUserId[MAX_SPID_LEN+1];	//用户ID
	char szRspData[MAX_WBS_RSPPACK_LEN+1];	//回应包内容
	unsigned int nFailNum;			//发送失败次数
	time_t nLastPendTime;				//最后一次发送时间
	time_t tRecvTime;				//回掉函数接收到报文时间

	int  nRspMode;                  //回应模式 1全成功，2全失败，3成功和失败，4成功失败详情
	
	/***************zhubo 2017-4-14*******************/
	int				   nRetMode;//响应格式 json xml
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
	UINT nHttpHeaderLen; //http头的长度
	UINT nDataLen;		//数据长度
	UINT nConClose;		//是否要连接断开
	UINT nReqMode;		//soap/post/get模式请求(默认为0=soap,1=post,2=get)
	UINT nCMD;			//请求命令
	UINT nPort;
	UINT nSeqId;
	//zhubo 2017-4-7
	//IOCP_KEY_PTR pkey;
	char szIp[MAX_IPADDR_LEN+1];
	//zhubo 2017-4-14  只包含body的内容
	char *szData;//[MAX_PACK_LEN+1];//数据内容
	time_t tRecvTime;
	TINYINT nVersion;                   //wbsV5.0中的协议版本
	
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
	int nSrcUID;			//源帐号UID
	int nDestUID;			//目标帐号UID
	int nRouteType;			//路由类型
	int nGateType;			//通道类型
	int nFeeFlag;			//通道费率
	int nExNoHold;			//是否保留扩展子号标志
	int nExpiredTm;			//超时时限
	int nRouteSeq;			//路由序号
	int nProportion;		//发送比例
	int nSrcChannelNo;		//源通道ID
	int nDestChannelNo;		//目标通道ID
	int nMaxRemained;		//该通道最大允许滞留数  该值为<0时表示不受滞留数限制
	int nUpperLen;			//短信范围上限
	int nLowerLen;			//短信范围下限
	char szSrcUserId[MAX_SPID_LEN+1];		//源帐号
	char szDestUserId[MAX_SPID_LEN+1];		//目标帐号
	char szSrcSpNumber[MAX_CHANNEL_LEN+1];	//源通道号
	char szSrcSpGate[MAX_CHANNEL_LEN+1];	//源主通道
	char szDestSpNumber[MAX_CHANNEL_LEN+1];	//目标通道号
	char szDestSpGate[MAX_CHANNEL_LEN+1];	//目标主通道
	char szParams[MAX_PARAMS_LEN+1];		//路由参数
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


//路由表主键
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
		//UID比较
		if (x.nSrcUID == y.nSrcUID)
		{
			//路由类型比较
			if (x.nRouteType == y.nRouteType)
			{
				//对于按发送比例路由做特殊处理
				if (4 == x.nRouteType)
				{
					//网关类型比较
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
	//用户帐号，对下行来说是将其送来的旧通道号换成新通道号；对上行来说，是将新通道号换成旧通道号并且将上行送给该帐号
	char szUserId[MAX_SPID_LEN+1];
	char szOldSpNo[MAX_CHANNEL_LEN+1]; //旧通道
	char szNewSpNo[MAX_CHANNEL_LEN+1]; //新通道	
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
							/* 交行webservice */
/************************************************************************/

/*
//发送下行请求消息（单一号码，单一内容）
struct MT_REQ_PACK
{
	COMMAND  command;		//  操作命令
	char spid[6+1];			//	SP的ID	
	char sppassword[32+1];	//	SP密码	
	char spsc[10+1];		//	服务代码
	char sa[21+1];			//	源地址	
	char da[13+1];			//	目标地址
	char ec;				//	esm_class
	char pid;				//	protocol_id	
	char dc;				//	消息编码格式
	char sm[720];			//	消息内容
	int	 nMsgLen;			//  消息长度
	char priority;			//	优先级		
	char attime[14+1];		//  时发送时间	
	char validtime[14+1];	//  有效时间	
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
//发送相同内容批量下行请求消息（不同号码，相同内容）
struct MULTI_MT_REQ_PACK
{
	COMMAND  command;						//  操作命令
	char spid[MAX_SPID_LEN+1];				//	SP的ID	
	char sppassword[MAX_PWD_LEN+1];			//	SP密码	
	char spsc[MAX_SRVID_LEN+1];				//	服务代码
	char sa[MAX_SRCID_LEN+1];				//	源地址	
	char das[SIGN_MAX_PHONE_LEN*MAX_HTTP_PHONE_NUM+1];//	目标地址列表(用","隔开，个数不超过100)
	int  nDaNum;							//  手机号个数
	int	 taskid;							//	任务id号
	char ec;								//	esm_class
	char pid;								//	protocol_id	
	char dc;								//	消息编码格式
	char sm[2*MAX_MSG_LEN+1];				//	消息内容
	int  nMsgLen;							//  消息长度
	char priority;							//	优先级		
	char attime[MAX_WBS_ATTIME_LEN+1];		//  时发送时间	
	char validtime[MAX_WBS_VALIDTIME_LEN+1];//  有效时间	
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
	char das[SIGN_MAX_PHONE_LEN+1];		//	目标地址列表
	char sm[MAX_MSG_LEN*2*2+1];		//	消息内容
	int  nMsgLen;					//  消息长度
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

//发送不同内容批量下行请求消息（不同号码，不同内容）
struct MULTIX_MT_REQ_PACK
{
	COMMAND  command;						//  操作命令
	char spid[MAX_SPID_LEN+1];				//	SP的ID	
	char sppassword[MAX_PWD_LEN+1];			//	SP密码	
	char spsc[MAX_SRVID_LEN+1];				//	服务代码
	char sa[MAX_SRCID_LEN+1];				//	源地址	
	MULTIX_MT dasmarr[MAX_HTTP_PHONE_NUM];		//  手机号+短信内容对应列表
	int  nDaNum;							//  手机号个数(dasmarr数组的大小)
	int	 taskid;							//	任务id号
	char dc;								//	消息编码格式
	char ec;								//	esm_class
	char pid;								//	protocol_id	
	char priority;							//	优先级		
	char attime[MAX_WBS_ATTIME_LEN+1];		//  时发送时间	
	char validtime[MAX_WBS_VALIDTIME_LEN+1];//  有效时间	
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

//发送文件群发下行请求消息
struct BATCH_MT_REQ_PACK
{
	COMMAND  command;						//  操作命令
	char spid[MAX_SPID_LEN+1];				//	SP的ID	
	char sppassword[MAX_PWD_LEN+1];			//	SP密码	
	char spsc[MAX_SRVID_LEN+1];				//	服务代码
	char sa[MAX_CHANNEL_LEN+1];				//  源地址
	char bmttype;							//  群发类型 1:相同内容群发 2:不同内容群发 3:动态模版群发
	int	 taskid;							//	任务id号
	char title[MAX_TITLE_LEN+1];			//	群发标题 需要Encode编码，编码前最长为20字符
	char content[MAX_MSG_LEN*2+1];			//	群发内容(可选)
	int  nMsgLen;							//  消息长度(没有内容填0)
	char url[MAX_PATH+1];					//	群发文件url
	char dc;								//	消息编码格式
	char priority;							//	优先级		
	char attime[MAX_WBS_ATTIME_LEN+1];		//  时发送时间	
	char validtime[MAX_WBS_VALIDTIME_LEN+1];//  有效时间	
	
	TINYINT			nEncodeFlag;      //编码标志 1：明文，2：base64
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

//响应回应消息
struct RSP_PACK
{
	COMMAND  command;								//  操作命令
	char spid[MAX_SPID_LEN+1];						//	SP的ID	
	char msgids[MAX_MSGIDSTR_LEN*MAX_PHONE_NUM+1];	//  消息ID列表，用","隔开，不超过100个 发送错误响应消息时该项无效
	int	 nMsgIdNum;									//  消息ID个数 发送错误响应消息时该项无效
	char stat[MAX_ERSTAT_LEN+1];					//  消息状态
	char errcode[MAX_WBS_ERCODE_LEN+1];				//	错误码

	int  nRspMode;  //回应模式
	int  nRspFmt;  //回应格式  
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

//发送Mo请求
struct MO_REQ_PACK
{
	COMMAND  command;					//  操作命令
	char spid[MAX_SPID_LEN+1];			//	SP的ID	
	char sppassword[MAX_PWD_LEN+1];		//  密码()
	char spsc[MAX_SRVID_LEN+1];			//  业务代码
	char momsgid[MAX_MSGIDSTR_LEN+1];	//  消息ID
	char sa[MAX_SRCID_LEN+1];			//  源地址
	char da[MAX_SRCID_LEN+1];			//  目的地址
	char ec;							//	esm_class
	char pid;							//	protocol_id	
	char dc;							//	消息编码格式
	char sm[MAX_MSG_LEN*2+1];			//	消息内容
	int  nMsgLen;						//  消息长度
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

//发送RPT请求
struct RPT_REQ_PACK
{
	COMMAND  command;						//  操作命令
	char spid[MAX_SPID_LEN+1];				//	SP的ID	
	char sppassword[MAX_PWD_LEN+1];			//  密码()
	char spsc[MAX_SRVID_LEN+1];				//  业务代码
	char mtmsgid[MAX_MSGIDSTR_LEN+1];		//  MT消息ID
	char mtstat[MAX_ERSTAT_LEN+1];			//  MT消息状态
	char mterrcode[MAX_WBS_ERCODE_LEN+1];	//	MT错误代码
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

//交行模式MT请求包(不包括文件请求)
struct HSBC_MT_REQ_PACK
{
	int		nUID;						//企业在数据库中的惟一编号
	char	szUsrId[MAX_SPID_LEN+1];	//登入用的户的ID。例：910000
	char	szLoginId[MAX_SPID_LEN+1];	//代理帐号ID
	char	szPwd[MAX_PWD_LEN+1];		//密码。例：910000
	char	szIp[MAX_IPADDR_LEN+1];
	int		nPort;
	short	nConClose;					//用户的请求模式
	short	nReqMode;					//哪种模式soap/post/get
	short	nCMD;						//何种请求命令
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

//交行模式文件提交mt请求包
struct HSBC_BATCH_MT_REQ_PACK
{
	int		nUID;						//企业在数据库中的惟一编号
	char	szUsrId[MAX_SPID_LEN+1];	//登入用的户的ID。例：910000
	char	szLoginId[MAX_SPID_LEN+1];	//代理帐号ID
	char	szPwd[MAX_PWD_LEN+1];		//密码。例：910000
	char	szIp[MAX_IPADDR_LEN+1];
	int		nPort;
	short	nConClose;					//用户的请求模式
	short	nReqMode;					//哪种模式soap/post/get
	short	nCMD;						//何种请求命令
	int		nSockId;
	int		nSocket;
	INT64   nbmtmsgid;					//文件msgid
	INT64   nMsgIdTmp;					//做为该批任务的MSGID，不做其他功效
	char	cDownload;					//文件是否已被下载 0:未下载 1:已下载
	char	szLocalPath[MAX_PATH+1];	//本地文件路径
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

//交行接收上行/状态报告的服务器信息/滑动窗口信息
struct HSBC_URLINFO
{
	int  nEcid;                      //账号所属企业ID
	int  nUID;						//帐号的UID
	char szUsrId[MAX_SPID_LEN+1];	//帐号
	char szPwd[MAX_PWD_LEN+1];		//密码
	char szMoUsrId[MAX_MO_USERID_LEN + 1];	//推送MO帐号
	char szMoPwd[MAX_PWD_LEN + 1];		//推送MO密码
	char szRptUsrId[MAX_RPT_USERID_LEN + 1];	//推送RPT帐号
	char szRptPwd[MAX_PWD_LEN + 1];		//推送RPT密码
	char szIp[MAX_DOMAIN_LEN+1];	//绑定IP
	char szDomain[MAX_DOMAIN_LEN+1];//域名
	int  nPort;						//绑定端口
	char szWebPath[MAX_PATH+1];		//web目录
	char cRptFlag;					//rpt/mo标志 0:mo 1:rpt
	short nWndCnt;					//当前窗口大小，为0说明空闲，>0说明还有未完成的数据
	short nState;					//在线/离线，默认为在线 OFFLINE ONLINE 从数据库读出来时默认在线，变量声明也是默认在线
	time_t tOffline;				//发现离线的时间
	time_t tWndFlow;				//窗口大小溢出的时间
	int    nPushCnt;                //推送次数
    int    nPushFail;               //uid帐号的推送失败次数 递增 .EXEC WS_GETURLBINDINFOV1 userdata表
	unsigned int  nOfftime;            //离线总时间单位S

	int nPushVersion;              //推送版本
	int nPushmoversion;
	int nPushrptversion;
	int nPushtimefmt;

	int nPushMoFmt;                //nPushVersionv=48以上，协议3.0以上,nPushMoFmt才有效。pushmofmt : WebServiceTool-V5.3  = 4 URL:JSON, 2 JSON:JSON, 1 XML:XML 。
	int nPushRptFmt;
	int nPushMsgEncode;                //推送内容的加密方式
	int nPushCodeType;                 //推送内容的编码格式
	int nPushFailCnt;                  //v3.0账号自定义最大推送失败次数。  从数据库到内存，再赋值给PACK_DELIVER::nReSendCnt。
	int nPushSlideWnd;                 //推送滑动窗口的大小

	int nPushrptmaxcnt;
	int nPushmomaxcnt;

    int nHttpMoFlag;                     //0：http 1：https
    int nHttpRptFlag;                     //0：http 1：https
    char szMoUrl[MAX_URL_LEN];		//mourl
    char szRptUrl[MAX_URL_LEN];		//rpturl
    char pCACertName[MAX_CA_NAME + 1]; //CA证书套件名,从https://curl.haxx.se/docs/caextract.html获取最新文件。（必须为pem格式，因为libcurl只支持pem格式。样例："E:\\ZWF\\https_client\\test_https\\cert\\ca-bundle.pem")
    int iVerifyPeer; //是否校验对端：1:不对服务端证书身份合法性校验， 2：对服务端证书身份合法性校验
    int iVerifyHost; //是否对主机进行校验：1：表示不检查证书，2:表示检查证书中是否有CN(common name)字段, 3:表示在1的基础上校验当前的域名是否与CN匹配

	TINYINT				 nPushCntType;
	TINYINT				 nSignType;    //签名格式
	
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


//获取系统当前时间的自定义拓展结构体
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
//平台监控相关结构等
//////////////////////////////////////////////////////////////////////////
enum
{	
	TYPE_MON_USERINFO=130,				//用户帐号信息
		TYPE_MON_SPGATEINFO,				//通道帐号信息
		TYPE_MON_THREADINFO,				//系统线程工作情况
		TYPE_MON_BUFINFO,					//系统当前缓冲信息
		TYPE_MON_SYSINFO,					//系统资源占用信息
		TYPE_MON_LOGINFO,					//平台输出日志信息
		TYPE_MON_ENDINFOR,						//传输完毕标识
		TYPE_MON_EXCEPTION,						//突发异常信息
};

enum
{
	THRSTATUS_INIT=0,					//线程正在启动中(初始状态)
	THRSTATUS_IDLE=10,					//空闲
	THRSTATUS_BUSY=20,					//繁忙
	THRSTATUS_VERYBUSY=30,				//非常繁忙
	THRSTATUS_DEADLOOP=40,				//死循环
	THRSTATUS_NORESPONSE=50,			//长时间无响应
	THRSTATUS_ERROR=60,					//发生异常
	THRSTATUS_EXIT=70					//已退出
};

#define LOGTYPE_LOGIN_BASE		100			//登陆日志
enum
{
	LOG_LOGIN_SUCCESS=LOGTYPE_LOGIN_BASE,	//登陆成功
	LOG_LOGIN_FAILED						//登陆失败
};

#define LOGTYPE_DBOPT_BASE		500			//db操作日志
enum
{
	LOG_DB_WR_SUCCESS=LOGTYPE_DBOPT_BASE,		//写库成功
	LOG_DB_WR_FAILED,							//写库失败
	LOG_DB_RD_SUCCESS=LOGTYPE_DBOPT_BASE+10,	//读库成功
	LOG_DB_RD_FAILED,							//读库失败
	LOG_DB_UPD_SUCCESS=LOGTYPE_DBOPT_BASE+20,	//更新库成功
	LOG_DB_UPD_FAILED,							//更新库失败
	LOG_DB_DEL_SUCCESS=LOGTYPE_DBOPT_BASE+30,	//删除记录成功	
	LOG_DB_DEL_FAILED,							//删除记录失败
	LOG_DB_OPT_SUCCESS=LOGTYPE_DBOPT_BASE+40,	//操作库成功(通用)
	LOG_DB_OPT_FAILED							//操作库失败(通用)
};

#define LOGTYPE_SYSER_BASE		600			//系统内部异常
enum
{
	LOG_SYS_NORMAL=LOGTYPE_SYSER_BASE,		//系统正常
	LOG_SYS_ERROR,							//系统异常
	LOG_SYS_BUSSINESS_ERROR					//系统中某项业务异常
};

//监控信息分类保存
typedef std::map<int, DEQUE_MO> MAP_MONINFO;

//平台监控结构
//平台系统信息
struct MON_SYSINFO
{
    int nCpuUsage;					//CPU使用量，有必要时格式成XX%的字符串后显示
    unsigned int nMemUsage;				//物理内存使用量，以M为单位
    unsigned int nVmemUsage;			//虚拟内存使用量，以M为单位
	int nDiskFreeSpace;				//磁盘剩余量，以M为单位
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

//平台日志信息
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

//用户帐号监控信息
struct MON_USERINFO
{
	int		nLinkNum;						//帐号连接数
	char	cOnlineStatus;					//在线状态 0:在线 1:离线 2:未知
	int		nUserFee;						//帐号费用
	int		nSuccessRate;					//成功率
	int		nMoTotal;						//MO总量
	int		nRptTotal;						//RPT总量
	int		nMtTotalSnd;					//MT提交总量 客户接交MT的总数量
	int		nMtHaveSnd;						//MT已转发量 已成功转发的MT数量
	int		nMtRemained;					//MT滞留量 当前滞留平台未转发的MT数量
	int		nMtSndSpd;						//MT提交速度 客户提交MT的速度
	int		nMoHaveRecv;					//MO接收总量 客户当前收到的Mo总量 
	int		nMoRemained;					//MO滞留量 客户当前滞留在平台的Mo数量
	int		nMoRptRecvSpd;					//Mo/Rpt接收速度 客户接收Mo/Rpt的速度
	char	cMoTmOutCnt;					//接收MO超时次数 客户接收Mo时超过时的次数
	int		nRptHaveRecv;					//Rpt接收总量 客户当前收到的Rpt总量
	int		nRptRemained;					//Rpt滞留量	客户当前滞留在平台的Rpt数量
	char	cRptTmOutCnt;					//接收Rpt超时次数 客户接收Rpt时超过时的次数
	char    szUserId[MAX_SPID_LEN+1];       //登入用的户的ID。例：910000
	unsigned char ucJType;					//用户使用的接入方式及接入协议
	char	szIp[MAX_IPADDR_LEN+1];			//帐号登陆IP
	char	szLoginInTm[MAX_TIME_LEN+1];	//最后一次登陆时间	最后一次登陆的时间
	char	szLoginOutTm[MAX_TIME_LEN+1];   //最后一次离线时间	最后一次离线的时间
	char	szMtSndInfo[1024+1];			//以字符串的形式描述用户每个通道的发送量/滞留量

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

//通道监控信息
struct MON_SPGATEINFO
{
	int		nLinkNum;						//帐号连接数
	char	cOnlineStatus;					//在线状态 0:在线 1:离线 2:未知
	int		nMtTotal;						//MT总量
	int		nMtHaveSnd;						//MT接收总量 已向该通道道发送的MT数量
	int		nMtRemained;					//MT滞留量	当前平台滞留该通道的MT数量
	int		nMtRecvSpd;						//MT接收速度 该通道接收网关转发MT的速度
	int		nMoTotalRecv;					//MO接收总量 该通道收到的MO总量
	int		nMoHaveSnd;						//MO转发量	已转发给用户的该通道的MO数量
	int		nMoRemained;					//MO滞留量	该通道滞留的MO总数
	int		nMoSndSpd;						//MO转发速度 该通道向网关转发MO的速度
	int		nRptTotalRecv;					//Rpt接收总量 该通道收到的Rpt总量
	int		nRptHaveSnd;					//Rpt转发量	已转发给用户的该通道的Rpt数量
	int		nRptRemained;					//Rpt滞留量	该通道滞留的Rpt总数
	int		nRptSndSpd;						//Rpt转发速度 该通道向网关转发Rpt的速度
	int		nSndErCnt;						//通道发送异常次数
	unsigned char ucErType;					//异常类型,0：表示无异常 其他值：表示该通道当前处理何种异常状态
	char    szUserId[MAX_SPID_LEN+1];       //登入用的户的ID。例：910000
	char	szIp[MAX_IPADDR_LEN+1];			//帐号登陆IP
	char	szLastSndErTm[MAX_TIME_LEN+1];	//最后一次通道发送异常的时间
	char	szLoginInTm[MAX_TIME_LEN+1];	//最后一次登陆时间	最后一次登陆的时间
	char	szLoginOutTm[MAX_TIME_LEN+1];   //最后一次离线时间	最后一次离线的时间
	char	szMtSndInfo[1024+1];			//以字符串的形式描述用户每个通道的发送量/滞留量/各省发送量/各省的成功率

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

//系统缓存信息
struct MON_BUFINFO
{
	int		nEndCnt;				//后端连接数
	int		nMoTotalRecv;			//MO接收总量 后端转发至网关的MO总数
	int		nMtTotalSnd;			//MT转发总量 网关成功转发至后端的MT总数
	int		nWrMoBuf;				//MO写库缓冲 写MO_TASK表缓冲大小
	int		nUpdMoBuf;				//MO更新缓冲 更新MO_TASK表缓冲大小
	int		nUpdRptBuf;				//RPT更新缓冲 更新Mt_Task表中SendStatus字段
	int		nWrRptBuf;				//Rpt写库缓冲 写Rpt_wait_b表缓冲大小
	int		nEndRspBuf;				//后端回应缓冲 后端高级别回应缓冲
	char 	szMtSndBuf[256+1];		//MT发送缓冲 网关向后端转发MT的发送缓冲（0-9级）中间以/分隔，如0/1/2/3/4/5/6/7/8/9
	int		nMtSndBuf;				//MT发送缓冲 网关向后端转发MT的发送缓冲（0-9级）0-9级的总和
	int		nMtWaitBuf;				//MT发送等待缓冲 网关向后端转发MT的发送等待缓冲
	int		nPreCnt;				//前端连接数
	int		nMtTotalRecv;			//MT接收总量 接收到前端MT的总量
	int		nMoTotalSnd;			//MO转发总量 成功转发给前端用户的MO总量
	int		nWrMtTaskBuf;			//写MTTASK缓冲 写Mt_task表缓冲大小
	int		nWrMtTmBuf;				//写MTTMR缓冲 写Mt_timer_que表缓冲大小
	int		nWrMtVfyBuf;			//写MTVFY缓冲 写Mt_verify_wait表缓冲大小
	int		nWrMtLvlBuf;			//写MTLVL缓冲 写Mt_level_que表缓冲大小
	int		nPreRspBuf;				//前端回应缓冲 前端高级别回应缓冲
	int		nPreRspTmpBuf;			//前端回应临时缓冲 前端高级别回应临时缓冲
	int		nMoSndBuf;				//Mo发送缓冲 Mo发送缓冲
	int		nRptSndBuf;				//Rpt发送缓冲 Rpt发送缓冲
	int		nMoRptWaitBuf;			//MO/RPT发送等待缓冲 MO/RPT发送等待缓冲
	int		nLogFileNum;			//日志文件数量
	int		nLogBuf;				//日志缓冲
	int		nRecvBuf;				//接收缓冲
	int		nReSndBuf;				//重发缓冲
	int		nSuppSndBuf;			//补发缓冲
	int		nMonLogBuf;				//监控日志缓冲
	int		nMtRemained;			//MT滞留量
	int		nMoRemained;			//MO滞留量
	int		nRptRemained;			//RPT滞留量
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

//监控信息传输结束信息
typedef int MON_ENDINFO;

/*
//突发异常监控类型
enum MON_EXCEPTION_TYPE
{
	MON_EXCEPTION_UNKNOWN,
	MON_EXCEPTION_USER_FEE,	//用户欠费
	MON_EXCEPTION_FWD_ERR,//运营商断线
	MON_EXCEPTION_USER_OFFLINE,//用户离线
	MON_EXCEPTION_FWD_OFFLINE,//SPGATE离线
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
//突发异常监控类型载体
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

//突发异常监控类型
enum MON_EXCEPTION_TYPE
{
	MON_EXCEPTION_UNKNOWN,
	MON_EXCEPTION_USERFEE_INDEBT,	//用户欠费
	MON_EXCEPTION_USERFEE_THRESHOLD,	//低于阀值
	MON_EXCEPTION_FWD_ERR,//运营商断线
	MON_EXCEPTION_USER_OFFLINE,//用户离线
	MON_EXCEPTION_FWD_OFFLINE,//SPGATE离线
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
//突发异常监控类型载体
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
	int nUID;	 //数据库用户编号
	int nFee;	 //国内余额
	int nGlbFee; //国际余额,按条数
	double dGlbFee;    //国际余额,按余额
	int  nMonType;     //报警类型,10/19
	char nCalcFeeType; //计费类型 1:按条数，2:按金额
	char nFeeType;	   //费用报警类型，1：国内，2，国外，3，俩者
	char szUsrId[MAX_SPID_LEN+1]; //登入用的户的ID。例：910000
	char szInnerThreshold[64];    //国内余额告警阀值
	char szGlobahreshold[64];     //国外余额告警阀值


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
#define ALL_SUCCESS_RSP	2           //全成功
#define ALL_FAIL_RSP 3              //全失败
#define SUCCESS_FAIL_RSP 4          //部分成功，部分失败
#define DETAILS_RSP 5				//详情
#define DEFAULT_MORPT_RETSIZE1 100
#define DEFAULT_MO_MAX_RETSIZE 200
#define DEFAULT_RPT_MAX_RETSIZE 500
#define MAX_PUSHMO_CNT		200
#define MAX_PUSHRPT_CNT		500
#define MAX_CPUSHMO_CNT	    200       //用户自定义最大推送MO个数
#define MAX_CPUSHRPT_CNT	500       //用户自定义最大推送RPT个数

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
	int	 nEcid;                     //企业ID               
	char szFunType[FUNTYPE_LEN + 1];         //方法类型
	char szFunName[FUNMAE_LEN + 1];         //方法名
	char szCFunName[CFUNMAE_LEN + 1];         //方法名
	TINYINT  nCmdType;                  //请求类型
	TINYINT  nRetMode;                  //返回模式 0未知，1xml，2json
	char szMArgName[MARGNAME_LEN + 1];        //梦网字段名
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
	char szCArgName[CARGNAME_LEN + 1];		//客户字段名
	char szBelong[BELONG_LEN + 1];			//节点关系
	TINYINT  nBelongtype;					//节点类型
	TINYINT  nCargType;                     //客户参数类型

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
typedef std::map<std::string, WBS_TEMPLATE_INFO> FILEDMAP;  //梦网字段与客户字段映射结构

//固定字段值结构
struct FIX_FILED
{
	char szCArgValue[CARGVALUE_LEN+1];	 //客户固定字段值
	char szCArgName[CARGNAME_LEN+1];					//客户字段名
	char szBelong[BELONG_LEN + 1];
	TINYINT nBelongtype;    //节点类型, 有无子节点

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

//字段结构
struct ALLFILED
{
	FILEDMAP filedmap;
	std::vector<FIX_FILED> vecFiled;
};

typedef std::map<int/*命令模式*/, ALLFILED> WBSTEMPLATEINFO; //命令模式下的映射结构
struct WBSTEMPLATE  //模板结构
{
	TINYINT retMode;           //返回模式 0未知，1xml，2json
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

typedef std::map<std::string/*请求路径*/, WBSTEMPLATE/*模板*/> PATHTMPLINKMAP;

typedef std::map<int/*ECID*/, WBSTEMPLATE/*模板*/> PUSHTEMPLATE;

//基础参数表
typedef  std::vector<std::string/*梦网基础参数*/>  M_BASE_ARG_PARAMS;
typedef std::map <std::string/*接口single_send*/, M_BASE_ARG_PARAMS> M_BASE_ARG;

//客户方法名与梦网方法名映射表
typedef std::map <std::string/*FUNTYPE/CFUNNAME*/, std::string/*FUNTYPE/FUNNAME*/> CM_NAME;

struct RSP_PUSH_PACK
{
	char szCArgName[CARGNAME_LEN+1];					//客户字段名
	char szCArgValue[CARGVALUE_LEN+1];	                //客户参数字段值

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

typedef std::map<TINYINT/*回应状态2345*/, RSP_PUSH_PACK> RSP_CMD_PUSH_PACK;

struct RSP_CMD_PUSH_TMP  //模板结构
{
	TINYINT retMode;           //返回模式 0未知，1xml，2json
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
//mos结构体
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

//rpts结构体
typedef struct rptsbody
{
	INT64 nMsgId;
	char szCustId[64 + 1];				  //对应下行请求时填写的custid
	int nPkNum;							  //当前条数
	int nPkTotal;						  //总条数
	char szMobile[22 + 1];
	char szSpNo[21 + 1];				  //完整的通道号
	char szExNo[22 + 1];				  //下行时填写的exno
	char szRetTime[20];                   //状态报告返回时间   YYYY-MM-DD HH:MM:SS
	int  nMtStat;						  //接收状态， 0:成功  非0:失败
	char MtErrCode[7 + 1];				  //状态报告错误代码, eg：DELIVRD
	char ExData[64 + 1];				  //下行时填写的exdata

}RPTSBODY_DATA;


#define MAX_SENDMT_CNT	    1000

#define FILED_MT_CNT    6    //MT字段个数

#define FILED_VER_LEN   25   //字段名平均长度

#define FILED_MO_CNT    6    //MOS字段个数

#define FILED_RPT_CNT   12   //RPTS字段个数

#define MAX_HTTP_HEAD   1024

#define MAX_PUSHRPT_LEN  (MAX_CPUSHRPT_CNT * (sizeof(RPTSBODY_DATA)+FILED_VER_LEN*FILED_RPT_CNT) + MAX_HTTP_HEAD)

#define MAX_PUSHMO_LEN  (MAX_CPUSHMO_CNT * (sizeof(MOSBODY_DATA)+FILED_VER_LEN*FILED_MO_CNT) + MAX_HTTP_HEAD)

#define MAX_MUTI_LEN   (MAX_SENDMT_CNT * (sizeof(MULTIXMSGV5) + FILED_VER_LEN*FILED_MT_CNT))

#define  MAX_PUSH_PRE_LEN     1024       //推送mo和rpt前半部分的最大长度

enum CONFIG_TYPE_USERID
{
    GOME_USERID = 1, //国美在线账号

    OTHER_USERID = 255
};



struct PT_INFO
{
	char szPtCode[10 + 1];	//当前平台的平台编号
	int  nPtCode;			//当前平台的平台编号
	std::map<int, std::string> AllPt;//所有平台的平台编号对应关系
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
//携号转网相关结构
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
//特殊手机号路由相关结构
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

//特殊手机号码的属性
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
	int		nEcId;				//企业ID
	TINYINT nProType;			//产品类型 1短信模板，6闪信  11富信
	int		nParamsNum;			//参数个数
	char	szTmplContent[MAX_TMPLCONTENT_LEN+1];	//模板内容
	TINYINT TmplStatus;         //zhubo 2018-02-05  0启用  非0禁用
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
typedef std::map<INT64/*模板ID*/, WBS_SENDTMPL> WBS_SENDTMPL_ID;


//*****************zhubo 2017-5.24


//运营商<--->BIND_INFO_VECTOR
typedef std::map<int, BIND_INFO_VECTOR> MAPMOBILE_GATE_GROUP;

//UID---MAPGGUID_MOBILE
typedef std::map<int, MAPMOBILE_GATE_GROUP> MAPGGUID_MOBILE;

//通道组ID<--->MAPGGUID_MOBILE
typedef std::map<int, MAPGGUID_MOBILE> MAPGATE_GROUP;

//UID<--->通道组ID
typedef std::map<int, int> MAPUID_GATEGROUPID;


//运营商<--->V_SPGATE_INFO
typedef std::map<int, V_SPGATE_INFO> MOBILE_SPGATE_MAP;

//UID<--->MOBILE_SPGATE_MAP
typedef std::map<int, MOBILE_SPGATE_MAP> UID_MOBILE_SPGATE_MAP;

//通道组ID<--->MOBILE_SPGATE_MAP
typedef std::map<int, UID_MOBILE_SPGATE_MAP> GATEID_VSPGATE_MAP;

//*****************zhubo 2017-5.24

#ifdef DBPARAMS
#undef DBPARAMS
#else
#define DBPARAMS DB_PARAMS
#endif
struct DBPARAMS
{
    std::string strHost;                //数据库地址
    std::string strUser;                //数据库用户名
    std::string strPwd;                 //数据库密码
    std::string strDBName;              //数据库名称
    std::string strConStr;				//数据库连接字符串
    int             nPort;              //数据库端口
    int             nDBType;            //数据库类型
    int         nDBMinSize;             //数据库连接池最小连接数
    int         nDBMaxSize;             //数据库连接池最大连接数
    int             CommandTimeout;     //执行超时时长

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

#define SURL_URIREQ						"geturi" //请求URI
#define SURL_URIREQRSP	                "geturirsp" //请求URI应答
#define SURL_ACTIVE_TEST                 "active"   //激活测试
#define SURL_ACTIVE_TEST_RESP            "activersp" //激活测试应答
#define SURL_SET_LONG_ADDR_REQ          "setlastatus" //设置长地址状态
#define SURL_SET_LONG_ADDR_RSP          "setlastatusrsp" //设置长地址状态回应


#define MINFO_CONNECT	 "pqlogin"
#define MINFO_CONNECT_RESP "pqloginrsp"
#define MINFO_ACTIVE_TEST                 "pqactive"   //激活测试
#define MINFO_ACTIVE_TEST_RESP            "pqactiversp" //激活测试应答
#define MINFO_INFOREQ						"pqgetms" //请求手机号码信息
#define MINFO_INFOREQRSP	                "pqgetmsrsp" //请求手机号码信息应答

#define MRMS_VERSION_V1                 0x01
#define MRMS_VERSION_V2                 0x02
#define MRMS_VERSION_V3                 0x03


#define MRMS_CONNECT                    0x00000001 //请求连接
#define MRMS_CONNECT_RESP               0x80000001 //请求连接应答
#define MRMS_LOGIN	                    0x00000002 //请求登录
#define MRMS_LOGIN_RESP	                0x80000002 //请求登录应答
#define MRMS_SUBMIT                     0x00000004 //提交短信
#define MRMS_SUBMIT_RESP                0x80000004 //提交短信应答
#define MRMS_DELIVER                    0x00000005 //短信下发
#define MRMS_DELIVER_RESP               0x80000005 //下发短信应答
#define MRMS_ACTIVE_TEST                0x00000006 //激活测试
#define MRMS_ACTIVE_TEST_RESP           0x80000006 //激活测试应答
#define MRMS_CMDTOURI					0x00000007 //给uri发送指令
#define MRMS_CMDTOURI_RESP				0x80000007 //uri发送指令应答

struct RMS_LINK_INFO
{
	int             nNodeId;            //节点号
	int				nMobileType;		//运营商
	int				nArea;				//区号
	UINT            nPort;              //端口号
	char            szIP[MAX_IPADDR_LEN + 1];
	char            szDown[25];			//下载IP或域名
	int				nNetUpWidth;		//上传物理带宽
	int				nNetDwWidth;		//下载物理带宽

	RMS_LINK_INFO()
	{
		memset(this, 0, sizeof(RMS_LINK_INFO));
	}
};

struct RMS_NODE_NETWIDTH_INFO
{
	int             nNodeId;            //节点号
	int				nError;				//错误码
	int				nOnline;			//在线状态
	int				nNetUpWidth;		//上传物理带宽
	int				nNetDwWidth;		//下载物理带宽
	time_t			tLastActiveTime;	//最后更新时间

	RMS_NODE_NETWIDTH_INFO()
	{
		memset(this, 0, sizeof(RMS_NODE_NETWIDTH_INFO));
		time(&tLastActiveTime);
	}
};

struct RMS_WIND_INFO
{
	int             nNodeId;            //节点号
	int				nWindowCnt;			//滑动窗口

	RMS_WIND_INFO()
	{
		memset(this, 0, sizeof(RMS_WIND_INFO));
	}
};

typedef std::map<int, RMS_LINK_INFO> RMSNODE_MAP_LINKINFO; //节点号为主键
typedef std::map<int, RMS_WIND_INFO> RMSNODE_MAP_WINDINFO; //节点号为主键
typedef std::map<int, RMS_NODE_NETWIDTH_INFO> RMSNODE_MAP_NETWIDINFO; //节点号为主键
typedef std::map<INT64, std::string> MAPRMSPACK_MSGIDSTRING;



//////////////////////////////////////////////////////////////////////////
struct MWRMS_LINKNODE
{
	int nNodeNum;//节点编号 注：节点编号唯一
	int nPort; //端口
	char szIp[MAX_IPADDR_LEN + 1]; //ip  注：MAX_IPADDR_LEN=18  

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
	int 	    nECID;//企业ID
	char	    chUserID[MAX_SPID_LEN+1];//用户ID
	TINYINT     nURILen;
	TINYINT     nBindStatus;//绑定状态
	//int         nDomainID;//域名ID
	char        chDomain[256];//域名
	INT64       nPrivilege;//权限
	TINYINT     nDomainStatus;//域名状态
	TINYINT     nURIMaxLen;//URI最大长度
	int 	    nValidDay;//有效天数
	//char        chDes[1024];//域名描述

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

