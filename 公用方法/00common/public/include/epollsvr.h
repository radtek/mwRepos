
#ifndef __SOCKET_H_EPOLLSVR__
#define __SOCKET_H_EPOLLSVR__

#include "nettypes.h"
#include "../../../00common/public/common/adapter/Adapter.h"



// #ifndef MAX_RECBUF_LEN
// #define MAX_RECBUF_LEN  32*1024     //最大接收缓冲(三个单包大小)
// #endif
// 
// #ifndef BUFFER_SIZE
// #define BUFFER_SIZE     8*1024      //完成端口一次接收的大小
// #endif
// 
// #ifndef MAX_PACK_LEN
// #define MAX_PACK_LEN    16*1024     //一个单包的最大内存值
// #endif


#ifndef MAX_LINGER
#define MAX_LINGER      10          //socket linger
#endif

#ifndef LISTENQUEUE
#define LISTENQUEUE     10000         //监听队列
#endif

#ifndef MAX_EVENTS
#define MAX_EVENTS      LISTENQUEUE+1         //最大事件数
#endif

/************************************************************************/
/* 功能：EPOLL SERVER 管理类                                            */
/************************************************************************/
class CEpollsvr
{
public:
	CEpollsvr();
	virtual ~CEpollsvr();

public:
	/************************************************************************/
	/*	函数功能：设置服务参数    											*/
	/*	参数说明：nMaxConNum	最大连接数									*/
	/*			  nUnlinkTime	空连接超时时间								*/
	/*			  nCheckFreq	空连接检测频率								*/
	/*			  nThrCnt		完成端口线程数								*/
	/*			  bShowLogs		是否显示日志								*/
	/*	函数返回：															*/
	/************************************************************************/
	virtual void SetParams(int nMaxConNum, int nUnlinkTime, int nCheckFreq, int nThrCnt, bool bShowLogs=false)=0;

	/************************************************************************/
	/*	函数功能：初始化并启动.										
		参数说明：
				  nPort				端口号		
				  pInvokePtr		调用者指针			
				  lpCallBackFun		数据接收回调函数							
		函数返回：
				  true	成功
				  false	失败												*/
	/************************************************************************/
	virtual bool Init(int nPort, void* pInvokePtr, DEALRECVCALLBACK lpCallBackFun)=0;



	/************************************************************************/
	/*	函数功能：异步发送函数 主键为KEY									
		参数说明：
				  pKey		KEY
				  buf		发送数据
				  len		发送数据长度
				  nState	状态
		函数返回：
				  大于0			成功
				  小于等于0		失败										*/
	/************************************************************************/
	virtual int  SendEpoll(epolldata_key *pKey, const void *buf, size_t len, int nState=(int)SOCK_STATE_CONNECTED)=0;


	/************************************************************************/
	/*	函数功能：异步发送函数 主键为sid&&s									
		参数说明：
				  sid		SOCKET ID
				  s			SOCKET
				  buf		发送数据
				  len		发送数据长度
				  nState	状态
		函数返回：
				  大于0			成功
				  小于等于0		失败										*/
	/************************************************************************/
	virtual int  SendEpoll(int sid, int s, const void *buf, size_t len, int nState=(int)SOCK_STATE_CONNECTED)=0;


	/************************************************************************/
	/*	函数功能：关闭服务器									
		参数说明：							
		函数返回：															*/
	/************************************************************************/
	virtual void Close()=0;

	/************************************************************************/
	/*	函数功能：更新KEY INFO									
		参数说明：
				pKey			KEY
				nSockId			SOCKET编号
				nUID			流水号
				nSockType				
				nUserPrivilege	自编流水号权限 0-无，非0-有	
				ucProtocol
				nVer
				szSpGate
				szSrcAddr
		函数返回：
				0		成功
				1		失败												*/
	/************************************************************************/
	virtual int  UpdateKeyInfo(
		epolldata_key* pKey,
		int nSockId,
		int nUID,
		int nSockType,
		int nUserPrivilege,
		unsigned char ucProtocol,
		unsigned int nVer,
// 		const char *szSpGate,
		const unsigned char *szSrcAddr
		)=0;


	/************************************************************************/
	/*	函数功能：取SOCKET状态									
		参数说明：
				  nSockId			SOCKET编号
		函数返回：SOCKET状态												*/
	/************************************************************************/
	virtual int  GetSockState(int nSockId)=0;


	/************************************************************************/
	/*	函数功能：关闭SOCKET									
		参数说明：
				  sock			SOCKET
		函数返回：															*/
	/************************************************************************/
	virtual void CloseSocketEx(SOCKET sock)=0;


	//删除SOCKET ID
	virtual void DeleteSockId(int nSockId)=0;

	//查找SOCKET ID
	virtual bool FindSockId(int nSockId)=0;

	//当前SOCKET ID 数量
	virtual int  GetSockIdNum()=0;

	//当前SOCKET 数量
	virtual int  GetAcceptConNum() const=0;

	//初始化连接数，置为0
	virtual void ResetAcceptConNum()=0;


	/************************************************************************/
	/*	函数功能：获取KEY信息									
		参数说明：
				  nBusy	[out]	使用中KEY结点数		 
				  nIdle	[out]	空闲KEY结点数
		函数返回：															*/
	/************************************************************************/
	virtual void ReportKeyInfo(int& nBusy, int& nIdle)=0;


	/************************************************************************/
	/*	函数功能：获取IO信息									
		参数说明：
				  nBusy	[out]	使用中IO结点数		 
				  nIdle	[out]	空闲IO结点数
		函数返回：															*/
	/************************************************************************/
	virtual void ReportIoInfo(int& nBusy, int& nIdle)=0;



	/************************************************************************/
	/*	函数功能：回收KEY									
		参数说明：
				  pKey	KEY
		函数返回：															*/
	/************************************************************************/
	virtual void RecycleKey(epolldata_key* pKey)=0;


	//添加socketId
	virtual void AddSockId(int nSockId)=0;

	/************************************************************************/
	/*	函数功能：启动延时    											    */
	/*	参数说明：nSocket	    需要延时的socket					    	*/
	/*			  delay     	延时时间							        */
	/*	函数返回：无													    */
	/************************************************************************/
	virtual void                NotifySocketSetDelay(int nSocketID, int delay) = 0;

	/************************************************************************/
	/*	函数功能：取消延时    											    */
	/*	参数说明：nSocket	    需要取消延时的socket					    */
	/*	函数返回：无													    */
	/************************************************************************/
	virtual void                NotifySocketCancelDelay(int nSocketID) = 0;

};


/************************************************************************/
/* 创建EPOLL SERVER 管理类												*/
/************************************************************************/
class CEpollsvrFactory
{
public:
	CEpollsvrFactory();
	virtual ~CEpollsvrFactory();

public:
	static CEpollsvr* New();				//创建类对象
	static void Destroy(CEpollsvr* p);		//释放资源
};

#endif
