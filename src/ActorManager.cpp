#include "ActorManager.h"
#include "Legacy.h"
#include "JCApi.h"

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

        auto contextOvls = Legacy::ConvertJArrayToVector(a_list);
        auto currOvls = thread->GetOverlaysByContext(a_context);

        std::vector<std::string_view> failed;
        std::vector<std::string_view> added;
        std::vector<std::string_view> removed;

        std::unordered_set<std::string_view> seen;
        for (auto ovl : contextOvls) {
            seen.insert(ovl.first);
        }

        for (auto ovl : currOvls) {
            if (!seen.contains(ovl)) {
                thread->RemoveOverlay(a_context, ovl);
                removed.push_back(ovl);
            }
        }

        for (auto ovl : contextOvls) {
            auto [color, alpha, glow, gloss, bump, _] = ovl.second;
            
            switch (thread->AddOverlay(a_context, ovl.first, color, alpha, glow, gloss, bump, "")) {
                case AddResult::Added:
                    added.emplace_back(ovl.first);
                    break;
                case AddResult::Failed:
                    failed.emplace_back(ovl.first);
                    break;
            }
        }

        // TODO: sync a_list
        // TODO: sync a_added
        // TODO: sync a_removed
    }
}

ActorThread* ActorManager::GetOrCreateActorThread(RE::Actor* a_target) {
    auto formId = a_target->GetFormID();
    if (!_actorThreads.count(formId)) _actorThreads.insert({formId, ActorThread{a_target}});
    return &_actorThreads.find(formId)->second;
}