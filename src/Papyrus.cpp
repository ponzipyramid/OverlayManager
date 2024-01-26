#include "Papyrus.h"
#include "OverlayManager.h"

using namespace OM;

namespace {
    constexpr std::string_view PapyrusClass = "OverlayManager";

    bool UpdateOverlays(RE::StaticFunctionTag*, RE::Actor* a_actor) {
        return OverlayManager::UpdateOverlays(a_actor);
    }
}

bool Papyrus::RegisterFunctions(RE::BSScript::IVirtualMachine* vm) {
    vm->RegisterFunction("UpdateOverlays", PapyrusClass, UpdateOverlays);

    return true;
}