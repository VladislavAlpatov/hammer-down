#include <gtest/gtest.h>
#include <HammerDown/HammerDown.h>

#include <Windows.h>


TEST(UnitTestHammerDown, Main)
{
    auto& hd = hammer_down::HammerDown::GetInstance();
    hd.SetSignatures({"48 83 EC 28 45 33 C9 4C 8D 05 ? ? ? ? 48 8D 15 ? ? ? ? 33 C9 FF 15 ? ? ? ? 33 C0 48 83 C4 28 C3"});
    hd.OnDllInjection([]{MessageBoxA(0, "Warning detected manual map!", "Hammer-Down", MB_OK | MB_ICONWARNING);});
    hd.OnSigMatch([]{MessageBoxA(0, "Warning cheat in data base!", "Hammer-Down", MB_OK | MB_ICONWARNING);});

    while (true)
    {
        hammer_down::HammerDown::GetInstance().Tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
}