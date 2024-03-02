#include "ActorManager.h"
#include "Util.h"
#include "JCApi.h"
#include "Registry.h"
#include "Serialization.hpp"
#include "Config.h"

using namespace OM;

void ActorManager::SyncContext(RE::Actor* a_target, std::string a_context, int a_list, int a_added, int a_removed) {
    if (!a_target) return;
    if (!JC::Api::IsInit()) return;


    if (auto thread = GetActorThread(a_target)) {

        auto count = JArray::count(a_list);
		auto delay = Config::GetApplicationDelay();

        std::vector<int> arr;
        for (int i = 0; i < count; i++) {
			arr.emplace_back(JArray::getObj(a_list, i));
        }

		auto contextOvls = Util::PopulateVector(a_list);

		auto currIds = thread->GetOverlaysByContext(a_context);

		std::unordered_map<std::string_view, int> seen;
		for (int i = 0; i < contextOvls.size(); i++) {
			seen[contextOvls[i].first] = i;
        }

		//thread->Print();

        for (auto id : currIds) {
			if (!seen.contains(id)) {
				thread->RemoveOverlay(a_context, std::string(id));
				JArray::addObj(a_removed, seen[id]);
            }
        }

        for (int i = 0; i < count; i++) {
			auto [id, data] = contextOvls[i];
            auto [color, alpha, glow, gloss, slot] = data;

			switch (thread->AddOverlay(a_context, id, color, alpha, glow, gloss, "", slot)) {
			case AddResult::Failed:
				JArray::eraseIndex(a_list, i);
				break;
			case AddResult::Added:
				JArray::addObj(a_added, seen[id]);
                // FALLTHROUGH
			default:
				JMap::setInt(arr[i], "slot", thread->GetOverlayData(id)->slot);
				break;
			}

			if (i < (count - 1)) {
				
				std::this_thread::sleep_for(delay);
			}
        }

		//thread->Print();
    }
}

ActorThread* ActorManager::GetActorThread(RE::Actor* a_target, bool a_create)
{
	auto formId = a_target->GetFormID();
	if (!_actorThreads.count(formId)) {
		if (a_create) {
			logger::info("creating actor thread {}", a_target->GetFormID());
			_actorThreads.insert({ formId, ActorThread{ a_target } });
		} else {
			return nullptr;
		}
	}
	return &_actorThreads.find(formId)->second;
}

void ActorManager::Revert() 
{
	_actorThreads.clear();
}

void ActorManager::Load(SKSE::SerializationInterface* a_intfc)
{
	auto count = Serialization::Read<std::size_t>(a_intfc);

	logger::info("deserializing {} threads", count);
	for (; count > 0; --count) {
		auto thread = ActorThread(a_intfc);
		
		if (thread.IsValid()) {
			auto formId = thread.GetActor()->GetFormID();

			_actorThreads.insert({ formId, thread });
			logger::info("fetched {} active", GetActorThread(thread.GetActor())->GetNumActive());
		} else {
			logger::info("failed to deserialize thread");
		}
	}
}

void ActorManager::Save(SKSE::SerializationInterface* a_intfc)
{
	logger::info("ActorManager::Save");

	std::size_t count = 0;
	for (auto& [_, thread] : _actorThreads) {
		if (thread.HasActiveOverlays()) {
			count++;
		}
	}

	logger::info("serializing {} threads", count);
	Serialization::Write<std::size_t>(a_intfc, count);
	for (auto& [_, thread] : _actorThreads) {
		if (thread.HasActiveOverlays()) {
			thread.Serialize(a_intfc);
		}
	}
}