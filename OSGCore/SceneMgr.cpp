#include "pch.h"
#include "SceneMgr.h"
#include "GLTFArrayHelper.h"

SceneMgr& SceneMgr::GetInstance()
{
	static SceneMgr	s_SceneMgr;
	return s_SceneMgr;
}

void SceneMgr::SetGLTF(std::shared_ptr<fx::gltf::Document> gltfObject)
{
	m_gltfObject = gltfObject;
	GLTFArrayHelper::GetInstance().SetGLTF(gltfObject);
	GLTFArrayHelper::GetInstance().Load();
}
