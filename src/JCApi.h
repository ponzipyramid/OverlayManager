#pragma once
#include "JContainers/jc_interface.h"

namespace {

    template<class F>
    void obtain_func(const jc::reflection_interface *refl, const char* a_funcName, const char* a_className, F& a_func) {
        a_func = reinterpret_cast<F>(refl->tes_function_of_class(a_funcName, a_className));

        if (!a_func) {
            logs::error("JCApi: couldn't obtain function {}::{}", a_className, a_funcName);
        }

    }
}

namespace OM {
    
    namespace JC {
        using Handle = std::uint32_t;
        
        inline void * default_domain = nullptr;

        inline int32_t(*JFormDB_getObj)(void*, RE::TESForm*, RE::BSFixedString) = nullptr;
        inline int32_t(*JArray_count)(void*, int32_t a_obj) = nullptr;
        inline int32_t(*JArray_getObj)(void*, int32_t a_obj, int32_t a_index) = nullptr;
        inline RE::BSFixedString(*JMap_getStr)(void*, int32_t a_obj, RE::BSFixedString a_key, RE::BSFixedString a_default) = nullptr;
        inline int32_t(*JMap_getInt)(void*, int32_t a_obj, RE::BSFixedString a_key, int32_t a_default) = nullptr;
        inline float(*JMap_getFlt)(void*, int32_t a_obj, RE::BSFixedString a_key, float a_default) = nullptr;

        namespace Api {
            inline void Init(const jc::root_interface* root) {
                auto refl = root->query_interface<jc::reflection_interface>();

                obtain_func(refl, "getObj", "JFormDB", JFormDB_getObj);
                obtain_func(refl, "count", "JArray", JArray_count);
                obtain_func(refl, "getObj", "JArray", JArray_getObj);
                obtain_func(refl, "getStr", "JMap", JMap_getStr);
                obtain_func(refl, "getInt", "JMap", JMap_getInt);
                obtain_func(refl, "getFlt", "JMap", JMap_getFlt);

                default_domain = root->query_interface<jc::domain_interface>()->get_default_domain();

                logs::info("finished initializing JC API");
            }
        };
        namespace JFormDB {
            inline Handle getObj(RE::TESForm* a_fKey, RE::BSFixedString a_key) {
                return JFormDB_getObj(default_domain, a_fKey, a_key);
            }
        }

        namespace JArray {
            inline int32_t count(Handle a_obj) {
                return JArray_count(default_domain, a_obj);
            }

            inline int32_t getObj(Handle a_obj, int32_t a_index) {
                return JArray_getObj(default_domain, a_obj, a_index);
            }
        }

        namespace JMap {
            inline std::string getStr(Handle a_obj, RE::BSFixedString a_key, RE::BSFixedString a_default = "") {
                return JMap_getStr(default_domain, a_obj, a_key, a_default).c_str();
            }

            inline int32_t getInt(Handle a_obj, RE::BSFixedString a_key, int32_t a_default = 0) {
                return JMap_getInt(default_domain, a_obj, a_key, a_default);
            }

            inline float getFlt(Handle a_obj, RE::BSFixedString a_key, float a_default = 0.f) {
                return JMap_getFlt(default_domain, a_obj, a_key, a_default);
            }
        }
    }
    
}