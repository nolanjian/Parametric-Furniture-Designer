#pragma once
#include "BaseObject.h"
class Handler : public BaseObject
{
public:
	Handler();
	virtual ~Handler();
	virtual bool InitFromParams() override;
};