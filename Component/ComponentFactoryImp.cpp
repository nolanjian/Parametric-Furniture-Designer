#include "ComponentFactoryImp.h"

namespace PFDComponent
{

	ComponentFactoryImp::~ComponentFactoryImp()
	{
	}

	osg::ref_ptr<BaseObject> ComponentFactoryImp::CreateObject(Type type)
	{
		switch (type)
		{
		case PFDComponent::IComponentFactory::BASE_OBJECT:
			return new BaseObject();
		default:
			return new BaseObject();
		}
	}



}