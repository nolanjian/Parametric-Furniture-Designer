#pragma once
#include <fx/gltf.h>
#include "OSGIncluding.h"
#include "STLIncluding.h"
#include "GLTFArrayHelper.h"
#include "GLTFTextureManager.h"
#include "GLTFMaterialsManager.h"
#include "GLTFMeshManager.h"

class SceneMgr
{
public:
	static SceneMgr& GetInstance();
	void SetGLTF(std::shared_ptr<fx::gltf::Document> gltfObject);
	const GLTFArrayHelper& GetArrayHelper() { return GLTFArrayHelper::GetInstance(); }
	const GLTFTextureManager& GetTextureManager() { return GLTFTextureManager::GetInstance(); }
	const GLTFMaterialsManager& GetMaterialsManager() { return GLTFMaterialsManager::GetInstance(); }
	const GLTFMeshManager& GetMeshManager() { return GLTFMeshManager::GetInstance(); }
protected:
private:
	std::shared_ptr<fx::gltf::Document> m_gltfObject;
};