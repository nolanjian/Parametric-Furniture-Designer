#pragma once

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/State>
#include <osgUtil/Optimizer>

#include <string>


class TestOSGWin
{
public:
	void Run(const std::string& path);
	void configureShaders(osg::StateSet* stateSet);
};