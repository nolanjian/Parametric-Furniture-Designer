#pragma once
#include "GLTFHelperExport.h"
#include "OSGIncluding.h"
#include "STLIncluding.h"
#include "fx/gltf.h"

namespace PFD_GLTF_Helper
{
	// JUST Load, do not do params update
	class PFD_GLTF_HELPER_API Loader
	{
	public:
		Loader();
		~Loader();

		osg::ref_ptr<osg::Group> Load(const std::wstring& path);
		osg::ref_ptr<osg::Group> Load(const std::string& utf8Json);
		osg::ref_ptr<osg::Group> Load(std::shared_ptr<fx::gltf::Document> gltf);

		std::shared_ptr<fx::gltf::Document> doc;
	};
}