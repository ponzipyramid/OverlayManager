#pragma once

using json = nlohmann::json;

namespace OM {
    enum OverlayArea {
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
        std::string_view path;
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
        j.at("texture").get_to(p.path);
        j.at("section").get_to(p.set);
        // TODO: add requirement/conflict parsing
        // TODO: convert remaining to meta fields
        // TODO: add area parsing

        // j.at("in_bsa").get_to(p.skipFileCheck);
        // j.at("event").get_to(p.event);
        // j.at("credit").get_to(p.author);
    }
}