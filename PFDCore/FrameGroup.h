#pragma once

#include "ParametricComponent.h"
#include "OSGCoreExport.h"

namespace PFDCore
{
	class FrameGroup : public ParametricComponent
	{
	public:
		FrameGroup();
		virtual ~FrameGroup();
		virtual bool InitFromParams();
	};
}