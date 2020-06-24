#include "EnvriomentBaseGeode.h"

namespace PFD
{
	namespace Scene
	{
		EnvriomentBaseGeode::EnvriomentBaseGeode()
			: osg::Geode()
		{
		}

		bool EnvriomentBaseGeode::Init()
		{
			InitShader();
			return false;
		}

		bool EnvriomentBaseGeode::InitShader()
		{
			assert(m_pConfig);
			if (!InitShaderSource())
			{
				return false;
			}
			
			osg::Shader* pVShader = new osg::Shader(osg::Shader::VERTEX, m_strVShader);
			assert(pVShader);
			pVShader->setName(getName() + "_VertexShader");

			osg::Shader* pFShader = new osg::Shader(osg::Shader::FRAGMENT, m_strFShader);
			assert(pFShader);
			pFShader->setName(getName() + "_FlagShader");

			osg::Program* pProgram = new osg::Program();
			assert(pProgram);
			pProgram->setName(getName() +  "_ShaderProgram");
			pProgram->addShader(pVShader);
			pProgram->addShader(pFShader);

			getOrCreateStateSet()->setAttribute(pProgram);
			return true;
		}

	}	// namespace Scene
}	// namespace PFD