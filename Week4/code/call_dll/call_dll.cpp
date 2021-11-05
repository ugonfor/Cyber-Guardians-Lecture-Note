#include <iostream>
#include "stdafx.h"

typedef int(*pDLLFunction)(int, int);
pDLLFunction pFunction = NULL;

int main()
{
    int a = 5;
    int b = 10;
    HMODULE hMod = NULL;
    hMod = LoadLibraryA("my_dll.dll");
    if (hMod == NULL)
    {
        printf("DLL Load Failed.\n");
        return 0;
    }
    pFunction = (pDLLFunction)GetProcAddress(hMod, "AddInt");
    printf("sum = %d \n", pFunction(a, b));
    pFunction = (pDLLFunction)GetProcAddress(hMod, "MultInt");
    printf("mult = %d \n", pFunction(a, b));

}