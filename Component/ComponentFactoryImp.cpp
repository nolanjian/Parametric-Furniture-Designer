#include "ComponentFactoryImp.h"

namespace PFD
{
	namespace Component
	{

		ComponentFactoryImp::~ComponentFactoryImp()
		{
		}

		osg::ref_ptr<BaseObject> ComponentFactoryImp::CreateObject(Type type)
		{
			switch (type)
			{
			case IComponentFactory::BASE_OBJECT:
				return new BaseObject();
			default:
				return new BaseObject();
			}
		}



	}
}