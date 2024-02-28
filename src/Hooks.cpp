#include "Hooks.h"
#include "ActorManager.h"

using namespace OM;

void Hooks::Install()
{
	REL::Relocation<std::uintptr_t> vtbl{ RE::Character::VTABLE[0] };
	_UpdateCharacter = vtbl.write_vfunc(REL::Module::GetRuntime() != REL::Module::Runtime::VR ? 0x0AD : 0x0AF, UpdateCharacter);
}


void Hooks::UpdateCharacter(RE::Actor* a_actor, float a_delta)
{
	_UpdateCharacter(a_actor, a_delta);
	if (auto thread = ActorManager::GetActorThread(a_actor))
		thread->Update();
}