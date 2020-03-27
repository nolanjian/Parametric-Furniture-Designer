#include "pch.h"
#include "GLTFMeshManager.h"
#include "SceneMgr.h"

GLTFMeshManager& GLTFMeshManager::GetInstance()
{
	static GLTFMeshManager s_GLTFMeshManager;
	return s_GLTFMeshManager;
}

void GLTFMeshManager::Load()
{
	mapGeode.clear();
	if (m_gltfObject == nullptr)
	{
		return;
	}

	for (size_t ii = 0; ii < m_gltfObject->meshes.size(); ++ii)
	{
		const fx::gltf::Mesh& mesh = m_gltfObject->meshes[ii];
		osg::ref_ptr<osg::Geode>	geo = new osg::Geode();
		geo->setName(mesh.name);
		geo->getOrCreateStateSet();
		mapGeode[ii] = geo;
		for (const fx::gltf::Primitive& primitive : mesh.primitives)
		{
			osg::ref_ptr<osg::Drawable> ptrDrawable = LoadDrawable(primitive);
			if (ptrDrawable)
			{
				geo->addDrawable(ptrDrawable);
			}
		}
	}
}

osg::ref_ptr<osg::Drawable> GLTFMeshManager::LoadDrawable(const fx::gltf::Primitive& primitive)
{
	osg::ref_ptr<osg::PrimitiveSet> ptrPrimitiveSet = LoadPrimitiveSet(primitive);
	if (ptrPrimitiveSet == nullptr)
	{
		return nullptr;
	}

	osg::ref_ptr<osg::Geometry>	ptrRet = new osg::Geometry();
	ptrRet->addPrimitiveSet(ptrPrimitiveSet);

	for (const std::pair<std::string, uint32_t>& attribute : primitive.attributes)
	{
		if (attribute.first == "POSITION")
		{
			osg::ref_ptr<osg::Array> pArr = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArray(attribute.second);
			if (pArr)
			{
				ptrRet->setVertexArray(pArr);
			}
		}
		else if (attribute.first == "NORMAL")
		{
			osg::ref_ptr<osg::Array> pArr = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArray(attribute.second);
			if (pArr)
			{
				ptrRet->setNormalArray(pArr, osg::Array::Binding::BIND_PER_VERTEX);
			}
		}
		else if (attribute.first == "TANGENT")
		{
			osg::ref_ptr<osg::Array> pArr = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArray(attribute.second);
			if (pArr)
			{
				// TODO
			}
		}
		else if (attribute.first == "TEXCOORD_0")
		{
			osg::ref_ptr<osg::Array> pArr = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArray(attribute.second);
			if (pArr)
			{
				ptrRet->setTexCoordArray(0, pArr, osg::Array::Binding::BIND_PER_VERTEX);
			}
		}
		else if (attribute.first == "TEXCOORD_1")
		{
			osg::ref_ptr<osg::Array> pArr = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArray(attribute.second);
			if (pArr)
			{
				ptrRet->setTexCoordArray(1, pArr, osg::Array::Binding::BIND_PER_VERTEX);
			}
		}
		else if (attribute.first == "JOINTS_0")
		{
			osg::ref_ptr<osg::Array> pArr = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArray(attribute.second);
			if (pArr)
			{
				// TODO
			}
		}
		else if (attribute.first == "WEIGHTS_0")
		{
			osg::ref_ptr<osg::Array> pArr = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArray(attribute.second);
			if (pArr)
			{
				// TODO
			}
		}
		else if (attribute.first == "COLOR_0")
		{
			osg::ref_ptr<osg::Array> pArr = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArray(attribute.second);
			if (pArr)
			{
				// TODO
			}
		}
		else
		{
			assert(false);
		}
	}

	//SceneMgr::GetInstance().GetMaterialsManager().GetInstance()

	//if (gltfObject->materials.size() > primitive.material&& primitive.material >= 0)
	//{
	//	ImportMaterial(gltfObject, gltfObject->materials[primitive.material]);
	//}

	// TODO 
	// std::vector<Attributes> targets{};

	return ptrRet;
}

osg::ref_ptr<osg::PrimitiveSet> GLTFMeshManager::LoadPrimitiveSet(const fx::gltf::Primitive& primitive)
{
	if (primitive.indices >= 0)
	{
		return LoadDrawElements(primitive);
	}
	else
	{
		if (m_mapPrimitives.find(primitive.mode) != m_mapPrimitives.end())
		{
			return new osg::DrawArrays(m_mapPrimitives.at(primitive.mode));
		}
		return new osg::DrawArrays(m_mapPrimitives.at(fx::gltf::Primitive::Mode::Triangles));
	}
}

osg::ref_ptr<osg::DrawElements> GLTFMeshManager::LoadDrawElements(const fx::gltf::Primitive& primitive)
{
	assert(primitive.indices >= 0);
	if (primitive.indices < 0)
	{
		return nullptr;
	}

	osg::ref_ptr<osg::Array> pArr = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArray(primitive.indices);
	if (pArr == nullptr)
	{
		return nullptr;
	}

	if (m_mapPrimitives.find(primitive.mode) == m_mapPrimitives.end())
	{
		return nullptr;
	}
	
	const auto mode = m_mapPrimitives.at(primitive.mode);

	switch (pArr->getType())
	{
	case osg::Array::Type::ByteArrayType:
	case osg::Array::Type::UByteArrayType:
		return new osg::DrawElementsUByte(mode, pArr->getNumElements(), (const GLubyte*)(pArr->getDataPointer()));
	case osg::Array::Type::ShortArrayType:
	case osg::Array::Type::UShortArrayType:
		return new osg::DrawElementsUShort(mode, pArr->getNumElements(), (const GLushort*)(pArr->getDataPointer()));
	case osg::Array::Type::UIntArrayType:
	case osg::Array::Type::FloatArrayType:
		return new osg::DrawElementsUInt(mode, pArr->getNumElements(), (const GLuint*)(pArr->getDataPointer()));
	default:
		return nullptr;
	}
}
