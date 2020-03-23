#include <iostream>

#include "../OSGCore/InterfaceOSGCore.h"

int main()
{
	OSGCore::InterfaceOSG* ptr = new OSGCore::InterfaceOSG();
	std::string strPath("D:\\glTF-Sample-Models\\2.0\\UnlitTest\\glTF\\UnlitTest.gltf");
	
	ptr->OpenSceneInNewWindow(strPath);
	delete ptr;
	return 0;
}