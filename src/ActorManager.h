#pragma once
#include "ActorThread.h"

namespace OM {
    class ActorManager {
    public:
        static void SyncContext(RE::Actor* a_target, std::string_view a_context, int a_list, int a_added, int a_removed);
    private:
        static ActorThread* GetOrCreateActorThread(RE::Actor* a_actor);

        static inline std::unordered_map<RE::FormID, ActorThread> _actorThreads;
    };
}