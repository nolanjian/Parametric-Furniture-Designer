#include "pch.h"
#include "BezierCurve.h"

osg::ref_ptr<osg::Vec3Array> BezierCurve::Generate(osg::ref_ptr<osg::Vec3Array> points, double dStep)
{
	if (dStep < 0.01)
		dStep = 0.01;
	if (dStep > 0.99)
		dStep = 1.0;

	osg::ref_ptr<osg::Vec3Array> pArr = new osg::Vec3Array();

	std::vector<double>	vecT;
	std::vector<double>	vec1_T;

	std::map<size_t, double>	mapT;
	std::map<size_t, double>	map1_T;

	auto fnGetPoint = [&](double t)
	{
		osg::Vec3	point;
		size_t nSize = points->size();
		for (size_t ii = 1; ii <= nSize; ++ii)
		{
			size_t nT = ii - 1;
			size_t n1_T = nSize - ii;
			double dT = pow(t, nT);
			double d1_T = pow(1 - t, n1_T);
			osg::Vec3	cur = points->at(ii - 1);
			point += cur * dT * d1_T;
		}

		return point;
	};

	for (double t = 0.0; t <= 1.0; t += dStep)
	{
		pArr->push_back(fnGetPoint(t));

		if (t + dStep > 0.99)
		{
			t = 1.0;
			pArr->push_back(fnGetPoint(t));
		}
	}

	return pArr;
}
