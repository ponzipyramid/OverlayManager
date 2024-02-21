#pragma once
#include "Serialization.hpp"

#define PREFIX_PATH "data\\textures"
#define ST_PATH "actors\\character\\slavetats"
#define OM_PATH "data\\textures\\om"
#define BLANK_PATH "om\\blank.dds"

using json = nlohmann::json;
namespace fs = std::filesystem;

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
        Form,
        Int,
        String,
        Float
    };

    struct MetaField {
        MetaType type;
        std::string strValue;
        int intValue;
        float fltValue;
        std::string rawFormValue;
        RE::TESForm* formValue;
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
        bool skipFileCheck;

        std::unordered_map<std::string, MetaField> meta;
        std::unordered_map<std::string, MetaField> requirements;
        std::unordered_map<std::string, MetaField> conflicts;
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

    inline void from_json(const json& j, OverlayST& p) {
        j.at("name").get_to(p.name);
        j.at("section").get_to(p.set);

		p.base = j.at("texture");
		p.path = std::format("{}\\{}", ST_PATH, p.base);
        
        std::string rawArea = j["area"];
        p.area = magic_enum::enum_cast<OverlayArea>(rawArea, magic_enum::case_insensitive).value_or(OverlayArea::Invalid);

        p.skipFileCheck = j.value("in_bsa", false);
		p.event = j.value("event", "");
		p.author = j.value("credit", "");

        // TODO: add requirement/conflict parsing
		// TODO: convert other keys to meta fields
    }
}