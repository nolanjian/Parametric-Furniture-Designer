#pragma once
/**
 * \brief  SkyBox
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/06/15
 * */

#include "OSGIncluding.h"
#include "../Utils/Utils.h"

namespace PFD
{
	namespace Scene
	{
		class ImageInfo
		{
		public:
			std::string strPath;
			int nWidth = 0;
			int nHeight = 0;
			int nChannels = 0;
			unsigned char* pImgObj = nullptr;
		public:
			bool valid();
			void Load();
			osg::ref_ptr<osg::Image> GetSubImage(const size_t x, const size_t y, const size_t width, const size_t height);
		};

		class SkyBox : public osg::Group
		{
		public:
			void Init();
		protected:
		private:
			void InitGeode();
			void LoadFilePath();
			void LoadCudeMap();
			void LoadImages();

		private:
			std::shared_ptr<spdlog::logger> logger = spdlog::get(PFD_LOGGER);
			osg::ref_ptr<osg::TextureCubeMap> m_pTextureCubeMap;
			ImageInfo	m_SkyImageInfo;
			ImageInfo	m_GroundImageInfo;
		};
	}	// namespace Scene
}	// namespace PFD