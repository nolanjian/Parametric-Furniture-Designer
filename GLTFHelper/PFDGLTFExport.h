#pragma once


#ifdef PFD_GLTF_EXPORTS
#define PFD_GLTF_API __declspec(dllexport)
#else
#define PFD_GLTF_API __declspec(dllimport)
#endif