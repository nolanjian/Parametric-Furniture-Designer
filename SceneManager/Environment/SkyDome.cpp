#include <SceneManager/Environment/SkyDome.h>
#include <Utils/Utils.h>
#include <osgUtil/Tessellator>

namespace PFD
{
	namespace Scene
	{
		SkyDome::SkyDome()
			: EnvriomentBaseGeode()
		{
			getOrCreateStateSet();
			setName("SkyDome");
		}

		bool SkyDome::Init()
		{
			if (!InitShader() || !InitCubMap())
			{
				return false;
			}

			SetRadius(m_pConfig->GetDouble("SKY_DOME_DEFAULT_RADIUS"));
			return true;
		}

		void SkyDome::SetRadius(double dRadius)
		{
			if (m_pSphereDrawable == nullptr)
			{
				m_pSphereDrawable = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0, 0, 0), dRadius));
				addDrawable(m_pSphereDrawable);
			}

			//assert(m_pSphereDrawable);
			//osg::Sphere* pSphere = dynamic_cast<osg::Sphere*>(m_pSphereDrawable->getShape());
			//if (pSphere)
			//{
			//	pSphere->setRadius(dRadius);

			//	//osg::ref_ptr<osgUtil::Tessellator>tscx = new osgUtil::Tessellator();
			//	//tscx->setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
			//	//tscx->setBoundaryOnly(false);
			//	//tscx->setWindingType(osgUtil::Tessellator::TESS_WINDING_NONZERO);
			//	//tscx->retessellatePolygons(*m_pSphereDrawable);
			//}
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

		bool SkyDome::InitShaderSource()
		{
			assert(m_pConfig);
			m_strVShader = Utils::LoadStringFromFile(m_pConfig->GetString("SKY_DOME_V_SHADER_PATH"));
			m_strFShader = Utils::LoadStringFromFile(m_pConfig->GetString("SKY_DOME_F_SHADER_PATH"));
			return !m_strVShader.empty() && !m_strFShader.empty();
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
					return false;
				}
				m_pTextureCubeMap->setImage(pair.first, pImage);
			}

			getOrCreateStateSet()->setTextureAttributeAndModes(0, m_pTextureCubeMap);
			getOrCreateStateSet()->addUniform(new osg::Uniform("SkyDomeCubeMap", 0));

			return true;
		}

	}	// namespace Scene
}	// namespace PFD
