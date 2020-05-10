// Lofting.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "Lofting.h"


// This is an example of an exported variable
LOFTING_API int nLofting=0;

// This is an example of an exported function.
LOFTING_API int fnLofting(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CLofting::CLofting()
{
    return;
}
