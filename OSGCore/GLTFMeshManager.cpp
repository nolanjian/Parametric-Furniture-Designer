#include "pch.h"
#include "GLTFMeshManager.h"

GLTFMeshManager& GLTFMeshManager::GetInstance()
{
	static GLTFMeshManager s_GLTFMeshManager;
	return s_GLTFMeshManager;
}

void GLTFMeshManager::Load()
{
	if (m_gltfObject == nullptr)
	{
		return;
	}


}
