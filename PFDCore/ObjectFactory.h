#pragma once
#include "BaseObject.h"

namespace PFDCore
{
	class ObjectFactory
	{
	public:
		enum CLASS_TYPE
		{
			SCENE = 1,
			PANEL = 2,
			FRAME_GROUP = 3,
			HARD = 4,
			HANDLER = 5,
			MODEL = 6,
		};

		static osg::ref_ptr<BaseObject>	CreateObject(CLASS_TYPE type);
	};
}