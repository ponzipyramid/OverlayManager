#pragma once
#include "Overlay.h"
#include "NiOverride.h"

namespace OM {
    enum AddResult {
        Failed,
        Added,
        Modified,
        Replaced
    };

    class ActorThread {
    public:
        ActorThread(RE::Actor* a_actor) { 
            _actor = a_actor; 
            _female = _actor->GetActorBase()->GetSex() == 1;

            if (a_actor->GetFormID() != 14) {
				NiOverride::CheckAndAddOverlays(a_actor);
			}
        }

        AddResult AddOverlay(std::string a_context, std::string a_id, int a_color, float a_alpha, int a_glow, int a_gloss, std::string a_bump, std::string a_replaceId = "", int a_slot = -1);        
        bool RemoveOverlay(std::string a_context, std::string a_id);

        inline std::unordered_set<std::string_view> GetOverlaysByContext(std::string a_context) { return _contexts[a_context]; }
        inline OverlayData* GetOverlayData(std::string a_id) { return _active.count(a_id) ? &_active[a_id] : nullptr; }
		std::vector<int> GetExternalOverlaySlots(std::string a_context, OverlayArea a_area);
		int GetAvailableSlot(OverlayArea a_area);

	private:
        RE::Actor* _actor;
        bool _female;
        std::unordered_map<std::string, OverlayData> _active;
        std::unordered_map<std::string, std::unordered_set<std::string_view>> _contexts;
    };
}