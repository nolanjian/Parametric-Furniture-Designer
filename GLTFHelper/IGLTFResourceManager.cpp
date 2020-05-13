
#include "GLTFResourceManager.h"

namespace PFD_GLTF_Helper
{
	std::shared_ptr<IGLTFResourceManager> IGLTFResourceManager::Load(std::shared_ptr<fx::gltf::Document> gltfObject)
	{
		return std::make_shared<GLTFResourceManager>(gltfObject);
	}
}