﻿#pragma once

#include <Commom/OSGIncluding.h>
#include <Commom/STLIncluding.h>

namespace PFD
{
	namespace Component
	{
		class Axis : public osg::MatrixTransform
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
	}
}