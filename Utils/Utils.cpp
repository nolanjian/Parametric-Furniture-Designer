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
		auto max_size = 1048576 * 5;
		auto max_files = 3;
		auto logger = spdlog::rotating_logger_mt(PFD_LOGGER, "logs/program.log", max_size, max_files);
		bInitLogger = true;
		return bInitLogger;
	}
}