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

//��������
#define ER_PARAM_ERROR	1	//��������
//�����ڵĺŶ�
#define ER_SEG_NOT_EXIST	2	//�����ڵĺŶ�

//�ļ������ڵĺŶ�
#define ER_FILE_NOT_EXIST	3	//�ļ������ڵĺŶ�


//������ѯ������������API���������Ժ���ǰ7λ����У��
//������pMobile����Ρ�����Ҫ��ѯ�ĺ��룬
//������pArea�����Ρ������ؽӿ�0ʱ���û������������������Ϣ������32�ֽ�
//����ֵ��0 ��ѯ��������
//����ֵ��1 ��������
//����ֵ��2 ��ѯ����������
API_PREFIX int WINAPI GetMobiArea( const char* Mobile, char* Area );




//������ɺŶ���
#define MAX_AREA_NUM 64
//�Ŷ�����������󻺳�
#define MAX_AREA_NAME_LEN 32
//�Ŷ��������
#define MAX_MOBI_SEG_NUM 1000000

//�Ŷβ�ѯ��
class CMobiAreaQry
{
public:
	// 
	//�������Ĭ��[bLoad=true]ʱ�������ʱ���ļ����غŶΡ�������Ϣ,���򲻼���
	// 
	CMobiAreaQry(bool bLoad = true);

	//
	//����
	//
	~CMobiAreaQry();

	// 
	//��ѯ���������أ�����ǰ7λ������Ч���ж�
	// 
	int QryMobiArea( const char* pMobile, char* pArea );

protected:				
	
	//���ص��������
	int m_iAreaCnt;	

	//���صĺŶθ���(�ظ���Ҳ����)
	int m_iLoadSegCnt;	

	//����ʱʧ�ܺŶ���Ϣ������
	int m_iFailLines;

	//���ص�������������,���ɽ���64�����������ƽ�����16������֮��
	char m_szAreaName[MAX_AREA_NUM][MAX_AREA_NAME_LEN];

	//�Ŷ�ֵ����������������
	//�±�Ϊ�����ֻ�����[1nnnnnn1234]����λ����nnnnnn
	//��ӦֵΪ�Ŷ����������������һά�±�ֵ��һ(0��ʾ�Ŷ���������δ֪)
	unsigned char m_arrSeg2Area[MAX_MOBI_SEG_NUM];	

	
private:

	//
	//AddAreaSegLine���Ѹ�ʽ���� "1nnnnnn,������" һ�кŶ�������Ϣ���ص��ڴ�
	//pLine��Դ�Ŷ���������
	//�ɹ����� 0��~0Ϊ���ʧ��
	//
	int AddAreaSegLine(char* pLine);


	//
	//pAreaName����ȡ����pAreaName�������Ѽ������������е��±��һ,�����ڲ��Ҳ����ʱ��������
	//pAreaName����������
	//����ֵ>0��Ӧ�����±��һ�� ����ֵ������̫�࣬���
	//
	int GetAreaIndex( const char* pAreaName );	
	
	
public:	
	//
	//LoadMobiSegAreaFromFile���Ӳ���[pFile]ָ���ļ����غŶ�������Ϣ���ڴ�
	//pFile���ļ�
	//����ֵ��
	//
	int LoadMobiSegAreaFromFile(const char* pFile);	

	//�Ӳ���ָ������ƺŶ�������Ϣ���Լ�
	void CopyFrom( const CMobiAreaQry *pObj );

	//
	//WriteLogMsg��д��־����
	//pLogMsg����־������
	//iSize����־����
	//����ֵ:��
	//
	void WriteLogMsg(const char* pLogMsg, int iSize=0);

};



