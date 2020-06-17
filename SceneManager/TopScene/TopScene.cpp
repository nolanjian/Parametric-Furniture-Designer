#include "TopScene.h"
#include "../SkyBox/SkyBox.h"

namespace PFD
{
	namespace Scene
	{
		void TopScene::ReInit()
		{
			ReInitSkyBox();
			m_pObjects = new osg::Group();
		}

		void TopScene::ReInitSkyBox()
		{
			osg::ref_ptr<SkyBox>	pSkyBox = nullptr;
			for (auto& child : _children)
			{
				SkyBox* pTmp = dynamic_cast<SkyBox*>(child.get());
				if (pTmp)
				{
					pSkyBox = pTmp;
				}
			}
			if (!pSkyBox)
			{
				pSkyBox = new SkyBox();
				addChild(pSkyBox);
			}

			pSkyBox->Init();
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

		void TopScene::SetSkyBox()
		{
		}

	}	// namespace Scene
}	// namespace PFD