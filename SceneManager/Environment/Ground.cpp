#include "Ground.h"

namespace PFD
{
	namespace Scene
	{
		Ground::Ground()
			: EnvriomentBaseGeode()
		{
			getOrCreateStateSet();
			setName("Ground");
		}

		bool Ground::Init()
		{
			if (!InitShader() || !InitTexture())
			{
				return false;
			}

			SetHalfLength(m_pConfig->GetDouble("GROUND_DEFAULT_HALF_LENGTH"));
			return true;
		}

		void Ground::SetHalfLength(double dHalfLength)
		{
			static double dFixHeight = 10.0;
			if (!m_pBoxDrawable)
			{
				m_pBoxDrawable = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0, 0, 0), dHalfLength * 2, dHalfLength * 2, dFixHeight));
				addDrawable(m_pBoxDrawable);
			}
			
			//assert(m_pBoxDrawable);
			//osg::Box* pBox = dynamic_cast<osg::Box*>(m_pBoxDrawable->getShape());
			//if (pBox)
			//{
			//	pBox->set(osg::Vec3(0, 0, 0), osg::Vec3(dHalfLength, dHalfLength, dFixHeight));
			//}
			//assert(pBox);
		}

		double Ground::GetHalfLength() const
		{
			if (m_pBoxDrawable)
			{
				osg::Box* pBox = dynamic_cast<osg::Box*>(m_pBoxDrawable->getShape());
				if (pBox)
				{
					return pBox->getHalfLengths().x();
				}
			}
			return 0.0;
		}

		bool Ground::InitTexture()
		{
			assert(m_pConfig);
			osg::ref_ptr<osg::Image> pImage = Utils::LoadImageFromPath(m_pConfig->GetString("GROUND_IMAGE_PATH"));
			if (!pImage)
			{
				return false;
			}

			m_pTexture2D = new osg::Texture2D(pImage);
			getOrCreateStateSet()->setTextureAttributeAndModes(0, m_pTexture2D);
			getOrCreateStateSet()->addUniform(new osg::Uniform("GroundTex2D", 0));
			return true;
		}

		bool Ground::InitShaderSource()
		{
			assert(m_pConfig);

			m_strVShader = Utils::LoadStringFromFile(m_pConfig->GetString("GROUND_V_SHADER_PATH"));
			m_strFShader = Utils::LoadStringFromFile(m_pConfig->GetString("GROUND_F_SHADER_PATH"));
			return !m_strVShader.empty() && !m_strFShader.empty();
		}
	}	// namespace Scene
}	// namespace PFD
