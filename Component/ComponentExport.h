#pragma once


#ifdef PFD_COMPONENT_EXPORTS
#define PFD_COMPONENT_API __declspec(dllexport)
#else
#define PFD_COMPONENT_API __declspec(dllimport)
#endif