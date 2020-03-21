#include <iostream>

#include "../OSGCore/InterfaceOSGCore.h"

int main()
{
	OSGCore::InterfaceOSG* ptr = new OSGCore::InterfaceOSG();
	ptr->OpenSceneInNewWindow("D:\\Box.gltf");
	delete ptr;
	return 0;
}