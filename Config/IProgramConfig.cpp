#include "ProgramConfig.h"

namespace PFDConfig
{
	IProgramConfig* PFDConfig::IProgramConfig::GetInstance()
	{
		static ProgramConfig s_ProgramConfig;
		return &s_ProgramConfig;
	}
};