/*****************************************************************//**
 * \file   Exporter.h
 * \brief  GLTF Exporter Interface
 * 
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/06/09
 * 
 *********************************************************************/
#pragma once
#include "PFDGLTFExport.h"
#include <Commom/OSGIncluding.h>
#include <Commom/STLIncluding.h>
#include "fx/gltf.h"

namespace PFD
{
	namespace GLTF
	{
		class PFD_GLTF_API Exporter
		{
		public:
			static std::shared_ptr<Exporter> Create();
			virtual bool Export(osg::ref_ptr<osg::Group> scene, const std::wstring& path) = 0;
			virtual bool Export(osg::ref_ptr<osg::Group> scene, std::string& utf8Json) = 0;
			virtual bool Export(osg::ref_ptr<osg::Group> scene) = 0;
		};
	}
}
