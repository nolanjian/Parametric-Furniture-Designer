#pragma once

#include "PFDGLTFExport.h"
#include "OSGIncluding.h"
#include "STLIncluding.h"
#include "fx/gltf.h"

namespace PFD
{
	namespace GLTF
	{
		// JUST Load, do not do params update
		class PFD_GLTF_API Exporter
		{
		public:
			Exporter();
			~Exporter();

			bool Export(osg::ref_ptr<osg::Group> scene, const std::wstring& path);
			bool Export(osg::ref_ptr<osg::Group> scene, std::string& utf8Json);
			bool Export(osg::ref_ptr<osg::Group> scene);

			std::shared_ptr<fx::gltf::Document> gltf;
		};
	}
}