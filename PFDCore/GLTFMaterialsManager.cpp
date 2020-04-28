#include "pch.h"
#include "GLTFMaterialsManager.h"

namespace PFDCore {

	GLTFMaterialsManager& GLTFMaterialsManager::GetInstance()
	{
		static GLTFMaterialsManager s_GLTFMaterialsManager;
		return s_GLTFMaterialsManager;
	}

	void GLTFMaterialsManager::Load()
	{

	}

}