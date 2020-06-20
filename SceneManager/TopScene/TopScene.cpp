#include "TopScene.h"

namespace PFD
{
	namespace Scene
	{
		void TopScene::ReInit()
		{
			removeChildren(0, getNumChildren());

			m_pObjects = new osg::Group();
			m_pEnvironment = new Enviroment();
			addChild(m_pObjects);
			addChild(m_pEnvironment);
		}

		void TopScene::AddObject(osg::ref_ptr<osg::Node> pNode)
		{
			if (m_pObjects)
			{
				m_pObjects->addChild(pNode);
			}
		}

		void TopScene::AddObjects(const std::vector<osg::ref_ptr<osg::Node>>& vecNode)
		{
			if (m_pObjects)
			{
				for (auto node : vecNode)
				{
					m_pObjects->addChild(node);
				}
			}
		}

		void TopScene::CleanObjects()
		{
			if (m_pObjects && m_pObjects->getNumChildren() > 0)
			{
				m_pObjects->removeChildren(0, m_pObjects->getNumChildren());
			}
		}

		void TopScene::SetViewer(osg::ref_ptr<osgViewer::Viewer> pViewer)
		{
			m_pViewer = pViewer;
		}

	}	// namespace Scene
}	// namespace PFD