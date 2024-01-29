#pragma once
#include "Script.h"

namespace OM {
    struct OverlayData {
        std::string area;
        int slot;
        std::string path;
        int color;
        int glow;
        bool gloss;
        std::string bump;
        float alpha;
    };
}