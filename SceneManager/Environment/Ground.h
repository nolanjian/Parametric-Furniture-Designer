/*****************************************************************//**
 * \file   Ground.h
 * \brief  
 * 
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/06/19
 * 
 *********************************************************************/
#pragma once

#include <SceneManager/Environment/EnvriomentBaseGeode.h>

namespace PFD
{
	namespace Scene
	{
		class Ground : public EnvriomentBaseGeode
		{
		public:
			Ground();

			virtual bool Init() override;

			void SetHalfLength(double dHalfLength);
			double GetHalfLength() const;

		protected:
			bool InitTexture();
			virtual bool InitShaderSource() override;
		private:
			Config::IProgramConfig* m_pConfig = Config::IProgramConfig::GetInstance();

			osg::ref_ptr<osg::Program>	m_pProgram;
			osg::ref_ptr<osg::Shader>	m_pVShader;
			osg::ref_ptr<osg::Shader>	m_pFShader;

			osg::ref_ptr<osg::Texture2D>	m_pTexture2D;
			osg::ref_ptr<osg::ShapeDrawable>	m_pBoxDrawable;
		};
	}	// namespace Scene
}	// namespace PFD