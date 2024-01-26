#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include "Papyrus.h"
#include "JCApi.h"

using namespace OM;

void InitLogging()
{
	auto path = logs::log_directory();
	if (!path)
		return;

	const auto plugin = SKSE::PluginDeclaration::GetSingleton();
	*path /= fmt::format("{}.log", plugin->GetName());

	std::vector<spdlog::sink_ptr> sinks{ 
		std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true), 
		std::make_shared<spdlog::sinks::msvc_sink_mt>() 
	};

	auto logger = std::make_shared<spdlog::logger>("global", sinks.begin(), sinks.end());
	logger->set_level(spdlog::level::info);
	logger->flush_on(spdlog::level::info);

	spdlog::set_default_logger(std::move(logger));
	spdlog::set_pattern("[%^%L%$] %v");
}

 void InitializeMessaging() {			
	logs::info("initializing messaging");

	if (!SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* message) {
		if (message->type == SKSE::MessagingInterface::kPostLoad) {
			logs::info("on post load");
			SKSE::GetMessagingInterface()->RegisterListener(JC_PLUGIN_NAME, [](SKSE::MessagingInterface::Message* a_msg) {
				if (a_msg && a_msg->type == jc::message_root_interface) {
					const jc::root_interface* root = jc::root_interface::from_void(a_msg->data);
					JC::Api::Init(root);
				}
			});
		}
	})) {
		stl::report_and_fail("Unable to register message listener.");
	}
 }

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	InitLogging();

	const auto plugin = SKSE::PluginDeclaration::GetSingleton();
	logs::info("{} v{} is loading...", plugin->GetName(), plugin->GetVersion());

	SKSE::Init(a_skse);

	const auto papyrus = SKSE::GetPapyrusInterface();
	papyrus->Register(Papyrus::RegisterFunctions);
	InitializeMessaging();

	logs::info("{} loaded.", plugin->GetName());

	return true;
}
