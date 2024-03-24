#include <gtest/gtest.h>
#include <HammerDown/HammerDown.h>

#include <Windows.h>


TEST(UnitTestHammerDown, Main)
{
    while (true)
    {
        auto& hd = hammer_down::HammerDown::GetInstance();
        hd.OnDllInjection([]{MessageBoxA(0, "Warning detected manual map!", "Hammer-Down", MB_OK | MB_ICONWARNING);});
        hd.OnSigMatch([]{MessageBoxA(0, "Warning cheat in data base!", "Hammer-Down", MB_OK | MB_ICONWARNING);});

        hammer_down::HammerDown::GetInstance().Tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
}