#pragma once
#include <windows.h>
#include <vector>

struct Patch
{
    uintptr_t offset;
    std::vector<BYTE> bytes;
};

void apply_patch(uintptr_t base, const Patch& patch);