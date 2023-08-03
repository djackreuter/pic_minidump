#include "structs.h"

#define DEREF_32( name )*(DWORD *)(name)

int my_strcmp (const char *p1, const char *p2) {
    const unsigned char *s1 = (const unsigned char *) p1;
    const unsigned char *s2 = (const unsigned char *) p2;
    unsigned char c1, c2;
    do {
        c1 = (unsigned char) *s1++;
        c2 = (unsigned char) *s2++;
        if (c1 == '\0') {
            return c1 - c2;
        }
    }
    while (c1 == c2);
    return c1 - c2;
}


HMODULE WINAPI hlpGetModuleHandle(LPCWSTR sModuleName)
{
    #ifdef _M_IX86
        PEB * ProcEnvBlk = (PEB *) __readfsdword(0x30);
    #else
        PEB *ProcEnvBlk = (PEB *) __readgsqword(0x60);
    #endif

    if (sModuleName == NULL)
        return (HMODULE) ProcEnvBlk->ImageBaseAddress;


    PEB_LDR_DATA *Ldr = ProcEnvBlk->Ldr;
    LIST_ENTRY *moduleList = NULL;

    moduleList = &Ldr->InMemoryOrderModuleList;
    LIST_ENTRY *pStartListEntry = moduleList->Flink;

    for (LIST_ENTRY *pListEntry = pStartListEntry; pListEntry != moduleList; pListEntry = pListEntry->Flink)
    {
        LDR_DATA_TABLE_ENTRY *pEntry = (LDR_DATA_TABLE_ENTRY *) ((BYTE *) pListEntry - sizeof(LIST_ENTRY));

        if (my_strcmp((const char *)pEntry->BaseDllName.Buffer, (const char *) sModuleName) == 0)
        {
            return (HMODULE) pEntry->DllBase;
        }
    }

    return NULL;
}

FARPROC WINAPI hlpGetProcAddress(HMODULE hMod, char *sProcName)
{
    char *pBaseAddr = (char *) hMod;

    IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER *) pBaseAddr;
    IMAGE_NT_HEADERS *pNTHeader = (IMAGE_NT_HEADERS *) (pBaseAddr + pDosHeader->e_lfanew);
    IMAGE_OPTIONAL_HEADER *pImageOptionalHeader = &pNTHeader->OptionalHeader;
    IMAGE_DATA_DIRECTORY *pExportDataDir = (IMAGE_DATA_DIRECTORY *) &pImageOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    IMAGE_EXPORT_DIRECTORY *pImageExportDir = (IMAGE_EXPORT_DIRECTORY *) (pBaseAddr + pExportDataDir->VirtualAddress);

    DWORD *pEAT = (DWORD *) (pBaseAddr + pImageExportDir->AddressOfFunctions);
    DWORD *pFuncNameTbl = (DWORD *) (pBaseAddr + pImageExportDir->AddressOfNames);
    WORD *pOrdTbl = (WORD *) (pBaseAddr + pImageExportDir->AddressOfNameOrdinals);

    void *pProcAddr = NULL;

    if (( (UINT64) sProcName & 0xFFFF0000 ) == 0x00000000)
    {
        pEAT += ( (IMAGE_ORDINAL( (UINT64)sProcName) - pImageExportDir->Base ) * sizeof(DWORD) );
        pProcAddr = (FARPROC) ( pBaseAddr + DEREF_32(pEAT)); 
    }
    else 
    {
        for (DWORD i = 0; i < pImageExportDir->NumberOfNames; i++)
        {
            char *sTmpFunc = (char *) pBaseAddr + (DWORD_PTR) pFuncNameTbl[i];

            if (my_strcmp(sProcName, sTmpFunc) == 0)
            {
                WORD funcIndex = pOrdTbl[i];
                pProcAddr = (FARPROC) (pBaseAddr + (DWORD_PTR) pEAT[funcIndex]);
                break;
            }
        }
    }

    // TODO: handle forward functions

    return (FARPROC) pProcAddr;
}