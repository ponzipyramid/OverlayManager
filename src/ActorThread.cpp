#include "ActorThread.h"
#include "Registry.h"
#include "Util.h"
#include "Serialization.hpp"

using namespace OM;

bool ActorThread::IsSlotOpen(OverlayArea a_area, int a_slot)
{
	return !NiOverride::HasOverrideInSlot(_actor, _female, a_area, a_slot);
}

int ActorThread::GetAvailableSlot(OverlayArea a_area, int a_applied)
{
	std::unordered_set<int> occupied;
	auto tats = Util::PopulateVector(a_applied);
	for (auto& [_, data] : tats) {
		if (data.slot >= 0)
		    occupied.insert(data.slot);
    }

    int max = NiOverride::GetNumOverlays(a_area);
	
    for (int i = 0; i < max; i++) {
		if (IsSlotOpen(a_area, i) && !occupied.contains(i)) {
            return i;
        }
    }
    return -1;
}

AddResult ActorThread::AddOverlay(std::string a_context, std::string a_id, int a_color, float a_alpha, int a_glow, int a_gloss, std::string a_replaceId, int a_slot)
{
	logger::info("AddOverlay: {}", a_id);
	
	AddResult result = AddResult::Failed;
    
    if (!Registry::GetOverlay(a_id)) {
		logger::info("could not find overlay id {}", a_id);
		return result;
    }

	NiOverride::CheckAndAddOverlays(_actor);
	if (auto ovl = Registry::GetOverlay(a_id)) {
        auto& [color, alpha, glow, gloss, slot] = _active[a_id];

        color = a_color;
		alpha = a_alpha;
		glow = a_glow;
		gloss = a_gloss;

        if (slot >= 0 && GetSlotId(ovl->area, slot) == ovl->path) {  // try to use existing
			logger::info("using existing slot {}", slot);
			result = AddResult::Modified;
		} else if (_active.count(a_replaceId) && GetSlotId(ovl->area, _active[a_replaceId].slot) == a_replaceId) { // try to replace existing
			slot = _active[a_replaceId].slot;
			logger::info("replacing existing tat {}", slot);
			_active.erase(a_replaceId);
			_contexts[a_context].erase(a_replaceId);
			result = AddResult::Replaced;
		} else if (a_slot > -1 && IsSlotOpen(ovl->area, a_slot)) { // try to use forced slot
			slot = a_slot;
			logger::info("using forced slot {}", slot);
			result = AddResult::Added;
		}
		
		if (slot < 0) { // try getting an available slot
			slot = GetAvailableSlot(ovl->area);
			logger::info("finding available {}", slot);
			result = AddResult::Added;
		} 

        if (slot < 0) {
			_active.erase(a_id);
			logger::info("failed due to no slots available {}", slot);
			return AddResult::Failed;
        }
		NiOverride::ApplyOverlay(_actor, _female, ovl->area, slot, ovl->path, color, alpha, glow, gloss, ovl->bump);
        _contexts[a_context].insert(ovl->path);
    }

    return result;
}

bool ActorThread::RemoveOverlay(std::string a_context, std::string a_id)
{
	
	logger::info("RemoveOverlay: {}", a_id);
	
	if (!_active.count(a_id)) {
		logger::info("overlay is not active on actor");
		return false;
	}
		

	if (!_contexts[a_context].contains(a_id)) {
		logger::info("context does not contain overlay");
		return false;
	}

    auto ovl = Registry::GetOverlay(a_id);
    auto data = _active[a_id];
    
    // don't clear if we've been overwritten by smth else
	auto path = GetSlotId(ovl->area, data.slot);

    if (path == a_id) {
		logger::info("actually clearing overlay");
		NiOverride::ClearOverlay(_actor, _female, ovl->area, data.slot);
    }

    _active.erase(a_id);
    _contexts[a_context].erase(a_id);

    return true;
}

std::vector<int> ActorThread::GetExternalOverlaySlots(std::string a_context, OverlayArea a_area)
{
	logger::info("GetExternalOverlaySlots");

	std::vector<int> slots;

    int numSlots = NiOverride::GetNumOverlays(a_area);
    for (int i = 0; i < numSlots; i++) {
		auto path = GetSlotId(a_area, i);

		if (!IsSlotOpen(a_area, i) && !_contexts[a_context].contains(path) && _active[path].slot == i) {
			logger::info("external slot: {} {}", NiOverride::GetNode(a_area, i), !IsSlotOpen(a_area, i), !_contexts[a_context].contains(path), _active[path].slot == i);
			slots.emplace_back(i);
        }
    }

	logger::info("GetExternalOverlaySlots {}", slots.size());
	
	return slots;
}

