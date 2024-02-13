#include "Papyrus.h"
#include "NiOverride.h"

using namespace OM;

namespace {
    constexpr std::string_view PapyrusClass = "OverlayManager";
    
    int GetNumBodyOverlays(RE::StaticFunctionTag*) {
        return NiOverride::GetNumBodyOverlays();
    }

    void AddNodeOverrideFloat(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index, float a_value, bool a_persist) {
        NiOverride::AddNodeOverrideFloat(a_ref, a_female, a_node, a_key, a_index, a_value, a_persist);
    }

    void AddNodeOverrideString(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index, std::string a_value, bool a_persist) {
        NiOverride::AddNodeOverrideString(a_ref, a_female, a_node, a_key, a_index, a_value, a_persist);
    }

    void AddNodeOverrideInt(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index, int a_value, bool a_persist) {
        NiOverride::AddNodeOverrideInt(a_ref, a_female, a_node, a_key, a_index, a_value, a_persist);
    }
}

bool Papyrus::RegisterFunctions(RE::BSScript::IVirtualMachine* vm) {
    vm->RegisterFunction("GetNumBodyOverlays", PapyrusClass, GetNumBodyOverlays);
        
    vm->RegisterFunction("AddNodeOverrideFloat", PapyrusClass, AddNodeOverrideFloat);
    vm->RegisterFunction("AddNodeOverrideString", PapyrusClass, AddNodeOverrideString);
    vm->RegisterFunction("AddNodeOverrideInt", PapyrusClass, AddNodeOverrideInt);
    
    return true;
} 