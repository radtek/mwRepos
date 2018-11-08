#if (defined  MW_WINDOWS) || (defined  WIN32)

#define API_PREFIX 
#ifdef _LIB
#undef WINAPI 
#define WINAPI 
#else
#endif


#else

#define API_PREFIX extern "C"
#define WINAPI 

#endif

//参数错误
#define ER_PARAM_ERROR	1	//参数错误
//不存在的号段
#define ER_SEG_NOT_EXIST	2	//不存在的号段

//文件不存在的号段
#define ER_FILE_NOT_EXIST	3	//文件不存在的号段


//导出查询号码所属区域API，函数近对号码前7位进行校验
//参数：pMobile【入参】：需要查询的号码，
//参数：pArea【出参】：返回接口0时，该缓冲区填充所属区域信息，至少32字节
//返回值：0 查询到所属地
//返回值：1 参数有误
//返回值：2 查询不到所属地
API_PREFIX int WINAPI GetMobiArea( const char* Mobile, char* Area );




//最大容纳号段数
#define MAX_AREA_NUM 64
//号段区域名称最大缓冲
#define MAX_AREA_NAME_LEN 32
//号段最大数量
#define MAX_MOBI_SEG_NUM 1000000

//号段查询类
class CMobiAreaQry
{
public:
	// 
	//构造对象，默认[bLoad=true]时构造对象时从文件加载号段、区域信息,否则不加载
	// 
	CMobiAreaQry(bool bLoad = true);

	//
	//析构
	//
	~CMobiAreaQry();

	// 
	//查询号码所属地，仅对前7位进行有效性判定
	// 
	int QryMobiArea( const char* pMobile, char* pArea );

protected:				
	
	//加载的区域个数
	int m_iAreaCnt;	

	//加载的号段个数(重复的也计数)
	int m_iLoadSegCnt;	

	//加载时失败号段信息的行数
	int m_iFailLines;

	//加载的区域名称数组,最多可接受64个地区，名称仅接受16个汉字之内
	char m_szAreaName[MAX_AREA_NUM][MAX_AREA_NAME_LEN];

	//号段值到地区的索引数组
	//下标为形如手机号码[1nnnnnn1234]中六位数字nnnnnn
	//对应值为号段所属地区在数组的一维下标值加一(0表示号段所属区域未知)
	unsigned char m_arrSeg2Area[MAX_MOBI_SEG_NUM];	

	
private:

	//
	//AddAreaSegLine：把格式形如 "1nnnnnn,北京等" 一行号段区域信息加载到内存
	//pLine：源号段所属地行
	//成功返回 0，~0为添加失败
	//
	int AddAreaSegLine(char* pLine);


	//
	//pAreaName：获取参数pAreaName区域在已加载区域数组中的下表加一,不存在并且不溢出时增加区域
	//pAreaName：区域名称
	//返回值>0对应区域下标加一， 其他值：区域太多，溢出
	//
	int GetAreaIndex( const char* pAreaName );	
	
	
public:	
	//
	//LoadMobiSegAreaFromFile：从参数[pFile]指向文件加载号段区域信息到内存
	//pFile：文件
	//返回值：
	//
	int LoadMobiSegAreaFromFile(const char* pFile);	

	//从参数指向对象复制号段区域信息到自己
	void CopyFrom( const CMobiAreaQry *pObj );

	//
	//WriteLogMsg：写日志函数
	//pLogMsg：日志缓冲区
	//iSize：日志长度
	//返回值:无
	//
	void WriteLogMsg(const char* pLogMsg, int iSize=0);

};



