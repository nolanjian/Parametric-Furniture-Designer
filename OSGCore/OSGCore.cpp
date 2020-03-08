// OSGCore.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "OSGCore.h"


// This is an example of an exported variable
OSGCORE_API int nOSGCore=0;

// This is an example of an exported function.
OSGCORE_API int fnOSGCore(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
COSGCore::COSGCore()
{
    return;
}
