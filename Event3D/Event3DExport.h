#pragma once

#ifdef PFD_EVENT3D_EXPORTS
#define PFD_EVENT3D_API __declspec(dllexport)
#else
#define PFD_EVENT3D_API __declspec(dllimport)
#endif