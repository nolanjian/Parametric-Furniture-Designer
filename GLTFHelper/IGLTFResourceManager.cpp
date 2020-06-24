#include <GLTFHelper/GLTFResourceManager.h>

namespace PFD
{
	namespace GLTF
	{

		std::shared_ptr<IGLTFResourceManager> IGLTFResourceManager::Load(std::shared_ptr<fx::gltf::Document> gltfObject)
		{
			return std::make_shared<GLTFResourceManager>(gltfObject);
		}
	}
}