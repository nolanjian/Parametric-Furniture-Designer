#pragma once
#include "Model.h"
class Handler : public Model
{
public:
	Handler();
	virtual ~Handler();
	virtual bool InitFromParams() override;
};