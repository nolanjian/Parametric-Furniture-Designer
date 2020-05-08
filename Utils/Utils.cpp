#include "Utils.h"
#include "../easyloggingpp/easylogging++.h"

namespace PFDUtils
{
	UTILS_API bool InitLogger(void)
	{
		static bool bInit = false;
		if (!bInit)
		{
			el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
			el::Configurations conf("file\\PFDCore.config");
			el::Loggers::reconfigureAllLoggers(conf);
			bInit = true;
		}

		return true;
	}

}