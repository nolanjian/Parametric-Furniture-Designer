#include "BaseObject.h"
#include "IDGenerater.h"
#include "IParamsConverter.h"
#include "Coordinate.h"

namespace PFDComponent
{
	BaseObject::BaseObject()
		: osg::MatrixTransform()
		, m_ID(PFDUtils::IDGenerater::Get()->GetNext())
	{
	}

	BaseObject::~BaseObject()
	{
	}

	osg::BoundingBox BaseObject::GetBoundingBox()
	{
		osg::ComputeBoundsVisitor boundVisitor;
		accept(boundVisitor);
		return boundVisitor.getBoundingBox();
	}

	osg::BoundingSphere BaseObject::GetBoundingSphere()
	{
		dirtyBound();
		return getBound();
	}


	//bool BaseObject::UpdatePrimitive()
	//{
	//	for (auto& child : _children)
	//	{
	//		BaseObject* bo = dynamic_cast<BaseObject*>(child.get());
	//		if (bo)
	//		{
	//			bo->UpdatePrimitive();
	//		}
	//	}
	//	return true;
	//}

	void BaseObject::ShowCoordinate(bool bShow)
	{
		std::list<osg::Node*>	lsCoordinate;
		for (osg::Node* pNode : _children)
		{
			if (pNode->getName() == "Coordinate")
			{
				lsCoordinate.push_back(pNode);
			}
		}
		for (osg::Node* pNode : lsCoordinate)
		{
			removeChild(pNode);
		}

		if (bShow)
		{
			addChild(new Coordinate());
		}
	}
}