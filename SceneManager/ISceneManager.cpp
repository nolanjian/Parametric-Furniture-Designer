#include "SceneManagerImp.h"

namespace PFDSceneManager
{
	ISceneManager* ISceneManager::GetInstance()
	{
		static SceneManagerImp s_Imp;
		return &s_Imp;
	}
}


