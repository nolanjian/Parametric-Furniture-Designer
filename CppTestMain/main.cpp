#include <iostream>

#include "../OSGCore/InterfaceOSGCore.h"

int main()
{
#ifdef _DEBUG
	std::filesystem::current_path("D:\\Library\\Parametric-Furniture-Designer\\");
#endif // _DEBUG
	std::string strCur = std::filesystem::current_path().string();

	while (1)
	{
		std::string strPath;
		std::getline(std::cin, strPath);

		PFDCore::InterfaceOSG* ptr = new PFDCore::InterfaceOSG();
		ptr->OpenSceneInNewWindow(strPath);
		delete ptr;
	}

	//OSGCore::InterfaceOSG* ptr = new OSGCore::InterfaceOSG();
	//std::string strPath("D:\\glTF-Sample-Models\\2.0\\UnlitTest\\glTF\\UnlitTest.gltf");
	
	return 0;
}