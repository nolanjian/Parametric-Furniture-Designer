#include "pch.h"
#include "IGLTFResourceManager.h"

namespace PFDCore {


	void IGLTFResourceManager::SetGLTF(std::shared_ptr<fx::gltf::Document> gltfObject)
	{
		m_gltfObject = gltfObject;
		m_strGLTFPath = gltfObject->extensionsAndExtras["LocalPath"].get<std::string>();
	}

}