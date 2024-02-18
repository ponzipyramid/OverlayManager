#include "ActorManager.h"
#include "Legacy.h"
#include "JCApi.h"
#include "Registry.h"

using namespace OM;

void ActorManager::SyncContext(RE::Actor* a_target, std::string_view a_context, int a_list, int a_added, int a_removed) {
    if (!a_target) return;
    if (!JC::Api::IsInit()) return;

    if (auto thread = GetOrCreateActorThread(a_target)) {
        // get overlays in context
        // diff with list 
        // add new overlays
        // clear removed overlays
        // populate lists

        auto contextOvls = Legacy::PopulateVector(a_list);
        auto currOvls = thread->GetOverlaysByContext(a_context);

        std::vector<std::pair<std::string_view, Overlay*>> curr;
        std::vector<std::pair<std::string_view, Overlay*>> added;
        std::vector<std::pair<std::string_view, Overlay*>> removed;

        std::unordered_set<std::string_view> seen;
        for (auto ovl : contextOvls) {
            seen.insert(ovl.first);
        }

        for (auto ovl : currOvls) {
            if (!seen.contains(ovl)) {
                thread->RemoveOverlay(a_context, ovl);
                removed.push_back({ovl, Registry::GetOverlay(ovl)});
            }
        }

        for (auto ovl : contextOvls) {
            auto [color, alpha, glow, gloss, bump, _] = ovl.second;

            std::pair pair = {ovl.first, Registry::GetOverlay(ovl.first)};

            switch (thread->AddOverlay(a_context, ovl.first, color, alpha, glow, gloss, bump, "")) {
                case AddResult::Added:
                    added.emplace_back(pair);
                    /* FALLTHROUGH */
                case AddResult::Modified:
                    curr.emplace_back(pair);
                    break;
                case AddResult::Failed:
                    curr.emplace_back(pair);
                    break;
            }
        }
        
        Legacy::PopulateJArray(curr, a_list);
        Legacy::PopulateJArray(added, a_added);
        Legacy::PopulateJArray(removed, a_removed);
    }
}

ActorThread* ActorManager::GetOrCreateActorThread(RE::Actor* a_target) {
    auto formId = a_target->GetFormID();
    if (!_actorThreads.count(formId)) _actorThreads.insert({formId, ActorThread{a_target}});
    return &_actorThreads.find(formId)->second;
}