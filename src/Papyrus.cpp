#include "Papyrus.h"
#include "ActorManager.h"
#include "Util.h"

/*
GetOverlayMetaString
GetOverlayMetaForm
*/

using namespace OM;

namespace {
    constexpr std::string_view PapyrusClass = "OverlayManager";

    void SyncContext(RE::StaticFunctionTag*, RE::Actor* a_target, std::string a_context, int a_list, int a_added, int a_removed) 
    {
		ActorManager::SyncContext(a_target, a_context, a_list, a_added, a_removed);
    }
   
    std::vector<int> GetExternalOverlaySlots(RE::StaticFunctionTag*, RE::Actor* a_target, std::string a_context, std::string a_area)
	{
		return ActorManager::GetActorThread(a_target)->GetExternalOverlaySlots(a_context, Util::GetArea(a_area));
    }

    int GetAvailableSlot(RE::StaticFunctionTag*, RE::Actor* a_target, std::string a_area, int a_applied) 
    {
		return ActorManager::GetActorThread(a_target)->GetAvailableSlot(Util::GetArea(a_area), a_applied);
	}

	bool DoesOverlayMatch(RE::StaticFunctionTag*, int a_template, int a_ovl, bool a_runtime)
	{
		return Util::DoesOverlayMatch(a_template, a_ovl, a_runtime);
	}

	std::vector<int> GetMatchingOverlays(RE::StaticFunctionTag*, int a_template, int a_list, int a_matches)
	{
		return Util::GetMatchingOverlays(a_template, a_list, a_matches);
	}

	void GetAllMatchingOverlays(RE::StaticFunctionTag*, std::string a_context, int a_template, int a_matches) 
	{
		return Util::GetAllMatchingOverlays(a_context, a_template, a_matches);
	}
}

bool Papyrus::RegisterFunctions(RE::BSScript::IVirtualMachine* vm) {
	vm->RegisterFunction("SyncContext", PapyrusClass, SyncContext);
	vm->RegisterFunction("GetExternalOverlaySlots", PapyrusClass, GetExternalOverlaySlots);
	
	vm->RegisterFunction("GetAvailableSlot", PapyrusClass, GetAvailableSlot);

	vm->RegisterFunction("DoesOverlayMatch", PapyrusClass, DoesOverlayMatch);
	vm->RegisterFunction("GetMatchingOverlays", PapyrusClass, GetMatchingOverlays);

	vm->RegisterFunction("GetAllMatchingOverlays", PapyrusClass, GetAllMatchingOverlays);


    return true;
}

