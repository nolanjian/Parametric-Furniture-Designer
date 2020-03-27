#include "pch.h"
#include "GLTFTextureManager.h"
#include "../easyloggingpp/easylogging++.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLTFTextureManager& GLTFTextureManager::GetInstance()
{
	static GLTFTextureManager s_GLTFTextureManager;
	return s_GLTFTextureManager;
}

void GLTFTextureManager::Load()
{
	mapTexture.clear();
	if (m_gltfObject == nullptr)
	{
		return;
	}
	for (size_t ii = 0; ii < m_gltfObject->textures.size(); ++ii)
	{
		const fx::gltf::Texture& texture = m_gltfObject->textures[ii];
		osg::ref_ptr<osg::Texture2D> pTexture = new osg::Texture2D();
		pTexture->setName(texture.name);

		if (texture.sampler >= 0 && texture.sampler < m_gltfObject->samplers.size())
		{
			LoadSampler(pTexture, m_gltfObject->samplers[texture.sampler]);
		}
		if (texture.source >= 0 && texture.source < m_gltfObject->images.size())
		{
			LoadImage(pTexture, m_gltfObject->images[texture.source]);
		}
		mapTexture[ii] = pTexture;
	}
}

osg::ref_ptr<osg::Texture> GLTFTextureManager::GetTexture(size_t index)
{
	return mapTexture[index];
}

void GLTFTextureManager::LoadSampler(osg::ref_ptr<osg::Texture2D> pTexture, const fx::gltf::Sampler& sampler)
{
	if (pTexture == nullptr || sampler.empty())
	{
		return;
	}

	if (m_mapWrapMode.find(sampler.wrapS) != m_mapWrapMode.end())
	{
		pTexture->setWrap(osg::Texture::WRAP_S, m_mapWrapMode.at(sampler.wrapS));
	}
	if (m_mapWrapMode.find(sampler.wrapT) != m_mapWrapMode.end())
	{
		pTexture->setWrap(osg::Texture::WRAP_T, m_mapWrapMode.at(sampler.wrapT));
	}
	if (m_mapMagFilter.find(sampler.magFilter) != m_mapMagFilter.end())
	{
		pTexture->setFilter(osg::Texture::MAG_FILTER, m_mapMagFilter.at(sampler.magFilter));
	}
	if (m_mapMinFilter.find(sampler.minFilter) != m_mapMinFilter.end())
	{
		pTexture->setFilter(osg::Texture::MIN_FILTER, m_mapMinFilter.at(sampler.minFilter));
	}
}

void GLTFTextureManager::LoadImage(osg::ref_ptr<osg::Texture2D> pTexture, const fx::gltf::Image& image)
{
	if (pTexture == nullptr)
	{
		return;
	}

	try
	{
		int width, height, nrChannels;
		unsigned char* imgObj = nullptr;
		if (image.IsEmbeddedResource())
		{
			std::vector<uint8_t>	imgData;
			image.MaterializeData(imgData);
			if (imgData.empty())
			{
				return;
			}
			imgObj = stbi_load_from_memory(&imgData[0], imgData.size(), &width, &height, &nrChannels, STBI_rgb_alpha);
		}
		else
		{
			std::string filePath = fx::gltf::detail::CreateBufferUriPath(fx::gltf::detail::GetDocumentRootPath(m_strGLTFPath), image.uri);
			imgObj = stbi_load(filePath.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
		}

		if (!imgObj)
		{
			return;
		}

		osg::ref_ptr<osg::Image>	osgIMG = new osg::Image();
		osgIMG->allocateImage(width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE);
		osgIMG->setImage(width, height, 1, GL_RGBA, GL_RGBA, GL_BYTE, imgObj, osg::Image::USE_NEW_DELETE);
		if (!osgIMG->valid())
		{
			return;
		}

		pTexture->setImage(osgIMG);
	}
	catch (const std::exception & e)
	{
		LOG(ERROR) << e.what();
		return;
	}
}
