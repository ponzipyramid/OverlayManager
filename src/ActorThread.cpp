#include "ActorThread.h"
#include "Registry.h"

using namespace OM;


int ActorThread::GetAvailableSlot(OverlayArea a_area) {
    int max = NiOverride::GetNumOverlays(a_area);
    for (int i = 0; i < max; i++) {
        auto path = NiOverride::GetPath(_actor, _female, a_area, i);

        // TODO: improve check
        if (path == "") { 
            return i;
        }
    }
    return -1;
}

AddResult ActorThread::AddOverlay(std::string_view a_context, std::string_view a_id, int a_color, float a_alpha, int a_glow, int a_gloss, std::string_view a_bump, std::string_view a_replaceId) {
    AddResult result = AddResult::Failed;
    
    if (!Registry::GetOverlay(a_id)) return result;

    if (auto ovl = Registry::GetOverlay(a_id)) {
        auto& [color, alpha, glow, gloss, bump, slot] = _active[a_id];

        color = a_color;
		alpha = a_alpha;
		glow = a_glow;
		gloss = a_gloss;
		bump = a_bump;

        if (_active.count(a_replaceId) && NiOverride::GetPath(_actor, _female, ovl->area, slot) == a_id) {
			slot = _active[a_replaceId].slot;
        } else if (slot < 0 || NiOverride::GetPath(_actor, _female, ovl->area, slot) != a_id) {
			slot = GetAvailableSlot(ovl->area);
		}

        if (slot < 0) return result;

        NiOverride::ApplyOverlay(_actor, _female, ovl->area, slot, ovl->GetPath(), color, alpha, glow, gloss, bump);

        _contexts[a_context].insert(ovl->GetPath());

        return result;

    }

    return result;
}

bool ActorThread::RemoveOverlay(std::string_view a_context, std::string_view a_id) {

    if (!_active.count(a_id)) return false;

    auto ovl = Registry::GetOverlay(a_id);
    auto data = _active[a_id];
    
    // don't clear if we've been overwritten by smth else
    if (NiOverride::GetPath(_actor, _female, ovl->area, data.slot) == a_id)
        NiOverride::ClearOverlay(_actor, _female, ovl->area, data.slot);

    _active.erase(a_id);
    _contexts[a_context].erase(a_id);

    return true;
}