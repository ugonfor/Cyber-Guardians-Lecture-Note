#include <windows.h>
#include <stdio.h>
#include <iostream>
using namespace std;

DWORD WINAPI MaliciousFt(LPVOID lParam){
    cout << "DLL injection Done!";
    
	LPVOID TargetAddr = (LPVOID)0x10014db;
	DWORD OldProtect = 0;

	// 0x10014db 부터 9바이트 -> 권한 PAGE_READWRITE
	VirtualProtect(TargetAddr, 9, PAGE_READWRITE, &OldProtect);

    // patch to [6A 0A E8 9E 1A 00 00 31 C0]
	*((LPBYTE)TargetAddr + 0) = 0x6A;
	*((LPBYTE)TargetAddr + 1) = 0x0A;
	*((LPBYTE)TargetAddr + 2) = 0xE8;
	*((LPBYTE)TargetAddr + 3) = 0x9E;
	*((LPBYTE)TargetAddr + 4) = 0x1A;
	*((LPBYTE)TargetAddr + 5) = 0x00;
	*((LPBYTE)TargetAddr + 6) = 0x00;
	*((LPBYTE)TargetAddr + 7) = 0x31;
	*((LPBYTE)TargetAddr + 8) = 0xC0;
    
    return 0;
}

INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved)
{
    HANDLE hThread = NULL;
	if(Reason == DLL_PROCESS_ATTACH){
        hThread = CreateThread(NULL, 0, MaliciousFt, NULL, 0, NULL);
        CloseHandle(hThread);
    }

	return 1;
}