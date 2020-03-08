// muparserx.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "muparserx.h"


// This is an example of an exported variable
MUPARSERX_API int nmuparserx=0;

// This is an example of an exported function.
MUPARSERX_API int fnmuparserx(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
Cmuparserx::Cmuparserx()
{
    return;
}
