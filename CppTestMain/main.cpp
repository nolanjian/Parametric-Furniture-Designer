#include <iostream>

#include "../OSGCore/InterfaceOSGCore.h"

int main()
{
	while (1)
	{
		std::string strPath;
		std::getline(std::cin, strPath);

		OSGCore::InterfaceOSG* ptr = new OSGCore::InterfaceOSG();
		ptr->OpenSceneInNewWindow(strPath);
		delete ptr;
	}

	//OSGCore::InterfaceOSG* ptr = new OSGCore::InterfaceOSG();
	//std::string strPath("D:\\glTF-Sample-Models\\2.0\\UnlitTest\\glTF\\UnlitTest.gltf");
	
	return 0;
}