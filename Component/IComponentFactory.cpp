#include <Component/ComponentFactoryImp.h>

namespace PFD
{
	namespace Component
	{
		IComponentFactory* IComponentFactory::Get()
		{
			static ComponentFactoryImp s_Imp;
			return &s_Imp;
		}

	}
}
