#include "IComponentFactory.h"
#include "ComponentFactoryImp.h"

namespace PFDComponent
{


	IComponentFactory* IComponentFactory::Get()
	{
		static ComponentFactoryImp s_Imp;
		return &s_Imp;
	}

}
