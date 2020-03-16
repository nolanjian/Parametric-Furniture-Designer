#pragma once
#ifdef PARAMS_EXPORTS
#define PARAMS_API __declspec(dllexport)
#else
#define PARAMS_API __declspec(dllimport)
#endif