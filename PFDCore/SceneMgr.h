#pragma once
#include "OSGIncluding.h"
#include "STLIncluding.h"
#include <fx/gltf.h>
#include "stb_image.h"
#include "GLTFArrayHelper.h"
#include "GLTFTextureManager.h"
#include "GLTFMaterialsManager.h"
#include "GLTFMeshManager.h"

namespace PFDCore {

	class SceneMgr
	{
	public:
		static SceneMgr& GetInstance();
		void SetGLTF(std::shared_ptr<fx::gltf::Document> gltfObject);
		const GLTFArrayHelper& GetArrayHelper() { return GLTFArrayHelper::GetInstance(); }
		const GLTFTextureManager& GetTextureManager() { return GLTFTextureManager::GetInstance(); }
		const GLTFMaterialsManager& GetMaterialsManager() { return GLTFMaterialsManager::GetInstance(); }
		const GLTFMeshManager& GetMeshManager() { return GLTFMeshManager::GetInstance(); }
		void Release();
	protected:
	private:
		std::shared_ptr<fx::gltf::Document> m_gltfObject;
	};
}