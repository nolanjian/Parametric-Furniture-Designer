#pragma once

#include "OSGIncluding.h"
#include "STLIncluding.h"
#include "IGLTFResourceManager.h"

namespace PFDCore {
	class GLTFMeshManager : public IGLTFResourceManager
	{
	public:
		static GLTFMeshManager& GetInstance();
		virtual void Load() override;
		osg::ref_ptr<osg::Geode> GetMesh(size_t index) { return mapGeode[index]; }
	protected:
		osg::ref_ptr<osg::Drawable>	LoadDrawable(const fx::gltf::Primitive& primitive);
		osg::ref_ptr<osg::PrimitiveSet> LoadPrimitiveSet(const fx::gltf::Primitive& primitive);
		osg::ref_ptr<osg::DrawElements> LoadDrawElements(const fx::gltf::Primitive& primitive);

		bool LoadMaterial(const fx::gltf::Material& material, osg::ref_ptr<osg::Geometry> pGeometry);
		bool LoadPBRTexture(const fx::gltf::Material::PBRMetallicRoughness& pbrMaterial, osg::ref_ptr<osg::Geometry> pGeometry);
		bool LoadNormalTexture(const fx::gltf::Material::NormalTexture& normalTexture, osg::ref_ptr<osg::Geometry> pGeometry);
		bool LoadOcclusionTexture(const fx::gltf::Material::OcclusionTexture& occlusionTexture, osg::ref_ptr<osg::Geometry> pGeometry);
	private:
		//GLTFMeshManager() {}

		std::map<size_t, osg::ref_ptr<osg::Geode>>	mapGeode;

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