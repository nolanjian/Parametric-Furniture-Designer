/*****************************************************************//**
 * \file   GLTFResourceManager.h
 * \brief  
 * 
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/06/24
 * 
 *********************************************************************/
#pragma once

#include <GLTFHelper/IGLTFResourceManager.h>
#include <osg/Texture2D>
#include <osg/Geometry>

namespace PFD
{
	namespace GLTF
	{
		class GLTFResourceManager : public IGLTFResourceManager
		{
		public:
			explicit GLTFResourceManager(std::shared_ptr<fx::gltf::Document> gltfObject);

			virtual osg::ref_ptr<osg::Array> GetArrayByAccessorIndex(size_t index) override;
			virtual osg::ref_ptr<osg::Texture> GetTexture(size_t index) override;
			virtual osg::ref_ptr<osg::Geode> GetMesh(size_t index) override;

		protected:
			void LoadArray();
			osg::ref_ptr<osg::RefMatrix> GetOSGMatrix(const fx::gltf::Accessor& accessor);
			osg::ref_ptr<osg::Array> GetOSGArray(const fx::gltf::Accessor& accessor);
			template<class ArrayType>
			osg::ref_ptr<osg::Array> GetOSGVecArrayImp(const fx::gltf::Accessor& accessor);
			template<class ArrayType>
			osg::ref_ptr<osg::Array> GetOSGArrayImp(const fx::gltf::Accessor& accessor);

			void LoadTexture();
			void LoadSampler(osg::ref_ptr<osg::Texture2D> pTexture, const fx::gltf::Sampler& sampler);
			void LoadImage(osg::ref_ptr<osg::Texture2D> pTexture, const fx::gltf::Image& image);

			void LoadMesh();
			osg::ref_ptr<osg::Drawable>	LoadDrawable(const fx::gltf::Primitive& primitive);
			osg::ref_ptr<osg::PrimitiveSet> LoadPrimitiveSet(const fx::gltf::Primitive& primitive);
			osg::ref_ptr<osg::DrawElements> LoadDrawElements(const fx::gltf::Primitive& primitive);
			bool LoadMaterial(const fx::gltf::Material& material, osg::ref_ptr<osg::Geometry> pGeometry);
			bool LoadPBRTexture(const fx::gltf::Material::PBRMetallicRoughness& pbrMaterial, osg::ref_ptr<osg::Geometry> pGeometry);
			bool LoadNormalTexture(const fx::gltf::Material::NormalTexture& normalTexture, osg::ref_ptr<osg::Geometry> pGeometry);
			bool LoadOcclusionTexture(const fx::gltf::Material::OcclusionTexture& occlusionTexture, osg::ref_ptr<osg::Geometry> pGeometry);
		private:
			std::string	m_strGLTFPath;
			std::shared_ptr<fx::gltf::Document> m_gltfObject;
			std::shared_ptr<spdlog::logger> logger = spdlog::get(PFD_LOGGER);

			std::map<size_t, osg::ref_ptr<osg::Array>>		m_mapArray;
			std::map<size_t, osg::ref_ptr<osg::RefMatrix>>	m_mapMatrix;
			std::map<size_t, osg::ref_ptr<osg::Texture>>	m_mapTexture;
			std::map<size_t, osg::ref_ptr<osg::Geode>>		m_mapGeode;

			const std::map<fx::gltf::Sampler::WrappingMode, osg::Texture::WrapMode>	m_mapWrapMode{
				{fx::gltf::Sampler::WrappingMode::ClampToEdge, osg::Texture::WrapMode::CLAMP},
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

			const std::map<fx::gltf::Primitive::Mode, osg::PrimitiveSet::Mode>	m_mapPrimitives = {
				{fx::gltf::Primitive::Mode::Points, osg::PrimitiveSet::POINTS},
				{fx::gltf::Primitive::Mode::Lines, osg::PrimitiveSet::LINES},
				{fx::gltf::Primitive::Mode::LineLoop, osg::PrimitiveSet::LINE_LOOP},
				{fx::gltf::Primitive::Mode::LineStrip, osg::PrimitiveSet::LINE_STRIP},
				{fx::gltf::Primitive::Mode::Triangles, osg::PrimitiveSet::TRIANGLES},
				{fx::gltf::Primitive::Mode::TriangleStrip, osg::PrimitiveSet::TRIANGLE_STRIP},
				{fx::gltf::Primitive::Mode::TriangleFan, osg::PrimitiveSet::TRIANGLE_FAN},
			};
		};
	}
}