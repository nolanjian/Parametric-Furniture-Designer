#pragma once
#include "BaseObject.h"

class Hard : public BaseObject
{
public:
	Hard();
	virtual ~Hard();
	virtual bool InitFromParams() override;
};