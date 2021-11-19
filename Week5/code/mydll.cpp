#include <windows.h>
#include <stdio.h>
#include <iostream>
using namespace std;

void MaliciousFt(){
    cout << "DLL injection Done!";
}

INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved)
{
	if(Reason == DLL_PROCESS_ATTACH)
        MaliciousFt();

	return 1;
}