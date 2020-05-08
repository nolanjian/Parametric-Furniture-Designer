#include "Utils.h"

namespace PFDUtils
{
	static bool bInitLogger = []() {
		return InitLogger();
	}();

	bool InitLogger()
	{
		if (bInitLogger)
			return true;
		spdlog::basic_logger_mt<spdlog::async_factory>(PFD_LOGGER, "logs/program.log");
		bInitLogger = true;
		return bInitLogger;
	}
}