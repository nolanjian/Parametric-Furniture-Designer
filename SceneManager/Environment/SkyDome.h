/*****************************************************************//**
 * \file   SkyDome.h
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
		class SkyDome : public osg::Geode
		{
		public:
			SkyDome();
			void SetRadius(double dRadius);
			double GetRadius() const;

		protected:
			bool InitShader();
			bool InitCubMap();
		private:
			osg::ref_ptr<osg::ShapeDrawable>	m_pSphereDrawable;
			
			std::string	m_strSkyImagePath;
			osg::ref_ptr<osg::TextureCubeMap>	m_pTextureCubeMap;

			osg::ref_ptr<osg::Program>	m_pProgram;
			osg::ref_ptr<osg::Shader>	m_pVShader;
			osg::ref_ptr<osg::Shader>	m_pFShader;

			Config::IProgramConfig* m_pConfig = Config::IProgramConfig::GetInstance();
		};
	}	// namespace Scene
}	// namespace PFD