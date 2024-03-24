#include "HammerDown/HammerDown.h"

#include <string>
#include <vector>
#include <print>
#include <algorithm>
#include <ranges>
#include <thread>

#include "utils/AntiInjectionUtils.h"
#include "modules/SignatureDetector.h"

namespace hammer_down
{
    void HammerDown::Tick()
    {
        DllInjectionCheck();
        DetectSignatures();
    }

    HammerDown::HammerDown() : m_threadPool(std::thread::hardware_concurrency())
    {

    }

    void HammerDown::OnDllInjection(const std::function<void()> &payload)
    {
        m_onDllInjection = payload;
    }

    void HammerDown::OnSigMatchInjection(const std::function<void()> &payload)
    {
        m_onSigInjection = payload;
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
    void HammerDown::DetectSignatures() const
    {
        bool detected = modules::SignatureDetector::FoundSignatureMatch({"48 83 EC 28 45 33 C9 4C 8D 05 ? ? ? ? 48 8D 15 ? ? ? ? 33 C9 FF 15 ? ? ? ? 33 C0 48 83 C4 28 C3"});

        if (!detected)
            return;

        m_onSigInjection();
    }

}