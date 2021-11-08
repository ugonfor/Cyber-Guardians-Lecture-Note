// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

extern "C" __declspec(dllexport)int AddInt(int a, int b) {
	return a + b;
}

extern "C" __declspec(dllexport)int MultInt(int a, int b) {
	return a * b;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
    case DLL_PROCESS_ATTACH: {
        MessageBox(nullptr, L"injection success", L"dll injection", MB_OK);

    }
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

