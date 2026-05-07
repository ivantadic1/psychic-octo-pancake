#include "../patcher/scanner.h"
#include <windows.h>

void apply_resolution_fix(uintptr_t base)
{
    uintptr_t addr = find_pattern(
        base,
        0x01000000,
        "89 4C 24 14 03 D2 89 6C 24 10"
    );

    if (!addr)
    {
        MessageBoxA(0, "Pattern not found", "Error", 0);
        return;
    }

    // Debug: verify location
    char buffer[100];
    sprintf_s(buffer, "Found at: %p", (void*)addr);
    MessageBoxA(0, buffer, "Debug", 0);

    uintptr_t patchAddr = addr + 4;

    DWORD oldProtect;
    VirtualProtect((LPVOID)patchAddr, 2, PAGE_EXECUTE_READWRITE, &oldProtect);

    // NOP NOP
    *(BYTE*)patchAddr = 0x90;
    *(BYTE*)(patchAddr + 1) = 0x90;

    VirtualProtect((LPVOID)patchAddr, 2, oldProtect, &oldProtect);
}