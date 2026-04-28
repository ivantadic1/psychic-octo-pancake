#include <windows.h>

DWORD WINAPI PatchThread(LPVOID lpParam)
{
    Sleep(1000); // let game initialize

    // Get module base (safer than hardcoding)
    DWORD base = (DWORD)GetModuleHandle(NULL);

    // Your offset (we'll confirm later if needed)
    DWORD addr = base + 0x00111A20;

    DWORD oldProtect;

    // Allow writing
    VirtualProtect((LPVOID)addr, 2, PAGE_EXECUTE_READWRITE, &oldProtect);

    // Patch: ADD EAX, EAX (01 C0)
    *(BYTE*)addr = 0x01;
    *(BYTE*)(addr + 1) = 0xC0;

    // Restore protection
    VirtualProtect((LPVOID)addr, 2, oldProtect, &oldProtect);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD reason,
    LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, PatchThread, NULL, 0, NULL);
    }
    return TRUE;
}