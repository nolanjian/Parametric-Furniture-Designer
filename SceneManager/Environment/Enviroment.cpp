#include "Enviroment.h"

namespace PFD
{
	namespace Scene
	{
		EnviromentCameraTrackCallback::EnviromentCameraTrackCallback(Enviroment* pEnviroment)
			: m_pEnviroment(pEnviroment)
		{

		}

		void EnviromentCameraTrackCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
		{
			if (m_pEnviroment && m_pEnviroment == node
				&& nv && nv->getVisitorType() == osg::NodeVisitor::CULL_VISITOR)
			{
				osgUtil::CullVisitor* pCullVisitor = dynamic_cast<osgUtil::CullVisitor*>(nv);
				if (pCullVisitor)
				{
					osg::Vec3 centre, up, eye;
					pCullVisitor->getRenderStage()->getCamera()->getViewMatrixAsLookAt(eye, centre, up);
					Enviroment* pEnviroment = dynamic_cast<Enviroment*>(node);
					if (pEnviroment)
					{
						double dRadius = osg::absolute<double>(eye.z()) * 1.5;
						osg::Vec3 pos(eye.x(), eye.y(), dRadius);
						pEnviroment->setMatrix(osg::Matrix::translate(pos));
						// maybe no need to build every time, just build when out off range
						pEnviroment->Build(dRadius);
					}
				}
			}
			traverse(node, nv);
		}

		Enviroment::Enviroment()
			: osg::MatrixTransform()
		{
			m_pSkyDome = new SkyDome();
			addChild(m_pSkyDome);
			m_pGround = new Ground();
			addChild(m_pGround);

			setCullCallback(new EnviromentCameraTrackCallback());
		}

		void Enviroment::Build(double dRadius)
		{
			assert(m_pSkyDome && m_pGround);

			

			m_pSkyDome->SetRadius(dRadius);
			m_pGround->SetHalfLength(dRadius);
		}

	}	// namespace Scene
}	// namespace PFD