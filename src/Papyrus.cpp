#include "Papyrus.h"
#include "ActorManager.h"
#include "Util.h"

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

	void GetAllMatchingOverlays(RE::StaticFunctionTag*, std::string a_context, int a_template, int a_matches, int a_applied) 
	{
		return Util::GetAllMatchingOverlays(a_context, a_template, a_matches, a_applied);
	}

	std::string GetEvent(RE::StaticFunctionTag*, std::string a_id)
	{
		if (auto ovl = Registry::GetOverlay(a_id))
			return ovl->event;
		else
			return "";
	}

	std::string GetOverlayMetaString(RE::StaticFunctionTag*, std::string a_id, std::string a_key, std::string a_default) 
	{
		if (auto ovl = Registry::GetOverlay(a_id))
			return ovl->GetMetaStr(a_key, a_default);
		else
			return a_default;
	}

	float GetOverlayMetaNumeric(RE::StaticFunctionTag*, std::string a_id, std::string a_key, float a_default)
	{
		if (auto ovl = Registry::GetOverlay(a_id))
			return ovl->GetMetaNum(a_key, a_default);
		else
			return a_default;
	}

	RE::TESForm* GetOverlayMetaForm(RE::StaticFunctionTag*, std::string a_id, std::string a_key, RE::TESForm* a_default)
	{
		if (auto ovl = Registry::GetOverlay(a_id))
			return ovl->GetMetaForm(a_key, a_default);
		else
			return a_default;
	}
}

bool Papyrus::RegisterFunctions(RE::BSScript::IVirtualMachine* vm) {
	vm->RegisterFunction("SyncContext", PapyrusClass, SyncContext, true);
	vm->RegisterFunction("GetExternalOverlaySlots", PapyrusClass, GetExternalOverlaySlots);
	
	vm->RegisterFunction("GetAvailableSlot", PapyrusClass, GetAvailableSlot);

	vm->RegisterFunction("DoesOverlayMatch", PapyrusClass, DoesOverlayMatch);
	vm->RegisterFunction("GetMatchingOverlays", PapyrusClass, GetMatchingOverlays);

	vm->RegisterFunction("GetAllMatchingOverlays", PapyrusClass, GetAllMatchingOverlays);

	vm->RegisterFunction("GetOverlayMetaString", PapyrusClass, GetOverlayMetaString);
	vm->RegisterFunction("GetOverlayMetaNumeric", PapyrusClass, GetOverlayMetaNumeric);
	vm->RegisterFunction("GetOverlayMetaForm", PapyrusClass, GetOverlayMetaForm);

    return true;
}

