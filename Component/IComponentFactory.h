#pragma once
#include "PFDComponentExport.h"
#include "BaseObject.h"

namespace PFD
{
	namespace Component
	{
		class PFD_COMPONENT_API IComponentFactory
		{
		public:
			enum Type
			{
				BASE_OBJECT = 1000,
			};
			virtual ~IComponentFactory() {}
			static IComponentFactory* Get();

			virtual osg::ref_ptr<BaseObject> CreateObject(Type type) = 0;

		};
	}
}