#pragma once


#ifdef PFD_CONFIG_EXPORTS
#define PFD_CONFIG_API __declspec(dllexport)
#else
#define PFD_CONFIG_API __declspec(dllimport)
#endif