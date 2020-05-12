#pragma once
#include "OSGIncluding.h"
#include "STLIncluding.h"
#include "IGLTFResourceManager.h"
#include <fx/gltf.h>
#include "stb_image.h"

namespace PFD_GLTF_Helper
{

	class GLTFTextureManager : public IGLTFResourceManager
	{
	public:
		static GLTFTextureManager& GetInstance();
		virtual void Load() override;
		osg::ref_ptr<osg::Texture> GetTexture(size_t index);

	protected:
		void LoadSampler(osg::ref_ptr<osg::Texture2D> pTexture, const fx::gltf::Sampler& sampler);
		void LoadImage(osg::ref_ptr<osg::Texture2D> pTexture, const fx::gltf::Image& image);
	private:
		//GLTFTextureManager() {}


		std::map<size_t, osg::ref_ptr<osg::Texture>>	mapTexture;

		const std::map<fx::gltf::Sampler::WrappingMode, osg::Texture::WrapMode>	m_mapWrapMode{
			{fx::gltf::Sampler::WrappingMode::ClampToEdge, osg::Texture::WrapMode::CLAMP_TO_EDGE},
			{fx::gltf::Sampler::WrappingMode::MirroredRepeat, osg::Texture::WrapMode::MIRROR},
			{fx::gltf::Sampler::WrappingMode::Repeat, osg::Texture::WrapMode::REPEAT},
		};

		const std::map<fx::gltf::Sampler::MagFilter, osg::Texture::FilterMode>	m_mapMagFilter{
			//{fx::gltf::Sampler::MagFilter::None, osg::Texture::FilterMode::None},
			{fx::gltf::Sampler::MagFilter::Nearest, osg::Texture::FilterMode::NEAREST},
			{fx::gltf::Sampler::MagFilter::Linear, osg::Texture::FilterMode::LINEAR},
		};

		const std::map<fx::gltf::Sampler::MinFilter, osg::Texture::FilterMode>	m_mapMinFilter{
			//{fx::gltf::Sampler::MinFilter::None, osg::Texture::FilterMode::None},
			{fx::gltf::Sampler::MinFilter::Nearest, osg::Texture::FilterMode::NEAREST},
			{fx::gltf::Sampler::MinFilter::Linear, osg::Texture::FilterMode::LINEAR},
			{fx::gltf::Sampler::MinFilter::NearestMipMapNearest, osg::Texture::FilterMode::NEAREST_MIPMAP_NEAREST},
			{fx::gltf::Sampler::MinFilter::LinearMipMapNearest, osg::Texture::FilterMode::LINEAR_MIPMAP_NEAREST},
			{fx::gltf::Sampler::MinFilter::NearestMipMapLinear, osg::Texture::FilterMode::NEAREST_MIPMAP_LINEAR},
			{fx::gltf::Sampler::MinFilter::LinearMipMapLinear, osg::Texture::FilterMode::LINEAR_MIPMAP_LINEAR},
		};
	};
}