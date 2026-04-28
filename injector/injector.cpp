#include <windows.h>
#include <iostream>
#include <string>

bool InjectDLL(HANDLE hProcess, const std::string& dllPath)
{
    SIZE_T size = dllPath.size() + 1;

    // Allocate memory in target process
    LPVOID remoteMemory = VirtualAllocEx(
        hProcess,
        nullptr,
        size,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );

    if (!remoteMemory)
    {
        std::cout << "[-] VirtualAllocEx failed\n";
        return false;
    }

    // Write DLL path into process
    if (!WriteProcessMemory(
        hProcess,
        remoteMemory,
        dllPath.c_str(),
        size,
        nullptr))
    {
        std::cout << "[-] WriteProcessMemory failed\n";
        return false;
    }

    // Get LoadLibraryA address
    HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
    LPVOID loadLibrary = GetProcAddress(kernel32, "LoadLibraryA");

    if (!loadLibrary)
    {
        std::cout << "[-] GetProcAddress failed\n";
        return false;
    }

    // Create remote thread
    HANDLE thread = CreateRemoteThread(
        hProcess,
        nullptr,
        0,
        (LPTHREAD_START_ROUTINE)loadLibrary,
        remoteMemory,
        0,
        nullptr
    );

    if (!thread)
    {
        std::cout << "[-] CreateRemoteThread failed\n";
        return false;
    }

    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);

    return true;
}

int main()
{
    std::string gamePath = "America.exe";
    std::string dllPath = "america-fix.dll";

    STARTUPINFOA si{};
    PROCESS_INFORMATION pi{};

    si.cb = sizeof(si);

    // Launch game
    if (!CreateProcessA(
        gamePath.c_str(),
        nullptr,
        nullptr,
        nullptr,
        FALSE,
        0,
        nullptr,
        nullptr,
        &si,
        &pi))
    {
        std::cout << "[-] Failed to launch game\n";
        return 1;
    }

    std::cout << "[+] Game launched\n";

    // Give game time to initialize
    Sleep(1000);

    // Inject DLL
    if (InjectDLL(pi.hProcess, dllPath))
        std::cout << "[+] DLL injected successfully\n";
    else
        std::cout << "[-] Injection failed\n";

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    return 0;
}