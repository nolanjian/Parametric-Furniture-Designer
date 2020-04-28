#pragma once
#include "STLIncluding.h"
#include "OSGIncluding.h"

namespace PFDCore
{


	class PickEventHandler : public osgGA::GUIEventHandler
	{
	public:
		PickEventHandler(osg::ref_ptr<osg::Group> scene)
			:m_scene(scene)
		{}
		PickEventHandler() {}
		PickEventHandler(const PickEventHandler& eh, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY) :
			osg::Object(eh, copyop),
			osg::Callback(eh, copyop),
			osgGA::GUIEventHandler(eh, copyop) {}

		//META_Object(PFD, PickEventHandler);

		/** Handle event. Override the handle(..) method in your event handlers to respond to events. */
		virtual bool handle(osgGA::Event* event, osg::Object* object, osg::NodeVisitor* nv) override;

		/** Handle events, return true if handled, false otherwise. */
		virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor*) override;

		/** Deprecated, Handle events, return true if handled, false otherwise. */
		//virtual bool handle(const osgGA::GUIEventAdapter&, osgGA::GUIActionAdapter&) override;

	protected:
		void PickObject(osgViewer::Viewer* viewer, double x, double y);

		virtual ~PickEventHandler();
	private:
		osg::ref_ptr<osg::Group> m_scene;
	};

}