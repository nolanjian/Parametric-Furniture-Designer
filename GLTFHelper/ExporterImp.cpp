#include "ExporterImp.h"

namespace PFD
{
	namespace GLTF
	{

		ExporterImp::ExporterImp()
		{
		}

		ExporterImp::~ExporterImp()
		{
		}

		bool ExporterImp::Export(osg::ref_ptr<osg::Group> scene, const std::wstring& path)
		{
			return false;
		}

		bool ExporterImp::Export(osg::ref_ptr<osg::Group> scene, std::string& utf8Json)
		{
			return false;
		}

		bool ExporterImp::Export(osg::ref_ptr<osg::Group> scene)
		{
			return false;
		}


	}
}