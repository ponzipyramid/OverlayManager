#pragma once
#include "JCApi.h"
#include "Overlay.h"

namespace OM {
    namespace Legacy {
        inline std::vector<std::pair<std::string_view, OverlayData>> PopulateVector(int) {
            std::vector<std::pair<std::string_view, OverlayData>> overlays;
            // TODO: implement
            return overlays;
        }

        inline void PopulateJArray(std::vector<std::pair<std::string_view, Overlay*>>, int, bool) {
            // TODO: implement
        }
    }
}