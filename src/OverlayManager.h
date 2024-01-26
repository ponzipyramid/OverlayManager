#pragma once
#include "OverlayData.h"

namespace OM {
    class OverlayManager {
    public:
        static bool UpdateOverlays(RE::Actor*);
    private:
        static void ApplyOverlay(RE::Actor* target, std::string area, int slot, std::string path, int color, int glow, bool gloss, std::string bump = "", float alpha = 1.0);

        static std::unordered_map<RE::Actor*, OverlayData> _data;
    };
}