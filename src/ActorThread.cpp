#include "ActorThread.h"
#include "Registry.h"
#include "Util.h"
#include "Serialization.hpp"

using namespace OM;

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
        auto path = NiOverride::GetPath(_actor, _female, a_area, i);

		if (Util::IsSlotOpen(path) && !occupied.contains(i)) {
            return i;
        }


    }
    return -1;
}

AddResult ActorThread::AddOverlay(std::string a_context, std::string a_id, int a_color, float a_alpha, int a_glow, int a_gloss, std::string a_bump, std::string a_replaceId, int)
{
    AddResult result = AddResult::Failed;
    
    if (!Registry::GetOverlay(a_id)) {
		logger::info("could not find overlay id {}", a_id);
		return result;
    }

    logger::info("AddOverlay");

    if (auto ovl = Registry::GetOverlay(a_id)) {
        auto& [color, alpha, glow, gloss, slot] = _active[a_id];

        color = a_color;
		alpha = a_alpha;
		glow = a_glow;
		gloss = a_gloss;

        if (slot >= 0) {
			result = AddResult::Modified;
		} else if (_active.count(a_replaceId) && NiOverride::GetPath(_actor, _female, ovl->area, _active[a_replaceId].slot) == a_replaceId) {
			slot = _active[a_replaceId].slot;
			_active.erase(a_replaceId);
			result = AddResult::Replaced;
		} else if (slot < 0 || NiOverride::GetPath(_actor, _female, ovl->area, slot) != a_id) {
			slot = GetAvailableSlot(ovl->area);
			result = AddResult::Added;
		} 

        // TODO: add back forced slot setting
		/*if (a_slot >= -1) {
			slot = a_slot;
        }*/

        logger::info("Found slot: {}", slot);

        if (slot < 0) {
			_active.erase(a_id);
			return AddResult::Failed;
        }

        // TODO: add bump
        NiOverride::ApplyOverlay(_actor, _female, ovl->area, slot, ovl->path, color, alpha, glow, gloss, "");

        _contexts[a_context].insert(ovl->path);

        logger::info("Context Size: {}", _contexts[a_context].size());

        return result;

    }

    return result;
}

bool ActorThread::RemoveOverlay(std::string a_context, std::string a_id)
{

    logger::info("RemoveOverlay: {}", a_id);
	
    if (!_active.count(a_id))
		return false;
	
    logger::info("RemoveOverlay: found id");


    auto ovl = Registry::GetOverlay(a_id);
    auto data = _active[a_id];
    
    // don't clear if we've been overwritten by smth else
	auto path = NiOverride::GetPath(_actor, _female, ovl->area, data.slot);

    if (path == a_id) {
		NiOverride::ClearOverlay(_actor, _female, ovl->area, data.slot);
    }

	logger::info("RemoveOverlay: matching {} {}", path, data.slot);

    _active.erase(a_id);
    _contexts[a_context].erase(a_id);

    return true;
}

std::vector<int> ActorThread::GetExternalOverlaySlots(std::string a_context, OverlayArea a_area)
{
	std::vector<int> slots;

    int numSlots = NiOverride::GetNumOverlays(a_area);
    for (int i = 0; i < numSlots; i++) {
		auto path = NiOverride::GetPath(_actor, _female, a_area, i);

		if (!Util::IsSlotOpen(path) && !_contexts[a_context].contains(path) && _active[path].slot == i) {
			slots.emplace_back(i);
        }
    }

    return slots;
}

