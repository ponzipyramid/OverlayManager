#pragma once
#include "OverlayData.h"

namespace OM {
    enum SlotStatus {
        Unchecked,
        Open,
        External,
        Internal,
        ST,
    };

    class OverlayManager {
    public:
        //static void RefreshSlotStatus(RE::Actor* a_actor);
        //static void SetSlotStatus(RE::Actor* a_actor, std::string a_area, int slot);
        static bool SetupOverlays(RE::Actor* a_actor);
        static bool UpdateOverlays(RE::Actor* a_actor);
        static int GetAvailableSlot(RE::Actor*, std::string area, bool a_refreshStatus);
    private:
        static void ApplyOverlay(RE::Actor* target, OverlayData a_data);
        static void ClearOverlay(RE::Actor* a_target, OverlayData a_data);

        static std::map<std::pair<std::string, int>, SlotStatus> _slotStatus;
    };
}