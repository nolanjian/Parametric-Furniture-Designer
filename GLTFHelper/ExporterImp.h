#pragma once
#include "Exporter.h"



namespace PFD
{
	namespace GLTF
	{
		class ExporterImp : public Exporter
		{
		public:
			ExporterImp();
			virtual ~ExporterImp();

			virtual bool Export(osg::ref_ptr<osg::Group> scene, const std::wstring& path) override;
			virtual bool Export(osg::ref_ptr<osg::Group> scene, std::string& utf8Json) override;
			virtual bool Export(osg::ref_ptr<osg::Group> scene) override;

		private:
			std::shared_ptr<fx::gltf::Document> gltf;
		};
	}
}