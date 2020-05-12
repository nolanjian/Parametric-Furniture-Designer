
#include "GLTFMaterialsManager.h"

namespace PFD_GLTF_Helper
{

	GLTFMaterialsManager& GLTFMaterialsManager::GetInstance()
	{
		static GLTFMaterialsManager s_GLTFMaterialsManager;
		return s_GLTFMaterialsManager;
	}

	void GLTFMaterialsManager::Load()
	{

	}

}