// Utils.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "Utils.h"


// This is an example of an exported variable
UTILS_API int nUtils=0;

// This is an example of an exported function.
UTILS_API int fnUtils(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CUtils::CUtils()
{
    return;
}
