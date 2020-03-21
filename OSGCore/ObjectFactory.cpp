#include "pch.h"
#include "ObjectFactory.h"
#include "Scene.h"
#include "Panel.h"
#include "FrameGroup.h"
#include "Hard.h"
#include "Handler.h"
#include "Model.h"

osg::ref_ptr<BaseObject> ObjectFactory::CreateObject(CLASS_TYPE type)
{
	switch (type)
	{
	case ObjectFactory::SCENE:
		return new Scene();
    case ObjectFactory::PANEL:
        return new Panel();
    case ObjectFactory::FRAME_GROUP:
        return new FrameGroup();
    case ObjectFactory::HARD:
        return new Hard();
    case ObjectFactory::HANDLER:
        return new Handler();
    case ObjectFactory::MODEL:
        return new Model();
    default:
        return new BaseObject();
	}
}
