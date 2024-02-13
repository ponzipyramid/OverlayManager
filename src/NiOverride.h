#pragma once

namespace OM {
    typedef void (*AddNodeOverrideIntFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int, int, bool);
    typedef void (*AddNodeOverrideFloatFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int, float, bool);
    typedef void (*AddNodeOverrideStringFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int, const char*, bool);
    
    typedef int (*GetNumOverlaysFunc)(RE::StaticFunctionTag*);

    typedef int (*GetNodeOverrideIntFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int);
    typedef float (*GetNodeOverrideFloatFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int);
    typedef RE::BSFixedString (*GetNodeOverrideStringFunc)(RE::StaticFunctionTag*, RE::TESObjectREFR*, bool, std::string, int, int);
    
    class NiOverride {
    public:
        static int GetNumBodyOverlays() {
            return _GetNumBodyOverlays(&_base);
        }

        static int GetNumHandOverlays() {
            return _GetNumHandOverlays(&_base);
        }

        static int GetNumFeetOverlays() {
            return _GetNumFeetOverlays(&_base);
        }

        static int GetNumFaceOverlays() {
            return _GetNumFaceOverlays(&_base);
        }

        static int GetNodeOverrideInt(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) {
            return _GetNodeOverrideInt(&_base, a_ref, a_female, a_node, a_key, a_index);
        }

        static float GetNodeOverrideFloat(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) {
            return _GetNodeOverrideFloat(&_base, a_ref, a_female, a_node, a_key, a_index);
        }
        
        static std::string GetNodeOverrideString(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index) {
            return std::string{_GetNodeOverrideString(&_base, a_ref, a_female, a_node, a_key, a_index).c_str()};
        }

        static void AddNodeOverrideInt(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index, int a_value, bool a_persist) {
            return _AddNodeOverrideInt(&_base, a_ref, a_female, a_node, a_key, a_index, a_value, a_persist);
        }

        static void AddNodeOverrideFloat(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index, float a_value, bool a_persist) {
            return _AddNodeOverrideFloat(&_base, a_ref, a_female, a_node, a_key, a_index, a_value, a_persist);
        }

        static void AddNodeOverrideString(RE::TESObjectREFR* a_ref, bool a_female, std::string a_node, int a_key, int a_index, std::string a_value, bool a_persist) {
            return _AddNodeOverrideString(&_base, a_ref, a_female, a_node, a_key, a_index, a_value.c_str(), a_persist);
        }

        static inline void Init() {
            HMODULE _baseAddress = GetModuleHandleA("skee64.dll");
            char* func_start;
            
            std::vector addresses = _addresses640;
            
            func_start = (char*)_baseAddress + addresses[0];
            _GetNumBodyOverlays = (GetNumOverlaysFunc) func_start;

            func_start = (char*)_baseAddress + addresses[1];
            _GetNumHandOverlays = (GetNumOverlaysFunc) func_start;

            func_start = (char*)_baseAddress + addresses[2];
            _GetNumFeetOverlays = (GetNumOverlaysFunc) func_start;
            
            func_start = (char*)_baseAddress + addresses[3];
            _GetNumFaceOverlays = (GetNumOverlaysFunc) func_start;

            func_start = (char*)_baseAddress + addresses[4];
            _GetNodeOverrideInt = (GetNodeOverrideIntFunc) func_start;

            func_start = (char*)_baseAddress + addresses[5];
            _GetNodeOverrideFloat = (GetNodeOverrideFloatFunc) func_start;

            func_start = (char*)_baseAddress + addresses[6];
            _GetNodeOverrideString = (GetNodeOverrideStringFunc) func_start;

            func_start = (char*)_baseAddress + addresses[7];
            _AddNodeOverrideInt = (AddNodeOverrideIntFunc) func_start;

            func_start = (char*)_baseAddress + addresses[8];
            _AddNodeOverrideFloat = (AddNodeOverrideFloatFunc) func_start;

            func_start = (char*)_baseAddress + addresses[9];
            _AddNodeOverrideString = (AddNodeOverrideStringFunc) func_start;
        }
    private:
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

        static inline std::vector<int> _addresses97 {  };
        static inline std::vector<int> _addresses640 { 0xA5EF0, 0xA5F00, 0xA5F10, 0xA5F20, 0xCA230, 0xCA160, 0xCA3C0, 0xC9AD0, 0xC9860, 0xC9D20 };
        static inline std::vector<int> _addresses1170 {  };

        static inline RE::StaticFunctionTag _base;
    };
}