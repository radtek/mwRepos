#include "utility/common/mwcorepch.h"

#ifdef MW_WIN
BOOL APIENTRY DllMain(HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#else
int main(char * args){
    return 0;
}
#endif

#include "utility/common/export.h"
#include "utility/json/export.h"

void ExportLib()
{
	if (0)
	{
		UTI::COM::ExportLib();
        Json::ExportLib();
	}
}
