#pragma once

#include "OSGIncluding.h"
#include "STLIncluding.h"

namespace PFDComponent
{


class Coordinate : public osg::MatrixTransform
{
public:
	Coordinate(unsigned int uLen = 1);
protected:
	void Init();
private:
	unsigned int m_uLen;
};
}