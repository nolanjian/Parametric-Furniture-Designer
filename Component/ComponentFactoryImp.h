#pragma once

#include "IComponentFactory.h"

namespace PFDComponent
{
	class ComponentFactoryImp : public IComponentFactory
	{
	public:
		virtual ~ComponentFactoryImp();

		virtual osg::ref_ptr<BaseObject> CreateObject(Type type) override;
	protected:
	private:
	};
}