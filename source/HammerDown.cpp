#include <HammerDown/HammerDown.h>

#include <Windows.h>
#include <tlhelp32.h>
#include <string>
#include <optional>
#include <vector>
#include <thread>
#include <print>

std::optional<std::string> GetModuleName(std::uintptr_t addr) 
{
    char buffer[MAX_PATH] = {0}; // Initialize the buffer to zero

    DWORD result = GetModuleFileNameA(reinterpret_cast<HMODULE>(addr), buffer, MAX_PATH);

    if (result == 0) 
        return std::nullopt;

    return std::string(buffer);
}

std::vector<std::pair<uintptr_t, uintptr_t>> GetExecutableMemoryRegions() {
    std::vector<std::pair<uintptr_t, uintptr_t>> regions;

    // Enumerate memory regions
    MEMORY_BASIC_INFORMATION mbi;
    for (uintptr_t addr = 0; VirtualQuery(reinterpret_cast<LPCVOID>(addr), &mbi, sizeof(mbi)) == sizeof(mbi); addr += mbi.RegionSize) {
        // Check for executable memory
        if (mbi.Protect == PAGE_EXECUTE_READ || mbi.Protect == PAGE_EXECUTE_READWRITE || mbi.Protect == PAGE_EXECUTE_WRITECOPY) {
            regions.push_back(std::make_pair(reinterpret_cast<uintptr_t>(mbi.BaseAddress), reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize));
        }
    }

    return regions;
}

namespace hammer_down
{
    void hammer_down::HammerDown::Tick()
    {
        for (const auto& [begin, end] : GetExecutableMemoryRegions())
            if (!GetModuleName(begin - 0x1000).has_value())
            {
                MessageBoxA(nullptr, "Cheat Detected!", "Alert", MB_OK);
                exit(0);
            }
    }
}
