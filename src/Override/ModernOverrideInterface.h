#pragma once

#include "OverrideInterface.h"
#include "SKEE.h"

namespace OM
{
	class SetVariant : public SKEE::IOverrideInterface::SetVariant
	{
	public:
		SetVariant(std::string a_value)
		{
			_type = Type::String;
			_strValue = a_value.c_str();
		}
		SetVariant(int a_value)
		{
			_type = Type::Int;
			_intValue = a_value;
		}
		SetVariant(float a_value)
		{
			_type = Type::Float;
			_fltValue = a_value;
		}
		SetVariant(bool a_value)
		{
			_type = Type::Bool;
			_bValue = a_value;
		}
		SetVariant(RE::BGSTextureSet* a_value)
		{
			_type = Type::TextureSet;
			_tsValue = a_value;
		}

		inline Type GetType() override { return _type; }
		inline int32_t Int() override { return _intValue; }
		inline float Float() override { return _fltValue; }
		inline const char* String() override { return _strValue; }
		inline bool Bool() override { return _bValue; }
		inline RE::BGSTextureSet* TextureSet() override { return _tsValue; }

	private:
		Type _type;
		int32_t _intValue;
		float _fltValue;
		const char* _strValue;
		bool _bValue;
		RE::BGSTextureSet* _tsValue;
	};

	class GetVariant : public SKEE::IOverrideInterface::GetVariant
	{
	public:
		inline void Int(const int32_t i) { intValue = i; }
		inline void Float(const float f) { fltValue = f; }
		inline void String(const char* str) { strValue = str; }
		inline void Bool(const bool b) { bValue = b; }
		inline void TextureSet(const RE::BGSTextureSet*) { }

		int32_t intValue;
		float fltValue;
		const char* strValue;
		bool bValue;
	};

	class ModernOverrideInterface : public OverrideInterface
	{
	public:
		static inline OverrideInterface* GetSingleton()
		{
			static ModernOverrideInterface instance;
			
			static std::atomic_bool initialized;
			static std::latch latch(1);

			if (!initialized.exchange(true)) {
				if (auto iMap = SKEE::GetInterfaceMap()) {
					instance._overrideInterface = SKEE::GetOverrideInterface(iMap);
					instance._overlayInterface = SKEE::GetOverlayInterface(iMap);

					logger::info("initialized overrides = {}", instance._overrideInterface != nullptr);
					logger::info("initialized overlays = {}", instance._overlayInterface != nullptr);
				}

				latch.count_down();
			}

			latch.wait();

			return &instance;
		}

		inline int GetNodeOverrideInt(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) override
		{
			logger::info("Start");
			GetVariant var;
			if (_overrideInterface->GetNodeOverride(a_ref, a_female, a_node.c_str(), (uint16_t) a_key, (uint8_t) a_index, var)) {
				logger::info("Stop");
				return var.intValue;
			} else {
				logger::info("Stop");
				return 0;
			}
		}

		inline float GetNodeOverrideFloat(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) override
		{
			logger::info("Start");
			GetVariant var;
			if (_overrideInterface->GetNodeOverride(a_ref, a_female, a_node.c_str(), (uint16_t) a_key, (uint8_t) a_index, var)) {
				logger::info("Stop");
				return var.fltValue;
			} else {
				logger::info("Stop");
				return 0.f;
			}
		}

		inline std::string GetNodeOverrideString(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) override
		{
			logger::info("Start");
			GetVariant var;
			if (_overrideInterface->GetNodeOverride(a_ref, a_female, a_node.c_str(), (uint16_t) a_key, (uint8_t) a_index, var)) {
				logger::info("Stop");
				return std::string(var.strValue);
			} else {
				logger::info("Stop");
				return "";
			}
		}

		inline void AddNodeOverrideInt(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index, int a_value, bool) override
		{
			logger::info("Start");
			SetVariant var(a_value);
			_overrideInterface->AddNodeOverride(a_ref, a_female, a_node.c_str(), (uint16_t) a_key, (uint8_t) a_index, var);
			logger::info("Stop");
		}

		inline void AddNodeOverrideFloat(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index, float a_value, bool) override
		{
			logger::info("Start");
			SetVariant var(a_value);
			_overrideInterface->AddNodeOverride(a_ref, a_female, a_node.c_str(), (uint16_t) a_key, (uint8_t) a_index, var);
			logger::info("Stop");
		}

		inline void AddNodeOverrideString(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index, std::string a_value, bool) override
		{
			logger::info("Start");
			SetVariant var(a_value);
			_overrideInterface->AddNodeOverride(a_ref, a_female, a_node.c_str(), (uint16_t) a_key, (uint8_t) a_index, var);
			logger::info("Stop");
		}

		inline bool HasNodeOverride(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) override
		{
			logger::info("Start");
			auto ret = _overrideInterface->HasNodeOverride(a_ref, a_female, a_node.c_str(), (uint16_t)a_key, (uint8_t)a_index);
			logger::info("Stop");
			return ret;
		}

		inline void RemoveNodeOverride(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) override
		{
			logger::info("Start");
			_overrideInterface->RemoveNodeOverride(a_ref, a_female, a_node.c_str(), (uint16_t)a_key, (uint8_t)a_index);
			logger::info("Stop");
		}

		inline void AddOverlays(RE::TESObjectREFR* a_ref) override
		{
			logger::info("Start");
			_overlayInterface->AddOverlays(a_ref);
			logger::info("Stop");
		}

		inline void ApplyNodeOverrides(RE::Actor* a_actor) override
		{
			logger::info("Start");
			_overrideInterface->SetNodeProperties(a_actor, false);
			logger::info("Stop");
		}

		inline bool HasOverlays(RE::TESObjectREFR* a_ref) override
		{
			logger::info("Start");
			auto ret = _overlayInterface->HasOverlays(a_ref);
			logger::info("Stop");
			return ret;
		}

		inline void RemoveOverlays(RE::TESObjectREFR* a_ref) override
		{
			logger::info("Start");
			_overlayInterface->RemoveOverlays(a_ref);
			logger::info("Stop");
		}

	private:
		SKEE::IOverrideInterface* _overrideInterface;
		SKEE::IOverlayInterface* _overlayInterface;
	};
}