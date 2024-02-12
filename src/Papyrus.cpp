#include "Papyrus.h"
#include <windows.h>

using namespace OM;

namespace {
    constexpr std::string_view PapyrusClass = "OverlayManager";

    int GetNumBodyOverlays(RE::StaticFunctionTag* base) {
        HMODULE baseAddress = GetModuleHandleA("skee64.dll");
        char* func_start = (char*)baseAddress + 0xA5EF0;
        auto func_ptr = (uint32_t (*)(RE::StaticFunctionTag*)) func_start;
        SKSE::log::info("DLL Base: {}, func address: {}, num body overlays = {}", (char*) baseAddress, func_start, func_ptr(base));
        return 0;
    }

    int GetNumHandOverlays(RE::StaticFunctionTag* base) {
        HMODULE baseAddress = GetModuleHandleA("skee64.dll");
        char* func_start = (char*)baseAddress + 0xA5F00;
        auto func_ptr = (uint32_t (*)(RE::StaticFunctionTag*)) func_start;
        SKSE::log::info("DLL Base: {}, func address: {}, num hand overlays = {}", (char*) baseAddress, func_start, func_ptr(base));
        return 0;
    }

    int GetNumFeetOverlays(RE::StaticFunctionTag* base) {
        HMODULE baseAddress = GetModuleHandleA("skee64.dll");
        char* func_start = (char*)baseAddress + 0xA5F10;
        auto func_ptr = (uint32_t (*)(RE::StaticFunctionTag*)) func_start;
        SKSE::log::info("DLL Base: {}, func address: {}, num feet overlays = {}", (char*) baseAddress, func_start, func_ptr(base));
        return 0;
    }

    int GetNumFaceOverlays(RE::StaticFunctionTag* base) {
        HMODULE baseAddress = GetModuleHandleA("skee64.dll");
        char* func_start = (char*)baseAddress + 0xA5F20;
        auto func_ptr = (uint32_t (*)(RE::StaticFunctionTag*)) func_start;
        SKSE::log::info("DLL Base: {}, func address: {}, num face overlays = {}", (char*) baseAddress, func_start, func_ptr(base));
        return 0;
    }

    std::string GetNodeOverrideString(RE::StaticFunctionTag* base, RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, uint32_t a_key, uint32_t a_index) {
        HMODULE baseAddress = GetModuleHandleA("skee64.dll");
        char* func_start = (char*)baseAddress + 0xCA3C0;

        auto func_ptr = (RE::BSFixedString (*)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, uint32_t, uint32_t)) func_start;
        auto result = func_ptr(base, a_ref, a_female, a_node, a_key, a_index);
        return std::string{result.c_str()};
    }

    float GetNodeOverrideFloat(RE::StaticFunctionTag* base, RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, uint32_t a_key, uint32_t a_index) {
        SKSE::log::info("GetNodeOverrideFloat");
        HMODULE baseAddress = GetModuleHandleA("skee64.dll");
        char* func_start = (char*)baseAddress + 0xCA160;

        auto func_ptr = (float (*)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, uint32_t, uint32_t)) func_start;
        return func_ptr(base, a_ref, a_female, a_node, a_key, a_index);
    }

    int GetNodeOverrideInt(RE::StaticFunctionTag* base, RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, uint32_t a_key, uint32_t a_index) {
        SKSE::log::info("GetNodeOverrideInt");
        HMODULE baseAddress = GetModuleHandleA("skee64.dll");
        char* func_start = (char*)baseAddress + 0xCA230;

        auto func_ptr = (int (*)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, uint32_t, uint32_t)) func_start;
        return func_ptr(base, a_ref, a_female, a_node, a_key, a_index);
    }


    void AddNodeOverrideFloat(RE::StaticFunctionTag* base, RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, uint32_t a_key, uint32_t a_index, float a_value, bool a_persist) {
        HMODULE baseAddress = GetModuleHandleA("skee64.dll");
        char* func_start = (char*)baseAddress + 0xC9860;
        auto func_ptr = (void (*)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, uint32_t, uint32_t, float, bool)) func_start;
        SKSE::log::info("AddNodeOverrideFloat");
        
        func_ptr(base, a_ref, a_female, a_node, a_key, a_index, a_value, a_persist);
        SKSE::log::info("Applied node override float {}", a_value == GetNodeOverrideFloat(base, a_ref, a_female, a_node, a_key, a_index));
    }

    void AddNodeOverrideString(RE::StaticFunctionTag* base, RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, uint32_t a_key, uint32_t a_index, std::string a_value, bool a_persist) {
        SKSE::log::info("AddNodeOverrideString");
        HMODULE baseAddress = GetModuleHandleA("skee64.dll");
        char* func_start = (char*)baseAddress + 0xC9D20;

        auto func_ptr = (void (*)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, uint32_t, uint32_t, const char*, bool)) func_start;
        func_ptr(base, a_ref, a_female, a_node, a_key, a_index, a_value.c_str(), a_persist);
        SKSE::log::info("Applied node override int {}", a_value == GetNodeOverrideString(base, a_ref, a_female, a_node, a_key, a_index));
    }

    void AddNodeOverrideInt(RE::StaticFunctionTag* base, RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, uint32_t a_key, uint32_t a_index, int a_value, bool a_persist) {
        SKSE::log::info("AddNodeOverrideInt");
        HMODULE baseAddress = GetModuleHandleA("skee64.dll");
        char* func_start = (char*)baseAddress + 0xC9AD0;

        auto func_ptr = (void (*)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, uint32_t, uint32_t, int, bool)) func_start;
        func_ptr(base, a_ref, a_female, a_node, a_key, a_index, a_value, a_persist);
        SKSE::log::info("Applied node override int {}", a_value == GetNodeOverrideInt(base, a_ref, a_female, a_node, a_key, a_index));
    }
}

bool Papyrus::RegisterFunctions(RE::BSScript::IVirtualMachine* vm) {
    vm->RegisterFunction("GetNumBodyOverlays", PapyrusClass, GetNumBodyOverlays);
    vm->RegisterFunction("GetNumHandOverlays", PapyrusClass, GetNumHandOverlays);
    vm->RegisterFunction("GetNumFeetOverlays", PapyrusClass, GetNumFeetOverlays);
    vm->RegisterFunction("GetNumFaceOverlays", PapyrusClass, GetNumFaceOverlays);

    vm->RegisterFunction("AddNodeOverrideFloat", PapyrusClass, AddNodeOverrideFloat);
    vm->RegisterFunction("AddNodeOverrideString", PapyrusClass, AddNodeOverrideString);
    vm->RegisterFunction("AddNodeOverrideInt", PapyrusClass, AddNodeOverrideInt);
    
    return true;
} 