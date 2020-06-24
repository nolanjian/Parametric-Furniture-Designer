/*****************************************************************//**
 * \file   Coordinate.h
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