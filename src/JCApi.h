#pragma once
#include "JContainers/jc_interface.h"

namespace {
    template<class F>
    void obtain_func(const jc::reflection_interface *refl, const char* a_funcName, const char* a_className, F& a_func) {
        a_func = reinterpret_cast<F>(refl->tes_function_of_class(a_funcName, a_className));

        if (!a_func) {
            logger::error("JCApi: couldn't obtain function {}::{}", a_className, a_funcName);
        }

    }
}

namespace OM::JC {
    using Handle = std::uint32_t;
    
    inline void * default_domain = nullptr;

    inline int32_t(*JFormDB_getObj)(void*, RE::TESForm*, RE::BSFixedString) = nullptr;
    inline int32_t(*JArray_count)(void*, int32_t a_obj) = nullptr;
    inline int32_t(*JArray_getObj)(void*, int32_t a_obj, int32_t a_index) = nullptr;
    inline RE::BSFixedString(*JMap_getStr)(void*, int32_t a_obj, RE::BSFixedString a_key, RE::BSFixedString a_default) = nullptr;
    inline int32_t(*JMap_getInt)(void*, int32_t a_obj, RE::BSFixedString a_key, int32_t a_default) = nullptr;
	inline float (*JMap_getFlt)(void*, int32_t a_obj, RE::BSFixedString a_key, float a_default) = nullptr;
	inline void (*JArray_clear)(void*, int32_t a_obj) = nullptr;
	inline int (*JMap_object)(void*) = nullptr;
	inline void (*JArray_addObj)(void*, int32_t a_arr, int32_t a_obj, int32_t a_index) = nullptr;
	inline void (*JArray_eraseIndex)(void*, int32_t a_arr, int32_t a_index) = nullptr;
	inline void (*JMap_setInt)(void*, int32_t a_obj, RE::BSFixedString a_key, int a_value) = nullptr;
	inline void (*JMap_setFlt)(void*, int32_t a_obj, RE::BSFixedString a_key, float a_value) = nullptr;
	inline void (*JMap_setStr)(void*, int32_t a_obj, RE::BSFixedString a_key, RE::BSFixedString a_value) = nullptr;

    class Api {
    public:
        static inline void Init(const jc::root_interface* root) {
            if (root) {
				logger::info("initializing JC API");
            } else {
				logger::info("failed to initialize JC API");
				return;
            }

            auto refl = root->query_interface<jc::reflection_interface>();

            obtain_func(refl, "getObj", "JFormDB", JFormDB_getObj);
            obtain_func(refl, "count", "JArray", JArray_count);
            obtain_func(refl, "getObj", "JArray", JArray_getObj);
            obtain_func(refl, "getStr", "JMap", JMap_getStr);
            obtain_func(refl, "getInt", "JMap", JMap_getInt);
			obtain_func(refl, "getFlt", "JMap", JMap_getFlt);
			obtain_func(refl, "clear", "JArray", JArray_clear);
			obtain_func(refl, "object", "JMap", JMap_object);
			obtain_func(refl, "addObj", "JArray", JArray_addObj);
			obtain_func(refl, "eraseIndex", "JArray", JArray_eraseIndex);
			
            obtain_func(refl, "setFlt", "JMap", JMap_setFlt);
			obtain_func(refl, "SetInt", "JMap", JMap_setInt);
			obtain_func(refl, "setStr", "JMap", JMap_setStr);

            default_domain = root->query_interface<jc::domain_interface>()->get_default_domain();

            _initialised = true;

            logger::info("finished initializing JC API");
        }
        
        static inline bool IsInit() { return _initialised; }
    private:
        static inline bool _initialised = false;
    };

    class JFormDB {
    public:
        static inline Handle getObj(RE::TESForm* a_fKey, RE::BSFixedString a_key) {
			return JFormDB_getObj ? JFormDB_getObj(default_domain, a_fKey, a_key) : 0;
        }
    };

    class JArray {
    public:
        static inline int32_t count(Handle a_obj) {
			return JArray_count ? JArray_count(default_domain, a_obj) : 0;
        }

        static inline void addObj(Handle a_arr, Handle a_obj, int a_index = -1) {
			if (JArray_addObj)
			    JArray_addObj(default_domain, a_arr, a_obj, a_index);
        }

        static inline void eraseIndex(Handle a_arr, int a_index = -1)
		{
			if (JArray_eraseIndex)
			    JArray_eraseIndex(default_domain, a_arr, a_index);
		}

        static inline int32_t getObj(Handle a_obj, int32_t a_index) {
			return JArray_getObj ? JArray_getObj(default_domain, a_obj, a_index) : 0;
        }

        static inline int32_t clear(Handle a_obj)
		{
			if (JArray_clear)
				JArray_clear(default_domain, a_obj);
		}
    };

    class JMap {
    public:
		static inline int object()
		{
			return JMap_object ? JMap_object(default_domain) : 0;
		}

        static inline std::string getStr(Handle a_obj, RE::BSFixedString a_key, RE::BSFixedString a_default = "") {
			return JMap_getStr ? JMap_getStr(default_domain, a_obj, a_key, a_default).c_str() : "";
        }

        static inline int32_t getInt(Handle a_obj, RE::BSFixedString a_key, int32_t a_default = 0) {
			return JMap_getInt ? JMap_getInt(default_domain, a_obj, a_key, a_default) : 0;
        }

        static inline float getFlt(Handle a_obj, RE::BSFixedString a_key, float a_default = 0.f) {
			return JMap_getFlt ? JMap_getFlt(default_domain, a_obj, a_key, a_default) : 0.0f;
        }

        static inline void setStr(Handle a_obj, RE::BSFixedString a_key, RE::BSFixedString a_value = "")
		{
			if (JMap_getStr)
				JMap_setStr(default_domain, a_obj, a_key, a_value);
		}

		static inline void setInt(Handle a_obj, RE::BSFixedString a_key, int32_t a_value = 0)
		{
			if (JMap_getInt)
				JMap_setInt(default_domain, a_obj, a_key, a_value);
		}

		static inline void setFlt(Handle a_obj, RE::BSFixedString a_key, float a_value = 0.f)
		{
			if (JMap_getFlt) 
                JMap_setFlt(default_domain, a_obj, a_key, a_value);
		}
    };
}