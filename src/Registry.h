#pragma once
#include "Overlay.h"

#define ST_PATH "data/textures/actors/character/slavetats"
#define OM_PATH "data/textures/om"

namespace fs = std::filesystem;
using json = nlohmann::json;

namespace OM {
    class Registry {
    public:
        static inline void Read() {
            for (const auto& entry : fs::directory_iterator(ST_PATH)) {
                auto path = entry.path();

                if (entry.is_directory()) continue;
                if (path.extension() != ".json") continue;
                
                logs::info("processing: {}", path.string());


                std::ifstream f(entry.path());
                json data = json::parse(f);

                auto tats = data.template get<std::vector<OverlayST>>();
                _overlays.insert(_overlays.end(), tats.begin(), tats.end());

                logs::info("Num STs found: {}", _overlays.size());
            }

            // TODO: ingest all OM JSONs
        };
        static inline void Init() {
            // TODO: convert all meta field forms
        }

        static inline Overlay* GetOverlay(std::string_view a_id) { return _overlaysById[a_id]; }
    private:
        static inline std::vector<Overlay> _overlays;
        static inline std::unordered_map<std::string_view, Overlay*> _overlaysById;
    };
}