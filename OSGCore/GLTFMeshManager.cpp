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
			osg::ref_ptr<osg::Array> pArr = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArrayByAccessorIndex(attribute.second);
			if (pArr)
			{
				ptrRet->setVertexArray(pArr);
				osg::DrawArrays* pDrawArrays = dynamic_cast<osg::DrawArrays*>(ptrPrimitiveSet.get());
				if (pDrawArrays)
				{
					pDrawArrays->setFirst(0);
					pDrawArrays->setCount(pArr->getNumElements());
				}
			}
		}
		else if (attribute.first == "NORMAL")
		{
			osg::ref_ptr<osg::Array> pArr = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArrayByAccessorIndex(attribute.second);
			if (pArr)
			{
				ptrRet->setNormalArray(pArr, osg::Array::Binding::BIND_PER_VERTEX);
			}
		}
		else if (attribute.first == "TANGENT")
		{
			osg::ref_ptr<osg::Array> pArr = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArrayByAccessorIndex(attribute.second);
			if (pArr)
			{
				// TODO
			}
		}
		else if (attribute.first == "TEXCOORD_0")
		{
			osg::ref_ptr<osg::Array> pArr = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArrayByAccessorIndex(attribute.second);
			if (pArr)
			{
				ptrRet->setTexCoordArray(0, pArr, osg::Array::Binding::BIND_PER_VERTEX);
			}
		}
		else if (attribute.first == "TEXCOORD_1")
		{
			osg::ref_ptr<osg::Array> pArr = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArrayByAccessorIndex(attribute.second);
			if (pArr)
			{
				ptrRet->setTexCoordArray(1, pArr, osg::Array::Binding::BIND_PER_VERTEX);
			}
		}
		else if (attribute.first == "JOINTS_0")
		{
			osg::ref_ptr<osg::Array> pArr = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArrayByAccessorIndex(attribute.second);
			if (pArr)
			{
				// TODO
			}
		}
		else if (attribute.first == "WEIGHTS_0")
		{
			osg::ref_ptr<osg::Array> pArr = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArrayByAccessorIndex(attribute.second);
			if (pArr)
			{
				// TODO
			}
		}
		else if (attribute.first == "COLOR_0")
		{
			osg::ref_ptr<osg::Array> pArr = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArrayByAccessorIndex(attribute.second);
			if (pArr)
			{
				ptrRet->setColorArray(pArr, osg::Array::Binding::BIND_PER_VERTEX);
			}
		}
		else
		{
			assert(false);
		}
	}

	if (m_gltfObject->materials.size() > primitive.material && primitive.material >= 0)
	{
		LoadMaterial(m_gltfObject->materials[primitive.material], ptrRet);
	}

	int useVertexColor = 0;
	if (ptrRet->getColorArray())
	{
		auto numColor = ptrRet->getColorArray()->getNumElements();
		if (numColor > 0)
		{
			useVertexColor = 1;
		}
	}
	ptrRet->getOrCreateStateSet()->addUniform(new osg::Uniform("useVertexColor", useVertexColor));

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

	osg::ref_ptr<osg::Array> pArr = SceneMgr::GetInstance().GetArrayHelper().GetInstance().GetArrayByAccessorIndex(primitive.indices);
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

bool GLTFMeshManager::LoadMaterial(const fx::gltf::Material& material, osg::ref_ptr<osg::Geometry> pGeometry)
{
	bool bRet1 = LoadPBRTexture(material.pbrMetallicRoughness, pGeometry);
	bool bRet2 = LoadNormalTexture(material.normalTexture, pGeometry);
	bool bRet3 = LoadOcclusionTexture(material.occlusionTexture, pGeometry);
	bool bRet4 = LoadImageTexture(material.emissiveTexture, pGeometry);

	// TODO
	//float alphaCutoff{ defaults::MaterialAlphaCutoff };
	//AlphaMode alphaMode{ AlphaMode::Opaque };

	//bool doubleSided{ defaults::MaterialDoubleSided };

	//Texture emissiveTexture;
	//std::array<float, 3> emissiveFactor = { defaults::NullVec3 };
	return bRet1 || bRet2 || bRet3 || bRet4;
}

bool GLTFMeshManager::LoadColorTexture(const std::array<float, 4>& baseColorFactor, osg::ref_ptr<osg::Geometry> pGeometry)
{
	osg::Vec4 objectColor(baseColorFactor[0], baseColorFactor[1], baseColorFactor[2], baseColorFactor[3]);

	bool buseBaseColorFactor = fx::gltf::defaults::IdentityVec4 != baseColorFactor;
	if (buseBaseColorFactor)
	{
		osg::Vec4Array* pArr = new osg::Vec4Array;
		pArr->push_back(objectColor);
		pGeometry->setColorArray(pArr, osg::Array::Binding::BIND_OVERALL);
	}

	return buseBaseColorFactor;
}

bool GLTFMeshManager::LoadImageTexture(const fx::gltf::Material::Texture& texture, osg::ref_ptr<osg::Geometry> pGeometry)
{
	if (texture.empty())
	{
		return false;
	}
	osg::ref_ptr<osg::Texture> pTexture = SceneMgr::GetInstance().GetTextureManager().GetInstance().GetTexture(texture.index);
	if (pTexture)
	{
		pGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(texture.texCoord, pTexture);
		osg::Uniform* baseTextureSampler = new osg::Uniform("baseTexture", texture.texCoord);
		pGeometry->getOrCreateStateSet()->addUniform(baseTextureSampler);
		return true;
	}
	return false;
}

bool GLTFMeshManager::LoadPBRTexture(const fx::gltf::Material::PBRMetallicRoughness& pbrMaterial, osg::ref_ptr<osg::Geometry> pGeometry)
{
	// TODO work better with COLOR_O

	if (pbrMaterial.empty())
	{
		return false;
	}

	bool bRet1 = LoadColorTexture(pbrMaterial.baseColorFactor, pGeometry);
	bool bRet2 = LoadImageTexture(pbrMaterial.baseColorTexture, pGeometry);
	bool bRet3 = LoadImageTexture(pbrMaterial.metallicRoughnessTexture, pGeometry);

	// TODO
	//float roughnessFactor{ defaults::IdentityScalar };
	//float metallicFactor{ defaults::IdentityScalar };

	return bRet1 || bRet2 || bRet3;
}

bool GLTFMeshManager::LoadNormalTexture(const fx::gltf::Material::NormalTexture& normalTexture, osg::ref_ptr<osg::Geometry> pGeometry)
{
	auto fn = [&]()
	{
		return false;
	};

	bool bRet = LoadImageTexture(normalTexture, pGeometry) && fn();

	// TODO
	// float scale{ defaults::IdentityScalar };

	return bRet;
}

bool GLTFMeshManager::LoadOcclusionTexture(const fx::gltf::Material::OcclusionTexture& occlusionTexture, osg::ref_ptr<osg::Geometry> pGeometry)
{
	auto fn = [&]()
	{
		return false;
	};

	bool bRet = LoadImageTexture(occlusionTexture, pGeometry) && fn();

	// TODO
	// float strength{ defaults::IdentityScalar };

	return bRet;
}
