#pragma once
#include "Serialization.hpp"

#define PREFIX_PATH "data\\textures"
#define ST_PATH "actors\\character\\slavetats"
#define OM_PATH "data\\textures\\om"
#define BLANK_PATH "actors\\character\\overlays\\default.dds"

namespace
{
	constexpr std::string_view Delimeter = "|"; 

	template <typename T>
	inline std::optional<T> Parse(std::string a_str)
	{
		return std::nullopt;
	}

	template <>
	inline std::optional<RE::TESForm*> Parse(std::string a_str)
	{
		auto pos = a_str.find("|");
		auto token = a_str.substr(0, pos);

		if (!token.empty()) {
			try {
				auto formId = std::stoi(token, 0, 16);
				auto plugin = a_str.substr(pos + 1, a_str.size());

				if (auto form = RE::TESDataHandler::GetSingleton()->LookupForm(formId, plugin)) {
					return std::optional(form);
				}
			} catch (...) {}
		}

		return std::nullopt;
	}

	template <>
	inline std::optional<float> Parse(std::string a_str)
	{
		try {
			return std::stof(a_str);
		} catch (...) {
			return std::nullopt;
		}
	}
}

namespace OM {
    enum OverlayArea {
        Invalid,
        Body,
        Hands,
        Feet,
        Face,
    };

    const std::vector<OverlayArea> Areas{ Body, Hands, Feet, Face };

    enum MetaType {
		None,
		String,
		Form,
        Numeric
    };

    struct MetaField {
		MetaField() = default;
        MetaField(std::string a_val) 
		{
			if (auto pForm = Parse<RE::TESForm*>(a_val)) {
				type = MetaType::Form;
				form = pForm.value();
			} else if (auto pNum = Parse<float>(a_val)) {
				type = MetaType::Numeric;
				numeric = pNum.value();
			} else {
				type = MetaType::String;
			}
			str = a_val;
        }
		MetaField(float a_val) 
		{
			type = MetaType::Numeric;
			numeric = a_val;
			str = std::to_string(a_val);
		}
		MetaField(RE::TESForm* a_val)
		{
			type = MetaType::Form;
			form = a_val;
			str = std::format("{}|{}", a_val->GetFormID(), a_val->GetFile()->GetFilename());
		}

		bool operator==(MetaField const& a_ovl)
		{
			if (type == MetaType::None || a_ovl.type == MetaType::None)
				return false;
			
			if (type != a_ovl.type)
				return false;

			switch (type) {
			case MetaType::Form:
				return form == a_ovl.form;
			case MetaType::Numeric:
				return numeric == a_ovl.numeric;
			case MetaType::String:
				return str == a_ovl.str;
			default:
				return false;
			}
		}

		MetaType type = MetaType::None;
        std::string str;
		RE::TESForm* form;
		float numeric;
    };

    struct Overlay {
        inline bool IsValid() {
			return path != "" && (skipFileCheck || fs::exists(std::format("{}\\{}", PREFIX_PATH, path))) && set != "" && name != "" && area != OverlayArea::Invalid;
        }

        std::string path;
		std::string base;
		std::string set;
        std::string name;
        OverlayArea area;
		std::string event;
		std::string author;
		std::string bump;
		std::string domain;
        bool skipFileCheck;

        std::map<std::string, MetaField> meta;

		// LATER: maybe refactor
		inline std::string GetMetaStr(std::string a_key, std::string a_default = "")
		{
			if (meta.count(a_key))
				return meta[a_key].str;
			else
				return a_default;
		}

		inline float GetMetaNum(std::string a_key, float a_default = 0.f)
		{
			if (meta.count(a_key) && meta[a_key].type == MetaType::Numeric)
				return meta[a_key].numeric;
			else
				return a_default;
		}

		inline RE::TESForm* GetMetaForm(std::string a_key, RE::TESForm* a_default = nullptr)
		{
			if (meta.count(a_key) && meta[a_key].type == MetaType::Form)
				return meta[a_key].form;
			else
				return a_default;
		}
    };

    struct OverlayST : Overlay {};

    struct OverlayData {
        int color;
        float alpha;
        int glow;
        int gloss;
        int slot = -1;

        
		OverlayData() = default;
		OverlayData(int a_color, float a_alpha, int a_glow, int a_gloss, int a_slot)
		{
			color = a_color;
			alpha = a_alpha;
			glow = a_glow;
			gloss = a_gloss;
			slot = a_slot;
        }
		OverlayData(SKSE::SerializationInterface* a_intfc)
		{
			color = Serialization::Read<int>(a_intfc);
			alpha = Serialization::Read<float>(a_intfc);
			glow = Serialization::Read<int>(a_intfc);
			gloss = Serialization::Read<int>(a_intfc);
			slot = Serialization::Read<int>(a_intfc);
		}
		inline void Serialize(SKSE::SerializationInterface* a_intfc)
		{
			Serialization::Write<int>(a_intfc, color);
			Serialization::Write<float>(a_intfc, alpha);
			Serialization::Write<int>(a_intfc, glow);
			Serialization::Write<int>(a_intfc, gloss);
			Serialization::Write<int>(a_intfc, slot);
		}

    };

    const std::unordered_set<std::string> primaryKeys{ "name", "section", "texture", "area", "in_bsa", "event", "credit", "domain" };
    inline void from_json(const json& j, OverlayST& p) {
        j.at("name").get_to(p.name);
        j.at("section").get_to(p.set);

		p.base = j.at("texture");
		p.path = std::format("{}\\{}", ST_PATH, p.base);
        
        std::string rawArea = j.value("area", "");
        p.area = magic_enum::enum_cast<OverlayArea>(rawArea, magic_enum::case_insensitive).value_or(OverlayArea::Invalid);

        p.skipFileCheck = j.value("in_bsa", false);
		p.event = j.value("event", "");
		p.author = j.value("credit", "");
		p.domain = j.value("domain", "");
		
		std::map<std::string, std::string> plugins;
		std::vector<std::pair<std::string, int>> formIds;

        for (auto& [key, val] : j.items()) {
            if (!primaryKeys.contains(key)) {
				if (key.ends_with("_plugin")) {
					plugins[key.substr(key.size() - 7)] = val;
				} else if (key.ends_with("_formid")) {
					std::pair pair = { key.substr(key.size() - 7), val };
					formIds.emplace_back(pair);
				} else {
					p.meta[key] = MetaField((std::string) val);
				}
            }
		}

		auto handler = RE::TESDataHandler::GetSingleton();
		for (auto& [key, formId] : formIds) {
			if (key == "")
				continue;

			if (plugins.count(key)) {
				if (auto form = handler->LookupForm(formId, plugins[key])) {
					p.meta[key + "_form"] = MetaField(form);
				}
			}
		}
    }
}