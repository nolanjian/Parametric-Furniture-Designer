#include <iostream>

#include "../OSGCore/InterfaceOSGCore.h"

int main()
{
	OSGCore::InterfaceOSG* ptr = new OSGCore::InterfaceOSG();
	ptr->OpenSceneInNewWindow("D:\\Library\\glTF-Sample-Models\\2.0\\BoxTextured\\glTF-Embedded\\BoxTextured.gltf");
	delete ptr;
	return 0;
}