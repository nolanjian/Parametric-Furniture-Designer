#pragma once
#include <fx/gltf.h>
#include "OSGIncluding.h"
#include "STLIncluding.h"
#include "ArrayHelper.h"

class SceneMgr
{
public:
	static SceneMgr& GetInstance();
	void SetGLTF(std::shared_ptr<fx::gltf::Document> gltfObject);
	std::shared_ptr<ArrayHelper> GetArrayHelper() { return m_ArrayHelper; }

protected:
private:
	std::shared_ptr<fx::gltf::Document> m_gltfObject;
	std::shared_ptr<ArrayHelper>	m_ArrayHelper;
};