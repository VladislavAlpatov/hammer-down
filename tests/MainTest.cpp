#include <gtest/gtest.h>
#include <HammerDown/HammerDown.h>

TEST(UnitTestHammerDown, Main)
{
    while (true)
    {
        hammer_down::HammerDown::GetInstance().Tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
}