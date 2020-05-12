
#include "IGLTFResourceManager.h"

namespace PFD_GLTF_Helper {


	void IGLTFResourceManager::SetGLTF(std::shared_ptr<fx::gltf::Document> gltfObject)
	{
		m_gltfObject = gltfObject;
		m_strGLTFPath = gltfObject->extensionsAndExtras["LocalPath"].get<std::string>();
	}

}