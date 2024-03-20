#pragma once

#include <cstdint>


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
        private:
            HammerDown() = default;
            uint64_t m_millisecondsPerTick = 0;
    };
} // namespace hammer_head
