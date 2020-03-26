#include "pch.h"
#include "SceneMgr.h"

SceneMgr& SceneMgr::GetInstance()
{
	static SceneMgr	s_SceneMgr;
	return s_SceneMgr;
}

void SceneMgr::SetGLTF(std::shared_ptr<fx::gltf::Document> gltfObject)
{
	m_gltfObject = gltfObject;
	m_ArrayHelper = std::make_shared<ArrayHelper>(m_gltfObject);
	m_ArrayHelper->Init();
}
