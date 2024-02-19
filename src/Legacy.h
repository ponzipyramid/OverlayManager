#pragma once
#include "JCApi.h"
#include "Overlay.h"
#include "Registry.h"

using namespace OM::JC;

namespace OM {
    namespace Legacy {
        inline std::vector<std::pair<std::string, OverlayData>> PopulateVector(int a_list) {
            std::vector<std::pair<std::string, OverlayData>> overlays;

            if (!a_list) return overlays;

            int count = JArray::count(a_list);
			overlays.reserve(count);

            for (int i = 0; i < count; i++) {
				auto tat = JArray::getObj(a_list, i);
				
                auto id = std::format("{}\\{}", ST_PATH, JMap::getStr(tat, "texture"));
				logger::info("Legacy::PopulateVector: {}", id);
				OverlayData od{
					JMap::getInt(tat, "color"),
					1 - JMap::getFlt(tat, "invertedAlpha"),
					JMap::getInt(tat, "glow"),
					JMap::getInt(tat, "gloss"),
					JMap::getStr(tat, "bump"),
					-1
				};

                overlays.push_back({ id, od });
            }

            return overlays;
        }
    }
}