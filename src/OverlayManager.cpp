#include "OverlayManager.h"
#include "JCApi.h"
#include "NiOverride.h"
#include "OverlayData.h"
#include <chrono>
#include <thread>

using namespace std::chrono;
using namespace OM;
using namespace OM::JC;



bool OverlayManager::UpdateOverlays(RE::Actor* a_actor) {
    auto start = high_resolution_clock::now();
    if (!a_actor) return false;

    // ST compatibility
    if (auto applied = JFormDB::getObj(a_actor, ".SlaveTats.applied")) {
        logs::info("syncing ST entries");
        for (int i = JArray::count(applied) - 1; i >= 0; i--) {
            auto entry = JArray::getObj(applied, i);
            auto area = JMap::getStr(entry, "area");
            auto slot = JMap::getInt(entry, "slot");
            auto path = "Actors\\Character\\slavetats\\" + JMap::getStr(entry, "texture");
            auto color = JMap::getInt(entry, "color");
            auto glow = JMap::getInt(entry, "glow");
            auto gloss = (bool) JMap::getInt(entry, "gloss");
            auto bump = JMap::getStr(entry, "bump");
            auto alpha = 1.f - JMap::getFlt(entry, "invertedAlpha");

            OverlayData data = {
                area,
                slot,
                path,
                color,
                glow,
                gloss, 
                bump,
                alpha
            };
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
        
    logs::info("updating overlays on {} - took {}us", a_actor->GetFormID(), duration.count());

    return true;
}

int OverlayManager::GetAvailableSlot(RE::Actor*, std::string, bool) {
    return 0;
}