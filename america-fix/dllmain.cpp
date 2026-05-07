#include <windows.h>
#include <cstdint>

// ============================
// Forward declaration
// ============================
void apply_resolution_fix(uintptr_t base);

// ============================
// GetTickCount hook
// ============================

typedef DWORD(WINAPI* GetTickCount_t)();
GetTickCount_t originalGetTickCount = nullptr;

DWORD WINAPI hookedGetTickCount()
{
    static DWORD startReal = originalGetTickCount();
    static DWORD startFake = 0;

    DWORD now = originalGetTickCount();

    // Adjust this divisor to control game speed
    // Smaller = faster, larger = slower
    DWORD fakeTime = startFake + (now - startReal);

    return fakeTime;
}

// ============================
// Hook IAT (Import Address Table)
// ============================

void hook_GetTickCount()
{
    HMODULE hModule = GetModuleHandle(NULL);

    auto dos = (IMAGE_DOS_HEADER*)hModule;
    auto nt = (IMAGE_NT_HEADERS*)((BYTE*)hModule + dos->e_lfanew);

    auto importDesc = (IMAGE_IMPORT_DESCRIPTOR*)((BYTE*)hModule +
        nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    for (; importDesc->Name; importDesc++)
    {
        const char* moduleName = (const char*)((BYTE*)hModule + importDesc->Name);

        if (_stricmp(moduleName, "KERNEL32.dll") != 0)
            continue;

        auto thunk = (IMAGE_THUNK_DATA*)((BYTE*)hModule + importDesc->FirstThunk);

        for (; thunk->u1.Function; thunk++)
        {
            PROC* func = (PROC*)&thunk->u1.Function;

            if (*func == (PROC)originalGetTickCount)
            {
                DWORD oldProtect;
                VirtualProtect(func, sizeof(PROC), PAGE_EXECUTE_READWRITE, &oldProtect);

                *func = (PROC)hookedGetTickCount;

                VirtualProtect(func, sizeof(PROC), oldProtect, &oldProtect);

                return;
            }
        }
    }
}

// ============================
// Main patch thread
// ============================

DWORD WINAPI PatchThread(LPVOID)
{
    // Let game initialize
    Sleep(1000);

    // Get original function
    originalGetTickCount = (GetTickCount_t)GetProcAddress(
        GetModuleHandleA("kernel32.dll"),
        "GetTickCount"
    );

    // Hook timing
    hook_GetTickCount();

    // Apply your resolution fix
    uintptr_t base = (uintptr_t)GetModuleHandle(NULL);
    apply_resolution_fix(base);

    return 0;
}

// ============================
// DLL Entry
// ============================

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD reason,
    LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, PatchThread, NULL, 0, NULL);
    }
    return TRUE;
}