#pragma once

#ifdef PFD_GLTF_HELPER_EXPORTS
#define PFD_GLTF_HELPER_API __declspec(dllexport)
#else
#define PFD_GLTF_HELPER_API __declspec(dllimport)
#endif