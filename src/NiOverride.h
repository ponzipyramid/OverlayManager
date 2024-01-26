#pragma once
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

namespace OM {
    namespace NiOverride {
        template<typename T>
        inline void AddNodeOverride(std::string a_type, RE::Actor* a_actor, bool a_female, RE::BSFixedString a_node, int a_key, int a_index, T a_value, bool a_persist) {
            logs::info("Invoking AddNodeOverride{} {} {} {} {} {} {} {}", a_type, a_actor->GetFormID(), a_female, a_node.c_str(), a_key, a_index, a_value, a_persist);
            RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
            auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
            auto args = RE::MakeFunctionArguments(std::move(a_actor->As<RE::TESObjectREFR>()), std::move(a_female), std::move(a_node), std::move(a_key), std::move(a_index), std::move(a_value), std::move(a_persist));
            vm->DispatchStaticCall("NiOverride",  "AddNodeOverride" + a_type, args, callback);
        }

        inline void AddNodeOverrideFloat(RE::Actor* a_actor, bool a_female, RE::BSFixedString a_node, int a_key, int a_index, float a_value, bool a_persist) {
           AddNodeOverride<float>("Float", a_actor, a_female, a_node, a_key, a_index, a_value, a_persist);
        }

        inline void AddNodeOverrideInt(RE::Actor* a_actor, bool a_female, RE::BSFixedString a_node, int a_key, int a_index, int a_value, bool a_persist) {
           AddNodeOverride<int>("Int", a_actor, a_female, a_node, a_key, a_index, a_value, a_persist);
        }

        inline void AddNodeOverrideBool(RE::Actor* a_actor, bool a_female, RE::BSFixedString a_node, int a_key, int a_index, bool a_value, bool a_persist) {
           AddNodeOverride<bool>("Bool", a_actor, a_female, a_node, a_key, a_index, a_value, a_persist);
        }
 
        inline void AddNodeOverrideString(RE::Actor* a_actor, bool a_female, RE::BSFixedString a_node, int a_key, int a_index, std::string a_value, bool a_persist) {
           AddNodeOverride<std::string>("String", a_actor, a_female, a_node, a_key, a_index, a_value, a_persist);
        }

        inline void ApplyNodeOverrides(RE::Actor* a_actor) {
            RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
            auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
            auto args = RE::MakeFunctionArguments(std::move(a_actor));
            vm->DispatchStaticCall("NiOverride",  "ApplyNodeOverrides", args, callback);
        }
    }
}