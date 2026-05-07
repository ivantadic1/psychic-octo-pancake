#include "patcher.h"

void apply_patch(uintptr_t base, const Patch& patch)
{
    DWORD oldProtect;

    uintptr_t addr = base + patch.offset;

    VirtualProtect((LPVOID)addr, patch.bytes.size(), PAGE_EXECUTE_READWRITE, &oldProtect);

    for (size_t i = 0; i < patch.bytes.size(); i++)
    {
        *(BYTE*)(addr + i) = patch.bytes[i];
    }

    VirtualProtect((LPVOID)addr, patch.bytes.size(), oldProtect, &oldProtect);
}