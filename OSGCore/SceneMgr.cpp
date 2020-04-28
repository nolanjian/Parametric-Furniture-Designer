#include "pch.h"
#include "SceneMgr.h"

namespace PFDCore
{

	SceneMgr& SceneMgr::GetInstance()
	{
		static SceneMgr	s_SceneMgr;
		return s_SceneMgr;
	}

	void SceneMgr::SetGLTF(std::shared_ptr<fx::gltf::Document> gltfObject)
	{
		m_gltfObject = gltfObject;
		// Run in fix order, must be
		GLTFArrayHelper::GetInstance().SetGLTF(gltfObject);
		GLTFArrayHelper::GetInstance().Load();
		GLTFTextureManager::GetInstance().SetGLTF(gltfObject);
		GLTFTextureManager::GetInstance().Load();
		GLTFMaterialsManager::GetInstance().SetGLTF(gltfObject);
		GLTFMaterialsManager::GetInstance().Load();
		GLTFMeshManager::GetInstance().SetGLTF(gltfObject);
		GLTFMeshManager::GetInstance().Load();
	}

	void SceneMgr::Release()
	{
		SetGLTF(nullptr);
	}

}