void ActorThread::Update() 
{

	if (!_initialized && _actor->Is3DLoaded()) {
		_initialized = true;
		logger::info("initializing {}", _actor->GetFormID());

		std::map<std::pair<OverlayArea, int>, std::string> ovlsBySlot;

		for (auto& [id, data] : _active) {
			ovlsBySlot[{ Registry::GetOverlay(id)->area, data.slot }] = id;
		}

		std::map<std::string_view, std::string> ovlContexts;
		for (auto [context, ids] : _contexts) {
			for (auto id : ids) {
				ovlContexts[id] = context;
			}
		}

		bool clearOvls = true;

        for (auto area : Areas) {
			auto numSlots = NiOverride::GetNumOverlays(area);

            for (int slot = 0; slot < numSlots; slot++) {
				std::pair node{ area, slot };
				if (ovlsBySlot.count(node)) {
					auto id = ovlsBySlot[node];
					auto data = _active[id];
					auto context = ovlContexts[id];

					auto [color, alpha, glow, gloss, _] = data;

					if (GetSlotId(area, slot) == id) {  // refresh overlay
						NiOverride::ApplyOverlay(_actor, _female, area, slot, id, color, alpha, glow, gloss, Registry::GetOverlay(id)->bump);
						logger::info("not clearing ovls due to {}", id);
						clearOvls = false;
					} else {
						_active.erase(id);
						_contexts[context].erase(id);
						logger::info("not clearing ovls due to {}", id);
						AddOverlay(context, id, color, alpha, glow, gloss);
						clearOvls = false;
					}
				} else {
					auto path = GetSlotId(area, slot);
					if (path.empty() || path.starts_with(ST_PATH) || path.starts_with(OM_PATH)) {
						NiOverride::ClearOverlay(_actor, _female, area, slot);
					} else {
						logger::info("not clearing ovls due to {}", path);
						clearOvls = false;
					}
				}
            }
        }

		/*if (_actor->GetFormID() != 20 && clearOvls) {
			logger::info("removing overlays on {}", _actor->GetFormID());
			NiOverride::CheckAndRemoveOverlays(_actor);
		}*/

    }

    // LATER: set/check update time
    // LATER: update if any effects are present
}

ActorThread::ActorThread(SKSE::SerializationInterface* a_intfc)
{
	auto formId = Serialization::Read<RE::FormID>(a_intfc);
	if (auto target = RE::TESForm::LookupByID<RE::Actor>(formId)) {
		
		_actor = target;
		_female = _actor->GetActorBase()->GetSex() == 1;

	    auto count = Serialization::Read<std::size_t>(a_intfc);
		_active.reserve(count);
		for (; count > 0; count--) {
			
			auto id = Serialization::Read<std::string>(a_intfc);
			auto data = OverlayData(a_intfc);

			if (Registry::GetOverlay(id))
				_active.insert({ id, data });
        }
		
        count = Serialization::Read<std::size_t>(a_intfc);
		_contexts.reserve(count);
		for (; count > 0; count--) {			
			auto context = Serialization::Read<std::string>(a_intfc);
									
            auto num = Serialization::Read<std::size_t>(a_intfc);
			_contexts[context].reserve(num);
            for (; num > 0; num--) {
				auto id = Serialization::Read<std::string>(a_intfc);

				if (_active.contains(id))
					_contexts[context].insert(id);
			}
        }
    }
}

void ActorThread::Serialize(SKSE::SerializationInterface* a_intfc)
{
	Serialization::Write<RE::FormID>(a_intfc, _actor->GetFormID());
	Serialization::Write<std::size_t>(a_intfc, _active.size());
	for (auto& [id, data] : _active) {
		Serialization::Write<std::string>(a_intfc, id);
		data.Serialize(a_intfc);
    }

    Serialization::Write<std::size_t>(a_intfc, _contexts.size());
	for (auto& [context, ovls] : _contexts) {
		Serialization::Write<std::string>(a_intfc, context);

		Serialization::Write<std::size_t>(a_intfc, ovls.size());
        for (auto id : ovls) {
			Serialization::Write<std::string>(a_intfc, std::string(id));
        }
	}
}

bool ActorThread::IsValid()
{
	return _actor != nullptr;
}

std::string ActorThread::Print() {
	std::string status;
	
	std::vector<std::vector<std::string>> ordered;
	ordered.resize(4);
	for (int i = 0; i < ordered.size(); i++) {
		ordered[i].resize(NiOverride::GetNumOverlays(magic_enum::enum_cast<OverlayArea>(i).value_or(OverlayArea::Invalid)));
	}

	for (auto& [id, data] : _active) {
		auto ovl = Registry::GetOverlay(id);
		ordered[(int)ovl->area][data.slot] = id;
	}

	for (int i = 0; i < ordered.size(); i++) {
		for (int j = 0; j < ordered[i].size(); j++) {
			auto id = ordered[i][j];

			auto area = magic_enum::enum_cast<OverlayArea>(i).value_or(OverlayArea::Invalid);
			auto areaName = magic_enum::enum_name(area);

			if (_active.count(id)) {
				status += std::format("{} Slot: {} = {} \n", areaName, j, id);
			} else {
				status += std::format("{} Slot: {} = {} \n", areaName, j, id);
			}
		}
	}

	return status;
}