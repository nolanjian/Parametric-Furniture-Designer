// Params.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "Params.h"


// This is an example of an exported variable
PARAMS_API int nParams=0;

// This is an example of an exported function.
PARAMS_API int fnParams(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CParams::CParams()
{
    return;
}
