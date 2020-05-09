// Manufacturing.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "Manufacturing.h"


// This is an example of an exported variable
MANUFACTURING_API int nManufacturing=0;

// This is an example of an exported function.
MANUFACTURING_API int fnManufacturing(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CManufacturing::CManufacturing()
{
    return;
}
