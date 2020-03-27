#pragma once

#include "OSGIncluding.h"
#include "STLIncluding.h"
#include "fx/gltf.h"
#include "IGLTFResourceManager.h"

class GLTFMeshManager : public IGLTFResourceManager
{
public:
	static GLTFMeshManager& GetInstance();
	virtual void Load() override;
protected:
private:
	//GLTFMeshManager() {}
	std::string	m_strGLTFPath;
	std::shared_ptr<fx::gltf::Document> m_gltfObject;
};