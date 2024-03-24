#pragma once

#include <vector>
#include <cstdint>


namespace hammer_down::utils
{
    [[nodiscard]]
    std::vector<std::pair<uintptr_t, uintptr_t>> GetExecutableMemoryRegions();

    [[nodiscard]]
    bool IsModuleManualyMapped(std::uintptr_t moduleBase);
}