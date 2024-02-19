#include "ActorManager.h"
#include "Legacy.h"
#include "JCApi.h"
#include "Registry.h"

using namespace OM;

void ActorManager::SyncContext(RE::Actor* a_target, std::string_view a_context, int a_list, int a_added, int a_removed) {
    if (!a_target) return;
    if (!JC::Api::IsInit()) return;

    logger::info("Syncing now");

    if (auto thread = GetOrCreateActorThread(a_target)) {
		logger::info("Syncing for real");
		// get overlays in context
        // diff with list 
        // add new overlays
        // clear removed overlays
        // populate lists

        auto contextOvls = Legacy::PopulateVector(a_list);
        auto currIds = thread->GetOverlaysByContext(a_context);

        logger::info("contextOvls: {}", contextOvls.size());


        std::unordered_map<std::string_view, int> seen;
		for (int i = 0; i < contextOvls.size(); i++) {
			seen[contextOvls[i].first] = i;
        }

        for (auto id : currIds) {
			if (!seen.contains(id)) {
				thread->RemoveOverlay(a_context, id);
				JArray::addObj(a_removed, seen[id]);
            }
        }

        for (auto [id, data] : contextOvls) {
            auto [color, alpha, glow, gloss, bump, _] = data;

            logger::info("Adding overlay {}", id.data());

            switch (thread->AddOverlay(a_context, id, color, alpha, glow, gloss, bump, "")) {
			case AddResult::Added:
				JArray::addObj(a_added, seen[id]);
				break;
			case AddResult::Failed:
				JArray::eraseIndex(a_list, seen[id]);
				break;
			}
        }

        NiOverride::ApplyNodeOverrides(a_target);
    }
}

ActorThread* ActorManager::GetOrCreateActorThread(RE::Actor* a_target) {
    auto formId = a_target->GetFormID();
    if (!_actorThreads.count(formId)) _actorThreads.insert({formId, ActorThread{a_target}});
    return &_actorThreads.find(formId)->second;
}