void ActorThread::Update() 
{

	if (!_initialized && _actor->Is3DLoaded()) {
		logger::info("initializing {}", _actor->GetFormID());

		std::map<std::pair<OverlayArea, int>, std::string> ovlsBySlot;

		logger::info("_active: {}", _active.size());
		for (auto& [id, data] : _active) {
			ovlsBySlot[{ Registry::GetOverlay(id)->area, data.slot }] = id;
			logger::info("{} in {} {}", id, int(Registry::GetOverlay(id)->area), data.slot);
		}


        // TODO: complete
        for (auto area : Areas) {
			auto numSlots = NiOverride::GetNumOverlays(area);

            for (int slot = 0; slot < numSlots; slot++) {
				std::pair node{ area, slot };
				if (ovlsBySlot.count(node)) {
					logger::info("applying {} {}", (int)area, slot);
					auto id = ovlsBySlot[node];
					auto data = _active[id];

					// TODO: actually fetch context
					std::string context = "slavetats";

					auto [color, alpha, glow, gloss, _] = data;

					if (NiOverride::GetPath(_actor, _female, area, slot) == id) { // refresh overlay
						logger::info("refreshing {} {}", (int)area, slot);
						// TODO: add bump
						NiOverride::ApplyOverlay(_actor, _female, area, slot, id, color, alpha, glow, gloss, "");
					} else { // TODO: attempt to add it elsewhere, remove from map if not
						logger::info("reapplying {} {}", (int)area, slot);
						_active.erase(id);
						_contexts[context].erase(id);
						// TODO: add bump
						AddOverlay(context, id, color, alpha, glow, gloss, "");
					}
				} else {
					logger::info("clearing {} {}", (int)area, slot);
					NiOverride::ClearOverlay(_actor, _female, area, slot);
				}

            }
        }

		_initialized = true;
    }

    // TODO: set/check update time
    // TODO: update if any effects are present
}

ActorThread::ActorThread(SKSE::SerializationInterface* a_intfc)
{
	auto formId = Serialization::Read<RE::FormID>(a_intfc);
	logger::info("ActorThread: {}", formId);
	if (auto target = RE::TESForm::LookupByID<RE::Actor>(formId)) {
		logger::info("ActorThread: actor {}", target != nullptr);
		
		_actor = target;
		_female = _actor->GetActorBase()->GetSex() == 1;

	    auto count = Serialization::Read<std::size_t>(a_intfc);
		_active.reserve(count);
		
		logger::info("ActorThread: active {}", count);
		for (; count > 0; count--) {
			logger::info("ActorThread: deserializing ovl {}", count);

			auto id = Serialization::Read<std::string>(a_intfc);
			auto data = OverlayData(a_intfc);

			if (Registry::GetOverlay(id))
				_active.insert({ id, data });
        }
		
        count = Serialization::Read<std::size_t>(a_intfc);
		_contexts.reserve(count);
		logger::info("ActorThread: _contexts {}", count);
		for (; count > 0; count--) {
			logger::info("ActorThread: deserializing context {}", count);
			
			auto context = Serialization::Read<std::string>(a_intfc);
			
			logger::info("ActorThread: context 1");
						
            auto num = Serialization::Read<std::size_t>(a_intfc);
			
			logger::info("ActorThread: context 2");
			
			_contexts[context].reserve(num);

			logger::info("ActorThread: num ovls {}", num);

            for (; num > 0; num--) {
				logger::info("ActorThread: deserializing context ovl {}", num);
				auto id = Serialization::Read<std::string>(a_intfc);

				if (_active.contains(id))
					_contexts[context].insert(id);
			}
        }

		logger::info("ActorThread::Deserialize {} {} {}", _actor->GetFormID(), _active.size(), _contexts.size());
    }

}

void ActorThread::Serialize(SKSE::SerializationInterface* a_intfc)
{
	Serialization::Write<RE::FormID>(a_intfc, _actor->GetFormID());
	Serialization::Write<std::size_t>(a_intfc, _active.size());
	logger::info("ActorThread: _active {}", _active.size());
	for (auto& [id, data] : _active) {
		Serialization::Write<std::string>(a_intfc, id);
		data.Serialize(a_intfc);
    }

    Serialization::Write<std::size_t>(a_intfc, _contexts.size());
	logger::info("ActorThread: _contexts {}", _contexts.size());
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