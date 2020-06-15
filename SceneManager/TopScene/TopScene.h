/**
 * \brief  Scene to hold sky box and objects
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/06/15
 * */

#pragma once
#include "OSGIncluding.h"

namespace PFD
{
	namespace Scene
	{
		class TopScene : public osg::Group
		{
		public:
			enum RenderMode
			{
				Photorealistic = 1000,
				WireFrames,
				Cartoon,
				BlackAndWhite,
			};

			void ReInit();
			void ReInitSkyBox();
			void AddObject(osg::ref_ptr<osg::Node> pNode);
			void AddObjects(const std::vector<osg::ref_ptr<osg::Node>>& vecNode);
			void CleanObjects();
			void SetSkyBox();
		private:
			osg::ref_ptr<osg::Group>	m_pObjects;
			osg::ref_ptr<osg::Node>		m_pSkyBox;
		};
	}	// namespace Scene
}	// namespace PFD