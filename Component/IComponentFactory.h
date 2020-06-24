/*****************************************************************//**
 * \file   IComponentFactory.h
 * \brief  
 * 
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/06/24
 * 
 *********************************************************************/
#pragma once
#include <Component/Export.h>
#include <Component/BaseObject.h>

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