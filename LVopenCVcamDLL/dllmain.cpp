// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"

BOOL APIENTRY DllMain( HMODULE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//OutputDebugStringA("OCV_DLL_PROCESS_ATTACH");
		break;
	case DLL_THREAD_ATTACH:
		//OutputDebugStringA("OCV_DLL_THREAD_ATTACH");
		break;
	case DLL_THREAD_DETACH:
		//OutputDebugStringA("OCV_DLL_THREAD_DETACH");
		break;
	case DLL_PROCESS_DETACH:
		//OutputDebugStringA("OCV_DLL_PROCESS_DETACH");
		break;
	}
	return TRUE;
}

