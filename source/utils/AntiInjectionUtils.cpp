#include "AntiInjectionUtils.h"

#include <Windows.h>
#include <tlhelp32.h>
#include <psapi.h>


namespace hammer_down::utils
{
    std::vector<std::pair<uintptr_t, uintptr_t>> GetExecutableMemoryRegions() 
    {
        std::vector<std::pair<uintptr_t, uintptr_t>> regions;

        const auto IsExecutable = [](const DWORD protection) 
        { 
            return protection == PAGE_EXECUTE_READ || protection == PAGE_EXECUTE_READWRITE || protection == PAGE_EXECUTE_WRITECOPY;
        };
        // Enumerate memory regions
        MEMORY_BASIC_INFORMATION mbi;
        for (uintptr_t addr = 0; VirtualQuery(reinterpret_cast<LPCVOID>(addr), &mbi, sizeof(mbi)) == sizeof(mbi); addr += mbi.RegionSize) 
        {
            if (!IsExecutable(mbi.Protect))
                continue;

            regions.emplace_back(reinterpret_cast<uintptr_t>(mbi.BaseAddress),
                                 reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize);
        }

    return regions;
}
    bool IsModuleManualyMapped(std::uintptr_t moduleBase)
    {
        MODULEINFO info;
        return !K32GetModuleInformation(GetCurrentProcess(),
                                        reinterpret_cast<HMODULE>(moduleBase), &info, sizeof(info));
    }
} // namespace hammer_down::utils
