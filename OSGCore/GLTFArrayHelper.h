#pragma once
#include "OSGIncluding.h"
#include "STLIncluding.h"
#include <fx/gltf.h>
#include "../easyloggingpp/easylogging++.h"
#include "stb_image.h"
#include "IGLTFResourceManager.h"

class GLTFArrayHelper : public IGLTFResourceManager
{
public:
	static GLTFArrayHelper& GetInstance();
	virtual void Load() override;

	osg::ref_ptr<osg::Array> GetArrayByAccessorIndex(size_t index);
protected:
	osg::ref_ptr<osg::RefMatrix> GetOSGMatrix(const fx::gltf::Accessor& accessor);
	osg::ref_ptr<osg::Array> GetOSGArray(const fx::gltf::Accessor& accessor);
	template<class ArrayType>
	osg::ref_ptr<osg::Array> GetOSGVecArrayImp(const fx::gltf::Accessor& accessor);
	template<class ArrayType>
	osg::ref_ptr<osg::Array> GetOSGArrayImp(const fx::gltf::Accessor& accessor);
private:
	//GLTFArrayHelper() {}
	std::map<size_t, osg::ref_ptr<osg::Array>>	mapArray;
	std::map<size_t, osg::ref_ptr<osg::RefMatrix>> mapMatrix;
};
