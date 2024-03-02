#pragma once

namespace OM
{
	constexpr std::string_view OM_CONFIG_PATH = "Data/SKSE/Plugins/OverlayManager.ini";

	class Config
	{
	public:
		static inline void Init() {
			static bool initialised = false;

			if (!initialised)
				_configStatus = _config.LoadFile(OM_CONFIG_PATH.data());

			initialised = true;

			auto delay = GetApplicationDelay();
			logger::info("config delay {}", delay.count());
		}

		static inline std::chrono::milliseconds GetApplicationDelay()
		{
			int delay;
			if (_configStatus >= 0) {
				clib_util::ini::get_value(_config, delay, "Application", "iApplicationDelay", "");
				logger::info("read delay from ini {}", delay);
			} else {
				logger::info("failed to read delay from ini");
				delay = 200;
			}

			return std::chrono::milliseconds(delay);
		}
	private:
		static inline CSimpleIniA _config;
		static inline SI_Error _configStatus;
	};
}