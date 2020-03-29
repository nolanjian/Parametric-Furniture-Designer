#pragma once
#include "OSGIncluding.h"
#include "STLIncluding.h"
#include "stb_image.h"
#include "IGLTFResourceManager.h"

class GLTFMaterialsManager : public IGLTFResourceManager
{
public:
	static GLTFMaterialsManager& GetInstance();
	virtual void Load() override;
protected:
private:
	//GLTFMaterialsManager() {}
};