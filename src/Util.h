#pragma once
#include "JCApi.h"
#include "Overlay.h"
#include "Registry.h"

using namespace OM::JC;

namespace OM {
    namespace Util {
        inline std::vector<std::pair<std::string, OverlayData>> PopulateVector(int a_list) 
		{
            std::vector<std::pair<std::string, OverlayData>> overlays;

            if (!a_list) return overlays;

            int count = JArray::count(a_list);
			overlays.reserve(count);

            for (int i = 0; i < count; i++) {
				auto tat = JArray::getObj(a_list, i);
				
                auto id = std::format("{}\\{}", ST_PATH, JMap::getStr(tat, "texture"));
				logger::info("Util::PopulateVector: {}", id);
				OverlayData od{
					JMap::getInt(tat, "color"),
					1 - JMap::getFlt(tat, "invertedAlpha"),
					JMap::getInt(tat, "glow"),
					JMap::getInt(tat, "gloss"),
					JMap::getStr(tat, "bump"),
					JMap::getInt(tat, "slot")
				};

                overlays.push_back({ id, od });
            }

            return overlays;
        }

		inline OverlayArea GetArea(std::string_view a_area)
		{
			return magic_enum::enum_cast<OverlayArea>(a_area).value_or(OverlayArea::Invalid);
		}

		inline bool IsSlotOpen(std::string_view a_path) {
			// TODO: validate this works
			return a_path == "" || a_path == BLANK_PATH || a_path == "actors\\character\\overlays\\default.dds";
		}

		inline bool WildcardMatch(std::string_view a_template, std::string_view a_str)
		{
			if (a_template == "ANY" || a_template == "")
				return true;

			if (a_template.size() != a_str.size())
				return false;

			if (a_template == a_str)
				return true;

			for (int i = 0; i < a_str.size(); i++) {
				if (a_template[i] != '*' && a_template[i] != a_str[i]) {
					return false;
				}
			} 
      
			return true;
		}

		inline bool DiffFieldsStr(int a_template, int a_obj, std::string_view a_key)
		{
			auto a1 = JMap::getStr(a_template, a_key);
			auto a2 = JMap::getStr(a_obj, a_key);

			auto result = !WildcardMatch(a1, a2);
			
			/*if (result)
				logger::info("DiffFieldsStr ({}): {} = {}", a_key, a1, a2, result);*/
			return result;
		}

		inline bool DiffFieldsInt(int a_template, int a_obj, std::string_view a_key)
		{
			auto a1 = JMap::getInt(a_template, a_key, -1);
			auto a2 = JMap::getInt(a_obj, a_key);

			auto result = (a1 == -1) || a1 != a2;
			
			/*if (result)
				logger::info("DiffFieldsInt ({}): {} = {}", a_key, a1, a2, result);
			*/
			return result;
		}


		inline bool DoesOverlayMatch(int a_template, int a_ovl, bool a_runtime)
		{
			// TODO: implement

			if (DiffFieldsStr(a_template, a_ovl, "name"))
				return false;

			if (DiffFieldsStr(a_template, a_ovl, "section"))
				return false;

			if (DiffFieldsStr(a_template, a_ovl, "texture"))
				return false;

			if (DiffFieldsStr(a_template, a_ovl, "area"))
				return false;

			if (DiffFieldsInt(a_template, a_ovl, "slot"))
				return false;

			if (a_runtime) {
				if (DiffFieldsInt(a_template, a_ovl, "color")) {
					return false;
				}

				if (DiffFieldsInt(a_template, a_ovl, "glow")) {
					return false;
				}

				if (DiffFieldsInt(a_template, a_ovl, "gloss")) {
					return false;
				}
			}

			return true;
		}

		inline std::vector<int> GetMatchingOverlays(int a_template, int a_list, int a_matches)
		{
			//logger::info("GetMatchingOverlays {} {} {}", a_template, a_list, a_matches);

			std::vector<int> matches;

			auto count = JArray::count(a_list);
			for (int i = 0; i < count; i++) {
				auto tat = JArray::getObj(a_list, i);

				//logger::info("checking tat {} {}", tat, JMap::getStr(tat, "name"));


				if (tat && DoesOverlayMatch(a_template, tat, false)) {
					//logger::info("tat matches");
					matches.push_back(tat);
					if (a_matches)
						JArray::addObj(a_matches, tat);
				} else {
					//logger::info("tat does not match");		
				}
			}

			return matches;
		}
    }
}

/*

if (overlay_path != "") && (StringUtil.Substring(overlay_path, 0, prefix_length) != prefix) && (overlay_path != suffix) && (StringUtil.SubString(overlay_path, StringUtil.GetLength(overlay_path) - suffix_length - 1) != ("\\" + suffix))
    Debug.Trace("SlaveTats: Found external overlay: " + overlay_path)
    JArray.addInt(matches, i)
else
    overlay_path = NiOverride.GetNodeOverrideString(target, isFemale, nodeName, 9, 6)
    if (overlay_path != "") && (StringUtil.Substring(overlay_path, 0, prefix_length) != prefix) && (overlay_path != suffix) && (StringUtil.SubString(overlay_path, StringUtil.GetLength(overlay_path) - suffix_length - 1) != ("\\" + suffix))
        Debug.Trace("SlaveTats: Found external non-file overlay: " + overlay_path)
        JArray.addInt(matches, i)
    endif
endif

*/