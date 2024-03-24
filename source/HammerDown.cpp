#include "HammerDown/HammerDown.h"

#include <string>
#include <vector>
#include <print>
#include <algorithm>
#include <ranges>
#include <thread>

#include "utils/AntiInjectionUtils.h"

namespace hammer_down
{
    void HammerDown::Tick()
    {
        DllInjectionCheck();
    }

    HammerDown::HammerDown() : m_threadPool(std::thread::hardware_concurrency())
    {

    }

    void HammerDown::OnDllInjection(const std::function<void()> &payload)
    {
        m_onDllInjection = payload;
    }
    void HammerDown::DllInjectionCheck() const
    {
        const bool detected = std::ranges::any_of(utils::GetExecutableMemoryRegions(),
        [](const auto& range) -> bool
        {
            return utils::IsModuleManualyMapped(range.first-0x1000);
        });
        
        if (!detected)
            return;
        
        m_onDllInjection();
    }
}