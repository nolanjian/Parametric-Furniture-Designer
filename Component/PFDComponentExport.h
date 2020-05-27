#pragma once


#ifdef PFD_Component_EXPORTS
#define PFD_COMPONENT_API __declspec(dllexport)
#else
#define PFD_COMPONENT_API __declspec(dllimport)
#endif