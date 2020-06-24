/**
 * \brief  Scene to hold sky box and objects
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/06/15
 * */

#pragma once
#include <SceneManager/Environment/Enviroment.h>
#include <osgViewer/Viewer>

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
			void AddObject(osg::ref_ptr<osg::Node> pNode);
			void AddObjects(const std::vector<osg::ref_ptr<osg::Node>>& vecNode);
			void CleanObjects();
			void SetViewer(osg::ref_ptr<osgViewer::Viewer> pViewer);
		private:
			osg::ref_ptr<osg::Group>	m_pObjects;
			osg::ref_ptr<Enviroment>	m_pEnvironment;
			osg::ref_ptr<osgViewer::Viewer>	m_pViewer;
		};
	}	// namespace Scene
}	// namespace PFD