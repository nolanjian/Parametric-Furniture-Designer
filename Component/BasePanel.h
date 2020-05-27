#pragma once

#include "OSGIncluding.h"

#include "Parametric.h"

namespace PFD
{
	namespace Component
	{


		class PFD_COMPONENT_API BasePanel : public Parametric
		{
		public:
			BasePanel();
			virtual ~BasePanel();
			virtual bool InitFromParams() override;

			void SetDepth(double dVal);
			double GetDepth();
			void SetHeight(double dVal);
			double GetHeight();
			void SetWidth(double dVal);
			double GetWidth();

			virtual bool Update() override;
			//virtual bool UpdatePrimitive() override;

			osg::ref_ptr<osg::Array> GenerateVertexDataArray();
			osg::ref_ptr<osg::Array> GenerateVertexIndexArray();
			osg::ref_ptr<osg::Array> GenerateNormalDataArray();


		protected:
		private:
			double dDepth = 0.0f;
			double dHeight = 0.0f;
			double dWidth = 0.0f;
		};
	}
}