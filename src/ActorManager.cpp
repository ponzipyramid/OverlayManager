#include "ActorManager.h"
#include "Util.h"
#include "JCApi.h"
#include "Registry.h"

using namespace OM;

void ActorManager::SyncContext(RE::Actor* a_target, std::string a_context, int a_list, int a_added, int a_removed) {
    if (!a_target) return;
    if (!JC::Api::IsInit()) return;

    logger::info("Syncing now");

    if (auto thread = GetActorThread(a_target)) {
		logger::info("Syncing for real");
		// get overlays in context
        // diff with list 
        // add new overlays
        // clear removed overlays
        // populate lists

        auto count = JArray::count(a_list);
        
        std::vector<int> arr;
        for (int i = 0; i < count; i++) {
			arr.emplace_back(JArray::getObj(a_list, i));
        }

        auto contextOvls = Util::PopulateVector(a_list);
        auto currIds = thread->GetOverlaysByContext(a_context);

        logger::info("contextOvls: {}", contextOvls.size());
		logger::info("currIds for {}: {}", a_context, currIds.size());


        std::unordered_map<std::string_view, int> seen;
		for (int i = 0; i < contextOvls.size(); i++) {
			seen[contextOvls[i].first] = i;
        }

        for (auto id : currIds) {
			if (!seen.contains(id)) {
				logger::info("{} not in list", id);
				thread->RemoveOverlay(a_context, std::string(id));
				JArray::addObj(a_removed, seen[id]);
            }
        }

        for (int i = 0; i < count; i++) {
			auto [id, data] = contextOvls[i];
            auto [color, alpha, glow, gloss, bump, slot] = data;

            // TODO: fix bump
			switch (thread->AddOverlay(a_context, id, color, alpha, glow, gloss, "", "", slot)) {
			case AddResult::Failed:
				logger::info("failed to add");
				JArray::eraseIndex(a_list, seen[id]);
				break;
			case AddResult::Added:
				JArray::addObj(a_added, seen[id]);
                // FALLTHROUGH
			default:
				JMap::setInt(arr[i], "slot", thread->GetOverlayData(id)->slot);
				break;
			}
        }

        NiOverride::ApplyNodeOverrides(a_target);
    }
}

ActorThread* ActorManager::GetActorThread(RE::Actor* a_target, bool a_create)
{
	auto formId = a_target->GetFormID();
	if (!_actorThreads.count(formId)) {
		if (a_create)
			_actorThreads.insert({ formId, ActorThread{ a_target } });
		else
			return nullptr;
	}
	return &_actorThreads.find(formId)->second;
}

void ActorManager::Revert() 
{
	_actorThreads.clear();
}

void ActorManager::Load(SKSE::SerializationInterface* a_intfc)
{
	std::size_t num_actors;
	a_intfc->ReadRecordData(&num_actors, sizeof(num_actors));

	for (; num_actors > 0; --num_actors) {
		auto thread = ActorThread(a_intfc);
		
		if (thread.IsValid()) {
			_actorThreads[thread.GetActor()->GetFormID()] = thread;
		}
	}
}

void ActorManager::Save(SKSE::SerializationInterface* a_intfc)
{
	std::size_t count;
	for (auto& [_, thread] : _actorThreads) {
		if (thread.HasActiveOverlays()) {
			count++;
		}
	}

	a_intfc->WriteRecordData(&count, sizeof(count));

	for (auto& [_, thread] : _actorThreads) {
		if (thread.HasActiveOverlays()) {
			thread.Serialize(a_intfc);
		}
	}
}