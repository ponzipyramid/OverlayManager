#include "OverlayManager.h"
#include "JCApi.h"
#include "NiOverride.h"
#include <chrono>

using namespace std::chrono;
using namespace OM;
using namespace OM::JC;

bool OverlayManager::UpdateOverlays(RE::Actor* a_actor) {
    auto start = high_resolution_clock::now();
    logs::info("updating overlays on {}", a_actor->GetFormID());

    if (!a_actor) return false;

    // ST compatibility
    auto applied = JFormDB::getObj(a_actor, ".SlaveTats.applied");
    logs::info("got st applied handle: {}", applied);
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

        ApplyOverlay(a_actor, area, slot, path, color, glow, gloss, bump, alpha);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    
    logs::info("updating overlays on {} - {}us", a_actor->GetFormID(), duration.count());

    NiOverride::ApplyNodeOverrides(a_actor);

    return true;
}

void OverlayManager::ApplyOverlay(RE::Actor* target, std::string area, int slot, std::string path, int color, int glow, bool gloss, std::string bump, float alpha) {
    bool isFemale = target->GetActorBase()->GetSex() == 1;
    auto nodeName = std::format("{} [ovl{}]", area, slot);

    logs::info("Applying {} Overlay: {} {} {} {} {} {} {} {} {} {}", target->GetFormID(), area, slot, nodeName, isFemale, path, color, glow, gloss, bump, alpha);

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