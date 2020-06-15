#include "SkyBox.h"
#include "stb_image.h"
#include "../Utils/Utils.h"
#include "../Config/IProgramConfig.h"

namespace PFD
{
	namespace Scene
	{
		void SkyBox::Init()
		{
			LoadFilePath();
			LoadCudeMap();
		}

		void SkyBox::InitGeode()
		{
			osg::ref_ptr<osg::Geode> pGeode = new osg::Geode();
			addChild(pGeode);

			osg::Box* cube = new osg::Box(osg::Vec3(), 10000);
			osg::ShapeDrawable* cudeDrawable = new osg::ShapeDrawable(cube);

			pGeode->setCullingActive(false);
			pGeode->addDrawable(cudeDrawable);

			pGeode->getOrCreateStateSet()->setRenderBinDetails(0, "RenderBin");
		}

		void SkyBox::LoadFilePath()
		{
			PFD::Config::IProgramConfig* pConfig = PFD::Config::IProgramConfig::GetInstance();
			m_SkyImageInfo.strPath = pConfig->GetString("SkyImage");
			m_GroundImageInfo.strPath = pConfig->GetString("GroundImage");
		}

		void SkyBox::LoadCudeMap()
		{
			LoadImages();

			m_pTextureCubeMap = new osg::TextureCubeMap();
			osg::Image* pImage;

			// sky
			m_pTextureCubeMap->setImage(osg::TextureCubeMap::POSITIVE_X, m_SkyImageInfo.GetSubImage(0, 1024, 1024, 1024));
			m_pTextureCubeMap->setImage(osg::TextureCubeMap::NEGATIVE_X, m_SkyImageInfo.GetSubImage(1024, 1024, 1024, 1024));
			m_pTextureCubeMap->setImage(osg::TextureCubeMap::POSITIVE_Y, m_SkyImageInfo.GetSubImage(2048, 1024, 1024, 1024));
			m_pTextureCubeMap->setImage(osg::TextureCubeMap::NEGATIVE_Y, m_SkyImageInfo.GetSubImage(3072, 1024, 1024, 1024));
			m_pTextureCubeMap->setImage(osg::TextureCubeMap::NEGATIVE_Z, m_SkyImageInfo.GetSubImage(2048, 0, 1024, 1024));
			
			// Ground
			m_pTextureCubeMap->setImage(osg::TextureCubeMap::POSITIVE_Z, m_GroundImageInfo.GetSubImage(0, 0, 1024, 1024));

			m_pTextureCubeMap->setUnRefImageDataAfterApply(true);
			m_pTextureCubeMap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
			m_pTextureCubeMap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
			m_pTextureCubeMap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);
			m_pTextureCubeMap->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
			m_pTextureCubeMap->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

			getOrCreateStateSet()->setTextureAttributeAndModes(0, m_pTextureCubeMap);
		}

		void SkyBox::LoadImages()
		{
			try
			{
				if (!m_SkyImageInfo.valid())
				{
					m_SkyImageInfo.Load();
				}
				if (!m_GroundImageInfo.valid())
				{
					m_GroundImageInfo.Load();
				}
			}
			catch (const std::exception& ex)
			{
				logger->error(ex.what());
			}
		}

		bool ImageInfo::valid()
		{
			return nWidth > 0 && nHeight > 0 && pImgObj != nullptr;
		}

		void ImageInfo::Load()
		{
			pImgObj = stbi_load(strPath.c_str(), &nWidth, &nHeight, &nChannels, STBI_rgb_alpha);
		}

		osg::ref_ptr<osg::Image> ImageInfo::GetSubImage(size_t x, size_t y, size_t width, size_t height)
		{
			if (!valid())
			{
				return nullptr;
			}
			bool bPosible = x + width < nWidth
				&& y + height < nHeight;

			unsigned char* pBuffer = new unsigned char[width * height];

			size_t index = 0;
			unsigned char* pBegin = pImgObj + x + y * height;
			for (size_t row = 0; row < height; ++row)
			{
				unsigned char* pCurRow = pBegin + row * nWidth;
				for (size_t col = 0; col < width; ++col)
				{
					pBuffer[index] = *(pCurRow + col);
					++index;
				}
			}

			osg::ref_ptr<osg::Image>	pIMG = new osg::Image();
			pIMG->setImage(width, height, 1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, pBuffer, osg::Image::USE_NEW_DELETE);
			return pIMG;
		}

	}	// namespace Scene
}	// namespace PFD