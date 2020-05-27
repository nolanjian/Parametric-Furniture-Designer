#pragma once

#ifdef PFD_SCENEMANAGER_EXPORTS
#define PFD_SCENEMANAGER_API __declspec(dllexport)
#else
#define PFD_SCENEMANAGER_API __declspec(dllimport)
#endif