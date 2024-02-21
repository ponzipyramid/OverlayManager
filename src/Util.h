#pragma once
#include "JCApi.h"
#include "Overlay.h"
#include "Registry.h"
#include <algorithm>
using namespace OM::JC;

namespace OM {
    namespace Util {
		inline void Lowercase(std::string a_str) {
			std::transform(a_str.begin(), a_str.end(), a_str.begin(),
				[](unsigned char c) { return (char) std::tolower(c); });
		}

        inline std::vector<std::pair<std::string, OverlayData>> PopulateVector(int a_list) 
		{
            std::vector<std::pair<std::string, OverlayData>> overlays;

            if (!a_list) return overlays;

            int count = JArray::count(a_list);
			overlays.reserve(count);

            for (int i = 0; i < count; i++) {
				auto tat = JArray::getObj(a_list, i);
				
                auto id = std::format("{}\\{}", ST_PATH, JMap::getStr(tat, "texture"));
				OverlayData od{
					JMap::getInt(tat, "color"),
					1 - JMap::getFlt(tat, "invertedAlpha"),
					JMap::getInt(tat, "glow"),
					JMap::getInt(tat, "gloss"),
					JMap::getInt(tat, "slot")
				};

                overlays.push_back({ id, od });
            }

            return overlays;
        }

		inline OverlayArea GetArea(std::string_view a_area)
		{
			return magic_enum::enum_cast<OverlayArea>(a_area, magic_enum::case_insensitive).value_or(OverlayArea::Invalid);
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

			auto result = (a1 != -1) && a1 != a2;
			
			/*if (result)
				logger::info("DiffFieldsInt ({}): {} = {}", a_key, a1, a2, result);
			*/
			return result;
		}


		inline bool DoesOverlayMatch(int a_template, int a_ovl, bool a_runtime = false, int a_applied = 0)
		{

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

			if (DiffFieldsInt(a_template, a_ovl, "domain"))
				return false;

			if (a_applied) {
				auto req = JMap::getStr(a_ovl, "requires");
				auto excl = JMap::getStr(a_ovl, "excluded_by");

				auto reqsMet = req == "";
				auto conflict = false;

				int count = JArray::count(a_applied);
				for (int i = 0; i < count; i++) {
					auto tat = JArray::getObj(a_applied, i);
					auto texture = JMap::getStr(tat, "texture");
					auto id = std::format("{}\\{}", ST_PATH, texture);

					if (auto ovl = Registry::GetOverlay(id)) {
						if (ovl->meta[req].str == "1")
							reqsMet = true;
						if (ovl->meta[excl].str == "1") {
							conflict = true;
							break;
						}
					}
				}

				if (!reqsMet || conflict) {
					return false;
				}
			}

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

		inline Handle CreateObject(Overlay* a_ovl) 
		{
			auto obj = JMap::object();

			/*
			name
			section
			texture
			area
			excluded_by
			requires
			requires_plugin
			requires_formid
			domain
			*/

			JMap::setStr(obj, "name", a_ovl->name);
			JMap::setStr(obj, "section", a_ovl->set);
			JMap::setStr(obj, "texture", a_ovl->base);
			JMap::setStr(obj, "area", magic_enum::enum_name(a_ovl->area));
			JMap::setStr(obj, "domain", a_ovl->domain);
			JMap::setStr(obj, "requires", a_ovl->domain);
			JMap::setStr(obj, "excluded_by", std::string{ a_ovl->GetMetaStr("excluded_by") });
			JMap::setStr(obj, "requires", std::string{ a_ovl->GetMetaStr("requires") });

			return obj;
		}

		inline void GetAllMatchingOverlays(std::string a_context, int a_template, int a_matches, int a_applied)
		{
			logger::info("GetAllMatchingOverlays");
			
			auto ovls = Registry::GetOverlaysByContext(a_context);
			for (auto ovl : ovls) {
				if (!ovl)
					return;

				auto obj = CreateObject(ovl);
				if (DoesOverlayMatch(a_template, obj, false, a_applied)) {
					JArray::addObj(a_matches, obj);
				}
			}
		}
    }
}