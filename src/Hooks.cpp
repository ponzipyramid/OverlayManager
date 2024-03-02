#include "Hooks.h"
#include "ActorManager.h"

using namespace OM;

void Hooks::Install()
{
	const auto offset = REL::Module::GetRuntime() != REL::Module::Runtime::VR ? 0x0AD : 0x0AF;

	REL::Relocation<std::uintptr_t> vtbl1{ RE::PlayerCharacter::VTABLE[0] };
	_UpdatePlayer = vtbl1.write_vfunc(offset, UpdatePlayer);

	REL::Relocation<std::uintptr_t> vtbl2{ RE::Character::VTABLE[0] };
	_UpdateCharacter = vtbl2.write_vfunc(offset, UpdateCharacter);
}

void Hooks::UpdatePlayer(RE::Actor* a_actor, float a_delta)
{
	_UpdatePlayer(a_actor, a_delta);
	if (auto thread = ActorManager::GetActorThread(a_actor))
		thread->Update();
}

void Hooks::UpdateCharacter(RE::Actor* a_actor, float a_delta)
{
	_UpdateCharacter(a_actor, a_delta);

	if (auto thread = ActorManager::GetActorThread(a_actor))
		thread->Update();
}