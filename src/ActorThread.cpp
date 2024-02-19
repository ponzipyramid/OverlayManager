#include "ActorThread.h"
#include "Registry.h"
#include "Util.h"

using namespace OM;

int ActorThread::GetAvailableSlot(OverlayArea a_area) {
    int max = NiOverride::GetNumOverlays(a_area);
    for (int i = 0; i < max; i++) {
        auto path = NiOverride::GetPath(_actor, _female, a_area, i);

        // TODO: improve check
		if (Util::IsSlotOpen(path)) {
            return i;
        }
    }
    return -1;
}

AddResult ActorThread::AddOverlay(std::string a_context, std::string a_id, int a_color, float a_alpha, int a_glow, int a_gloss, std::string a_bump, std::string a_replaceId, int a_slot)
{
    AddResult result = AddResult::Failed;
    
    if (!Registry::GetOverlay(a_id)) {
		logger::info("could not find overlay id {}", a_id);
		return result;
    }

    logger::info("AddOverlay");

    if (auto ovl = Registry::GetOverlay(a_id)) {
        auto& [color, alpha, glow, gloss, bump, slot] = _active[a_id];

        color = a_color;
		alpha = a_alpha;
		glow = a_glow;
		gloss = a_gloss;
		bump = a_bump;

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

        if (a_slot >= - 1) {
            // TODO: validate further
			slot = a_slot;
        }

        logger::info("Found slot: {}", slot);

        if (slot < 0) {
			_active.erase(a_id);
			return AddResult::Failed;
        }

        NiOverride::ApplyOverlay(_actor, _female, ovl->area, slot, ovl->path, color, alpha, glow, gloss, bump);

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

    logger::info("RemoveOverlay: matching {} {} = {}", path, a_id, path == a_id);

    if (path == a_id) {
		NiOverride::ClearOverlay(_actor, _female, ovl->area, data.slot);
    }

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