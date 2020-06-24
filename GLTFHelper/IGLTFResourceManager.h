/*****************************************************************//**
 * \file   IGLTFResourceManager.h
 * \brief  
 * 
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/06/24
 * 
 *********************************************************************/
#pragma once
#include <Utils/Utils.h>
#include <spdlog/spdlog.h>
#include <fx/gltf.h>

namespace PFD
{
	namespace GLTF
	{
		class IGLTFResourceManager
		{
		public:
			static std::shared_ptr<IGLTFResourceManager> Load(std::shared_ptr<fx::gltf::Document> gltfObject);
			virtual osg::ref_ptr<osg::Array> GetArrayByAccessorIndex(size_t index) = 0;
			virtual osg::ref_ptr<osg::Texture> GetTexture(size_t index) = 0;
			virtual osg::ref_ptr<osg::Geode> GetMesh(size_t index) = 0;

			IGLTFResourceManager() {}
			IGLTFResourceManager(const IGLTFResourceManager&) = delete;
			IGLTFResourceManager(const IGLTFResourceManager&&) = delete;
			IGLTFResourceManager& operator= (const IGLTFResourceManager&) = delete;
			IGLTFResourceManager& operator= (const IGLTFResourceManager&&) = delete;
			virtual ~IGLTFResourceManager() {}
		};
	}
}