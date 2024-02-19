#pragma once


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
        std::string_view root;
        std::string_view set;
        std::string_view name;
        OverlayArea area;
        std::string_view event;
        std::string_view author;
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
        std::string_view bump;
        int slot = -1;
    };

    inline void from_json(const json& j, OverlayST& p) {
        j.at("name").get_to(p.name);
        j.at("section").get_to(p.set);

		std::string path = j.at("texture");
        p.path = std::format("{}\\{}", ST_PATH, path);
        
        std::string rawArea = j["area"];
        p.area = magic_enum::enum_cast<OverlayArea>(rawArea, magic_enum::case_insensitive).value_or(OverlayArea::Invalid);

        p.skipFileCheck = j.value("in_bsa", false);
		p.event = j.value("event", "");
		p.author = j.value("credit", "");

        // TODO: add requirement/conflict parsing
		// TODO: convert other keys to meta fields
    }
}