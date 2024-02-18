#pragma once
#include "Overlay.h"

#define PREFIX "Actors\\Character\\slavetats"

namespace OM {

    typedef void (*AddNodeOverrideIntFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int, int, bool);
    typedef void (*AddNodeOverrideFloatFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int, float, bool);
    typedef void (*AddNodeOverrideStringFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int, const char*, bool);
    
    typedef int (*GetNumOverlaysFunc)(RE::StaticFunctionTag*);

    typedef int (*GetNodeOverrideIntFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int);
    typedef float (*GetNodeOverrideFloatFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int);
    typedef RE::BSFixedString (*GetNodeOverrideStringFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int);
    
    typedef bool (*HasNodeOverrideFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int);
    
    typedef void (*RemoveNodeOverrideFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int);

    class NiOverride {
    public:
        static int GetNumOverlays(OverlayArea a_area) {
            switch (a_area) {
                case Body:
                    return GetNumBodyOverlays();
                case Hands:
                    return GetNumHandOverlays();
                case Feet:
                    return GetNumFeetOverlays();
                case Face:
                    return GetNumFaceOverlays();
                default:
                    return 0;
            }
        }

        static std::string GetPath(RE::Actor* a_target, bool a_female, OverlayArea a_area, int a_slot) {        
            auto nodeName = GetNode(a_area, a_slot);
            return GetNodeOverrideString(a_target, a_female, nodeName, 9, 0);
        }

        static void ApplyOverlay(RE::Actor* a_target, bool a_female, OverlayArea a_area, int a_slot, std::string_view a_path, int a_color, float a_alpha, int a_glow, bool a_gloss, std::string_view a_bump)
		{
            auto nodeName = GetNode(a_area, a_slot);

            AddNodeOverrideString(a_target, a_female, nodeName, 9, 0, std::string(a_path), true);

            if (a_bump != "")
                AddNodeOverrideString(a_target, a_female, nodeName, 9, 1, std::string(a_bump), true);

            AddNodeOverrideInt(a_target, a_female, nodeName, 7, -1, a_color, true);
            AddNodeOverrideInt(a_target, a_female, nodeName, 0, -1, a_glow, true);
            AddNodeOverrideFloat(a_target, a_female, nodeName, 1, -1, 1.0, true);
            AddNodeOverrideFloat(a_target, a_female, nodeName, 8, -1, a_alpha, true);

            if (a_gloss) {
                AddNodeOverrideFloat(a_target, a_female, nodeName, 2, -1, 5.0, true);
                AddNodeOverrideFloat(a_target, a_female, nodeName, 3, -1, 5.0, true);
            } else {
                AddNodeOverrideFloat(a_target, a_female, nodeName, 2, -1, 0.0, true);
                AddNodeOverrideFloat(a_target, a_female, nodeName, 3, -1, 0.0, true);
            }
        }

        static void ClearOverlay(RE::Actor* a_target, bool a_female, OverlayArea a_area, int a_slot) {
            auto nodeName = GetNode(a_area, a_slot);

            AddNodeOverrideString(a_target, a_female, nodeName, 9, 0, std::format("{}\\blank.dds", PREFIX), true);
            
            if (HasNodeOverride(a_target, a_female, nodeName, 9, 1)) {
                AddNodeOverrideString(a_target, a_female, nodeName, 9, 1, std::format("{}\\blank.dds", PREFIX), true);
                RemoveNodeOverride(a_target, a_female, nodeName, 9, 1);
            }

            RemoveNodeOverride(a_target, a_female, nodeName, 9, 0);
            RemoveNodeOverride(a_target, a_female, nodeName, 7, -1);
            RemoveNodeOverride(a_target, a_female, nodeName, 0, -1);
            RemoveNodeOverride(a_target, a_female, nodeName, 8, -1);
        }

