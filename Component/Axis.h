﻿/*****************************************************************//**
 * \file   Axis.h
 * \brief  
 * 
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/06/24
 * 
 *********************************************************************/
#pragma once

#include <osg/MatrixTransform>

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