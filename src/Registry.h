#pragma once
#include "Overlay.h"

namespace fs = std::filesystem;
using json = nlohmann::json;

namespace OM {
    class Registry {
    public:
        static inline void Read() {
			auto dir = std::format("{}\\{}", PREFIX_PATH, ST_PATH);
			logger::info("Directory: {}", dir);

            for (const auto& entry : fs::directory_iterator(dir)) {
                auto path = entry.path();

                if (entry.is_directory()) continue;
                if (path.extension() != ".json") continue;
                
                logger::info("processing: {}", path.string());

                std::ifstream f(entry.path());
                json data = json::parse(f);

                auto tats = data.template get<std::vector<OverlayST>>();
                
                _overlays.reserve(tats.size());

                std::set<std::string_view> seen;
                for (auto& tat : tats) {
                    if (!tat.IsValid()) {
                        logger::info("skipping {}/{}: invalid data", tat.set, tat.name);
                        continue;
                    }
                    if (seen.contains(tat.path)) {
                        logger::info("skipping {}/{}: duplicate path", tat.set, tat.name);
                        continue;
                    }

                    _overlays.push_back(tat);
                    seen.insert(tat.path);
                }
            }

            logger::info("Num STs found: {}", _overlays.size());

            for (auto& ovl : _overlays) {
				_overlaysByContext["slavetats"].push_back(&ovl);
			}


            // TODO: ingest all OM JSONs

            for (auto& ovl : _overlays) {
                _overlaysById[ovl.path] = &ovl;
            }
        };
        
        static inline void Init() {
            // TODO: convert all meta field forms
        }

        static inline Overlay* GetOverlay(std::string_view a_id) { return _overlaysById[a_id]; }
		static inline std::vector<Overlay*> GetOverlaysByContext(std::string a_context) { return _overlaysByContext[a_context]; }
    private:
		static inline std::vector<Overlay> _overlays;
		static inline std::unordered_map<std::string, std::vector<Overlay*>> _overlaysByContext;
        static inline std::unordered_map<std::string_view, Overlay*> _overlaysById;
    };
}