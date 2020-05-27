#include "ProgramConfig.h"

namespace PFD
{
	namespace Config
	{

		IProgramConfig* IProgramConfig::GetInstance()
		{
			static ProgramConfig s_ProgramConfig;
			return &s_ProgramConfig;
		}
	}
};