#pragma once
#ifdef OSGCORE_EXPORTS
#define OSGCORE_API __declspec(dllexport)
#else
#define OSGCORE_API __declspec(dllimport)
#endif