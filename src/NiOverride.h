#pragma once
#include <future>

using namespace std::chrono_literals;

namespace OM {
    namespace NiOverride {
        class VmCallback : public RE::BSScript::IStackCallbackFunctor
        {
        public:
            using OnResult = std::function<void(const RE::BSScript::Variable& result)>;

            static auto New(const OnResult& onResult_)
            {
                RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> res;
                res.reset(new VmCallback(onResult_));
                return res;
            }

            VmCallback(const OnResult& onResult_) : onResult(onResult_) {}

        private:
            void operator()(RE::BSScript::Variable result) override
            {
                if (onResult)
                    onResult(result);
            }

            bool CanSave() const override { return false; }

            void SetObject(const RE::BSTSmartPointer<RE::BSScript::Object>&) override {}

            const OnResult onResult;
        };

        template<typename F>
        inline std::string GetNodeOverrideString(RE::Actor* a_actor, bool a_female, std::string a_node, int a_key, int a_index, F a_callback) {

            logs::info("GetNodeOverrideString - START");
            auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
            auto args = RE::MakeFunctionArguments(std::move(a_actor->As<RE::TESObjectREFR>()), std::move(a_female), std::move(a_node), std::move(a_key), std::move(a_index));
            auto onResult = [](const RE::BSScript::Variable& a_var) {
                logs::info("received callback value = {}", a_var.GetString());
                std::string s = {a_var.GetString().begin(), a_var.GetString().end()};
            };
            RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
            callback.reset(new VmCallback(onResult));

            vm->DispatchStaticCall("NiOverride", "GetNodeOverrideString", args, a_callback);
            logs::info("dispatched call");

            return "";
        }


        template<typename T>
        inline void AddNodeOverride(std::string a_type, RE::Actor* a_actor, bool a_female, RE::BSFixedString a_node, int a_key, int a_index, T a_value, bool a_persist) {
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

        inline void RemoveNodeOverride(RE::Actor* a_actor, bool a_female, std::string a_node, int a_key, int a_index) {
            RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
            auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
            auto args = RE::MakeFunctionArguments(std::move(a_actor->As<RE::TESObjectREFR>()), std::move(a_female), std::move(a_node), std::move(a_key), std::move(a_index));
            vm->DispatchStaticCall("NiOverride", "RemoveNodeOverride", args, callback);
        }
    }
}