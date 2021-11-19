#include <Windows.h>
#include <TlHelp32.h>
#include <stdio.h>

#include <stdint.h>
#include <iostream>
#include <string>
#include <tchar.h>

using namespace std;

void printError( TCHAR* msg )
{
  DWORD eNum;
  TCHAR sysMsg[256];
  TCHAR* p;

  eNum = GetLastError( );
  FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL, eNum,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         sysMsg, 256, NULL );

  // Trim the end of the line and terminate it with a null
  p = sysMsg;
  while( ( *p > 31 ) || ( *p == 9 ) )
    ++p;
  do { *p-- = 0; } while( ( p >= sysMsg ) &&
                          ( ( *p == '.' ) || ( *p < 33 ) ) );

  // Display the message
  _tprintf( TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, eNum, sysMsg );
}

int main(int argc, char const *argv[])
{
    if(argc != 3){
        printf("usage: %s <dll name> <exe name> <is64>\n", argv[0]);
    }

    string dll = string(argv[1]);
    string exe = string(argv[2]);

    cout << "DLL : " << dll << "\n";
    cout << "EXE : " << exe << "\n";


    HANDLE hProcessSnap;
    HANDLE hprocess = 0;
    PROCESSENTRY32 PE32;
    
    // from https://docs.microsoft.com/ko-kr/windows/win32/toolhelp/taking-a-snapshot-and-viewing-processes

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
    if (hProcessSnap == INVALID_HANDLE_VALUE) return 1;

    // Set the size of the structure before using it.
    PE32.dwSize = sizeof( PROCESSENTRY32 );

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if( !Process32First( hProcessSnap, &PE32 ) )
    {
        printf( "Process32First" ); // show cause of failure
        CloseHandle( hProcessSnap );          // clean the snapshot object
        return 1;
    }


    do
    {
        if(strcmp(PE32.szExeFile, exe.c_str()) == 0) {
			hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PE32.th32ProcessID);
            break;
        }
    } while (Process32Next(hProcessSnap, &PE32));
    CloseHandle(hProcessSnap);

    if(hprocess == 0){
        cout << exe << " is not on process\n";
        return 1;
    }

    // dll injection
    
	/* INJECT DLL */
	LPVOID LoadLibraryAddr = (LPVOID) GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
	LPVOID BaseAddr = (LPVOID) VirtualAllocEx(hprocess, 0, dll.length(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    if(BaseAddr == 0){
        cout << "Base Address Error\n";
        return 1;
    }

    cout << "Base Addr : " << BaseAddr << "\n";
    cout << "Alloc Addr : " << LoadLibraryAddr << "\n";

    if(WriteProcessMemory(hprocess, BaseAddr, dll.c_str(), dll.length(), 0) == 0){
        cout << "Write Memory Error" << "\n";
    }

    printf("BaseAddr : %p\n",BaseAddr);

	HANDLE hthread = CreateRemoteThread(hprocess, 0, 0, (LPTHREAD_START_ROUTINE) LoadLibraryAddr, BaseAddr, 0, 0);
	if(!hthread)
	{
        //printError(TEXT("CreateRemoteThread Error\n"));
        printf("CreateRemoteThread Error\n");
        printf("Maybe... compile this binary in 64 bit process / 32 bit process\n");
		CloseHandle(hprocess);
		return 1;
	}
    cout << "Done!";

	CloseHandle(hprocess);

    return 0;
}
