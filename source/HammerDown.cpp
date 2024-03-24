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

    HammerDown::HammerDown()
    {

    }

    void HammerDown::OnDllInjection(const std::function<void()> &payload)
    {
        m_onDllInjection = payload;
    }

    void HammerDown::OnSigMatch(const std::function<void()> &payload)
    {
        m_onSigMatch = payload;
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
        bool detected = false;
        {
        std::scoped_lock lock(m_mutex);
        modules::SignatureDetector::FoundSignatureMatch(m_sigs);
        }
        if (!detected)
            return;

        m_onSigMatch();
    }
    void HammerDown::SetSignatures(const std::vector<std::string>& sigs)
    {
        m_sigs = sigs;
    }
}