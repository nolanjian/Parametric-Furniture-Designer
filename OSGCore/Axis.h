#pragma once

#include "OSGIncluding.h"
#include "STLIncluding.h"

class Axis : osg::MatrixTransform
{
public:
	enum Dim {
		X,
		Y,
		Z,
	};

	Axis(Dim dim, unsigned int uLen = 1);
protected:
	void Init();
private:
	Dim m_dim;
	unsigned int m_uLen;
};