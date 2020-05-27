#include "Manager.h"

namespace PFD
{
	namespace Scene
	{

		IManager* IManager::GetInstance()
		{
			static Manager s_Imp;
			return &s_Imp;
		}
	}



}