        static inline void Init() {
            // TODO: check version at runtime and swap addresses
            // use actual racemenu if game version > 640

            HMODULE baseAddress = GetModuleHandleA("skee64.dll");
            char* func_start; 

            std::vector addresses = _addresses640;
            
            func_start = (char*)baseAddress + addresses[0];
            _GetNumBodyOverlays = (GetNumOverlaysFunc) func_start;

            func_start = (char*)baseAddress + addresses[1];
            _GetNumHandOverlays = (GetNumOverlaysFunc) func_start;

            func_start = (char*)baseAddress + addresses[2];
            _GetNumFeetOverlays = (GetNumOverlaysFunc) func_start;
            
            func_start = (char*)baseAddress + addresses[3];
            _GetNumFaceOverlays = (GetNumOverlaysFunc) func_start;

            func_start = (char*)baseAddress + addresses[4];
            _GetNodeOverrideInt = (GetNodeOverrideIntFunc) func_start;

            func_start = (char*)baseAddress + addresses[5];
            _GetNodeOverrideFloat = (GetNodeOverrideFloatFunc) func_start;

            func_start = (char*)baseAddress + addresses[6];
            _GetNodeOverrideString = (GetNodeOverrideStringFunc) func_start;

            func_start = (char*)baseAddress + addresses[7];
            _AddNodeOverrideInt = (AddNodeOverrideIntFunc) func_start;

            func_start = (char*)baseAddress + addresses[8];
            _AddNodeOverrideFloat = (AddNodeOverrideFloatFunc) func_start;

            func_start = (char*)baseAddress + addresses[9];
            _AddNodeOverrideString = (AddNodeOverrideStringFunc) func_start;

            func_start = (char*)baseAddress + addresses[10];
            _HasNodeOverride = (HasNodeOverrideFunc) func_start;

            func_start = (char*)baseAddress + addresses[11];
            _RemoveNodeOverride = (RemoveNodeOverrideFunc) func_start;
        }
    private:
        static inline std::string GetNode(OverlayArea a_area, int a_slot) {
            return std::format("{} [ovl{}]", magic_enum::enum_name(a_area), a_slot);
        }

        static inline int GetNumBodyOverlays() {
            return _GetNumBodyOverlays(&_base);
        }

        static inline int GetNumHandOverlays() {
            return _GetNumHandOverlays(&_base);
        }

        static inline int GetNumFeetOverlays() {
            return _GetNumFeetOverlays(&_base);
        }

        static inline int GetNumFaceOverlays() {
            return _GetNumFaceOverlays(&_base);
        }

        static inline int GetNodeOverrideInt(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) {
            return _GetNodeOverrideInt(&_base, a_ref, a_female, a_node, a_key, a_index);
        }

        static inline float GetNodeOverrideFloat(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) {
            return _GetNodeOverrideFloat(&_base, a_ref, a_female, a_node, a_key, a_index);
        }
        
        static inline std::string GetNodeOverrideString(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) {
            return std::string{_GetNodeOverrideString(&_base, a_ref, a_female, a_node, a_key, a_index).c_str()};
        }

        static inline void AddNodeOverrideInt(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index, int a_value, bool a_persist) {
            return _AddNodeOverrideInt(&_base, a_ref, a_female, a_node, a_key, a_index, a_value, a_persist);
        }

        static inline void AddNodeOverrideFloat(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index, float a_value, bool a_persist) {
            return _AddNodeOverrideFloat(&_base, a_ref, a_female, a_node, a_key, a_index, a_value, a_persist);
        }

        static inline void AddNodeOverrideString(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index, std::string a_value, bool a_persist) {
            return _AddNodeOverrideString(&_base, a_ref, a_female, a_node, a_key, a_index, a_value.c_str(), a_persist);
        }

        static inline bool HasNodeOverride(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) {
            return _HasNodeOverride(&_base, a_ref, a_female, a_node, a_key, a_index);
        }

        static inline void RemoveNodeOverride(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) {
            return _RemoveNodeOverride(&_base, a_ref, a_female, a_node, a_key, a_index);
        }

        static inline GetNumOverlaysFunc _GetNumBodyOverlays;
        static inline GetNumOverlaysFunc _GetNumHandOverlays;
        static inline GetNumOverlaysFunc _GetNumFeetOverlays;
        static inline GetNumOverlaysFunc _GetNumFaceOverlays;

        static inline GetNodeOverrideIntFunc _GetNodeOverrideInt;
        static inline GetNodeOverrideFloatFunc _GetNodeOverrideFloat;
        static inline GetNodeOverrideStringFunc _GetNodeOverrideString;
        
        static inline AddNodeOverrideIntFunc _AddNodeOverrideInt;
        static inline AddNodeOverrideFloatFunc _AddNodeOverrideFloat;
        static inline AddNodeOverrideStringFunc _AddNodeOverrideString;

        static inline HasNodeOverrideFunc _HasNodeOverride;
        
        static inline RemoveNodeOverrideFunc _RemoveNodeOverride;

        static inline std::vector<int> _addresses97 {  };
        static inline std::vector<int> _addresses640 { 0xA5EF0, 0xA5F00, 0xA5F10, 0xA5F20, 0xCA230, 0xCA160, 0xCA3C0, 0xC9AD0, 0xC9860, 0xC9D20, 0xA5CA0, 0xA20A0 };

        static inline RE::StaticFunctionTag _base;
    };
}