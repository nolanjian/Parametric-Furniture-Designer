#include "SkyDome.h"
#include "../Utils/Utils.h"

namespace PFD
{
	namespace Scene
	{
		SkyDome::SkyDome()
			:Geode::Geode()
		{
			setName("SkyDome");
			getOrCreateStateSet();
			InitCubMap();
			InitShader();
			SetRadius(m_pConfig->GetDouble("SKY_DOME__DEFAULT_RADIUS"));
		}

		void SkyDome::SetRadius(double dRadius)
		{
			if (m_pSphereDrawable == nullptr)
			{
				m_pSphereDrawable = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0, 0, 0), dRadius));
				addDrawable(m_pSphereDrawable);
				return;
			}
			
			assert(m_pSphereDrawable);
			osg::Sphere* pSphere = dynamic_cast<osg::Sphere*>(m_pSphereDrawable->getShape());
			if (pSphere)
			{
				pSphere->setRadius(dRadius);
			}
		}

		double SkyDome::GetRadius() const
		{
			if (m_pSphereDrawable)
			{
				osg::Sphere* pSphere = dynamic_cast<osg::Sphere*>(m_pSphereDrawable->getShape());
				if (pSphere)
				{
					return pSphere->getRadius();
				}
			}
			return 0.0;
		}

		bool SkyDome::InitShader()
		{
			assert(m_pConfig);

			std::string strVShader = Utils::LoadStringFromFile(m_pConfig->GetString("SKY_DOME_V_SHADER_PATH"));
			std::string strFShader = Utils::LoadStringFromFile(m_pConfig->GetString("SKY_DOME_F_SHADER_PATH"));

			m_pVShader = new osg::Shader(osg::Shader::VERTEX, strVShader);
			m_pFShader = new osg::Shader(osg::Shader::FRAGMENT, strFShader);

			if (!m_pVShader.valid() || !m_pFShader.valid())
			{
				return false;
			}

			m_pProgram = new osg::Program();
			m_pProgram->addShader(m_pVShader);
			m_pProgram->addShader(m_pFShader);

			getOrCreateStateSet()->setAttribute(m_pProgram);
			return true;
		}

		bool SkyDome::InitCubMap()
		{
			assert(m_pConfig);
			m_pTextureCubeMap = new osg::TextureCubeMap();

			const std::vector<std::pair<osg::TextureCubeMap::Face, std::string>> dirMappings
			{
				{osg::TextureCubeMap::POSITIVE_X, "PX"},
				{osg::TextureCubeMap::NEGATIVE_X, "NX"},
				{osg::TextureCubeMap::POSITIVE_Y, "PY"},
				{osg::TextureCubeMap::NEGATIVE_Y, "NY"},
				{osg::TextureCubeMap::POSITIVE_Z, "PZ"},
				{osg::TextureCubeMap::NEGATIVE_Z, "NZ"},
			};

			for (const auto& pair : dirMappings)
			{
				osg::ref_ptr<osg::Image> pImage = Utils::LoadImageFromPath(m_pConfig->GetString("SKY_DOME_" + pair.second));
				if (!pImage)
				{
					// 
				}
				m_pTextureCubeMap->setImage(pair.first, pImage);
			}

			getOrCreateStateSet()->setTextureAttributeAndModes(0, m_pTextureCubeMap);

			return true;
		}

	}	// namespace Scene
}	// namespace PFD
