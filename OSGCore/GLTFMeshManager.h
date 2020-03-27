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
	osg::ref_ptr<osg::Geode> GetMesh(size_t index) { return mapGeode[index]; }
protected:
	osg::ref_ptr<osg::Drawable>	LoadDrawable(const fx::gltf::Primitive& primitive);
	osg::ref_ptr<osg::PrimitiveSet> LoadPrimitiveSet(const fx::gltf::Primitive& primitive);
	osg::ref_ptr<osg::DrawElements> LoadDrawElements(const fx::gltf::Primitive& primitive);
private:
	//GLTFMeshManager() {}
	std::string	m_strGLTFPath;
	std::shared_ptr<fx::gltf::Document> m_gltfObject;
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