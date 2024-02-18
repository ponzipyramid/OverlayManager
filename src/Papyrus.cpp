#include "Papyrus.h"
#include "ActorManager.h"

/*
QueryAppliedOverlays
GetOverlayMetaString
GetOverlayMetaForm
DoesOverlayMatch
GetMatchingOverlays
GetOverlayObject
ConvertOverlayList
SyncContext
*/

using namespace OM;

namespace {
    constexpr std::string_view PapyrusClass = "OverlayManager";

    void SyncContext(RE::StaticFunctionTag*, RE::Actor* a_target, std::string a_context, int a_list, int a_added, int a_removed) {
        ActorManager::SyncContext(a_target, a_context, a_list, a_added, a_removed);
    }
   
}

bool Papyrus::RegisterFunctions(RE::BSScript::IVirtualMachine* vm) {
    vm->RegisterFunction("SyncContext", "OverlayManager", SyncContext);

    return true;
}