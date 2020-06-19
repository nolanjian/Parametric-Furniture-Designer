/*****************************************************************//**
 * \file   Enviroment.h
 * \brief  
 * 
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/06/19
 * 
 *********************************************************************/
#pragma once

#include <OSGIncluding.h>

#include "SkyDome.h"
#include "Ground.h"

namespace PFD
{
	namespace Scene
	{
		class EnviromentCameraTrackCallback : public osg::NodeCallback
		{
		public:
			virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) override;
		};

		class Enviroment : public osg::MatrixTransform
		{
		public:
			Enviroment();
			void Build(double dRadius);
		protected:
		private:
			osg::ref_ptr<Group>		m_pObjectTree;
			osg::ref_ptr<SkyDome>	m_pSkyDome;
			osg::ref_ptr<Ground>	m_pGround;
		};
	}	// namespace Scene
}	// namespace PFD