#pragma once

#include "ParametricComponent.h"
#include "OSGCoreExport.h"

class FrameGroup : public ParametricComponent
{
public:
	FrameGroup();
	virtual ~FrameGroup();
	virtual bool InitFromParams();
};