#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <string>

namespace hammer_down
{
    class HammerDown
    {
        public:
            HammerDown(const HammerDown&) = delete;

            [[nodiscard]]
            static HammerDown& GetInstance()
            {
                static HammerDown instance;
                return instance;
            }
            void Tick();

            void OnDllInjection(const std::function<void()>& payload);
            void OnSigMatchInjection(const std::function<void()>& payload);
            void SetSignatures(const std::vector<std::string>& sigs);

        private:
            explicit HammerDown();

            void DllInjectionCheck() const;
            void DetectSignatures() const;
            std::vector<std::string> m_sigs;
            uint64_t m_millisecondsPerTick = 0;
            std::function<void()> m_onDllInjection = []{};
            std::function<void()> m_onSigInjection = []{};
    };
} // namespace hammer_head
