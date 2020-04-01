#include "pch.h"
#include "PickEventHandler.h"

bool PickEventHandler::handle(osgGA::Event* event, osg::Object* object, osg::NodeVisitor* nv)
{
	return osgGA::GUIEventHandler::handle(event, object, nv);
}

bool PickEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor*)
{
	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
	if (!viewer)
		return false;

	switch (ea.getEventType())
	{
	case(osgGA::GUIEventAdapter::PUSH):
		switch (ea.getButtonMask())
		{
		case osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON:
			PickObject(viewer, ea.getX(), ea.getY());
			return true;
		default:
			return false;
		}
		break;
	case osgGA::GUIEventAdapter::KEYDOWN:
		if (m_scene)
		{
			//osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
			//m_scene->getOrCreateStateSet()->setAttribute(
			//	pm, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);


			//m_scene->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
		}
	default:
		break;
	}
	return false;
}

//bool PickEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
//{
//	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
//	if (!viewer) 
//		return false;
//	switch (ea.getEventType())
//	{
//	case(osgGA::GUIEventAdapter::PUSH):
//		switch (ea.getButtonMask())
//		{
//		case osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON:
//			return true;
//		default:
//			return false;
//		}
//		break;
//	default:
//		break;
//	}
//	return false;
//}

void PickEventHandler::PickObject(osgViewer::Viewer* viewer, double x, double y)
{
	if (!viewer)
		return;

	osgUtil::LineSegmentIntersector::Intersections intersections;
	if (viewer->computeIntersections(x, y, intersections))
	{
		//for (const osgUtil::LineSegmentIntersector::Intersection& intersection : intersections)
		//{
		//	for (osg::Node* node : intersection.nodePath)
		//	{
		//		std::cout << node->getName() << std::endl;
		//	}

		//}
	}
}

PickEventHandler::~PickEventHandler()
{
}
