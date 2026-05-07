#include "scanner.h"
#include <sstream>

std::vector<int> pattern_to_bytes(const std::string& pattern)
{
    std::vector<int> bytes;
    std::istringstream stream(pattern);
    std::string byte;

    while (stream >> byte)
    {
        if (byte == "??")
            bytes.push_back(-1);
        else
            bytes.push_back(std::stoi(byte, nullptr, 16));
    }

    return bytes;
}

uintptr_t find_pattern(uintptr_t base, size_t size, const std::string& pattern)
{
    auto patternBytes = pattern_to_bytes(pattern);
    auto data = (BYTE*)base;

    size_t patternSize = patternBytes.size();

    for (size_t i = 0; i < size - patternSize; i++)
    {
        bool found = true;

        for (size_t j = 0; j < patternSize; j++)
        {
            if (patternBytes[j] != -1 &&
                data[i + j] != patternBytes[j])
            {
                found = false;
                break;
            }
        }

        if (found)
            return base + i;
    }

    return 0;
}