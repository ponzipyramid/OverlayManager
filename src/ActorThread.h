#pragma once
#include "Overlay.h"
#include "NiOverride.h"
#include "Util.h"

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
		ActorThread(SKSE::SerializationInterface* a_intfc);

        AddResult AddOverlay(std::string a_context, std::string a_id, int a_color, float a_alpha, int a_glow, int a_gloss, std::string a_replaceId = "", int a_slot = -1);        
        bool RemoveOverlay(std::string a_context, std::string a_id);

        inline std::unordered_set<std::string_view> GetOverlaysByContext(std::string a_context) { return _contexts[a_context]; }
        inline OverlayData* GetOverlayData(std::string a_id) { return _active.count(a_id) ? &_active[a_id] : nullptr; }
		std::vector<int> GetExternalOverlaySlots(std::string a_context, OverlayArea a_area);
		int GetAvailableSlot(OverlayArea a_area, int a_applied = 0);
		void Update();
		inline bool HasActiveOverlays() { return !_active.empty(); }
		void Serialize(SKSE::SerializationInterface* a_intfc);
		bool IsValid();
		inline RE::Actor* GetActor() { return _actor; } 
        inline std::string GetSlotId(OverlayArea a_area, int a_slot) { return NiOverride::GetPath(_actor, _female, a_area, a_slot); }
		inline bool IsSlotOpen(OverlayArea a_area, int a_slot);
        inline std::size_t GetNumActive() { return _active.size(); }
	private:
        RE::Actor* _actor = nullptr;
        bool _female;

        std::unordered_map<std::string, OverlayData> _active;
        std::unordered_map<std::string, std::unordered_set<std::string_view>> _contexts;

		bool _initialized = false;
    };
}