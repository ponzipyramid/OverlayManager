#pragma once

namespace OM
{
	class OverrideInterface
	{
	public:
		virtual int GetNodeOverrideInt(RE::TESObjectREFR*, bool, std::string, int, int) = 0;
		virtual float GetNodeOverrideFloat(RE::TESObjectREFR*, bool, std::string, int, int) = 0;
		virtual std::string GetNodeOverrideString(RE::TESObjectREFR*, bool, std::string, int, int) = 0;
		virtual void AddNodeOverrideFloat(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index, float a_value, bool a_persist) = 0;
		virtual void AddNodeOverrideString(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index, std::string a_value, bool a_persist) = 0;
		virtual void AddNodeOverrideInt(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index, int a_value, bool a_persist) = 0;
		virtual bool HasNodeOverride(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) = 0;
		virtual void RemoveNodeOverride(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) = 0;
		virtual void AddOverlays(RE::TESObjectREFR* a_ref) = 0;
		virtual void ApplyNodeOverrides(RE::Actor* a_actor) = 0;
		virtual bool HasOverlays(RE::TESObjectREFR* a_actor) = 0;
		virtual void RemoveOverlays(RE::TESObjectREFR* a_actor) = 0;
	};
}