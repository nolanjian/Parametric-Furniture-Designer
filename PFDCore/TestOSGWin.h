#pragma once

#include "OSGIncluding.h"
#include "STLIncluding.h"
#include <fx/gltf.h>
#include "stb_image.h"

namespace PFDCore
{


	class TestOSGWin
	{
	public:
		void Run(const std::string& path);
		void configureShaders(osg::StateSet* stateSet);
	};
}