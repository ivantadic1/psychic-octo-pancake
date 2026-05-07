#pragma once
#include <windows.h>
#include <vector>
#include <string>

uintptr_t find_pattern(uintptr_t base, size_t size, const std::string& pattern);