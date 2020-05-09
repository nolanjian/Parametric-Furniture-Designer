#pragma once

#include "SpaceManagerExport.h"

namespace PFDSpaceMgr
{
	class SPACEMANAGER_API ISpaceManager
	{
	public:
		static ISpaceManager* GetInstance();
		virtual ~ISpaceManager() {}
	};
}
