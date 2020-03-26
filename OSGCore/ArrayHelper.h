#pragma once
#include <fx/gltf.h>
#include "OSGIncluding.h"
#include "STLIncluding.h"

class ArrayHelper
{
public:
	ArrayHelper(const ArrayHelper&) = delete;
	ArrayHelper(const ArrayHelper&&) = delete;
	ArrayHelper& operator=(const ArrayHelper&) = delete;
	ArrayHelper& operator=(const ArrayHelper&&) = delete;
	ArrayHelper() = delete;

	explicit ArrayHelper(std::shared_ptr<fx::gltf::Document> gltfObject);
	void Init();
	osg::ref_ptr<osg::Array> GetArray(size_t index);
protected:
	osg::ref_ptr<osg::Array> GetOSGArray(const fx::gltf::Accessor& accessor);
	template<class ArrayType>
	osg::ref_ptr<osg::Array> GetOSGVecArrayImp(const fx::gltf::Accessor& accessor);
	template<class ArrayType>
	osg::ref_ptr<osg::Array> GetOSGArrayImp(const fx::gltf::Accessor& accessor);
private:
	std::shared_ptr<fx::gltf::Document> m_gltfObject;
	std::map<size_t, osg::ref_ptr<osg::Array>>	mapArray;
};
