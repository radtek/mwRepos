// WBSDLL.cpp : ���� DLL Ӧ�ó���ĵ���������
//


#include "WBSDLL.h"
#include "CParseToolImpl.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//�ɹ�0 ʧ�ܷ��������������ظ�ֵ
CParseTool::CParseTool()
{

}

CParseTool::~CParseTool()
{

}

int CParseTool::MakeMoRequestV5_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, string &strMo)
{

	return -1;
}

int CParseTool::MakeMoRequestV5_cstd(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strMo)
{
	return -1;
}

int CParseTool::MakeMoRequestV5_std(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, string &strMo)
{
	return -1;
}

int CParseTool::MakeMoRequestV5_cstd(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strMo)
{
	return -1;
}

int CParseTool::MakeRptRequestV5_cstd(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strRpt)
{
	return -1;
}

int CParseTool::MakeRptRequestV5_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, const WBS_RPTERRCODEMAP& errcodemap, string &strRpt)
{
	return -1;
}

int CParseTool::MakeRptRequestV5_cstd(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strRpt)
{
	return -1;
}

int CParseTool::MakeRptRequestV5_std(const PACK_DELIVER &MoRptPack, const HSBC_URLINFO& urlinfo, const WBS_RPTERRCODEMAP& errcodemap, string &strRpt)
{
	return -1;
}

void CParseTool::ParseResponse_std(const char* pStart, RSP_PACK &rsp)
{
	return;
}

void CParseTool::ParseResponse_cstd(const char* pStart, const WBSTEMPLATEINFO &mapTmp, const RSP_CMD_PUSH_TMP &pushRsp, RSP_PACK &rsp, std::vector<INT64> &vSuccess, std::vector<INT64> &vFailed)
{
	return;
}

int CParseTool::MakeMoRequestV32_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, string &strMo)
{
	return -1;
}

int CParseTool::MakeRptRequestV32_std(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, string &strRpt)
{
	return -1;
}

int CParseTool::MakeMoRequestV32_cstd(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strMo)
{
	return -1;
}

int CParseTool::MakeRptRequestV32_cstd(const std::vector<PACK_DELIVER> &vMoRptPack, const HSBC_URLINFO& urlinfo, int nRetMode, const ALLFILED &allFiled, string &strRpt)
{
	return -1;
}

void CParseTool::LoadConfig(int nType)
{
	return;
}

//�½�һ������
CParseTool* CParseToolFactory::New()
{
	return  new CParseToolImpl();
}

//����һ������
void CParseToolFactory::Destroy(CParseTool* pbase)
{
	if (NULL == pbase)
	{
		return;
	}
	else
	{
		delete pbase;
		pbase = NULL;
	}

	return;
}

