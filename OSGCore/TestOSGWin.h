#pragma once

#include "OSGIncluding.h"
#include "STLIncluding.h"
#include <fx/gltf.h>
#include "../easyloggingpp/easylogging++.h"
#include "stb_image.h"


class TestOSGWin
{
public:
	void Run(const std::string& path);
	void configureShaders(osg::StateSet* stateSet);
};