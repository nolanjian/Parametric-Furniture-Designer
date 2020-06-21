/*****************************************************************//**
 * \file   Importer.h
 * \brief  GLTF Importer Interface
 * 
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/06/09
 * 
 *********************************************************************/
#pragma once
#include "PFDGLTFExport.h"
#include "fx/gltf.h"
#include <Commom/OSGIncluding.h>
#include <Commom/STLIncluding.h>

namespace PFD
{
	namespace GLTF
	{
		class PFD_GLTF_API Importer
		{
		public:
			static std::shared_ptr<Importer> Create();
			virtual ~Importer() {}
			virtual osg::ref_ptr<osg::Group> Load(const std::wstring& unicodePath) = 0;
			virtual osg::ref_ptr<osg::Group> Load(const std::string& utf8Json) = 0;
			virtual osg::ref_ptr<osg::Group> Load(std::shared_ptr<fx::gltf::Document> gltf) = 0;
			virtual std::shared_ptr<fx::gltf::Document> GetGLTF() = 0;
		private:
		};
	}
}
