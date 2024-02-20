#pragma once
#include "ActorThread.h"

namespace OM {
    class ActorManager {
    public:
        static void SyncContext(RE::Actor* a_target, std::string a_context, int a_list, int a_added, int a_removed);
		static ActorThread* GetActorThread(RE::Actor* a_actor, bool a_create = true);
		static void Revert();
		static void Save(SKSE::SerializationInterface* a_intfc);
		static void Load(SKSE::SerializationInterface* a_intfc);
	private:
        static inline std::unordered_map<RE::FormID, ActorThread> _actorThreads;
    };
}