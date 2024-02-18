#pragma once
#include "Overlay.h"
#include "NiOverride.h"

namespace OM {
    enum AddResult {
        Failed,
        Added,
        Modified
    };

    class ActorThread {
    public:
        ActorThread(RE::Actor* a_actor) { 
            _actor = a_actor; 
            _female = _actor->GetActorBase()->GetSex() == 0;
        }

        AddResult AddOverlay(std::string_view a_context, std::string_view a_id, int a_color, float a_alpha, int a_glow, int a_gloss, std::string_view a_bump, std::string_view a_replaceId);        
        bool RemoveOverlay(std::string_view a_context, std::string_view a_id);

        inline std::unordered_set<std::string_view> GetOverlaysByContext(std::string_view a_context) { return _contexts[a_context]; }
    private:
        int GetAvailableSlot(OverlayArea a_area);
        
        RE::Actor* _actor;
        bool _female;
        std::unordered_map<std::string_view, OverlayData> _active;
        std::unordered_map<std::string_view, std::unordered_set<std::string_view>> _contexts;
    };
}