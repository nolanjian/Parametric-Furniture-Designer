#include <iostream>

#include "../OSGCore/InterfaceOSGCore.h"

int main()
{
	OSGCore::InterfaceOSG* ptr = new OSGCore::InterfaceOSG();
	std::string strPath("D:\\glTF-Sample-Models-master\\2.0\\Box\\glTF-Embedded\\Box.gltf");
	
	ptr->OpenSceneInNewWindow(strPath);
	delete ptr;
	return 0;
}