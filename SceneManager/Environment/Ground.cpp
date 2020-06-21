#include "Ground.h"

namespace PFD
{
	namespace Scene
	{
		Ground::Ground()
			: osg::Geode::Geode()
		{
			setName("Ground");
			getOrCreateStateSet();
			InitTexture();
			SetHalfLength(m_pConfig->GetDouble("GROUND_DEFAULT_HALF_LENGTH"));

		}

		void Ground::SetHalfLength(double dHalfLength)
		{
			static double dFixHeight = 10.0;
			if (!m_pBoxDrawable)
			{
				m_pBoxDrawable = new osg::ShapeDrawable(new osg::Box());
				addDrawable(m_pBoxDrawable);
			}
			
			assert(m_pBoxDrawable);
			osg::Box* pBox = dynamic_cast<osg::Box*>(m_pBoxDrawable->getShape());
			if (pBox)
			{
				pBox->set(osg::Vec3(0, 0, 0), osg::Vec3(dHalfLength, dHalfLength, dFixHeight));
			}
			assert(pBox);
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

		bool Ground::InitShader()
		{
			assert(m_pConfig);

			std::string strVShader = Utils::LoadStringFromFile(m_pConfig->GetString("GROUND_V_SHADER_PATH"));
			std::string strFShader = Utils::LoadStringFromFile(m_pConfig->GetString("GROUND_F_SHADER_PATH"));

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

	}	// namespace Scene
}	// namespace PFD
