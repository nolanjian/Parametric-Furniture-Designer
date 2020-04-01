#include "pch.h"
#include "GLTFMeshManager.h"
#include "SceneMgr.h"
#include "ShadingPreDefine.h"

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

	bool useTangent = false;

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
				ptrRet->setVertexAttribArray(TANGENT_INDEX, pArr, osg::Array::Binding::BIND_PER_VERTEX);
				useTangent = true;
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

	ptrRet->getOrCreateStateSet()->addUniform(new osg::Uniform("useTangent", useTangent));

	if (m_gltfObject->materials.size() > primitive.material && primitive.material >= 0)
	{
		LoadMaterial(m_gltfObject->materials[primitive.material], ptrRet);
	}

	bool useVertexColor = false;
	if (ptrRet->getColorArray())
	{
		auto numColor = ptrRet->getColorArray()->getNumElements();
		if (numColor > 0)
		{
			useVertexColor = true;
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

	switch (material.alphaMode)
	{
	case  fx::gltf::Material::AlphaMode::Opaque:
		pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform("enableAlphaCutoff", false));
		break;
	case  fx::gltf::Material::AlphaMode::Mask:
		pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform("enableAlphaCutoff", true));
		pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform("alphaCutoff", material.alphaCutoff));
		pGeometry->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
		break;
	case  fx::gltf::Material::AlphaMode::Blend:
		pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform("enableAlphaCutoff", false));
		pGeometry->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
		break;
	default:
		break;
	}

	pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform("MaterialDoubleSided", material.doubleSided));

	bool useEmissiveTexture = false;
	if (!material.emissiveTexture.empty() && material.emissiveFactor != fx::gltf::defaults::NullVec3)
	{
		osg::ref_ptr<osg::Texture> pTexture = SceneMgr::GetInstance().GetTextureManager().GetInstance().GetTexture(material.emissiveTexture.index);
		if (pTexture)
		{
			pGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(EMISSIVE_TEXTURE_INDEX, pTexture);
			pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(EMISSIVE_TEXTURE, material.emissiveTexture.texCoord));
			pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(EMISSIVE_FACTOR,
				osg::Vec3f(material.emissiveFactor[0], material.emissiveFactor[1], material.emissiveFactor[2])));
			useEmissiveTexture = true;
		}
	}
	pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_EMISSIVE_TEXTURE, useEmissiveTexture));

	return bRet1 || bRet2 || bRet3;
}

bool GLTFMeshManager::LoadPBRTexture(const fx::gltf::Material::PBRMetallicRoughness& pbrMaterial, osg::ref_ptr<osg::Geometry> pGeometry)
{
	if (pbrMaterial.empty())
	{
		return false;
	}

	bool useBaseColorFactor = false;
	if (pbrMaterial.baseColorFactor != fx::gltf::defaults::IdentityVec4)
	{
		osg::Vec4 objectColor(pbrMaterial.baseColorFactor[0], pbrMaterial.baseColorFactor[1],
			pbrMaterial.baseColorFactor[2], pbrMaterial.baseColorFactor[3]);
		pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(BASE_COLOR_FACTOR, objectColor));
		useBaseColorFactor = true;
		
	}
	pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_BASE_COLOR_FACTOR, useBaseColorFactor));

	bool useBaseColorTexture = false;
	if (!pbrMaterial.baseColorTexture.empty())
	{
		osg::ref_ptr<osg::Texture> pTexture = SceneMgr::GetInstance().GetTextureManager().GetInstance().GetTexture(pbrMaterial.baseColorTexture.index);
		if (pTexture)
		{
			pGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(BASE_COLOR_TEXTURE_INDEX, pTexture);
			pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(BASE_COLOR_TEXTURE, pbrMaterial.baseColorTexture.texCoord));
			useBaseColorTexture = true;
		}
	}
	pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_BASE_COLOR_TEXTURE, useBaseColorTexture));

	bool useMetallicRoughnessTexture = false;
	if (!pbrMaterial.metallicRoughnessTexture.empty())
	{
		osg::ref_ptr<osg::Texture> pTexture = SceneMgr::GetInstance().GetTextureManager().GetInstance().GetTexture(pbrMaterial.metallicRoughnessTexture.index);
		if (pTexture)
		{
			pGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(METALLIC_ROUGHNESS_TEXTURE_INDEX, pTexture);
			pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(METALLIC_ROUGHNESS_TEXTURE, pbrMaterial.metallicRoughnessTexture.texCoord));
			useMetallicRoughnessTexture = true;
		}
	}
	pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_METALLIC_ROUGHNESS_TEXTURE, useMetallicRoughnessTexture));

	pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(ROUGHNESS_FACTOR, pbrMaterial.roughnessFactor));
	pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(METALLIC_FACTOR, pbrMaterial.metallicFactor));

	return useBaseColorFactor || useBaseColorTexture || useMetallicRoughnessTexture;
}

bool GLTFMeshManager::LoadNormalTexture(const fx::gltf::Material::NormalTexture& normalTexture, osg::ref_ptr<osg::Geometry> pGeometry)
{
	if (normalTexture.empty())
	{
		pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_NORMAL_TEXTURE, false));
		return false;
	}
	osg::ref_ptr<osg::Texture> pTexture = SceneMgr::GetInstance().GetTextureManager().GetInstance().GetTexture(normalTexture.index);
	if (pTexture)
	{
		pGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(NORMAL_TEXTURE_INDEX, pTexture);
		pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(NORMAL_TEXTURE, normalTexture.texCoord));
		pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(NORMAL_TEXTURE_SCALE, normalTexture.scale));
		pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_NORMAL_TEXTURE, true));
		return true;
	}
	pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_NORMAL_TEXTURE, false));

	return false;
}

bool GLTFMeshManager::LoadOcclusionTexture(const fx::gltf::Material::OcclusionTexture& occlusionTexture, osg::ref_ptr<osg::Geometry> pGeometry)
{
	if (occlusionTexture.empty())
	{
		pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_OCCLUSION_TEXTURE, false));
		return false;
	}
	osg::ref_ptr<osg::Texture> pTexture = SceneMgr::GetInstance().GetTextureManager().GetInstance().GetTexture(occlusionTexture.index);
	if (pTexture)
	{
		pGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(OCCLUSION_TEXTURE_INDEX, pTexture);
		pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(OCCLUSION_TEXTURE, occlusionTexture.texCoord));
		pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(OCCLUSION_TEXTURE_STRENGTH, occlusionTexture.strength));
		pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_OCCLUSION_TEXTURE, true));
		return true;
	}
	pGeometry->getOrCreateStateSet()->addUniform(new osg::Uniform(USE_OCCLUSION_TEXTURE, false));

	return false;
}