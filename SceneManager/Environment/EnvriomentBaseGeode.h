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

#include <Commom/OSGIncluding.h>
#include <Config/IProgramConfig.h>

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