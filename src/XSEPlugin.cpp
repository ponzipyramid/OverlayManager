#include "Papyrus.h"
#include "JCApi.h"
#include "NiOverride.h"
#include "Registry.h"
#include "Hooks.h"
#include "Serialize.h"

#define DLLEXPORT __declspec(dllexport)

using namespace OM;

void InitializeLog([[maybe_unused]] spdlog::level::level_enum a_level = spdlog::level::info)
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		util::report_and_fail("Failed to find standard logging directory"sv);
	}

	*path /= std::format("{}.log"sv, Plugin::NAME);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	const auto level = a_level;

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
	log->set_level(level);
	log->flush_on(spdlog::level::info);

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] [%s:%#] %v");
}

void InitializeMessaging() 
{			
	logger::info("initializing messaging");

	if (!SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* message) {
		if (message->type == SKSE::MessagingInterface::kPostLoad) {
			SKSE::GetMessagingInterface()->RegisterListener(JC_PLUGIN_NAME, [](SKSE::MessagingInterface::Message* a_msg) {
				if (a_msg && a_msg->type == jc::message_root_interface) {
					if (const jc::root_interface* root = jc::root_interface::from_void(a_msg->data))
						JC::Api::Init(root);
				}
			});
			
			NiOverride::Init();

		} else if (message->type == SKSE::MessagingInterface::kDataLoaded) {
			Registry::Init();
		}
	})) {
		stl::report_and_fail("Unable to register message listener.");
	}
 }

void InitializeSerialization() 
{
	const auto serialization = SKSE::GetSerializationInterface();
	serialization->SetUniqueID(Serialize::RecordName);
	serialization->SetSaveCallback(Serialize::Save);
	serialization->SetLoadCallback(Serialize::Load);
	serialization->SetRevertCallback(Serialize::Revert);
}

void InitializePapyrus()
{
	const auto papyrus = SKSE::GetPapyrusInterface();
	papyrus->Register(Papyrus::RegisterFunctions);
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	InitializeLog();
	
	logger::info("Loaded plugin {} {}", Plugin::NAME, Plugin::VERSION.string());
	SKSE::Init(a_skse);

	InitializeMessaging();
	InitializeSerialization();
	InitializePapyrus();

	Registry::Read();
	Hooks::Install();

	return true;
}

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() noexcept {
	SKSE::PluginVersionData v;
	v.PluginName(Plugin::NAME.data());
	v.PluginVersion(Plugin::VERSION);
	v.UsesAddressLibrary(true);
	v.HasNoStructUse();
	return v;
}();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface*, SKSE::PluginInfo* pluginInfo)
{
	pluginInfo->name = SKSEPlugin_Version.pluginName;
	pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
	pluginInfo->version = SKSEPlugin_Version.pluginVersion;
	return true;
}