#include "SignatureDetector.h"

#include <asio.hpp>
#include <thread>
#include <atomic>
#include "utils/AntiInjectionUtils.h"


namespace hammer_down::modules
{
    bool SignatureDetector::FoundSignatureMatch(const std::vector<std::string>& signatures)
    {
        asio::thread_pool threadPool(std::thread::hardware_concurrency());

        std::atomic<size_t> matches = 0;

        const auto work  = [&signatures, &matches](const std::pair<uintptr_t, uintptr_t>& range)
        {
            for (const auto& sig : signatures)
            {
                if (matches > 0)
                    return;
                
                if (FindPattern(range, sig))
                    matches++;
            }
        };

        for (const auto& range : utils::GetExecutableMemoryRegions())
            asio::post(threadPool, std::bind(work, range));

        threadPool.join();

        return matches > 0;
    }
    bool SignatureDetector::FindPattern(const std::pair<uintptr_t, uintptr_t>& range, const std::string &pattern)
    {
        const int64_t rangeSize = range.second - range.first;

        const auto patternBytes = ParseSignatureString(pattern);
        for (auto i = 0; i < rangeSize-patternBytes.size(); i++)
        {
            bool found = true;

            for (size_t j = 0; j < patternBytes.size(); j++)
            {
                found = patternBytes[j] == '\?' or patternBytes[j] == *(uint8_t*)(range.first+i+j);
                if (not found) break;
            }
            if (found)
                return true;
        }
        return false;

    }
    std::vector<uint8_t> SignatureDetector::ParseSignatureString(const std::string &pattern)
    {
        std::vector<uint8_t> bytes;

        const auto HexCharToUint =  [](char chr) -> uint8_t
        {
            chr = tolower(chr);

            return ('a' <= chr and chr <= 'z') ? chr - 'a' + 10 : chr - '0';
        };

        for (size_t i = 0; i < pattern.size();)
        {
            if (pattern.at(i) == ' ')
            {
                i += 1;
                continue;
            }
            if (pattern[i] == '?')
            {
                bytes.push_back('\?');
                i+1 < pattern.size() and pattern[i+1] == '?' ? i += 2 : i++;
                continue;
            }

            bytes.push_back(HexCharToUint(pattern[i]) * 16 + HexCharToUint(pattern[i + 1]));
            i += 2;
        }

        return bytes;
    }
}