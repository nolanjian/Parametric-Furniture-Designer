#pragma once

#include <Commom/OSGIncluding.h>
#include <Commom/STLIncluding.h>

namespace PFD
{
	namespace Component
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
}