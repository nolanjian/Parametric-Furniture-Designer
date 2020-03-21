#pragma once

#include "ParametricComponent.h"

class FrameGroup : public ParametricComponent
{
public:
	FrameGroup();
	virtual ~FrameGroup();
	virtual bool InitFromParams();
};