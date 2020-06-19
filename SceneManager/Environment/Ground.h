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

#include <OSGIncluding.h>
#include "../Config/IProgramConfig.h"

namespace PFD
{
	namespace Scene
	{
		class Ground : public osg::Geode
		{
		public:
			Ground();
			void SetHalfLength(double dHalfLength);
			double GetHalfLength() const;

		protected:
			bool InitTexture();
			bool InitShader();
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