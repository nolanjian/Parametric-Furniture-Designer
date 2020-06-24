#pragma once


#ifdef PFD_UTILS_EXPORTS
#define PFD_UTILS_API __declspec(dllexport)
#else
#define PFD_UTILS_API __declspec(dllimport)
#endif