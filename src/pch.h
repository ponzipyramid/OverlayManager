#pragma once

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <windows.h>
#include <magic_enum.hpp>
#include <any>
#include <vector>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <unordered_set>
#include <unordered_map>

namespace WinAPI = SKSE::WinAPI;
namespace logs = SKSE::log;
namespace stl = SKSE::stl;
using namespace std::literals;
