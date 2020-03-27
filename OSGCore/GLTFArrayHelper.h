#pragma once
#include <fx/gltf.h>
#include "OSGIncluding.h"
#include "STLIncluding.h"
#include "IGLTFResourceManager.h"

class GLTFArrayHelper : public IGLTFResourceManager
{
public:
	static GLTFArrayHelper& GetInstance();
	virtual void Load() override;

	osg::ref_ptr<osg::Array> GetArray(size_t index);
protected:
	osg::ref_ptr<osg::Array> GetOSGArray(const fx::gltf::Accessor& accessor);
	template<class ArrayType>
	osg::ref_ptr<osg::Array> GetOSGVecArrayImp(const fx::gltf::Accessor& accessor);
	template<class ArrayType>
	osg::ref_ptr<osg::Array> GetOSGArrayImp(const fx::gltf::Accessor& accessor);
private:
	//GLTFArrayHelper() {}
	std::map<size_t, osg::ref_ptr<osg::Array>>	mapArray;
};
