#pragma once
#ifdef PFDCORE_EXPORTS
#define PFDCORE_API __declspec(dllexport)
#else
#define PFDCORE_API __declspec(dllimport)
#endif