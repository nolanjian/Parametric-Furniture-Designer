#pragma once
#include "BaseObject.h"

class Scene : public BaseObject
{
public:
	Scene();
	virtual ~Scene();
	virtual bool InitFromParams() override;
};