#pragma once

#include "Overlay.h"
#include "SKEE.h"
#include "Override/OverrideInterface.h"
#include "Override/LegacyOverrideInterface.h"
#include "Override/ModernOverrideInterface.h"

#define SKEE_PATH "Data/SKSE/Plugins/skee64.ini"

namespace OM {
    class NiOverride {
    public:
        static int GetNumOverlays(OverlayArea a_area) {
            switch (a_area) {
                case Body:
				    return _numBodyOvls;
                case Hands:
					return _numHandsOvls;
                case Feet:
					return _numFeetOvls;
                case Face:
					return _numFaceOvls;
                default:
                    return 0;
            }
		}

        static inline void CheckAndAddOverlays(RE::Actor* a_target)
        {
			if (!_interface->HasOverlays(a_target))
				_interface->AddOverlays(a_target);
        }

		static inline void CheckAndRemoveOverlays(RE::Actor* a_target) 
		{
			if (_interface->HasOverlays(a_target))
				_interface->RemoveOverlays(a_target);
		}

		static inline bool HasOverrideInSlot(RE::Actor* a_target, bool a_female, OverlayArea a_area, int a_slot)
		{
				auto node = GetNode(a_area, a_slot);

                if (_interface->HasNodeOverride(a_target, a_female, node, 9, 0)) {
					return true;
                }

                auto path = _interface->GetNodeOverrideString(a_target, a_female, node, 9, 0);

				return path != "" && path != BLANK_PATH;
		}

        static inline std::string GetPath(RE::Actor* a_target, bool a_female, OverlayArea a_area, int a_slot) {        
            auto nodeName = GetNode(a_area, a_slot);
				return _interface->GetNodeOverrideString(a_target, a_female, nodeName, 9, 0);
        }

        static void ApplyOverlay(RE::Actor* a_target, bool a_female, OverlayArea a_area, int a_slot, std::string_view a_path, int a_color, float a_alpha, int a_glow, bool a_gloss, std::string_view a_bump)
		{
			//logger::info("ApplyOverlay {} {} {} {} {} {} {} {} {} {}", a_target != nullptr, a_female, (int) a_area, a_slot, std::string(a_path), a_color, a_alpha, a_glow, a_gloss, std::string(a_bump));
			auto nodeName = GetNode(a_area, a_slot);

            _interface->AddNodeOverrideString(a_target, a_female, nodeName, 9, 0, std::string(a_path), true);

			if (a_bump != "")
					_interface->AddNodeOverrideString(a_target, a_female, nodeName, 9, 1, std::string(a_bump), true);

            _interface->AddNodeOverrideInt(a_target, a_female, nodeName, 7, -1, a_color, true);
			_interface->AddNodeOverrideInt(a_target, a_female, nodeName, 0, -1, a_glow, true);
			_interface->AddNodeOverrideFloat(a_target, a_female, nodeName, 1, -1, 1.0, true);
			_interface->AddNodeOverrideFloat(a_target, a_female, nodeName, 8, -1, a_alpha, true);

            if (a_gloss) {
				_interface->AddNodeOverrideFloat(a_target, a_female, nodeName, 2, -1, 5.0, true);
				_interface->AddNodeOverrideFloat(a_target, a_female, nodeName, 3, -1, 5.0, true);
            } else {
				_interface->AddNodeOverrideFloat(a_target, a_female, nodeName, 2, -1, 0.0, true);
				_interface->AddNodeOverrideFloat(a_target, a_female, nodeName, 3, -1, 0.0, true);
            }

			_interface->ApplyNodeOverrides(a_target);
		}

        static void ClearOverlay(RE::Actor* a_target, bool a_female, OverlayArea a_area, int a_slot) {
            auto nodeName = GetNode(a_area, a_slot);

			//logger::info("ClearOverlay: clearing {}", nodeName);

            _interface->AddNodeOverrideString(a_target, a_female, nodeName, 9, 0, "actors\\character\\overlays\\default.dds", true);
            
            if (_interface->HasNodeOverride(a_target, a_female, nodeName, 9, 1)) {
				_interface->AddNodeOverrideString(a_target, a_female, nodeName, 9, 1, "actors\\character\\overlays\\default.dds", true);
            }

            _interface->ApplyNodeOverrides(a_target);

            _interface->RemoveNodeOverride(a_target, a_female, nodeName, 9, 1);
			_interface->RemoveNodeOverride(a_target, a_female, nodeName, 9, 0);
            _interface->RemoveNodeOverride(a_target, a_female, nodeName, 7, -1);
            _interface->RemoveNodeOverride(a_target, a_female, nodeName, 0, -1);
            _interface->RemoveNodeOverride(a_target, a_female, nodeName, 8, -1);
        }

        static inline void Init() {
            bool useLegacy = true;

            if (auto iMap = SKEE::GetInterfaceMap()) {
				logger::info("got skee interface");
				if (auto overrideInt = SKEE::GetOverrideInterface(iMap)) {
					auto ver = overrideInt->GetVersion();

					logger::info("got override interface v{}", ver);

                    if (ver >= SKEE::IOverrideInterface::kCurrentPluginVersion) {
						useLegacy = false;
					} else {
						logger::info("interface not up to date");
					}
				} else {
			        logger::warn("could not get override interface");
				}
			} else {
				logger::warn("could not get skee interface");
            }

            if (useLegacy) {
				logger::info("using legacy interface");
				_interface = LegacyOverrideInterface::GetSingleton();
			} else {
				logger::info("using modern interface");
				_interface = ModernOverrideInterface::GetSingleton();
            }

			CSimpleIniA config;
			config.SetUnicode();

			SI_Error rc = config.LoadFile(SKEE_PATH);

			if (rc >= 0) {
				clib_util::ini::get_value(config, _numBodyOvls, "Overlays/Body", "iNumOverlays", "");
				clib_util::ini::get_value(config, _numHandsOvls, "Overlays/Hands", "iNumOverlays", "");
				clib_util::ini::get_value(config, _numFeetOvls, "Overlays/Feet", "iNumOverlays", "");
				clib_util::ini::get_value(config, _numFaceOvls, "Overlays/Face", "iNumOverlays", "");
			}

			logger::info("# Overlay: {} {} {} {}", _numBodyOvls, _numHandsOvls, _numFeetOvls, _numFaceOvls);
        }

        static inline std::string GetNode(OverlayArea a_area, int a_slot)
		{
			return std::format("{} [ovl{}]", magic_enum::enum_name(a_area), a_slot);
		}

    private:
		static inline OverrideInterface* _interface;
		static inline int _numBodyOvls = 0;
		static inline int _numFaceOvls = 0;
		static inline int _numHandsOvls = 0;
		static inline int _numFeetOvls = 0;
    };
}