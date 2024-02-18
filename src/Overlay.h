#pragma once

using json = nlohmann::json;

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
            // TODO: implement file existence check
            return path != "" && set != "" && name != "" && area != OverlayArea::Invalid;
        }
        virtual std::string_view GetPath() {
			return path;
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
        j.at("texture").get_to(p.path);
        
        std::string rawArea = j["area"];
        p.area = magic_enum::enum_cast<OverlayArea>(rawArea, magic_enum::case_insensitive).value_or(OverlayArea::Invalid);
        

        // TODO: add area parsing
        
        // TODO: add requirement/conflict parsing
        // TODO: convert remaining to meta fields

        // j.at("in_bsa").get_to(p.skipFileCheck);
        // j.at("event").get_to(p.event);
        // j.at("credit").get_to(p.author);
    }
}