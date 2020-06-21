#pragma once
#include "PFDComponentExport.h"
#include <Commom/OSGIncluding.h>
#include <Commom/STLIncluding.h>
#include <Utils/Utils.h>
#include <3rdPart/fx/gltf.h>
#include <spdlog/spdlog.h>

namespace PFD
{
	namespace Component
	{

		class PFD_COMPONENT_API BaseObject : public osg::MatrixTransform
		{
		public:
			BaseObject();
			virtual ~BaseObject();

			osg::BoundingBox GetBoundingBox();
			osg::BoundingSphere GetBoundingSphere();

			void	ShowCoordinate(bool bShow);

			std::shared_ptr<spdlog::logger> logger = spdlog::get(PFD_LOGGER);



		private:
			bool m_bEnableParams = false;	// TODO, if get not any params from gltf, just let it load and disable params changes.
			unsigned long long int	m_ID = 0;
			bool m_bDirty = true;
			osg::ref_ptr<BaseObject>	m_parent;

			osg::ref_ptr<osg::Array>	m_vertex;
			osg::ref_ptr<osg::Array>	m_normal;
			osg::ref_ptr<osg::Array>	m_indexl;
			osg::ref_ptr<osg::Array>	m_texCoord0;
			osg::ref_ptr<osg::Array>	m_texCoord1;
		};
	}
}