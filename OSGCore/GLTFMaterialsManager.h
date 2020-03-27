#pragma once
#include "OSGIncluding.h"
#include "STLIncluding.h"
#include "fx/gltf.h"
#include "IGLTFResourceManager.h"

class GLTFMaterialsManager : IGLTFResourceManager
{
public:
	static GLTFMaterialsManager& GetInstance();
	virtual void Load() override;
protected:
private:
	GLTFMaterialsManager();
};