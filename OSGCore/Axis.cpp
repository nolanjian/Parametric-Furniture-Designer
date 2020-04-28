#include "pch.h"
#include "Axis.h"

namespace PFDCore {

	Axis::Axis(Dim dim, unsigned int uLen)
		: m_dim(dim), m_uLen(uLen)
	{
		Init();
	}

	void Axis::Init()
	{
		removeChildren(0, getNumChildren());

		osg::ref_ptr<osg::Geode> geode = new osg::Geode;

		geode->addDrawable(new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.0f, 0.0f, 0.0f), 0.1, m_uLen * 1.0f)));
		geode->addDrawable(new osg::ShapeDrawable(new osg::Cone(osg::Vec3(0.0f, 0.0f, m_uLen * 1.0f), 0.15, 0.4)));

		osg::Matrixd	mat;

		switch (m_dim)
		{
		case Axis::X:
			mat.rotate(90, osg::Y_AXIS);
			break;
		case Axis::Y:
			mat.rotate(-90, osg::X_AXIS);
			break;
		case Axis::Z:
			break;
		default:
			break;
		}

		setMatrix(mat);

		addChild(geode);
	}

}