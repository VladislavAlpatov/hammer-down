#pragma once
#include <vector>
#include <string>

namespace hammer_down::modules
{
    class SignatureDetector
    {
    public:
        SignatureDetector() = delete;
        static bool FoundSignatureMatch(const std::vector<std::string>& signatures);

    private:
        static bool FindPattern(const std::pair<uintptr_t, uintptr_t>& range, const std::string &pattern);
        static std::vector<uint8_t> ParseSignatureString(const std::string &pattern);
    };
    
}
