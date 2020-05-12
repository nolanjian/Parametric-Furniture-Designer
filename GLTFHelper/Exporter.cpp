#include "Exporter.h"

namespace PFD_GLTF_Helper
{

	Exporter::Exporter()
	{
	}

	Exporter::~Exporter()
	{
	}

	bool Exporter::Export(osg::ref_ptr<osg::Group> scene, const std::wstring& path)
	{
		return false;
	}

	bool Exporter::Export(osg::ref_ptr<osg::Group> scene, std::string& utf8Json)
	{
		return false;
	}

	bool Exporter::Export(osg::ref_ptr<osg::Group> scene)
	{
		return false;
	}


}