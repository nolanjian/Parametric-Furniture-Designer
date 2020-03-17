#pragma once

#include "BaseObject.h"

class FrameGroup : public BaseObject
{
public:
	FrameGroup();
	virtual ~FrameGroup();
	virtual bool InitFromParams();
};