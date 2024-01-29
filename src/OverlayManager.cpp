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

            ApplyOverlay(a_actor, data);
        }
    }

    NiOverride::ApplyNodeOverrides(a_actor);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
        
    logs::info("updating overlays on {} - took {}us", a_actor->GetFormID(), duration.count());

    return true;
}

int OverlayManager::GetAvailableSlot(RE::Actor*, std::string, bool) {
    return 0;
}

void OverlayManager::ApplyOverlay(RE::Actor* target, OverlayData a_data) {
    auto [ area, slot, path, color, glow, gloss, bump, alpha ] = a_data;

    logs::info("Applying {} Overlay: {} {} {} {} {} {} {} {}", target->GetFormID(), area, slot, path, color, glow, gloss, bump, alpha);

    bool isFemale = target->GetActorBase()->GetSex() == 1;
    auto nodeName = std::format("{} [ovl{}]", area, slot);

    NiOverride::AddNodeOverrideString(target, isFemale, nodeName, 9, 0, path, true);

    if (bump != "")
        NiOverride::AddNodeOverrideString(target, isFemale, nodeName, 9, 1, bump, true);

    NiOverride::AddNodeOverrideInt(target, isFemale, nodeName, 7, -1, color, true);
    NiOverride::AddNodeOverrideInt(target, isFemale, nodeName, 0, -1, glow, true);
    NiOverride::AddNodeOverrideFloat(target, isFemale, nodeName, 1, -1, 1.0, true);
    NiOverride::AddNodeOverrideFloat(target, isFemale, nodeName, 8, -1, alpha, true);
    
    if (gloss) {
        NiOverride::AddNodeOverrideFloat(target, isFemale, nodeName, 2, -1, 5.0, true);
        NiOverride::AddNodeOverrideFloat(target, isFemale, nodeName, 3, -1, 5.0, true);
    } else {
        NiOverride::AddNodeOverrideFloat(target, isFemale, nodeName, 2, -1, 0.0, true);
        NiOverride::AddNodeOverrideFloat(target, isFemale, nodeName, 3, -1, 0.0, true);
    }
}

void OverlayManager::ClearOverlay(RE::Actor* a_target, OverlayData a_data) {
    auto [ area, slot, path, color, glow, gloss, bump, alpha ] = a_data;
    
    logs::info("Removing {} Overlay: {} {}", a_target->GetFormID(), area, slot);

    bool isFemale = a_target->GetActorBase()->GetSex() == 1;
    auto nodeName = std::format("{} [ovl{}]", area, slot);

    NiOverride::AddNodeOverrideString(a_target, isFemale, nodeName, 9, 1, "Actors\\Character\\slavetats\\blank.dds", true);
    NiOverride::RemoveNodeOverride(a_target, isFemale, nodeName, 9, 1);
    NiOverride::RemoveNodeOverride(a_target, isFemale, nodeName, 9, 0);
    NiOverride::RemoveNodeOverride(a_target, isFemale, nodeName, 7, -1);
    NiOverride::RemoveNodeOverride(a_target, isFemale, nodeName, 0, -1);
    NiOverride::RemoveNodeOverride(a_target, isFemale, nodeName, 8, -1);
}