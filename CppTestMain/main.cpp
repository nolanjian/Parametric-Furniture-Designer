#include <iostream>

#include "../OSGCore/InterfaceOSGCore.h"

int main()
{
	OSGCore::InterfaceOSG* ptr = new OSGCore::InterfaceOSG();
	ptr->LoadScene("D:\\Box.gltf");

	return 0;
}