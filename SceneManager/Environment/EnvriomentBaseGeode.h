/*****************************************************************//**
 * \file   EnvriomentBaseGeode.h
 * \brief  
 * 
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/06/24
 * 
 *********************************************************************/
#pragma once

#include <Config/IProgramConfig.h>
#include <osg/Geode>

namespace PFD
{
	namespace Scene
	{
		class EnvriomentBaseGeode : public osg::Geode
		{
		public:
			EnvriomentBaseGeode();
			virtual bool Init() = 0;
		protected:
			Config::IProgramConfig* m_pConfig = Config::IProgramConfig::GetInstance();
			
			virtual bool InitShaderSource() = 0;
			virtual bool InitShader() final;

			std::string m_strVShader;
			std::string m_strFShader;
		};
	}	// namespace Scene
}	// namespace PFD