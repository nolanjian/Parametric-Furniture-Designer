#pragma once

#include <osg/Array>

class BezierCurve
{
public:
	static osg::ref_ptr<osg::Vec3Array> Generate(osg::ref_ptr<osg::Vec3Array> points, double dStep = 0.05);
protected:
private:
};