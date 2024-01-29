#include "Papyrus.h"
#include "OverlayManager.h"

using namespace OM;

namespace {
    constexpr std::string_view PapyrusClass = "OverlayManager";

    void UpdateOverlaysLatent(RE::BSScript::Internal::VirtualMachine* a_vm, RE::VMStackID a_stackId) {
        a_vm->ReturnLatentResult<bool>(a_stackId, true);
    }

    RE::BSScript::LatentStatus UpdateOverlays([[maybe_unused]] RE::BSScript::Internal::VirtualMachine* a_vm, [[maybe_unused]] RE::VMStackID a_stackId, [[maybe_unused]] RE::StaticFunctionTag* a_cl, [[maybe_unused]] RE::Actor* a_actor) {
        std::thread t1(OverlayManager::UpdateOverlays, a_actor);
        t1.detach();
        return RE::BSScript::LatentStatus::kStarted;
    }
}

bool Papyrus::RegisterFunctions(RE::BSScript::IVirtualMachine* vm) {
    vm->RegisterLatentFunction<bool>("UpdateOverlays", PapyrusClass, UpdateOverlays);
    return true;
}