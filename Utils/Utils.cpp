// Utils.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "Utils.h"
#include "../easyloggingpp/easylogging++.h"


// This is an example of an exported variable
UTILS_API int nUtils=0;

// This is an example of an exported function.
UTILS_API bool InitLogger(void)
{
	el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
	el::Configurations conf("config\\OSGCoreLog.config");
	el::Loggers::reconfigureAllLoggers(conf);
    return true;
}

// This is the constructor of a class that has been exported.
CUtils::CUtils()
{
    return;
}
