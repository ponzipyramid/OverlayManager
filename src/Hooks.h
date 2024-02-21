#pragma once 

namespace OM
{
	class Hooks
	{
	public:
		static void Install();
	private:
		static inline void UpdatePlayer(RE::Actor* a_actor, float a_delta);
		static inline void UpdateCharacter(RE::Actor* a_actor, float a_delta);
		inline static REL::Relocation<decltype(UpdatePlayer)> _UpdatePlayer;
		inline static REL::Relocation<decltype(UpdateCharacter)> _UpdateCharacter;
	};
}