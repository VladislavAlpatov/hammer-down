#pragma once

#include <cstdint>
#include <functional>
#include <asio.hpp>

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
        private:
            explicit HammerDown();

            void DllInjectionCheck() const;
            void DetectSignatures() const;
            uint64_t m_millisecondsPerTick = 0;
            std::function<void()> m_onDllInjection = []{};
            std::function<void()> m_onSigInjection = []{};
            asio::thread_pool m_threadPool;
    };
} // namespace hammer_head
