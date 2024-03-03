#pragma once

#include "OverrideInterface.h"

namespace OM
{

	typedef void (*AddNodeOverrideIntFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int, int, bool);
	typedef void (*AddNodeOverrideFloatFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int, float, bool);
	typedef void (*AddNodeOverrideStringFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int, const char*, bool);

	typedef int (*GetNodeOverrideIntFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int);
	typedef float (*GetNodeOverrideFloatFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int);
	typedef RE::BSFixedString (*GetNodeOverrideStringFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int);

	typedef bool (*HasNodeOverrideFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int);

	typedef void (*RemoveNodeOverrideFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int);

	typedef void (*ApplyNodeOverridesFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*);
	typedef void (*AddOverlaysFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*);

	typedef bool (*HasOverlaysFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*);
	typedef void (*RemoveOverlaysFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*);

	class LegacyOverrideInterface : public OverrideInterface
	{
	public:
		static inline OverrideInterface* GetSingleton()
		{
			static LegacyOverrideInterface instance;

			static std::atomic_bool initialized;
			static std::latch latch(1);

			if (!initialized.exchange(true)) {
				// yes, this is terrible but there isn't a better option for 1.5/1.6.640 support

				HMODULE baseAddress = GetModuleHandleA("skee64.dll");

				if (baseAddress) {
					logger::info("initializing NiOverride API");
					char* func_start;

					auto& addresses = instance._addresses640;
					
					const auto ver = REL::Module::get().version();
					logger::info("current version: {} {} {}", ver.major(), ver.minor(), ver.patch());
					if (ver.major() == 1 && ver.minor() == 5 && ver.patch() == 97) {
						logger::info("switching to 1.5.97 ver");
						addresses = instance._addresses97;
					}

					func_start = (char*)baseAddress + addresses[0];
					instance._GetNodeOverrideInt = (GetNodeOverrideIntFunc)func_start;

					func_start = (char*)baseAddress + addresses[1];
					instance._GetNodeOverrideFloat = (GetNodeOverrideFloatFunc)func_start;

					func_start = (char*)baseAddress + addresses[2];
					instance._GetNodeOverrideString = (GetNodeOverrideStringFunc)func_start;

					func_start = (char*)baseAddress + addresses[3];
					instance._AddNodeOverrideInt = (AddNodeOverrideIntFunc)func_start;

					func_start = (char*)baseAddress + addresses[4];
					instance._AddNodeOverrideFloat = (AddNodeOverrideFloatFunc)func_start;

					func_start = (char*)baseAddress + addresses[5];
					instance._AddNodeOverrideString = (AddNodeOverrideStringFunc)func_start;

					func_start = (char*)baseAddress + addresses[6];
					instance._HasNodeOverride = (HasNodeOverrideFunc)func_start;

					func_start = (char*)baseAddress + addresses[7];
					instance._RemoveNodeOverride = (RemoveNodeOverrideFunc)func_start;

					func_start = (char*)baseAddress + addresses[8];
					instance._ApplyNodeOverrides = (ApplyNodeOverridesFunc)func_start;

					func_start = (char*)baseAddress + addresses[9];
					instance._AddOverlays = (AddOverlaysFunc)func_start;

					func_start = (char*)baseAddress + addresses[10];
					instance._HasOverlays = (HasOverlaysFunc)func_start;

					func_start = (char*)baseAddress + addresses[11];
					instance._RemoveOverlays = (RemoveOverlaysFunc)func_start;
				} else {
					logger::error("failed to find SKEE base address");
				}

				latch.count_down();
			}

			latch.wait();

			return &instance;
		}

		inline int GetNodeOverrideInt(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) override
		{
			return _GetNodeOverrideInt(&_base, a_ref, a_female, a_node, a_key, a_index);
		}

		inline float GetNodeOverrideFloat(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) override
		{
			return _GetNodeOverrideFloat(&_base, a_ref, a_female, a_node, a_key, a_index);
		}

		inline std::string GetNodeOverrideString(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) override
		{
			return std::string{ _GetNodeOverrideString(&_base, a_ref, a_female, a_node, a_key, a_index).c_str() };
		}

		inline void AddNodeOverrideInt(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index, int a_value, bool a_persist) override
		{
			return _AddNodeOverrideInt(&_base, a_ref, a_female, a_node, a_key, a_index, a_value, a_persist);
		}

		inline void AddNodeOverrideFloat(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index, float a_value, bool a_persist) override
		{
			return _AddNodeOverrideFloat(&_base, a_ref, a_female, a_node, a_key, a_index, a_value, a_persist);
		}

		inline void AddNodeOverrideString(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index, std::string a_value, bool a_persist) override
		{
			return _AddNodeOverrideString(&_base, a_ref, a_female, a_node, a_key, a_index, a_value.c_str(), a_persist);
		}

		inline bool HasNodeOverride(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) override
		{
			return _HasNodeOverride(&_base, a_ref, a_female, a_node, a_key, a_index);
		}

		inline void RemoveNodeOverride(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) override
		{
			return _RemoveNodeOverride(&_base, a_ref, a_female, a_node, a_key, a_index);
		}

		inline void AddOverlays(RE::TESObjectREFR* a_ref) override
		{
			return _AddOverlays(&_base, a_ref);
		}

		inline void ApplyNodeOverrides(RE::Actor* a_actor) override
		{
			return _ApplyNodeOverrides(&_base, a_actor);
		}

		inline bool HasOverlays(RE::TESObjectREFR* a_ref) override
		{
			return _HasOverlays(&_base, a_ref);
		}

		inline void RemoveOverlays(RE::TESObjectREFR* a_actor) override
		{
			return _RemoveOverlays(&_base, a_actor);
		}

		GetNodeOverrideIntFunc _GetNodeOverrideInt;
		GetNodeOverrideFloatFunc _GetNodeOverrideFloat;
		GetNodeOverrideStringFunc _GetNodeOverrideString;

		AddNodeOverrideIntFunc _AddNodeOverrideInt;
		AddNodeOverrideFloatFunc _AddNodeOverrideFloat;
		AddNodeOverrideStringFunc _AddNodeOverrideString;

		HasNodeOverrideFunc _HasNodeOverride;

		RemoveNodeOverrideFunc _RemoveNodeOverride;

		ApplyNodeOverridesFunc _ApplyNodeOverrides;
		AddOverlaysFunc _AddOverlays;
		HasOverlaysFunc _HasOverlays;
		RemoveOverlaysFunc _RemoveOverlays;

		std::vector<int> _addresses97{
			0xBE160,
			0xBE090,
			0xBE2F0,
			0xBD9E0,
			0xBD780,
			0xBDC30,
			0x99D80,
			0x9A1D0,
			0x99D30,
			0x99760,
			0x99780,
			0x997A0
		};
		std::vector<int> _addresses640{
			0xCA230,
			0xCA160,
			0xCA3C0,
			0xC9AD0,
			0xC9860,
			0xC9D20,
			0xA5940,
			0xA5CA0,
			0xA5930,
			0xA5350,
			0xA20A0,
			0xA53F0
		};

		RE::StaticFunctionTag _base;
	};
}