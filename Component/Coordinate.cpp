#include <Component/Coordinate.h>
#include <Component/Axis.h>

namespace PFD
{
	namespace Component
	{
		Coordinate::Coordinate(unsigned int uLen)
			:m_uLen(uLen)
		{
			Init();
			setName("Coordinate");
		}

		void Coordinate::Init()
		{
			removeChildren(0, getNumChildren());

			osg::ref_ptr<Axis>	pAxisX = new Axis(Axis::Dim::X, 1);
			osg::ref_ptr<Axis>	pAxisY = new Axis(Axis::Dim::Y, 1);
			osg::ref_ptr<Axis>	pAxisZ = new Axis(Axis::Dim::Z, 1);
			addChild(pAxisX);
			addChild(pAxisY);
			addChild(pAxisZ);
		}

	}
}